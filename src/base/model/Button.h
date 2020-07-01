
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//


#ifndef ENGINE_BUTTON_H
#define ENGINE_BUTTON_H

#include "InteractionContainer.h"
#include "Label.h"
#include "../../math/BiVector.h"

class Button : public InteractionContainer {
private:
    Label *label = nullptr;
    Color buttonColor, hoverColor, focusColor;

public:
    Button(const BiLine &p_margin, const Color &p_buttonColor, const Color &p_hoverColor, const Color &p_focusColor, const char *p_text, const double &p_textSize, const BiLine &p_textMargin, const Color &p_textColor) :
    InteractionContainer(InteractionContainer::FlexType::Column, InteractionContainer::ExpandType::Wrap, InteractionContainer::GrowType::Contain, BiVector(), p_margin, p_buttonColor) {
        this -> label = new Label(p_text, p_textSize, p_textColor, BiVector(), p_textMargin);
        this -> buttonColor = p_buttonColor;
        this -> hoverColor = p_hoverColor;
        this -> focusColor = p_focusColor;
    }

    Button(const Button &that) : InteractionContainer((InteractionContainer)that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Button &operator=(const Button &that) {
        InteractionContainer::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Button() {
        this->clear();
    }

    void clear() {
        InteractionContainer::clear();
        if(this -> label != nullptr) {
            delete this -> label;
            this -> label = nullptr;
        }
    }

    void copy(const Button &that) {
        InteractionContainer::copy(that);
        this -> label = new Label(*that.label);
        this -> buttonColor = that.buttonColor;
        this -> hoverColor = that.hoverColor;
        this -> focusColor = that.focusColor;
    }

    virtual Button* setOnLeftClick(AliveObject *p_context, void(AliveObject::*p_onLeftClick)(std::string)) {
        InteractionContainer::setOnLeftClick(p_context, p_onLeftClick);
        return this;
    }

    virtual Button* setOnLeftClick(AliveObject *p_context, void(AliveObject::*p_onLeftClick)(std::string), std::string p_id) {
        InteractionContainer::setOnLeftClick(p_context, p_onLeftClick, p_id);
        return this;
    }

    virtual Button* setOnRightClick(AliveObject *p_context, void(AliveObject::*p_onRightClick)(std::string)) {
        InteractionContainer::setOnRightClick(p_context, p_onRightClick);
        return this;
    }

    virtual Button* setOnRightClick(AliveObject *p_context, void(AliveObject::*p_onRightClick)(std::string), std::string p_id) {
        InteractionContainer::setOnRightClick(p_context, p_onRightClick, p_id);
        return this;
    }

    virtual void Initialise(SDL_Renderer *p_renderer) {
        this -> addChild(this -> label);
    };

    void Start(SDL_Renderer* p_renderer) {
        Initialise(p_renderer);
        InteractionContainer::Start(p_renderer);
    };

    void Update(double dt) {
        InteractionContainer::Update(dt);
    };

    void Render(double dt, double inter) {
        InteractionContainer::Render(dt, inter);
    };

    virtual void renderPrepare() {
        if(this -> hasHover) {
            this -> setColor(this -> hoverColor);
        } else {
            this -> setColor(this -> buttonColor);
        }

        if(this -> hasLeftClick) {
            this -> setColor(this -> focusColor);
        }

        if(this -> hasRightClick) {
            this -> setColor(this -> focusColor);
        }
    };

    virtual void renderPost() {}

    void Stop() {
        InteractionContainer::Stop();
    };

    void OnNotify(int tag, std::string message) {
        InteractionContainer::OnNotify(tag, message);
    };
};

#endif // ENGINE_SCENE_H
