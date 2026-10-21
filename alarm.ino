#include "secrets.h"
#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif

#include <Firebase_ESP_Client.h>

#include <addons/TokenHelper.h>

#include <addons/RTDBHelper.h>

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

#include <RH_ASK.h>  
#include <SPI.h>     

RH_ASK driver(2000, -1, D3);  

char *rf_mesaji;
bool b = true;

#include <DFRobot_BMI160.h>       
#include <SoftwareSerial.h>       
#include <DFRobotDFPlayerMini.h>  

DFRobot_BMI160 bmi160;
const int8_t i2c_addr = 0x69;

SoftwareSerial mySoftwareSerial(D5, D6);  
DFRobotDFPlayerMini myDFPlayer;

bool deprem_oldu = false;

void setup_firebase() {

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  multi.addAP(WIFI_SSID, WIFI_PASSWORD);
  multi.run();
#else
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

  Serial.print("Connecting to Wi-Fi");
  unsigned long ms = millis();
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    if (millis() - ms > 10000)
      break;
#endif
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

  Firebase.setDoubleDigits(5);
  config.timeout.serverResponse = 10 * 1000;
}

void deprem_kaydet() {
  if (deprem_oldu) {
    rf_mesaji = "1";
  } else {
    rf_mesaji = "0";
  }

  if (Firebase.ready()) {
    int s = Firebase.RTDB.setBool(&fbdo, F("/veriler/DepremDurumu"), deprem_oldu);
  }
}

void rf_ile_gonder() {
  driver.send((uint8_t *)rf_mesaji, strlen(rf_mesaji));
  driver.waitPacketSent();  
  Serial.println("rf gönderildi");
}

void setup() {
  rf_mesaji = "0";

  Serial.begin(115200);

  if (!driver.init()) {
    Serial.println("RF modülü başlatılamadı!");
  } else {
    Serial.println("RF Verici Başladı");
  }

  setup_firebase();
  mySoftwareSerial.begin(9600);

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer modülü başlatılamadı. Lütfen bağlantıları kontrol edin.");
    while (true)
      ;
  }
  myDFPlayer.volume(20);  

  if (bmi160.softReset() != BMI160_OK) {
    Serial.println("Sensör sıfırlama hatası");
    while (1)
      ;
  }

  if (bmi160.I2cInit(i2c_addr) != BMI160_OK) {
    Serial.println("Sensör başlatma hatası");
    while (1)
      ;
  }

  Serial.println("Sistem hazır.");
}

void bekle(int t) {
  long z = millis();
  while (millis() - z < t) {
      rf_ile_gonder();
      delay(100);
  }
}

void loop() {
  int16_t accelGyro[6] = { 0 };

  if (bmi160.getAccelGyroData(accelGyro) == 0) {
    float ax = accelGyro[3] / 16384.0;  
    float ay = accelGyro[4] / 16384.0;  
    float az = accelGyro[5] / 16384.0;  

    if (abs(ax) > 1.5 || abs(ay) > 1.5 || abs(az) > 1.5) {
      Serial.println("Deprem algılandı! Ses çalınıyor...");
      deprem_oldu = true;
      deprem_kaydet();
      myDFPlayer.play(1);  
      bekle(27000);        
    } else if (deprem_oldu) {
      deprem_oldu = false;
      deprem_kaydet();
      myDFPlayer.play(2);  
      bekle(12000);
    }
  } else {
    Serial.println("Sensör okuma hatası");
  }

  delay(100);  
}
