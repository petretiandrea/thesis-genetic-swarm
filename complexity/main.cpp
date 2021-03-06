//
// Created by Andrea Petreti on 27/01/2021.
//

#include "Complexity.h"
#include <iostream>

#include <core/BooleanNetworkLoader.h>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign.hpp>
#include <utility>
#include <vector>
#include <core/SolutionLoader.h>
#include <boost/unordered_set.hpp>
#include <genetic/test/testing.h>
#include <argos3/core/simulator/simulator.h>
#include <core/configuration/ArgosExperiment.h>
#include <core/configuration/ConfigurationJson.h>
#include <loop_function/evolution/BaseLoop.h>
#include <iostream>

#define COMPLEXITY_STATISTICS_BASENAME "statistics/complexity/task3/task3_30_f_"
#define GENERATIONS_FILENAME "statistics/task3/task3_nodes50_k3_bias0.21_prox1.5_p80_g100_pM0.01_pC0_el5_trials3_10-17_23-01.csv"
#define USER_CONFIG  "experiments/task3/config.json"
#define EXPERIMENT_FILE "experiments/task3/task3-test.argos"
#define NETWORKS 5
#define TRIALS 15

using namespace std;
using namespace boost::assign;
using namespace boost::adaptors;

vector<Solution> distinct(vector<Solution>& vector);
bool highestFitness(Solution& current, Solution& toCompare);
std::function<bool(const Solution&)> targetGeneration(int generation);

struct ComplexityData {
    int trial;
    double fitness;
    map<RobotID, vector<vector<bool>>> sensorTuples;
    map<RobotID, vector<int>> motorTuples;
};

void evaluate_and_dump(vector<Solution>& solutions, argos::CSimulator& simulator, BaseLoop& loop, int trials, const string& basename);
vector<ComplexityData> evaluate_solution(Solution& solution, argos::CSimulator& simulator, BaseLoop& loop, int trials);

int main() {

    vector<Solution> best;
    vector<Solution> middle;
    vector<Solution> worst;

    auto booleanFunctions = SolutionLoader::loadSolutionsLog(GENERATIONS_FILENAME);

    // getting the worst from 0-th generation
    boost::copy(*booleanFunctions | filtered(targetGeneration(0)), back_inserter(worst));
    boost::copy(*booleanFunctions | filtered(targetGeneration(30)), back_inserter(middle));
    boost::copy(*booleanFunctions | filtered(targetGeneration(100)), back_inserter(best));

    worst = distinct(worst);
    middle = distinct(middle);
    best = distinct(best);

    cout << "Found " << worst.size() << " distinct worst networks" << endl;
    cout << "Found " << middle.size() << " distinct middle networks" << endl;
    cout << "Found " << best.size() << " distinct best networks" << endl;

    delete booleanFunctions;

    // ordering
    sort(worst.begin(), worst.end(), highestFitness);
    sort(middle.begin(), middle.end(), highestFitness);
    sort(best.begin(), best.end(), highestFitness);

    /*worst.erase(std::unique(worst.begin(), worst.end(), [](const Solution& sol1, const Solution& sol2) {
        return fabs(sol1.fitness - sol2.fitness) < std::numeric_limits<double>::epsilon();
    }), worst.end());

    middle.erase(std::unique(middle.begin(), middle.end(), [](const Solution& sol1, const Solution& sol2) {
        return fabs(sol1.fitness - sol2.fitness) < std::numeric_limits<double>::epsilon();
    }), middle.end());

    best.erase(std::unique(best.begin(), best.end(), [](const Solution& sol1, const Solution& sol2) {
        return fabs(sol1.fitness - sol2.fitness) < std::numeric_limits<double>::epsilon();
    }), best.end());*/

    cout << best.size() <<endl;

    // getting: first n-th worst, first n best, last 5 of last generation
    vector<Solution>(worst.begin(), worst.begin() + NETWORKS).swap(worst);
    vector<Solution>(best.begin(), best.begin() + NETWORKS).swap(best);
    vector<Solution>(middle.end() - NETWORKS, middle.end()).swap(middle);

    //cout.setf(ios::scientific);
    for(auto& net : best) {
        cout << "Best network with " << net.fitness << " fitness " << net.solution.size() << endl;
    }

    for(auto& net : middle) {
        cout << "Middle network with " << net.fitness << " fitness " << net.solution.size() << endl;
    }

    for(auto& net : worst) {
        cout << "Worst network with " << net.fitness << " fitness " << net.solution.size() << endl;
    }

    // create argos simulator
    argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    config::ExperimentConfiguration experiment = config::create_from_file(USER_CONFIG);
    config::load_experiment_config(simulator, EXPERIMENT_FILE, experiment);

    BaseLoop* loop = dynamic_cast<BaseLoop*>(&simulator.GetLoopFunctions());
    loop->GenerateRandomSpawnLocation(TRIALS);

    // evaluate_solution(middle[2], simulator, *loop, 10);

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

    for(int i = 0; i < solutions.size(); i++) {
        ofstream oo(basename + "_" + to_string(i) + ".csv");
        oo << "solution_fitness;trial;fitness;bot_id;sensor_tuples;motor_value" << flush << endl;

        auto results = evaluate_solution(solutions[i], simulator, loop, trials);

        for(auto& result : results) {
            for(auto& entropy : result.sensorTuples) {
                auto robot_id = entropy.first;
                oo << solutions[i].fitness << ";"
                   << result.trial << ";"
                   << result.fitness << ";"
                   << robot_id << ";"
                   << result.sensorTuples[robot_id] << ";"
                   << result.motorTuples[robot_id] << flush << endl;
            }
        }

        oo.close();
    }
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

        ComplexityData data {
            .trial = trial,
            .fitness = loop.Evaluate(),
            .sensorTuples = loop.GetComplexityMeasures(),
            .motorTuples = loop.GetMotorMeasures()
        };

        /*auto robotResults = loop.GetComplexityMeasures();
        for(auto& robot : robotResults) {
            data.entropies[robot.first] = complexity::shannon2(robot.second);
        }*/

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