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
	BayesianNetwork(DataSet<int> dataSet);
	BayesianNetwork withParamsLearned(DataSet<int> dataSet);
	BayesianNetwork withStructureLearned(DataSet<int> dataSet);
	BayesianNetwork withConnection(int fromIndex, int toIndex);
	BayesianNetwork withoutConnection(int fromIndex, int toIndex);
	bool haveConnection(int fromIndex, int toIndex);
	bool isCorrect();
	long double probabilityOf(int node, int value, map<int, int> knowlagde);
	long double probabilityOf(int node, int value);
	vector<long double> evaluate(vector<int> entry);
	long double qualityFunction(DataSet<int> dataSet);

	vector<Node> getNodes();

	~BayesianNetwork();

	class Node
	{
		BayesianNetwork* network;
		vector<int> parentNodes;
		vector<int> params;
		vector<long double> paramDistribution;
		//List of values indexes for next parents to probability od properties
		map<vector<int>, vector<long double>> conditionalProbability = {};
		friend class BayesianNetwork;
		long double possibilityOf(vector<int> situation, map<int, int> knowladge, vector<Node> parents);

	public:
		Node(BayesianNetwork* network, vector<int> params);
		Node(BayesianNetwork* network, vector<int> params, vector<int> parentNodes);
		Node(const Node &);
		long double probabilityOf(int value, map<int, int> knowladge);
		Node withParamsLearned(DataSet<int> dataSet, int index);

		vector<int> getParams() {
			return vector<int>(params);
		}

		vector<int> getParentNodes() {
			return vector<int>(parentNodes);
		}

		int getParentNodeNum() {
			if (parentNodes.empty())
				return 0;
			else
				return parentNodes.size();
		}
		
		~Node();
	};

	friend class Node;
};

