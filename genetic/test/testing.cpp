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
        loop->PrepareForTrial(i);
        simulator.Reset();
        simulator.Reset();
        simulator.Execute();

        auto score = loop->Evaluate();
        performance.push_back(score);
        cout << "Performance " << score << endl;
    }
    simulator.Destroy();

    PerformanceLogger::saveTestPerformance(resultFilename, performance);
}