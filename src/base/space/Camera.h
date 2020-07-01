//
// Created by Mugur Tomita Jr. on 31/03/2020.
// Copyright (c) Mugur Tomita Jr. All rights reserved.
//

#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <algorithm>
#include <cmath>
#include <queue>
#include <SDL2/SDL.h>

#include "Space.h"
#include "../drawer/Drawer.h"
#include "../../math/Transformer.h"

#include "../../math/Plane.h"

#include "../../math/Quaternion.h"

class Camera : public KeyboardHandler {
private:
    SDL_Surface *surface = nullptr;
    SDL_Renderer *renderer = nullptr;
    Space *space = nullptr;

    TriVector position;
    BiVector size;
    double fieldOfView = 0.0f;
    double verticalFieldOfView = 0.0f;
    double near = 0.1f;
    double far = 1000.0f;

    TriVector turningSpeed = TriVector(0.6, 0.6, 0.6);
    TriVector movingSpeed = TriVector(60, 60, 60);

    Matrix projectionMatrix;

    TriVector forward = TriVector::OZ();
    TriVector up = TriVector::OY();
    TriVector right = TriVector::OX();

    Plane clipPlanes[6];
    Plane spaceClipPlanes[6];
    Plane spaceNearPlane;

    Matrix rightRunningNormal;
    Matrix leftRunningNormal;
    Matrix topRunningNormal;
    Matrix bottomRunningNormal;

    TriVector rotation;

    double viewFadeDistance = 1000.0f;

public:
    Camera(const TriVector &p_position, const BiVector &p_size, const double &p_fieldOfView, const double &p_near, const double &p_far) {
        this -> position = p_position;
        this -> size = p_size;
        this -> fieldOfView = p_fieldOfView;
        this -> verticalFieldOfView = 2 * asin(sin(this -> fieldOfView / 2.0f) * this -> size.y / this -> size.x);
        this -> near = p_near;
        this -> far = p_far;
        this -> viewFadeDistance = p_far;

        this -> forward = TriVector::OZ();
        this -> up = TriVector::OY();
        this -> right = TriVector::OX();

        this -> projectionMatrix = transformer::BuildProjectionMatrix(this -> size.x / this -> size.y, this -> fieldOfView, this -> near, this -> far);

        this -> spaceNearPlane = Plane(TriVector(0, 0, this -> near), TriVector(0, 0, 1)); // near

        this -> clipPlanes[0] = Plane(TriVector(0, 0, 0.1f), TriVector(0, 0, 1)); // near
        this -> clipPlanes[1] = Plane(TriVector(0, 0, this -> viewFadeDistance), TriVector(0, 0, -1)); // far

        this -> clipPlanes[2] = Plane(TriVector(0, this -> size.y, 0), TriVector(0, -1, 0)); // top
        this -> clipPlanes[3] = Plane(TriVector(0, 0, 0), TriVector(0, 1, 0)); // bottom

        this -> clipPlanes[4] = Plane(TriVector(this -> size.x, 0, 0), TriVector(-1, 0, 0)); // right
        this -> clipPlanes[5] = Plane(TriVector(0, 0, 0), TriVector(1, 0, 0)); // bottom


        this -> rightRunningNormal = transformer::BuildYRotationMatrix(-(PI_2o4 - (this -> fieldOfView / 2.0f)));
        this -> leftRunningNormal = transformer::BuildYRotationMatrix((PI_2o4 - (this -> fieldOfView / 2.0f)));
        this -> topRunningNormal =  transformer::BuildXRotationMatrix((PI_2o4 - (this -> verticalFieldOfView / 2.0f)));
        this -> bottomRunningNormal =  transformer::BuildXRotationMatrix(-PI_2o4 + this -> verticalFieldOfView / 2.0f);
    }

    Camera(const Camera &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
    }

    Camera &operator=(const Camera &that) {
        if (this != &that) {
            this->clear();
            this->copy(that);
        }
        return *this;
    }

    ~Camera() {
        this->clear();
    }

    void clear() {
        if(this -> surface != nullptr) {
            SDL_FreeSurface(this -> surface);
            this -> surface = nullptr;
        }

        if(this -> renderer != nullptr) {
            SDL_DestroyRenderer(this -> renderer);
            this -> renderer = nullptr;
        }
    }

    void copy(const Camera &that) {
        this -> space = that.space;
        this -> surface = that.surface;
        this -> renderer = that.renderer;

        this -> position = that.position;
        this -> size = that.size;
        this -> fieldOfView = that.fieldOfView;
        this -> verticalFieldOfView = that.verticalFieldOfView;
        this -> near = that.near;
        this -> far = that.far;

        this -> forward = that.forward;
        this -> up = that.up;
        this -> right = that.right;

        this -> rotation = that.rotation;

        this -> projectionMatrix = that.projectionMatrix;
    }

