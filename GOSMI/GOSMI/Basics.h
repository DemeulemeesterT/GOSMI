#pragma once
#include <vector>
using namespace std;
// Just some basic functions that can be used in all kinds of general circumstances

double average(double* array, int length);
double average(int* array, int length);
double average(vector<short int> array, int length);

int sum(bool* array, int length);
int sum(int* array, int length);
double sum(double* array, int length);

int max_int(int* array, int length);
int max_int(vector<short int> array, int length);
int max_int(vector<int> array, int length);

int min_int(vector<short int> array, int UB, int length);



double corr(int* X, int* Y, int length);
double corr(double* X, double* Y, int length);
double corr(int* X, double* Y, int length);
double corr(double* X, int* Y, int length);

double stand_dev(int* array, int length);
double stand_dev(double* array, int length);
double stand_dev(vector<short int> array, int length);