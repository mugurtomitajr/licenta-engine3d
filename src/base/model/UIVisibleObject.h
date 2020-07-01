//
// Created by Mugur Tomita Jr. on 16/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_UIVISIBLEOBJECT_H
#define ENGINE_UIVISIBLEOBJECT_H

#include "AliveObject.h"
#include "../drawer/Drawer.h"

class UIVisibleObject : public AliveObject {
private:

public:
    UIVisibleObject() : AliveObject(BiVector(), BiLine()) {

    }

    UIVisibleObject(Shape *p_shape) : AliveObject(BiVector(), BiLine(), p_shape) {

    }

    UIVisibleObject(BiVector p_position, BiLine p_margin) : AliveObject(p_position, p_margin) {

    }

    UIVisibleObject(BiVector p_position, BiLine p_margin, Shape *p_shape) : AliveObject(p_position, p_margin, p_shape) {

    }

    UIVisibleObject(const UIVisibleObject &that) : AliveObject((AliveObject)that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    UIVisibleObject &operator=(const UIVisibleObject &that) {
        AliveObject::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~UIVisibleObject() {
        this->clear();
    }

    void clear() {
        AliveObject::clear();
    }

    void copy(const UIVisibleObject &that) {
        AliveObject::copy(that);
    }

    void Start(SDL_Renderer* p_renderer) {
        //this -> speed = BiVector(30, 30);
        AliveObject::Start(p_renderer);
    };

    void Update(double dt) {
        AliveObject::ResetUpdatePosition(dt);
        AliveObject::Update(dt);
    };

    void Render(double dt, double inter) {
        AliveObject::ResetRenderPosition(dt, inter);
        AliveObject::Render(dt, inter);
    };

    void Stop() {
        AliveObject::Stop();
    };

    void OnNotify(int tag, std::string message) {
        AliveObject::OnNotify(tag, message);
    };
};

#endif // ENGINE_UIVISIBLEOBJECT_H
