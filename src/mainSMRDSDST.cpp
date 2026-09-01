#include <iostream>
#include <vector>
#include "SMRDSDST.h"

int main() {
    std::string filename = "data/instances/Instances/in02_001.dat"; 
    SMRDSDST problem(filename);

    solSMRDSDST s;
    s.sequence = {3, 4, 11, 20, 7, 10, 13, 1, 21, 9, 8, 19, 17, 16, 14, 5, 25, 6, 22, 15, 24, 12, 23, 18, 2};  
    double value = problem.evaluate(s);

    std::cout << "Sequence: ";
    for (int j : s.sequence) std::cout << j << " ";
    std::cout << "\n";

    std::cout << "Evaluate = " << value << "\n";

    solSMRDSDST initial = problem.construction();
    std::cout << "Initial construction: ";
    for (int j : initial.sequence) std::cout << j << " ";
    std::cout << "\n";
    std::cout << "Initial evaluate = " << initial.evalSol << "\n";

    return 0;
}