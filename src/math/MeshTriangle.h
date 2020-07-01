//
// Created by Mugur Tomita Jr. on 02/04/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_MESHTRIANGLE_H
#define ENGINE_MESHTRIANGLE_H

#include "TriVector.h"

class MeshTriangle {
private:
    bool ownsHisPoints = false;

public:
    TriVector *point[3];
    double illumination = 0.0f;

    TriVector* normalPoint;
    TriVector* centerPoint;

    MeshTriangle() {
        this -> point[0] = nullptr;
        this -> point[1] = nullptr;
        this -> point[2] = nullptr;
        this -> ownsHisPoints = false;
    }

    MeshTriangle(TriVector *p_p0, TriVector *p_p1, TriVector *p_p2) {
        this -> point[0] = p_p0;
        this -> point[1] = p_p1;
        this -> point[2] = p_p2;
        this -> ownsHisPoints = false;
    }

    MeshTriangle(TriVector p_p0, TriVector p_p1, TriVector p_p2) {
        this -> point[0] = new TriVector(p_p0);
        this -> point[1] = new TriVector(p_p1);
        this -> point[2] = new TriVector(p_p2);
        this -> ownsHisPoints = true;
    }

    MeshTriangle(const MeshTriangle &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    MeshTriangle &operator=(const MeshTriangle &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~MeshTriangle() {
        this->clear();
    }

    void clear() {
        if(this -> ownsHisPoints) {
            if(this -> point[0] != nullptr) {
                delete this -> point[0];
                this -> point[0] = nullptr;
            }
            if(this -> point[1] != nullptr) {
                delete this -> point[1];
                this -> point[1] = nullptr;
            }
            if(this -> point[2] != nullptr) {
                delete this -> point[2];
                this -> point[2] = nullptr;
            }
            if(this -> normalPoint != nullptr) {
                delete this -> normalPoint;
                this -> normalPoint = nullptr;
            }
            if(this -> centerPoint != nullptr) {
                delete this -> centerPoint;
                this -> centerPoint = nullptr;
            }
        }
    }

    void copy(const MeshTriangle &that) {
        this -> point[0] = that.point[0];
        this -> point[1] = that.point[1];
        this -> point[2] = that.point[2];
        this -> illumination = that.illumination;
        this -> normalPoint = that.normalPoint;
        this -> centerPoint = that.centerPoint;
    }

    TriVector center() const {
        return TriVector(this -> midX(), this -> midY(), this -> midZ());
    }

    TriVector normal() const {
        return TriVector(((*(this -> point[1]) - *(this -> point[0])) & (*(this -> point[2]) - *(this -> point[0]))).normal());
    }

    double midX() const {
        return (point[0] -> x + point[1] -> x + point[2] -> x) / 3.0f;
    }

    double midY() const {
        return (point[0] -> y + point[1] -> y + point[2] -> y) / 3.0f;
    }

    double midZ() const {
        return (point[0] -> z + point[1] -> z + point[2] -> z) / 3.0f;
    }

    MeshTriangle sortedByY() const { // TODO: Check if works
        MeshTriangle sorted = MeshTriangle(this -> point[0], this -> point[1], this -> point[2]);
        auto swap = [](TriVector *x, TriVector *y) { TriVector *t = x; x = y; y = t; };

        if(sorted.point[0] -> y > sorted.point[1] -> y) {
            swap(sorted.point[0], sorted.point[1]);
        }

        if(sorted.point[1] -> y > sorted.point[2] -> y) {
            swap(sorted.point[1], sorted.point[2]);
        }

        if(sorted.point[0] -> y > sorted.point[1] -> y) {
            swap(sorted.point[0], sorted.point[1]);
        }

        if(sorted.point[0] -> y <= sorted.point[1] -> y && sorted.point[1] -> y <= sorted.point[2] -> y) {
            std::cout << "OKAY!\n";
        } else {
            std::cout << "Fail!!!";
        }

        return sorted;
    }

    std::string toString() const {
        std::string result = "MeshTriangle : \np0 = ";
        result += this -> point[0] -> toString();
        result += ";\np1 = ";
        result += this -> point[1] -> toString();
        result += ";\np2 = ";
        result += this -> point[2] -> toString();
        result += ";\ncenter = ";
        result += this -> center().toString();
        result += ";\nnormal = ";
        result += this -> normal().toString();
        result += ";\n";
        return result;
    }
};

#endif // ENGINE_MESHTRIANGLE_H
