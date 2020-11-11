#pragma once
class DataGen_Param
{
public:
	// See thesis pag. 42-43 for a detailed description of these parameters
	double capacity_ratio;
	double corr_cap_pop;
	double mean_pref;
	double sigma_pref;
	double CV_cap;
	double CV_pop;
	double delta_1;
	double delta_2;
	double pop_percentage;
	DataGen_Param();
	DataGen_Param(double capacity_ratio, double corr_cap_pop, double mean_pref, double sigma_pref, double CV_cap, double CV_pref, double delta_1, double delta_2, double pop_percentage);
	~DataGen_Param();
};

