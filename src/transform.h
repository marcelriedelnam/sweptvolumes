#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "common.h"
#include "obj_reader.h"
#include "csv_reader.h"


class Transform {
public:
  static std::unique_ptr<Mesh> transform(const Mesh &mesh, 
                                         const std::vector<Matrix4, allocM> &matrices);

private:
    Transform() {}

    static void transformVerticesNaive(const Mesh &inputMesh,
                                       const std::vector<Matrix4, allocM> &matrices,
                                       Mesh &outputMesh);
    static void transformVerticesDM(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh);
    static void transformVerticesRA(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh);
    static std::tuple<Vector3,Vector3> calculateRA(Matrix4 &matrix);

};

#endif