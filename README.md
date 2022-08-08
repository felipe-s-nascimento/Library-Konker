
# Library Konker for ESP32

This library aims to create devices on the konker platform remotely just using the login and password of your account on the platform

You must have a registered account on the [konker platform](https://prod.konkerlabs.net/registry/subscription/)

You need to have these two libraries installed [WiFiClientSecure](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFiClientSecure) and [HTTPClient](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient)


## Functions

#### Connect your account konker

```javascript
device.connect_account("your email account konker","your account password");
```

#### Create device on the platform and establish connection

```javascript
  device.create("define your device id","define your device name");
```

#### Send json data on device channel

```javascript
  device.send_data("define your channel","{\"Text\":\"test\"}");
```

#### Get json data from device channel

```javascript
device.get_data("define your channel",1));
```

## Example

```javascript
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
```
