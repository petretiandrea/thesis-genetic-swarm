//
// Created by Andrea Petreti on 26/01/2021.
//

#include "LoopTask1.h"

#define KEY_CIRCLE1 "circle1"

LoopTask1::LoopTask1() : robotCount(0) { }

void LoopTask1::Reset() {
    BaseLoop::Reset();
    robotCount = 0;
}

void LoopTask1::PostExperiment() {
    CLoopFunctions::PostExperiment();

    CVector2 botPosition;
    robotCount = 0;
    auto& blackCircle = blackCircles.back();

    for(auto& bot : bots) {
        bot->GetEmbodiedEntity().GetOriginAnchor().Position.ProjectOntoXY(botPosition);
        if(blackCircle.containsPoint(botPosition)) robotCount += 1;
    }
}

double LoopTask1::Evaluate() {
    return robotCount / (double) bots.size();
}

vector<Circle> LoopTask1::loadBlackZones(TConfigurationNode &t_tree) {
    CVector3 tmpCircle;
    vector<Circle> blacks;
    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE1, tmpCircle, CIRCLE1);
    blacks.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });
    robotCount = 0;
    return blacks;
}

Rect LoopTask1::loadSpawnArea(TConfigurationNode &t_tree) {
    return SPAWN_AREA;
}

bool LoopTask1::forbiddenSpawn(CVector2 &position) {
    return isInsideBlackZone(position);
}

REGISTER_LOOP_FUNCTIONS(LoopTask1, "loop_task1")