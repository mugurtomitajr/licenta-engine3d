//
// Created by Mugur Tomita Jr. on 19/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_BILINE_H
#define ENGINE_BILINE_H

#include "BiVector.h"

class BiLine {
public:
    BiVector source;
    BiVector destination;

    BiLine() {
        this -> source = BiVector();
        this -> destination = BiVector();
    }

    BiLine(BiVector p_source, BiVector p_destination) {
        this -> source = p_source;
        this -> destination = p_destination;
    }

    BiLine(const BiLine &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    BiLine &operator=(const BiLine &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~BiLine() {
        this->clear();
    }

    void clear() {

    }

    void copy(const BiLine &that) {
        this -> source = that.source;
        this -> destination = that.destination;
    }
};

#endif // ENGINE_BILINE_H
