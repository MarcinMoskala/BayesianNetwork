#include "stdafx.h"
#include "BayesianNetwork.h"
#include "Templates.cpp"

vector<vector<long double>> matrixOf(int c, int r, long double value) {
	vector<vector<long double>> m = {};
	for (int i = 0; i < c; i++) {
		vector<long double> v = {};
		for (int j = 0; j < c; j++)
			v.push_back(value);
		m.push_back(v);
	}
	return m;
}

vector<vector<long double>> zeroes(int c, int r) {
	return matrixOf(c, r, 0.0L);
}

vector<vector<long double>> ones(int c, int r) {
	return matrixOf(c, r, 1.0L);
}

vector<vector<long double>> equallyDistributedRows(int c, int r) {
	long double p = 1.0L / c;
	return matrixOf(c, r, p);
}

BayesianNetwork::~BayesianNetwork()
{
}

BayesianNetwork::BayesianNetwork(DataSet dataSet)
{
	nodes = map<vector<int>, Node>(dataSet.getParams(), [](vector<int> p) -> Node {
		return Node(p);
	});
	learnParams(dataSet);
}

void BayesianNetwork::learnParams(DataSet dataSet)
{
}

void BayesianNetwork::learnStructure(DataSet dataSet)
{
}

void BayesianNetwork::addConnection(int fromIndex, int toIndex)
{
}

void BayesianNetwork::removeConnection(int fromIndex, int toIndex)
{
}

bool BayesianNetwork::isConnection(int fromIndex, int toIndex)
{
	return false;
}

vector<long double> BayesianNetwork::qualityFunction(vector<int> entry)
{
	return vector<long double>();
}

vector<long double> BayesianNetwork::evaluate(vector<int> entry)
{
	return vector<long double>();
}

// TODO Copy
vector<BayesianNetwork::Node> BayesianNetwork::getNodes() {
	return nodes;
}

BayesianNetwork::Node::Node(vector<int> params)
	:BayesianNetwork::Node::Node(params, vector<int> {})
{
}

BayesianNetwork::Node::Node(vector<int> params, vector<int> parentNodes)
	:BayesianNetwork::Node::Node(params, parentNodes, equallyDistributedRows(params.size(), pow(2, parentNodes.size())))
{
}
BayesianNetwork::Node::Node(vector<int> params, vector<int> parentNodes, vector<vector<long double>> probabilities)
	: params(params), parentNodes(parentNodes), probabilities(probabilities) 
{
}

BayesianNetwork::Node::Node(const Node& n)
	: params(n.params), parentNodes(n.parentNodes), probabilities(n.probabilities)
{
}

BayesianNetwork::Node::~Node()
{
}
