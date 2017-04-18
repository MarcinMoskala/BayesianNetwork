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
	return mapTo<vector<int>, int>(v, [columnIndex](vector<int> line) -> int {
		return line.at(columnIndex);
	});
}

vector<vector<int>> getValues(vector<vector<int>> v, vector<vector<int>> params) {
	vector<vector<int>> values = vector<vector<int>>();
	for (int columnIndex = 0; columnIndex != params.size(); columnIndex++) {
		vector<int> valuesAtColumn = valuesInColumn(v, columnIndex);
		vector<int> paramsAtColumn = params.at(columnIndex);
		vector<int> valuesInColumnUnlabeled = mapTo<int, int>(valuesAtColumn, [paramsAtColumn](int i) -> int {
			return indexOf<int>(paramsAtColumn, i);
		});
		values.push_back(valuesInColumnUnlabeled);
	}
	return values;
}

vector<vector<int>> extractParams(vector<vector<int>> v) {
	int paramsNum = getColumnsNum(v);

	vector<vector<int>> paramsSets = vector<vector<int>>(0);
	for (vector<int>::size_type columnIndex = 0; columnIndex != paramsNum; columnIndex++) {
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
	return countParams(columnNum, map<int, int>());
}

vector<int> intZeroes(int size) {
	vector<int> v = vector<int>(size);
	std::fill(v.begin(), v.end(), 0);
	return v;
}

vector<int> DataSet::countParams(int columnNum, map<int, int> knowladge)
{
	vector<int> column = data.at(columnNum);
	int rows = column.size();
	vector<int> counter = intZeroes(params.at(columnNum).size());
	auto par = params;
	for (int row = 0; row < rows; row++) {
		if (any(knowladge, [this, row, par](pair<int, int> r) -> bool {
			int colNum = r.first;
			auto dataInColumn = this->getDataInColumn(colNum);
			auto v = par.at(colNum).at(dataInColumn.at(row));
			return v != r.second;
		})) continue;

		 int index = column.at(row);
		 if(index >= 0 && index <= params.size())
			counter.at(index)++;
	}
	return counter;
}

vector<vector<int>> DataSet::getDataPoints() {
	int dPNum = dataPointsNum();
	int pNum = paramsNum();
	vector<vector<int>> dataPoints = {};
	for (int i = 0; i < dPNum; i++) {
		vector<int> dataPoint = {};
		for (int j = 0; j < pNum; j++) {
			auto dataIndex = data.at(j).at(i);
			auto data = params.at(j).at(dataIndex);
			dataPoint.push_back(data);
		}
		dataPoints.push_back(dataPoint);
	}
	return dataPoints;
}

#endif