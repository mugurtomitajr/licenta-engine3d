//
// Created by Mugur Tomita Jr. on 31/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_SPACE_H
#define ENGINE_SPACE_H

#include <vector>

#include "../../math/TriVector.h"
#include "objects/ObjectCuboid.h"
#include "../../math/Plane.h"
#include "../../math/Transformer.h"

class Space {
private:
    std::vector<SpaceObject*> objects;

public:
    Space() {
        this -> objects = std::vector<SpaceObject*>();
    }

    Space(const Space &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Space &operator=(const Space &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Space() {
        this->clear();
    }

    void addObject(SpaceObject *p_object) {
        this -> objects.push_back(p_object);
    }

    void clear() {
        for (auto & object : this->objects) {
            delete object;
            object = nullptr;
        }
    }

    void copy(const Space &that) {
        this -> objects = that.objects;
    }

    void loadControlMeshClipping(const TriVector &cameraPosition, Plane *clip, int clipNb, std::vector<TriVector*> &p_control, std::vector<MeshTriangle> &p_mesh) {
        std::vector<TriVector*> control;
        std::vector<MeshTriangle> mesh;
        std::vector<MeshTriangle> refinedMesh;
        std::vector<TriVector*> generatedControlPoints = std::vector<TriVector*>();
        std::vector<TriVector*> removedPoints = std::vector<TriVector*>();
        std::queue<MeshTriangle> accumulator;

        for (int i = 0; i < clipNb; ++i) {
            clip[i].normal.selfNormalize();
        }

        for(auto &object : this -> objects) {
            object -> loadControlMesh(control, mesh);
        }

        for(auto &meshTriangle : mesh) {
            meshTriangle.illumination = meshTriangle.normal() * (cameraPosition - meshTriangle.center()).normal();
            if(meshTriangle.illumination > 0) {
                meshTriangle.normalPoint = new TriVector (meshTriangle.center() + (meshTriangle.normal() * 10.0f));
                meshTriangle.centerPoint = new TriVector (meshTriangle.center());
                generatedControlPoints.push_back(meshTriangle.normalPoint);
                generatedControlPoints.push_back(meshTriangle.centerPoint);
                refinedMesh.push_back(meshTriangle);
            }
        }

        mesh = refinedMesh;
        refinedMesh = std::vector<MeshTriangle>();

        for(auto &meshTriangle : mesh) {
            accumulator = std::queue<MeshTriangle>();
            accumulator.push(meshTriangle);

            for(int i=0; i<clipNb; ++i) {
                MeshTriangle generated, verify;
                int nbToVerify = accumulator.size();
                while(nbToVerify > 0) {
                    nbToVerify -= 1;
                    verify = accumulator.front();
                    accumulator.pop();
                    generated = MeshTriangle();
                    int nbOfResulted = transformer::ClipTriangleOnPlane(
                            clip[i],
                            verify,
                            generated,
                            generatedControlPoints,
                            removedPoints);
                    if (nbOfResulted > 0) {
                        accumulator.push(verify);
                    }
                    if (nbOfResulted > 1) {
                        accumulator.push(generated);
                    }
                }
            }

            while(!accumulator.empty()) {
                refinedMesh.push_back(accumulator.front());
                accumulator.pop();
            }
        }

        p_control.insert(std::end(p_control), std::begin(control), std::end(control));
        p_control.insert(std::end(p_control), std::begin(generatedControlPoints), std::end(generatedControlPoints));
        p_mesh.insert(std::end(p_mesh), std::begin(refinedMesh), std::end(refinedMesh));
    }

    void loadControlMesh(Plane *clip, int clipNb, std::vector<TriVector*> &p_control, std::vector<MeshTriangle> &p_mesh) {
        std::vector<TriVector*> control;
        std::vector<MeshTriangle> mesh;
        std::vector<MeshTriangle> refinedMesh;

        for (int i = 0; i < clipNb; ++i) {
            clip[i].normal.selfNormalize();

            //std::cout << "Plane nb: " << i << "\n";
            //std::cout << "Plane point: " << clip[i].point.toString() << "\n";
            //std::cout << "Plane normal: " << clip[i].normal.toString() << "\n\n";
        }

        for(auto &object : this -> objects) {
            object -> loadControlMesh(control, mesh);
        }

        int checkCount = 0;
        bool pointInsideAll = false;
        for(auto &meshTriangle : mesh) {
            checkCount = 0;

            for(int pointNb = 0; pointNb < 3; ++pointNb) {
                pointInsideAll = true;
                if(meshTriangle.point[0]->helper) {
                    pointInsideAll = false;
                } else {
                    for (int i = 0; i < clipNb; ++i) {
                        if (clip[i].positionOfPoint(*(meshTriangle.point[0])) < 0) {
                            pointInsideAll = false;
                            meshTriangle.point[0]->helper = true;
                            break;
                        }
                    }
                }
                if(pointInsideAll) {
                    checkCount += 1;
                }
            }

            if(checkCount != 0) {
                meshTriangle.point[0] -> used = true;
                meshTriangle.point[1] -> used = true;
                meshTriangle.point[2] -> used = true;
                refinedMesh.push_back(meshTriangle);
            }
        }

        //std::cout << "Control original: " << control.size() << "\n";

        for(std::vector<TriVector*>::iterator it = control.begin(); it!=control.end(); it++) {
            if(!(*it)->used) {
                control.erase(it--);
            }
        }

        //std::cout << "Control after: " << control.size() << "\n";

        //std::cout << "Mesh original: " << mesh.size() << "\n";
        //std::cout << "Mesh after: " << refinedMesh.size() << "\n\n";

        p_control.insert(std::end(p_control), std::begin(control), std::end(control));
        p_mesh.insert(std::end(p_mesh), std::begin(refinedMesh), std::end(refinedMesh));
    }

    void Initialize () {

        //this -> addObject(new ObjectCuboid(nullptr, TriVector(0, 0, 800), TriVector(100, 100, 100)));

        /*this -> addObject(new ObjectCuboid(nullptr, TriVector(300, 300, 900), TriVector(100, 100, 100)));
        this -> addObject(new ObjectCuboid(nullptr, TriVector(-300, 300, 700), TriVector(100, 100, 100)));
        this -> addObject(new ObjectCuboid(nullptr, TriVector(-300, -300, 700), TriVector(100, 100, 100)));
        this -> addObject(new ObjectCuboid(nullptr, TriVector(300, -300, 700), TriVector(100, 100, 100)));*/

        this -> addObject(new SpaceObject(nullptr, TriVector(0, 0, 0), "mountains.obj", 10));

    }

    void Update (const double &dt) {
        for(auto &object : this -> objects) {
            object -> Update(dt);
        }
    }
};

#endif // ENGINE_SPACE_H
