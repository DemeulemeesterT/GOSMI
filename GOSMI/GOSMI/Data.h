#pragma once
#include "DataGen_Param.h"
#include "Matching.h"
#include <random>
#include <vector>
#include <fstream> // For reading files
using namespace std;

class Data
{
public:
	// Define the variables in this class
	int nagents; // Number of agents
	int* agents; // Array with ids of agents
	int* pref_length; // Array with how many preferences are submitted by each agent
	int** pref;	// 2D array with agents in the rows, and an array with length 'pref_length[i]' that contains the preferences of agent 'i'

	int nobjects; // Number of objects
	int* objects; // Array with ids of objects
	int* capacities; // Array with the capacities of all objects
	double* popularity; // Array with the popularity of all objects
	bool* popular; // Array stating whether or not each of the objects is popular
		// An object is popular when it is among the 'pop_percentage' most popular schools (see 'DataGen_Param.h')

	// The parameters that are used for generating the data
	DataGen_Param* parameters;

	// Constructor
	Data(int nagents, int nobjects, DataGen_Param* parameters, bool print, unsigned seed = 123456789);

	// Destructor
	~Data();

	void print_data(void);
};

