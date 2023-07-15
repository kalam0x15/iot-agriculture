#define BLYNK_PRINT Serial
 
 
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "8Ahw4QTXBtmxJXwMeVri2qfn2LgIuHGG";  //authentication key from Blynk app
 
char ssid[] = "Devil";    //Wi-fi name
char pass[] = "devil015";  //Wi-fi password
 
#define DHTPIN 0          // D3
int sensorpin=A0;
int sensorvalue=0;
int outputvalue=0;
int relaypin = D4;
int value=0;


#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301
 
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
 
// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit
 
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V6, h); //setting the humidity value in V6(Blynk app)
  Blynk.virtualWrite(V4,t);  //setting the humidity value in V6(Blynk app)
}
 
void setup()
{
  // Debug console
  Serial.begin(9600);
 
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);
  pinMode(relaypin, OUTPUT);
 
  dht.begin();
 
 
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

BLYNK_WRITE(V1){
  value = param.asInt();
  if (value == 1){
    digitalWrite(relaypin, HIGH);
  }else{
    digitalWrite(relaypin, LOW);
  }
}

 
void loop()
{
    outputvalue=analogRead(sensorpin);
    //0,1023,0,100
outputvalue = map(outputvalue, 1023, 0, 0, 100);
  delay(1000);

  if(outputvalue>74)
  {
       Serial.println("water your plant");
       Serial.print(outputvalue);
       //Blynk.notify("water your plant");
       delay(1000); 
  }
  else if(outputvalue<45)
  {
       Serial.println("soil is wet enough to water");
       Serial.print(outputvalue);
       //Blynk.notify("soil is wet enough to water");
       delay(1000);
  }
  Blynk.run();
  Blynk.virtualWrite(V7,outputvalue);
 

  timer.run();
}
