//
// Created by andrea on 04/01/21.
//

#include <constants.h>
#include "BNController.h"
#include "Footbot.h"
#include <core/configuration/config.h>
#include <core/BooleanNetworkLoader.h>

#define INPUT_NODE 12
#define OUTPUT_NODE 2

BNController::BNController() : wheels(nullptr), proximity(nullptr), motorGround(nullptr), booleanNetwork(nullptr), lastInputTuple() {}

BNController::~BNController() {
    delete booleanNetwork;
}

void BNController::Init(TConfigurationNode &t_node) {
    try {
        wheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
        proximity = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
        motorGround = GetSensor<CCI_FootBotMotorGroundSensor>("footbot_motor_ground");
    } catch(CARGoSException& ex) {
        THROW_ARGOSEXCEPTION_NESTED("Error initializing sensors/actuators", ex);
    }

    motorGroundThreshold = t_node.GetAttribute<double>(config::key::MOTOR_GROUND_THRESHOLD);
    proximityThreshold = t_node.GetAttribute<double>(config::key::PROXIMITY_THRESHOLD);
    constantOutputSpeed = t_node.GetAttribute<double>(config::key::CONSTANT_SPEED);

    auto network_file = t_node.GetAttribute(config::key::BOOLEAN_NETWORK_FILE);

    booleanNetwork = new BooleanNetwork(
            t_node.GetAttribute<int>(config::key::NODES),
            t_node.GetAttribute<int>(config::key::INPUT_FOR_NODE),
            t_node.GetAttribute<double>(config::key::BIAS),
            INPUT_NODE,
            OUTPUT_NODE,
            constants::RANDOM_SEED);

    if(!network_file.empty()) {
        LoadFromFile(network_file);// TODO: load entire network, including structure
    }
}

#include <utility/Utility.h>

void BNController::LoadFromFile(const string &filename) {
    auto booleanFunctions = BooleanNetworkLoader::loadBestBooleanFunctions(filename);
    booleanNetwork->changeBooleanFunction(booleanFunctions);
}

void BNController::ControlStep() {
    auto proximityValues = footboot::readProximityValues(*proximity, 8);
    auto groundValues = footboot::readMotorGroundValues(*motorGround);

    auto proximityBooleans = footboot::sensorValuesToBooleans(proximityValues, this->proximityThreshold);
    auto groundBooleans = footboot::sensorValuesToBooleans(groundValues, this->motorGroundThreshold, true);

    for(int i = 0; i < proximityBooleans.size(); i++) {
        booleanNetwork->forceInputValue(i, proximityBooleans[i]);
    }

    for(int i = 0, offset = proximityBooleans.size(); i < groundBooleans.size(); i++) {
        booleanNetwork->forceInputValue(i + offset, groundBooleans[i]);
    }

    if(complexityMeasureEnabled) {
        lastInputTuple.clear();
        lastInputTuple.insert(lastInputTuple.end(), proximityBooleans.begin(), proximityBooleans.end());
        lastInputTuple.insert(lastInputTuple.end(), groundBooleans.begin(), groundBooleans.end());
    }

    booleanNetwork->update();
    footboot::moveByBooleans(*wheels, booleanNetwork->getOutputValues(), this->constantOutputSpeed);
}

void BNController::Reset() {
    CCI_Controller::Reset();
    booleanNetwork->resetStates();
    lastInputTuple.clear();
}

void BNController::Destroy() { }

REGISTER_CONTROLLER(BNController, "bn_controller")