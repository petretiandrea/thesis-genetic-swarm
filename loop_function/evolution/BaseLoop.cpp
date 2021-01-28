//
// Created by Andrea Petreti on 26/01/2021.
//

#include <constants.h>
#include <utility/Utility.h>
#include "BaseLoop.h"

#define KEY_FOOTBOT_NUMBER "n_footbot"
#define KEY_VISUALIZATION "visualization"
#define KEY_TRIAL "trial"

BaseLoop::BaseLoop() :
    initialSpawnLocations(),
    rnd(nullptr),
    currentTrial(0),
    inputTuples() { }

BaseLoop::~BaseLoop() = default;

void BaseLoop::Init(TConfigurationNode &t_tree) {
    CLoopFunctions::Init(t_tree);

    // load black area
    auto blackZones = loadBlackZones(t_tree);
    for(auto zone : blackZones) {
        blackCircles.push_back(zone);
    }

    spawnArea = this->loadSpawnArea(t_tree);

    int footbotNumber;
    GetNodeAttribute(t_tree, KEY_FOOTBOT_NUMBER, footbotNumber);

    bool visualization_mode;
    GetNodeAttributeOrDefault(t_tree, KEY_VISUALIZATION, visualization_mode, false);

    rnd = CRandom::CreateRNG("argos");

    for(int i = 0; i < footbotNumber; i++) {
        auto bot = new CFootBotEntity("fb_" + std::to_string(i), "bn");
        bots.push_back(bot);
        controllers.push_back(&dynamic_cast<BNController&>(bot->GetControllableEntity().GetController()));
        AddEntity(*bot);
    }

    // if visualization is enabled generate n trials and prepare for n-th trial
    if(visualization_mode) {
        int trial;
        GetNodeAttribute(t_tree, KEY_TRIAL, trial);
        this->GenerateRandomSpawnLocation(trial + 1);
        this->PrepareForTrial(trial);
    }
}

CColor BaseLoop::GetFloorColor(const CVector2 &c_pos_on_floor) {
    if (isInsideBlackZone(c_pos_on_floor)) {
        return CColor::BLACK;
    }
    return CLoopFunctions::GetFloorColor(c_pos_on_floor);
}

void BaseLoop::ConfigureFromGenome(const vector<bool> &genome) {
    auto booleanMatrix = utility::vectorToMatrix(genome,
                                                 controllers.back()->associatedNetwork().getBooleanFunctions().getRows(),
                                                 controllers.back()->associatedNetwork().getBooleanFunctions().getColumns());

    //cout << "Matrix update " << endl;
    //cout << booleanMatrix << endl;

    for(auto& controller : controllers) {
        controller->associatedNetwork().changeBooleanFunction(booleanMatrix);
    }
}

void BaseLoop::PrepareForTrial(int nTrial) {
    this->currentTrial = nTrial;
}

bool BaseLoop::isInsideBlackZone(const CVector2 &point)  {
    return std::any_of(blackCircles.begin(), blackCircles.end(), [point](Circle& circle) { return circle.containsPoint(point); });
}

void BaseLoop::Reset() {
    CLoopFunctions::Reset();
    for(int i = 0; i < this->initialSpawnLocations.getColumns(); i++) {
        auto location = initialSpawnLocations(currentTrial, i);
        auto bot = bots[i];

        if(!MoveEntity(bot->GetEmbodiedEntity(), location.Position, location.Orientation, false, true)) {
            //std::cerr << "Error for bot: " << bot->GetId() << " on trial: " << currentTrial << " reason: " << " cannot move entity " << std::endl;
        }
    }
    inputTuples.clear();
}

// Spawning methods
void BaseLoop::GenerateRandomSpawnLocation(int nTrials) {

    this->initialSpawnLocations = Matrix<SInitSetup>(nTrials, bots.size());

    for (int i = 0; i < nTrials; i++) {
        // compute all locations for trials
        auto locations = ComputeSpawnLocations(i, this->spawnArea);
        for(int j = 0; j < locations.size(); j++) {
            this->initialSpawnLocations.put(i, j, locations[j]);
        }
    }
}

