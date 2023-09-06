#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502IncrementAndDecrementTest : public testing::Test
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

	void IncrementRegister(
		Byte OpcodeToTest, 
		Byte CPU::* RegisterToTest, 
		bool zero, 
		bool negative);

	void DecrementRegister(
		Byte OpcodeToTest, 
		Byte CPU::* RegisterToTest, 
		bool zero, 
		bool negative);

	void IncrementMemoryLocationZeroPage(
		Byte OpcodeToTest,
		Byte CPU::* OffsetRegister,
		bool zero,
		bool negative);

	void IncrementMemoryLocationAbsolute(
		Byte OpcodeToTest,
		Byte CPU::* OffsetRegister,
		bool zero,
		bool negative);

	void DecrementMemoryLocationZeroPage(
		Byte OpcodeToTest,
		Byte CPU::* OffsetRegister,
		bool zero,
		bool negative);

	void DecrementMemoryLocationAbsolute(
		Byte OpcodeToTest,
		Byte CPU::* OffsetRegister,
		bool zero,
		bool negative);

};

void M6502IncrementAndDecrementTest::IncrementMemoryLocationZeroPage(
	Byte OpcodeToTest,
	Byte CPU::* OffsetRegister,
	bool zero,
	bool negative)
{
	// Given:
	Byte offsetAddr = 0x0;
	s32 offsetCycles = 0;
	if (OffsetRegister != nullptr)
	{
		cpu.*OffsetRegister = 0x5;
		offsetAddr = 0x5;
		offsetCycles = 1;

	}
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;					// Zero Page Address
	mem[0x0080 + offsetAddr] = zero ? 0xFF : (negative ? 0x7F : 0x01);
	cpu.PS.Flags.Z = zero ? false : true;
	cpu.PS.Flags.N = negative ? false : true;
	const s32 EXPECTED_CYCLES = 5 + offsetCycles;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0080 + offsetAddr], zero ? 0x00 : (negative ? 0x80 : 0x02));
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);
}

void M6502IncrementAndDecrementTest::IncrementMemoryLocationAbsolute(
	Byte OpcodeToTest,
	Byte CPU::* OffsetRegister,
	bool zero,
	bool negative)
{
	// Given:
	Byte offsetAddr = 0x0;
	s32 offsetCycles = 0;
	if (OffsetRegister != nullptr)
	{
		cpu.*OffsetRegister = 0x5;
		offsetAddr = 0x5;
		offsetCycles = 1;
	}
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x01;					// Absolute Address : Low Byte
	mem[0xFFFE] = 0x80;					// Absolute Address : High Byte
	mem[0x8001 + offsetAddr] = zero ? 0xFF : (negative ? 0x7F : 0x01);
	cpu.PS.Flags.Z = zero ? false : true;
	cpu.PS.Flags.N = negative ? false : true;
	const s32 EXPECTED_CYCLES = 6 + offsetCycles;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8001 + offsetAddr], zero ? 0x00 : (negative ? 0x80 : 0x02));
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);
}

void M6502IncrementAndDecrementTest::DecrementMemoryLocationZeroPage(
	Byte OpcodeToTest,
	Byte CPU::* OffsetRegister,
	bool zero,
	bool negative)
{
	// Given:
	Byte offsetAddr = 0x0;
	s32 offsetCycles = 0;
	if (OffsetRegister != nullptr)
	{
		cpu.*OffsetRegister = 0x5;
		offsetAddr = 0x5;
		offsetCycles = 1;

	}
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;					// Zero Page Address
	mem[0x0080 + offsetAddr] = zero ? 0x01 : (negative ? 0x00 : 0x02);
	cpu.PS.Flags.Z = zero ? false : true;
	cpu.PS.Flags.N = negative ? false : true;
	const s32 EXPECTED_CYCLES = 5 + offsetCycles;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0080 + offsetAddr], zero ? 0x00 : (negative ? 0xFF : 0x01));
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);
}

