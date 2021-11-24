The data in the included files has the following meaning.

For files *_agents.txt:
	- The first column contains the agent ID (starting from index 0)
	- The second column contains the object ID (starting from index 0)
	- The third column indicates in which position the object in this row occurs in the preference list of the agent in this row (starting from index 1)

For files *_objects.txt:
	- The first column contains the object ID (starting from index 0)
	- The second column contains the capacity of that object

For files *_P.txt:
	- MEAN = the average number of assigned agents by X^RSD
	- MIN = the worst-case number of assigned agents by the RSD algorithm (over all 10,000 sampled matchings)
	- MAX = the maximum number of assigned agents by the RSD algorithm (over all 10,000 sampled matchings)
	- The matrix displays X^RSD, in which the columns refer to the agents and the rows to the objects