#include "SMRDSDST.h"
#include <algorithm>
#include <random>

SMRDSDST::SMRDSDST(std::string filename){
	
	fn = filename;
	std::string line; 
    std::ifstream ifs;
	ifs.open(filename);		

	if ( ifs.is_open()){
		
		getline(ifs,line);
        std::stringstream header(line);
        int numberMachinesDisregard;
        header >> numberJobs >> numberMachinesDisregard;
            
		getline(ifs,line);        
        getline(ifs, line); // ignora as duas linhas com 0

        release.resize(numberJobs);
        processing.resize(numberJobs);
        setup.assign(numberJobs + 1, std::vector<int>(numberJobs, 0));

        for (int i = 0; i < numberJobs; ++i) {
            getline(ifs, line);
            std::stringstream ssJob(line);
            int idDisregard, dueDateDisregard;

            ssJob >> idDisregard >> release[i] >> processing[i] >> dueDateDisregard;
        }

        for (int i = 0; i <= numberJobs; ++i) {
            for (int j = 0; j < numberJobs; ++j) {
                ifs >> setup[i][j];
            }
        }
        ifs.close();
	} else{
		std::cout << "Could not open file! \n";
	}	
}

solSMRDSDST SMRDSDST::construction() {
    solSMRDSDST solution;
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());

    for (int job = 1; job <= numberJobs; ++job) {
        solution.sequence.push_back(job);
    }

    std::shuffle(solution.sequence.begin(), solution.sequence.end(), generator);

    solution.evalSol = evaluate(solution);
    solution.Nup = false;
    solution.Ndown = false;

    return solution;
}


solSMRDSDST SMRDSDST::neighbor(solSMRDSDST sol) {
    return neighborSwap(sol);
    // return neighborLBlockInsertion(sol, 2);
    // return neighborInversion(sol);
}

// troca as pos de dois jobs quaisquer dentro da sequência
solSMRDSDST SMRDSDST::neighborSwap(solSMRDSDST sol) {
    solSMRDSDST neighbor = sol;
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(0, numberJobs - 1);

    int first;
    int second;
    do {
        first = distribution(generator);
        second = distribution(generator);
    } while (first == second);

    std::swap(neighbor.sequence[first], neighbor.sequence[second]);
    return neighbor;
}

// ref anand
// a block of 𝑙 adjacent jobs is removed from its current position and reinserted in another position of the sequence
solSMRDSDST SMRDSDST::neighborLBlockInsertion(solSMRDSDST sol, int blockSize) {
    solSMRDSDST neighbor = sol;

    if (blockSize <= 0 || blockSize > numberJobs) {
        return neighbor;
    }

    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> startDistribution(0, numberJobs - blockSize);
    int blockStart = startDistribution(generator);

    std::vector<int> block(
        neighbor.sequence.begin() + blockStart,
        neighbor.sequence.begin() + blockStart + blockSize);
    neighbor.sequence.erase(
        neighbor.sequence.begin() + blockStart,
        neighbor.sequence.begin() + blockStart + blockSize);

    std::uniform_int_distribution<int> insertionDistribution(0, numberJobs - blockSize);
    int insertionPosition = insertionDistribution(generator);
    neighbor.sequence.insert(
        neighbor.sequence.begin() + insertionPosition,
        block.begin(),
        block.end());

    return neighbor;
}

solSMRDSDST SMRDSDST::neighborInversion(solSMRDSDST sol) {
    solSMRDSDST neighbor = sol;
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distribution(0, numberJobs - 1);

    int first = distribution(generator);
    int last = distribution(generator);
    if (first > last) {
        std::swap(first, last);
    }

    std::reverse(
        neighbor.sequence.begin() + first,
        neighbor.sequence.begin() + last + 1);
    return neighbor;
}

// [3, 4, 11, 20, 7, 10, 13, 1, 21, 9, 8, 19, 17, 16, 14, 5, 25, 6, 22, 15, 24, 12, 23, 18, 2]

SMRDSDST::evaluate(solSMRDSDST sol) {
    int currentTime = 0;
    int lastJob = 0;

    for (int jobIndex : sol.sequence) {
        int setupTime = setup[lastJob][jobIndex - 1];
        int startTime = std::max(currentTime + setupTime, release[jobIndex - 1]);
        currentTime = startTime + processing[jobIndex - 1];

        lastJob = jobIndex;
    }

    return static_cast<double>(currentTime);
}