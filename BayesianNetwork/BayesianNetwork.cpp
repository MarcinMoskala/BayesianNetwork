#include "stdafx.h"
#include "BayesianNetwork.h"
#include "Templates.cpp"

vector<vector<long double>> matrixOf(int colNum, int rowNum, long double value) {
	vector<vector<long double>> m = {};
	for (int i = 0; i < rowNum; i++) {
		vector<long double> v = {};
		for (int j = 0; j < colNum; j++)
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

vector<vector<long double>> equallyDistributedRows(int colNum, int rowNum) {
	long double p = 1.0L / rowNum;
	return matrixOf(colNum, rowNum, p);
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
}



BayesianNetwork BayesianNetwork::withParamsLearned(DataSet dataSet)
{
	auto bn = BayesianNetwork(*this);
	bn.nodes = mapIndexed<Node, Node>(nodes, [dataSet](Node node, int index) -> Node {
		return node.withParamsLearned(dataSet, index);
	});
	return bn;
}

BayesianNetwork BayesianNetwork::withStructureLearned(DataSet dataSet)
{
	return BayesianNetwork(*this);
}

BayesianNetwork BayesianNetwork::withConnection(int fromIndex, int toIndex)
{
	BayesianNetwork bn = BayesianNetwork(*this);
	bn.nodes.at(toIndex).parentNodes.push_back(fromIndex);
	return bn;
}

BayesianNetwork BayesianNetwork::withoutConnection(int fromIndex, int toIndex)
{
	BayesianNetwork bn = BayesianNetwork(*this);
	auto n = bn.nodes.at(toIndex).parentNodes;
	n.erase(find(n.begin(), n.end(), fromIndex));
	bn.nodes.at(toIndex).parentNodes = n;
	return bn;
}

bool BayesianNetwork::haveConnection(int fromIndex, int toIndex)
{
	return contains(nodes.at(toIndex).parentNodes, fromIndex);
}

long double BayesianNetwork::probabilityOf(int node, int valueParam, map<int, int> knowladge)
{
	return nodes.at(node).probabilityOf(valueParam, knowladge);
}

long double BayesianNetwork::probabilityOf(int node, int valueParam)
{
	return nodes.at(node).probabilityOf(valueParam, map<int, int> {});
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

long double BayesianNetwork::Node::probabilityOf(int valueParam, map<int, int> knowladge)
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
{
	this->params = params;
	this->parentNodes = parentNodes;
	this->colNum = pow(2, parentNodes.size()); 
	this->rowNum = params.size();
	this->probabilities = equallyDistributedRows(colNum, rowNum);
}

BayesianNetwork::Node::Node(const Node& n)
	: params(n.params), parentNodes(n.parentNodes), colNum(n.colNum), rowNum(n.rowNum), probabilities(n.probabilities)
{
}

BayesianNetwork::Node BayesianNetwork::Node::withParamsLearned(DataSet dataSet, int index)
{
	vector<int> allCounted = dataSet.countParams(index);
	int sumOfAll = sum(allCounted);
	for (int c = 0; c < colNum; c++) {
		for (int r = 0; r < rowNum; r++) {
			probabilities.at(r).at(c) = 1.0L * allCounted.at(r) / sumOfAll;
		}
	}
	return *this;
}

BayesianNetwork::Node::~Node()
{
}
