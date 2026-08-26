#include "SMRDSDST.h"

SMRDSDST::SMRDSDST(std::string filename){
	
	fn = filename;
	std::string line; 
    std::ifstream ifs;
	ifs.open(filename);		

	if ( ifs.is_open()){
		
		getline(ifs,line);
        std::stringstream header(line);
        header >> numberJobs >> numberMachines;
		
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