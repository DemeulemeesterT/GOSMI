#pragma once

class Matching
{
public:
	// An array that contains for each agent the object to which (s)he is matched:
	int* m_agent;

	// An array that contains for each agent the preference of the matched object:
	int* m_agent_pref;

	// An array that contains for each object the number of agents that is assigned there:
	int* m_object_count;

	// An array that contains for each object the set of agents that are assigned there:
	int** m_object;

	// Store which order of the agents (STB or MTB) let to the current matching
	int* order_STB;
	int** order_MTB;

	int nagents;
	int nobjects;

	Matching(int nagents, int nobjects);
	~Matching();
	void print_matching();
};

