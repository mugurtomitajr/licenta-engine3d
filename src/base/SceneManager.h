
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//


#ifndef ENGINE_SCENEMANAGER_H
#define ENGINE_SCENEMANAGER_H

#include <SDL2/SDL.h>
#include <iostream>
#include "drawer/Drawer.h"
#include "scene/Scene.h"
#include "../custom/scene/ExampleScene.h"
#include "events/KeyboardHandler.h"

class SceneManager : public KeyboardHandler {
private:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;

    Scene *currentScene = nullptr;

    bool inTransition = false;

    // EVENTS TREATED HERE

    BiVector mousePosition;

    const Uint8 *keyboardState;

public:
    bool applicationShouldClose = false;

    SceneManager() {
        this -> currentScene = nullptr;
    }

    SceneManager(const SceneManager &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    SceneManager &operator=(const SceneManager &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~SceneManager() {
        this->clear();
    }

    void clear() {
        if(this -> currentScene != nullptr) {
            delete this -> currentScene;
            this -> currentScene = nullptr;
        }
    }

    void copy(const SceneManager &that) {
        //this -> currentScene = new Scene(*(that.currentScene));
        this -> currentScene = nullptr;
    }

    void Start(SDL_Renderer* p_renderer, SDL_Window* p_window) {
        this -> renderer = p_renderer;
        this -> window = p_window;

        this -> currentScene = new ExampleScene();

        this -> currentScene -> Start(this -> renderer);

        this -> delegateTo(this -> currentScene);

        this -> keyboardState = SDL_GetKeyboardState(NULL);
    }

    void Update(double dt) {
        bool mouseLeft = false;
        bool mouseRight = false;
        this -> updateMouseState(mouseLeft, mouseRight);

        SDL_Event event;
        if(SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                this -> applicationShouldClose = true;
            } else if (event.type == SDL_KEYDOWN) {
                if(event.key.keysym.sym == SDLK_ESCAPE) {
                    this -> applicationShouldClose = true;
                }
            }
        }

        if(this -> keyboardState[SDL_SCANCODE_ESCAPE]) {
            this -> applicationShouldClose = true;
        } else {
            this -> verifyKeyboardMaster(dt, this -> keyboardState);
        }

        this -> currentScene -> Update(dt, this -> mousePosition, mouseLeft, mouseRight);

        this -> checkVitalInformation();
    }

    void Render(double dt, double inter) {
        this -> currentScene -> Render(dt, inter);
    }

    void Stop() {
        this -> currentScene -> Stop();
    }

    void Broadcast(int tag, std::string message) {
        if (this->currentScene != nullptr) {
            this->currentScene->OnNotify(tag, message);
        }
    }

    void checkVitalInformation() {
        if (this -> currentScene != nullptr) {
            this->applicationShouldClose = this -> applicationShouldClose || this->currentScene->shouldQuitApplication;
        }
    }

    void updateMouseState(bool &mouseLeft, bool &mouseRight) {
        BiVector position;
        int x = 0;
        int y = 0;
        Uint32 mouseState = SDL_GetMouseState(&x, &y);

        this -> mousePosition.x = viewportToWorldport_X(x);
        this -> mousePosition.y = viewportToWorldport_Y(y);

        mouseLeft = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
        mouseRight = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
    }

    void setRenderFps(std::string p_fps) {
        if(this -> currentScene != nullptr) {
            this -> currentScene->setRenderFps(p_fps);
        }
    }

    void setUpdateFps(std::string p_fps) {
        if(this -> currentScene != nullptr) {
            this -> currentScene->setUpdateFps(p_fps);
        }
    }

    void verifyKeyboard(const double &dt, const Uint8 *keyboardState) {

    }
};

#endif // ENGINE_SCENEMANAGER_H
