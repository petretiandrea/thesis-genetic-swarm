//
// Created by Andrea Petreti on 20/01/2021.
//

#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <loop_function/evolution/evolution_loop.h>

int main() {

    static argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    static config::ExperimentConfiguration userconfig = config::create_from_file("experiments/task2.json");

    config::load_experiment_config(simulator, "experiments/check-arena.argos", userconfig);

    static auto* loop = dynamic_cast<EvolutionLoop*>(&simulator.GetLoopFunctions());

    loop->GenerateRandomSpawnLocation(1);
    loop->PrepareForTrial(0);

    simulator.Reset();
    simulator.Execute();
    simulator.Destroy();

    return 0;
}