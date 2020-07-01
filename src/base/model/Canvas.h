
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//


#ifndef ENGINE_CANVAS_H
#define ENGINE_CANVAS_H

#include "UIVisibleObject.h"
#include "Label.h"
#include "../../math/BiVector.h"
#include "../space/Camera.h"

class Canvas : public UIVisibleObject {
private:
    BiVector canvasSize;
    Camera *camera;
    SDL_Surface *canvasSurface = nullptr;
    SDL_Renderer *canvasRenderer = nullptr;

public:
    Canvas(const BiVector &p_position, const BiLine &p_margin, const BiVector &p_size, const Color &p_color) :
    UIVisibleObject(p_position, p_margin, new Rectangle(BiVector(), p_size, p_color)) {
        this -> canvasSize = p_size;
    }

    Canvas(const Canvas &that) : UIVisibleObject((UIVisibleObject)that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Canvas &operator=(const Canvas &that) {
        UIVisibleObject::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Canvas() {
        this->clear();
    }

    void clear() {
        UIVisibleObject::clear();
    }

    void copy(const Canvas &that) {
        UIVisibleObject::copy(that);
    }

    void bindCamera(Camera *p_camera) {
        this -> camera = p_camera;
    }

    virtual void Initialise(SDL_Renderer *p_renderer) {
        this -> shape -> setDimension(this -> canvasSize);
        this -> camera -> Start();
    };

    void Start(SDL_Renderer* p_renderer) {
        Initialise(p_renderer);
        UIVisibleObject::Start(p_renderer);
    };

    void Update(double dt) {
        UIVisibleObject::Update(dt);
    };

    void Render(double dt, double inter) {
        if(this -> camera != nullptr) {
            SDL_Surface *cameraSurface = this -> camera -> getSurface();

            // TODO: Optimize - no need to convert to viewport every frame
            BiVector destinationPosition = worldportToViewport(this -> renderPosition);
            BiVector destinationSize = worldsizeToViewsize(this -> canvasSize);

            SDL_Rect destination = {destinationPosition.intX(), destinationPosition.intY() - destinationSize.intY(), destinationSize.intX(), destinationSize.intY()};
            SDL_Texture *texture = SDL_CreateTextureFromSurface(this->renderer, cameraSurface);
            SDL_RenderCopy(this -> renderer, texture, nullptr, &destination);
            SDL_DestroyTexture(texture);
        }
        UIVisibleObject::Render(dt, inter);
    };

    virtual void renderPrepare() {

    };

    virtual void renderPost() {

    }

    void Stop() {
        UIVisibleObject::Stop();
    };

    void OnNotify(int tag, std::string message) {
        UIVisibleObject::OnNotify(tag, message);
    };
};

#endif // ENGINE_CANVAS_H
