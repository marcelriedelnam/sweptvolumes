#ifndef _TRANSFORM_H
#define _TRANSFORM_H

#include "common.h"
#include "obj_reader.h"
#include "csv_reader.h"
#include "process_mesh.h"


class Transform {
public:

  class TransformResult {
  public:
    Float longestMeshEdge;
    Float longestTemporalEdge;
    std::vector<std::unique_ptr<std::vector<Vector3, allocV>>> resultVertices;

    std::unique_ptr<Mesh> convertToMesh() {
      auto mesh = std::make_unique<Mesh>();
      mesh->longestMeshEdge = longestMeshEdge;
      mesh->longestTemporalEdge = longestTemporalEdge;
      for(auto& vs : resultVertices)
        mesh->vertices.insert(mesh->vertices.end(), vs->begin(), vs->end());
      return mesh;
    }
    static std::unique_ptr<TransformResult> newFromMesh(const Mesh& mesh) {
      auto result = std::make_unique<TransformResult>();
      result->longestMeshEdge = mesh.longestMeshEdge;
      result->longestTemporalEdge = mesh.longestTemporalEdge;
      auto v = std::make_unique<std::vector<Vector3, allocV>>();
      v->insert(v->end(), mesh.vertices.begin(), mesh.vertices.end());
      result->resultVertices.push_back(std::move(v));
      return result;
    }
  };

  static std::unique_ptr<TransformResult> transform(const Mesh &mesh, 
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
                                    TransformResult &result);
    static void transformVerticesRA(const Mesh &inputMesh,
                                    const std::vector<Matrix4, allocM> &matrices,
                                    Mesh &outputMesh);
    static std::tuple<Vector3,Vector3> calculateRA(Matrix4 &matrix);
    static Matrix4 invert(const Matrix4 &matrix);
    static bool insertCriteria(const Vector4 &hv, const Matrix4 &after, const std::vector<Vector3, allocV> &Ns, 
                               Cases c);
    static std::vector<Vector3, allocV> getAABB(const Mesh &inputMesh);
    static Float getLargestMovement(const std::vector<Vector3, allocV> &aabb, const Matrix4 &current, 
                                    const Matrix4 &next);
    static Matrix4 buildHomogenousMatrix(const Matrix3 &mat, const Vector3 &vec);
    static std::vector<Matrix4, allocM> subsampling(const std::vector<Matrix4, allocM> &matrices);
    static std::vector<Matrix4, allocM> newsampling(const std::vector<Matrix4, allocM> &matrices);

};

#endif