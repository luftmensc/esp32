#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include <stdio.h>
HardwareSerial SerialPort(2); // use UART2
//----------------------------------------

//----------------------------------------Define variables to store incoming readings
int receive_rnd_val_1[3];

//----------------------------------------

//----------------------------------------Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int rnd_1[3];
} struct_message;

struct_message receive_Data; //--> Create a struct_message to receive data.
//----------------------------------------
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Callback when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int ln) {
  memcpy(&receive_Data, incomingData, sizeof(receive_Data));

  Serial.print(receive_Data.rnd_1[0]);
  Serial.print(receive_Data.rnd_1[1]);
  Serial.println(receive_Data.rnd_1[2]);

/*
  SerialPort.print(receive_Data.rnd_1[0]);
  SerialPort.print(receive_Data.rnd_1[1]);
  SerialPort.println(receive_Data.rnd_1[2]);
*/
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ VOID SETUP
void setup() {
  Serial.begin(921600);
  SerialPort.begin(9600, SERIAL_8N1, 16, 17); 

  WiFi.mode(WIFI_STA); //--> Set device as a Wi-Fi Station
  pinMode(33,OUTPUT);
  //----------------------------------------Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //----------------------------------------
  
  esp_now_register_recv_cb(OnDataRecv); //--> Register for a callback function that will be called when data is received
}

void loop() {

}
