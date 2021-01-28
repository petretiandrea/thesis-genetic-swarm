//
// Created by Andrea Petreti on 27/01/2021.
//

#include "Complexity.h"
#include <iostream>

#include <core/BooleanNetworkLoader.h>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign.hpp>
#include <vector>
#include <core/SolutionLoader.h>
#include <boost/unordered_set.hpp>
#include <genetic/test/testing.h>
#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <core/configuration/ConfigurationJson.h>
#include <loop_function/evolution/BaseLoop.h>
#include <iostream>

#define COMPLEXITY_STATISTICS_BASENAME "statistics/complexity/task1/task1_"
#define GENERATIONS_FILENAME "statistics/task1/task1_nodes50_k3_bias0.21_prox1.5_p80_g100_pM0.01_pC0_el5_trials3_10-41_20-01.csv"
#define USER_CONFIG  "experiments/task1/config.json"
#define EXPERIMENT_FILE "experiments/task1/task1-test.argos"
#define NETWORKS 5
#define TRIALS 10

using namespace std;
using namespace boost::assign;
using namespace boost::adaptors;

vector<Solution> distinct(vector<Solution>& vector);
bool highestFitness(Solution& current, Solution& toCompare);
std::function<bool(const Solution&)> targetGeneration(int generation);

struct ComplexityData {
    int trial;
    double fitness;
    map<RobotID, double> entropies;
};

void evaluate_and_dump(vector<Solution>& solutions, argos::CSimulator& simulator, BaseLoop& loop, int trials, const string& basename);
vector<pair<Solution, vector<ComplexityData>>> evaluate_solutions(vector<Solution>& solutions, argos::CSimulator& simulator, BaseLoop& loop, int trials);
vector<ComplexityData> evaluate_solution(Solution& solution, argos::CSimulator& simulator, BaseLoop& loop, int trials);


int main() {

    vector<Solution> best;
    vector<Solution> middle;
    vector<Solution> worst;

    auto booleanFunctions = SolutionLoader::loadSolutionsLog(GENERATIONS_FILENAME);

    // getting the worst from 0-th generation
    vector<Solution> lastGen;
    boost::copy(*booleanFunctions | filtered(targetGeneration(1)), back_inserter(worst));
    boost::copy(*booleanFunctions | filtered(targetGeneration(100)), back_inserter(lastGen));

    worst = distinct(worst);
    lastGen = distinct(lastGen);

    delete booleanFunctions;

    // ordering
    sort(worst.begin(), worst.end(), highestFitness);
    sort(lastGen.begin(), lastGen.end(), highestFitness);

    // getting: first n-th worst, first n best, last 5 of last generation
    vector<Solution>(worst.begin(), worst.begin() + NETWORKS).swap(worst);
    vector<Solution>(lastGen.begin(), lastGen.begin() + NETWORKS).swap(best);
    vector<Solution>(make_reverse_iterator(lastGen.end()),make_reverse_iterator(lastGen.end() - NETWORKS)).swap(middle);

    // create argos simulator
    argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    config::ExperimentConfiguration experiment = config::create_from_file(USER_CONFIG);
    config::load_experiment_config(simulator, EXPERIMENT_FILE, experiment);

    BaseLoop* loop = dynamic_cast<BaseLoop*>(&simulator.GetLoopFunctions());
    loop->GenerateRandomSpawnLocation(TRIALS);

    map<string, vector<Solution>> types = { { "best", best }, { "middle", middle }, { "worst", worst } };
    for (auto& type : types) {
        evaluate_and_dump(type.second, simulator, *loop, TRIALS, string(COMPLEXITY_STATISTICS_BASENAME) + type.first);
    }

    return 0;
};

void evaluate_and_dump(vector<Solution>& solutions,
                       argos::CSimulator& simulator,
                       BaseLoop& loop,
                       int trials,
                       const string& basename) {

    auto results = evaluate_solutions(solutions, simulator, loop, trials);

    for(int i = 0; i < results.size(); i++) {
        ofstream oo(basename + "_" + to_string(i) + ".csv");
        oo << "solution_fitness;trial;fitness;bot_id;entropy" << flush << endl;

        auto& solutionsResult = results[i];
        for(auto& result : solutionsResult.second) {
            for(auto& entropy : result.entropies) {
                oo << solutionsResult.first.fitness << ";"
                   << result.trial << ";"
                   << result.fitness << ";"
                   << entropy.first << ";"
                   << std::scientific << entropy.second << flush << endl;
            }
        }

        oo.close();
    }
}

vector<pair<Solution, vector<ComplexityData>>> evaluate_solutions(vector<Solution>& solutions,
                                           argos::CSimulator& simulator,
                                           BaseLoop& loop,
                                           int trials) {
    vector<pair<Solution, vector<ComplexityData>>> results;

    for(auto& solution : solutions) {
        auto result = evaluate_solution(solution, simulator, loop, trials);
        results.push_back(pair<Solution, vector<ComplexityData>>(solution, result));
    }

    return results;
}

vector<ComplexityData> evaluate_solution(Solution& solution, argos::CSimulator& simulator, BaseLoop& loop, int trials) {
    vector<ComplexityData> results;

    loop.EnableComplexityMeasures();
    loop.ConfigureFromGenome(solution.solution);
    for(int trial = 0; trial < trials; trial++) {

        loop.PrepareForTrial(trial);
        simulator.Reset();
        simulator.Reset();
        simulator.Execute();

        ComplexityData data { .trial = trial, .fitness = loop.Evaluate() };

        auto robotResults = loop.GetComplexityMeasures();
        for(auto& robot : robotResults) {
            data.entropies[robot.first] = complexity::shannon2(robot.second);
        }

        results.push_back(data);
    }
    return results;
}

vector<Solution> distinct(vector<Solution>& vector) {
    struct Hasher {
        std::size_t operator()(const Solution& solution) const {
            stringstream ss;
            ss << solution.solution;
            return boost::hash_value(ss.str());
        }
    };

    boost::unordered_set<Solution, Hasher> myset(vector.begin(), vector.end());
    std::vector<Solution> vec(myset.begin(), myset.end());
    return vec;
}

bool highestFitness(Solution& current, Solution& toCompare) {
    return current.fitness > toCompare.fitness;
}

std::function<bool(const Solution&)> targetGeneration(int generation) {
    return [generation](const Solution& sol) {
        return sol.generation == generation;
    };
}

