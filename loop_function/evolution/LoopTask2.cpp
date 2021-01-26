//
// Created by Andrea Petreti on 26/01/2021.
//

#include "LoopTask2.h"

#define KEY_CIRCLE1 "circle1"
#define KEY_CIRCLE2 "circle2"

#include "EvaluationFunctions.h"


LoopTask2::LoopTask2() : botCount {0, 0} {}

void LoopTask2::PostExperiment() {
    CLoopFunctions::PostExperiment();
    CVector2 botPosition;

    std::fill(std::begin(botCount), std::end(botCount), 0);

    for(auto& bot : bots) {
        bot->GetEmbodiedEntity().GetOriginAnchor().Position.ProjectOntoXY(botPosition);
        if(blackCircles[0].containsPoint(botPosition)) botCount[0] += 1;
        if(blackCircles[1].containsPoint(botPosition)) botCount[1] += 1;
    }
}

void LoopTask2::Reset() {
    BaseLoop::Reset();
    std::fill(std::begin(botCount), std::end(botCount), 0);
}

double LoopTask2::Evaluate() {
    auto eval = evaluation::triangular(0, bots.size());

    auto count1 = eval(botCount[0]) * (bots.size() / 2);
    auto count2 = eval(botCount[1]) * (bots.size() / 2);

    //cout << "C1 " << count1 << " C2 " << count2 << endl;

    return (count1 + count2) / 2;
}

vector<Circle> LoopTask2::loadBlackZones(TConfigurationNode &t_tree) {
    CVector3 tmpCircle;
    vector<Circle> blackCircles;
    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE1, tmpCircle, CIRCLE1);
    blackCircles.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });

    GetNodeAttributeOrDefault(t_tree, KEY_CIRCLE2, tmpCircle, CIRCLE2);
    blackCircles.push_back(Circle { tmpCircle.GetX(), tmpCircle.GetY(), tmpCircle.GetZ() });

    std::fill(std::begin(botCount), std::end(botCount), 0);

    return blackCircles;
}

Rect LoopTask2::loadSpawnArea(TConfigurationNode &t_tree) {
    return SPAWN_AREA;
}

bool LoopTask2::forbiddenSpawn(CVector2 &position) {
    return isInsideBlackZone(position);
}

REGISTER_LOOP_FUNCTIONS(LoopTask2, "loop_task2")
