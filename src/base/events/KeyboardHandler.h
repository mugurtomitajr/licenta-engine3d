//
// Created by Mugur Tomita Jr. on 03/04/2020.
//

#ifndef ENGINE_KEYBOARDHANDLER_H
#define ENGINE_KEYBOARDHANDLER_H


#include <SDL_keycode.h>

class KeyboardHandler {
public:
    KeyboardHandler *manager = nullptr;
    KeyboardHandler *delegate = nullptr;

    void delegateTo(KeyboardHandler *target) {
        this -> delegate = target;
        this -> delegate -> manager = this;
    }

    void regainControl() {
        if(this -> delegate != nullptr) {
            this -> delegate -> manager = nullptr;
            this -> delegate = nullptr;
        }
    }

    void backup() {
        if(this -> delegate != nullptr && this -> delegate -> manager != nullptr) {
            this -> manager -> regainControl();
        }
    }

    void verifyKeyboardMaster(const double &dt, const Uint8 *keyboardState) {
        if(this -> delegate != nullptr) {
            this->delegate->verifyKeyboardMaster(dt, keyboardState);
        } else {
            this->verifyKeyboard(dt, keyboardState);
        }
    }

    virtual void verifyKeyboard(const double &dt, const Uint8 *keyboardState) {};
};


#endif //ENGINE_KEYBOARDHANDLER_H
