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
			Assert::AreEqual(3, (int)bn.getNodes().size());
		}

		TEST_METHOD(When_there_is_only_one_row_then_probability_of_it_is_1_ant_for_other_values_it_is_0)
		{
			BayesianNetwork bn = BayesianNetwork(DataSet(vector<vector<int>>{
				vector<int>{ 1, 2, 3 }
			}));
			Assert::AreEqual(vector<long double>{ 1.0, 1.0, 1.0 }, bn.evaluate(vector<int>{ 1, 2, 3 }));
			Assert::AreEqual(vector<long double>{ 1.0, 0.0, 0.0 }, bn.evaluate(vector<int>{ 1, 3, 1 }));
			Assert::AreEqual(vector<long double>{ 0.0, 0.0, 0.0 }, bn.evaluate(vector<int>{ 2, 3, 1 }));
			Assert::AreEqual(vector<long double>{ 0.0, 0.0, 0.0 }, bn.evaluate(vector<int>{ 3, 1, 2 }));
		}

		TEST_METHOD(Equaly_distributed_elements_are_equaly_probable)
		{
			BayesianNetwork bn = BayesianNetwork(DataSet(vector<vector<int>>{
					vector<int>{ 1 },
					vector<int>{ 2 }
			}));
			//Assert::AreEqual(vector<long double>{ 0.5 }, bn.evaluate(vector<int>{ 1 }));
			Assert::AreEqual(vector<long double>{ 0.5 }, bn.evaluate(vector<int>{ 2 }));
		}
	};
}