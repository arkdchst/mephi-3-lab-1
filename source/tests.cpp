
#include "gtest/gtest.h"
#include "sort.hpp"

#include <utility>
#include <functional>

class TestSort :	public testing::TestWithParam	<std::tuple<
														std::pair<
															std::vector<int>,
															std::vector<int>
														>,
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

TEST_P(TestSort,){ //test sorting algorithms
	Sequence<int> *sorted = fun(sequence, defaultCompareFun);
	ASSERT_EQ(*sorted, *expected);

	delete sorted;
}

const std::pair<std::vector<int>,std::vector<int>> data_TestSort[] = {//format: {input},{expected output}
																{{1,2,3,4,4},{1,2,3,4,4}},
																{{4,4,3,2,1},{1,2,3,4,4}},
																{{3,4,2,1,4},{1,2,3,4,4}},
																{{2,1},{1,2}},
																{{1},{1}},
																{{},{}},
															};
const SortFun<int> *algos[] = {bubbleSort, shakerSort, quickSort};
INSTANTIATE_TEST_SUITE_P(, TestSort, testing::Combine(testing::ValuesIn(data_TestSort), testing::ValuesIn(algos)));



enum struct SequenceType {array, list};
class SequenceTest : public testing::TestWithParam<SequenceType>{
public:
	Sequence<int>* createSequence(int *data, int len){
		SequenceType type = GetParam();
		if(type == SequenceType::array)
			return new ArraySequence<int>(data, len);
		else if(type == SequenceType::list)
			return new ListSequence<int>(data, len);
		else return nullptr;
	}

};

INSTANTIATE_TEST_SUITE_P(, SequenceTest, testing::Values(SequenceType::array, SequenceType::list));

TEST_P(SequenceTest,get){
	int data[] = {1,2,3,4,5};
	Sequence<int> *seq = createSequence(data, 5);
	ASSERT_EQ(seq->get(0), 1);
	ASSERT_EQ(seq->get(2), 3);
	ASSERT_EQ(seq->get(4), 5);
	ASSERT_THROW(seq->get(-1), std::out_of_range);
	delete seq;
}

TEST_P(SequenceTest,getSize){
	int data[] = {1,2,3,4,5};
	Sequence<int> *seq1 = createSequence(data, 5);
	Sequence<int> *seq2 = createSequence(data, 1);
	Sequence<int> *seq3 = createSequence(data, 0);

	ASSERT_EQ(seq1->getSize(), 5);
	ASSERT_EQ(seq2->getSize(), 1);
	ASSERT_EQ(seq3->getSize(), 0);

	delete seq1, seq2, seq3;
}

TEST_P(SequenceTest,set){
	int data1[] = {1,2,3,4,5};
	int data2[] = {0,2,3,4,5};
	int data3[] = {0,2,0,4,5};
	int data4[] = {0,2,0,4,0};
	Sequence<int> *seq = createSequence(data1, 5);
	Sequence<int> *exp1 = createSequence(data2, 5);
	Sequence<int> *exp2 = createSequence(data3, 5);
	Sequence<int> *exp3 = createSequence(data4, 5);

	seq->set(0,0);
		ASSERT_EQ(*seq, *exp1);
	seq->set(0,2);
		ASSERT_EQ(*seq, *exp2);
	seq->set(0,4);
		ASSERT_EQ(*seq, *exp3);

	delete seq, exp1, exp2, exp3;
}

TEST_P(SequenceTest,clone){
	int data[] = {1,2,3,4,5};
	Sequence<int> *seq1 = createSequence(data, 5);
	Sequence<int> *exp1 = createSequence(data, 5);
	Sequence<int> *seq2 = createSequence(data, 1);
	Sequence<int> *exp2 = createSequence(data, 1);
	Sequence<int> *seq3 = createSequence(data, 0);
	Sequence<int> *exp3 = createSequence(data, 0);

	ASSERT_EQ(*seq1, *exp1);
	ASSERT_EQ(*seq2, *exp2);
	ASSERT_EQ(*seq3, *exp3);

	delete seq1, seq2, seq3, exp1, exp2, exp3;
}