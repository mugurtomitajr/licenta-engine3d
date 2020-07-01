
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//


#ifndef ENGINE_INTERCATIONCONTAINER_H
#define ENGINE_INTERCATIONCONTAINER_H

#include "ViewContainer.h"
#include "Label.h"
#include "../../math/BiVector.h"

class InteractionContainer : public ViewContainer {
protected:
    bool hasHover = false;
    bool hasLeftClick = false;
    bool hasRightClick = false;

    bool leftClickDebounce = false;
    bool rightClickDebounce = false;

    bool hasFocus = false;

    std::string leftClickId = "";
    std::string rightClickId = "";
    void (AliveObject::*onLeftClick) (std::string) = nullptr;
    void (AliveObject::*onRightClick) (std::string) = nullptr;

    AliveObject *leftContext;
    AliveObject *rightContext;

public:
    InteractionContainer(const ViewContainer::FlexType &p_flexType, const ViewContainer::ExpandType &p_expandType, const ViewContainer::GrowType &p_growType, const BiVector &p_position, const BiLine &p_margin, const Color &p_containerColor) :
    ViewContainer(p_flexType, p_expandType, p_growType, p_position, p_margin, p_containerColor) {

    }

    InteractionContainer(const InteractionContainer &that) : ViewContainer((ViewContainer)that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    InteractionContainer &operator=(const InteractionContainer &that) {
        ViewContainer::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~InteractionContainer() {
        this->clear();
    }

    virtual InteractionContainer* setOnLeftClick(AliveObject *p_context, void(AliveObject::*p_onLeftClick)(std::string)) {
        this -> onLeftClick = p_onLeftClick;
        this -> leftContext = p_context;
        return this;
    }

    virtual InteractionContainer* setOnLeftClick(AliveObject *p_context, void(AliveObject::*p_onLeftClick)(std::string), std::string p_id) {
        this -> onLeftClick = p_onLeftClick;
        this -> leftClickId = p_id;
        this -> leftContext = p_context;
        return this;
    }

    virtual InteractionContainer* setOnRightClick(AliveObject *p_context, void(AliveObject::*p_onRightClick)(std::string)) {
        this -> onRightClick = p_onRightClick;
        this -> rightContext = p_context;
        return this;
    }

    virtual InteractionContainer* setOnRightClick(AliveObject *p_context, void(AliveObject::*p_onRightClick)(std::string), std::string p_id) {
        this -> onRightClick = p_onRightClick;
        this -> rightClickId = p_id;
        this -> rightContext = p_context;
        return this;
    }

    void clear() {
        ViewContainer::clear();
    }

    void copy(const InteractionContainer &that) {
        ViewContainer::copy(that);
        this -> hasHover = that.hasHover;
        this -> hasLeftClick = that.hasLeftClick;
        this -> hasRightClick = that.hasRightClick;
        this -> hasFocus = that.hasFocus;
        this -> onRightClick = that.onRightClick;
        this -> onLeftClick = that.onLeftClick;
        this -> leftContext = that.leftContext;
        this -> rightContext = that.rightContext;
    }

    virtual void Start(SDL_Renderer* p_renderer) {
        ViewContainer::Start(p_renderer);
    };

    virtual void Update(double dt) {
        this -> hasHover = false;
        this -> hasLeftClick = false;
        this -> hasRightClick = false;
        ViewContainer::Update(dt);
    };

    virtual void Render(double dt, double inter) {
        this -> renderPrepare();
        ViewContainer::Render(dt, inter);
        this -> renderPost();
    };

    virtual void renderPrepare() {};
    virtual void renderPost() {
        if (this -> hasHover) {
            drawRectangle(this -> renderer, this -> renderPosition, this -> getDimensions(), Color::BLACK(), 4);
        }
    }

    virtual void hoverInside(const BiVector &p_position, const bool &mouseLeft, const bool &mouseRight) {
        this -> hasHover = true;
        this -> hasLeftClick = mouseLeft;
        if(this -> hasLeftClick && !this -> leftClickDebounce) {
            if(this -> onLeftClick != nullptr) {
                (*(this->leftContext).*this -> onLeftClick)(this -> leftClickId);
            }
        }
        this -> leftClickDebounce = this -> hasLeftClick;

        this -> hasRightClick = mouseRight;
        if(this -> hasRightClick && !this -> rightClickDebounce) {
            if(this -> onRightClick != nullptr) {
                (*(this->rightContext).*this -> onRightClick)(this -> rightClickId);
            }
        }
        this -> rightClickDebounce = this -> hasRightClick;

        this -> hasFocus = this -> hasFocus || this -> hasLeftClick;
    }

    virtual void Stop() {
        ViewContainer::Stop();
    };

    virtual void OnNotify(int tag, std::string message) {
        ViewContainer::OnNotify(tag, message);
    };
};

#endif // ENGINE_SCENE_H
