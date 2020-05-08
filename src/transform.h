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
    static Matrix4 invert(const Matrix4 &matrix);
    static bool insertCriteria(const Vector4 &hv, const Matrix4 &diffMat, const Matrix4 &before,
                                 const Matrix4 &after, const std::vector<Vector3, allocV> &Ns, Cases c);
    static std::vector<Vector3, allocV> getAABB(const Mesh &inputMesh);
    static Float getLargestMovement(const std::vector<Vector3, allocV> &aabb, const Matrix4 &current, 
                                    const Matrix4 &next);

                                 

};

#endif