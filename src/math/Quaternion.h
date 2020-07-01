//
// Created by Mugur Tomita Jr. on 06/04/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_QUATERNION_H
#define ENGINE_QUATERNION_H

class Quaternion {
private:
    double x, y, z;
    double w;
public:
    Quaternion() {
        this -> w = 0.0f;
        this -> x = 0.0f;
        this -> y = 0.0f;
        this -> z = 0.0f;
    }

    Quaternion(const double &p_w, const double &p_x, const double &p_y, const double &p_z) {
        this -> w = p_w;
        this -> x = p_x;
        this -> y = p_y;
        this -> z = p_z;
    }

    Quaternion(const double &p_w, const TriVector &p_vector) {
        this -> w = p_w;
        this -> x = p_vector.x;
        this -> y = p_vector.y;
        this -> z = p_vector.z;
    }

    Quaternion(const Quaternion &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Quaternion &operator=(const Quaternion &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Quaternion() {
        this->clear();
    }

    void clear() {

    }

    void copy(const Quaternion &that) {
        this -> x = that.x;
        this -> y = that.y;
        this -> z = that.z;
        this -> w = that.w;
    }

    double square() const { return w * w + x * x + y * y + z * z; }
    double length() const { return sqrt(this -> square()); }

    Quaternion conjugate() const {
        return Quaternion(this -> w, -this -> x, -this -> y, -this -> z);
    }

    Quaternion& selfConjugate() {
        this -> x *= -1;
        this -> y *= -1;
        this -> z *= -1;
        return *this;
    }

    Quaternion inverse() const {
        return this -> conjugate() / this -> square();
    }

    Quaternion& selfInvert() {
        this -> selfConjugate();
        *this /= this -> square();
        return *this;
    }

    Quaternion normal() const {
        return (*this) / this -> length();
    }

    Quaternion& selfNormalize() {
        *this /= this -> length();
        return *this;
    }

    Quaternion operator+(const Quaternion &that) const {
        return Quaternion(this -> w + that.w, this->x + that.x, this->y + that.y, this->z + that.z);
    }

    Quaternion& operator+=(const Quaternion &that) {
        this -> w += that.w;
        this -> x += that.x;
        this -> y += that.y;
        this -> z += that.z;
        return *this;
    }

    Quaternion operator-(const Quaternion &that) const {
        return Quaternion(this -> w - that.w, this -> x - that.x, this -> y - that.y, this -> z - that.z);
    }

    Quaternion& operator-=(const Quaternion &that) {
        this -> w -= that.w;
        this -> x -= that.x;
        this -> y -= that.y;
        this -> z -= that.z;
        return *this;
    }

    Quaternion operator*(double value) const {
        return Quaternion(this -> w * value, this -> x * value, this -> y * value, this -> z * value);
    }

    Quaternion& operator*=(double value) {
        this -> w *= value;
        this -> x *= value;
        this -> y *= value;
        this -> z *= value;
        return *this;
    }

    Quaternion operator*(const Quaternion &that) const {
        return Quaternion(
                this->w*that.w - this->x*that.x - this->y*that.y - this->z*that.z,
                this->w*that.x + this->x*that.w + this->y*that.z - this->z*that.y,
                this->w*that.y + this->y*that.w + this->z*that.x - this->x*that.z,
                this->w*that.z + this->z*that.w + this->x*that.y - this->y*that.x);
    }

    Quaternion&operator*=(const Quaternion &that) {
        this -> w = this->w*that.w - this->x*that.x - this->y*that.y - this->z*that.z;
        this -> x = this->w*that.x + this->x*that.w + this->y*that.z - this->z*that.y;
        this -> y = this->w*that.y + this->y*that.w + this->z*that.x - this->x*that.z;
        this -> z = this->w*that.z + this->z*that.w + this->x*that.y - this->y*that.x;
        return *this;
    }

    Quaternion operator/(double value) const {
        return Quaternion(this -> w / value, this -> x / value, this -> y / value, this -> z / value);
    }

    Quaternion& operator/=(double value) {
        this -> w /= value;
        this -> x /= value;
        this -> y /= value;
        this -> z /= value;
        return *this;
    }

    Quaternion operator/(const Quaternion &that) const {
        return ((*this) * (that.inverse()));
    }

    Quaternion& operator/=(const Quaternion &that) {
        (*this) = (*this)*that.inverse();
        return *this;
    }

    bool operator==(Quaternion &that) const {
        return this->w == that.w && this->x == that.x && this->y == that.y && this->z == that.z;
    }

    bool operator>(Quaternion &that) const {
        return this->length() > that.length();
    }

    bool operator<(Quaternion &that) const {
        return this->length() < that.length();
    }

    bool operator>=(Quaternion &that) const {
        return (*this) > that || (*this) == that;
    }

    bool operator<=(Quaternion &that) const {
        return (*this) < that || (*this) == that;
    }

    TriVector rotate(const TriVector &p_vector) const {
        Quaternion aux = Quaternion(0, p_vector);
        Quaternion result = (*this) * aux * ((*this).inverse());

        return TriVector(result.x, result.y, result.z);
    }

    std::string toString() const {
        std::string result = "Q : (";
        result += std::to_string(this->w);
        result += ", <";
        result += std::to_string(this->x);
        result += ", ";
        result += std::to_string(this->y);
        result += ", ";
        result += std::to_string(this->z);
        result += ">)";
        return result;
    }
    
};

#endif // ENGINE_QUATERNION_H
