#define BLYNK_TEMPLATE_ID "TMPL3ifbcDimH"
#define BLYNK_TEMPLATE_NAME "Weather Monitoring System"
#define BLYNK_AUTH_TOKEN "6i-4fXB5LBZ1jlR6piaaGdQ5gchMPq-G"


#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <SFE_BMP180.h>


// Create an object for the BMP180 sensor
SFE_BMP180 bmp;

char auth[] = BLYNK_AUTH_TOKEN;//Enter your Auth token
char ssid[] = "Ahmed Omar";//Enter your WIFI name
char pass[] = "12345678";//Enter your WIFI password

DHT dht(D3, DHT11);//(DHT sensor pin,sensor type)
BlynkTimer timer;

//Define Rain and LDR pins
#define rain A0
#define light D0

//Create three variables for pressure
double T, P;
char status;

void setup() {
  Serial.begin(9600);
  bmp.begin();
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();

  //Call the functions
  timer.setInterval(100L, DHT11sensor);
  timer.setInterval(100L, rainSensor);
  timer.setInterval(100L, pressure);
  timer.setInterval(100L, LDRsensor);
}

//Get the DHT11 sensor values
void DHT11sensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

 }

//Get the rain sensor values
void rainSensor() {
  int value = analogRead(rain);
  Blynk.virtualWrite(V2, 100-(value/10.24));
}

//Get the pressure values
void pressure() {
  status =  bmp.startTemperature();
  if (status != 0) {
    delay(status);
    status = bmp.getTemperature(T);

    status = bmp.startPressure(3);// 0 to 3
    if (status != 0) {
      delay(status);
      status = bmp.getPressure(P, T);
      if (status != 0) {

      }
    }
  }

  Blynk.virtualWrite(V3, P);
}

//Get the LDR sensor values
void LDRsensor() {
  bool value = digitalRead(light);
  if (value == 0) {
    WidgetLED LED(V4);
    LED.on();
  } else {
    WidgetLED LED(V4);
    LED.off();
  }

}

void loop() {
  Blynk.run();//Run the Blynk library
  timer.run();//Run the Blynk timer
}