    void bindSpace(Space *p_space) {
        this -> space = p_space;
    }

    void Start() {
        this -> surface = SDL_CreateRGBSurface(0, this -> size.intX(), this -> size.intY(), 32, rmask, gmask, bmask, amask);
        this -> renderer = SDL_CreateSoftwareRenderer(this -> surface);
    }

    void Render(double dt, double inter) {
        if(this -> space != nullptr) {
            Color baseColor = Color::BLACK();
            Color drawColor = Color::WHITE();
            Color illuminatedColor = Color::WHITE();
            Color outlineColor = Color::RED();
            Color normalColor = Color::GREEN();
            SDL_SetRenderDrawColor(this -> renderer, baseColor.r, baseColor.g, baseColor.b, baseColor.a);
            SDL_RenderClear(this -> renderer);

            SDL_SetRenderDrawColor(this -> renderer, drawColor.r, drawColor.g, drawColor.b, drawColor.a);

            std::vector<TriVector*> controlPoints = std::vector<TriVector*>();
            std::vector<MeshTriangle> mesh = std::vector<MeshTriangle>();

            Quaternion rotationX = Quaternion(this -> rotation.x, TriVector::OX()).normal();
            Quaternion rotationY = Quaternion(this -> rotation.y, TriVector::OY()).normal();

            Quaternion rotationQ = rotationY * rotationX;

            //this -> forward = rotationQ.rotate(TriVector::OZ()).normal();
            //this -> up = rotationQ.rotate(TriVector::OY()).normal();

            //std::cout << "Forward A " << this -> forward.toString() << "\n";

            /*IMPQuaternion<double>qX = IMPQuaternion<double>(this -> rotation.x, 1, 0, 0).UnitIMPQuaternion();
            IMPQuaternion<double>qY = IMPQuaternion<double>(this -> rotation.y, 0, 1, 0).UnitIMPQuaternion();

            //compose.QuatRotation(initial);

            IMPQuaternion<double>compose = qX * qY;
            IMPQuaternion <double> qForwardBefore(0, 0, 0, 1);
            IMPQuaternion <double> qForward = compose * qForwardBefore * compose.inverse();

            //compose = qY * qX;
            IMPQuaternion <double> qUpBefore(0, 0, -1, 0);
            IMPQuaternion <double> qUp = compose * qUpBefore * compose.inverse();

            this -> forward = TriVector(qForward.x, qForward.y, qForward.z);
            this -> up = TriVector(qUp.x, qUp.y, qUp.z);*/

            //this -> right = this -> up & this -> forward;

            //std::cout << "Rot: (" << qm.x << ", " << qm.y << ", " << qm.z << ")\n";

            //this -> forward = TriVector::OZ() * transformer::BuildXRotationMatrix(this -> rotation.x) * transformer::BuildYRotationMatrix(this -> rotation.y);
            //std::cout << "Forward B " << this -> forward.toString() << "\n";

            //this -> up = TriVector::OY() * transformer::BuildXRotationMatrix(this -> rotation.x) * transformer::BuildYRotationMatrix(this -> rotation.y);
            //this -> right = this -> up & this -> forward;

            //this -> forward = TriVector::OZ() *

            //double halfBackScreenSize = this -> viewFadeDistance * tan(this -> fieldOfView / 2.0f);
            //TriVector backCameraPoint = this -> position + (this -> forward * this -> viewFadeDistance);

            this -> spaceClipPlanes[0] = Plane(this -> position, this -> forward);
            this -> spaceClipPlanes[1] = Plane(this -> position + (this -> forward * this -> viewFadeDistance), this -> forward * -1);
            this -> spaceClipPlanes[2] = Plane(this -> position, this -> forward * rightRunningNormal);
            this -> spaceClipPlanes[3] = Plane(this -> position, this -> forward * leftRunningNormal);


            //IMPQuaternion <double> qFor(0, -this -> forward.x, -this -> forward.y, -this -> forward.z);

            //IMPQuaternion <double> oXRot(-(PI_2o4 - this -> verticalFieldOfView), 1, 0, 0);
            //IMPQuaternion <double> runTopN = oXRot * qFor * oXRot.inverse();

            //std::cout << "runTopN: (" << runTopN.x << ", " << runTopN.y << ", " << runTopN.z << ")\n";

            //this -> spaceClipPlanes[4] = Plane(this -> position, TriVector(runTopN.x, runTopN.y, runTopN.z));
            this -> spaceClipPlanes[4] = Plane(this -> position, this -> forward * topRunningNormal);
            this -> spaceClipPlanes[5] = Plane(this -> position, (this -> right * transformer::BuildYRotationMatrix(-PI_2o4)) * bottomRunningNormal);



            this -> space -> loadControlMeshClipping(this -> position, this -> spaceClipPlanes, 4, controlPoints, mesh);

            TriVector focusPoint = this -> position + this -> forward;

            Matrix cameraMatrix = transformer::PointAtMatrix(this -> position, focusPoint, this -> up);
            Matrix viewMatrix = transformer::QuickMatrixInverseScoped(cameraMatrix);

            // Illumination
            /*std::vector<MeshTriangle> newMesh = std::vector<MeshTriangle>();
            for(auto &meshTriangle : mesh) {
                meshTriangle.illumination = meshTriangle.normal() * (this -> position - meshTriangle.center()).normal();
                if(meshTriangle.illumination > 0) {
                    newMesh.push_back(meshTriangle);
                }
            }
            mesh = newMesh;*/

            // Transforming
            for(auto &controlPoint : controlPoints) {
                *controlPoint *= viewMatrix;
            }

            // Initial near clipping
            /*std::vector<TriVector*> generatedControlPoints = std::vector<TriVector*>();
            std::vector<TriVector*> removedPoints = std::vector<TriVector*>();
            std::vector<MeshTriangle> newMesh = std::vector<MeshTriangle>();;
            for(auto &meshTriangle : mesh) {
                MeshTriangle verify = meshTriangle;
                MeshTriangle generated;

                int nbOfResulted = transformer::ClipTriangleOnPlane(
                        this->spaceNearPlane,
                        meshTriangle,
                        generated,
                        generatedControlPoints,
                        removedPoints);

                if(nbOfResulted > 0) {
                    newMesh.push_back(meshTriangle);
                }
                if(nbOfResulted > 1) {
                    newMesh.push_back(generated);
                }
            }
            mesh = newMesh;*/

            /*for(std::vector<TriVector*>::iterator it = controlPoints.begin(); it!=controlPoints.end(); it++) {
                for(auto &removedPoint : removedPoints) {
                    if(*it == removedPoint) {
                        controlPoints.erase(it--);
                    }
                }
            }*/

            /*for(auto &removedPoint : removedPoints) {
                if(removedPoint != nullptr) {
                    delete removedPoint;
                    removedPoint = nullptr;
                }
            }*/

            //controlPoints.insert(std::end(controlPoints), std::begin(generatedControlPoints), std::end(generatedControlPoints));

            // Sorting
            sort(mesh.begin(), mesh.end(), [](MeshTriangle &m1, MeshTriangle &m2) {return m1.midZ() > m2.midZ();});

            // Projection
            for(auto &controlPoint : controlPoints) {
                controlPoint -> extend = 1;
                *controlPoint *= this -> projectionMatrix;
                controlPoint->selfOverExtend(true);

                controlPoint->x += 1.0f;
                controlPoint->y += 1.0f;

                // TODO: Check this
                //controlPoint->z += 1.0f;

                controlPoint->x /= 2.0f;
                controlPoint->y /= 2.0f;
                //controlPoint->z /= 2.0f;

                controlPoint->x *= this->size.x;
                controlPoint->y *= this->size.y;
                //controlPoint->z *= this->far - this -> near;
            }

            /*newMesh = std::vector<MeshTriangle>();;
            generatedControlPoints = std::vector<TriVector*>();
            removedPoints = std::vector<TriVector*>();
            std::queue<MeshTriangle> accumulator;
            for(auto &meshTriangle : mesh) {
                accumulator = std::queue<MeshTriangle>();
                accumulator.push(meshTriangle);

                for(int i=0; i<6; ++i) {
                    MeshTriangle generated, verify;
                    int nbToVerify = accumulator.size();
                    while(nbToVerify > 0) {
                        nbToVerify -= 1;
                        verify = accumulator.front();
                        accumulator.pop();
                        int nbOfResulted = transformer::ClipTriangleOnPlane(
                                this->clipPlanes[i],
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
                    newMesh.push_back(accumulator.front());
                    accumulator.pop();
                }
            }
            controlPoints.insert(std::end(controlPoints), std::begin(generatedControlPoints), std::end(generatedControlPoints));
            mesh = newMesh;*/

            // Drawing
            for(auto &meshTriangle : mesh) {
                if(meshTriangle.illumination >= 0) {
                    illuminatedColor = drawColor * meshTriangle.illumination;
                    SDL_SetRenderDrawColor(this -> renderer, illuminatedColor.r, illuminatedColor.g, illuminatedColor.b, illuminatedColor.a);
                    drawTriangle(this -> renderer, meshTriangle, true);

                    SDL_SetRenderDrawColor(this -> renderer, normalColor.r, normalColor.g, normalColor.b, normalColor.a);
                    drawLineCanvas(this -> renderer, BiVector(meshTriangle.centerPoint->x, meshTriangle.centerPoint->y), BiVector(meshTriangle.normalPoint->x, meshTriangle.normalPoint->y));
                }
            }

            // Outlining
            SDL_SetRenderDrawColor(this -> renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
            for(auto &meshTriangle : mesh) {
                if(meshTriangle.illumination >= 0) {
                    drawTriangle(this->renderer, meshTriangle, false);
                }
            }

            // Deallocate
            for(auto &controlPoint : controlPoints) {
                if(controlPoint != nullptr){
                    delete controlPoint;
                    controlPoint = nullptr;
                }
            }
        }
    };

    SDL_Surface *getSurface() {
        return this -> surface;
    }

    void verifyKeyboard(const double &dt, const Uint8 *keyboardState) {

        if(keyboardState[SDL_SCANCODE_0] || keyboardState[SDL_SCANCODE_1]) {
            this -> backup();
            return;
        }

        // TODO: Respect UP, LEFT, RIGHT
        double viewAngleX = 0.0f;
        double viewAngleY = 0.0f;
        if(keyboardState[SDL_SCANCODE_UP]) {
            this -> rotation.x -= this -> turningSpeed.x * dt;
            viewAngleX -= this -> turningSpeed.x * dt;
        }

        if(keyboardState[SDL_SCANCODE_DOWN]) {
            this -> rotation.x += this -> turningSpeed.x * dt;
            viewAngleX += this -> turningSpeed.x * dt;
        }

        if(keyboardState[SDL_SCANCODE_LEFT]) {
            this -> rotation.y -= this -> turningSpeed.y * dt;
            viewAngleY -= this -> turningSpeed.y * dt;
        }

        if(keyboardState[SDL_SCANCODE_RIGHT]) {
            this -> rotation.y += this -> turningSpeed.y * dt;
            viewAngleY += this -> turningSpeed.y * dt;
        }

        if(viewAngleX != 0.0f || viewAngleY != 0.0f) {
            Quaternion rot;

            rot = Quaternion(viewAngleX, TriVector::OX()).normal() * Quaternion(viewAngleY, TriVector::OY()).normal();
            this -> forward = rot.rotate(this -> forward);

            //rot = Quaternion(viewAngleY, TriVector::OY()).normal();
            //this -> forward = rot.rotate(this -> forward);
        }

        /*if(viewAngleX != 0.0f || viewAngleY != 0.0f) {
            double theta = 0.0f;
            double phi = 0.0f;

            theta = atan2(this -> forward.x, this -> forward.z);

            double sqrtDivider = sqrt(this->forward.x * this->forward.x + this->forward.z * this->forward.z);
            phi = atan2(this->forward.y, sqrtDivider);

            Matrix xRotation = transformer::BuildXRotationMatrix(viewAngleX);
            Matrix yRotation = transformer::BuildYRotationMatrix(viewAngleY);

            this -> forward *= transformer::BuildXRotationMatrix(phi);
            this -> forward *= transformer::BuildYRotationMatrix(-theta);
            this -> forward *= yRotation;
            this -> forward *= xRotation;
            this -> forward *= transformer::BuildYRotationMatrix(theta);
            this -> forward *= transformer::BuildXRotationMatrix(-phi);
            this -> forward.selfNormalize();

            this -> up = TriVector::OY();
            this -> up *= xRotation;
            this -> up *= yRotation;
            this -> up *= transformer::BuildXRotationMatrix(-phi);
            this -> up *= transformer::BuildYRotationMatrix(theta);
            this -> up.selfNormalize();

            this -> right = this -> up & this -> forward;
            this -> right.selfNormalize();
        }*/

        TriVector moving;

        if(keyboardState[SDL_SCANCODE_W]) {
            moving += this -> forward * this -> movingSpeed.z * dt;
        }

        if(keyboardState[SDL_SCANCODE_S]) {
            moving -= this -> forward * this -> movingSpeed.z * dt;
        }

        if(keyboardState[SDL_SCANCODE_A]) {
            moving -= this -> right * this -> movingSpeed.x * dt;
        }

        if(keyboardState[SDL_SCANCODE_D]) {
            moving += this -> right * this -> movingSpeed.x * dt;
        }

        if(keyboardState[SDL_SCANCODE_SPACE]) {
            moving += this -> up * this -> movingSpeed.y * dt;
        }

        if(keyboardState[SDL_SCANCODE_LSHIFT]) {
            moving -= this -> up * this -> movingSpeed.y * dt;
        }

        this -> position += moving;
    }
};

#endif // ENGINE_CAMERA_H
