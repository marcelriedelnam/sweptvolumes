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
    enum Cases {
      FIRST, MID, LAST
    };

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
    static Matrix4 invert(Matrix4 matrix);
    static bool insertIfCriteria(const Vector4 &hv, const Matrix4 &diffMat, 
                                 const std::vector<Vector3, allocV> &Ns, Cases c);

};

#endif