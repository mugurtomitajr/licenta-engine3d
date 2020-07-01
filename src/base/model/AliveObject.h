
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//


#ifndef ENGINE_ALIVEOBJECT_H
#define ENGINE_ALIVEOBJECT_H

#include <SDL2/SDL.h>
#include <string>
#include <vector>

#include "../../math/BiVector.h"
#include "../../math/BiLine.h"
#include "shape/Shape.h"

class AliveObject {
private:
    BiVector position;
    BiLine margin;

protected:
    SDL_Renderer *renderer;

    std::vector<AliveObject*> children;
    AliveObject *parent = nullptr;

    BiVector speed;
    BiVector updatePosition;

    Shape *shape = nullptr;

public:
    bool hasStarted = false;
    BiVector renderPosition;

    AliveObject() {
        this -> position = BiVector(0.0f, 0.0f);
        this -> speed = BiVector(0.0f, 0.0f);
        this -> children = std::vector<AliveObject*>();
        this -> parent = nullptr;
        this -> shape = nullptr;
    }

    AliveObject(BiVector p_position, BiLine p_margin) {
        this -> position = p_position;
        this -> margin = p_margin;
        this -> speed = BiVector(0.0f, 0.0f);
        this -> children = std::vector<AliveObject*>();
        this -> parent = nullptr;
        this -> shape = nullptr;
    }

    AliveObject(BiVector p_position, BiLine p_margin, Shape *p_shape) : AliveObject(p_position, p_margin) {
        p_shape->setParent(this);
        this -> shape = p_shape;
    }

    AliveObject(const AliveObject &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    AliveObject &operator=(const AliveObject &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~AliveObject() {
        this->clear();
    }

    virtual void clear() {
        for (auto &it : this -> children) {
            delete it;
            it = nullptr;
        }
        if(this -> shape != nullptr) {
            delete this -> shape;
            this -> shape = nullptr;
        }
    }

    virtual void copy(const AliveObject &that) {
        this -> children = that.children;
        this -> parent = that.parent;
        this -> renderer = that.renderer;
        this -> position = that.position;
        this -> margin = that.margin;
        this -> speed = that.speed;

        // TODO: ALLOCATION - possible problem - maybe new shape needed
        this -> shape = that.shape;
    }

    virtual AliveObject* setMargin(BiLine p_margin) {
        this -> margin = p_margin;
        return this;
    }

    virtual BiLine getMargin() const {
        return this -> margin;
    }

    virtual AliveObject* setPosition(BiVector p_position) {
        this -> position = p_position;
        return this;
    }

    virtual BiVector getPosition() const {
        return this -> position;
    }

    virtual void resetLayout() {};
    virtual void fillLayout() {};
    virtual void rearrangeFilledLayout() {};

    virtual void hoverOver(const BiVector &p_position, const bool &mouseLeft, const bool &mouseRight) {
        if(p_position.isBetween(this->renderPosition, this->renderPosition + this->getDimensions())) {
            hoverInside(p_position, mouseLeft, mouseRight);
            for (auto &it : this -> children) {
                it->hoverOver(p_position, mouseLeft, mouseRight);
            }
        }
    };

    virtual void hoverInside(const BiVector &p_position, const bool &mouseLeft, const bool &mouseRight) {};

    virtual AliveObject* setUsedSpace(const BiVector &p_usedSpace) {
        return this;
    }

    virtual BiVector getUsedSpace() const {
        return this -> getDimensions();
    }

    virtual Color getColor() const {
        if(this -> shape != nullptr) {
            return this -> shape -> color;
        }
        return Color();
    };

    virtual AliveObject* setColor(const Color &p_color) {
        if (this -> shape != nullptr) {
            this -> shape -> color = p_color;
        }
        return this;
    }

    virtual AliveObject* setDimensions(BiVector p_dimensions) {
        if(this -> shape != nullptr) {
            this -> shape -> setDimension(p_dimensions);
        }
        return this;
    }

    virtual BiVector getDimensions() const {
        if(this -> shape != nullptr) {
            return this -> shape -> getDimensions();
        }
        return BiVector(0, 0);
    }

    virtual void Start(SDL_Renderer* p_renderer) {
        this -> renderer = p_renderer;
        this -> hasStarted = true;
        for (auto &it : this -> children) {
            it->Start(p_renderer);
        }
    }

    void ResetUpdatePosition(double dt) {
        if(!this -> speed.isZero()) {
            this -> position = this -> position + (this -> speed * dt);
        }

        if(this -> parent != nullptr) {
            this -> updatePosition = this -> position + this -> parent -> updatePosition;
        } else {
            this -> updatePosition = this -> position;
        }
    }

    virtual void Update(double dt) {
        for (auto &it : this -> children) {
            if(!it -> hasStarted){
                it -> Start(this -> renderer);
            }
            it->Update(dt);
        }
    }

    void ResetRenderPosition(double dt, double inter) {
        if(this -> parent != nullptr) {
            this->renderPosition = this -> parent -> renderPosition + this -> position + (this->speed * (dt * inter));
        } else {
            this->renderPosition = this -> position + (this->speed * (dt * inter));
        }
    }

    virtual void Render(double dt, double inter) {
        if(this -> shape != nullptr) {
            this -> shape -> Draw(this -> renderer);
        }
        for (auto &it : this -> children) {
            if(it -> hasStarted){
                it->Render(dt, inter);
            }
        }
    }

    virtual void Stop() {
        for (auto &it : this -> children) {
            it->Stop();
        }
    }

    virtual void addChild(AliveObject *child) {
        child -> parent = this;
        this -> children.push_back(child);
    }

    virtual void deleteChildren() {
        for (auto &it : this -> children) {
            if(it != nullptr){
                delete it;
                it = nullptr;
            }
        }
        this -> children = std::vector<AliveObject*>();
    }

    virtual void OnNotify(int tag, std::string message) {
        for (auto &it : this -> children) {
            it -> OnNotify(tag, message);
        }
    };

    virtual void Broadcast(int tag, std::string message) {
        if (this -> parent != nullptr) {
            this -> parent -> Broadcast(tag, message);
        }
    }

    virtual void onLeftButtonClick(std::string buttonId) {}

    virtual void onRightButtonClick(std::string buttonId) {}
};

// TODO: REORGANIZE
void Shape::UpdateDrawPosition() {
    this -> drawPosition = this -> pivot;
    if(this -> parent != nullptr) {
        drawPosition += this -> parent -> renderPosition;
    }
}

#endif // ENGINE_ALIVEOBJECT_H
