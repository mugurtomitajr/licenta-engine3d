//
// Created by Mugur Tomita Jr. on 03/04/2020.
//

#ifndef ENGINE_TRANSFORMER_H
#define ENGINE_TRANSFORMER_H

#include <cmath>
#include "MathConstants.h"
#include "Matrix.h"
#include "TriVector.h"
#include "Plane.h"

namespace transformer {
    Matrix BuildProjectionMatrix(double aspectRatio, double fieldOfView, double near, double far) {
        Matrix matrix = Matrix(4, 4);
        double ctg = 1.0f / tan(fieldOfView / 2.0f);

        matrix.set(0, 0, -ctg);
        matrix.set(1, 1, aspectRatio * ctg);
        matrix.set(2, 2, (far + near) / (far - near));
        matrix.set(3, 2, (2.0f * far * near) / (far - near));
        matrix.set(2, 3, -1.0f);
        matrix.set(3, 3, 0.0f);

        /*matrix.set(0, 0, ctg);
        matrix.set(1, 1, aspectRatio * ctg);
        matrix.set(2, 2, far / (far - near));
        matrix.set(3, 2, (-far * near) / (far - near));
        matrix.set(2, 3, -1.0f);
        matrix.set(3, 3, 0.0f);*/

        return matrix;
    }

    Matrix BuildXRotationMatrix(double angle) {
        Matrix matrix = Matrix(4, 4);
        matrix.set(0, 0, 1.0f);
        matrix.set(1, 1, cosf(angle));
        matrix.set(1, 2, sinf(angle));
        matrix.set(2, 1, -sinf(angle));
        matrix.set(2, 2, cosf(angle));
        matrix.set(3, 3, 1.0f);
        return matrix;
    }

    Matrix BuildYRotationMatrix(double angle) {
        Matrix matrix = Matrix(4, 4);
        matrix.set(1, 1, 1.0f);
        matrix.set(0, 0, cosf(angle));
        matrix.set(0, 2, -sinf(angle));
        matrix.set(2, 0, sinf(angle));
        matrix.set(2, 2, cosf(angle));
        matrix.set(3, 3, 1.0f);
        return matrix;
    }

    Matrix BuildZRotationMatrix(double angle) {
        Matrix matrix = Matrix(4, 4);
        matrix.set(0, 0, cosf(angle));
        matrix.set(0, 1, sinf(angle));
        matrix.set(1, 0, -sinf(angle));
        matrix.set(1, 1, cosf(angle));
        matrix.set(2, 2, 1.0f);
        matrix.set(3, 3, 1.0f);
        return matrix;
    }
    
    Matrix PointAtMatrix(TriVector position, TriVector target, TriVector up) {
        Matrix matrix = Matrix(4, 4);

        TriVector newForward = target - position;
        newForward.selfNormalize();

        TriVector newUp = up - (newForward * (up * newForward));
        newUp.selfNormalize();

        TriVector newRight = newUp & newForward;

        matrix.set(0, 0, newRight.x);
        matrix.set(0, 1, newRight.y);
        matrix.set(0, 2, newRight.z);
        matrix.set(0, 3, 0);

        matrix.set(1, 0, newUp.x);
        matrix.set(1, 1, newUp.y);
        matrix.set(1, 2, newUp.z);
        matrix.set(1, 3, 0);

        matrix.set(2, 0, newForward.x);
        matrix.set(2, 1, newForward.y);
        matrix.set(2, 2, newForward.z);
        matrix.set(2, 3, 0);

        matrix.set(3, 0, position.x);
        matrix.set(3, 1, position.y);
        matrix.set(3, 2, position.z);
        matrix.set(3, 3, 1);

        return matrix;
    }

    static Matrix QuickMatrixInverseScoped(Matrix source) {
        Matrix result = Matrix(4, 4);
        if(source.height == 4 && source.width == 4) {
            result.set(0, 0, source.at(0, 0));
            result.set(0, 1, source.at(1, 0));
            result.set(0, 2, source.at(2, 0));
            result.set(0, 3, 0);

            result.set(1, 0, source.at(0, 1));
            result.set(1, 1, source.at(1, 1));
            result.set(1, 2, source.at(2, 1));
            result.set(1, 3, 0);

            result.set(2, 0, source.at(0, 2));
            result.set(2, 1, source.at(1, 2));
            result.set(2, 2, source.at(2, 2));
            result.set(2, 3, 0);

            result.set(3, 0, -(source.at(3, 0) * result.at(0, 0) + source.at(3, 1) * result.at(1, 0) + source.at(3, 2) * result.at(2, 0)));
            result.set(3, 1, -(source.at(3, 0) * result.at(0, 1) + source.at(3, 1) * result.at(1, 1) + source.at(3, 2) * result.at(2, 1)));
            result.set(3, 2, -(source.at(3, 0) * result.at(0, 2) + source.at(3, 1) * result.at(1, 2) + source.at(3, 2) * result.at(2, 2)));
            result.set(3, 3, 1);
        }
        return result;
    }

