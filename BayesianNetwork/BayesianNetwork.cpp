#include "stdafx.h"
#include "BayesianNetwork.h"
#include "Templates.cpp"

#pragma once
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

BayesianNetwork::BayesianNetwork(DataSet<int> dataSet)
{
	nodes = vector<Node>{};
	for (int i = 0; i < dataSet.paramsNum(); i++) {
		auto params = dataSet.paramsForColumn(i);
		Node node = Node(this, params);
		nodes.push_back(node);
	}
}

BayesianNetwork BayesianNetwork::withParamsLearned(DataSet<int> dataSet)
{
	auto bn = BayesianNetwork(*this);
	bn.nodes = mapIndexed<Node, Node>(nodes, [dataSet](Node node, int index) -> Node {
		return node.withParamsLearned(dataSet, index);
	});
	return bn;
}

BayesianNetwork bestOnGreedyLearn(BayesianNetwork bn, DataSet<int> ds) {
	int nodesNum = bn.getNodes().size();
	BayesianNetwork bestBn = bn;
	long double bestBnResult = bn.withParamsLearned(ds).qualityFunction(ds);
	bool changed = false;
	for (int i = 0; i < nodesNum; i++) {
		for (int j = 0; j < nodesNum; j++) {
			if (i == j) {
				continue;
			}

			BayesianNetwork newBn = BayesianNetwork(bn);
			if (newBn.haveConnection(i, j)) {
				newBn = newBn.withoutConnection(i, j);
			}
			else {
				newBn = newBn.withConnection(i, j);
			}

			if (!newBn.isCorrect()) {
				continue;
			}

			auto r = newBn.withParamsLearned(ds).qualityFunction(ds);
			if (r > bestBnResult) {
				bestBnResult = r;
				bestBn = newBn;
				changed = true;
			}
		}
	}
	if (changed) {
		return bestOnGreedyLearn(bestBn, ds);
	}
	else {
		return bestBn;
	}
}

BayesianNetwork BayesianNetwork::withStructureLearned(DataSet<int> dataSet)
{
	// Brute force algorithm
	return bestOnGreedyLearn(BayesianNetwork(*this), dataSet);
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

bool parentsDoesNotContainsId(BayesianNetwork* net, BayesianNetwork::Node n, int id) {
	auto p = n.getParentNodes();
	if (p.empty()) {
		return true;
	}
	int i = indexOf(p, id);
	if (i >= 0 && i < p.size()) {
		return false;
	}

	auto parents = mapTo<int, BayesianNetwork::Node>(p, [net](int i) -> BayesianNetwork::Node { return net->getNodes().at(i); });
	return allTrue(parents, [net, id](BayesianNetwork::Node n) -> bool { return parentsDoesNotContainsId(net, n, id); });
}

bool BayesianNetwork::isCorrect()
{
	return allTrueIndexed(nodes, [this](Node n, int id) -> bool { return parentsDoesNotContainsId(this, n, id); });
}

long double BayesianNetwork::probabilityOf(int node, int valueParam, map<int, int> knowladge)
{
	if (knowladge.count(node))
		return knowladge[node] == valueParam;
	else
		return nodes.at(node).probabilityOf(valueParam, knowladge);
}

long double BayesianNetwork::probabilityOf(int node, int valueParam)
{
	return probabilityOf(node, valueParam, map<int, int> {});
}

vector<long double> BayesianNetwork::evaluate(vector<int> entry)
{
	return mapIndexed<int, long double>(entry, [this](int e, int index) -> long double {
		return this->probabilityOf(index, e);
	});
}

long double BayesianNetwork::qualityFunction(DataSet<int> dataSet)
{
	vector<vector<int>> dataPoints = dataSet.getDataPoints();
	long double r = 1.0L;
	for (const auto point : dataPoints) {
		auto knowlagdeAboutEverything = mapToMapIndexed<int, int, int>(point, [](int i, int index) -> pair<int, int> { return make_pair(index, i); });
		for (int i = 0; i < point.size(); i++) {
			auto knowladge = mapFilter(knowlagdeAboutEverything, [i](int nodeIndex, int value) -> bool { return nodeIndex != i; });
			r += probabilityOf(i, point.at(i), knowladge);
		}
	}
	int connections = sumBy(nodes, 1, [](Node n) -> int { return n.getParentNodeNum(); });
	return r/ dataSet.dataPointsNum() / dataSet.paramsNum() / pow(connections, 1.0 / 10);
}

// TODO Copy
vector<BayesianNetwork::Node> BayesianNetwork::getNodes() {
	return vector<Node>(nodes);
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

long double BayesianNetwork::Node::possibilityOf(vector<int> situation, map<int, int> knowladge, vector<Node> parents) {
	long double p = 1.0L;
	for (int i = 0; i < situation.size(); i++) {
		int parentIndex = parentNodes.at(i);
		auto it = knowladge.find(parentIndex);
		if (it == knowladge.end()) {
			// No knowladge about this param
			auto paramValue = situation.at(i);
			auto pos = network->probabilityOf(parentIndex, paramValue, knowladge);
			p *= pos;
		}
		else if (it->second == situation.at(i)) {
			p *= 1.0L;
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

	if (knowladge.empty() || conditionalProbability.empty()) {
		return paramDistribution.at(indexOfParam);
	}
	else 
	{
		long double finalP = 0.0L;
		vector<Node> parents = mapTo<int, Node>(parentNodes, [this](int i) -> Node { return network->getNodes().at(i); });
		for (auto const &prob : conditionalProbability) {
			auto situation = prob.first;
			auto paramProbabilities = prob.second;
			auto situationPossibility = possibilityOf(situation, knowladge, parents);
			auto p = paramProbabilities.at(indexOfParam);
			finalP += p * situationPossibility;
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

vector<long double> countDistribution(DataSet<int> dataSet, int index, int paramSize, map<int, int> knowladge) {
	vector<int> allCounted = dataSet.countParams(index, knowladge);
	int sumOfAll = sum(allCounted);

	vector<long double> paramDistribution = vector<long double>{};
	for (int r = 0; r < paramSize; r++) {
		paramDistribution.push_back(1.0L * allCounted.at(r) / sumOfAll);
	}
	return paramDistribution;
}

BayesianNetwork::Node BayesianNetwork::Node::withParamsLearned(DataSet<int> dataSet, int index)
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
