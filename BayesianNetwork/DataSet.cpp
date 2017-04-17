#include "stdafx.h"
#include "DataSet.h"
#include "Templates.cpp"
#include <stdexcept>

#ifndef CUSTOM_TEMPLATE_BN
#define CUSTOM_TEMPLATE_BN

int getColumnsNum(vector<vector<int>> v) {
	return v.at(0).size();
}

vector<vector<int>> getValues(vector<vector<int>> v, vector<vector<int>> params) {
	vector<vector<int>> values = map<vector<int>, vector<int>>(v, [params](vector<int> line) -> vector<int> { 
		return map<int, int>(line, [params](int line) -> int {
			return 0;
		});
	}
	);
	int columnsNum = getColumnsNum(v);
	return values;
}

vector<vector<int>> extractParams(vector<vector<int>> v) {
	int columnsNum = getColumnsNum(v);

	vector<vector<int>> paramsSets = vector<vector<int>>(0);
	for (vector<int>::size_type columnIndex = 0; columnIndex != columnsNum; columnIndex++) {
		vector<int> params = vector<int>();
		for (auto const& line : v) {
			auto value = line.at(columnIndex);
			params.push_back(value);
		}
		paramsSets.push_back(uniquify(params));
	}
	return paramsSets;
}

DataSet::DataSet(vector<vector<int>> v)
{
	params = extractParams(v);
	data = getValues(v, params);
}

vector<string> DataSet::paramsForColumn(int columnNum)
{
	return map<int, string>(params.at(columnNum), [](int it) -> string { return to_string(it); });
}

#endif