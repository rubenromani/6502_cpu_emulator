#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

enum class ELogicalOp
{
	AND, XOR, OR
};

class M6502LogicalOperationsTest : public testing::Test
{
public:
	Mem mem;
	CPU cpu;
	CPU cpuCopy;

	virtual void SetUp()
	{
		cpu.Reset(mem);
	}

	virtual void TearDown()
	{

	}

	void CpuMakeCopy()
	{
		cpuCopy = cpu;
	}

	void TestLogicalOpOnARegisterImmediate(
		ELogicalOp op, 
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterZeroPage(
		ELogicalOp op, 
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterZeroPageOffset(
		ELogicalOp op, 
		Byte CPU::* OffsetRegister, 
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterAbsolute(
		ELogicalOp op, 
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterAbsoluteOffset(
		ELogicalOp op, 
		Byte CPU::* OffsetRegister, 
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterAbsoluteOffsetPageCross(
		ELogicalOp op, 
		Byte CPU::* OffsetRegister, 
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterIndirectX(
		ELogicalOp op,
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterIndirectY(
		ELogicalOp op,
		Byte OpcodeToTest);

	void TestLogicalOpOnARegisterIndirectYPageCross(
		ELogicalOp op,
		Byte OpcodeToTest);

	void SwitchOp(ELogicalOp op, Word Address, Byte& Result);

};

void M6502LogicalOperationsTest::SwitchOp(ELogicalOp op, Word Address, Byte& Result)
{
	switch (op)
	{
	case ELogicalOp::AND:
		mem[Address] =	0b11110000;
		cpu.A =			0b01010101;
		Result =		0b01010000;
		break;
	case ELogicalOp::XOR:
		mem[Address] =	0b01110000;
		cpu.A =			0b01010101;
		Result =		0b00100101;
		break;
	case ELogicalOp::OR:
		mem[Address] =	0b01110000;
		cpu.A =			0b01010101;
		Result =		0b01110101;
		break;
	default:
		break;
	}
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterImmediate(
	ELogicalOp op,
	Byte OpcodeToTest)
{
	// Given :
	Byte Result;
	cpu.PS.Reg = 0x00;
	mem[0xFFFC] = OpcodeToTest;
	SwitchOp(op, 0xFFFD, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterZeroPage(
	ELogicalOp op,
	Byte OpcodeToTest)
{
	// Given : 
	Byte Result;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x42;
	SwitchOp(op, 0x0042, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
	
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterZeroPageOffset(
	ELogicalOp op,
	Byte CPU::* OffsetRegister,
	Byte OpcodeToTest)
{
	// Given :
	Byte Result;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x42;
	cpu.*OffsetRegister = 5;
	SwitchOp(op, 0x0047, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterAbsolute(
	ELogicalOp op,
	Byte OpcodeToTest)
{
	// Given :
	Byte Result;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	SwitchOp(op, 0x4480, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterAbsoluteOffset(
	ELogicalOp op,
	Byte CPU::* OffsetRegister,
	Byte OpcodeToTest)
{
	// Given :
	Byte Result;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	cpu.*OffsetRegister = 5;
	SwitchOp(op, 0x4485, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterAbsoluteOffsetPageCross(
	ELogicalOp op,
	Byte CPU::* OffsetRegister,
	Byte OpcodeToTest)
{
	// Given :
	Byte Result;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x02;
	mem[0xFFFE] = 0x44;	//0x4402
	cpu.*OffsetRegister = 0xFF;
	SwitchOp(op, 0x4501, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterIndirectX(
	ELogicalOp op,
	Byte OpcodeToTest)
{
	// Given :
	Byte Result;
	cpu.X = 0x04;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x02; //0x02 + 0x04 = 0x06
	mem[0x0006] = 0x00;	
	mem[0x0007] = 0x80;
	SwitchOp(op, 0x8000, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 6;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterIndirectY(
	ELogicalOp op,
	Byte OpcodeToTest)
{
	// Given :
	Byte Result;
	cpu.Y = 0x04;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x02;
	mem[0x0002] = 0x00;
	mem[0x0003] = 0x80; // 0x8000 + 0x0004 = 0x8004
	SwitchOp(op, 0x8004, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 5;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

void M6502LogicalOperationsTest::TestLogicalOpOnARegisterIndirectYPageCross(
	ELogicalOp op,
	Byte OpcodeToTest) 
{
	// Given :
	Byte Result;
	cpu.Y = 0xFF;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x02;
	mem[0x0002] = 0x02;
	mem[0x0003] = 0x80; // 0x8002 + 0x00FF = 0x8101
	SwitchOp(op, 0x8101, Result);
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 6;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.A, Result);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}

TEST_F(M6502LogicalOperationsTest, ANDImmediateCanPerformTheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterImmediate(ELogicalOp::AND, CPU::INS_AND_IM);
}

TEST_F(M6502LogicalOperationsTest, XORImmediateCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterImmediate(ELogicalOp::XOR, CPU::INS_XOR_IM);
}

TEST_F(M6502LogicalOperationsTest, ORImmediateCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterImmediate(ELogicalOp::OR, CPU::INS_OR_IM);
}


TEST_F(M6502LogicalOperationsTest, ANDZeroPageCanPerformTheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterZeroPage(ELogicalOp::AND, CPU::INS_AND_ZP);
}

TEST_F(M6502LogicalOperationsTest, XORZeroPageCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterZeroPage(ELogicalOp::XOR, CPU::INS_XOR_ZP);
}

TEST_F(M6502LogicalOperationsTest, ORZeroPageCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterZeroPage(ELogicalOp::OR, CPU::INS_OR_ZP);
}


TEST_F(M6502LogicalOperationsTest, ANDZeroPageXCanPerformTheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterZeroPageOffset(ELogicalOp::AND, &CPU::X, CPU::INS_AND_ZPX);
}

TEST_F(M6502LogicalOperationsTest, XORZeroPageXCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterZeroPageOffset(ELogicalOp::XOR, &CPU::X, CPU::INS_XOR_ZPX);
}

TEST_F(M6502LogicalOperationsTest, ORZeroPageXCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterZeroPageOffset(ELogicalOp::OR, &CPU::X, CPU::INS_OR_ZPX);
}


TEST_F(M6502LogicalOperationsTest, ANDAbsoluteCanPerformtheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsolute(ELogicalOp::AND, CPU::INS_AND_ABS);
}

TEST_F(M6502LogicalOperationsTest, XORAbsoluteCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsolute(ELogicalOp::XOR, CPU::INS_XOR_ABS);
}

TEST_F(M6502LogicalOperationsTest, ORAbsoluteCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsolute(ELogicalOp::OR, CPU::INS_OR_ABS);
}


TEST_F(M6502LogicalOperationsTest, ANDAbsoluteXCanPerformTheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsoluteOffset(ELogicalOp::AND, &CPU::X, CPU::INS_AND_ABSX);
}

TEST_F(M6502LogicalOperationsTest, XORAbsoluteXCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsoluteOffset(ELogicalOp::XOR, &CPU::X, CPU::INS_XOR_ABSX);
}

TEST_F(M6502LogicalOperationsTest, ORAbsoluteXCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsoluteOffset(ELogicalOp::OR, &CPU::X, CPU::INS_OR_ABSX);
}


TEST_F(M6502LogicalOperationsTest, ANDAbsoluteXCanPerformTheAndOperationOnTheARegisterWhenItCrossesThePageBoundary)
{
	TestLogicalOpOnARegisterAbsoluteOffsetPageCross(ELogicalOp::AND, &CPU::X, CPU::INS_AND_ABSX);
}

TEST_F(M6502LogicalOperationsTest, XORAbsoluteXCanPerformTheXorOperationOnTheARegisterWhenItCrossesThePageBoundary)
{
	TestLogicalOpOnARegisterAbsoluteOffsetPageCross(ELogicalOp::XOR, &CPU::X, CPU::INS_XOR_ABSX);
}

TEST_F(M6502LogicalOperationsTest, ORAbsoluteXCanPerformTheOrOperationOnTheARegisterWhenItCrossesThePageBoundary)
{
	TestLogicalOpOnARegisterAbsoluteOffsetPageCross(ELogicalOp::OR, &CPU::X, CPU::INS_OR_ABSX);
}


TEST_F(M6502LogicalOperationsTest, ANDAbsoluteYCanPerformTheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsoluteOffset(ELogicalOp::AND, &CPU::Y, CPU::INS_AND_ABSY);
}

TEST_F(M6502LogicalOperationsTest, XORAbsoluteYCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsoluteOffset(ELogicalOp::XOR, &CPU::Y, CPU::INS_XOR_ABSY);
}

TEST_F(M6502LogicalOperationsTest, ORAbsoluteYCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterAbsoluteOffset(ELogicalOp::OR, &CPU::Y, CPU::INS_OR_ABSY);
}


TEST_F(M6502LogicalOperationsTest, ANDAbsoluteYCanPerformTheAndOperationOnTheARegisterWhenItCrossesThePageBoundary)
{
	TestLogicalOpOnARegisterAbsoluteOffsetPageCross(ELogicalOp::AND, &CPU::Y, CPU::INS_AND_ABSY);
}

TEST_F(M6502LogicalOperationsTest, XORAbsoluteYCanPerformTheXorOperationOnTheARegisterWhenItCrossesThePageBoundary)
{
	TestLogicalOpOnARegisterAbsoluteOffsetPageCross(ELogicalOp::XOR, &CPU::Y, CPU::INS_XOR_ABSY);
}

TEST_F(M6502LogicalOperationsTest, ORAbsoluteYCanPerformTheOrOperationOnTheARegisterWhenItCrossesThePageBoundary)
{
	TestLogicalOpOnARegisterAbsoluteOffsetPageCross(ELogicalOp::OR, &CPU::Y, CPU::INS_OR_ABSY);
}


TEST_F(M6502LogicalOperationsTest, ANDIndirectXCanPerformTheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterIndirectX(ELogicalOp::AND, CPU::INS_AND_INDX);
}

TEST_F(M6502LogicalOperationsTest, XORIndirectXCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterIndirectX(ELogicalOp::XOR, CPU::INS_XOR_INDX);
}

TEST_F(M6502LogicalOperationsTest, ORIndirectXCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterIndirectX(ELogicalOp::OR, CPU::INS_OR_INDX);
}


TEST_F(M6502LogicalOperationsTest, ANDIndirectYCanPerformTheAndOperationOnTheARegister)
{
	TestLogicalOpOnARegisterIndirectY(ELogicalOp::AND, CPU::INS_AND_INDY);
}

TEST_F(M6502LogicalOperationsTest, XORIndirectYCanPerformTheXorOperationOnTheARegister)
{
	TestLogicalOpOnARegisterIndirectY(ELogicalOp::XOR, CPU::INS_XOR_INDY);
}

TEST_F(M6502LogicalOperationsTest, ORIndirectYCanPerformTheOrOperationOnTheARegister)
{
	TestLogicalOpOnARegisterIndirectY(ELogicalOp::OR, CPU::INS_OR_INDY);
}


TEST_F(M6502LogicalOperationsTest, ANDIndirectYCanPerformTheAndOperationOnTheARegisterWhenItCrossesThePage)
{
	TestLogicalOpOnARegisterIndirectYPageCross(ELogicalOp::AND, CPU::INS_AND_INDY);
}

TEST_F(M6502LogicalOperationsTest, XORIndirectYCanPerformTheXorOperationOnTheARegisterWhenItCrossesThePage)
{
	TestLogicalOpOnARegisterIndirectYPageCross(ELogicalOp::XOR, CPU::INS_XOR_INDY);
}

TEST_F(M6502LogicalOperationsTest, ORIndirectYCanPerformTheOrOperationOnTheARegisterWhenItCrossesThePage)
{
	TestLogicalOpOnARegisterIndirectYPageCross(ELogicalOp::OR, CPU::INS_OR_INDY);
}


TEST_F(M6502LogicalOperationsTest, BitZeroPageCanSetTheZeroFlag)
{
	// Given : 
	cpu.A = 0b11001100;
	cpu.PS.Flags.Z = false;
	mem[0xFFFC] = CPU::INS_BIT_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x00;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	Byte InibitZeroFlagMask = 0b11111101;
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg & InibitZeroFlagMask, cpuCopy.PS.Reg);
	EXPECT_TRUE(cpu.PS.Flags.Z);
}

TEST_F(M6502LogicalOperationsTest, BitZeroPageCanSetTheNegativeFlag)
{
	// Given : 
	cpu.A = 0b10000001;
	cpu.PS.Flags.N = false;
	mem[0xFFFC] = CPU::INS_BIT_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0b10000000;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	Byte InibitNegativeFlagMask = 0b01111111;
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg & InibitNegativeFlagMask, cpuCopy.PS.Reg);
	EXPECT_TRUE(cpu.PS.Flags.N);
}

TEST_F(M6502LogicalOperationsTest, BitZeroPageCanSetTheOverflowFlag)
{
	// Given : 
	cpu.A = 0b01000001;
	cpu.PS.Flags.V = false;
	mem[0xFFFC] = CPU::INS_BIT_ZP;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0b01000000;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	Byte InibitOverflowFlagMask = 0b10111111;
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg & InibitOverflowFlagMask, cpuCopy.PS.Reg);
	EXPECT_TRUE(cpu.PS.Flags.V);
}


TEST_F(M6502LogicalOperationsTest, BitAbsoluteCanSetTheZeroFlag)
{
	// Given : 
	cpu.A = 0b11001100;
	cpu.PS.Flags.Z = false;
	mem[0xFFFC] = CPU::INS_BIT_ABS;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x80;
	mem[0x8042] = 0x00;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	Byte InibitZeroFlagMask = 0b11111101;
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg & InibitZeroFlagMask, cpuCopy.PS.Reg);
	EXPECT_TRUE(cpu.PS.Flags.Z);
}

TEST_F(M6502LogicalOperationsTest, BitAbsoluteCanSetTheNegativeFlag)
{
	// Given : 
	cpu.A = 0b10000001;
	cpu.PS.Flags.N = false;
	mem[0xFFFC] = CPU::INS_BIT_ABS;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x80;
	mem[0x8042] = 0b10000000;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	Byte InibitNegativeFlagMask = 0b01111111;
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg & InibitNegativeFlagMask, cpuCopy.PS.Reg);
	EXPECT_TRUE(cpu.PS.Flags.N);
}

TEST_F(M6502LogicalOperationsTest, BitabsoluteCanSetTheOverflowFlag)
{
	// Given : 
	cpu.A = 0b01000001;
	cpu.PS.Flags.V = false;
	mem[0xFFFC] = CPU::INS_BIT_ABS;
	mem[0xFFFD] = 0x42;
	mem[0xFFFE] = 0x80;
	mem[0x8042] = 0b01000000;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	Byte InibitOverflowFlagMask = 0b10111111;
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg & InibitOverflowFlagMask, cpuCopy.PS.Reg);
	EXPECT_TRUE(cpu.PS.Flags.V);
}
