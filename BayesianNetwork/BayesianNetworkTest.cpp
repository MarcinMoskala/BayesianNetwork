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

		TEST_METHOD(Copying_constructor_test)
		{
			DataSet ds = DataSet(vector<vector<int>>{
					vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 },
					vector<int>{ 1, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			BayesianNetwork bn2 = bn.withParamsLearned(ds);
			
			Assert::AreNotSame(bn, bn2);
			
			for (auto const p : zip(bn.getNodes(), bn2.getNodes())) {
				Assert::AreNotSame(p.first, p.second);
			}
		}

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
			DataSet ds = DataSet(vector<vector<int>>{
					vector<int>{ 1, 2, 3 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withParamsLearned(ds);
			Assert::AreEqual(vector<long double>{ 1.0, 1.0, 1.0 }, bn.evaluate(vector<int>{ 1, 2, 3 }));
			Assert::AreEqual(vector<long double>{ 1.0, 0.0, 0.0 }, bn.evaluate(vector<int>{ 1, 3, 1 }));
			Assert::AreEqual(vector<long double>{ 0.0, 0.0, 0.0 }, bn.evaluate(vector<int>{ 2, 3, 1 }));
			Assert::AreEqual(vector<long double>{ 0.0, 0.0, 0.0 }, bn.evaluate(vector<int>{ 3, 1, 2 }));
		}

		TEST_METHOD(Equaly_distributed_elements_are_equaly_probable)
		{
			DataSet ds = DataSet(vector<vector<int>>{
					vector<int>{ 1 },
					vector<int>{ 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withParamsLearned(ds);
			Assert::AreEqual(vector<long double>{ 0.5 }, bn.evaluate(vector<int>{ 1 }));
			Assert::AreEqual(vector<long double>{ 0.5 }, bn.evaluate(vector<int>{ 2 }));
		}

		TEST_METHOD(Simple_probability_check_with_no_restrictions)
		{
			DataSet ds = DataSet(vector<vector<int>>{
					vector<int>{ 1 },
					vector<int>{ 1 },
					vector<int>{ 1 },
					vector<int>{ 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withParamsLearned(ds);
			Assert::AreEqual(vector<long double>{ 0.75 }, bn.evaluate(vector<int>{ 1 }));
			Assert::AreEqual(vector<long double>{ 0.25 }, bn.evaluate(vector<int>{ 2 }));
		}

		TEST_METHOD(Simple_connections_can_excist_and_be_deleted)
		{
			BayesianNetwork bn = BayesianNetwork(DataSet(vector<vector<int>>{
					vector<int>{ 1, 1 },
					vector<int>{ 1, 1 },
					vector<int>{ 1, 2 },
					vector<int>{ 2, 2 }
			}));
			bn = bn.withConnection(1, 0);
			Assert::IsTrue(bn.haveConnection(1, 0));
			bn = bn.withoutConnection(1, 0);
			Assert::IsFalse(bn.haveConnection(1, 0));
		}

		TEST_METHOD(Simple_probability_check_with_restrictions_and_full_knowladge)
		{
			DataSet ds = DataSet(vector<vector<int>>{
					vector<int>{ 1, 1 },
					vector<int>{ 1, 1 },
					vector<int>{ 1, 2 },
					vector<int>{ 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withConnection(1, 0);
			bn = bn.withParamsLearned(ds);
			Assert::AreEqual(1.0L, bn.probabilityOf(0, 1, map<int, int> { make_pair(1, 1) }));
			Assert::AreEqual(0.0L, bn.probabilityOf(0, 2, map<int, int> { make_pair(1, 1) }));
			Assert::AreEqual(0.5L, bn.probabilityOf(0, 1, map<int, int> { make_pair(1, 2) }));
			Assert::AreEqual(0.5L, bn.probabilityOf(0, 2, map<int, int> { make_pair(1, 2) }));
		}

		TEST_METHOD(Simple_probability_check_with_restrictions_and_partial_knowladge)
		{
			DataSet ds = DataSet(vector<vector<int>>{
					vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 1, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withConnection(1, 0);
			bn = bn.withConnection(2, 0);
			bn = bn.withParamsLearned(ds);
			Assert::AreEqual(0.5L, bn.probabilityOf(2, 1, map<int, int> { make_pair(1, 1) }));
			Assert::AreEqual(0.5L, bn.probabilityOf(2, 2, map<int, int> { make_pair(1, 1) }));

			Assert::AreEqual(1.0L, bn.probabilityOf(1, 1, map<int, int> { make_pair(1, 1) }));
			Assert::AreEqual(0.0L, bn.probabilityOf(1, 2, map<int, int> { make_pair(1, 1) }));

			Assert::AreEqual(1.0L, bn.probabilityOf(0, 1, map<int, int> { make_pair(1, 1) }));
			Assert::AreEqual(0.0L, bn.probabilityOf(0, 2, map<int, int> { make_pair(1, 1) }));
			Assert::AreEqual(0.5L, bn.probabilityOf(0, 1, map<int, int> { make_pair(1, 2) }));
			Assert::AreEqual(0.5L, bn.probabilityOf(0, 2, map<int, int> { make_pair(1, 2) }));
		}

		TEST_METHOD(Transitive_probability_check_with_restrictions_and_partial_knowladge)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 },
					vector<int>{ 1, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withConnection(1, 0);
			bn = bn.withConnection(2, 1);
			bn = bn.withParamsLearned(ds);

			Assert::AreEqual(33.0 / 54, bn.probabilityOf(0, 1, map<int, int> { make_pair(2, 2) }), 0.01);
		}

		TEST_METHOD(Evaluation_function_simple_test)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 },
					vector<int>{ 1, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withConnection(1, 0);
			bn = bn.withConnection(2, 1);
			bn = bn.withParamsLearned(ds);

			Assert::AreEqual(33.0 / 54, bn.probabilityOf(0, 1, map<int, int> { make_pair(2, 2) }), 0.01);
		}
	};

	TEST_CLASS(BayesianNetwork_quality_function_tests)
	{
	public:

		TEST_METHOD(Quality_function_is_returning_lesstthen0_value)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 1, 2 },
					vector<int>{ 1, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			bn = bn.withParamsLearned(ds);
			Assert::IsTrue(bn.qualityFunction(ds) < 1.0L);

			bn = bn.withConnection(1, 0);
			bn = bn.withConnection(2, 1);
			bn = bn.withParamsLearned(ds);
			Assert::IsTrue(bn.qualityFunction(ds) < 1.0L);
		}

		TEST_METHOD(Quality_function_is_bigger_when_connection_is_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
					vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			auto prevQF = bn
				.withParamsLearned(ds)
				.qualityFunction(ds);

			auto newQF = bn.withConnection(1, 0)
				.withParamsLearned(ds)
				.qualityFunction(ds);
			Assert::IsTrue(newQF > prevQF);

			newQF = bn.withConnection(0, 1)
				.withParamsLearned(ds)
				.qualityFunction(ds);
			Assert::IsTrue(newQF > prevQF);
		}

		TEST_METHOD(Quality_function_is_worse_when_connection_is_useless)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds);
			auto prevQF = bn
				.withParamsLearned(ds)
				.qualityFunction(ds);

			auto newQF = bn.withConnection(2, 1)
				.withParamsLearned(ds)
				.qualityFunction(ds);
			Assert::IsTrue(newQF < prevQF);

			newQF = bn.withConnection(2, 1)
				.withParamsLearned(ds)
				.qualityFunction(ds);

			Assert::IsTrue(newQF < prevQF);
		}
	};


	TEST_CLASS(BayesianNetwork_structure_is_correct_tests)
	{
	public:
		TEST_METHOD(Single_connection_is_always_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			Assert::IsTrue(BayesianNetwork(ds).withConnection(1, 2).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(0, 2).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(1, 0).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(2, 0).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(2, 1).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(0, 1).isCorrect());
		}

		TEST_METHOD(V_connections_are_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			Assert::IsTrue(BayesianNetwork(ds).withConnection(1, 2).withConnection(0, 2).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(2, 1).withConnection(0, 1).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(1, 0).withConnection(2, 0).isCorrect());
		}

		TEST_METHOD(A_connections_are_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			Assert::IsTrue(BayesianNetwork(ds).withConnection(2, 1).withConnection(2, 0).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(1, 0).withConnection(1, 2).isCorrect());
			Assert::IsTrue(BayesianNetwork(ds).withConnection(0, 1).withConnection(0, 2).isCorrect());
		}

		TEST_METHOD(Self_connections_are_not_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			Assert::IsFalse(BayesianNetwork(ds).withConnection(0, 0).isCorrect());
			Assert::IsFalse(BayesianNetwork(ds).withConnection(1, 1).isCorrect());
			Assert::IsFalse(BayesianNetwork(ds).withConnection(2, 2).isCorrect());
		}

		TEST_METHOD(Loops_are_not_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			Assert::IsFalse(BayesianNetwork(ds).withConnection(0, 1).withConnection(1, 0).isCorrect());
			Assert::IsFalse(BayesianNetwork(ds).withConnection(1, 2).withConnection(2, 1).isCorrect());
			Assert::IsFalse(BayesianNetwork(ds).withConnection(0, 2).withConnection(2, 0).isCorrect());
		}

		TEST_METHOD(Long_loops_are_not_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			Assert::IsFalse(BayesianNetwork(ds).withConnection(0, 1).withConnection(1, 2).withConnection(2, 0).isCorrect());
			Assert::IsFalse(BayesianNetwork(ds).withConnection(1, 0).withConnection(0, 2).withConnection(2, 1).isCorrect());
		}
	};


	TEST_CLASS(BayesianNetwork_learninig_tests)
	{
	public:
		TEST_METHOD(Simple_function_learning_is_correct)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1 },
					vector<int>{ 1, 1, 2 },
					vector<int>{ 2, 2, 1 },
					vector<int>{ 2, 2, 2 }
			});
			BayesianNetwork bn = BayesianNetwork(ds)
				.withStructureLearned(ds);

			Assert::IsTrue(bn.haveConnection(0, 1) || bn.haveConnection(1, 0));
			Assert::IsFalse(
				bn.haveConnection(1, 2) || bn.haveConnection(0, 2) ||
				bn.haveConnection(2, 1) || bn.haveConnection(2, 0)
			);
		}

		TEST_METHOD(Complex_function_learning_is_working)
		{
			DataSet ds = DataSet(vector<vector<int>>{
				vector<int>{ 1, 1, 1, 4 },
				vector<int>{ 1, 1, 2, 2 },
				vector<int>{ 2, 2, 1, 4 },
				vector<int>{ 2, 2, 2, 4 },
				vector<int>{ 1, 2, 2, 4 },
				vector<int>{ 1, 1, 2, 4 },
				vector<int>{ 2, 2, 1, 4 },
				vector<int>{ 1, 2, 2, 2 },
				vector<int>{ 1, 1, 1, 2 },
				vector<int>{ 2, 1, 2, 4 },
			});
			BayesianNetwork bn = BayesianNetwork(ds)
				.withStructureLearned(ds);
		}
	};
};