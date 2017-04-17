#include <vector>
#include <set>

using namespace std;

#pragma once
class DataSet
{
private:
	// Possible value of data on this column
	vector<vector<int>> params;

	// Number of param from 0 in column
	vector<vector<int>> data;

	DataSet() {};
public:

	DataSet(vector<vector<int>> arr);
	vector<string> paramsForColumn(int columnNum);

	vector<vector<int>> valuesInRow(int row) {
		return data;
	}

	int rows() {
		return data.size();
	}

	int columns() {
		return params.size();
	}

	~DataSet() {};
};

