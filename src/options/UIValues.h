//
// Created by Mugur Tomita Jr. on 15/03/2020.
//

#ifndef ENGINE_UIVALUES_H
#define ENGINE_UIVALUES_H

#include "../support/Color.h"
#include "../math/BiVector.h"

int USED_SCREEN_WIDTH = 512;
int USED_SCREEN_HEIGHT = 390;
double USED_SCREEN_RATIO = 1.0f;
int USED_HEIGHT_OFFSET = 0;
int USED_WIDTH_OFFSET = 0;

int SCREEN_WIDTH = 512;
int SCREEN_HEIGHT = 390;
double SCREEN_RATIO = 1.0f;

const int WORLD_WIDTH = 2560;
const int WORLD_HEIGHT = 1440;
double WORLD_RATIO = (double)((double)WORLD_WIDTH) / ((double)WORLD_HEIGHT);

BiVector WORLD_VECTOR = BiVector(WORLD_WIDTH, WORLD_HEIGHT);

double drawFactor = 1.0;

void SetScreenSize(int width, int height, double dF) {
    drawFactor = dF;
    SCREEN_WIDTH = width;
    SCREEN_HEIGHT = height;
    SCREEN_RATIO = (double)((double)width) / ((double)height);

    USED_SCREEN_HEIGHT = SCREEN_HEIGHT;
    USED_SCREEN_WIDTH = USED_SCREEN_HEIGHT * WORLD_RATIO;
    USED_HEIGHT_OFFSET = 0;
    USED_WIDTH_OFFSET = (SCREEN_WIDTH - USED_SCREEN_WIDTH) / 2;

    if(USED_SCREEN_WIDTH > SCREEN_WIDTH) {
        USED_SCREEN_WIDTH = SCREEN_WIDTH;
        USED_SCREEN_HEIGHT = (1.0f / WORLD_RATIO) * (double) USED_SCREEN_WIDTH;
        USED_HEIGHT_OFFSET = (SCREEN_HEIGHT - USED_SCREEN_HEIGHT) / 2;
        USED_WIDTH_OFFSET = 0;
    }

    USED_SCREEN_RATIO = USED_SCREEN_WIDTH / USED_SCREEN_HEIGHT;
}

const BiVector MAIN_CAMERA_CANVAS_DIMENSIONS = BiVector(960, 650);
const BiVector MAIN_CAMERA_DIMENSIONS = BiVector(960, 650);
const double MAIN_CAMERA_FIELD_OF_VIEW = PI_2o4;
const double MAIN_CAMERA_NEAR = 0.0f;
const double MAIN_CAMERA_FAR = 1000.0f;

const Color APPLICATION_COLOR_BRIGHT = Color(220, 220, 240);
const Color APPLICATION_COLOR_BRIGHT_A_BIT_DARKER = Color(190, 200, 210);
const Color APPLICATION_COLOR_DARK = Color(24, 24, 48);

const Color SCREEN_OFFSET_COLOR = APPLICATION_COLOR_DARK;
const Color SCREEN_BACKGROUND_COLOR = APPLICATION_COLOR_BRIGHT;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
rmask = 0xff000000;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0x0000ff00;
    Uint32 amask = 0x000000ff;
#else
    Uint32 rmask = 0x000000ff;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x00ff0000;
    Uint32 amask = 0xff000000;
#endif

#endif //ENGINE_UIVALUES_H
