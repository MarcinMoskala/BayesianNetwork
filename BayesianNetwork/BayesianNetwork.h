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
	void learnParams(DataSet dataSet);
public:
	BayesianNetwork(DataSet dataSet);
	BayesianNetwork withParamsLearned(DataSet dataSet);
	BayesianNetwork withStructureLearned(DataSet dataSet);
	BayesianNetwork withConnection(int fromIndex, int toIndex);
	BayesianNetwork withoutConnection(int fromIndex, int toIndex);
	bool haveConnection(int fromIndex, int toIndex);
	long double probabilityOf(int node, int value, map<int, int> knowlagde);
	long double probabilityOf(int node, int value);
	vector<long double> evaluate(vector<int> entry);
	long double qualityFunction(DataSet dataSet);

	vector<Node> getNodes();

	~BayesianNetwork();

	class Node
	{
		vector<int> parentNodes;
		vector<int> params;
		// Row is predicted probability of param by index on params (equal to params.size())
		// Columns are parents probability (2^parents columns num)
		vector<vector<long double>> probabilities;
		int colNum;
		int rowNum;
		friend class BayesianNetwork;

	public:
		Node(vector<int> params);
		Node(vector<int> params, vector<int> parentNodes);
		Node(const Node &);
		long double probabilityOf(int value, map<int, int> knowladge);
		Node withParamsLearned(DataSet dataSet, int index);
		~Node();

		static bool parentOnInRowId(int rowCode, int row) {
			return rowCode >> row % 2 == 1;
		}
	};
};

