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
        transformVertices(vertices, faces, matrices);
    }
    std::vector<Vector3, allocV> getTransformedVertices();
    std::vector<std::tuple<IndexType,IndexType,IndexType>> getTransformedFaces();

private:
    void transformVertices(std::vector<Vector3, allocV> vertices,
                           std::vector<std::tuple<IndexType,IndexType,IndexType>> faces,
                           std::vector<Matrix4, allocM> matrices);
    Vector3 calculateVector(Vector4 vec);

private:
    std::vector<Vector3, allocV> transformedVertices;
    std::vector<std::tuple<IndexType,IndexType,IndexType>> transformedFaces;
};

#endif