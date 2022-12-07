
/*
SENDER
30:C6:F7:04:D0:7C
RECEIVER
30:C6:F7:04:CA:D8
*/

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
uint8_t ar[6]={0,0,0,0,0,0};
esp_now_peer_info_t peerInfo;
uint8_t broadcastAddress[] = {0x30, 0xC6, 0xF7, 0x04, 0xD0, 0x7C}; //--> REPLACE WITH THE MAC Address of your receiver / ESP32 Receiver.


int send_rnd_val_1;
float map(float c);


String success; //--> Variable to store if sending data was successful.

//----------------------------------------Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int rnd_1[3];
    
} struct_message;

struct_message send_Data; //--> Create a struct_message to send data.
//----------------------------------------

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Succesls" : "Delivery Failll");
  if (status ==0){
    success = "Delivery lSuccess :)";
  }
  else{
    success = "Delivery lFail :(";
  }
  Serial.println(">>>>>");
  //Serial.println(send_Data.rnd_1);
  
  
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ VOID SETUP
void setup() {
  Serial.begin(921600);
  WiFi.mode(WIFI_STA); //--> Set device as a Wi-Fi Station.

  pinMode(32,INPUT);
  pinMode(27,INPUT);
  pinMode(14,INPUT);

  //----------------------------------------Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //----------------------------------------
  
  //----------------------------------------Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  //esp_now_register_send_cb(OnDataSent);
  //----------------------------------------
  
  //----------------------------------------Register peer
  //esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  //----------------------------------------
  
  //----------------------------------------Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  //----------------------------------------
}
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ VOID LOOP
void loop() {
  //----------------------------------------Set values to send
  
  send_rnd_val_1=analogRead(34)*180/4095;
  send_Data.rnd_1[0]=analogRead(34)*180/4095;//+digitalRead(35)*10+digitalRead(32);
  send_Data.rnd_1[1]=digitalRead(27);
  send_Data.rnd_1[2]=digitalRead(14);
  //send_Data.rnd_1=map(send_Data.rnd_1)/2;
  /*Serial.println();
  Serial.print(">>>>> ");
  Serial.print("Send data:  ");
  Serial.println(send_Data.rnd_1);
  Serial.print(analogRead(34));
  Serial.print("--");
  Serial.print(digitalRead(14));
  Serial.print("--");*/
  Serial.println(send_Data.rnd_1[0]);
  Serial.println(send_Data.rnd_1[1]);
  Serial.println(send_Data.rnd_1[2]);

  
  /*Serial.print("--");
  Serial.println(analogRead(32));*/
  //Serial.println(analogRead(32));
  

  //----------------------------------------Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &send_Data, sizeof(send_Data));
 
  delay(500);


}
float map(float c){
  
  c=c*360/4095;
 return c;
}