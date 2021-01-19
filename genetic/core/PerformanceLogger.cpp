//
// Created by Andrea Petreti on 19/01/2021.
//

#include "PerformanceLogger.h"

#include <utility/Utility.h> // enable flush of vector and matrix structures

PerformanceLogger::PerformanceLogger(const string& folder, const string& basename) : bestFilePath(folder + basename + "_best_all.csv")
{
    generationsFile.open(folder + basename + ".csv");

    generationsFile << "generation;fitness;score;genome" << endl << flush;

}

float actual_best_score = -1;
void PerformanceLogger::saveStatistics(const GAPopulation &population) {

    for(int i = 0; i < population.size(); i++) {
        auto& genome = population.individual(i);

        saveIndividual(generationsFile, population.geneticAlgorithm()->generation(), genome);

        if(genome.score() >= actual_best_score) {
            actual_best_score = genome.score();
            bestFile.open(bestFilePath);
            bestFile << "generation;fitness;score;genome" << endl << flush;
            saveIndividual(bestFile, population.geneticAlgorithm()->generation(), genome);
            bestFile.close();
        }
    }
}

void PerformanceLogger::saveIndividual(ofstream& stream, int generation, GAGenome &genome) {
    stream << generation << ";"
            << genome.score() << ";"
            << 0 << ";"
            << genome << ";"
            << endl << flush;
}

string PerformanceLogger::statisticsBasenameFromConfiguration(const config::ExperimentConfiguration &configuration) {
    std::ostringstream metainfo;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%H-%M_%d-%m",timeinfo);
    metainfo
            << "nodes" << configuration.controller_config.nodes
            << "_k" << configuration.controller_config.input_for_node
            << "_bias" << configuration.controller_config.bias
            << "_prox" << configuration.controller_config.proximity_threshold
            << "_p" << configuration.genetic_config.population_size
            << "_g" << configuration.genetic_config.generations
            << "_pM" << configuration.genetic_config.mutation_probability
            << "_pC" << configuration.genetic_config.crossover_probability
            << "_el" << configuration.genetic_config.elitism
            << "_trials" << configuration.genetic_config.n_trials
            << "_" << string(buffer);
    return metainfo.str();
}
