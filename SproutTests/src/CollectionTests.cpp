#include "CppUnitTest.h"
#include "Sprout/Core/Collections.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SproutTests
{
	TEST_CLASS(CollectionTests)
	{
	public:
		TEST_METHOD(SparseArray_Insert_OneElement_CollectionContainsIt)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 1);
			Assert::AreEqual(true, testArr.Has(0));
		}

		TEST_METHOD(SparseArray_Insert_OneElement_ValueEquals)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 4);
			Assert::AreEqual(4u, testArr[0]);
		}

		TEST_METHOD(SparseArray_Insert_WithoutValue_AddsElementWithDefaultValue)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(4);
			Assert::AreEqual(0u, testArr[4]);
		}

		TEST_METHOD(SparseArray_Clear_WithTwoElements_RemovesAllElements)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 1);
			testArr.Insert(5, 2);
			testArr.Clear();
			Assert::AreEqual(true, testArr.IsEmpty());
		}

		TEST_METHOD(SparseArray_Erase_WithTwoElement_DeletesOnlyOneElement)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 1);
			testArr.Insert(4, 5);
			testArr.Erase(0);
			Assert::AreEqual(false, testArr.Has(0));
		}

		TEST_METHOD(SparseArray_Swap_TwoArrays_SwapsInternalData)
		{
			Sprout::SparseArray<uint32_t> testArrA;
			Sprout::SparseArray<uint32_t> testArrB;
			testArrA.Insert(2, 4);
			testArrB.Insert(5, 3);
			Swap(testArrA, testArrB);
			Assert::AreEqual(3u, testArrA[5]);
			Assert::AreEqual(4u, testArrB[2]);
		}

		TEST_METHOD(SparseArray_CopyOperator_FilledArray_FillsTheSecondArrayWithSameDataButDifferentMemory)
		{
			Sprout::SparseArray<uint32_t> testArrA;
			Sprout::SparseArray<uint32_t> testArrB;
			testArrA.Insert(2, 4);
			testArrA.Insert(6, 3);
			testArrB = testArrA;
			Assert::AreEqual(2u, testArrA.GetSize());
			Assert::AreEqual(2u, testArrB.GetSize());
			Assert::AreNotEqual(&testArrA[2], &testArrB[2]);
		}

		TEST_METHOD(SparseArray_MoveOperator_FilledArray_FillsTheSecondArrayAndDestroysAllDataFromFirst)
		{
			Sprout::SparseArray<uint32_t> testArrA;
			Sprout::SparseArray<uint32_t> testArrB;
			testArrA.Insert(2, 4);
			testArrA.Insert(6, 3);
			testArrB = std::move(testArrA);
			Assert::AreEqual(0u, testArrA.GetCapacity());
			Assert::AreEqual(2u, testArrB.GetSize());
		}

		//TEST_METHOD(Freelist_WithName_ReturnsName)
	};	
}
