//
// Created by Andrea Petreti on 19/01/2021.
//

#ifndef SWARM_GEN_PERFORMANCELOGGER_H
#define SWARM_GEN_PERFORMANCELOGGER_H

#include <ga/ga.h>
#include <iostream>
#include <core/configuration/ExperimentConfiguration.h>
#include <vector>

using namespace std;

class PerformanceLogger {

private:
    ofstream generationsFile;
    ofstream bestFile;
    string bestFilePath;

public:
    PerformanceLogger(const string& folder, const string& basename);
    void saveStatistics(const GAPopulation &population);

    static string statisticsBasenameFromConfiguration(const config::ExperimentConfiguration& configuration, const string& prefix = "");
    static void saveTestPerformance(const string& filename, std::vector<double>& testPerformance);

private:
    static void saveIndividual(ofstream& stream, int generation, GAGenome& genome);
};

#endif //SWARM_GEN_PERFORMANCELOGGER_H
