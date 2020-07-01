//
// Created by Mugur Tomita Jr. on 01/04/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_SPACEOBJECT_H
#define ENGINE_SPACEOBJECT_H

#include "../../math/TriVector.h"
#include "../../math/MeshTriangle.h"


#include "../interaction/Importer.h"
#include <vector>

class SpaceObject {
private:
    SpaceObject *parent = nullptr;

    TriVector position;
    TriVector speed;

    std::vector<SpaceObject*> children;

    std::vector<TriVector*> control;
    std::vector<MeshTriangle> mesh;

    std::string filename;
public:
    SpaceObject(SpaceObject* p_parent, const TriVector &p_position) {
        this -> parent = p_parent;
        this -> position = p_position;
        //this -> speed = TriVector(10, 0, -5);

        this -> control = std::vector<TriVector*>();
        this -> children = std::vector<SpaceObject*>();
        this -> mesh = std::vector<MeshTriangle>();
    }

    SpaceObject(SpaceObject* p_parent, const TriVector &p_position, const std::string p_filename, const double &scale) {
        this -> parent = p_parent;
        this -> position = p_position;
        this -> filename = p_filename;

        this -> control = std::vector<TriVector*>();
        this -> children = std::vector<SpaceObject*>();
        this -> mesh = std::vector<MeshTriangle>();

        this -> loadFromFile(true, scale);
    }

    SpaceObject(const SpaceObject &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    SpaceObject &operator=(const SpaceObject &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~SpaceObject() {
        this->clear();
    }

    virtual void clear() {
        for (auto &it : this -> control) {
            delete it;
            it = nullptr;
        }

        for (auto &it : this -> children) {
            delete it;
            it = nullptr;
        }
    }

    virtual void copy(const SpaceObject &that) {
        this -> parent = that.parent;
        this -> position = that.position;

        this -> control = that.control;
        this -> mesh = that.mesh;
        this -> children = that.children;
    }

    void addControl(TriVector *p_control) {
        this -> control.push_back(p_control);
    }

    void addControls(TriVector* p_control[], int p_len) {
        for (int i=0; i<p_len;++i){
            this -> control.push_back(p_control[i]);
        }
    }

    void addMesh(const MeshTriangle &p_mesh) {
        this -> mesh.push_back(p_mesh);
    }

    void addChild(SpaceObject *p_child) {
        p_child -> parent = this;
        this -> children.push_back(p_child);
    }

    void deleteChildren() {
        for (auto &it : this -> children) {
            if(it != nullptr){
                delete it;
                it = nullptr;
            }
        }
        this -> children = std::vector<SpaceObject*>();
    }

    virtual void OnNotify(int tag, std::string message) {
        for (auto &it : this -> children) {
            it -> OnNotify(tag, message);
        }
    };

    virtual void Broadcast(int tag, std::string message) {
        if (this -> parent != nullptr) {
            this -> parent -> Broadcast(tag, message);
        }
    }

    void loadFromFile(bool clearExisting, const double &scale) {
        if(clearExisting) {
            for (auto &it : this->control) {
                delete it;
                it = nullptr;
            }
            this -> control = std::vector<TriVector*>();
            this -> mesh = std::vector<MeshTriangle>();
        }

        if(this -> filename .length() > 0) {
            importer::OBJFile(this -> filename, scale, this -> control, this -> mesh);
        }
    }

    void loadControlMesh(std::vector<TriVector*> &p_control, std::vector<MeshTriangle> &p_mesh) {
        // TODO: Optimize
        std::vector<MeshTriangle> local_mesh = std::vector<MeshTriangle>(this -> mesh);
        for(auto &controlPoint : this -> control) {
            TriVector *newControl = new TriVector(*controlPoint + this -> position);
            p_control.push_back(newControl);

            for(auto &meshTriangle : local_mesh) {
                for(int i=0; i<3; ++i) {
                    if(meshTriangle.point[i] == controlPoint) {
                        meshTriangle.point[i] = newControl;
                        break;
                    }
                }
            }
        }
        p_mesh.insert(std::end(p_mesh), std::begin(local_mesh), std::end(local_mesh));
        for(auto &child : this -> children) {
            child -> loadControlMesh(p_control, p_mesh);
        }
    }

    void Update (const double &dt) {
        this -> position += this -> speed * dt;
        for(auto &child : this -> children) {
            child -> Update(dt);
        }
    }

};

#endif // ENGINE_SPACEOBJECT_H
