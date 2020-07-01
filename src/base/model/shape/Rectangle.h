//
// Created by Mugur Tomita Jr. on 17/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_RECTANGLE_H
#define ENGINE_RECTANGLE_H

#include <SDL2/SDL.h>
#include "Shape.h"
#include "../../drawer/Drawer.h"

class Rectangle : public Shape {
public:
    BiVector dimensions;

    Rectangle() : Shape() {

    }

    Rectangle(const BiVector &p_pivot, const BiVector &p_dimensions) : Shape(p_pivot, Color::TRANSPARENT()) {
        this -> dimensions = p_dimensions;
    }

    Rectangle(const BiVector &p_pivot, const BiVector &p_dimensions, const Color p_color) : Shape(p_pivot, p_color) {
        this -> dimensions = p_dimensions;
    }

    Rectangle(const Rectangle &that) : Shape(that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Rectangle &operator=(const Rectangle &that) {
        Shape::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Rectangle() {
        this->clear();
    }

    void clear() {
        Shape::clear();
    }

    void copy(const Rectangle &that) {
        Shape::copy(that);
        this -> dimensions = that.dimensions;
    }

    void Draw(SDL_Renderer* renderer) {
        Shape::UpdateDrawPosition();
        if(this -> drawType == Fill) {
            fillRectangle(renderer, this -> drawPosition, this -> dimensions, this -> color);
        } else if(this -> drawType == Outline) {
            drawRectangle(renderer, this -> drawPosition, this -> dimensions, this -> color, 1);
        }
    };

    void setDimension(BiVector p_dimensions) {
        this -> dimensions = p_dimensions;
    };

    BiVector getDimensions() const {
        return this -> dimensions;
    };

    double Area() const {
        return this->dimensions.x * this -> dimensions.y;
    };

    bool isPointWithin(const BiVector &point) const {
        return point.isBetween(this -> drawPosition, this -> drawPosition + this -> dimensions);
    }

};

#endif // ENGINE_RECTANGLE_H
