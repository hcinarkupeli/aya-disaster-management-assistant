#include <VirtualWire.h>  
#include <DFRobot_MLX90614.h>

DFRobot_MLX90614_I2C sensor;
const int rxPin = 6;
const int PulseWire = A0;
const int LED13 = 13;
int Threshold = 550;
int motorpin = 12;
long zaman = 0;

void setup_sicaklik() {
  while (NO_ERR != sensor.begin()) {
    Serial.println("Sensör bağlantı hatası!");
    delay(3000);
  }
  Serial.println("Sensör bağlantısı başarılı!");
}

float sicaklik_oku() {
  float objectTemp = sensor.getObjectTempCelsius();
  Serial.print("Sıcaklık: ");
  Serial.print(objectTemp);
  Serial.println(" °C");
  return objectTemp;
}

void setup() {
  Serial.begin(9600);  
  Serial.println("433 MHz Alıcı Başlatılıyor...");

  vw_set_rx_pin(rxPin);  
  vw_setup(2000);        
  vw_rx_start();         
  Serial.println("Başladı");
  pinMode(motorpin, OUTPUT);
  digitalWrite(motorpin, 0);

  setup_sicaklik();

  zaman = millis();
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
  } else if (okuma < Threshold) {
    digitalWrite(LED13, LOW);
    atimAlgilandi = false;
  }
  return nabiz;
}

void loop() {
  int nabiz = nabiz_oku();
  if (millis() - zaman > 500) {
    zaman = millis();
    float sicaklik = sicaklik_oku();
    Serial.print("Sıcaklık: ");
    Serial.println(sicaklik);

    Serial.print("Nabız: ");
    Serial.println(nabiz);
  }

  uint8_t buf[VW_MAX_MESSAGE_LEN];  
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) {  
    Serial.print("Gelen mesaj: ");
    digitalWrite(motorpin, 1);
    for (int i = 0; i < buflen; i++) {
      Serial.write(buf[i]);  
    }
    Serial.println();  
  }
}
