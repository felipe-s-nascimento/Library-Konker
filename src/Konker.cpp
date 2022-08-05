#include <Konker.h>

extern "C" {
#include "crypto/base64.h"
}

WiFiClientSecure client;
HTTPClient https;
String token;
String nameApplication;

String user_device;
String password_device;
String guid_device;

long time_response = 0;

void application_default(){

    if(client.connect("api.prod.konkerlabs.net",443)){

        https.begin(String("https://api.prod.konkerlabs.net:443/v1/applications/"));
        https.addHeader("Authorization",String("Bearer "+ token));
        
        int httpResponseCode = https.GET();
        
        if (httpResponseCode > 0) {

            String received = https.getString();
            //Serial.println();
            //Serial.println(received);

            nameApplication = received.substring(received.indexOf("name")+7,received.lastIndexOf("\"}]}"));
            
            log_e("Connected account");
            https.end();
            client.stop();
         
        }
        else{
            log_e("Connection account Failed");
            
            https.end();
            client.stop();
        }
    
    }

    https.end();
    client.stop();

}

void get_guid_device(String device_id){

    if(client.connect("api.prod.konkerlabs.net",443)){
        
        https.begin(String("https://api.prod.konkerlabs.net:443/v1/"+nameApplication+"/devices/"));
        https.addHeader("Connection", "keep-alive");
        https.addHeader("Authorization",String("Bearer "+ token));

        int httpResponseCode = https.GET();
        
        if (httpResponseCode > 0){

            String received = https.getString();
            //Serial.println();
            //Serial.println(received);
            
            received = received.substring(received.indexOf(device_id),received.lastIndexOf("\"}")+1);

            guid_device = received.substring(received.indexOf("guid")+7,received.lastIndexOf("\""));

            https.end();
            client.stop();

        }
        else{
            log_e("Failed to get guid device");
        }
       
    }

    https.end();
    client.stop();


}

void create_connection_device(){

    if(client.connect("api.prod.konkerlabs.net",443)){
        
        https.begin(String("https://api.prod.konkerlabs.net:443/v1/"+nameApplication+"/deviceCredentials/"+guid_device));
        https.addHeader("Authorization",String("Bearer "+ token));

        int httpResponseCode = https.POST("");
        
        if (httpResponseCode > 0) {

            String received = https.getString();
            //Serial.println();
            //Serial.println(received);
            
            user_device = received.substring(received.indexOf("username")+11,received.lastIndexOf("httpURLPub")-3);
            password_device = received.substring(received.indexOf("password")+11,received.lastIndexOf("username")-3);

            //Serial.println(user_device);
            //Serial.println(password_device);

            log_e("Connected device");

  
        }
        else{
            
            log_e("Failed connection device");

        }

    }

    https.end();
    client.stop();

}

void Konker::connect_account(String email, String password){
    
    //const char * code = String(email+":"+password).c_str();
    
    String str_code = email+":"+password;

    const char * code = str_code.c_str();

    size_t outputLength;
    
    unsigned char * coded = base64_encode((const unsigned char *)code,strlen(code), &outputLength);

    String str_coded = (const char*)coded;
    
    free(coded);

    if(client.connect("api.demo.konkerlabs.net",443)){

        https.begin(String("https://api.demo.konkerlabs.net:443/v1/oauth/token?grant_type=client_credentials"));
        https.addHeader("Authorization",String("Basic "+ str_coded));
        
        int httpResponseCode = https.GET();
        
        if (httpResponseCode > 0) {

            String received = https.getString();
            //Serial.println();
            //Serial.println(received);

            token = received.substring(received.indexOf("access_token")+15,received.indexOf("token_type")-3);

            https.end();
            client.stop();
            
            application_default();
        }
        else{
            log_e("Connection with Konker Failed");
        }
       
    }

    https.end();
    client.stop();

}

void Konker::create(String device_id, String device_name){

    if(client.connect("api.prod.konkerlabs.net",443)){
        
        String json;
            json+= "{";
            json+= "\"id\":\"";
            json+= device_id;
            json+= "\",";
            json+= "\"name\":\"";
            json+= device_name;
            json+= "\",";
            json+= "\"description\":\"\",";
            json+= "\"locationName\":\"default\",";
            json+= "\"deviceModelName\":\"default\",";
            json+= "\"active\":true,";
            json+= "\"debug\":false,";
            json+= "\"tags\":[";
            json+= "\"string\"";
        json+="]}";

        //Serial.println(json);

        https.begin(String("https://api.prod.konkerlabs.net:443/v1/"+nameApplication+"/devices/"));
        https.addHeader("Authorization",String("Bearer "+ token));
        https.addHeader("Content-Type","application/json");

        int httpResponseCode = https.POST(json);
        
        if (httpResponseCode > 0) {

            String received = https.getString();
            //Serial.println();
            //Serial.println(received);
            
            int message = received.substring(received.indexOf("code")+6,received.indexOf("status")-2).toInt();

            if(message == 400){
                
                https.end();
                client.stop();
                
                get_guid_device(device_id);

            }
            else{

                guid_device = received.substring(received.indexOf("guid")+7,received.indexOf("\"}}"));
                
                log_e("Device created");

                //Serial.println(guid_device);

            }
            

            https.end();
            client.stop();
        }
        else{
            log_e("Failed created device");
        }
        https.end();
        client.stop();

    }


}

String Konker::get_data(String channel, int qty_results){

    unsigned long time_now = millis();

    if(time_now - time_response >= 2000) {
        time_response = time_now;

        create_connection_device();

        https.begin(String("https://api.demo.konkerlabs.net:443/v1/default/incomingEvents?q=channel:"+String(channel)+"&limit="+String(qty_results)));
        https.addHeader("Authorization",String("Bearer "+ token));
        https.addHeader("Connection", "keep-alive");
        
        int httpResponseCode = https.GET();
        
        if (httpResponseCode > 0) {
            //Serial.print("HTTP ");
            //Serial.println(httpResponseCode);
            
            String received = https.getString();
            
            //Serial.println();
            //Serial.println(received);
            
            https.end();
            client.stop();

            return received;
        }
        else{
            return "null";
        }
        
        https.end();
        client.stop();

    }

}

int Konker::send_data(String channel, String data_json){
    
    create_connection_device();

    String str_code = user_device+":"+password_device;
    
    const char * code = str_code.c_str();

    size_t outputLength;
    
    unsigned char * coded = base64_encode((const unsigned char *)code,strlen(code), &outputLength);

    String str_coded = (const char*)coded;
    
    free(coded);
        
    if(client.connect("data.prod.konkerlabs.net",443)){

        int tamanho = data_json.length();

        //Serial.println(data_json);
        
        client.print(String("POST ")+"/pub/"+user_device+"/"+channel+" HTTP/1.1\r\n"+
                        "Host: data.prod.konkerlabs.net"+"\r\n"+
                        "Authorization: Basic "+str_coded+
                        "Content-Type: application/json\r\n"+
                        "Content-Length: "+tamanho+"\r\n\r\n"+
                        data_json +
                        "Connection: closed"+"\r\n\""
                        );
            
        delay(1000);
        
        String received;
        char charactere;

        while (client.available()) {
            //char c = client.read();
            //Serial.write(c);
            charactere = client.read();
            received = received + charactere;
        }

        client.stop();
        
        int response = (received.substring(received.indexOf("code")+7,received.indexOf("message")-3)).toInt();

        if(response == 200){

            log_e("Message send success");
            return 1;
        }
        else{

            log_e("Message send fail");
            return -1;
        }

    }

}