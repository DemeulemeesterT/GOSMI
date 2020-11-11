#include "Data.h"
#include <time.h>
#include "Basics.h"

double* cholesky(double* covar, int n);

int pick_object(bool pop, int p, int i, int nobjects, int popular_count, bool* popular, double* selection_pop_cum, double* selection_n_pop_cum, int* preferences, int pref_length, std::mt19937 generator);

// ***************************************************
// ****** EVERYTHING RELATED TO GENERATING DATA ******
// ***************************************************

Data::Data(int nagents, int nobjects, DataGen_Param* parameters, bool print, unsigned seed) {
	int i;

	// Create the arrays
	this->nagents = nagents;
	this->nobjects = nobjects;

	this->agents = new int[nagents];
	this->pref = new int* [nagents];
	this->pref_length = new int[nagents];
	this->popular = new bool[nagents];

	this->objects = new int[nobjects];
	this->capacities = new int[nobjects];
	this->popularity = new double[nobjects];
	double* capacity_aid = new double[nobjects];
	double* popularity_aid = new double[nobjects];

	this->parameters = parameters;


	// **********************************
	// ********* INITIALIZATION *********
	// **********************************

	// Define the pseudo-random number generator
	unsigned seed_init = (unsigned)time(NULL);
	std::mt19937 generator(seed_init);
	if (seed != 123456789) {
		generator.seed(seed);
	}

	// Fill in the arrays 'agents' and 'objects' with indices
	for (i = 0; i < nagents; i++) {
		this->agents[i] = i;
	}

	for (i = 0; i < nobjects; i++) {
		this->objects[i] = i;
	}

	double capacity_total;
	capacity_total = round(parameters->capacity_ratio * this->nagents);

	// Obtain correlated capacities and popularity ratios
	// First, generate normally distributed values for capacity and popularity
	std::normal_distribution<double> randn(0.0, 1.0);
	for (i = 0; i < nobjects; i++) popularity_aid[i] = randn(generator);
	for (i = 0; i < nobjects; i++) capacity_aid[i] = randn(generator);

	// Define the desired covariance matrix:
	double covar[] = { 1, parameters->corr_cap_pop,
					parameters->corr_cap_pop, 1 };

	// Use Cholesky factorization
	double* G = cholesky(covar, 2);

	// Change the values in 'popularity_aid'
	for (i = 0; i < nobjects; i++) {
		popularity_aid[i] = G[2] * capacity_aid[i] + G[3] * popularity_aid[i]; // Multiply with the TRANSPOSED G matrix
	}



	// ******************************
	// ********* CAPACITIES *********
	// ******************************

	// Now make sure that the sum of the capacities is equal to 'capacity_total'
	double mean_capacity_aid = average(capacity_aid, nobjects);
	for (i = 0; i < nobjects; i++) {
		this->capacities[i] = (int)round((capacity_total / nobjects) * (1 + parameters->CV_cap * (capacity_aid[i] - mean_capacity_aid)));
	}

	double* capacities_double = new double[nobjects];


	// Replace the capacities if they are smaller than 1
	for (i = 0; i < nobjects; i++) {
		// Use 'capacity_aid' to avoid rounding twice 
		capacities_double[i] = this->capacities[i];
		if (capacities_double[i] < 1) {
			while (capacities_double[i] < 1) {
				// In this case, redraw a capacity from the same standardized normal distribution
				// AND, draw a new correlated popularity ratio
				capacities_double[i] = randn(generator);
				popularity_aid[i] = randn(generator);

				// Once again, make popularity correlated in expected terms with capacity
				popularity_aid[i] = G[2] * capacities_double[i] + G[3] * popularity_aid[i];

				capacities_double[i] = (capacity_total / nobjects) * (1 + parameters->CV_cap * capacities_double[i]);
				// Not - 'mean_capacity_aid', because we the expected value should be 0
			}
		}
	}

	// Rescale capacities to still ensure that the sum of all capacities equals 'capacity_total'
	for (i = 0; i < nobjects; i++) {
		this->capacities[i] = (int)round(((int)capacity_total / sum(capacities_double, nobjects)) * capacities_double[i]);
	}
	delete[] capacities_double;

	// *******************************************
	// ********* LENGTH PREFERENCE LISTS *********
	// *******************************************
	std::normal_distribution<double> normal_pref_length(parameters->mean_pref, parameters->sigma_pref);
	for (i = 0; i < nagents; i++) {
		this->pref_length[i] = (int)normal_pref_length(generator);

		// An empty preference list is not possible
		if (this->pref_length[i] < 1) {
			this->pref_length[i] = 1;
		}

		// It is not possible to submit more preferences than there are objects
		if (this->pref_length[i] > nobjects) {
			this->pref_length[i] = nobjects;
		}
	}

	// Now that we know the preference lengths, we can decide how many preferences each agent has:
	for (i = 0; i < nagents; i++) {
		this->pref[i] = new int[this->pref_length[i]];
	}

	// IMPORTANT: initialize this preference list, otherwise the data generation might not work!
	// The reason is that sometimes the value of 'pref[i][p]' might be the value of a object in the list.
	// If this is the case, the program will loop sometimes because it cannot find a object that is not yet in the list (if all (non-)popular objects are already included)
	for (i = 0; i < nagents; i++) {
		for (int p = 0; p < this->pref_length[i]; p++) {
			this->pref[i][p] = -1;
		}
	}

	// *************************************
	// ********* POPULARITY RATIOS *********
	// *************************************
	// The mean popularity ratio is the total number of submitted preferences divided by the total capacity.
	double mean_pop_ratio = (double)sum(this->pref_length, nagents) / (double)sum(this->capacities, nobjects);

	// Transform the earlier obtained popularity ratios
	double mean_popularity_aid = average(popularity_aid, nobjects);
	for (int i = 0; i < nobjects; i++) {
		popularity_aid[i] = mean_pop_ratio * (1 + parameters->CV_pop * (popularity_aid[i] - mean_popularity_aid));
	}

	// It is not possible that objects have a negative popularity ratio...
	// Therefore, we simply replace these by another draw from the same normal distribution
	// and we make it correlated to the capacity.
	for (i = 0; i < nobjects; i++) {
		while (popularity_aid[i] < 0.2) {
			// Draw new popularity ratio that is correlated to the capacity
			popularity_aid[i] = randn(generator);
			popularity_aid[i] = G[2] * capacity_aid[i] + G[3] * popularity_aid[i];
			popularity_aid[i] = mean_pop_ratio * (1 + parameters->CV_pop * popularity_aid[i]);
		}
	}



	// Shift all popularity ratios to ensure that the sum of all submitted preferences is indeed
	// equal to the sum over all objects of the expected number of occurrences of each object.
	double* requests = new double[nobjects];
	for (i = 0; i < nobjects; i++) {
		requests[i] = this->capacities[i] * popularity_aid[i];
	}

	for (i = 0; i < nobjects; i++) {
		popularity_aid[i] = popularity_aid[i] * (sum(this->pref_length, nagents) / sum(requests, nobjects));
	}

	delete[] requests;

	for (i = 0; i < nobjects; i++) {
		this->popularity[i] = popularity_aid[i];
	}


	// **************************************************
	// ********* ASSIGN PREFERENCES TO agents *********
	// **************************************************
	// We want to parametrize the correlation between the number of submitted preferences
	// and the popularity of the objects in the preference list

	// Create a deep copy of 'popularity_aid' that will be sorted:
	double* popularity_aid_sorted = new double[nobjects];
	for (i = 0; i < nobjects; i++) {
		popularity_aid_sorted[i] = popularity_aid[i];
	}

	// Define a object to be popular if it is in the 'popularity_percentage' % most popular objects
	std::sort(popularity_aid_sorted, popularity_aid_sorted + nobjects, std::greater<double>());
	int popular_index = (int)ceil(parameters->pop_percentage * nobjects) - 1;
	double popularity_treshold = popularity_aid_sorted[popular_index] - 0.00001;

	// Fill in the array 'popular'
	for (i = 0; i < nobjects; i++) {
		if (popularity_aid[i] > popularity_treshold) {
			this->popular[i] = true;
		}
		else this->popular[i] = false;
	}

	delete[] popularity_aid_sorted;

	// What is the normal probability with which a popular object is mentioned in the preference lists?
	double popular_choice_prob = 0;
	for (i = 0; i < nobjects; i++) {
		if (this->popular[i]) {
			popular_choice_prob += popularity_aid[i] * (double)this->capacities[i];
		}
	}
	popular_choice_prob = popular_choice_prob / sum(this->pref_length, nagents);

	// Assume that the probability of submitting a popular object as the first choice is 
	// a linear function of the length of the preference list.
	// Based on observations from the data of Ghent and Antwerp, we can make an estimation of the difference in probability 
	// for someone who submits a list of length one compared to the mean 
	// (and equivalently, based on the assumed linearity, the difference between the mean and the person with the longest preference list)
	// The difference in percentage for a list that is one object longer is then:
	double popular_diff_unit = parameters->delta_1 / ((max_int(this->pref_length, nagents) + 1) / 2);

	// Create arrays with the selection probabilities for non-popular and for popular objects
	double* selection_n_pop = new double[nobjects];
	double* selection_n_pop_cum = new double[nobjects];
	double* selection_pop = new double[nobjects];
	double* selection_pop_cum = new double[nobjects];

	// Count the number of expected cases in which a (non-)popular object will be selected
	double all_n_pop = 0;
	double all_pop = 0;
	for (i = 0; i < nobjects; i++) {
		if (popular[i]) {
			all_pop += popularity_aid[i] * (double)this->capacities[i];
		}
		else {
			all_n_pop += popularity_aid[i] * (double)this->capacities[i];
		}
	}

	// Now calculate the probability that each object will be selected, given that a (non-)popular object should be selected
	for (i = 0; i < nobjects; i++) {
		if (this->popular[i]) {
			selection_pop[i] = popularity_aid[i] * (double)this->capacities[i] / all_pop;
			selection_n_pop[i] = 0;
		}
		else {
			selection_n_pop[i] = popularity_aid[i] * (double)this->capacities[i] / all_n_pop;
			selection_pop[i] = 0;
		}
	}

	// Define the cumulative selection arrays
	if (nobjects > 1) {
		selection_pop_cum[0] = selection_pop[0];
		selection_n_pop_cum[0] = selection_n_pop[0];
		for (i = 1; i < nobjects; i++) {
			selection_pop_cum[i] = selection_pop_cum[i - 1] + selection_pop[i];
			selection_n_pop_cum[i] = selection_n_pop_cum[i - 1] + selection_n_pop[i];
		}
	}
	else {
		selection_n_pop_cum[0] = selection_n_pop[0];
		selection_pop_cum[0] = selection_pop[0];
	}

	// Make sure that the last elements of the cumulative arrays are equal to one:
	int check_object = nobjects - 1;
	while (selection_n_pop_cum[check_object] == 0) {
		selection_n_pop_cum[check_object] = 1;
		check_object--;
	}

	check_object = nobjects - 1;
	while (selection_pop_cum[check_object] == 0) {
		selection_pop_cum[check_object] = 1;
		check_object--;
	}



	// Now determine which objects will fill the preference lists of the agents
	// 'popular_choice[i]' = true if agent i chooses a popular object
	bool* popular_choice = new bool[nagents];

	// 'popular_first_choice[i]' is true is the first choice of agent i is popular
	bool* popular_first_choice = new bool[nagents];

	// 'popular_rand[i]' is a random number that is allocated to agent i
	double* popular_rand = new double[nagents];

	// 'check_prob[i]' is the cutoff probability;
		// If the randomly drawn number 'popular_rand[i]' <= 'check_prob[i]', agent i chooses a popular object
	double* check_prob = new double[nagents];

	// Define a uniform distribution between 0 and 1:
	std::uniform_real_distribution <double> rand_prob(0.0, 1.0);

	int popular_count = sum(popular, nobjects);

	// First, determine the first preferences of all agents and store whether or not they are popular
	int p = 0;

	for (i = 0; i < nagents; i++) {
		popular_choice[i] = false;
		popular_first_choice[i] = false;
		popular_rand[i] = rand_prob(generator);
		check_prob[i] = (double)(popular_choice_prob + ((double)this->pref_length[i] - (max_int(pref_length, nagents) + 1) / 2) * popular_diff_unit);

		if (popular_rand[i] <= check_prob[i]) {
			// In this case, the agent chooses a popular object as his/her first choice
			popular_first_choice[i] = true;
			popular_choice[i] = true;
		}

		this->pref[i][p] = pick_object(popular_choice[i], p, i, nobjects, popular_count, popular, selection_pop_cum, selection_n_pop_cum, this->pref[i], this->pref_length[i], generator);
	}

	// The probability of submitting a popular object for agents who did not submit
	// a popular object as their first choice is:
	double p_0;
	p_0 = popular_choice_prob - parameters->delta_2 * sum(popular_first_choice, nagents) / nagents;
	// (Because 'p_0' had to be calculated, we could not do everything in one single for-loop)

// Next, fill in the remainder of the preference lists
	for (i = 0; i < nagents; i++) {
		for (p = 1; p < this->pref_length[i]; p++) {
			popular_choice[i] = false;
			popular_rand[i] = rand_prob(generator);
			check_prob[i] = p_0 + ((double)pref_length[i] - ((double)max_int(pref_length, nagents) + 1) / 2) * popular_diff_unit + popular_first_choice[i] * parameters->delta_2;


			if (popular_rand[i] <= check_prob[i]) {
				// In this case, the agent chooses a popular object as his/her next choice
				popular_choice[i] = true;
			}

			this->pref[i][p] = pick_object(popular_choice[i], p, i, nobjects, popular_count, popular, selection_pop_cum, selection_n_pop_cum, this->pref[i], this->pref_length[i], generator);
		}
	}

	if (print) {
		printf("\t\t*****************************************************************\n\t\t A data set with %i agents and %i objects has been generated!\n\t\t*****************************************************************\n\n", nagents, nobjects);
		this->print_data();
	}

	delete[] popular_choice;
	delete[] check_prob;
	delete[] popular_rand;
	delete[] popular_first_choice;
	delete[] capacity_aid;
	delete[] popularity_aid;
	delete[] G;
	delete[] selection_n_pop;
	delete[] selection_n_pop_cum;
	delete[] selection_pop;
	delete[] selection_pop_cum;
}

