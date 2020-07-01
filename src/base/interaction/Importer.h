//
// Created by Mugur Tomita Jr. on 04/04/2020.
//

#ifndef ENGINE_IMPORTER_H
#define ENGINE_IMPORTER_H

#include <string>
#include <vector>

#include <fstream>
#include <strstream>

#include "../../math/MeshTriangle.h"
#include "../../options/Path.h"

namespace importer {
    void OBJFile(const std::string &filename, const double &scale, std::vector<TriVector*> &p_control, std::vector<MeshTriangle> &p_mesh) {
        std::string filepath = PATH_RESOURCES_OBJECTS + filename;
        std::ifstream file(filepath);

        if(file.is_open()) {

            std::vector<TriVector*> local_control;
            TriVector *currentControl = nullptr;

            char junk;

            while(!file.eof()) {
                char line[512];
                file.getline(line, 512);
                std::strstream lineStream;
                lineStream << line;

                if(line[0] == 'v') {
                    double x, y, z;
                    lineStream >> junk >> x >> y >> z;
                    currentControl = new TriVector(scale * x, scale * y, scale * z);
                    local_control.push_back(currentControl);
                } else if(line[0] == 'f') {
                    int a, b, c;
                    lineStream >> junk >> a >> b >> c;
                    if(a <= local_control.size() && b <= local_control.size() && c <= local_control.size()) {
                        p_mesh.push_back(MeshTriangle(local_control[a-1], local_control[b-1], local_control[c-1]));
                    }
                }
            }

            p_control.insert(std::end(p_control), std::begin(local_control), std::end(local_control));
        }
    }
}

#endif //ENGINE_IMPORTER_H
