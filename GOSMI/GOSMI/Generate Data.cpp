#include "Data.h"

int main() {
	// Create a data instance with the following parameters

	// Number of agents:
	int nagents = 25;

	// Number of objects:
	int nobjects = 5;

	// Do you want to print the instance?
	bool print = true;

	// Would you like to select a seed for the random number generator?
		// Not specifying a seed, will use a random seed
	unsigned seed = 0;

	DataGen_Param* parameters = new DataGen_Param();
	
	// If you would like to change some parameters, do it here:
		// e.g., parameters->mean_pref = 4;
			// See 'DataGen_Param.h' for a description of the parameters 

	// Generate your data
	Data* D = new Data(nagents, nobjects, parameters, print, seed);

	// Enter the name of the file you want to export the data to
		// Comment out the next line if you do not wish to export the data
	D->export_data((string)"test");

	delete parameters;
	delete D;


}