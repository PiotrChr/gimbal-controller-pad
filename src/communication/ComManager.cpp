#pragma once

#include "Arduino.h"
#include "../config.hpp"
#include <esp_now.h>
#include <WiFi.h>

class ComManager
{
public:
    ComManager(String name) : name(name), peerAdded(false) {
    }

    void setup() {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_STA);
        // WiFi.channel(COM_CHANNEL);
        LOG_PRINTLN("WiFi channel set to " + String(WiFi.channel()));

        if (esp_now_init() != ESP_OK) {
            LOG_PRINTLN("Error initializing ESP-NOW");
            return;
        } else {
            LOG_PRINTLN("ESP-NOW initialized");
        }

        esp_now_register_send_cb(onDataSent); // Register callback function to check if message was sent to peer
    }

    static void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
        LOG_PRINTLN("Last Packet Send Status: ");
        if (status == ESP_NOW_SEND_SUCCESS) {
            LOG_PRINTLN("Delivery success");
        } else if (status == ESP_NOW_SEND_FAIL) {
            LOG_PRINTLN("Delivery fail");
        } else {
            LOG_PRINTLN("Delivery unknown");
        }
    }

    void sendMessage(String message) {
        uint8_t data[message.length() + 1];
        message.getBytes(data, message.length() + 1);
        sendData(NULL, data, message.length() + 1);
    }

    void addPeer(const uint8_t* peerAddress) {
        esp_now_peer_info_t peerInfo = {};
        memcpy(peerInfo.peer_addr, peerAddress, 6);
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;
        peerInfo.ifidx = WIFI_IF_STA;

        if (esp_now_add_peer(&peerInfo) == ESP_OK) {
            peerAdded = true;
            LOG_PRINTLN("Peer added successfully");
        } else {
            peerAdded = false;
            LOG_PRINTLN("Failed to add peer");
        }
    }

    bool isPeerAdded() {
        return peerAdded;
    }

private:
    String name;
    bool peerAdded;

    void sendData(uint8_t *mac_addr, uint8_t *data, int len) {
        esp_err_t result = esp_now_send(mac_addr, data, len);
        LOG_PRINT("Sending data: ");
        LOG_PRINTLN(result);

        if (result == ESP_OK) {
            LOG_PRINTLN("Sent with success");
        } else {
            LOG_PRINTLN("Error sending the data");
        }
    }
};

