#include <iostream>
#include <fstream>
#include <vector>
#include <map>

#include "regression.h"
#include "matrix.h"
#include "utils.h"

using namespace std;

void read_file(Matrix *matrix, string file) {
	UTILS utils;
	string line;
	ifstream in_f(file.c_str());
	size_t row = 0;
	while (getline(in_f, line)) {
		vector<string> split_vec;
		utils.strsplit(line, " ", split_vec);
		size_t col = 0;
		for (vector<string>::iterator it_vec = split_vec.begin(); 
				it_vec != split_vec.end(); it_vec++) {
			cout << *it_vec << " ";
			string d_s = *it_vec;
			int d = atoi(d_s.c_str());
			set_value(matrix, row, col, d);
			col++;
		}
		cout << endl;
		row++;
	}
	in_f.close();
}

void Regression :: init() {
	
}

void Regression :: release() {
}

void Regression :: read_train_file(Matrix *matrix, string train_file) {
	UTILS utils;
	string line;
	ifstream in_train(train_file.c_str());
	size_t row = 0;
	size_t max_col = 0;
	vector<vector<index_value *> *> *matrix_vec_vec;
	matrix_vec_vec = new vector<vector<index_value *> *>();
	while (getline(in_train, line)) {
		vector<string> split_vec;
		utils.strsplit(line, " ", split_vec);
		bool first;
		vector<index_value *> *iv_vec = new vector<index_value *>();
		matrix_vec_vec->push_back(iv_vec);
		for (vector<string>::iterator it_vec = split_vec.begin(); 
				it_vec != split_vec.end(); it_vec++) {
			if (first) {
				string pre_v = *it_vec;
				float pre_d = atof(pre_v.c_str());
				predict_vec->push_back(pre_d);
				first = false;
			} else {
				string index_fea = *it_vec;
				string index = index_fea.substr(0, index_fea.find(":"));
				string fea = index_fea.substr(index_fea.find(":") + 1);
				size_t index_i = atoi(index.c_str());
				if (index_i > max_col) max_col = index_i;
				float fea_f = atof(fea.c_str());
				index_value *iv = new index_value(index_i, fea_f);
				iv_vec->push_back(iv);
			}
		}
		row++;
	}

	col_len = max_col;
	row_len = row;
	in_train.close();
	
	init_matrix(matrix, row_len, col_len);
	
	vector<vector<index_value *> *>::iterator it_vec_vec;
	row = 0;
	for (it_vec_vec = matrix_vec_vec->begin(); it_vec_vec != matrix_vec_vec->end(); it_vec_vec++) {
		vector<index_value *> *matrix_vec = *it_vec_vec;
		vector<index_value *>::iterator it_vec;
		for (it_vec = matrix_vec->begin(); it_vec != matrix_vec->end(); it_vec++) {
			index_value *iv = *it_vec;
			size_t index = iv->getIndex();
			float value = iv->getValue();
			set_value(matrix, row, index - 1, value);
			delete iv;
		}
		delete matrix_vec;
		row++;
	}
	delete matrix_vec_vec;
}

void Regression :: train(string src_file, string model_file) {
	
}

void Regression :: predict(string src_file, string model_file, string tgt_file) {
	
}

void Regression :: validate(string predict_file, string pre_result_file) {
	
}

void mention_msg() {
	cout << "train train_file model_file" << endl;
	cout << "predict predict_file model_file result_file" << endl;
}

int main(int argc, char **argv) {
	if (argc < 3) {
		mention_msg();
		return 1;
	}
	string comm = string(argv[1]);
	if (comm == "train") {
		string train_file = string(argv[2]);
		string model_file = string(argv[3]);
		Regression reg;
		reg.train(train_file, model_file);
	} else if (comm == "predict") {
		string predict_file = string(argv[2]);
		string model_file = string(argv[3]);
		string result_file = string(argv[4]);
		Regression reg;
		reg.predict(predict_file, model_file, result_file);
	} else {
		mention_msg();
	}
}

