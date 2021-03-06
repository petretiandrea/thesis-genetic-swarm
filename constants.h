//
// Created by andrea on 04/01/21.
//

#ifndef SWARM_GEN_CONSTANTS_H
#define SWARM_GEN_CONSTANTS_H

#include <cmath>
#include <argos3/core/utility/math/vector3.h>
#include <loop_function/evolution/Rect.h>

namespace constants {
    extern const int RANDOM_SEED = 123;

    /* Environment constants */
    extern const float FOOTBOT_RADIUS = 0.1f;
    extern const int MAX_ATTEMPTS_LOCATION_SPAWN = 20;  // number of max attempts for locate randomly the bot
//    extern const float ARENA_SIDE_SIZE = 6.0f - 0.1f;   // arena size - wall depth
//    extern const argos::CVector3 CIRCLE1(-1.83f, 0.0f, 0.8f);  // location of first black circle
//    extern const argos::CVector3 CIRCLE2(1.83f, 0.0f, 0.8f);   // location of second black circle


    // all arena config
    extern const float ARENA_SIDE_SIZE = 5.0f - 0.1f;   // arena size - wall depth
    //extern const Rect SPAWN_AREA { .topLeft = {-2.4, 2.4}, .downRight = {2.4, -2.4} };
    //extern const Rect SPAWN_AREA { .topLeft = {-2.4, 2.4}, .downRight = {2.4, -2.4} };
    /*extern const argos::CVector3 CIRCLE1(-1.25f, -1.25f, 0.7f);  // location of first black circle
    extern const argos::CVector3 CIRCLE2(1.25f, -1.25f, 0.7f); // location of second black circle
    extern const argos::CVector3 CIRCLE3(0.0f, 1.25f, 0.7f); // location of third black circle
*/}

#endif //SWARM_GEN_CONSTANTS_H
