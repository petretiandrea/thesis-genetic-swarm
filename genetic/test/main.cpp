//
// Created by Andrea Petreti on 20/01/2021.
//

#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <loop_function/evolution/evolution_loop.h>
#include <genetic/core/PerformanceLogger.h>
#include "testing.h"

const int RUNS = 30;
const char* USER_CONFIG = "experiments/task2.json";
const char* ARGOS_EXPERIMENT = "experiments/task2-test.argos";
const char* RESULT_FILENAME = "statistics/task2/result_task2__nodes50_k3_bias0.21_prox1.5_p100_g80_pM0.01_pC0_el5_trials3_11-16_22-01_best_all.csv";

int main() {

    config::ExperimentConfiguration userconfig = config::create_from_file(USER_CONFIG);
    auto expfilename = string(ARGOS_EXPERIMENT);
    auto resultFilename = string(RESULT_FILENAME);

    testing::runTest(userconfig, expfilename, resultFilename, RUNS);

    return 0;
}