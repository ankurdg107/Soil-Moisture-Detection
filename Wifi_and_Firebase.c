//change WIFI_SSID and WIFI_PASSWORD

#if defined(ESP32)
#include <WiFi.h>
#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>
#endif

//Provide the token generation process info.
#include <addons/TokenHelper.h>

//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Realme"
#define WIFI_PASSWORD "77777777"

//For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyCeCrg7IO-BaFt4D9wbfcb3nIQzyGmH1HI"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://agrotech-d6e76-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

#define DHTPIN 4 //Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);


int msensor = A0; // moisture sensor is connected with the analog pin A0 of ESP8266
int msvalue = 0; // moisture sensor value 
int setmoisture;
int mspercent; // moisture value in percentage
int Relay = D0; // you can connect a dc or ac water pump

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;


//String main="";

int a, b, x, y;


void setup()
{

  Serial.begin(115200);
  delay(2000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;



  //////////////////////////////////////////////////////////////////////////////////////////////
  //Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  //otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(DATABASE_URL, API_KEY);

  //Comment or pass false value when WiFi reconnection will control by your code or third party library
 // Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

}

void loop()
{
 
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    // return;
  }

  //soilmoisture sensor data
  msvalue = analogRead(msensor);
  //mspercent=15;
  mspercent = map(msvalue,0,1023,100,0); // To display the soil moisture value in percentag
    Serial.print("Soil Moisture : ");
    Serial.println(mspercent);

    //x=random(0,9);
    //y=random(10,19);
    
  if (Firebase.ready()) 
  {
    
    //Firebase.setInt(fbdo, main, 5);
    Serial.print(mspercent);
    Firebase.setFloat(fbdo, "/Readings/Humidity", h);
    Firebase.setFloat(fbdo, "/Readings/Soil Moisture", mspercent);
    Firebase.setFloat(fbdo, "/Readings/Temperature", t);
    delay(200);

    Serial.printf("Get int a--  %s\n", Firebase.getInt(fbdo, "/test/a") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     a=fbdo.to<int>();
    Serial.printf("Get int b--  %s\n", Firebase.getInt(fbdo, "/test/b") ? String(fbdo.to<int>()).c_str() : fbdo.errorReason().c_str());
     b=fbdo.to<int>();

  Serial.println();  
  Serial.print("a:");
  Serial.print(a);
  Serial.print("  b: ");
  Serial.print(b);
  
  Serial.println();
  Serial.println("------------------");
  Serial.println();
  

  delay(2500);
  }
}
