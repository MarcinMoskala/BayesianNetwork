#include "stdafx.h"
#include <CppUnitTest.h>  
#include "DataSet.h"
#include "BayesianNetwork.h"
#include "Templates.cpp"
#include "TestHelpers.cpp"
#include <vector>
#include <cassert>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

#pragma once
namespace BN
{

	TEST_CLASS(BayesianNetwork_tests)
	{
	public:
		TEST_METHOD(BayesianNetwork_from_data_model_have_nodes_for_each_row)
		{
			BayesianNetwork bn = BayesianNetwork(DataSet(vector<vector<int>>{
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 }
			}));
			Assert::AreEqual(3, (int) bn.getNodes().size());
		}
	};
}