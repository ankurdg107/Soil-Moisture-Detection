#define BLYNK_TEMPLATE_ID "TMPLpDlOfwmL"
#define BLYNK_DEVICE_NAME "Smart Irrigation System"
#define BLYNK_AUTH_TOKEN "Tsq2hLmrdxLMaG5mSOvIThY_eljLmRaa"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp8266.h>

#include <DHT.h>



char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Realme";  // type your wifi name
char pass[] = "77777777";  // type your wifi password

BlynkTimer timer;


#define DHTPIN 4 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);


int msensor = A0; // moisture sensor is connected with the analog pin A0 of ESP8266
int msvalue = 0; // moisture sensor value 
int setmoisture;
int mspercent; // moisture value in percentage
int Relay = D0; // you can connect a dc or ac water pump


//auto and manual
int ButtonPressed;
int ButtonState;
const String BLYNK_RED =   "#FF0000";
const String BLYNK_GREEN =  "#00FF00";

BLYNK_WRITE(V1) //button to be disable
{
  ButtonPressed = param.asInt();
 if (ButtonPressed == 1) {
   digitalWrite(D0, LOW);
   Blynk.setProperty(V1, "color", BLYNK_GREEN);
 } else {
   digitalWrite(D0, HIGH);
   Blynk.setProperty(V1, "color", BLYNK_RED);
 }
}

BLYNK_WRITE(V4) //button to disable V1
{
 int ButtonState = param.asInt(); // assigning incoming value from pin V4 to ButtonState
 Serial.println(ButtonState);
 //0-manual and 1 - auto
 //when manual mode is on
 if (ButtonState == 0) {
  
   Blynk.virtualWrite(V1, LOW);
  // ButtonPressed = 1;
  // Blynk.setProperty(V1, "color", BLYNK_YELLOW);
  Blynk. setProperty(V1, "isDisabled", "false");
   Blynk.setProperty(V1, "onLabel", "ON");
   Blynk.setProperty(V1, "offLabel", "OFF");
   Serial.println("Button V1 enable");
   Serial.println("Button MANUAL enable");
 } 
 //when auto mode is on 
 else { 
   //ButtonPressed = 0;
   Blynk.setProperty(V1, "color", "#000000");
   Blynk. setProperty(V1, "isDisabled", "true");
   Blynk.virtualWrite(V1, LOW);
   Blynk.setProperty(V1, "onLabel", "disable !");
   Blynk.setProperty(V1, "offLabel", "disable");
   Serial.println("Button V1 disable");
    Serial.println("Button AUTO enable");
 }
 if(mspercent <40 && ButtonState == 1){
    digitalWrite(D0,LOW);
  }
  else{
   digitalWrite(D0,HIGH);
  }

}

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    // return;
  }

  //soilmoisture sensor data
  msvalue = analogRead(msensor);
  mspercent = map(msvalue,0,1023,100,0); // To display the soil moisture value in percentag
    Serial.print("Soil Moisture : ");
    Serial.println(mspercent);

    Blynk.virtualWrite(V0,mspercent);
    Blynk.virtualWrite(V2, t);
    Blynk.virtualWrite(V3, h);

    //Serial.println();
    Blynk.syncVirtual(V1);
   
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
}
void setup()
{   
  
  Serial.begin(115200);
  pinMode(D0,OUTPUT);
  digitalWrite(D0, HIGH);
  //for buitin LED
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, HIGH);
  

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(msensor, INPUT);
  timer.setInterval(100L, sendSensor);
 
  }

void loop()
{

  Blynk.run();
  //Serial.println(ButtonState);
  //Serial.println(mspercent);
 
  timer.run();
 }
