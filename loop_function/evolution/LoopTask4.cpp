//
// Created by Andrea Petreti on 26/01/2021.
//

#include "LoopTask4.h"

#define KEY_CIRCLE1 "circle1"
#define KEY_CIRCLE2 "circle2"

void LoopTask4::PostExperiment() {
    CLoopFunctions::PostExperiment();
    CVector2 botPosition;

    std::fill(std::begin(botCount), std::end(botCount), 0);

    for(auto& bot : bots) {
        bot->GetEmbodiedEntity().GetOriginAnchor().Position.ProjectOntoXY(botPosition);
        if(blackCircles[0].containsPoint(botPosition)) botCount[0] += 1;
        if(blackCircles[1].containsPoint(botPosition)) botCount[1] += 1;
    }
}

void LoopTask4::Reset() {
    BaseLoop::Reset();
    std::fill(std::begin(botCount), std::end(botCount), 0);
}

double LoopTask4::Evaluate() {
    auto max = *std::max_element(std::begin(botCount), std::end(botCount));
    return max / (double) bots.size();
}

vector<Circle> LoopTask4::loadBlackZones(TConfigurationNode &t_tree) {
    CVector3 tmpCircle;
    vector<Circle> blackCircles;
    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE1, tmpCircle, CIRCLE1);
    blackCircles.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });

    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE2, tmpCircle, CIRCLE2);
    blackCircles.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });

    std::fill(std::begin(botCount), std::end(botCount), 0);

    return blackCircles;
}

Rect LoopTask4::loadSpawnArea(TConfigurationNode &t_tree) {
    return SPAWN_AREA;
}

bool LoopTask4::forbiddenSpawn(CVector2 &position) {
    return isInsideBlackZone(position);
}

REGISTER_LOOP_FUNCTIONS(LoopTask4, "loop_task4")