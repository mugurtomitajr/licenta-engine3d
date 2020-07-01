
//
// Created by Mugur Tomita Jr. on 15/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//


#ifndef ENGINE_BIVECTOR_H
#define ENGINE_BIVECTOR_H

#include <cmath>
#include <string>
#include <cstdio>
#include <algorithm>

#include "MathConstants.h"

class BiVector {
public:
    double x;
    double y;

    BiVector() {
        this -> x = 0.0f;
        this -> y = 0.0f;
    }

    BiVector(double p_x, double p_y) {
        this -> x = p_x;
        this -> y = p_y;
    }

    BiVector(const BiVector &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    BiVector& operator=(const BiVector &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~BiVector() {
        this->clear();
    }

    void clear() {

    }

    void copy(const BiVector &that) {
        this -> x = that.x;
        this -> y = that.y;
    }

    int intX() const { return (int) ceil(this -> x); }
    int intY() const { return (int) ceil(this -> y); }

    double square() const { return x * x + y * y; }
    double length() const { return sqrt(this -> square()); }

    BiVector selfNormalize() {
        float lg = this -> length();
        if(lg != 0) {
            *this /= lg;
        }
        return *this;
    }

    BiVector normal() const {
        float lg = this -> length();
        if(lg != 0) {
            return BiVector(this -> x / lg, this -> y / lg);
        }
        return BiVector(0.0f, 0.0f);
    }

    bool isBetween(const BiVector &thatFirst, const BiVector &thatSecond) const {
        double minX = std::min(thatFirst.x, thatSecond.x);
        double maxX = std::max(thatFirst.x, thatSecond.x);

        double minY = std::min(thatFirst.y, thatSecond.y);
        double maxY = std::max(thatFirst.y, thatSecond.y);

        return (this->x <= maxX && this->x >= minX) && (this->y <= maxY && this->y >= minY);
    }

    bool isZero() {
        return this -> x == 0 && this -> y == 0;
    }

    double angleTo(const BiVector &that) const {
        return atan2(that.y, that.x) - atan2(this -> y, this -> x);
    }

    BiVector rotate(const BiVector &origin, double rad) const {
        while(rad >= PI_8o4) { rad -= PI_8o4; }

        BiVector difference = (*this) - origin;

        double sinR = sin(rad);
        double cosR = cos(rad);

        return BiVector(
                cosR * difference.x - sinR * difference.y + origin.x,
                sinR * difference.x + cosR * difference.y + origin.y);
    }

    BiVector selfRotate(const BiVector &origin, double rad) {
        while(rad >= PI_8o4) { rad -= PI_8o4; }

        BiVector difference = (*this) - origin;

        double sinR = sin(rad);
        double cosR = cos(rad);

        this -> x = cosR * difference.x - sinR * difference.y + origin.x;
        this -> y = sinR * difference.x + cosR * difference.y + origin.y;

        return (*this);
    }

    BiVector mirror(const BiVector &origin) const {
        return this -> rotate(origin, PI);
    }

    BiVector selfMirror(const BiVector &origin) {
        return this -> selfRotate(origin, PI);
    }

    BiVector operator+(const double &that) const {
        return BiVector(this->x + that, this->y + that);
    }

    BiVector& operator+=(const double &that) {
        this -> x += that;
        this -> y += that;
        return *this;
    }

    BiVector operator+(const BiVector &that) const {
        return BiVector(this->x + that.x, this->y + that.y);
    }

    BiVector& operator+=(const BiVector &that) {
        this -> x += that.x;
        this -> y += that.y;
        return *this;
    }

    BiVector operator-(const double &that) const {
        return BiVector(this -> x - that, this -> y - that);
    }

    BiVector& operator-=(const double &that) {
        this->x -= that;
        this->y -= that;
        return *this;
    }

    BiVector operator-(const BiVector &that) const {
        return BiVector(this -> x - that.x, this -> y - that.y);
    }

    BiVector& operator-=(const BiVector &that) {
        this->x -= that.x;
        this->y -= that.y;
        return *this;
    }

    BiVector operator*(double value) const {
        return BiVector(this -> x * value, this -> y * value);
    }

    BiVector& operator*=(double value) {
        this -> x *= value;
        this -> y *= value;
        return *this;
    }

    double operator*(const BiVector &that) const {
        return this->x * that.x + this->y * that.y;
    }

    double operator&(const BiVector &that) const {
        return this->x * that.y - this->y * that.x;
    }

    BiVector operator/(double value) const {
        if(value != 0) {
            return BiVector(this -> x / value, this -> y / value);
        }
        return BiVector(0, 0);
    }

    BiVector& operator/=(double value) {
        if(value != 0) {
            this -> x /= value;
            this -> y /= value;
        }
        return *this;
    }

    bool operator==(BiVector &that) const {
        return this->x == that.x && this->y == that.y;
    }

    bool operator>(BiVector &that) const {
        return this->length() > that.length();
    }

    bool operator<(BiVector &that) const {
        return this->length() < that.length();
    }

    bool operator>=(BiVector &that) const {
        return (*this) > that || (*this) == that;
    }

    bool operator<=(BiVector &that) const {
        return (*this) < that || (*this) == that;
    }

    std::string toString() const {
        std::string result = "BiVector : x = ";
        result += std::to_string(this->x);
        result += "; y = ";
        result += std::to_string(this->y);
        result += ";";
        return result;
    }

    static BiVector OX() {
        return BiVector(1.0f, 0.0f);
    }

    static BiVector OY() {
        return BiVector(0.0f, 1.0f);
    }
};

#endif // ENGINE_BIVECTOR_H
