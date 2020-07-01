//
// Created by Mugur Tomita Jr. on 17/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_COLOR_H
#define ENGINE_COLOR_H

#include <SDL2/SDL.h>
#include <algorithm>

class Color {
public:
    int r, g, b, a;

    Color() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
        this->a = 0;
    }

    Color(int p_r, int p_g, int p_b) {
        this->r = p_r;
        this->g = p_g;
        this->b = p_b;
        this->a = 255;
    }

    Color(int p_r, int p_g, int p_b, int p_a) {
        this->r = p_r;
        this->g = p_g;
        this->b = p_b;
        this->a = p_a;
    }

    Color(const Color &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Color &operator=(const Color &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Color() {
        this->clear();
    }

    void clear() {

    }

    void copy(const Color &that) {
        this -> r = that.r;
        this -> g = that.g;
        this -> b = that.b;
        this -> a = that.a;
    }

    Color operator*(double factor) {
        Color result = Color(this->r, this->g, this->b, 255);
        result.r = std::min<double>(result.r * factor, 255.0f);
        result.g = std::min<double>(result.g * factor, 255.0f);
        result.b = std::min<double>(result.b * factor, 255.0f);
        result.a = 255;
        return result;
    }

    Color& operator*=(double factor) {
        this -> r = std::min<double>(this -> r * factor, 255.0f);
        this -> g = std::min<double>(this -> g * factor, 255.0f);
        this -> b = std::min<double>(this -> b * factor, 255.0f);
        this -> a = 255;
        return *this;
    }

    SDL_Color toSDL() const {
        return SDL_Color{(Uint8)this->r, (Uint8)this->g, (Uint8)this->b, (Uint8)this->a};
    }

    static Color WHITE() {
        return Color(255, 255, 255, 255);
    }

    static Color BLACK() {
        return Color(0, 0, 0, 255);
    }

    static Color TRANSPARENT() {
        return Color(0, 0, 0, 0);
    }

    static Color RED() {
        return Color(255, 0, 0, 255);
    }

    static Color GREEN() {
        return Color(0, 255, 0, 255);
    }

    static Color BLUE() {
        return Color(0, 0, 255, 255);
    }

    static Color YELLOW() {
        return Color(255, 255, 0, 255);
    }
};

#endif // ENGINE_COLOR_H
