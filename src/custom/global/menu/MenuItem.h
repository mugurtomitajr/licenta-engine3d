//
// Created by Mugur Tomita Jr. on 23/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_MENUITEM_H
#define ENGINE_MENUITEM_H

class MenuItem {
public:
    std::string label;
    std::string id;
    void (AliveObject::*onLeftClick) (std::string) = nullptr;
    void (AliveObject::*onRightClick) (std::string) = nullptr;

    AliveObject *leftContext;
    AliveObject *rightContext;

    MenuItem(std::string p_id, std::string p_label) {
        this -> id = p_id;
        this -> label = p_label;
    }

    MenuItem* setOnLeftClick(AliveObject *p_context, void (AliveObject::*p_onLeftClick) (std::string)) {
        this -> onLeftClick = p_onLeftClick;
        this -> leftContext = p_context;
        return this;
    }

    MenuItem* setOnRightClick(AliveObject *p_context, void (AliveObject::*p_onRightClick) (std::string)) {
        this -> onRightClick = p_onRightClick;
        this -> rightContext = p_context;
        return this;
    }

    MenuItem(const MenuItem &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    MenuItem &operator=(const MenuItem &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~MenuItem() {
        this->clear();
    }

    void clear() {

    }

    void copy(const MenuItem &that) {
        this -> id = that.id;
        this -> label = that.label;
        this -> onLeftClick = that.onLeftClick;
        this -> onRightClick = that.onRightClick;
        this -> leftContext = that.leftContext;
        this -> rightContext = that.rightContext;
    }
};

#endif // ENGINE_MENUITEM_H
