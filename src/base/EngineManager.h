
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_ENGINEMANAGER_H
#define ENGINE_ENGINEMANAGER_H

#include <chrono>
#include <thread>

#include <OpenGL/OpenGL.h>

#include "SceneManager.h"

class EngineManager {
private:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;

    SDL_Surface *surface = nullptr;
    SDL_Renderer *softwareRenderer = nullptr;

    SceneManager *sceneManager = nullptr;

    double cumulativeFramerateRefresh;
    int cumulativeFramerateRefreshCount;

    long long frameCount = 0;

public:
    bool isRunning;

    EngineManager() {
        this -> cumulativeFramerateRefresh = 0.0f;
        this -> cumulativeFramerateRefreshCount = 0;

        this -> sceneManager = new SceneManager();
    }

    EngineManager(const EngineManager &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    EngineManager &operator=(const EngineManager &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~EngineManager() {
        this->clear();
    }

    void clear() {
        if(this -> sceneManager != nullptr) {
            delete this -> sceneManager;
            this -> sceneManager = nullptr;
        }
    }

    void copy(const EngineManager &that) {
        this -> sceneManager = new SceneManager();
    }

    void Start(SDL_Renderer* p_renderer, SDL_Window* p_window) {
        this -> renderer = p_renderer;
        this -> window = p_window;

        //this -> surface = SDL_CreateRGBSurface(0, WORLD_VECTOR.intX(), WORLD_VECTOR.intY(), 32, 0, 0, 0, 0);
        this -> surface = SDL_CreateRGBSurface(0, USED_SCREEN_WIDTH, USED_SCREEN_HEIGHT, 32, rmask, gmask, bmask, amask);

        this -> softwareRenderer = SDL_CreateSoftwareRenderer(this -> surface);

        this -> isRunning = true;

        this -> sceneManager -> Start(this -> softwareRenderer, this -> window);
    }


    // TODO: VERIFY IF CORRECT - FPS counter (difference betrween 60 Render / 60 Update & 60 Render / 10 Update)
    void Run() {
        auto lastTime = std::chrono::system_clock::now();
        auto currentTime = std::chrono::system_clock::now();
        auto afterTime = std::chrono::system_clock::now();
        std::chrono::duration<float> lastElapsed{};

        auto lastTimeUpdate = std::chrono::system_clock::now();

        long long waitTime = 0;
        long long cumulativeUpdate = 0;
        long long elapsedMicroseconds;
        long long lastElapsedMicroseconds;
        long long lastElapsedUpdateMicroseconds = 1;
        long long cumulativeInterpolator = 0;

        while (this -> isRunning) {
            this -> frameCount += 1;

            currentTime = std::chrono::system_clock::now();
            lastElapsedMicroseconds = (currentTime - lastTime).count();
            lastTime = currentTime;

            cumulativeUpdate += lastElapsedMicroseconds;
            cumulativeInterpolator += lastElapsedMicroseconds;
            if(cumulativeUpdate > UPDATE_TIME_PER_FRAME) {
                cumulativeUpdate -= UPDATE_TIME_PER_FRAME;

                lastElapsedUpdateMicroseconds = (currentTime - lastTimeUpdate).count();
                lastTimeUpdate = currentTime;
                this->Update(((double)lastElapsedUpdateMicroseconds) / 1000000.0f);
                cumulativeInterpolator = 0;
            }

            lastElapsedMicroseconds += (std::chrono::system_clock::now() - currentTime).count();
            this -> Render(((double)lastElapsedMicroseconds) / 1000000.0f, ((double)lastElapsedUpdateMicroseconds) / 1000000.0f, (double)(cumulativeInterpolator) / (double)(lastElapsedUpdateMicroseconds));

            elapsedMicroseconds = (std::chrono::system_clock::now() - currentTime).count();
            if(elapsedMicroseconds < RENDER_TIME_PER_FRAME) {
                waitTime = RENDER_TIME_PER_FRAME - elapsedMicroseconds;
                std::this_thread::sleep_for(std::chrono::microseconds (waitTime));
            }
        }
    }

    void Update(double dt) {
        if(this -> isRunning) {
            this -> sceneManager -> Update(dt);
            this -> checkVitalInformation();
        }
    }

    void checkVitalInformation() {
        if(this -> sceneManager -> applicationShouldClose) {
            this -> isRunning = false;
        }
    }

    void Render(double renderDelta, double dt, double inter) {
        //std::cout << "Render: " << 1.0f / dt << "; Interpolator: " << inter << "\n";
        this -> FramerateCalculation(renderDelta, dt);

        SDL_SetRenderDrawColor(this -> renderer, SCREEN_OFFSET_COLOR.r, SCREEN_OFFSET_COLOR.g, SCREEN_OFFSET_COLOR.b, SCREEN_OFFSET_COLOR.a);
        SDL_RenderClear(this -> renderer);

        SDL_SetRenderDrawColor(this -> softwareRenderer, SCREEN_BACKGROUND_COLOR.r, SCREEN_BACKGROUND_COLOR.g, SCREEN_BACKGROUND_COLOR.b, SCREEN_BACKGROUND_COLOR.a);
        SDL_RenderClear(this -> softwareRenderer);

        this -> sceneManager -> Render(dt, inter);

        SDL_Rect origin = {0, 0, WORLD_VECTOR.intX(), WORLD_VECTOR.intY()};
        SDL_Rect destination = {USED_WIDTH_OFFSET, USED_HEIGHT_OFFSET, USED_SCREEN_WIDTH, USED_SCREEN_HEIGHT};

        SDL_Texture *texture = SDL_CreateTextureFromSurface(this->renderer, this->surface);

        SDL_RenderCopy(this -> renderer, texture, &origin, &destination);

        SDL_RenderPresent(this -> renderer);
    }

    void FramerateCalculation(double renderDelta, double dt) {
        this -> cumulativeFramerateRefresh += renderDelta;
        this -> cumulativeFramerateRefreshCount += 1;

        if(this -> cumulativeFramerateRefresh >= 1.0f) {

            int fps = (int)(((double)this -> cumulativeFramerateRefreshCount) / this->cumulativeFramerateRefresh);

            // TODO: Remove all this
            //std::string title = "Game - fps: ";
            //title += std::to_string(fps);

            //title += " - update rate: ";
            //title += std::to_string((int)(1.0f / dt));
            //title += "";

            //std::cout << title << "\n";

            std::string renderFps = "Render: " + std::to_string(fps) + " hz;";
            std::string updateFps = "Update: " + std::to_string((int)(1.0f / dt)) + " hz;";

            this -> sceneManager -> setRenderFps(renderFps);
            this -> sceneManager -> setUpdateFps(updateFps);

            this -> cumulativeFramerateRefresh -= 1.0f;
            this -> cumulativeFramerateRefreshCount = 0;
        }
    }

    void Stop() {
        this -> sceneManager -> Stop();
    }
};

#endif // ENGINE_ENGINEMANAGER_H
