//
// Created by andrea on 05/01/21.
//

#include "GenomeEvalData.h"

using namespace bngenome;

GenomeEvalData::GenomeEvalData() = default;
GenomeEvalData::GenomeEvalData(double robotCount) : robotCount(robotCount) {}

GAEvalData *GenomeEvalData::clone() const {
    return new GenomeEvalData(robotCount);
}

void GenomeEvalData::copy(const GAEvalData &data) {
    const auto& customData = dynamic_cast<const GenomeEvalData*>(&data);
    this->robotCount = customData->robotCount;
}

