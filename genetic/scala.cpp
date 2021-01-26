//
// Created by Andrea Petreti on 21/01/2021.
//

#include <iostream>
#include <string>
#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <loop_function/evolution/BaseLoop.h>
#include <utility/Utility.h>

using namespace std;

int main(int argc, char* argv[]) {

    auto userConfig = config::create_from_file(argv[1]);

    argos::CSimulator& simulator = argos::CSimulator::GetInstance();

    config::load_experiment_config(simulator, argv[2], userConfig);
    auto* loop = dynamic_cast<BaseLoop*>(&simulator.GetLoopFunctions());
    loop->GenerateRandomSpawnLocation(3);


    while (true) {
        std::string genome;
        int trials;

        cin >> genome;
        cin >> trials;

        loop->ConfigureFromGenome(utility::boolVectorFromString(genome));
        auto performance = 0.0;
        for(int i = 0; i < trials; i++) {
            loop->PrepareForTrial(i);
            simulator.Reset();
            simulator.Reset();
            simulator.Execute();

            performance += loop->Evaluate();
        }

        cout << (performance / trials);
    }

}