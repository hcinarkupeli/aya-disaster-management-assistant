#include <DFRobot_MLX90614.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <RH_ASK.h>
#include <SPI.h>

DFRobot_MLX90614_I2C sensor;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(3, 4);  

RH_ASK rf_driver;

const int motorPin = 12;
const int PulseWire = A0;
const int LED13 = 13;
int Threshold = 550;
bool konum_alindi = false;

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);

  pinMode(LED13, OUTPUT);

  setup_sicaklik();

  if (!rf_driver.init()) {
    Serial.println("RF başlatılamadı.");
  } else {
    Serial.println("RF hazır.");
  }
}

void setup_sicaklik() {
  while (NO_ERR != sensor.begin()) {
    Serial.println("Sensör bağlantı hatası!");
    delay(3000);
  }
  Serial.println("Sensör bağlantısı başarılı!");
}

void loop() {
  uint8_t buf[1];
  uint8_t buflen = sizeof(buf);

  if (rf_driver.recv(buf, &buflen)) {
    if (buf[0] == '1') {
      digitalWrite(motorPin, HIGH);
      delay(1000);
      digitalWrite(motorPin, LOW);
    }
  }

  int nabiz = nabiz_oku();
  float sicaklik = sicaklik_oku();
  loop_gps();

  verileri_gonder(nabiz, sicaklik);

  delay(500);
}

float sicaklik_oku() {
  float objectTemp = sensor.getObjectTempCelsius();
  Serial.print("Sıcaklık: ");
  Serial.print(objectTemp);
  Serial.println(" °C");
  return objectTemp;
}

int nabiz_oku() {
  int okuma = analogRead(PulseWire);
  static unsigned long sonAtimZamani = 0;
  static int nabiz = 0;
  static bool atimAlgilandi = false;
  
  if (okuma > Threshold && !atimAlgilandi) {
    digitalWrite(LED13, HIGH);
    unsigned long simdi = millis();
    nabiz = 60000 / (simdi - sonAtimZamani);
    sonAtimZamani = simdi;
    atimAlgilandi = true;
    Serial.print("Nabız: ");
    Serial.println(nabiz);
  }
  else if (okuma < Threshold) {
    digitalWrite(LED13, LOW);
    atimAlgilandi = false;
  }
  return nabiz;
}

void loop_gps() {
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }
}

void verileri_gonder(int nabiz, float sicaklik) {
  double latitude = gps.location.lat();
  double longitude = gps.location.lng();

  char veri[100];
  snprintf(veri, sizeof(veri), "Sicaklik:%.2f,Nabiz:%d,Lat:%.6f,Lon:%.6f", sicaklik, nabiz, latitude, longitude);

  rf_driver.send((uint8_t *)veri, strlen(veri));
  rf_driver.waitPacketSent();

  Serial.println("Veriler RF ile gönderildi.");
}
