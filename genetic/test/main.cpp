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
const char* USER_CONFIG = "experiments/task3/config.json";
const char* ARGOS_EXPERIMENT = "experiments/task3/task3-test.argos";
const char* UNDER_TEST = "statistics/task3/task3_nodes50_k3_bias0.21_prox0_p80_g100_pM0.01_pC0.1_el5_trials3_14-01_23-01_best_all.csv";
const char* RESULT_FILENAME = "statistics/task3/result_task3_nodes50_k3_bias0.21_prox0_p80_g100_pM0.01_pC0.1_el5_trials3_14-01_23-01";

int main() {

    config::ExperimentConfiguration userconfig = config::create_from_file(USER_CONFIG);
    auto expfilename = string(ARGOS_EXPERIMENT);
    auto resultFilename = string(RESULT_FILENAME);

    auto best = BooleanNetworkLoader::loadBestBooleanFunctions(UNDER_TEST);

    testing::evaluate(best, userconfig, expfilename, resultFilename, RUNS);

    return 0;
}