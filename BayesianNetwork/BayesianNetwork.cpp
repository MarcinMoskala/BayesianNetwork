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

vector<long double> filledVector(int size, long double value) {
	vector<long double> v = {};
	for (int j = 0; j < size; j++)
		v.push_back(value);
	return v;
}

BayesianNetwork::~BayesianNetwork()
{
}

BayesianNetwork::BayesianNetwork(DataSet dataSet)
{
	nodes = vector<Node>{};
	for (int i = 0; i < dataSet.columnsNum(); i++) {
		auto params = dataSet.paramsForColumn(i);
		Node node = Node(this, params);
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

BayesianNetwork::Node::Node(BayesianNetwork* network, vector<int> params)
	:BayesianNetwork::Node::Node(network, params, vector<int> {})
{
}

BayesianNetwork::Node::Node(BayesianNetwork* network, vector<int> params, vector<int> parentNodes)
{
	this->network = network;
	this->params = params;
	this->parentNodes = parentNodes;
	this->paramDistribution = filledVector(params.size(), 1.0L / params.size());
}

BayesianNetwork::Node::Node(const Node& n):
	network(n.network),
	params(n.params), 
	parentNodes(n.parentNodes), 
	paramDistribution(n.paramDistribution),
	conditionalProbability(n.conditionalProbability)
{
}

long double BayesianNetwork::Node::possibilityOf(vector<int> situation, map<int, int> knowladge) {
	long double p = 0.0L;
	for (int i = 0; i < situation.size(); i++) {
		int parentIndex = parentNodes.at(i);
		auto it = knowladge.find(parentIndex);
		if (it == knowladge.end()) {
			// No knowladge about this param
			p *= paramDistribution.at(parentIndex);
		}
		else if (it->second == situation.at(i)) {
			return 1.0L;
		}
		else {
			return 0.0L;
		}
	}
	return p;
}

long double BayesianNetwork::Node::probabilityOf(int valueParam, map<int, int> knowladge)
{
	int indexOfParam = indexOf(params, valueParam);
	if (indexOfParam < 0 || indexOfParam >= paramDistribution.size())
		return 0.0L;
	if (knowladge.empty()) {
		return paramDistribution.at(indexOfParam);
	}
	else 
	{
		long double finalP = 0.0L;
		for (auto const &prob : conditionalProbability) {
			auto situation = prob.first;
			auto paramProbabilities = prob.second;
			auto p = paramProbabilities.at(indexOfParam);
			finalP += p * possibilityOf(situation, knowladge);
		}
		return finalP;
	}
}

vector<vector<int>> getAllSituations(vector<vector<int>> parentParams) {
	if (parentParams.empty())
		return {};
	if (parentParams.size() == 1)
		return mapTo<int, vector<int>>(parentParams.at(0), [](int i) -> vector<int> {	return vector<int> { i }; });
	
	vector<int> last = parentParams.at(parentParams.size() - 1);
	auto init = vecInit(parentParams);
	auto initSituations = getAllSituations(init);
	return flatMap<vector<int>, vector<int>>(initSituations, [last](vector<int> v) -> vector<vector<int>> {
		return mapTo<int, vector<int>>(last, [v](int i) -> vector<int> {
			auto vec = vector<int>(v);
			vec.push_back(i);
			return vec;
		});
	});
}

map<int, int> situationAsKnowladge(vector<int> situation, vector<int> parentNodes) {
	map<int, int> k = map<int, int>();
	for (int i = 0; i < situation.size(); i++) {
		k.insert(make_pair(parentNodes.at(i), situation.at(i)));
	}
	return k;
}

vector<long double> countDistribution(DataSet dataSet, int index, int paramSize, map<int, int> knowladge) {
	vector<int> allCounted = dataSet.countParams(index, knowladge);
	int sumOfAll = sum(allCounted);

	vector<long double> paramDistribution = vector<long double>{};
	for (int r = 0; r < paramSize; r++) {
		paramDistribution.push_back(1.0L * allCounted.at(r) / sumOfAll);
	}
	return paramDistribution;
}

BayesianNetwork::Node BayesianNetwork::Node::withParamsLearned(DataSet dataSet, int index)
{
	paramDistribution = countDistribution(dataSet, index, params.size(), map<int, int> {});
	
	vector<Node> parents = mapTo<int, Node>(parentNodes, [this](int i) -> Node { return network->getNodes().at(i); });
	auto parentParams = mapTo<Node, vector<int>>(parents, [](Node n) -> vector<int> { return n.params; });
	auto situations = getAllSituations(parentParams);
	for (auto s : situations) {
		map<int, int> k = situationAsKnowladge(s, parentNodes);
		auto p = countDistribution(dataSet, index, params.size(), k);
		conditionalProbability.insert( make_pair(s, p) );
	}
	return *this;
}

BayesianNetwork::Node::~Node()
{
}
