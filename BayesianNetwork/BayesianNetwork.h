#include <vector>
#include <set>
#include "DataSet.h"

using namespace std;

#pragma once
class BayesianNetwork
{
	class Node;
	vector<Node> nodes;

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

	class Node {
		vector<int> parentNodes;
		vector<int> params;
		// Row are parents probability (2^parents columns num), 
		// column is predicted probability of param by index on params (equeal to params.size())
		vector<vector<long double>> probabilities;
	};
	friend class Node;
};