vector<SInitSetup> BaseLoop::ComputeSpawnLocations(int nTrial, const Rect& spawnArea) {
    // spawn bot and generate position
    CRadians orientation;
    std::vector<SInitSetup> botLocations;
    for(auto& bot : bots) {
        try {
            SInitSetup spawnLocation;
            orientation = rnd->Uniform(CRadians::UNSIGNED_RANGE);
            spawnLocation.Orientation.FromEulerAngles(
                    orientation,        // rotation around Z
                    CRadians::ZERO,     // rotation around Y
                    CRadians::ZERO      // rotation around X
            );

            CVector3 position = GenerateLocationWithoutCollision(constants::MAX_ATTEMPTS_LOCATION_SPAWN, botLocations, spawnArea);
            spawnLocation.Position.Set(position.GetX(), position.GetY(), position.GetZ());
            botLocations.push_back(spawnLocation);
        } catch (std::logic_error& e) {
            std::cerr << "Error for bot: " << bot->GetId() << " on trial: " << nTrial << " reason: " << e.what() << std::endl;
        }
    }
    return botLocations;
}

CVector3 BaseLoop::GenerateLocationWithoutCollision(int maxAttempts, const std::vector<SInitSetup>& botLocations, const Rect& spawnArea) {
    bool collision = true;
    CVector3 position;
    int currentAttempt;

    cout << "Width " << spawnArea.width() << " Height " << spawnArea.height() << endl;

    CRange<Real> xRange = {
            spawnArea.topLeft.GetX() + constants::FOOTBOT_RADIUS,
            spawnArea.downRight.GetX() - constants::FOOTBOT_RADIUS
    };
    CRange<Real> yRange = {
            spawnArea.downRight.GetY() + constants::FOOTBOT_RADIUS,
            spawnArea.topLeft.GetY() - constants::FOOTBOT_RADIUS
    };

    cout << "x: " << xRange << endl;
    cout << "y: " << yRange << endl;
    /*double rangeSize = (constants::ARENA_SIDE_SIZE - 2 * constants::FOOTBOT_RADIUS) / 2;
    CRange<Real> rangeArena(-rangeSize, rangeSize);*/
    for(currentAttempt = 0; collision && currentAttempt < maxAttempts; currentAttempt++) {
        position.SetZ(0);
        position.SetX(rnd->Uniform(xRange));
        position.SetY(rnd->Uniform(yRange));
        collision = CheckCollision(position, botLocations);
    }
    if(currentAttempt >= maxAttempts) {
        throw std::logic_error("Total attempts reached for generate random location.");
    } else {
        return position;
    }
}

bool BaseLoop::CheckCollision(CVector3& position, const std::vector<SInitSetup>& botLocations) {
    // check collision with robots
    for(const auto& botLocation : botLocations) {
        //(r1+r2)^2 >= (x2-x1)^2+(y2-y1)^2
        double xDiff = position.GetX() - botLocation.Position.GetX();
        double yDiff = position.GetY() - botLocation.Position.GetY();
        double centerDiff = pow(xDiff, 2) + pow(yDiff, 2);
        if(pow(2 * constants::FOOTBOT_RADIUS, 2) >= centerDiff) return true;
    }

    // check collision over circles
    auto spawnLocation = CVector2(position.GetX(), position.GetY());
    if(forbiddenSpawn(spawnLocation)) return true;
    return false;
}

void BaseLoop::PostStep() {
    CLoopFunctions::PostStep();
    if(!collectComplexityMeasures) return;

    // enabled complexity measure collecting
    for(auto* controller : controllers) {
        inputTuples[controller->GetId()].push_back(controller->getLastInputTuple());
    }
}

void BaseLoop::EnableComplexityMeasures() {
    collectComplexityMeasures = true;
    for (auto* controller : controllers) {
        controller->enableComplexityMeasurement();
    }
}


