//
// Created by Andrea Petreti on 20/01/2021.
//

#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <loop_function/evolution/BaseLoop.h>
#include <genetic/core/PerformanceLogger.h>
#include <core/BooleanNetworkLoader.h>
#include "testing.h"

const int RUNS = 30;
const char* USER_CONFIG = "experiments/task2/config.json";
const char* ARGOS_EXPERIMENT = "experiments/task2/task2.argos";
const char* UNDER_TEST = "_best_all.csv";
const char* RESULT_FILENAME = "statistics/task2/result_task2_nodes50_k3_bias0.21_prox1.5_p100_g80_pM0.01_pC0.1_el5_trials3_13-48_22-01_best_all.csv";

int main() {

    config::ExperimentConfiguration userconfig = config::create_from_file(USER_CONFIG);
    auto expfilename = string(ARGOS_EXPERIMENT);
    auto resultFilename = string(RESULT_FILENAME);

    auto best = BooleanNetworkLoader::loadBestBooleanFunctions(UNDER_TEST);

    testing::evaluate(best, userconfig, expfilename, resultFilename, RUNS);

    return 0;
}