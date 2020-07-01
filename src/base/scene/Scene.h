
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//


#ifndef ENGINE_SCENE_H
#define ENGINE_SCENE_H

#include "../model/InteractionContainer.h"
#include "../../math/BiVector.h"
#include "../drawer/Drawer.h"

#include "../model/UIVisibleObject.h"
#include "../model/shape/Rectangle.h"

#include "../../options/EngineConstants.h"
#include "../events/KeyboardHandler.h"

class Scene : public InteractionContainer, public KeyboardHandler {
protected:
    const std::string DEFAULT_UI_MESSAGE = "Engine running normally...";
    const double DEFAULT_MESSAGE_TIME = 2.0f;

    std::string currentUiPermanentMessage = DEFAULT_UI_MESSAGE;
    std::string currentUiMessage = currentUiPermanentMessage;
    double messageDisplayedSoFar = -1.0f;

    Label *uiMessage = nullptr;
    Label *renderFps = nullptr;
    Label *updateFps = nullptr;

public:
    bool shouldQuitApplication = false;

    Scene() : InteractionContainer(InteractionContainer::FlexType::Column, InteractionContainer::ExpandType::Wrap, InteractionContainer::GrowType::Grow, BiVector(), BiLine(), SCREEN_BACKGROUND_COLOR) {

    }

    Scene(const Scene &that) : InteractionContainer((InteractionContainer)that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Scene &operator=(const Scene &that) {
        InteractionContainer::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Scene() {
        this->clear();
    }

    void clear() {
        InteractionContainer::clear();
    }

    void copy(const Scene &that) {
        InteractionContainer::copy(that);
    }

    virtual void Initialise(SDL_Renderer *p_renderer) {};

    void Start(SDL_Renderer* p_renderer) {
        Initialise(p_renderer);
        InteractionContainer::Start(p_renderer);
    };

    virtual void UpdateSpaces(const double &dt) {};

    virtual void RenderSpaces(const double &dt, const double &inter) {};
    virtual void RenderCameras(const double &dt, const double &inter) {};

    virtual void Update(const double &dt, const BiVector &mousePosition, const bool &mouseLeft, const bool &mouseRight) {
        if(this -> messageDisplayedSoFar >= 0) {
            this -> messageDisplayedSoFar += dt;
            if(this -> messageDisplayedSoFar > this -> DEFAULT_MESSAGE_TIME) {
                this -> messageDisplayedSoFar = -1.0f;
                this -> currentUiMessage = this -> currentUiPermanentMessage;
            }
        }

        if (this -> uiMessage != nullptr) {
            if(this -> uiMessage->getText().compare(this -> currentUiMessage)) {
                this -> uiMessage -> setText(this -> currentUiMessage);
            }
        }

        this -> UpdateSpaces(dt);

        InteractionContainer::Update(dt);
        this -> hoverOver(mousePosition, mouseLeft, mouseRight);
    };

    void Render(const double &dt, const double &inter) {

        this -> RenderSpaces(dt, inter);
        this -> RenderCameras(dt, inter);

        InteractionContainer::Render(dt, inter);
    };

    void Stop() {
        InteractionContainer::Stop();
    };

    BiVector getPosition() {
        return BiVector();
    }

    BiVector getMargin() {
        return BiVector();
    }

    BiVector getDimensions() {
        return BiVector(WORLD_WIDTH, WORLD_HEIGHT);
    }

    void Broadcast(int tag, std::string message) {
        this -> OnNotify(tag, message);
    }

    void OnNotify(int tag, std::string message) {
        bool treated = false;

        if(tag == NOTIFY_TAG_QUIT_APPLICATION) {
            this -> shouldQuitApplication = true;
            treated = true;
        } else if(tag == NOTIFY_TAG_SET_MESSAGE_TEMPORARY) {
            this -> setTemporaryUIMessage(message);
            treated = true;
        } else if(tag == NOTIFY_TAG_SET_MESSAGE_PERMANENT) {
            this -> setPermanentUIMessage(message);
            treated = true;
        }

        if(!treated) {
            InteractionContainer::OnNotify(tag, message);
        }
    }

    void setPermanentUIMessage(std::string p_message) {
        if(this -> uiMessage != nullptr) {
            this -> currentUiPermanentMessage = p_message;
            this -> currentUiMessage = p_message;
        }
    }

    void setTemporaryUIMessage(std::string p_message) {
        if(this -> uiMessage != nullptr) {
            this -> currentUiMessage = p_message;
            this -> messageDisplayedSoFar = 0.0f;
        }
    }

    void setRenderFps(std::string p_fps) {
        if(this -> renderFps != nullptr) {
            this -> renderFps->setText(p_fps);
        }
    }

    void setUpdateFps(std::string p_fps) {
        if(this -> updateFps != nullptr) {
            this -> updateFps->setText(p_fps);
        }
    }

    void verifyKeyboard(const double &dt, const Uint8 *keyboardState) {

    }
};



#endif // ENGINE_SCENE_H
