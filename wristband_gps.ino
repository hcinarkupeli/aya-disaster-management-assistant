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

bool konum_alindi = false;
bool nabiz_setup_tamam = false;

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino - GPS module");
  gpsSerial.begin(9600);
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
  if (konum_alindi)
    loop_nabiz();
  else
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
