#include "stdafx.h"
#include <vector>
#include <set>
#include <map>

#include "Templates.cpp"
#include <stdexcept>

using namespace std;

#pragma once
template<class T>
class DataSet
{
private:
	// Possible value of data on this column
	vector<vector<T>> params;

	// Number of param from 0 in column
	// Rows are the columns of data!
	vector<vector<int>> data;

	DataSet() {};
public:

	DataSet(vector<vector<T>> arr);
	vector<T> paramsForColumn(int columnNum);
	vector<int> countParams(int columnNum);
	vector<int> countParams(int columnNum, map<int, T> knowladge);

	vector<T> getDataInColumn(int col) {
		return data.at(col);
	}

	vector<vector<T>> getDataPoints();

	int dataPointsNum() {
		return data.at(0).size();
	}

	int paramsNum() {
		return params.size();
	}

	~DataSet() {};
};

// DataSet.cpp
#ifndef DATA_SET
#define DATA_SET


template<class T>
int getColumnsNum(vector<vector<T>> v) {
	return v.at(0).size();
}

template<class T>
vector<T> valuesInColumn(vector<vector<T>> v, int columnIndex)
{
	return mapTo<vector<T>, T>(v, [columnIndex](vector<int> line) -> int {
		return line.at(columnIndex);
	});
}

template<class T>
vector<vector<int>> getValues(vector<vector<T>> v, vector<vector<T>> params) {
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

template<class T>
vector<vector<T>> extractParams(vector<vector<T>> v) {
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

template<class T>
DataSet<T>::DataSet(vector<vector<T>> v)
{
	params = extractParams(v);
	data = getValues(v, params);
}

template<class T>
vector<T> DataSet<T>::paramsForColumn(int columnNum)
{
	return params.at(columnNum);
}

template<class T>
vector<int> DataSet<T>::countParams(int columnNum)
{
	return countParams(columnNum, map<int, T>());
}

template<class T>
vector<int> DataSet<T>::countParams(int columnNum, map<int, T> knowladge)
{
	vector<int> column = data.at(columnNum);
	int rows = column.size();

	int size = params.at(columnNum).size();
	vector<int> counter = vector<int>(size);
	std::fill(counter.begin(), counter.end(), 0);
	auto par = params;
	for (int row = 0; row < rows; row++) {
		if (any(knowladge, [this, row, par](pair<T, int> r) -> bool {
			int colNum = r.first;
			auto dataInColumn = this->getDataInColumn(colNum);
			auto v = par.at(colNum).at(dataInColumn.at(row));
			return v != r.second;
		})) continue;

		int index = column.at(row);
		if (index >= 0 && index <= params.size())
			counter.at(index)++;
	}
	return counter;
}

template<class T>
vector<vector<T>> DataSet<T>::getDataPoints() {
	int dPNum = dataPointsNum();
	int pNum = paramsNum();
	vector<vector<T>> dataPoints = {};
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