//
// Created by Mugur Tomita Jr. on 23/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_MENU_H
#define ENGINE_MENU_H

#include "../../../base/model/InteractionContainer.h"
#include "MenuItem.h"
#include "../../../base/model/Button.h"

const BiLine sMargin = BiLine(BiVector(20, 10), BiVector(20, 20));

class Menu : public InteractionContainer {
private:
    std::vector<MenuItem*> items;

    bool menuUpdated = false;

public:
    Menu() : InteractionContainer(FlexType::Row, ExpandType::Wrap, GrowType::Grow, BiVector(), sMargin, SCREEN_OFFSET_COLOR) {

    }

    void addMenuItem(MenuItem *menuItem) {
        this -> items.push_back(menuItem);
        this -> menuUpdated = true;
    }

    void initChildren() {
        for (auto &it : this -> items) {
            if(it != nullptr){
                this->addChild((new Button(
                        BiLine(BiVector(20, 10), BiVector(0, 10)),
                        SCREEN_BACKGROUND_COLOR,
                        Color::WHITE(),
                        APPLICATION_COLOR_BRIGHT_A_BIT_DARKER,
                        (it -> label).c_str(),
                        42,
                        BiLine(BiVector(20, 10), BiVector(20, 10)),
                        SCREEN_OFFSET_COLOR ))
                                                  ->setOnLeftClick(it -> leftContext, it->onLeftClick, it->id)
                                                  ->setOnRightClick(it -> rightContext, it->onRightClick, it->id));
            }
        }
    }

    virtual void Start(SDL_Renderer *p_renderer) {
        InteractionContainer::Start(p_renderer);
        this -> initChildren();
    };

    void clearItems() {
        for (auto &it : this -> items) {
            if(it != nullptr){
                delete it;
                it = nullptr;
            }
        }
        this -> items = std::vector<MenuItem*>();
        this -> menuUpdated = true;
    }

    Menu(const Menu &that) : InteractionContainer(that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Menu &operator=(const Menu &that) {
        InteractionContainer::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Menu() {
        this->clear();
    }

    void clear() {
        InteractionContainer::clear();
        this -> clearItems();
    }

    void copy(const Menu &that) {
        InteractionContainer::copy(that);
        this -> items = that.items;
        this -> menuUpdated = that.menuUpdated;
    }

    void Update(double dt) {
        if(this -> menuUpdated) {
            this -> menuUpdated = false;
            this -> deleteChildren();
            this -> initChildren();
        }
        InteractionContainer::Update(dt);
    };
};

#endif // ENGINE_MENU_H
