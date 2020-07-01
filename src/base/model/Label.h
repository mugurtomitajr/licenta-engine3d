//
// Created by Mugur Tomita Jr. on 18/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_LABEL_H
#define ENGINE_LABEL_H

#include <string>

#include "UIVisibleObject.h"
#include "shape/Rectangle.h"

#include "../../support/Util.h"
#include "../drawer/Drawer.h"

class Label : public UIVisibleObject {
private:
    double size = 0;
    Color color = Color::BLACK();
    SDL_Texture *texture = nullptr;

    std::string text;

public:
    Label() : UIVisibleObject(BiVector(), BiLine(), new Rectangle(BiVector(), BiVector())) {
        this -> text = "Empty...";
        this -> color = Color::BLACK();
        this -> size = 50;
    }

    Label(std::string p_text, const double &p_size, const Color &p_color) : UIVisibleObject(BiVector(), BiLine(), new Rectangle(BiVector(), BiVector())) {
        this -> text = p_text;
        this -> color = p_color;
        this -> size = p_size;
    }

    Label(const char* p_text, const double &p_size, const Color &p_color, BiVector p_position, BiLine p_margin) : UIVisibleObject(p_position, p_margin, new Rectangle(BiVector(), BiVector())) {
        this -> text = p_text;
        this -> color = p_color;
        this -> size = p_size;
    }

    Label(const Label &that) : UIVisibleObject(that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Label &operator=(const Label &that) {
        UIVisibleObject::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Label() {
        this->clear();
    }

    void clear() {
        UIVisibleObject::clear();
        if(this -> texture != nullptr) {
            SDL_DestroyTexture(this -> texture);
            this -> texture = nullptr;
        }
    }

    void copy(const Label &that) {
        UIVisibleObject::copy(that);

        this -> texture = nullptr;
        this -> size = that.size;
        this -> color = that.color;
        this -> text = that.text;
    }

    void Start(SDL_Renderer* p_renderer) {
        BiVector dim = prepareText(p_renderer, this -> texture, this -> size, this -> text, this -> color);
        this->setDimensions(dim);

        UIVisibleObject::Start(p_renderer);
    }

    void Update(double dt) {
        UIVisibleObject::Update(dt);
    };

    void setText(std::string p_text) {
        if(this->text.compare(p_text)) {
            this -> text = p_text;
            if (this->texture != nullptr) {
                SDL_DestroyTexture(this->texture);
                this->texture = nullptr;
            }
            if (this->parent != nullptr) {
                if (ViewContainer *casted = dynamic_cast<ViewContainer *>(this->parent)) {
                    casted->layoutUpdated = true;
                }
            }
        }
    }

    std::string getText() {
        return std::string(this -> text);
    }

    void Render(double dt, double inter) {
        UIVisibleObject::Render(dt, inter);

        // TODO : CHECK - RENDER OVER CHILDREN - MAYBE NEVER HAS CHILDREN
        if(this -> texture == nullptr) {
            BiVector dim = prepareText(this -> renderer, this -> texture, this -> size, this -> text, this -> color);
            this->setDimensions(dim);
        }
        drawText(this -> renderer, this -> texture, this -> renderPosition, this -> getDimensions());
    }
};

#endif // ENGINE_LABEL_H
