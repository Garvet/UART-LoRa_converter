#include <Arduino.h>
#include <heltec.h>

#define BAND    4330E5

int packet_size;
char *packet_data;

void setup() { 
    Heltec.begin(false, true, false, true, BAND);
    LoRa.receive();

    Serial.begin(9600);
    while(!Serial) {}
    delay(2000);
    Serial.println("Start!");
}

void loop() {
    packet_size = LoRa.parsePacket();
    if (packet_size) { 
        packet_data = new char [packet_size+1];
        for (int i = 0; i < packet_size; i++)
            packet_data[i] = (char) LoRa.read(); 
        packet_data[packet_size] = '\0';
        Serial.println(packet_data);
        packet_size = 0;
        delete[] packet_data;
    }

    if(Serial.available())
    {
        int read_S = Serial.read();
        bool print = false;
        if(read_S == '+') {
            digitalWrite(LED, HIGH);
            print = true;
        }
        else if(read_S == '-') {
            digitalWrite(LED, LOW);
            print = true;
        }
        else if(('0'<=read_S)&&(read_S<='9')) {
            print = true;
        }

        if(print) {
            LoRa.beginPacket();
            LoRa.setTxPower(14, RF_PACONFIG_PASELECT_PABOOST);
            LoRa.print(read_S);
            LoRa.endPacket();
            Serial.print("read: ");
            Serial.println((char)read_S);
        }
    }
}