void Data::print_data() {
	printf("\n Objects:\n");
	for (int i = 0; i < nobjects; i++) {
		if (i == 0) {
			printf("\t object  0 | ");
			// Otherwise, nothing will be printed instead of "0".
		}
		else {
			printf("\t object %2.i | ", i);
		}

		if (this->popular[i]) {
			printf("Capacity =   %i | popular\n", this->capacities[i]);
		}
		else {
			printf("Capacity =   %i | non-popular\n", this->capacities[i]);
		}
	}

	printf("\n\n Agents:\n");
	for (int i = 0; i < nagents; i++) {
		if (i == 0) {
			printf("\t agent    0 |");
		}
		else {
			printf("\t agent %4.i |", i);
		}
		for (int p = 0; p < this->pref_length[i]; p++) {
			printf("\t%i", this->pref[i][p]);
		}
		printf("\n");
	}
}

DataGen_Param* Data::obtain_statistics(bool print) {
	// Create an object with the default parameters
	DataGen_Param* parameters = new DataGen_Param();

	// Create an object with the actual statistics
	DataGen_Param* stats = new DataGen_Param();
	stats->capacity_ratio = (double)sum(this->capacities, nobjects) / nagents;
	stats->corr_cap_pop = corr(this->capacities, this->popularity, nobjects);
	stats->CV_cap = stand_dev(this->capacities, nobjects) / average(this->capacities, nobjects);
	stats->CV_pop = stand_dev(this->popularity, nobjects) / average(this->popularity, nobjects);
	stats->delta_1 = NULL;
	stats->delta_2 = NULL;
	stats->mean_pref = average(this->pref_length, nagents);
	stats->pop_percentage = (double)sum(this->popular, nobjects) / nobjects;
	stats->sigma_pref = stand_dev(this->pref_length, nagents);

	if (print) {
		printf("\n\nSome statistics (default in brackets):\n");
		printf("\tCapacity ratio =  %.2f (%.2f)\n", stats->capacity_ratio, parameters->capacity_ratio);
		printf("\tCorr_cap_pop = %.2f (%.2f)\n", stats->corr_cap_pop, parameters->corr_cap_pop);
		printf("\tCV_cap = %.2f (%.2f)\n", stats->CV_cap, parameters->CV_cap);
		printf("\tCV_pop = %.2f (%.2f)\n", stats->CV_pop, parameters->CV_pop);
		printf("\tMean preference length = %.2f (%.2f)\n", stats->mean_pref, parameters->mean_pref);
		printf("\tPopularity percentage = %.2f (%.2f)\n", stats->pop_percentage, parameters->pop_percentage);
	}

	return stats;

	delete parameters;
}

