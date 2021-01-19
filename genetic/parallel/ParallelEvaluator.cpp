//
// Created by andrea on 06/01/21.
//

#include "ParallelEvalutator.h"

#include <argos3/core/simulator/simulator.h>
#include <loop_function/evolution/evolution_loop.h>
#include <genetic/core/BNGenome.h>
#include <csignal>
#include <utility/Utility.h>

class EvaluatedHack : public GAGenome {
public:
    static bool isEvaluated(GAGenome* genome) {
        auto a = (EvaluatedHack*)(genome);
        return a->_evaluated;
    }
};

ParallelEvaluator::ParallelEvaluator(const config::ExperimentConfiguration& configuration,
                                     const char* argosExperimentFilename,
                                     int parallelism) :
    xmlExperiment(config::load_experiment_config(argosExperimentFilename, configuration)),
    memory(configuration.genetic_config.population_size, parallelism),
    workers(memory, parallelism),
    parallelism(parallelism) {


    workers.start([this, &configuration](int slaveId) {
        this->slaveBehaviour(slaveId, configuration.genetic_config, this->xmlExperiment);
    });
}

void genomeToShortArray(GA1DBinaryStringGenome& genome, short* array) {
    for(int i = 0; i < genome.size(); i++) {
        array[i] = genome.gene(i);
    }
}

std::function<void(GAPopulation&)> ParallelEvaluator::populationEvaluator() {
    return [this](GAPopulation& population) { this->evaluatePopulation(population); };
}

void ParallelEvaluator::evaluatePopulation(GAPopulation &population) {
    std::vector<int> needEvaluation; // cache of already evaluated invididuals

    for(int i = 0, memoryIndex = 0; i < population.size(); i++) {
        auto& boolGenome = dynamic_cast<GA1DBinaryStringGenome&>(population.individual(i));
        auto actualScore = EvaluatedHack::isEvaluated(&boolGenome) ? boolGenome.score() : -1;

        if (actualScore == -1) {
            short genomeArray[boolGenome.size()];
            genomeToShortArray(boolGenome, genomeArray);
            //TODO: check if is already evaluated
            this->memory.putGenome(memoryIndex, genomeArray, boolGenome.size(), actualScore, -1);
            memoryIndex++;
            needEvaluation.push_back(i);
        }

        // debug print
        /*cout << "Bool " << boolGenome << endl;
        std::vector<short> w;
        w.assign(genomeArray, genomeArray + boolGenome.size());
        cout << "Short " << w << endl;*/


        //auto* evalData = dynamic_cast<bngenome::CustomEvalData*>(boolGenome.evalData());
        /*if(evalData == NULL) {
            boolGenome.evalData(bngenome::CustomEvalData());
            evalData = dynamic_cast<bngenome::CustomEvalData*>(boolGenome.evalData());
        }*/
    }

    prepareTaskSlave(needEvaluation.size());
    workers.resumeSlaves();
    workers.waitSlaves();

    for(int i = 0; i < needEvaluation.size(); i++) {
        int individualIndex = needEvaluation[i];

        auto& boolGenome = population.individual(individualIndex);

        auto& slaveData = memory.getGenome(i);
        boolGenome.score((float) slaveData.fitness);

        //cout << "Slave data " << (float) slaveData.fitness << endl;
        //((bngenome::CustomEvalData*) boolGenome.evalData())->robotCount = robotCount;
    }
}


void ParallelEvaluator::prepareTaskSlave(int genomeCount) {
    auto populationSlice = genomeCount / parallelism;
    auto populationOvers = genomeCount % parallelism;
    for(int i = 0; i < parallelism; i++) {
        auto populationIndexStart = (i * populationSlice);
        // assign to last one the leftovers population
        auto size = (populationOvers > 0 && i == parallelism - 1) ?
                    populationSlice + populationOvers :
                    populationSlice;
        memory.putWorkSlice(i, populationIndexStart, size);
    }
}

[[noreturn]] void ParallelEvaluator::slaveBehaviour(int slaveId,
                                                    const config::GeneticConfiguration &configuration,
                                                    ticpp::Document& experimentXML) {

    std::ofstream slaveLog(std::string("logs/ARGoS_LOG_" + ToString(::getpid())).c_str(), std::ios::out);
    LOG.DisableColoredOutput();
    LOG.GetStream().rdbuf(slaveLog.rdbuf());
    std::ofstream slaveLogErr(std::string("logs/ARGoS_LOGERR_" + ToString(::getpid())).c_str(), std::ios::out);
    LOGERR.DisableColoredOutput();
    LOGERR.GetStream().rdbuf(slaveLogErr.rdbuf());

    // setup and load experiment
    LOG << "Started " << slaveId << endl;
    argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    simulator.Load(experimentXML);
    auto experiment = bngenome::create_context(configuration, simulator);

    experiment.loop.GenerateRandomSpawnLocation(configuration.n_trials);

    auto& memory = workers.sharedMemory();
    while (true) {
        ::raise(SIGSTOP);
        int from, size;
        memory.getWorkSlice(slaveId, &from, &size);
        LOG << " Slave " << slaveId << " pid: " << ::getpid() << " slice " << from << " to " << (from + size) << " size: " << size << endl;

        for(int i = from; i < from + size; i++) {
            auto& shared = memory.getGenome(i);

            //cout << i << " " << shared.genome << endl;
           /*std::vector<short> w;
           w.assign(shared.genome, shared.genome + shared.genomeSize);
           cout << "Short2 " << w << endl;
           cout << shared.fitness << endl;*/

           GA1DBinaryStringGenome genome(shared.genomeSize);
           genome.operator=(shared.genome);
           //genome.operator=()
           //genome = genome.operator=(shared.genome);
           //genome.evalData(bngenome::CustomEvalData());
           //genome.userData(&experiment);

           bool needEvaluation = shared.fitness == -1;
           if(needEvaluation) {
               auto performance = bngenome::evaluator(experiment)(genome);
               //auto robotCount = ((bngenome::CustomEvalData*) genome.evalData())->robotCount;
               memory.updateGenomeEvaluation(i, performance, 0);
           }
       }

       LOG.Flush();
       LOGERR.Flush();
   }
}
