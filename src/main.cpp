#include "Arduino.h"
#include "config.hpp"
#include "communication/ComManager.cpp"
#include "pad/PadManager.cpp"

ComManager comManager("bt-gimbal-pad");
PadManager padManager(
    JOY_BTN_UP,
    JOY_BTN_DOWN,
    JOY_BTN_LEFT,
    JOY_BTN_RIGHT,
    BTN1_PIN,
    BTN2_PIN,
    &comManager
);

const unsigned long retryInterval = 10000; // Retry every 10 seconds
unsigned long lastRetryTime = 0;

void attemptAddPeer();

void setup() {
    #if LOG == 1
    Serial.begin(9600);
    while (!Serial) {
        delay(10);
    }
    #endif
    
    comManager.setup();
    padManager.setup();

    lastRetryTime = millis();
    attemptAddPeer();
    delay(1000);
    
    comManager.sendMessage("Hello World");
}

void loop() {
    padManager.checkButtonsAndSend();
    
    // if (!comManager.isPeerAdded()) {
    //     unsigned long currentMillis = millis();

    //     if (currentMillis - lastRetryTime >= retryInterval) {
    //         lastRetryTime = currentMillis;
    //         attemptAddPeer();
    //     }
    // }
}

void attemptAddPeer() {
    if (!comManager.isPeerAdded()) {
        comManager.addPeer(GIMBAL_MAC);
    }
}


// #include <WiFi.h>
// #include <esp_now.h>
// // #include "config.hpp"

// // Replace with your gimbal's MAC address
// const uint8_t peerMAC[] = {0x9C, 0x9C, 0x1F, 0xC8, 0x87, 0x88};

// void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

// void setup() {
//   Serial.begin(9600);
  
//   // Initialize WiFi in STA mode and ESP-NOW
//   WiFi.disconnect(true);
//   WiFi.mode(WIFI_STA);
// //   WiFi.channel(1);
//   Serial.println("Setting WiFi channel to " + String(WiFi.channel()));

//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }
  
//   esp_now_register_send_cb(onDataSent);

//   // Add peer
// esp_now_peer_info_t peerInfo = {};
// memcpy(peerInfo.peer_addr, peerMAC, 6);
// peerInfo.channel = 1;
// peerInfo.encrypt = false;
// peerInfo.ifidx = WIFI_IF_STA;

// if (esp_now_add_peer(&peerInfo) != ESP_OK) {
//     Serial.println("Failed to add peer");
//     return;
// }

// // Send a test message
// const char* testMsg = "Test";

// auto response = esp_now_send(peerMAC, (uint8_t *)testMsg, strlen(testMsg));
// Serial.println("response: " + String(response));
//   if (response != ESP_OK) {
//     Serial.println("Failed to send test message");
//   } else {
//     Serial.println("Test message sent");
//   }
// }

// void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
//   Serial.print("Data sent status: ");
//   if (status == ESP_NOW_SEND_SUCCESS) {
//     Serial.println("Success");
//   } else {
//     Serial.print("Failure, Error Code: ");
//     Serial.println(status, HEX); // Log the error code in hexadecimal format
//   }
// }
// void loop() {
//   // Nothing to do here
// }
