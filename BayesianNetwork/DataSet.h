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

	vector<vector<int>> getDataPoints();

	int dataPointsNum() {
		return data.at(0).size();
	}

	int paramsNum() {
		return params.size();
	}

	~DataSet() {};
};

