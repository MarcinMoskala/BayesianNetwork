#include <vector>
#include <set>
#include "DataSet.h"

using namespace std;

#pragma once
class BayesianNetwork
{
public: 
	class Node;
private:
	vector<Node> nodes;
public:
	BayesianNetwork(DataSet dataSet);
	void learnParams(DataSet dataSet);
	void learnStructure(DataSet dataSet);
	void addConnection(int fromIndex, int toIndex);
	void removeConnection(int fromIndex, int toIndex);
	bool isConnection(int fromIndex, int toIndex);
	long double probabilityOf(int node, int value);
	vector<long double> evaluate(vector<int> entry);
	long double qualityFunction(DataSet dataSet);

	vector<Node> getNodes();

	~BayesianNetwork();

	class Node
	{
		vector<int> parentNodes;
		vector<int> params;
		// Columns are parents probability (2^parents columns num), 
		// Row is predicted probability of param by index on params (equal to params.size())
		vector<vector<long double>> probabilities;

	public:
		Node(vector<int> params);
		Node(vector<int> params, vector<int> parentNodes);
		Node(vector<int> params, vector<int> parentNodes, vector<vector<long double>> probabilities);
		Node(const Node &);
		long double probabilityOf(int value);
		~Node();
	};
};

