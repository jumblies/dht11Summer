// DS1307 RTC ENTRIES ++++++++++++++++
// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"
#include <TimeLord.h>
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

// what is our longitude (west values negative) and latitude (south values negative)
// Greensboro, NC  Latitude: 36.145833 | Longitude: -79.801728
float const LONGITUDE = -79.80;
float const LATITUDE = 36.14;

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

  TimeLord tardis;
  tardis.TimeZone(-4 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
  // as long as the RTC never changes back and forth between DST and non-DST
  tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are
  //  byte today[] = {  0, 0, 12, 29, 03, 2017    }; // store today's date (at noon) in an array for TimeLord to use


  DateTime now = rtc.now();

  byte today[] = {now.second(), now.minute(), now.hour(), now.day(), now.month(), now.year() };
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();




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

  TimeLord tardis;
  tardis.TimeZone(-4 * 60); // tell TimeLord what timezone your RTC is synchronized to. You can ignore DST
  // as long as the RTC never changes back and forth between DST and non-DST
  tardis.Position(LATITUDE, LONGITUDE); // tell TimeLord where in the world we are
  //  byte today[] = {  0, 0, 12, 29, 03, 2017    }; // store today's date (at noon) in an array for TimeLord to use


  DateTime now = rtc.now();


  byte today[] = {now.second(), now.minute(), now.hour(), now.day(), now.month(), now.year() };

  //TimeLord Library Print times at init
  if (tardis.SunRise(today)) // if the sun will rise today (it might not, in the [ant]arctic)
  {
    Serial.print("Sunrise: ");
    Serial.print((int) today[tl_hour]);
    Serial.print(":");
    Serial.print((int) today[tl_minute]);
    Serial.print(" ");
  }
  int sunriseHour = ((int) today[tl_hour]);
  int sunriseMinute = ((int) today[tl_minute]);
  if (tardis.SunSet(today)) // if the sun will set today (it might not, in the [ant]arctic)
  {
    Serial.print("Sunset: ");
    Serial.print((int) today[tl_hour]);
    Serial.print(":");
    Serial.println((int) today[tl_minute]);
  }
  int sunsetHour = (int) today[tl_hour];
  int sunsetMinute = (int) today[tl_minute];

  float h = dht.readHumidity();    // reading Humidity
  float t = dht.readTemperature(); // read Temperature as Celsius (the default)
  // check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  t = (32 + (9 * t) / 5);
  float farenheight = t;
  Serial.print("Current conditions (Temp/Hum): ");    //print current environmental conditions
  Serial.print(t, 2);    //print the temperature
  Serial.print("\t");
  Serial.println(h, 2);  //print the humidity
  delay(2000);           //wait 2 seconds
  Serial.print("Human time = ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.println(now.minute(), DEC);
  Serial.print("Unixtime = ");
  Serial.print(now.unixtime());
  Serial.println("s");
  Serial.print("Morning Lights from: ");
  Serial.print(sunriseHour);
  Serial.print(" until ");
  Serial.println(sunriseHour + 2);
  Serial.print("Evening Lights from: ");
  Serial.print(sunsetHour);
  Serial.print(" until ");
  Serial.println(sunsetHour + 2);
  if ((now.hour() >= sunsetHour && now.hour() <= (sunsetHour + 2)) || (now.hour() >= sunriseHour && now.hour() <= (sunriseHour + 2))) {

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
  else {                                //Shut the lights off if the above conditions are not met
    analogWrite(REDPIN, 0);             //Meaning that it's not time for them to be on.
    analogWrite(BLUEPIN, 0);
    analogWrite(GREENPIN, 0);
  }
}

