#include <Arduino.h>
#include <Konker.h>

Konker device;

void setup() {

  Serial.begin(115200);

  //if true show the library log
  debug_log = true;

  WiFi.begin("your SSID","your password");

  long tempo_reconexao = millis();

  while(WiFi.status() != WL_CONNECTED){
    
    Serial.println(".");
    delay(1000);

  }

  Serial.println("connected");

  //Function to connect your account konker
  device.connect_account("your email account konker","your password account");
  
  //Function to create device on the platform and establish connection
  device.create("define your device id","define your device name");

}

void loop() {
  
  int value = random(0,10);

  //Function to get json data on device channel
  device.send_data("define your channel","{\"Text\":\"test\",\"paramers\":{\"value random\":"+String(value)+"}}");
  
  //Function to get json data from device channel
  Serial.println(device.get_data("define your channel",1));

  delay(500);

}