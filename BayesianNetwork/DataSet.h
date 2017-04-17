#include <vector>
#include <set>

using namespace std;

#pragma once
class DataSet
{
private:
	// Possible value of data on this column
	vector<set<string>> params;

	// Number of param from 0 in column
	vector<vector<int>> data;

	DataSet() {};
public:

	DataSet(vector<vector<int>> arr);
	set<string> paramsForColumn(int columnNum);

	~DataSet() {};
};

