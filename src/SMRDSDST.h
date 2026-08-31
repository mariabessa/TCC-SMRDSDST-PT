#ifndef SMRDSDST_H
#define SMRDSDST_H

#include "../lib/pt-api/include/Problem.h"
//Single Machine Scheduling with Release Dates and Sequence-Dependent Setup Times
struct solSMRDSDST : public solution {
    std::vector<int> sequence;
};

class SMRDSDST : public Problem<solSMRDSDST> {
private:
    int numberJobs;
    std::vector<int> release;
    std::vector<int> processing;
    std::vector<std::vector<int>> setup;

public:
    SMRDSDST(std::string filename);

    solSMRDSDST construction() override;
    solSMRDSDST neighbor(solSMRDSDST sol) override;
    solSMRDSDST neighborSwap(solSMRDSDST sol);
    solSMRDSDST neighborLBlockInsertion(solSMRDSDST sol, int blockSize);
    solSMRDSDST neighborInversion(solSMRDSDST sol);
    double evaluate(solSMRDSDST sol) override;
};