#include "secrets.h"
#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#endif

#include <Firebase_ESP_Client.h>

#include <addons/TokenHelper.h>

#include <addons/RTDBHelper.h>

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int KisiSayisi = 0;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

#define trigPin1 D5
#define echoPin1 D6

#define trigPin2 D2
#define echoPin2 D3

bool yazdir = true;
bool bekle = false;
long sure;
int mesafe;

int girenkisisayisi = 0;
int cikankisisayisi = 0;

int esik_degeri = 30;

void setup_firebase() {

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;

  config.token_status_callback = tokenStatusCallback;  

  config.signer.tokens.legacy_token = FIREBASE_LEGACY_TOKEN;

  Firebase.reconnectNetwork(true);

  fbdo.setBSSLBufferSize(4096 , 1024 );

  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  config.timeout.serverResponse = 10 * 1000;
}

void kisi_sayisi_kaydet() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    int ksayisi = girenkisisayisi - cikankisisayisi;
    Firebase.RTDB.setInt(&fbdo, F("/veriler/KisiSayisi"), ksayisi);
  }
}

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  Serial.println("Arduino İle Mesafe Sensörü Uygulaması Başlatılıyor...");
  delay(1000);
  setup_firebase();
}
bool kisiVarmi1() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  sure = pulseIn(echoPin1, HIGH);
  mesafe = (sure / 29.1) / 2;
  if (yazdir) {
  }
  if (mesafe < esik_degeri)
    return true;
  return false;
}
bool kisiVarmi2() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(3);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  sure = pulseIn(echoPin2, HIGH);
  mesafe = (sure / 29.1) / 2;
  if (yazdir) {
  }
  if (mesafe < esik_degeri)
    return true;
  return false;
}

void verileri_yazdir() {
  Serial.print("Giren: ");
  Serial.print(girenkisisayisi);
  Serial.print(" - Çıkan: ");
  Serial.print(cikankisisayisi);
  Serial.print(" - İçerdeki Kişi Sayısı: ");
  Serial.println(girenkisisayisi - cikankisisayisi);
  if (bekle) {
    delay(2000);
    bekle = false;
  }
}

void loop() {

  if (kisiVarmi1()) {
    Serial.println("Kişi 2 bekleniyor");
    while (!kisiVarmi2()) {}
    while (kisiVarmi2()) {}
    girenkisisayisi++;
    kisi_sayisi_kaydet();
    bekle = true;
    verileri_yazdir();
  }

  if (kisiVarmi2()) {
    Serial.println("Kişi 1 bekleniyor");
    while (!kisiVarmi1()) {}
    while (kisiVarmi1()) {}
    cikankisisayisi++;
    kisi_sayisi_kaydet();
    bekle = true;
    verileri_yazdir();
  }

  verileri_yazdir();
}
