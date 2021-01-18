//
// Created by andrea on 04/01/21.
//

#ifndef SWARM_GEN_FOOTBOT_H
#define SWARM_GEN_FOOTBOT_H

#include <vector>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_motor_ground_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>

using namespace std;
using namespace argos;

namespace footboot {

    vector<double> readProximityValues(const CCI_FootBotProximitySensor& proximitySensor, int groupSensor);

    vector<double> readMotorGroundValues(const CCI_FootBotMotorGroundSensor& groundSensor);

    vector<bool> sensorValuesToBooleans(const std::vector<double>& values, double threshold, bool invert = false);

    void moveByBooleans(CCI_DifferentialSteeringActuator& wheels, const vector<bool>& output, double speed);
}

#endif //SWARM_GEN_FOOTBOT_H
