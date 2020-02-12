#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "common.h"
#include "obj_reader.h"
#include "csv_reader.h"


class Transform {
public:
    Transform(std::vector<Vector3, allocV> vertices,
              std::vector<std::tuple<IndexType,IndexType,IndexType>> faces,
              std::vector<Matrix4, allocM> matrices) 
    {
    transformedMesh = std::make_unique<Mesh>();
		transformVertices(vertices, faces, matrices);
    }


private:
    void transformVertices(std::vector<Vector3, allocV> vertices,
                           std::vector<std::tuple<IndexType,IndexType,IndexType>> faces,
                           std::vector<Matrix4, allocM> matrices);
    Vector3 calculateVector(Vector4 vec);

public:
	std::unique_ptr<Mesh> transformedMesh;
};

#endif