#ifndef _REGRESSION_H_
#define _REGRESSION_H_

#include <string>
#include <vector>
#include "matrix.h"

using namespace std;

class index_value {
	size_t index;
	float value;
public:
	index_value(size_t index, float value):index(index),value(value) {}
	size_t getIndex() {return index;} 
	float getValue() {return value;}
};

class Regression {
	
	size_t col_len;
	size_t row_len;
	vector<float> *predict_vec;
	void read_train_file(Matrix *matrix, string train_file);
public:
	Regression() {
		predict_vec = new vector<float>();
	}
	~Regression() {
		delete predict_vec;
	}

	void init();
	void release();
	int train(string train_file, string model_file, float ridge_e);
	int predict(string predict_file, string model_file, string result_file);
	int validate(string pre_result_file, string validate_file);
};

#endif
