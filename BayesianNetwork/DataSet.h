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
	// Rows are the columns of data!
	vector<vector<int>> data;

	DataSet() {};
public:

	DataSet(vector<vector<int>> arr);
	vector<string> paramsForColumn(int columnNum);

	vector<vector<int>> valuesInRow(int row) {
		return data;
	}

	vector<vector<int>> getParams() {
		return params;
	}

	int rowNum() {
		return data.size();
	}

	int columnsNum() {
		return params.size();
	}

	~DataSet() {};
};

