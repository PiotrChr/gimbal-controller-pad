#include "Arduino.h"
#include "../communication/ComManager.cpp"

struct Button {
    int pin;
    bool state;
    long lastDebounceTime;

    Button(int pin) : pin(pin), state(HIGH), lastDebounceTime(0) {}
};

class PadManager {
private:
    Button upButton, downButton, leftButton, rightButton, btn1, btn2;
    ComManager* comManager;
    const long debounceDelay = 50; // milliseconds for debounce

public:
    PadManager(int up, int down, int left, int right, int btn1, int btn2, ComManager* comManager)
        : upButton(up), downButton(down), leftButton(left), rightButton(right), btn1(btn1), btn2(btn2), comManager(comManager) {
    }

    void setupButtons() {
        pinMode(upButton.pin, INPUT_PULLUP);
        pinMode(downButton.pin, INPUT_PULLUP);
        pinMode(leftButton.pin, INPUT_PULLUP);
        pinMode(rightButton.pin, INPUT_PULLUP);
        pinMode(btn1.pin, INPUT_PULLUP);
        pinMode(btn2.pin, INPUT_PULLUP);
    }

    void setup() {
        setupButtons();
    }

    int isButtonPressed(Button &button) {
        bool reading = digitalRead(button.pin);
    
        if (LOW == button.state && HIGH == reading) {
            button.state = HIGH;
            return -1;
        }

        button.state = reading;

        return reading == LOW ? 1 : 0;
        // if ((millis() - button.lastDebounceTime) > debounceDelay) {
        //     // if the button state has changed:
        //     if (reading != button.state) {
        //         button.state = reading;
        //         if (button.state == LOW) {
        //             return true;
        //         }
        //     }
        // }
        // return false;
    }

    void checkButtonsAndSend() {
        const int up = isButtonPressed(upButton);
        const int down = isButtonPressed(downButton);
        const int left = isButtonPressed(leftButton);
        const int right = isButtonPressed(rightButton);
        const int b1 = isButtonPressed(btn1);
        const int b2 = isButtonPressed(btn2);

        if (up == 1) {
            sendMessage(MSG_UP);
        }
        if (down == 1) {
            sendMessage(MSG_DOWN);
        }
        if (left == 1) {
            sendMessage(MSG_LEFT);
        }
        if (right == 1) {
            sendMessage(MSG_RIGHT);
        }
        if (b1 == 1) {
            sendMessage(MSG_BTN1);
        }
        if (b2 == 1) {
            sendMessage(MSG_BTN2);
        }

        if (up == -1 || down == -1 || left == -1 || right == -1) {
            sendMessage(MSG_STOP);
        }
    }

    void sendMessage(String message) {
        LOG_PRINT("Sending message: ");
        LOG_PRINTLN(message);

        comManager->sendMessage(message);
        delay(100);
    }
};
