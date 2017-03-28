// DS1307 RTC ENTRIES ++++++++++++++++
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// DHT11 Temperature and Humidity Sensors
#include "DHT.h"         //include DHT library
#define DHTPIN 13         //define as DHTPIN the Pin 10 used to connect the Sensor
#define DHTTYPE DHT11    //define the sensor used(DHT11)
#define REDPIN 5
#define GREENPIN 6
#define BLUEPIN 3


unsigned long previousMillis = 0;  //last time it was changed
unsigned long interval = 3000;  //time delay between color changes to debounce temp variation 5min = 300,000
//problem currenty is to turn it on to start but not delay too much for the first color change.



DHT dht(DHTPIN, DHTTYPE);//create an instance of DHT
/*setup*/
void setup() {
  Serial.begin(9600);    //initialize the Serial communication
  //  Critical setupline for establishing correct serial comms
  while (!Serial); // for Leonardo/Micro/Zero
  Serial.begin(9600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }



  pinMode(A2, OUTPUT);      //Setting the A2-A4 pins to output
  digitalWrite(A2, LOW);    //digital mode to power the RTC
  pinMode(A3, OUTPUT);      //without wires!
  digitalWrite(A3, HIGH);
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);


  delay(1000);           //wait 1 seconds
  Serial.println("Temperature and Humidity test!");//print on Serial monitor
  Serial.println("T(F) \tH(%)");                   //print on Serial monitor
  dht.begin();           //initialize the Serial communication
  //Turn the lights on so I know it's working
  analogWrite(REDPIN, 255);
  analogWrite(BLUEPIN, 255);
  analogWrite(GREENPIN, 255);

}
/*loop*/
void loop() {
  DateTime now = rtc.now();
  float h = dht.readHumidity();    // reading Humidity
  float t = dht.readTemperature(); // read Temperature as Celsius (the default)
  // check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  t = (32 + (9 * t) / 5);
  float farenheight = t;

  Serial.print(t, 2);    //print the temperature
  Serial.print("\t");
  Serial.println(h, 2);  //print the humidity
  delay(2000);           //wait 2 seconds
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();


  if ((now.hour() >= 18 && now.hour() < 22) || (now.hour() >= 6 && now.hour() < 8)) {

    Serial.println("turn on the lights");
    //Turn the lights on so I know it's working
    analogWrite(REDPIN, 255);
    analogWrite(BLUEPIN, 255);
    analogWrite(GREENPIN, 255);
    //Winter Version
    if (farenheight > 85) {
      analogWrite(REDPIN, 255);
      analogWrite(BLUEPIN, 0);
      analogWrite(GREENPIN, 0);
    }
    else if (farenheight <= 85 && farenheight >= 80) {
      analogWrite(GREENPIN, 255);
      analogWrite(BLUEPIN, 0);
      analogWrite(REDPIN, 255);
    }
    else if (farenheight <= 80 && farenheight >= 70) {
      analogWrite(GREENPIN, 150);
      analogWrite(BLUEPIN, 150);
      analogWrite(REDPIN, 0);
    }
    else if (farenheight <= 70 && farenheight >= 60) {
      analogWrite(GREENPIN, 255);
      analogWrite(BLUEPIN, 0);
      analogWrite(REDPIN, 0);
    }
    else if (farenheight <= 60 && farenheight >= 50) {
      analogWrite(GREENPIN, 0);
      analogWrite(BLUEPIN, 150);
      analogWrite(REDPIN, 150);
    }
    else {
      analogWrite(BLUEPIN, 255);
      analogWrite(REDPIN, 255);
      analogWrite(GREENPIN, 255);
    }
  }
  else {
    analogWrite(REDPIN, 0);
    analogWrite(BLUEPIN, 0);
    analogWrite(GREENPIN, 0);
  }
}

