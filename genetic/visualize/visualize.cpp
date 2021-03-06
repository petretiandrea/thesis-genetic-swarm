//
// Created by Andrea Petreti on 20/01/2021.
//

#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <loop_function/evolution/BaseLoop.h>

int main() {

    static argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    static config::ExperimentConfiguration userconfig = config::create_from_file("experiments/task4/config.json");

    config::load_experiment_config(simulator, "experiments/task4/task4-viz.argos", userconfig);

    simulator.Reset();
    simulator.Execute();
    simulator.Destroy();

    return 0;
}