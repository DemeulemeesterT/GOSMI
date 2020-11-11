#include "Matching.h"
#include <iostream>

Matching::Matching(int nagents, int nobjects) {
	this->nagents = nagents;
	this->nobjects = nobjects;
	this->m_agent = new int[nagents];
	this->m_agent_pref = new int[nagents];
	this->m_object_count = new int[nobjects];
	this->m_object = new int* [nobjects];
	for (int i = 0; i < nobjects; i++) {
		this->m_object[i] = new int[nagents];

	}
	this->order_STB = new int[nagents];
	this->order_MTB = new int* [nobjects];
	for (int i = 0; i < nobjects; i++) {
		this->order_MTB[i] = new int[nagents];
	}

	// Initialize the variables to values that cannot be true:
	for (int i = 0; i < nagents; i++) {
		this->m_agent[i] = -1;
		this->m_agent_pref[i] = -1;
		this->order_STB[i] = -1;
		for (int j = 0; j < nobjects; j++) {
			this->order_MTB[j][i] = -1;
		}
	}

	for (int i = 0; i < nobjects; i++) {
		this->m_object_count[i] = 0;
		for (int j = 0; j < nagents; j++) {
			this->m_object[i][j] = false;
		}
	}
}

void Matching::print_matching() {
	printf("\n\n MATCHINGS:\n");
	printf("\t       agent  |  object  |  PREFERENCE  \n");
	printf("\t________________________________________________\n");
	for (int i = 0; i < this->nagents; i++) {
		// Print agent number:
		// Somehow, 0 is not printed...
		if (i != 0) {
			printf("\t        %4.i     |", i);
		}
		else printf("\t          0     |");

		// Print assigned object:
		if (this->m_agent[i] == -1) {
			printf("    --    |");
		}
		else if (this->m_agent[i] != 0) {
			printf("    %2.i    |", this->m_agent[i]);
		}
		else {
			printf("     0    |");
		}

		// Print preference:
		if (this->m_agent[i] == -1) {
			printf("     --\n ");
		}
		else {
			printf("      %i\n ", this->m_agent_pref[i] + 1);
		}
		//printf("\t        %3.i     |    %2.i    |      %i\n ", i, this->m_agent[i], this->m_agent_pref[i] + 1);
	}
}


Matching::~Matching() {
	delete[] m_agent;
	delete[] m_agent_pref;
	for (int i = 0; i < nobjects; i++) {
		delete[] m_object[i];
		delete[] order_MTB[i];
	}
	delete[] m_object;
	delete[] order_MTB;
	delete[] order_STB;
	delete[] m_object_count;
}