#include "utils.h"

void UTILS :: strsplit(string src_line, string split, vector<string> &split_vec) {
	size_t split_len = split.size();
	size_t init_pos = 0;
	size_t pos = src_line.find(split);
	while (pos != string::npos) {
		string sub = src_line.substr(init_pos, pos - init_pos);
		split_vec.push_back(sub);
		init_pos = pos + split_len;
		pos = src_line.find(split, init_pos);
	}
	string sub = src_line.substr(init_pos);
	split_vec.push_back(sub);

}
