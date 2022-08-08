#ifndef _KONKER_H
#define _KONKER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

extern bool debug_log;

class Konker{

    private:

    public:
    
        void connect_account(String email, String password);
        void create(String device_id, String device_name);
        int send_data(String channel, String data_json);
        String get_data(String channel, int qty_results);

};


#endif