#include "DataGen_Param.h"

DataGen_Param::DataGen_Param() {
	// Create an instane of the Data Generation Parameters with the values from Ghent:
	this->capacity_ratio = 1.2;
	this->corr_cap_pop = 0.21;
	this->mean_pref = 2.42;
	this->sigma_pref = 1.05;
	this->CV_cap = 0.8;
	this->CV_pop = 0.6;
	this->delta_1 = 0.14;
	this->delta_2 = 0.009;
	this->pop_percentage = 0.10;
}

DataGen_Param::DataGen_Param(double capacity_ratio, double corr_cap_pop, double mean_pref, double sigma_pref, double CV_cap, double CV_pref, double delta_1, double delta_2, double pop_percentage) {
	this->capacity_ratio = capacity_ratio;
	this->corr_cap_pop = corr_cap_pop;
	this->mean_pref = mean_pref;
	this->sigma_pref = sigma_pref;
	this->CV_cap = CV_cap;
	this->CV_pop = CV_pref;
	this->delta_1 = delta_1;
	this->delta_2 = delta_2;
	this->pop_percentage = pop_percentage;
}

DataGen_Param::~DataGen_Param() {

}