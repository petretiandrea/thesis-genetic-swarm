//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_PARALLELEVALUTATOR_H
#define SWARM_GEN_PARALLELEVALUTATOR_H

#include <boost/mpi.hpp>
#include <vector>
#include <core/configuration/ExperimentConfiguration.h>
#include <core/configuration/ArgosExperiment.h>
#include <ga/ga.h>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;
namespace mpi = boost::mpi;

enum PROTOCOL {
    GENOMES,
    RESULT
};

struct SharedBinaryGenome {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & genomeIndex;
        ar & genome;
    }

public:
    int genomeIndex;
    vector<bool> genome;


    SharedBinaryGenome() : genomeIndex(0), genome() {};
    explicit SharedBinaryGenome(int genomeIndex, vector<bool> genome) : genomeIndex(genomeIndex), genome(genome) {};
};

struct ResultBinaryGenome {
private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version) {
        ar & genomeIndex;
        ar & fitness;
    }

public:
    int genomeIndex;
    double fitness;

    ResultBinaryGenome() : genomeIndex(-1), fitness(-1) {};
    explicit ResultBinaryGenome(int genomeIndex, double fitness) : genomeIndex(genomeIndex), fitness(fitness) {};
};

class MPIEvaluator {

public:
    MPIEvaluator(mpi::communicator& world);

    std::function<void(GAPopulation&)> populationEvaluator();
    std::function<double(GA1DBinaryStringGenome&)> genomeEvaluator();
    [[noreturn]] void slaveBehaviour(const config::GeneticConfiguration &configuration,
                                     ticpp::Document& experimentXML);

private:
    void evaluatePopulation(GAPopulation& population);


private:
    mpi::communicator world;
    int parallelism;
};

#endif //SWARM_GEN_PARALLELEVALUTATOR_H
