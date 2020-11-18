#ifndef _SIMPLIFY_H
#define _SIMPLIFY_H

#include "common.h"
#include "transform.h"

class Simplify {

public:
    static std::unique_ptr<Mesh> simplify(const Transform::TransformResult &inputMesh);
    static std::unique_ptr<Mesh> simplify(const Mesh &inputMesh) {
        auto transformResult = Transform::TransformResult::newFromMesh(inputMesh);
        return simplify(*transformResult);
    }
private:   
};

#endif