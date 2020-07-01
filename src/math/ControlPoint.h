//
// Created by Mugur Tomita Jr. on 03/04/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_CONTROLPOINT_H
#define ENGINE_CONTROLPOINT_H

#include "TriVector.h"

class ControlPoint : public TriVector {
private:

public:
    ControlPoint() : TriVector() {

    }

    ControlPoint(double p_x, double p_y, double p_z) : TriVector(p_x, p_y, p_z) {

    }

    ControlPoint(const ControlPoint &that) : TriVector(that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    ControlPoint &operator=(const ControlPoint &that) {
        TriVector::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~ControlPoint() {
        this->clear();
    }

    void clear() {
        TriVector::clear();
    }

    void copy(const ControlPoint &that) {
        TriVector::copy(that);
    }
};

#endif // ENGINE_CONTROLPOINT_H
