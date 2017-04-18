#include "stdafx.h"
#include <CppUnitTest.h>  
#include "DataSet.h"
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

	TEST_CLASS(Template_tests)
	{
	public:
		
		TEST_METHOD(Mapping_test)
		{
			Assert::AreEqual(vector<int> { 2, 3 }, mapTo<int, int>(vector<int>{1, 2}, [](int i) -> int { return i + 1; }));
			Assert::AreEqual(vector<int> { 2, 4, 6 }, mapTo<int, int>(vector<int>{1, 2, 3}, [](int i) -> int { return i * 2; }));
			Assert::AreEqual(vector<int> { 1, 4, 9 }, mapTo<int, int>(vector<int>{1, 2, 3}, [](int i) -> int { return i * i; }));
		}

		TEST_METHOD(Vector_mapping_test)
		{
			Assert::AreEqual(vector<vector<int>> { {3, 4}, { 5, 6 } }, mapTo<vector<int>, vector<int>>(vector<vector<int>>{ {2, 3}, { 4, 5 } }, [](vector<int> v) -> vector<int> {
				return mapTo<int, int>(v, [](int i) -> int {
					return i + 1;
				});
			}));
		}

		TEST_METHOD(Uniquify_test)
		{
			std::vector<int> v = { 1, 1, 2, 2 };
			v.erase(uniquify(v.begin(), v.end()), v.end());
			Assert::AreEqual(vector<int> { 1, 2 }, v);
		}

		TEST_METHOD(Custom_uniquify_test)
		{
			std::vector<int> v = { 1, 1, 2, 2 };
			v = uniquify(v);
			Assert::AreEqual(vector<int> { 1, 2 }, v);
		}

		TEST_METHOD(Any_tests)
		{
			Assert::IsTrue(any(vector<int> { 1, 2, 3}, [](int i) -> bool {
				return i == 3;
			}));
			Assert::IsFalse(any(vector<int> { 1, 2, 3}, [](int i) -> bool {
				return i == 4;
			}));
			Assert::IsTrue(any(vector<bool> { true, false, false }, [](bool b) -> bool {
				return b;
			}));
			Assert::IsTrue(any(vector<bool> { true, true }, [](bool b) -> bool {
				return b;
			}));
			Assert::IsTrue(any(vector<bool> { true }, [](bool b) -> bool {
				return b;
			}));
			Assert::IsFalse(any(vector<bool> {}, [](bool b) -> bool {
				return b;
			}));
		}
		TEST_METHOD(sumBy_tests)
		{
			Assert::AreEqual(0, sumBy(vector<int> { }, 0, [](int i) -> int { return i; }));
			Assert::AreEqual(1, sumBy(vector<int> { 1 }, 0, [](int i) -> int { return i; }));
			Assert::AreEqual(6, sumBy(vector<int> { 1, 2, 3 }, 0, [](int i) -> int { return i; }));
			Assert::AreEqual(1.0L, sumBy(vector<int> { 2, 4, 4 }, 0.0L, [](int i) -> long double { return 1.0L / i; }));
		}
	};
}