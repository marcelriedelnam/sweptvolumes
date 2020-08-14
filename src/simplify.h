#ifndef _SIMPLIFY_H
#define _SIMPLIFY_H

#include "common.h"
#include "transform.h"

class Simplify {

public:
    static std::unique_ptr<Mesh> simplify(const Mesh &inputMesh);
private:   
};

#endif