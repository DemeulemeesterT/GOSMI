#include "Basics.h"
#include <cmath>

double average(double* array, int length) {
	double mean = 0;
	for (int i = 0; i < length; i++) {
		mean += array[i];
	}
	mean = mean / length;
	return mean;
}

double average(int* array, int length) {
	double mean = 0;
	for (int i = 0; i < length; i++) {
		mean += array[i];
	}
	mean = mean / length;
	return mean;
}

double average(vector<short int> array, int length) {
	double mean = 0;
	for (int i = 0; i < length; i++) {
		mean += array[i];
	}
	mean = mean / length;
	return mean;
}

int sum(bool* array, int length) {
	int sum = 0;
	for (int i = 0; i < length; i++) {
		if (array[i]) {
			sum++;
		}
	}
	return sum;
}

int sum(int* array, int length) {
	int sum = 0;
	for (int i = 0; i < length; i++) {
		sum += array[i];
	}
	return sum;
}

double sum(double* array, int length) {
	double sum = 0;
	for (int i = 0; i < length; i++) {
		sum += array[i];
	}
	return sum;
}

int max_int(int* array, int length) {
	int max = 0;
	for (int i = 0; i < length; i++) {
		if (array[i] > max) {
			max = array[i];
		}
	}
	return max;
}

int max_int(vector<short int> array, int length) {
	int max = 0;
	for (int i = 0; i < length; i++) {
		if (array[i] > max) {
			max = array[i];
		}
	}
	return max;
}

int max_int(vector<int> array, int length) {
	int max = 0;
	for (int i = 0; i < length; i++) {
		if (array[i] > max) {
			max = array[i];
		}
	}
	return max;
}

int min_int(vector<short int> array, int UB, int length) {
	int min = UB;
	for (int i = 0; i < length; i++) {
		if (array[i] < min) {
			min = array[i];
		}
	}
	return min;
}

double corr(int* X, int* Y, int length) {
	// Just for reference: https://www.geeksforgeeks.org/program-find-correlation-coefficient/
	int sum_X = 0;
	int sum_Y = 0;
	int sum_XY = 0;
	int square_sumX = 0;
	int square_sumY = 0;

	for (int i = 0; i < length; i++) {
		sum_X += X[i];
		sum_Y += Y[i];
		sum_XY += X[i] * Y[i];
		square_sumX += X[i] * X[i];
		square_sumY += Y[i] * Y[i];
	}

	double corr;
	corr = ((double)length * sum_XY - sum_X * sum_Y) / sqrt((length * square_sumX - sum_X * sum_X) * (length * square_sumY - sum_Y * sum_Y));

	return corr;
}

double corr(double* X, double* Y, int length) {
	// Just for reference: https://www.geeksforgeeks.org/program-find-correlation-coefficient/
	double sum_X = 0;
	double sum_Y = 0;
	double sum_XY = 0;
	double square_sumX = 0;
	double square_sumY = 0;

	for (int i = 0; i < length; i++) {
		sum_X += X[i];
		sum_Y += Y[i];
		sum_XY += X[i] * Y[i];
		square_sumX += X[i] * X[i];
		square_sumY += Y[i] * Y[i];
	}

	double corr;
	corr = ((double)length * sum_XY - sum_X * sum_Y) / sqrt((length * square_sumX - sum_X * sum_X) * (length * square_sumY - sum_Y * sum_Y));

	return corr;
}

double corr(int* X, double* Y, int length) {
	// Just for reference: https://www.geeksforgeeks.org/program-find-correlation-coefficient/
	int sum_X = 0;
	double sum_Y = 0;
	double sum_XY = 0;
	double square_sumX = 0;
	double square_sumY = 0;

	for (int i = 0; i < length; i++) {
		sum_X += X[i];
		sum_Y += Y[i];
		sum_XY += (double)X[i] * Y[i];
		square_sumX += (double)X[i] * (double)X[i];
		square_sumY += Y[i] * Y[i];
	}

	double corr;
	corr = ((double)length * sum_XY - (double)sum_X * sum_Y) / sqrt((length * square_sumX - (double)sum_X * (double)sum_X) * (length * square_sumY - sum_Y * sum_Y));

	return corr;
}

double corr(double* X, int* Y, int length) {
	// Just for reference: https://www.geeksforgeeks.org/program-find-correlation-coefficient/
	double sum_X = 0;
	int sum_Y = 0;
	double sum_XY = 0;
	double square_sumX = 0;
	double square_sumY = 0;

	for (int i = 0; i < length; i++) {
		sum_X += X[i];
		sum_Y += Y[i];
		sum_XY += X[i] * (double)Y[i];
		square_sumX += X[i] * X[i];
		square_sumY += (double)Y[i] * (double)Y[i];
	}

	double corr;
	corr = ((double)length * sum_XY - sum_X * (double)sum_Y) / sqrt((length * square_sumX - sum_X * sum_X) * (length * square_sumY - (double)sum_Y * (double)sum_Y));

	return corr;
}

double stand_dev(int* array, int length) {
	double mean;
	mean = average(array, length);

	double working_sum = 0.0;

	for (int i = 0; i < length; i++) {
		working_sum += ((double)array[i] - mean) * ((double)array[i] - mean);
	}
	working_sum = (1 / ((double)length - 1)) * working_sum;

	working_sum = sqrt(working_sum);

	return working_sum;
}

double stand_dev(vector<short int> array, int length) {
	double mean;
	mean = average(array, length);

	double working_sum = 0.0;

	for (int i = 0; i < length; i++) {
		working_sum += ((double)array[i] - mean) * ((double)array[i] - mean);
	}
	working_sum = (1 / ((double)length - 1)) * working_sum;

	working_sum = sqrt(working_sum);

	return working_sum;
}

double stand_dev(double* array, int length) {
	double mean;
	mean = average(array, length);

	double working_sum = 0.0;

	for (int i = 0; i < length; i++) {
		working_sum += (array[i] - mean) * (array[i] - mean);
	}
	working_sum = (1 / ((double)length - 1)) * working_sum;

	working_sum = sqrt(working_sum);

	return working_sum;
}