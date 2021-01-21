//
// Created by Andrea Petreti on 20/01/2021.
//

#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <loop_function/evolution/evolution_loop.h>
#include <genetic/core/PerformanceLogger.h>
#include "testing.h"

const int RUNS = 30;
const char* USER_CONFIG = "experiments/task1.json";
const char* ARGOS_EXPERIMENT = "experiments/task1-test.argos";
const char* RESULT_FILENAME = "statistics/result_prova.csv";

int main() {

    config::ExperimentConfiguration userconfig = config::create_from_file(USER_CONFIG);
    auto expfilename = string(ARGOS_EXPERIMENT);
    auto resultFilename = string(RESULT_FILENAME);

    testing::runTest(userconfig, expfilename, resultFilename, RUNS);

    return 0;
}