
#include "gtest/gtest.h"
#include "sort.hpp"

#include <utility>
#include <functional>

class SortTest : public testing::Test, public testing::WithParamInterface	<std::tuple<
																				std::pair<std::vector<int>,std::vector<int>>,
																				SortFun<int>*
																			>>
{
public:
	Sequence<int> *sequence;
	Sequence<int> *expected;

	SortFun<int> *fun;


	virtual void SetUp(){
		sequence = new ArraySequence<int>(&(std::get<0>(GetParam())).first[0], std::get<0>(GetParam()).first.size());
		expected = new ArraySequence<int>(&(std::get<0>(GetParam())).second[0], std::get<0>(GetParam()).second.size());

		fun = std::get<1>(GetParam());
	}

	virtual void TearDown(){
		delete sequence;
		delete expected;
	}

};

TEST_P(SortTest,){ //test sorting algorithms
	Sequence<int> *sorted = fun(sequence, defaultCompareFun);
	ASSERT_TRUE(*sorted == *expected);

	delete sorted;
}

const std::pair<std::vector<int>,std::vector<int>> data[] = {	
																{{1,2,3,4,4},{1,2,3,4,4}},
																{{4,4,3,2,1},{1,2,3,4,4}},
																{{3,4,2,1,4},{1,2,3,4,4}},
																{{2,1},{1,2}},
																{{1},{1}},
																{{},{}},
															};
const SortFun<int> *algos[] = {bubbleSort, shakerSort, quickSort};
INSTANTIATE_TEST_SUITE_P(, SortTest, testing::Combine(testing::ValuesIn(data), testing::ValuesIn(algos)));
