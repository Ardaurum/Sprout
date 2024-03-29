#include "CppUnitTest.h"
#include "Sprout/Core/Collections.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SproutTests
{
	TEST_CLASS(SparseArrayTests)
	{
	public:
		TEST_METHOD(Insert_OneElement_CollectionContainsIt)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 1);
			Assert::AreEqual(true, testArr.Has(0));
		}

		TEST_METHOD(Insert_OneElement_ValueEquals)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 4);
			Assert::AreEqual(4u, testArr[0]);
		}

		TEST_METHOD(Insert_WithoutValue_AddsElementWithDefaultValue)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(4);
			Assert::AreEqual(0u, testArr[4]);
		}

		TEST_METHOD(Clear_WithTwoElements_RemovesAllElements)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 1);
			testArr.Insert(5, 2);
			testArr.Clear();
			Assert::AreEqual(true, testArr.IsEmpty());
		}

		TEST_METHOD(Erase_WithTwoElement_DeletesOnlyOneElement)
		{
			Sprout::SparseArray<uint32_t> testArr;
			testArr.Insert(0, 1);
			testArr.Insert(4, 5);
			testArr.Erase(0);
			Assert::AreEqual(false, testArr.Has(0));
		}

		TEST_METHOD(Swap_TwoArrays_SwapsInternalData)
		{
			Sprout::SparseArray<uint32_t> testArrA;
			Sprout::SparseArray<uint32_t> testArrB;
			testArrA.Insert(2, 4);
			testArrB.Insert(5, 3);
			Swap(testArrA, testArrB);
			Assert::AreEqual(3u, testArrA[5]);
			Assert::AreEqual(4u, testArrB[2]);
		}

		TEST_METHOD(CopyOperator_FilledArray_FillsTheSecondArrayWithSameDataButDifferentMemory)
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

		TEST_METHOD(MoveOperator_FilledArray_FillsTheSecondArrayAndDestroysAllDataFromFirst)
		{
			Sprout::SparseArray<uint32_t> testArrA;
			Sprout::SparseArray<uint32_t> testArrB;
			testArrA.Insert(2, 4);
			testArrA.Insert(6, 3);
			testArrB = std::move(testArrA);
			Assert::AreEqual(0u, testArrA.GetCapacity());
			Assert::AreEqual(2u, testArrB.GetSize());
		}
	};

	TEST_CLASS(Freelist)
	{
		Sprout::Freelist testList;

		TEST_METHOD_CLEANUP(Cleanup)
		{
			testList.Clear();
		}

		TEST_METHOD(AllocateFirstSlot_ReturnsFirstIndex)
		{
			uint32_t slot = testList.AllocateSlot();
			Assert::AreEqual(0u, slot);
		}

		TEST_METHOD(AllocateSecondSlot_ReturnsSecondIndex)
		{
			uint32_t slot1 = testList.AllocateSlot();
			uint32_t slot2 = testList.AllocateSlot();
			Assert::AreEqual(1u, slot2);
		}

		TEST_METHOD(AllocateThreeSlots_ReturnsFirstIndex)
		{
			uint32_t slot = testList.AllocateSlots(3);
			Assert::AreEqual(0u, slot);
		}

		TEST_METHOD(AllocateThreeSlotsTwice_SecondSlotIsGreaterThanPreviousAllocatedSize)
		{
			uint32_t slot1 = testList.AllocateSlots(3);
			uint32_t slot2 = testList.AllocateSlots(3);
			Assert::IsTrue(slot2 >= 3u);
		}

		TEST_METHOD(AllocateSlotsFreeAndAllocateNewOne_ReusesFreedSlots)
		{
			uint32_t slot1 = testList.AllocateSlots(3);
			uint32_t slot2 = testList.AllocateSlots(3);
			testList.FreeSlot(slot1);
			uint32_t slot3 = testList.AllocateSlot();
			Assert::AreEqual(0u, slot3);
		}
	};	
}
