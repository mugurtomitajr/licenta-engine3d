//
// Created by Mugur Tomita Jr. on 04/04/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_PLANE_H
#define ENGINE_PLANE_H

class Plane {
private:

public:
    TriVector point;
    TriVector normal;

    Plane() {
        this -> point = TriVector();
        this -> normal = TriVector();
    }

    Plane(const TriVector &p_point, const TriVector &p_normal) {
        this -> point = p_point;
        this -> normal = p_normal;
    }

    Plane(const Plane &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Plane &operator=(const Plane &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Plane() {
        this->clear();
    }

    void clear() {

    }

    void copy(const Plane &that) {
        this -> point = that.point;
        this -> normal = that.normal;
    }

    double positionOfPoint(const TriVector p_point) const {
        return (this -> normal * p_point) - (this -> normal * this -> point);
    }
};

#endif // ENGINE_PLANE_H
