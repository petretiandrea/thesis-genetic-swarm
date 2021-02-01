//
// Created by andrea on 06/01/21.
//

#include "MPIEvaluator.h"

#include <argos3/core/simulator/simulator.h>
#include <loop_function/evolution/BaseLoop.h>
#include <genetic/core/BNGenome.h>
#include <csignal>
#include <utility/Utility.h>

#include <boost/mpi.hpp>

class EvaluatedHack : public GAGenome {
public:
    static bool isEvaluated(GAGenome* genome) {
        auto a = (EvaluatedHack*)(genome);
        return a->_evaluated;
    }
};


MPIEvaluator::MPIEvaluator(mpi::communicator& world) :
    world(world),
    parallelism(world.size() - 1) { }



void genomeToBoolArray(GA1DBinaryStringGenome& genome, vector<bool>* array) {
    for(int i = 0; i < genome.size(); i++) {
        array->push_back(genome.gene(i) == 1);
    }
}

std::function<void(GAPopulation&)> MPIEvaluator::populationEvaluator() {
    return [this](GAPopulation& population) { this->evaluatePopulation(population); };
}

void MPIEvaluator::evaluatePopulation(GAPopulation &population) {
    vector<int> needEvaluation;
    for(int i = 0; i < population.size(); i++) {
        auto& individual = population.individual(i);
        auto actualScore = EvaluatedHack::isEvaluated(&individual) ? individual.score() : -1;
        if (actualScore == -1) {
            needEvaluation.push_back(i);
        }
    }

    cout << "Need evaluations " << population.size() << endl;
    // send requests
    auto populationSlice = needEvaluation.size() / parallelism;
    auto populationOvers = needEvaluation.size() % parallelism;
    for(int i = 0; i < parallelism; i++) {
        auto populationIndexStart = (i * populationSlice);
        // assign to last one the leftovers population
        auto size = (populationOvers > 0 && i == parallelism - 1) ? populationSlice + populationOvers : populationSlice;

        vector<SharedBinaryGenome> toSend;
        for(int j = populationIndexStart; j < populationIndexStart + size; j++) {
            auto genomeIndex = needEvaluation[j];
            auto& boolGenome = dynamic_cast<GA1DBinaryStringGenome&>(population.individual(genomeIndex));
            vector<bool> genomeArray;
            genomeToBoolArray(boolGenome, &genomeArray);
            toSend.push_back(SharedBinaryGenome(genomeIndex, genomeArray));
        }

        cout << "Send to " << i + 1 << ": " << toSend.size() << endl;
        world.send(i + 1, PROTOCOL::GENOMES, toSend);
    }

    // collect responses
    for(int i = 0; i < parallelism; i++) {
        vector<ResultBinaryGenome> evaluations;

        cout <<  "Wait receive from " << i + 1 << endl;
        world.recv(i + 1, PROTOCOL::RESULT, evaluations);
        cout << "Received results from " << i + 1 << " " << evaluations.size() << endl;

        for(auto evaluation : evaluations) {
            population.individual(evaluation.genomeIndex).score((float) evaluation.fitness);
        }
    }
}


[[noreturn]] void MPIEvaluator::slaveBehaviour(const config::GeneticConfiguration &configuration,
                                               ticpp::Document& experimentXML) {

    std::ofstream slaveLog(std::string("logs/ARGoS_LOG_" + ToString(::getpid())).c_str(), std::ios::out);
    LOG.DisableColoredOutput();
    LOG.GetStream().rdbuf(slaveLog.rdbuf());
    std::ofstream slaveLogErr(std::string("logs/ARGoS_LOGERR_" + ToString(::getpid())).c_str(), std::ios::out);
    LOGERR.DisableColoredOutput();
    LOGERR.GetStream().rdbuf(slaveLogErr.rdbuf());

    // setup and load experiment
    cout << "Slave started " << world.rank() << endl;
    argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    simulator.Load(experimentXML);
    auto experiment = bngenome::create_context(configuration, simulator);

    experiment.loop.GenerateRandomSpawnLocation(configuration.n_trials);

    //auto& memory = workers.sharedMemory();

    while(true) {
        vector<SharedBinaryGenome> toEvaluate;
        world.recv(0, PROTOCOL::GENOMES, toEvaluate);

        //cout << world.rank() << " received " << toEvaluate.size() << " genomes to evaluate" << endl;

        vector<ResultBinaryGenome> results;
        for(auto genome : toEvaluate) {
            //cout << "Size " << genome.genome.size() << endl;
            double performance = bngenome::evaluatorByExperiment2(experiment)(genome.genome);
            results.push_back(ResultBinaryGenome(genome.genomeIndex, performance));
        }


        world.send(0, PROTOCOL::RESULT, results);

        //LOG.Flush();
        //LOGERR.Flush();
    }
}

std::function<double(GA1DBinaryStringGenome &)> MPIEvaluator::genomeEvaluator() {
    return [](GA1DBinaryStringGenome &genome) {
        return genome.score();
    };
}
