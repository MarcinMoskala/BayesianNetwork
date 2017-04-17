#include <vector>
#include <set>
#include "DataSet.h"

using namespace std;

#pragma once
class BayesianNetwork
{
public:
	BayesianNetwork(DataSet dataSet);
	void learnParams(DataSet dataSet);
	void learnStructure(DataSet dataSet);
	void addConnection(int fromIndex, int toIndex);
	void removeConnection(int fromIndex, int toIndex);
	bool isConnection(int fromIndex, int toIndex);
	vector<long double> qualityFunction(vector<int> entry);
	vector<long double> evaluate(vector<int> entry);

	~BayesianNetwork();
};

