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
		bool first = true;
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

int Regression :: train(string src_file, string model_file, float ridge_e) {
	Matrix matrix, tr_matrix, multi_matrix;
	read_train_file(&matrix, src_file);
	fprintf(stderr, "train matrix init ok\n");
	transpose(&matrix, &tr_matrix);
	fprintf(stderr, "transpos ok\n");
	multiple(&tr_matrix, &matrix, &multi_matrix);
	fprintf(stderr, "multiple 1 ok\n");
	release_matrix(&matrix);
	//ridge regression;
	add_E(&multi_matrix, ridge_e);
	bool is_inv = inverse(&multi_matrix, &matrix);
	if (is_inv != 0) {
		release_matrix(&matrix);
		release_matrix(&tr_matrix);
		release_matrix(&multi_matrix);
		fprintf(stderr, "There is no inverse matrix!\n");
		return 1;
	}
	fprintf(stderr, "inverse ok\n");
	release_matrix(&multi_matrix);
	multiple(&matrix, &tr_matrix, &multi_matrix);
	fprintf(stderr, "multiple transpose ok\n");
	size_t predict_len = predict_vec->size();
	Matrix predict_matrix;
	init_matrix(&predict_matrix, predict_len, 1);
	float *pre_arr = predict_matrix.arr;
	for (size_t i = 0; i < predict_vec->size(); i++) {
		float pre = predict_vec->at(i);
		*(pre_arr + i) = pre;
	}
	Matrix weight_matrix;
	multiple(&multi_matrix, &predict_matrix, &weight_matrix);
	fprintf(stderr, "multiple predict ok\n");
	float *weight_arr = weight_matrix.arr;
	size_t wei_col = weight_matrix.row_len;
	ofstream out_model(model_file.c_str());
	for (size_t i = 0; i < wei_col; i++) {
		float weight = *(weight_arr + i);
		if (i == 0) out_model << i + 1 << ":" << weight;
		else out_model << " " << i + 1 << ":" << weight;
	}
	out_model << endl;
	fprintf(stderr, "model output ok\n");
	release_matrix(&weight_matrix);
	release_matrix(&predict_matrix);
	release_matrix(&matrix);
	release_matrix(&tr_matrix);
	release_matrix(&multi_matrix);
	out_model.close();
	return 0;
}

int Regression :: predict(string predict_file, string model_file, string result_file) {
	UTILS utils;
	string line;
	map<size_t, float> *coeff_map = new map<size_t, float>();
	//init model
	ifstream in_model(model_file.c_str());
	if (getline(in_model, line)) {
		vector<string> split_vec;
		utils.strsplit(line, " ", split_vec);
		for (vector<string>::iterator it_vec = split_vec.begin(); it_vec != split_vec.end(); it_vec++) {
			string index_coeff = *it_vec;
			string index = index_coeff.substr(0, index_coeff.find(":"));
			string coeff = index_coeff.substr(index_coeff.find(":") + 1);
			int index_i = atoi(index.c_str());
			float coeff_f = atof(coeff.c_str());
			coeff_map->insert(pair<size_t, float>(index_i, coeff_f));
		}
	}
	in_model.close();

	//read predict file
	ofstream out_result(result_file.c_str());
	ifstream in_predict(predict_file.c_str());
	map<size_t, float>::iterator it_coeff;
	while (getline(in_predict, line)) {
		vector<string> split_vec;
		utils.strsplit(line, " ", split_vec);
		bool first = true;
		float result = 0.0f;
		for (vector<string>::iterator it_vec = split_vec.begin(); 
				it_vec != split_vec.end(); it_vec++) {
			if (first) {
				first = false;
			} else {
				string index_fea = *it_vec;
				string index = index_fea.substr(0, index_fea.find(":"));
				string fea = index_fea.substr(index_fea.find(":") + 1);
				size_t index_i = atoi(index.c_str());
				it_coeff = coeff_map->find(index_i);
				if (it_coeff != coeff_map->end()) {
					float coeff = it_coeff->second;
					float fea_f = atof(fea.c_str());
					result += coeff * fea_f;
				}
			}
		}
		out_result << result << endl;
	}
	in_predict.close();
	out_result.close();
	delete coeff_map;
	return 0;
}

int Regression :: validate(string predict_file, string pre_result_file) {
	ifstream in_predict(predict_file.c_str());
	ifstream in_pre_result(pre_result_file.c_str());
	string line_predict, line_pre_result;
	float mse = 0;
	size_t line_num = 0;
	while (getline(in_predict, line_predict) && getline(in_pre_result, line_pre_result))
	{
		string pre = line_predict.substr(0, line_predict.find(" "));
		float pre_f = atof(pre.c_str());
		float pre_result_f = atof(line_pre_result.c_str());
		mse += (pre_f - pre_result_f) * (pre_f - pre_result_f);
		line_num++;
	}
	in_pre_result.close();
	in_predict.close();
	if (line_num != 0) {
		mse /= line_num;
		fprintf(stderr, "mse:%f\n", mse);
	} else fprintf(stderr, "no predict\n");
	return 0;
}

void mention_msg() {
	cout << "train train_file model_file ridge_e" << endl;
	cout << "predict predict_file model_file result_file" << endl;
	cout << "valid predict_file pre_result_file" << endl;
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
		string ridge_e_s = string(argv[4]);
		float ridge_f = atof(ridge_e_s.c_str());
		Regression reg;
		int r = reg.train(train_file, model_file, ridge_f);
		if (r == 0) fprintf(stderr, "train ok\n");
		else fprintf(stderr, "train failed\n");
	} else if (comm == "predict") {
		string predict_file = string(argv[2]);
		string model_file = string(argv[3]);
		string result_file = string(argv[4]);
		Regression reg;
		reg.predict(predict_file, model_file, result_file);
		cout << "predict ok" << endl;
	} else if (comm == "valid") {
		string predict_file = string(argv[2]);
		string pre_result_file = string(argv[3]);
		Regression reg;
		reg.validate(predict_file, pre_result_file);
		cout << "validate ok" << endl;
	} else {
		mention_msg();
	}
}

