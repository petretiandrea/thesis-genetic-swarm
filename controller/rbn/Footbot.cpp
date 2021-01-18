//
// Created by andrea on 04/01/21.
//

#include "Footbot.h"
#include <boost/range/adaptor/transformed.hpp>
#include <utility/Utility.h>

vector<double> footboot::readProximityValues(const CCI_FootBotProximitySensor& proximitySensor, int groupSensor) {
    auto readings = proximitySensor.GetReadings();
    vector<double> groupedValued;
    groupedValued.reserve(groupSensor);
    int groupRange = readings.size() / groupSensor;
    for(int i = 0; i < groupSensor; i++) {
        // sum policy
        double sum = 0;
        for(int j = 0; j < groupRange; j++) {
            sum += readings[(i * groupRange) + j].Value;
        }
        groupedValued.push_back(sum);
    }
    return groupedValued;
}

vector<double> footboot::readMotorGroundValues(const CCI_FootBotMotorGroundSensor& groundSensor) {
    return boost::copy_range<vector<double>>(
            groundSensor.GetReadings() | boost::adaptors::transformed([](CCI_FootBotMotorGroundSensor::SReading reading) {
                return reading.Value;
            }));
}

vector<bool> footboot::sensorValuesToBooleans(const std::vector<double>& values, double threshold, bool invert) {
    return boost::copy_range<vector<bool>>(
            values | boost::adaptors::transformed([invert, threshold](double value) {
                return (invert) ? value <= threshold : value > threshold;
            }));
}

void footboot::moveByBooleans(CCI_DifferentialSteeringActuator& wheels, const vector<bool>& output, double speed) {
    wheels.SetLinearVelocity(((double) utility::boolToInt(output[0])) * speed, ((double) utility::boolToInt(output[1])) * speed);
}