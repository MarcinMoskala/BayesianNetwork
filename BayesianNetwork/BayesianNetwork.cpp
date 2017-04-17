#include "stdafx.h"
#include "BayesianNetwork.h"


BayesianNetwork::BayesianNetwork()
{
}


BayesianNetwork::~BayesianNetwork()
{
}

BayesianNetwork::BayesianNetwork(DataSet dataSet)
{
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
