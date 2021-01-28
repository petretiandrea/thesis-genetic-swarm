//
// Created by Andrea Petreti on 20/01/2021.
//

#include "testing.h"
#include <loop_function/evolution/BaseLoop.h>
#include <genetic/core/PerformanceLogger.h>
#include <utility/Utility.h>

void testing::evaluate(const string& networkGenome,
                      config::ExperimentConfiguration& configuration,
                      const string& argosTestFilename,
                      const string& resultFilename,
                      int runs) {

    evaluate(utility::boolVectorFromString(networkGenome), configuration, argosTestFilename, resultFilename, runs);
}
#include <chrono>
void testing::evaluate(const vector<bool>& networkBooleanFunction,
                       const config::ExperimentConfiguration& configuration,
                       const string& argosTestFilename,
                       const string& resultFilename,
                       int runs) {

    argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    config::load_experiment_config(simulator, argosTestFilename.c_str(), configuration);
    auto* loop = dynamic_cast<BaseLoop*>(&simulator.GetLoopFunctions());
    loop->GenerateRandomSpawnLocation(runs);

    vector<double> performance;

    loop->ConfigureFromGenome(networkBooleanFunction);
    for(int i = 0; i < runs; i++) {

        auto t1 = std::chrono::high_resolution_clock::now();
        loop->PrepareForTrial(i);
        simulator.Reset();
        simulator.Reset();
        simulator.Execute();

        auto score = loop->Evaluate();
        performance.push_back(score);
        cout << "Performance " << score << endl;
        auto t2 = std::chrono::high_resolution_clock::now();

        cout << "Time " << std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count() << endl;

    }
    simulator.Destroy();

    PerformanceLogger::saveTestPerformance(resultFilename, performance);
}