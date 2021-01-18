//
// Created by andrea on 06/01/21.
//

#ifndef SWARM_GEN_GENETICEVALUATOR_H
#define SWARM_GEN_GENETICEVALUATOR_H

class GeneticEvaluator {
public:
    virtual float evaluate_genome(GAGenome& genome);
    virtual void evaluate_population(GAPopulation& population);
};

#endif //SWARM_GEN_GENETICEVALUATOR_H