    TriVector LineCrossPlane(Plane plane, TriVector lineStart, TriVector lineEnd){
        plane.normal.selfNormalize();
        double dot = -(plane.normal * plane.point);
        double startDot = lineStart * plane.normal;
        double endDot = lineEnd * plane.normal;
        double percent = (-dot - startDot) / (endDot - startDot);
        TriVector intersectionPoint = (lineEnd - lineStart) * percent;
        return lineStart + intersectionPoint;
    }

    static TriVector Vector_IntersectPlane(TriVector plane_p, TriVector plane_n, TriVector lineStart, TriVector lineEnd)
    {
        plane_n.selfNormalize();
        float plane_d = -(plane_n * plane_p);
        float ad = lineStart * plane_n;
        float bd = lineEnd * plane_n;
        float t = (-plane_d - ad) / (bd - ad);
        TriVector lineStartToEnd = lineEnd-lineStart;
        TriVector lineToIntersect = lineStartToEnd * t;
        return lineStart + lineToIntersect;
    }

    int ClipTriangleOnPlane(Plane &plane, MeshTriangle &triangle, MeshTriangle &generated, std::vector<TriVector*> &newPoints, std::vector<TriVector*> &removedPoints) {
        int nbInside = 0;
        int nbOutside = 0;
        TriVector *inside[3];
        TriVector *outside[3];
        TriVector *newPoint = nullptr;

        if(plane.positionOfPoint(*triangle.point[0]) >= -EPSILON) { inside[nbInside] = triangle.point[0]; nbInside += 1; }
        else { outside[nbOutside] = triangle.point[0]; nbOutside += 1; }

        if(plane.positionOfPoint(*triangle.point[1]) >= -EPSILON) { inside[nbInside] = triangle.point[1]; nbInside += 1; }
        else { outside[nbOutside] = triangle.point[1]; nbOutside += 1; }

        if(plane.positionOfPoint(*triangle.point[2]) >= -EPSILON) { inside[nbInside] = triangle.point[2]; nbInside += 1; }
        else { outside[nbOutside] = triangle.point[2]; nbOutside += 1; }

        for(int i=0; i<nbOutside; ++i) {
            removedPoints.push_back(outside[i]);
        }

        if(nbInside == 0) return 0;
        if(nbInside == 3) return 1;
        if(nbInside == 1) {
            triangle.point[0] = inside[0];

            //newPoint = new TriVector (LineCrossPlane(plane, *(inside[0]), *(outside[0])));
            newPoint = new TriVector (Vector_IntersectPlane(plane.point, plane.normal, *(inside[0]), *(outside[0])));
            triangle.point[1] = newPoint;
            newPoints.push_back(newPoint);

            //newPoint = new TriVector (LineCrossPlane(plane, *(inside[0]), *(outside[1])));
            newPoint = new TriVector (Vector_IntersectPlane(plane.point, plane.normal, *(inside[0]), *(outside[1])));
            triangle.point[2] = newPoint;
            newPoints.push_back(newPoint);

            return 1;
        } else if(nbInside == 2) {
            generated = MeshTriangle(triangle);

            triangle.point[0] = inside[0];
            triangle.point[1] = inside[1];
            //newPoint = new TriVector (LineCrossPlane(plane, *(inside[0]), *(outside[0])));
            newPoint = new TriVector (Vector_IntersectPlane(plane.point, plane.normal, *(inside[0]), *(outside[0])));
            triangle.point[2] = newPoint;
            newPoints.push_back(newPoint);

            generated.point[0] = inside[1];
            generated.point[1] = newPoint;
            //newPoint = new TriVector (LineCrossPlane(plane, *(inside[1]), *(outside[0])));
            newPoint = new TriVector (Vector_IntersectPlane(plane.point, plane.normal, *(inside[1]), *(outside[0])));
            generated.point[2] = newPoint;
            newPoints.push_back(newPoint);

            return 2;
        }

        return 0;
    }

}

#endif //ENGINE_TRANSFORMER_H
