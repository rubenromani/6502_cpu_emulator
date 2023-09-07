#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502StatusFlagsTest : public testing::Test
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

};

TEST_F(M6502StatusFlagsTest, CLCClearCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.C = true;
	mem[0xFF00] = CPU::INS_CLC;
	const s32 EXPECTED_CYCLES = 2;

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502StatusFlagsTest, SECSetCarryFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.C = false;
	mem[0xFF00] = CPU::INS_SEC;
	const s32 EXPECTED_CYCLES = 2;

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_TRUE(cpu.PS.Flags.C);
}

TEST_F(M6502StatusFlagsTest, CLIClearInterruptFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.I = true;
	mem[0xFF00] = CPU::INS_CLI;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.I);
}

TEST_F(M6502StatusFlagsTest, SEISetInterruptFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.I = false;
	mem[0xFF00] = CPU::INS_SEI;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_TRUE(cpu.PS.Flags.I);
}

TEST_F(M6502StatusFlagsTest, CLVClearOverflowFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.V = true;
	mem[0xFF00] = CPU::INS_CLV;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.V);
}

TEST_F(M6502StatusFlagsTest, CLDClearDecimalFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.D = true;
	mem[0xFF00] = CPU::INS_CLD;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.D);
}

TEST_F(M6502StatusFlagsTest, SEDSetDecimalFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.D = false;
	mem[0xFF00] = CPU::INS_SED;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_TRUE(cpu.PS.Flags.D);
}

TEST_F(M6502StatusFlagsTest, NOPDNothing)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	mem[0xFF00] = CPU::INS_NOP;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
}