#pragma once

#define LOG 1

#include <Arduino.h>
#if LOG == 1
  #define LOG_PRINT(x) Serial.print(x)
  #define LOG_PRINTLN(x) Serial.println(x)
#else
  #define LOG_PRINT(x)
  #define LOG_PRINTLN(x)
#endif

// Joystick
#define JOY_BTN_UP 4
#define JOY_BTN_DOWN 16
#define JOY_BTN_LEFT 17
#define JOY_BTN_RIGHT 18

// Buttons
#define BTN1_PIN 26
#define BTN2_PIN 25

// Communication
const uint8_t GIMBAL_MAC[] = {0x9C, 0x9C, 0x1F, 0xC8, 0x87, 0x88};

#define COM_CHANNEL 1
#define MSG_UP "u"
#define MSG_DOWN "d"
#define MSG_LEFT "l"
#define MSG_RIGHT "r"
#define MSG_BTN1 "b1"
#define MSG_BTN2 "b2"
#define MSG_STOP "s"

