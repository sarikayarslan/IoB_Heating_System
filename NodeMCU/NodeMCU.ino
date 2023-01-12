#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <DHT.h>
#include <DHT_U.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "secret.h"

#define WIFI_SSID "Arslan"
#define WIFI_PASSWORD "esp826612e"


#define DHTTYPE DHT11
#define ROLE D8

const int DHTPin =D4;
DHT dht (DHTPin,DHTTYPE);


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);



  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
   pinMode(ROLE,OUTPUT);
  dht.begin();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  int occupancy =0;
  
  if (Firebase.ready() && signupOK ){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    float t = dht.readTemperature();
    Serial.println(t);
    float h = dht.readHumidity();
    Serial.println(h);
    
    if (Firebase.RTDB.setFloat(&fbdo, "Temperature", t)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    if (Firebase.RTDB.setFloat(&fbdo, "Humadity", h)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

  if (Firebase.RTDB.getInt(&fbdo, "Occupancy")) {
      if (fbdo.dataType() == "int") {
        occupancy = fbdo.intData();
        Serial.println(occupancy);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

    if(occupancy ==1){
      digitalWrite(ROLE,HIGH);
  }
  else{
      digitalWrite(ROLE,LOW);
  }

    
  }
}


