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
namespace BayesianNetwork
{

	TEST_CLASS(BayesianNetwork_tests)
	{
	public:
		TEST_METHOD(Failing)
		{
			Assert::AreEqual(1, 1);
		}
	};
}