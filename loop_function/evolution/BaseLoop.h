//
// Created by Andrea Petreti on 26/01/2021.
//

#ifndef SWARM_GEN_BASELOOP_H
#define SWARM_GEN_BASELOOP_H

#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <controller/rbn/BNController.h>
#include <utility/Matrix.h>
#include <vector>
#include "Circle.h"
#include "Rect.h"

struct SInitSetup {
    CVector3 Position;
    CQuaternion Orientation;
};

typedef vector<bool> InputTuple;
typedef string RobotID;

using namespace argos;
using namespace std;

class BaseLoop : public CLoopFunctions {

public:
    BaseLoop();
    ~BaseLoop() override;

    void Init(TConfigurationNode &t_tree) override;

    /* Allow to choose what color render, useful to draw floor shape */
    CColor GetFloorColor(const CVector2 &c_pos_on_floor) override;

    void Reset() override;

    void PrepareForTrial(int nTrial);

    void ConfigureFromGenome(const vector<bool>& genome);

    virtual double Evaluate() = 0;

    void GenerateRandomSpawnLocation(int nTrials);

    void EnableComplexityMeasures();

    inline map<RobotID , vector<InputTuple>> GetComplexityMeasures() { return inputTuples; }

    inline map<RobotID, vector<int>> GetMotorMeasures() { return motorMeasures; }

    void PostStep() override;

private:
    vector<SInitSetup> ComputeSpawnLocations(int nTrial, const Rect& spawnArea);
    CVector3 GenerateLocationWithoutCollision(int maxAttempts, const std::vector<SInitSetup>& botLocations, const Rect& spawnArea);
    bool CheckCollision(CVector3& position, const std::vector<SInitSetup>& botLocations);

protected:
    bool isInsideBlackZone(const CVector2& point);

    virtual vector<Circle> loadBlackZones(TConfigurationNode &t_tree) = 0;
    virtual Rect loadSpawnArea(TConfigurationNode &t_tree) = 0;
    virtual bool forbiddenSpawn(CVector2& position) = 0;

protected:
    CRandom::CRNG* rnd;
    Matrix<SInitSetup> initialSpawnLocations;
    vector<CFootBotEntity*> bots;
    vector<BNController*> controllers;
    vector<Circle> blackCircles;
    Rect spawnArea;
    int currentTrial;
    bool collectComplexityMeasures = false;
    map<string, vector<vector<bool>>> inputTuples;
    map<string, vector<int>> motorMeasures;
};

#endif //SWARM_GEN_BASELOOP_H
