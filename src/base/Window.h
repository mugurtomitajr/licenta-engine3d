#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

#include "../base/EngineManager.h"

class Window {
private:
    int height;
    int width;
    float ratio;
    double drawFactor = 0;

    bool started = false;

    SDL_Window* window;
    SDL_Renderer* renderer;

    EngineManager engineManager;
    
public:

    Window() {
        this -> height = SCREEN_HEIGHT;
        this -> width = SCREEN_WIDTH;
        this -> ratio = (float)this -> height / (float)this -> width;

        this -> window = nullptr;
        this -> renderer = nullptr;

        this -> engineManager = EngineManager();
    }
    
    void Start() {
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        } else {
            SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
            this -> window = SDL_CreateWindow( "Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this -> width, this -> height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP );
            this -> renderer = SDL_CreateRenderer(window, -1, 0);
            if( this -> window == nullptr) {
                printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            } else {
                this -> SetupWindow();
                if(InitDrawer()) {
                    this -> started = true;
                    this -> engineManager.Start(this->renderer, this->window);
                }
            }
        }
    }

    void Run() {
        this -> engineManager.Run();
    }

    void Stop() {
        if(this -> started) {
            this->engineManager.Stop();
        }

        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void SetupWindow() {

        int renderWidth = 0;
        int renderHeight = 0;
        SDL_GetRendererOutputSize(this -> renderer, &renderWidth, &renderHeight);

        SDL_DisplayMode displayMode;
        SDL_GetDesktopDisplayMode(0, &displayMode);
        this -> width = displayMode.w;
        this -> height = displayMode.h;

        this -> drawFactor =  (double) renderHeight / (double) this -> height;

        SetScreenSize(this -> width, this -> height, this -> drawFactor);
    }

};

#endif // ENGINE_WINDOW_H
