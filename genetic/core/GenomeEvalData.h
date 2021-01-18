//
// Created by andrea on 05/01/21.
//

#ifndef SWARM_GEN_GENOMEEVALDATA_H
#define SWARM_GEN_GENOMEEVALDATA_H

#include <ga/GAGenome.h>
#include <ga/GA1DBinStrGenome.h>

namespace bngenome {
    class GenomeEvalData : public GAEvalData {

    public:
        double robotCount;

    public:
        GenomeEvalData();
        explicit GenomeEvalData(double robotCount);
        ~GenomeEvalData() override = default;

        GAEvalData *clone() const override;

        void copy(const GAEvalData &data) override;

    };
}

#endif //SWARM_GEN_GENOMEEVALDATA_H
