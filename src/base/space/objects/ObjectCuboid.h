//
// Created by Mugur Tomita Jr. on 03/04/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_OBJECTCUBOID_H
#define ENGINE_OBJECTCUBOID_H

#include "../SpaceObject.h"

class ObjectCuboid : public SpaceObject {
private:
    TriVector size;

public:
    ObjectCuboid(SpaceObject* p_parent, const TriVector &p_position, const TriVector &p_size) : SpaceObject(p_parent, p_position) {
        this -> size = p_size;

        TriVector *p0 = new TriVector( - this->size.x / 2.0f, - this->size.y / 2.0f, - this->size.z / 2.0f);
        TriVector *p1 = new TriVector( - this->size.x / 2.0f, - this->size.y / 2.0f, this->size.z / 2.0f);
        TriVector *p2 = new TriVector( - this->size.x / 2.0f, this->size.y / 2.0f, - this->size.z / 2.0f);
        TriVector *p3 = new TriVector( - this->size.x / 2.0f, this->size.y / 2.0f, this->size.z / 2.0f);

        TriVector *p4 = new TriVector( this->size.x / 2.0f, - this->size.y / 2.0f, - this->size.z / 2.0f);
        TriVector *p5 = new TriVector( this->size.x / 2.0f, - this->size.y / 2.0f, this->size.z / 2.0f);
        TriVector *p6 = new TriVector( this->size.x / 2.0f, this->size.y / 2.0f, - this->size.z / 2.0f);
        TriVector *p7 = new TriVector( this->size.x / 2.0f, this->size.y / 2.0f, this->size.z / 2.0f);

        this -> addControls((TriVector*[]){p0, p1, p2, p3, p4, p5, p6, p7}, 8);

        this -> addMesh(MeshTriangle(p0, p2, p6));
        this -> addMesh(MeshTriangle(p0, p6, p4));

        this -> addMesh(MeshTriangle(p4, p6, p7));
        this -> addMesh(MeshTriangle(p4, p7, p5));

        this -> addMesh(MeshTriangle(p5, p7, p3));
        this -> addMesh(MeshTriangle(p5, p3, p1));

        this -> addMesh(MeshTriangle(p1, p3, p2));
        this -> addMesh(MeshTriangle(p1, p2, p0));

        this -> addMesh(MeshTriangle(p2, p3, p7));
        this -> addMesh(MeshTriangle(p2, p7, p6));

        this -> addMesh(MeshTriangle(p5, p1, p0));
        this -> addMesh(MeshTriangle(p5, p0, p4));
    }

    ObjectCuboid(const ObjectCuboid &that) : SpaceObject(that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    ObjectCuboid &operator=(const ObjectCuboid &that) {
        SpaceObject::operator=(that);
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~ObjectCuboid() {
        this->clear();
    }

    void clear() {
        SpaceObject::clear();
    }

    void copy(const ObjectCuboid &that) {
        SpaceObject::copy(that);
        this -> size = that.size;
    }
};

#endif // ENGINE_OBJECTCUBOID_H
