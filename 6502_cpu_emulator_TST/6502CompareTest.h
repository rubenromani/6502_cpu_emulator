#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502CompareTest : public testing::Test
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

	void M6502CompareTest::CompareRegisterImmediateZeroAndCarry(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

	void M6502CompareTest::CompareRegisterImmediateCarry(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

	void M6502CompareTest::CompareRegisterImmediateNegative(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

	void M6502CompareTest::CompareRegisterZeroPageZeroAndCarry(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

	void M6502CompareTest::CompareRegisterAbsoluteZeroAndCarry(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

};


void M6502CompareTest::CompareRegisterImmediateZeroAndCarry(
	Byte OpcodeToTest, 
	Byte CPU::*RegisterToTest)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.*RegisterToTest = 0x01;
	mem[0xFF00] = OpcodeToTest;
	mem[0xFF01] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

void M6502CompareTest::CompareRegisterImmediateCarry(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.*RegisterToTest = 0x02;
	mem[0xFF00] = OpcodeToTest;
	mem[0xFF01] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

void M6502CompareTest::CompareRegisterImmediateNegative(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.*RegisterToTest = 0x01;
	mem[0xFF00] = OpcodeToTest;
	mem[0xFF01] = 0x02;
	cpu.PS.Flags.N = false;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = true;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_TRUE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

void M6502CompareTest::CompareRegisterZeroPageZeroAndCarry(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.*RegisterToTest = 0x01;
	mem[0xFF00] = OpcodeToTest;
	mem[0xFF01] = 0x80;
	mem[0x0080] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 3;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502CompareTest, CMP_ZPXCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0x05;
	mem[0xFF00] = CPU::INS_CMP_ZPX;
	mem[0xFF01] = 0x80;
	mem[0x0085] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

void M6502CompareTest::CompareRegisterAbsoluteZeroAndCarry(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.*RegisterToTest = 0x01;
	mem[0xFF00] = OpcodeToTest;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8000] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502CompareTest, CMP_ABSXCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0x05;
	mem[0xFF00] = CPU::INS_CMP_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8005] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502CompareTest, CMP_ABSXCrossPageCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0xFF;
	mem[0xFF00] = CPU::INS_CMP_ABSX;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	mem[0x8101] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 5;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}


TEST_F(M6502CompareTest, CMP_ABSYCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0x05;
	mem[0xFF00] = CPU::INS_CMP_ABSY;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[0x8005] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502CompareTest, CMP_ABSYCrossPageCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0xFF;
	mem[0xFF00] = CPU::INS_CMP_ABSY;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	mem[0x8101] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 5;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502CompareTest, CMP_INXCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0x05;
	mem[0xFF00] = CPU::INS_CMP_INX;
	mem[0xFF01] = 0x80;
	mem[0x0085] = 0x00;
	mem[0x0086] = 0x80;
	mem[0x8000] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502CompareTest, CMP_INYCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0x05;
	mem[0xFF00] = CPU::INS_CMP_INY;
	mem[0xFF01] = 0x80;
	mem[0x0080] = 0x00;
	mem[0x0081] = 0x80;
	mem[0x8005] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 5;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502CompareTest, CMP_INYCrossPageCanCompareSettingZeroAndCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0xFF;
	mem[0xFF00] = CPU::INS_CMP_INY;
	mem[0xFF01] = 0x80;
	mem[0x0080] = 0x02;
	mem[0x0081] = 0x80;
	mem[0x8101] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}


TEST_F(M6502CompareTest, CMP_IMCanSetZeroAndCarryFlag)
{
	CompareRegisterImmediateZeroAndCarry(CPU::INS_CMP_IM, &CPU::A);
}

TEST_F(M6502CompareTest, CMP_IMCanSetCarryFlag)
{
	CompareRegisterImmediateCarry(CPU::INS_CMP_IM,  &CPU::A);
}

TEST_F(M6502CompareTest, CMP_IMCanSetNegativeFlag)
{
	CompareRegisterImmediateNegative(CPU::INS_CMP_IM,  &CPU::A);
}

TEST_F(M6502CompareTest, CMP_ZPCanCompareSettingZeroAndCarryFlag)
{
	CompareRegisterZeroPageZeroAndCarry(CPU::INS_CMP_ZP,  &CPU::A);
}

TEST_F(M6502CompareTest, CMP_ABSCanCompareSettingZeroAndCarryFlag)
{
	CompareRegisterAbsoluteZeroAndCarry(CPU::INS_CMP_ABS,  &CPU::A);
}


TEST_F(M6502CompareTest, CMX_IMCanSetZeroAndCarryFlag)
{
	CompareRegisterImmediateZeroAndCarry(CPU::INS_CMX_IM, &CPU::X);
}

TEST_F(M6502CompareTest, CMX_IMCanSetCarryFlag)
{
	CompareRegisterImmediateCarry(CPU::INS_CMX_IM, &CPU::X);
}

TEST_F(M6502CompareTest, CMX_IMCanSetNegativeFlag)
{
	CompareRegisterImmediateNegative(CPU::INS_CMX_IM, &CPU::X);
}

TEST_F(M6502CompareTest, CMX_ZPCanCompareSettingZeroAndCarryFlag)
{
	CompareRegisterZeroPageZeroAndCarry(CPU::INS_CMX_ZP, &CPU::X);
}

TEST_F(M6502CompareTest, CMX_ABSCanCompareSettingZeroAndCarryFlag)
{
	CompareRegisterAbsoluteZeroAndCarry(CPU::INS_CMX_ABS, &CPU::X);
}


TEST_F(M6502CompareTest, CMY_IMCanSetZeroAndCarryFlag)
{
	CompareRegisterImmediateZeroAndCarry(CPU::INS_CMY_IM, &CPU::Y);
}

TEST_F(M6502CompareTest, CMY_IMCanSetCarryFlag)
{
	CompareRegisterImmediateCarry(CPU::INS_CMY_IM, &CPU::Y);
}

TEST_F(M6502CompareTest, CMY_IMCanSetNegativeFlag)
{
	CompareRegisterImmediateNegative(CPU::INS_CMY_IM, &CPU::Y);
}

TEST_F(M6502CompareTest, CMY_ZPCanCompareSettingZeroAndCarryFlag)
{
	CompareRegisterZeroPageZeroAndCarry(CPU::INS_CMY_ZP, &CPU::Y);
}


TEST_F(M6502CompareTest, CMY_ABSCanCompareSettingZeroAndCarryFlag)
{
	CompareRegisterAbsoluteZeroAndCarry(CPU::INS_CMY_ABS, &CPU::Y);
}
