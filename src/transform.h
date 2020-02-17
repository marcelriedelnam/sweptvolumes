#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "common.h"
#include "obj_reader.h"
#include "csv_reader.h"


class Transform {
public:
    Transform(const Mesh &mesh,
              const std::vector<Matrix4, allocM> &matrices) 
    {
      transformedMesh = std::make_unique<Mesh>();
      transformVertices(mesh, matrices);
    }


private:
    void transformVertices(const Mesh &mesh,
                           const std::vector<Matrix4, allocM> &matrices);

public:
	std::unique_ptr<Mesh> transformedMesh;
};

#endif