void M6502IncrementAndDecrementTest::DecrementMemoryLocationAbsolute(
	Byte OpcodeToTest,
	Byte CPU::* OffsetRegister,
	bool zero,
	bool negative)
{
	// Given:
	Byte offsetAddr = 0x0;
	s32 offsetCycles = 0;
	if (OffsetRegister != nullptr)
	{
		cpu.*OffsetRegister = 0x5;
		offsetAddr = 0x5;
		offsetCycles = 1;
	}
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x01;					// Absolute Address : Low Byte
	mem[0xFFFE] = 0x80;					// Absolute Address : High Byte
	mem[0x8001 + offsetAddr] = zero ? 0x01 : (negative ? 0x00 : 0x02);
	cpu.PS.Flags.Z = zero ? false : true;
	cpu.PS.Flags.N = negative ? false : true;
	const s32 EXPECTED_CYCLES = 6 + offsetCycles;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8001 + offsetAddr], zero ? 0x00 : (negative ? 0xFF : 0x01));
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);
}

void M6502IncrementAndDecrementTest::IncrementRegister(
	Byte OpcodeToTest,
	Byte CPU::*RegisterToTest, 
	bool zero, 
	bool negative)
{
	// Given :
	cpu.*RegisterToTest = zero ? 0xFF : (negative ? 0x7F :0x01);
	cpu.PS.Flags.Z = zero ? false : true;
	cpu.PS.Flags.N = negative ? false : true;
	mem[0xFFFC] = OpcodeToTest;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.*RegisterToTest, zero ? 0x00 : (negative ? 0x80 : 0x02));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);
}

void M6502IncrementAndDecrementTest::DecrementRegister(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest,
	bool zero,
	bool negative)
{
	// Given :
	cpu.*RegisterToTest = zero ? 0x01 : (negative ? 0x00 : 0x02);
	cpu.PS.Flags.Z = zero ? false : true;
	cpu.PS.Flags.N = negative ? false : true;
	mem[0xFFFC] = OpcodeToTest;
	cpuCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.*RegisterToTest, zero ? 0x00 : (negative ? 0xFF : 0x01));
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);
}

