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
	int nagents;
	int nobjects;
	int* agents;
	int* objects;
	int** pref;
	int* pref_length;
	int* capacities;
	double* popularity;
	bool* popular;
	DataGen_Param* parameters;
	Data(int nagents, int nobjects, DataGen_Param* parameters, bool print, unsigned seed = 123456789);
	// This constructor creates a data set

	~Data();

	void print_data(void);
	DataGen_Param* obtain_statistics(bool print);
};

