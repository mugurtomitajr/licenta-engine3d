//
// Created by Mugur Tomita Jr. on 01/04/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_MATRIX_H
#define ENGINE_MATRIX_H

#include <cstring>
#include <cassert>

class Matrix {
private:
    float *mat = nullptr;

public:
    int height = 0;
    int width = 0;

    Matrix() {
        this -> height = 0;
        this -> width = 0;
    }

    Matrix(const int &p_height, const int &p_width) {
        this -> height = p_height;
        this -> width = p_width;

        if(this -> mat != nullptr) {
            delete[] this -> mat;
            this -> mat = nullptr;
        }

        int dim = this -> height * this -> width;
        this -> mat = new float[dim];
        std::memset(this -> mat, 0, sizeof(float) * dim);
    }

    Matrix(const int &p_height, const int &p_width, double *p_mat) {
        this -> height = p_height;
        this -> width = p_width;

        if(this -> mat != nullptr) {
            delete[] this -> mat;
            this -> mat = nullptr;
        }

        int dim = this -> height * this -> width;
        this -> mat = new float[dim];
        for(int i=0; i<this -> height; ++i){
            for(int j=0; j<this -> width; ++j) {
                this -> mat[i * this -> width + j] = p_mat[i*this -> width + j];
                //this -> mat[i * this -> width + j] = p_mat[i][j];
            }
        }
    }

    Matrix(const Matrix &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Matrix &operator=(const Matrix &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Matrix() {
        this->clear();
    }

    void clear() {
        if(this -> mat != nullptr) {
            delete[] this -> mat;
            this -> mat = nullptr;
        }
    }

    void copy(const Matrix &that) {
        this -> height = that.height;
        this -> width = that.width;

        if(this -> mat != nullptr) {
            delete[] this -> mat;
            this -> mat = nullptr;
        }

        int dim = this -> height * this -> width;
        this -> mat = new float[dim];
        std::memcpy(this -> mat, that.mat, sizeof(float) * dim);
    }

    double at(int p_x, int p_y) const {
        if(p_x >= this -> height || p_y >= this -> width || p_x < 0 || p_y < 0) {
            return 0;
        }
        return this -> mat[p_x * this -> width + p_y];
    }

    Matrix* set(int p_x, int p_y, double p_value) {
        if(p_x < this -> height && p_y < this -> width && p_x >= 0 && p_y >= 0) {
            this -> mat[p_x * this -> width + p_y] = p_value;
        }
        return this;
    }

    Matrix operator+(const Matrix &that) const {
        assert(this -> width == that.width && this -> height == that.width);

        Matrix result = Matrix(this -> height, this -> width);
        int position = 0;
        for(int i=0; i<result.height; ++i) {
            for(int j=0; j<result.width; ++j) {
                position = i * result.width + j;
                result.mat[position] = this->mat[position] + that.mat[position];
            }
        }
        return result;
    }

    Matrix& operator+=(const Matrix &that) {
        assert(this -> width == that.width && this -> height == that.width);
        int position = 0;
        for(int i=0; i < this -> height; ++i) {
            for(int j=0; j < this -> width; ++j) {
                position = i * this -> width + j;
                this -> mat[position] = this -> mat[position] + that.mat[position];
            }
        }
        return *this;
    }

    Matrix operator-(const Matrix &that) const {
        assert(this -> width == that.width && this -> height == that.width);

        Matrix result = Matrix(this -> height, this -> width);
        int position = 0;
        for(int i=0; i<result.height; ++i) {
            for(int j=0; j<result.width; ++j) {
                position = i * result.width + j;
                result.mat[position] = this->mat[position] - that.mat[position];
            }
        }
        return result;
    }

    Matrix& operator-=(const Matrix &that) {
        assert(this -> width == that.width && this -> height == that.width);
        int position = 0;
        for(int i=0; i < this -> height; ++i) {
            for(int j=0; j < this -> width; ++j) {
                position = i * this -> width + j;
                this -> mat[position] = this -> mat[position] - that.mat[position];
            }
        }
        return *this;
    }

    Matrix operator*(const Matrix &that) const {
        // TODO: OPTIMIZE Operators, no access through 'at' and 'set'
        assert(this -> width == that.height);
        int commonDimension = this -> width;

        Matrix result = Matrix(this -> height, that.width);
        double sum = 0.0f;
        for(int i=0; i<result.height; ++i) {
            for(int j=0; j<result.width; ++j) {
                sum = 0.0f;
                for(int k=0; k<commonDimension; ++k) {
                    sum += this -> at(i, k) * that.at(k, j);
                }
                result.set(i, j, sum);
            }
        }

        return result;
    }

    Matrix& operator*=(const Matrix &that) {
        assert(this -> width == this -> height && that.width == that.height && this->width == that.width);
        double sum = 0.0f;
        for(int i=0; i< this -> height; ++i) {
            for(int j=0; j< this -> width; ++j) {
                sum = 0.0f;
                for(int k=0; k<this -> width; ++k) {
                    sum += this -> at(i, k) * that.at(k, j);
                }
                this -> set(i, j, sum);
            }
        }

        return *this;
    }

    Matrix operator*(float value) const {
        Matrix result = Matrix(this -> height, this -> width);
        for(int i=0; i<result.height; ++i) {
            for(int j=0; j<result.width; ++j) {
                result.mat[i*result.width + j] = this -> mat[i*this -> width + j] * value;
            }
        }
        return result;
    }

    Matrix& operator*=(float value) {
        for(int i=0; i<this -> height; ++i) {
            for(int j=0; j<this -> width; ++j) {
                this -> mat[i * this -> width + j] *= value;
            }
        }
        return *this;
    }

    Matrix operator/(float value) const {
        assert(value != 0.0f);
        Matrix result = Matrix(this -> height, this -> width);
        for(int i=0; i<result.height; ++i) {
            for(int j=0; j<result.width; ++j) {
                result.mat[i*result.width + j] = this -> mat[i*this -> width + j] / value;
            }
        }
        return result;
    }

    Matrix& operator/=(float value) {
        assert(value != 0.0f);
        for(int i=0; i<this -> height; ++i) {
            for(int j=0; j<this -> width; ++j) {
                this -> mat[i * this -> width + j] /= value;
            }
        }
        return *this;
    }

    bool operator==(Matrix &that) const {
        if(this -> height != that.height || this -> width != that.width) {
            return false;
        }
        for(int i=0; i<this -> height; ++i) {
            for(int j=0; j<this -> width; ++j) {
                if(this -> mat[i * this -> width + j] != that.mat[i * that.width + j]) {
                    return false;
                }
            }
        }
        return true;
    }

    std::string toString() {
        std::string result = "Matrix : [";
        result += std::to_string(this->height);
        result += "] [";
        result += std::to_string(this->width);
        result += "] : \n";
        for(int i=0; i<this->height; ++i) {
            for(int j=0; j<this->width; ++j) {
                result += std::to_string(this->at(i, j));
                result += " ";
            }
            result += "\n";
        }
        return result;
    }

    static Matrix Unit(int size) {
        Matrix result = Matrix(size, size);
        for(int i=0; i<size; ++i) {
            result.mat[i * size + i] = 1.0f;
        }
        return result;
    }
};

#endif // ENGINE_MATRIX_H
