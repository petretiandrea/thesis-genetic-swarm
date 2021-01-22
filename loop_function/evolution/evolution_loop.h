//
// Created by andrea on 04/01/21.
//

#ifndef SWARM_GEN_EVOLUTION_LOOP_H
#define SWARM_GEN_EVOLUTION_LOOP_H

#include <utility/Matrix.h>
#include <vector>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <controller/rbn/BNController.h>
#include "EvaluationFunctions.h"
#include "Circle.h"
#include "Rect.h"

#define KEY_CIRCLE1 "circle1"
#define KEY_CIRCLE2 "circle2"
#define KEY_FOOTBOT_NUMBER "n_footbot"
#define KEY_VISUALIZATION "visualization"
#define KEY_TRIAL "trial"

using namespace argos;
using namespace std;

class EvolutionLoop : public CLoopFunctions {

public:
    static const char* LABEL;

public:
    EvolutionLoop();
    ~EvolutionLoop() override;

    void Init(TConfigurationNode &t_tree) override;

    /* Allow to choose what color render, useful to draw floor shape */
    CColor GetFloorColor(const CVector2 &c_pos_on_floor) override;

    /**
     * Calculate all random location for x trials
     * @param nTrials
     */
    void GenerateRandomSpawnLocation(int nTrials);

    /**
     * Setup the environment for start the nth trial. It spawn footbot at random location (previously generated)
     * @param nTrial
     */
    void PrepareForTrial(int nTrial);

    void PostExperiment() override;

    void ConfigureFromGenome(const vector<bool>& genome);

    double CalculateEvaluation();
    double MaxRobotCount();

    void Reset() override;

private:
    struct SInitSetup {
        CVector3 Position;
        CQuaternion Orientation;
    };

    Matrix<SInitSetup> initialSpawnLocations;
    CRandom::CRNG* randomGenerator;
    vector<Circle> blackCircles;
    vector<CFootBotEntity*> bots;
    vector<BNController*> controllers;
    int currentTrial;
    evaluation::EvaluationFunction evaluationFunction;
    int botCountInsideCircle;

private:
    bool CheckCollision(CVector3& position, const std::vector<SInitSetup>& botLocations);
    vector<SInitSetup> ComputeSpawnLocations(int nTrial, const Rect& spawnArea);
    CVector3 GenerateLocationWithoutCollision(int maxAttempts, const std::vector<SInitSetup>& botLocations, const Rect& spawnArea);
    bool IsInsideCircles(const CVector2& point);
};

#endif //SWARM_GEN_EVOLUTION_LOOP_H
