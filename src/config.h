#ifndef _CONFIG_H
#define _CONFIG_H

#include "common.h"

struct  ParameterConfig {
    static Float epsilon;
    static Float maxEdgelength;
    static int samples;
    static bool toggleSubdivision;
    static Float gridCellLength;
    static Float simplifyRadius;
};


#endif