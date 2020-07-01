//
// Created by Mugur Tomita Jr. on 17/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_SHAPE_H
#define ENGINE_SHAPE_H

#include "../AliveObject.h"
#include "../../../support/Color.h"

class AliveObject;


// TODO: OPTIMIZATION - MAKE OBJECT SURFACE / TEXTURE - AND PUT TEXTURE ON RENDERER / SURFACE

class Shape {
private:
    AliveObject *parent = nullptr;

public:
    Color color = Color::TRANSPARENT();
    enum DrawType {Fill, Outline};
    DrawType drawType = Fill;

    BiVector pivot;
    BiVector drawPosition;

    Shape() {
        this -> pivot = BiVector();
        this -> parent = nullptr;
    }

    Shape(const BiVector &p_pivot, const Color &color) {
        this -> pivot = p_pivot;
        this -> parent = nullptr;
        this -> color = color;
    }

    Shape(const Shape &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Shape &operator=(const Shape &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Shape() {
        this->clear();
    }

    Shape* setParent(AliveObject *p_parent){
        this -> parent = p_parent;
        return this;
    }

    virtual void clear() {

    }

    virtual void copy(const Shape &that) {
        this -> pivot = that.pivot;
        this -> parent = that.parent;
        this -> drawType = that.drawType;
        this -> color = that.color;
    }

    void UpdateDrawPosition();

    virtual void Draw(SDL_Renderer* renderer) {};
    virtual BiVector getDimensions() const {return BiVector(0, 0);};
    virtual void setDimension(BiVector p_dimensions) {};
    virtual double Area() const {return 0.0f;};

    virtual bool isPointWithin(const BiVector &point) const {return false;};

    Shape *setDrawType(DrawType p_drawType) {
        this -> drawType = p_drawType;
        return this;
    }
};

#endif // ENGINE_SHAPE_H