double* cholesky(double* covar, int n) {
	// From site: https://rosettacode.org/wiki/Cholesky_decomposition#C
	double* G = (double*)calloc(n * n, sizeof(double));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < (i + 1); j++) {
			double s = 0;
			for (int k = 0; k < j; k++) {
				s += G[i * n + k] * G[j * n + k];
			}
			if (i != j) {
				G[i * n + j] = (1.0 / G[j * n + j] * (covar[i * n + j] - s));
			}
			else {
				G[i * n + j] = sqrt(covar[i * n + i] - s);
			}

		}
	}
	return G;
}

int pick_object(bool pop, int p, int i, int nobjects, int popular_count, bool* popular, double* selection_pop_cum, double* selection_n_pop_cum, int* preferences, int pref_length, std::mt19937 generator) {
	// 'pick_object' randomly selects a object based on the objects that are already in the preference list,
	// whether or not it should be a popular object and based on the popularity ratios of the objects

	bool new_object = false;
	int object_proposal = nobjects;
	int submitted_pop;
	int submitted_n_pop;
	std::uniform_real_distribution<double> rand_prob(0.0, 1.0);

	if (preferences[p] < nobjects&& preferences[p] >= 0) {
		printf("ERROR, THIS CANNOT BE TRUE!\n");
	}

	while (new_object == false) {
		double check_rand = rand_prob(generator);

		if (pop) { // Pick a popular object
			if (p == 0) { // First choice
				for (int j = nobjects - 1; j > -1; j--) { // Start from the last object
					if (check_rand <= selection_pop_cum[j]) {
						object_proposal = j;
					}
					else break;
				}
			}
			else {
				// Are there popular objects left that are not yet in the preference list of the agent?

				// Count the number of popular objects that are already in the preference list of agent i
				submitted_pop = 0;
				for (int pref_index = 0; pref_index < p; pref_index++) {
					if (popular[preferences[pref_index]]) {
						submitted_pop++;
					}
				}

				// Compare this number to the total number of popular objects
				if (submitted_pop == popular_count) {
					// agent i will receive a non-popular object
					pop = false;

					// Set 'object_proposal' to a object that is already in the list
					object_proposal = preferences[0];
				}
				else {
					for (int j = nobjects - 1; j > -1; j--) { // Start from the last object
						if (check_rand <= selection_pop_cum[j]) {
							object_proposal = j;
						}
						else break;
					}
				}

			}
		}
		else { // Pick a non-popular object
			if (p == 0) { // First choice
				for (int j = nobjects - 1; j > -1; j--) { // Start from the last object
					if (check_rand <= selection_n_pop_cum[j]) {
						object_proposal = j;
					}
					else break;
				}
			}
			else {
				// Are there non-popular objects left that are not yet in the preference list of the agent?

				// Count the number of non-popular objects that are already in the preference list of agent i
				submitted_n_pop = 0;
				for (int pref_index = 0; pref_index < p; pref_index++) {
					if (popular[preferences[pref_index]] == false) {
						submitted_n_pop++;
					}
				}

				// Compare this number to the total number of non-popular objects
				if (submitted_n_pop == (nobjects - popular_count)) {
					// agent i will receive a popular object
					pop = true;

					// Set 'object_proposal' to a object that is already in the list
					object_proposal = preferences[0];
				}
				else {
					for (int j = nobjects - 1; j > -1; j--) { // Start from the last object
						if (check_rand <= selection_n_pop_cum[j]) {
							object_proposal = j;
						}
						else break;
					}
				}

			}
		}

		// Does 'object_proposal already appear in the preference list of agent i?
		new_object = true;
		for (int pref_index = 0; pref_index < pref_length; pref_index++) {
			if (preferences[pref_index] == object_proposal) {
				new_object = false;
				/*
				// Are all objects of this category finished?
				// Go through all objects in the preference list and check their popularity.
				int popular_count_pref = 0;
				int n_popular_count_pref = 0;
				for (int p_ind = 0; p_ind < pref_length; p_ind++) {
					if (popular[preferences[p_ind]]) popular_count_pref++;
					else n_popular_count_pref++;
				}

				if (pop) {
					if (popular_count_pref == popular_count) {
						// If all popular objects have been included in the preference list:
						// Indicate that we have to look for a non-popular object instead of a popular object.
						pop = false;
					}
				}
				else {
					if (n_popular_count_pref == nobjects - popular_count) {
						// If all non-popular objects have been included in the preference list
						pop = true;
					}
				}

				if (popular_count_pref + n_popular_count_pref == nobjects) {
					new_object = true;
					object_proposal = -1;
				}

				*/
				break;
			}
		}


	}
	return object_proposal;
}


Data::~Data() {
	delete[] agents;
	delete[] objects;
	for (int i = 0; i < this->nagents; i++) {
		delete[] pref[i];
	}
	delete[] pref;
	delete[] pref_length;
	delete[] capacities;
	//delete parameters;
	delete[] popular;
	delete[] popularity;
}