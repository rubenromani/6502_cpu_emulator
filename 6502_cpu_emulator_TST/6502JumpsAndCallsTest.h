#pragma once
#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502JumpsAndCallsTest : public testing::Test
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

	void VerifyUnmodifiedFlagsFromStoreRegister();


};

void M6502JumpsAndCallsTest::VerifyUnmodifiedFlagsFromStoreRegister()
{
	EXPECT_EQ(cpu.PS.Flags.C, cpuCopy.PS.Flags.C);
	EXPECT_EQ(cpu.PS.Flags.I, cpuCopy.PS.Flags.I);
	EXPECT_EQ(cpu.PS.Flags.D, cpuCopy.PS.Flags.D);
	EXPECT_EQ(cpu.PS.Flags.B, cpuCopy.PS.Flags.B);
	EXPECT_EQ(cpu.PS.Flags.V, cpuCopy.PS.Flags.V);
	EXPECT_EQ(cpu.PS.Flags.Z, cpuCopy.PS.Flags.Z);
	EXPECT_EQ(cpu.PS.Flags.N, cpuCopy.PS.Flags.N);
}

TEST_F(M6502JumpsAndCallsTest, CanJumpToASubroutineAndJumpBackAgain)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_JSR;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	mem[0x8002] = CPU::INS_RTS;
	mem[0xFF03] = CPU::INS_LDA_IM;
	mem[0xFF04] = 0x42;
	constexpr s32 EXPECTED_CYCLES = 6 + 6 + 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x42);
}

TEST_F(M6502JumpsAndCallsTest, JSRDoesNotAffectTheProcessorStatus)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_JSR;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
	EXPECT_NE(cpu.SP, cpuCopy.SP);
}

TEST_F(M6502JumpsAndCallsTest, RTSDoesNotAffectTheProcessorStatus)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_JSR;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	mem[0x8002] = CPU::INS_RTS;
	constexpr s32 EXPECTED_CYCLES = 6 + 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
	EXPECT_EQ(cpu.PC, 0xFF03);
}

TEST_F(M6502JumpsAndCallsTest, JumpAbsoluteCanJumpToANewLocationInTheProgram)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_JMP_ABS;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	constexpr s32 EXPECTED_CYCLES = 3;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
	EXPECT_EQ(cpu.SP, cpuCopy.SP);
	EXPECT_EQ(cpu.PC, 0x8002);
}

TEST_F(M6502JumpsAndCallsTest, JumpIndirectCanJumpToANewLocationInTheProgram)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0;
	mem[0xFF00] = CPU::INS_JMP_IND;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	mem[0x8002] = 0x00;
	mem[0x8003] = 0x90;
	constexpr s32 EXPECTED_CYCLES = 5;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
	EXPECT_EQ(cpu.SP, cpuCopy.SP);
	EXPECT_EQ(cpu.PC, 0x9000);
}