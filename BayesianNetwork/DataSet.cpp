#include "stdafx.h"
#include "DataSet.h"
#include <stdexcept>

int getColumnsNum(vector<vector<int>> v) {
	return v.at(0).size();
}

vector<set<string>> extractParams(vector<vector<int>> v) {
	int columnsNum = getColumnsNum(v);

	vector<set<string>> paramsSets = vector<set<string>>(0);
	for (vector<int>::size_type columnIndex = 0; columnIndex != columnsNum; columnIndex++) {
		set<string> params = set<string>();
		for (auto const& line : v) {
			auto value = line.at(columnIndex);
			params.insert(to_string(value));
		}
		paramsSets.push_back(params);
	}
	return paramsSets;
}

DataSet::DataSet(vector<vector<int>> v)
{
	params = extractParams(v);
	data = vector<vector<int>>{ {0}, {0}, {0} };
}

set<string> DataSet::paramsForColumn(int columnNum)
{
	return params.at(columnNum);
}