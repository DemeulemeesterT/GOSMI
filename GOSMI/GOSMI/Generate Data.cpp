#include "Data.h"

void main() {
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

	// Generate your data
	Data* D = new Data(nagents, nobjects, parameters, print, seed);

	delete parameters;
	delete D;


}