TEST_F(M6502IncrementAndDecrementTest, INXCanIncrementTheValueInTheXRegister)
{
	IncrementRegister(CPU::INS_INX, &CPU::X, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, INXCanIncrementTheValueInTheXRegisterTurnItToZero)
{
	IncrementRegister(CPU::INS_INX, &CPU::X, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, INXCanIncrementTheValueInTheXRegisterTurnItToNegative)
{
	IncrementRegister(CPU::INS_INX, &CPU::X, false, true);
}

TEST_F(M6502IncrementAndDecrementTest, INYCanIncrementTheValueInTheYRegister)
{
	IncrementRegister(CPU::INS_INY, &CPU::Y, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, INYCanIncrementTheValueInTheYRegisterTurnItToZero)
{
	IncrementRegister(CPU::INS_INY, &CPU::Y, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, INYCanIncrementTheValueInTheYRegisterTurnItToNegative)
{
	IncrementRegister(CPU::INS_INY, &CPU::Y, false, true);
}

TEST_F(M6502IncrementAndDecrementTest, DEXCanDecrementTheValueInTheXRegister)
{
	DecrementRegister(CPU::INS_DEX, &CPU::X, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, DEXCanDecrementTheValueInTheXRegisterTurnItToZero)
{
	DecrementRegister(CPU::INS_DEX, &CPU::X, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, DEXCanDecrementTheValueInTheXRegisterTurnItToNegative)
{
	DecrementRegister(CPU::INS_DEX, &CPU::X, false, true);
}

TEST_F(M6502IncrementAndDecrementTest, DEYCanDecrementTheValueInTheYRegister)
{
	DecrementRegister(CPU::INS_DEY, &CPU::Y, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, DEYCanDecrementTheValueInTheYRegisterTurnItToZero)
{
	DecrementRegister(CPU::INS_DEY, &CPU::Y, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, DEYCanDecrementTheValueInTheYRegisterTurnItToNegative)
{
	DecrementRegister(CPU::INS_DEY, &CPU::Y, false, true);
}


TEST_F(M6502IncrementAndDecrementTest, INCAbsoluteCanIncrementTheValueInTheMemoryLocation)
{
	IncrementMemoryLocationAbsolute(CPU::INS_INC_ABS, nullptr, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, INCAbsoluteCanIncrementTheValueInTheMemoryLocationTurnItToZero)
{
	IncrementMemoryLocationAbsolute(CPU::INS_INC_ABS, nullptr, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, INCAbsoluteCanIncrementTheValueInTheMemoryLocationTurnItToNegative)
{
	IncrementMemoryLocationAbsolute(CPU::INS_INC_ABS, nullptr, false, true);
}

TEST_F(M6502IncrementAndDecrementTest, INCAbsoluteXCanIncrementTheValueInTheMemoryLocation)
{
	IncrementMemoryLocationAbsolute(CPU::INS_INC_ABSX, &CPU::X, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, INCZeroPageCanIncrementTheValueInTheMemoryLocation)
{
	IncrementMemoryLocationZeroPage(CPU::INS_INC_ZP, nullptr, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, INCZeroPageCanIncrementTheValueInTheMemoryLocationTurnItToZero)
{
	IncrementMemoryLocationZeroPage(CPU::INS_INC_ZP, nullptr, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, INCZeroPageCanIncrementTheValueInTheMemoryLocationTurnItToNegative)
{
	IncrementMemoryLocationZeroPage(CPU::INS_INC_ZP, nullptr, false, true);
}

TEST_F(M6502IncrementAndDecrementTest, INCZeroPageXCanIncrementTheValueInTheMemoryLocation)
{
	IncrementMemoryLocationZeroPage(CPU::INS_INC_ZPX, &CPU::X, false, false);
}


TEST_F(M6502IncrementAndDecrementTest, DECAbsoluteCanDecrementTheValueInTheMemoryLocation)
{
	DecrementMemoryLocationAbsolute(CPU::INS_DEC_ABS, nullptr, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, DECAbsoluteCanDecrementTheValueInTheMemoryLocationTurnItToZero)
{
	DecrementMemoryLocationAbsolute(CPU::INS_DEC_ABS, nullptr, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, DECAbsoluteCanDecrementTheValueInTheMemoryLocationTurnItToNegative)
{
	DecrementMemoryLocationAbsolute(CPU::INS_DEC_ABS, nullptr, false, true);
}

TEST_F(M6502IncrementAndDecrementTest, DECAbsoluteXCanDecrementTheValueInTheMemoryLocation)
{
	DecrementMemoryLocationAbsolute(CPU::INS_DEC_ABSX, &CPU::X, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, DECZeroPageCanDecrementTheValueInTheMemoryLocation)
{
	DecrementMemoryLocationZeroPage(CPU::INS_DEC_ZP, nullptr, false, false);
}

TEST_F(M6502IncrementAndDecrementTest, DECZeroPageCanDecrementTheValueInTheMemoryLocationTurnItToZero)
{
	DecrementMemoryLocationZeroPage(CPU::INS_DEC_ZP, nullptr, true, false);
}

TEST_F(M6502IncrementAndDecrementTest, DECZeroPageCanDecrementTheValueInTheMemoryLocationTurnItToNegative)
{
	DecrementMemoryLocationZeroPage(CPU::INS_DEC_ZP, nullptr, false, true);
}

TEST_F(M6502IncrementAndDecrementTest, DECZeroPageXCanDecrementTheValueInTheMemoryLocation)
{
	DecrementMemoryLocationZeroPage(CPU::INS_DEC_ZPX, &CPU::X, false, false);
}