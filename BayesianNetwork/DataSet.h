#include <vector>
#include <set>
#include <map>

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
	vector<int> paramsForColumn(int columnNum);
	vector<int> countParams(int columnNum);
	vector<int> countParams(int columnNum, map<int, int> knowladge);

	vector<int> getDataInColumn(int col) {
		return data.at(col);
	}

	int rowNum() {
		return data.size();
	}

	int columnsNum() {
		return params.size();
	}

	~DataSet() {};
};

