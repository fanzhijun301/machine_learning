#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "matrix.h"

using namespace std;

void strsplit(string src_line, string split, vector<string> &split_vec) {
	int split_len = split.size();
	int init_pos = 0;
	int pos = src_line.find(split);
	while (pos != string::npos) {
		string sub = src_line.substr(init_pos, pos - init_pos);
		split_vec.push_back(sub);
		init_pos = pos + split_len;
		pos = src_line.find(split, init_pos);
	}
	string sub = src_line.substr(init_pos);
	split_vec.push_back(sub);
}

void test_trans() {
	Matrix matrix, inv_matrix;
	init(&matrix, 4, 4);
	init(&inv_matrix, 4, 4);
	int i, j, s = 1;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++)	{
			set_value(&matrix, i, j, s);
			s++;
		}
	}
	
	print_matrix(&matrix);
	//transpose(&matrix);
	printf("====================\n");
	inverse(&matrix, &inv_matrix);
	print_matrix(&inv_matrix);
	release(&matrix);
}

void read_file(Matrix *matrix, string file) {
	string line;
	ifstream in_f(file.c_str());
	size_t row = 0;
	while (getline(in_f, line)) {
		vector<string> split_vec;
		strsplit(line, " ", split_vec);
		size_t col = 0;
		for (vector<string>::iterator it_vec = split_vec.begin(); it_vec != split_vec.end(); it_vec++) {
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

void test_inverse() {
	char *file = "matr_data.txt";
	Matrix matrix, inv_matrix;
	init(&matrix, 5, 5);
	init(&inv_matrix, 5, 5);
	read_file(&matrix, file);
	printf("====================\n");
	print_matrix(&matrix);
	inverse(&matrix, &inv_matrix);
	printf("====================\n");
	print_matrix(&inv_matrix);
	release(&matrix);
	release(&inv_matrix);
}

int main(int argc, char **argv) {
	test_inverse();
}
