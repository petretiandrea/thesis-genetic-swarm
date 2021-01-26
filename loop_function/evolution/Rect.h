//
// Created by Andrea Petreti on 21/01/2021.
//

#ifndef SWARM_GEN_RECT_H
#define SWARM_GEN_RECT_H

#include <argos3/core/utility/math/vector2.h>

struct Rect {
    argos::CVector2 topLeft;
    argos::CVector2 downRight;

public:
    double width() const {
        return sqrt(pow(topLeft.GetX() - downRight.GetX(), 2));
    }
    double height() const {
        return sqrt(pow(topLeft.GetY() - downRight.GetY(), 2));
    }
};

#endif //SWARM_GEN_RECT_H
