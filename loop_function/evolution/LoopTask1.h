//
// Created by Andrea Petreti on 26/01/2021.
//

#ifndef SWARM_GEN_LOOPTASK1_H
#define SWARM_GEN_LOOPTASK1_H

#include "BaseLoop.h"
#include "Rect.h"

const Rect SPAWN_AREA { .topLeft = {-2.4, 2.4}, .downRight = {2.4, -2.4} };
const argos::CVector3 CIRCLE1(-0.0f, 0.0f, 0.8f);

class LoopTask1 : public BaseLoop {

public:
    LoopTask1();

    void Reset() override;

    double Evaluate() override;

    vector<Circle> loadBlackZones(TConfigurationNode &t_tree) override;

    Rect loadSpawnArea(TConfigurationNode &t_tree) override;

    bool forbiddenSpawn(CVector2 &position) override;

    void PostExperiment() override;

private:
    int robotCount;

};

#endif //SWARM_GEN_LOOPTASK1_H
