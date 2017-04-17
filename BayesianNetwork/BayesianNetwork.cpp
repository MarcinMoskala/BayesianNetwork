#include "stdafx.h"
#include "BayesianNetwork.h"
#include "Templates.cpp"

vector<vector<long double>> matrixOf(int c, int r, long double value) {
	vector<vector<long double>> m = {};
	for (int i = 0; i < r; i++) {
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
	long double p = 1.0L / r;
	return matrixOf(c, r, p);
}

BayesianNetwork::~BayesianNetwork()
{
}

BayesianNetwork::BayesianNetwork(DataSet dataSet)
{
	nodes = vector<Node>{};
	for (int i = 0; i < dataSet.columnsNum(); i++) {
		auto params = dataSet.paramsForColumn(i);
		Node node = Node(params);
		nodes.push_back(node);
	}
	learnParams(dataSet);
}

void BayesianNetwork::learnParams(DataSet dataSet)
{
	for_each_indexed(nodes, [dataSet](Node node, int index) -> void {
//		vector<int> data = dataSet.getDataInColumn(index);
//		return i + 1;
	});
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

long double BayesianNetwork::probabilityOf(int node, int valueParam)
{
	return nodes.at(node).probabilityOf(valueParam);
}

vector<long double> BayesianNetwork::evaluate(vector<int> entry)
{
	return mapIndexed<int, long double>(entry, [this](int e, int index) -> long double {
		return this->probabilityOf(index, e);
	});
}

long double BayesianNetwork::qualityFunction(DataSet dataSet)
{
	return 0;
}

// TODO Copy
vector<BayesianNetwork::Node> BayesianNetwork::getNodes() {
	return nodes;
}

long double BayesianNetwork::Node::probabilityOf(int valueParam)
{
	int index = indexOf(params, valueParam);
	if (index < 0 || index >= probabilities.size())
		return 0.0L;
	auto probs = probabilities.at(index);
	long double sumOfP = sumBy(probs, 0.0L, [this](long double p) -> long double {
		return p;
	});
	return sumOfP;
}

BayesianNetwork::Node::Node(vector<int> params)
	:BayesianNetwork::Node::Node(params, vector<int> {})
{
}

BayesianNetwork::Node::Node(vector<int> params, vector<int> parentNodes)
	:BayesianNetwork::Node::Node(params, parentNodes, equallyDistributedRows(pow(2, parentNodes.size()), params.size()))
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
