#include <DallasTemperature.h>
#include <OneWire.h>


#include <ESP8266WiFi.h>
 
String apiKey = "";     //  API key from ThingSpeak

const char *ssid =  "Enchilada";     //  wifi ssid and wpa2 key
const char *pass =  "123456780";
const char* server = "api.thingspeak.com";


 


WiFiClient client;
const int buzzer =  4;
#define ONE_WIRE_BUS 5 // D1 pin for temperature sensor
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
int distance;
long dist;
float temp;
float level;
float liter;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
void setup() 
{
pinMode(buzzer, OUTPUT);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication
sensors.begin();
       Serial.begin(115200);
       delay(10);
      
 
       Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 
}
 
void loop() 
{
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
level = 80 - distance;
liter = level/100;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
Serial.print("cm");
sensors.requestTemperatures(); 
Serial.println("Temperature is: ");
temp = sensors.getTempCByIndex(0);
Serial.println(temp);
Serial.println("Quantity is : ");
Serial.println(liter);
Serial.println("Liters");
if (liter > 0.75)
{
  digitalWrite(buzzer, HIGH);
  delay(2000);
  }
  else{
  digitalWrite(buzzer, LOW);
  }
  


                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(temp);//level
                             postStr +="&field2=";
                             postStr += String(level);//level
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.print("Temperature: ");
                             Serial.print(temp);
                             Serial.print(" MilkPot: ");
                             Serial.print(distance);
                             Serial.println("%. Send to Thingspeak.");
                        }
          client.stop();
 
          Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates, i've set it to 30 seconds
  delay(30000);
}
