#include <DFRobot_MLX90614.h>

DFRobot_MLX90614_I2C sensor; 

#define USE_ARDUINO_INTERRUPTS true  
#include <PulseSensorPlayground.h>   

const int PulseWire = A0;  
const int LED13 = 13;      
int Threshold = 550;       

PulseSensorPlayground pulseSensor;  
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define RX_PIN 3  
#define TX_PIN 4  

TinyGPSPlus gps;                           
SoftwareSerial gpsSerial(RX_PIN, TX_PIN);  

bool konum_alindi = true;
bool nabiz_setup_tamam = false;

void setup_sicaklik() {

  while (NO_ERR != sensor.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  sensor.enterSleepMode();
  delay(50);
  sensor.enterSleepMode(false);
  delay(200);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino - GPS module");
  gpsSerial.begin(9600);

  setup_sicaklik();
}

void sicaklik_oku() {
  float objectTemp = sensor.getObjectTempCelsius();
  Serial.print("Object celsius : ");
  Serial.print(objectTemp);
  Serial.println(" °C");

  Serial.println();
  delay(500);
}

void setup_nabiz() {
  nabiz_setup_tamam = true;
  pulseSensor.analogInput(PulseWire);
  pulseSensor.blinkOnPulse(LED13);      
  pulseSensor.setThreshold(Threshold);  

  if (pulseSensor.begin()) {
    
    Serial.println("Sensör basariyla çalistirildi.");
  }
}

void loop() {
  if (konum_alindi) {
    loop_nabiz();
    sicaklik_oku();
  } else
    loop_gps();
}

void loop_nabiz() {
  if (!nabiz_setup_tamam) {
    setup_nabiz();
  }
  
  int nabiz = pulseSensor.getBeatsPerMinute();

  if (pulseSensor.sawStartOfBeat()) {
    Serial.println("Nabiz tespit edildi! ");

    Serial.print("NABIZ: ");  
    Serial.println(nabiz);    
  }
  delay(20);  
}

void loop_gps() {

  if (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isValid()) {
        double latitude = gps.location.lat();
        double longitude = gps.location.lng();

        Serial.print(F("- latitude: "));
        Serial.println(latitude);

        Serial.print(F("- longitude: "));
        Serial.println(longitude);
        konum_alindi = true;
      } else {
        Serial.println(F("- location: INVALID"));
      }

      Serial.println();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}
