#pragma once
class DataGen_Param
{
public:
	// See README for a more detailed description of the parameters

	// The ratio of the total capacity over the number of agents
	double capacity_ratio;

	// The correlation between the capacities and the popularity of an object
	double corr_cap_pop;

	// The mean length of the preference lists
	double mean_pref;

	// The standard deviation of the length of the preference lists
	double sigma_pref;

	// The ratio of the standard deviation of the capacities over the mean capacity
	double CV_cap;

	// The ratio of the standard deviation of the popularity ratio over the mean popularity ratio
	double CV_pop;

	// The difference in the popularity of the object of someone who submitted a preference list of average length, 
	// compared to someone who submitted a preference list with only one object.
		// A preference list of average length is defined to be equal to the mean of one and the maximum length of a preference list.
	double delta_1;

	// The difference in probability of submitting a popular object if the first choice was a popular object,
	// compared to when the first choice was an unpopular object
	double delta_2;

	// The percentage of the objects with the highest popularity ratio that will be defined "popular"
	double pop_percentage;

	// Construct an object with default parameter values:
		// See 'DataGen_Param.cpp' for the exact default values
	DataGen_Param();

	// Construct an object with specified parameter values:
	DataGen_Param(double capacity_ratio, double corr_cap_pop, double mean_pref, double sigma_pref, double CV_cap, double CV_pref, double delta_1, double delta_2, double pop_percentage);
	~DataGen_Param();
};

