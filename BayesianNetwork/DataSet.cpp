#include "stdafx.h"
#include "DataSet.h"
#include "Templates.cpp"
#include <stdexcept>

#ifndef CUSTOM_TEMPLATE_BN
#define CUSTOM_TEMPLATE_BN

#pragma once

int getColumnsNum(vector<vector<int>> v) {
	return v.at(0).size();
}

vector<int> valuesInColumn(vector<vector<int>> v, int columnIndex) 
{
	return map<vector<int>, int>(v, [columnIndex](vector<int> line) -> int {
		return line.at(columnIndex);
	});
}

vector<vector<int>> getValues(vector<vector<int>> v, vector<vector<int>> params) {
	vector<vector<int>> values = vector<vector<int>>();
	for (int columnIndex = 0; columnIndex != params.size(); columnIndex++) {
		vector<int> valuesAtColumn = valuesInColumn(v, columnIndex);
		vector<int> paramsAtColumn = params.at(columnIndex);
		vector<int> valuesInColumnUnlabeled = map<int, int>(valuesAtColumn, [paramsAtColumn](int i) -> int {
			return indexOf<int>(paramsAtColumn, i);
		});
		values.push_back(valuesInColumnUnlabeled);
	}
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

vector<int> DataSet::paramsForColumn(int columnNum)
{
	return params.at(columnNum);
}

vector<int> DataSet::countParams(int columnNum)
{
	return countParams(columnNum, vector<pair<int, int>> { });
}

vector<int> intZeroes(int size) {
	vector<int> v = vector<int>(size);
	std::fill(v.begin(), v.end(), 0);
	return v;
}

vector<int> DataSet::countParams(int columnNum, vector<pair<int, int>> restrictions)
{
	vector<int> column = data.at(columnNum);
	int rows = column.size();
	vector<int> counter = intZeroes(params.size());
	for (int row = 0; row < rows; row++) {
		 int paramIndex = column.at(row);
		 if(paramIndex >= 0 && paramIndex < params.size())
			counter.at(paramIndex)++;
	}
	return counter;
}

#endif