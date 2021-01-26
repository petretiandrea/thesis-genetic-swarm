//
// Created by Andrea Petreti on 26/01/2021.
//

#include "LoopTask3.h"

#define KEY_CIRCLE1 "circle1"
#define KEY_CIRCLE2 "circle2"
#define KEY_CIRCLE3 "circle3"

LoopTask3::LoopTask3() : botCount {0, 0, 0} {}

void LoopTask3::PostExperiment() {
    CLoopFunctions::PostExperiment();
    CVector2 botPosition;

    std::fill(std::begin(botCount), std::end(botCount), 0);

    for(auto& bot : bots) {
        bot->GetEmbodiedEntity().GetOriginAnchor().Position.ProjectOntoXY(botPosition);
        if(blackCircles[0].containsPoint(botPosition)) botCount[0] += 1;
        if(blackCircles[1].containsPoint(botPosition)) botCount[1] += 1;
        if(blackCircles[2].containsPoint(botPosition)) botCount[2] += 1;
    }
}

void LoopTask3::Reset() {
    BaseLoop::Reset();
    std::fill(std::begin(botCount), std::end(botCount), 0);
}

double LoopTask3::Evaluate() {
    auto max = *std::max_element(std::begin(botCount), std::end(botCount));
    return max / (double) bots.size();
}

Rect LoopTask3::loadSpawnArea(TConfigurationNode &t_tree) {
    return SPAWN_AREA;
}

bool LoopTask3::forbiddenSpawn(CVector2 &position) {
    return isInsideBlackZone(position);
}

vector<Circle> LoopTask3::loadBlackZones(TConfigurationNode &t_tree) {
    CVector3 tmpCircle;
    vector<Circle> blackCircles;
    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE1, tmpCircle, CIRCLE1);
    blackCircles.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });

    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE2, tmpCircle, CIRCLE2);
    blackCircles.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });

    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE3, tmpCircle, CIRCLE3);
    blackCircles.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });

    std::fill(std::begin(botCount), std::end(botCount), 0);

    return blackCircles;
}

REGISTER_LOOP_FUNCTIONS(LoopTask3, "loop_task3")