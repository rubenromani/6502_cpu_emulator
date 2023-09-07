#include "pch.h"
#include "6502LoadRegisterTest.h" 
#include "6502StoreRegisterTest.h"
#include "6502JumpsAndCallsTest.h" 
#include "6502StackOperationsTest.h" 
#include "6502RegisterTransferTest.h"
#include "6502LogicalOperationsTest.h"
#include "6502LoadProgramTest.h"
#include "6502IncrementAndDecrementTest.h"
#include "6502BranchesTest.h"
#include "6502StatusFlagsTest.h"
#include "6502ArithmeticOperationsTest.h"
#include "6502CompareTest.h"

GTEST_API_ int main(int argc, char** argv)
{
	printf("Running main()");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}