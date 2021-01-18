//
// Created by andrea on 06/01/21.
//

#include "ParallelEvalutator.h"

#include <argos3/core/simulator/simulator.h>
#include <loop_function/evolution/evolution_loop.h>
#include <csignal>
#include <genetic/core/BNGenome.h>

/*ParallelEvaluator::ParallelEvaluator(int parallelism) :
    workers(MasterSlave<BNGenomeSharedMemory>(BNGenomeSharedMemory(), parallelism)) {

    workers.start([](int slaveId) {
        slaveBehaviour(slaveId, *nullptr)
    })
}

void ParallelEvaluator::slaveBehaviour(int slaveId, const config::GeneticConfiguration &configuration) {

    std::ofstream slaveLog(std::string("logs/ARGoS_LOG_" + ToString(::getpid())).c_str(), std::ios::out);
    LOG.DisableColoredOutput();
    LOG.GetStream().rdbuf(slaveLog.rdbuf());
    std::ofstream slaveLogErr(std::string("logs/ARGoS_LOGERR_" + ToString(::getpid())).c_str(), std::ios::out);
    LOGERR.DisableColoredOutput();
    LOGERR.GetStream().rdbuf(slaveLogErr.rdbuf());

    argos::CSimulator& simulator = argos::CSimulator::GetInstance();
    try {
        // load experiment
    } catch (CARGoSException& ex) {
        cerr << ex.what() << endl;
        ::raise(SIGTERM);
    }

    auto experiment = bngenome::create_context(configuration, simulator, bngenome::genome_boolfunctions_initializer(&rnd, 0.21f));
    experiment.loop.GenerateRandomSpawnLocation(configuration.n_trials);

    while (true) {
        ::raise(SIGSTOP);
        auto* slices = sharedMemory->GetWork(slaveId);
        LOG << " Slave " << slaveId << " pid: " << ::getpid() << " slice " << slices[0] << " to " << (slices[0] + slices[1]) << " size: " << slices[1] << endl;

        for(int i = slices[0]; i < slices[0] + slices[1]; i++) {
            auto& genomeData = sharedMemory->GetGenome(i);
            GA1DBinaryStringGenome genome(genomeData.genomeSize);
            genome = genome.operator=(genomeData.genome);
            genome.evalData(bngenome::CustomEvalData());
            genome.userData(&experiment);

            bool needEvaluation = genomeData.fitness == -1;
            if(needEvaluation) {
                auto performance = bngenome::genome_evaluator(genome);
                auto robotCount = ((bngenome::CustomEvalData*) genome.evalData())->robotCount;
                sharedMemory->UpdateEvaluation(i, performance, robotCount);
            }
        }
        delete[] slices;
        LOG.Flush();
        LOGERR.Flush();
    }
}*/
