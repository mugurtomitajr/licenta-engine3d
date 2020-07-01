//
// Created by Mugur Tomita Jr. on 16/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_TRIVECTOR_H
#define ENGINE_TRIVECTOR_H

#include "Matrix.h"
#include <cmath>

class TriVector {
public:
    double x;
    double y;
    double z;

    float extend = 1.0f; // complex operations

    bool helper = false;
    bool used = false;
    bool valid = false;

    TriVector() {
        this -> x = 0.0f;
        this -> y = 0.0f;
        this -> z = 0.0f;
        this -> helper = false;
        this -> used = false;
        this -> valid = false;
    }

    TriVector(double p_x, double p_y, double p_z) {
        this -> x = p_x;
        this -> y = p_y;
        this -> z = p_z;
        this -> helper = false;
        this -> used = false;
        this -> valid = false;
    }

    TriVector(double p_x, double p_y, double p_z, double p_extend) {
        this -> x = p_x;
        this -> y = p_y;
        this -> z = p_z;
        this -> extend = p_extend;
        this -> helper = false;
        this -> used = false;
        this -> valid = false;
    }

    TriVector(const TriVector &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    TriVector &operator=(const TriVector &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~TriVector() {
        this->clear();
    }

    virtual void clear() {

    }

    virtual void copy(const TriVector &that) {
        this -> x = that.x;
        this -> y = that.y;
        this -> z = that.z;
        this -> extend = that.extend;
        this -> helper = that.helper;
        this -> used = that.helper;
        this -> valid = that.helper;
    }

    int intX() const { return (int) ceil(this -> x); }
    int intY() const { return (int) ceil(this -> y); }
    int intZ() const { return (int) ceil(this -> z); }

    double square() const { return x * x + y * y + z * z; }
    double length() const { return sqrt(this -> square()); }

    bool isZero() { return this -> x == 0 && this -> y == 0 && this -> z == 0; }

    TriVector& selfNormalize() {
        double lg = this -> length();
        if(lg != 0) {
            *this /= lg;
        }
        return *this;
    }

    TriVector normal() const {
        double lg = this -> length();
        if(lg != 0) {
            return TriVector(this -> x / lg, this -> y / lg, this -> z / lg);
        }
        return TriVector(0.0f, 0.0f, 0.0f);
    }
    
    double distance(const TriVector &that) const { return (*this - that).length(); }

    double angleTo(const TriVector &that) const {
        return acos(*this * that / this -> length() * that.length());
    }

    TriVector overExtend() const {
        if(this -> extend != 0.0f) {
            return TriVector(this -> x / this -> extend, this -> y / this -> extend, this -> z / this -> extend, 1.0f);
        }
        return TriVector(0, 0, 0, 1);
    }

    TriVector& selfOverExtend(bool preserveZ) {
        if(this -> extend != 0.0f) {
            this -> x /= this -> extend;
            this -> y /= this -> extend;
            if(!preserveZ) {
                this->z /= this->extend;
            }
        } else {
            this -> x = 0;
            this -> y = 0;
            this -> z = 0;
        }
        this -> extend = 1.0f;
        return *this;
    }

    TriVector operator+(const double &that) const {
        return TriVector(this->x + that, this->y + that, this->z + that);
    }

    TriVector& operator+=(const double &that) {
        this -> x += that;
        this -> y += that;
        this -> z += that;
        return *this;
    }

    TriVector operator+(const TriVector &that) const {
        return TriVector(this->x + that.x, this->y + that.y, this->z + that.z);
    }

    TriVector& operator+=(const TriVector &that) {
        this -> x += that.x;
        this -> y += that.y;
        this -> z += that.z;
        return *this;
    }

    TriVector operator-(const double &that) const {
        return TriVector(this -> x - that, this -> y - that, this -> z - that);
    }

    TriVector& operator-=(const double &that) {
        this -> x -= that;
        this -> y -= that;
        this -> z -= that;
        return *this;
    }

    TriVector operator-(const TriVector &that) const {
        return TriVector(this -> x - that.x, this -> y - that.y, this -> z - that.z);
    }

    TriVector& operator-=(const TriVector &that) {
        this -> x -= that.x;
        this -> y -= that.y;
        this -> z -= that.z;
        return *this;
    }

    TriVector operator*(double value) const {
        return TriVector(this -> x * value, this -> y * value, this -> z * value);
    }

    TriVector& operator*=(double value) {
        this -> x *= value;
        this -> y *= value;
        this -> z *= value;
        return *this;
    }

    double operator*(const TriVector &that) const {
        return this->x * that.x + this->y * that.y + this->z * that.z;
    }

    TriVector operator*(const Matrix &transform) const {
        Matrix thisMatrix = Matrix(1, 4);
        thisMatrix.set(0, 0, this -> x);
        thisMatrix.set(0, 1, this -> y);
        thisMatrix.set(0, 2, this -> z);
        thisMatrix.set(0, 3, this -> extend);

        Matrix result = thisMatrix * transform;
        return TriVector(
                result.at(0, 0),
                result.at(0, 1),
                result.at(0, 2),
                result.at(0, 3));
    }

    TriVector& operator*=(const Matrix &transform) {
        Matrix thisMatrix = Matrix(1, 4);
        thisMatrix.set(0, 0, this -> x);
        thisMatrix.set(0, 1, this -> y);
        thisMatrix.set(0, 2, this -> z);
        thisMatrix.set(0, 3, this -> extend);

        Matrix result = thisMatrix * transform;
        this -> x = result.at(0, 0);
        this -> y = result.at(0, 1);
        this -> z = result.at(0, 2);
        this -> extend = result.at(0, 3);
        return *this;
    }

    TriVector operator&(const TriVector &that) const {
        double ni = this -> y * that.z - this -> z * that.y;
        double nj = this -> z * that.x - this -> x * that.z;
        double nk = this -> x * that.y - this -> y * that.x;
        return TriVector(ni, nj, nk);
    }

    TriVector operator/(double value) const {
        if(value != 0) {
            return TriVector(this -> x / value, this -> y / value, this -> z / value);
        }
        return TriVector(0, 0, 0);
    }

    TriVector& operator/=(double value) {
        if(value != 0) {
            this -> x /= value;
            this -> y /= value;
            this -> z /= value;
        }
        return *this;
    }

    bool operator==(TriVector &that) const {
        return this->x == that.x && this->y == that.y && this->z == that.z;
    }

    bool operator>(TriVector &that) const {
        return this->length() > that.length();
    }

    bool operator<(TriVector &that) const {
        return this->length() < that.length();
    }

    bool operator>=(TriVector &that) const {
        return (*this) > that || (*this) == that;
    }

    bool operator<=(TriVector &that) const {
        return (*this) < that || (*this) == that;
    }

    std::string toString() const {
        std::string result = "V : <";
        result += std::to_string(this->x);
        result += ", ";
        result += std::to_string(this->y);
        result += ", ";
        result += std::to_string(this->z);
        result += "> extends <";
        result += std::to_string(this->extend);
        result += ">";
        return result;
    }

    static TriVector OX() {
        return TriVector(1.0f, 0.0f, 0.0f);
    }

    static TriVector OY() {
        return TriVector(0.0f, 1.0f, 0.0f);
    }

    static TriVector OZ() {
        return TriVector(0.0f, 0.0f, 1.0f);
    }
    
};

#endif // ENGINE_TRIVECTOR_H
