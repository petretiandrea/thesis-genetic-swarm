//
// Created by Andrea Petreti on 26/01/2021.
//

#ifndef SWARM_GEN_LOOPTASK4_H
#define SWARM_GEN_LOOPTASK4_H

#include "BaseLoop.h"

const Rect SPAWN_AREA { .topLeft = {-2.4, 2.4}, .downRight = {2.4, -2.4} };
const argos::CVector3 CIRCLE1(-1.5f, -1.25f, 0.8f);  // location of first black circle
const argos::CVector3 CIRCLE2(1.5f, -1.25f, 0.8f); // location of second black circle

class LoopTask4 : public BaseLoop {

private:
    int botCount[2];

public:
    void PostExperiment() override;

    void Reset() override;

    double Evaluate() override;

protected:
    vector<Circle> loadBlackZones(TConfigurationNode &t_tree) override;

    Rect loadSpawnArea(TConfigurationNode &t_tree) override;

    bool forbiddenSpawn(CVector2 &position) override;

};


#endif //SWARM_GEN_LOOPTASK4_H
