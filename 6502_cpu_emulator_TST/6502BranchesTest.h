#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502BranchTest : public testing::Test
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

	void BranchConditionForward(
		Byte OpcodeToTest,
		bool condition,
		bool crosspage);

	void BranchConditionBackward(
		Byte OpcodeToTest,
		bool condition,
		bool crosspage);
};

void M6502BranchTest::BranchConditionForward(
	Byte OpcodeToTest,
	bool condition,
	bool crosspage)
{
	// given:
	CpuMakeCopy();
	Word ResetAddress = crosspage ? 0xF0FD : 0xF000;
	cpu.Reset(mem, ResetAddress);
	cpu.PS = cpuCopy.PS;
	mem[ResetAddress] = OpcodeToTest;
	mem[ResetAddress+1] = 0x01;
	// after this PC = 0xFF02
	// then it is incremento up to 0xF101/0x003
	s32 EXPECTED_CYCLES = 2 + condition*(1 + crosspage*2); // 2, 3, 5

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, condition ? (crosspage ? 0xF100 : 0xF003) : 0xF002);
}

void M6502BranchTest::BranchConditionBackward(
	Byte OpcodeToTest,
	bool condition,
	bool crosspage)
{
	// given:
	CpuMakeCopy();
	Word ResetAddress = crosspage ? 0xF000 : 0xF005;
	cpu.Reset(mem, ResetAddress);
	cpu.PS = cpuCopy.PS;
	mem[ResetAddress] = OpcodeToTest;
	mem[ResetAddress + 1] = static_cast<Byte>(-0x03);
	// after this PC = 0xFF02
	// then it is incremento up to 0xF101/0x003
	s32 EXPECTED_CYCLES = 2 + condition * (1 + crosspage * 2); // 2, 3, 5

	// when:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PC, condition ? (crosspage ? 0xEFFF : 0xF004) : 0xF007);
}

TEST_F(M6502BranchTest, BEQForwardNoSucceed)
{
	cpu.PS.Flags.Z = false;
	BranchConditionForward(CPU::INS_BEQ, cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BEQForwardSucceed)
{
	cpu.PS.Flags.Z = true;
	BranchConditionForward(CPU::INS_BEQ, cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BEQForwardCrosspage)
{
	cpu.PS.Flags.Z = true;
	BranchConditionForward(CPU::INS_BEQ, cpu.PS.Flags.Z, true);
}

TEST_F(M6502BranchTest, BEQBackwardNoSucceed)
{
	cpu.PS.Flags.Z = false;
	BranchConditionBackward(CPU::INS_BEQ, cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BEQBackwardSucceed)
{
	cpu.PS.Flags.Z = true;
	BranchConditionBackward(CPU::INS_BEQ, cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BEQBackwardCrosspage)
{
	cpu.PS.Flags.Z = true;
	BranchConditionBackward(CPU::INS_BEQ, cpu.PS.Flags.Z, true);
}

TEST_F(M6502BranchTest, BNEForwardNoSucceed)
{
	cpu.PS.Flags.Z = true;
	BranchConditionForward(CPU::INS_BNE, !cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BNEForwardSucceed)
{
	cpu.PS.Flags.Z = false;
	BranchConditionForward(CPU::INS_BNE, !cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BNEForwardCrosspage)
{
	cpu.PS.Flags.Z = false;
	BranchConditionForward(CPU::INS_BNE, !cpu.PS.Flags.Z, true);
}

TEST_F(M6502BranchTest, BNEBackwardNoSucceed)
{
	cpu.PS.Flags.Z = true;
	BranchConditionBackward(CPU::INS_BNE, !cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BNEBackwardSucceed)
{
	cpu.PS.Flags.Z = false;
	BranchConditionBackward(CPU::INS_BNE, !cpu.PS.Flags.Z, false);
}

TEST_F(M6502BranchTest, BNEBackwardCrosspage)
{
	cpu.PS.Flags.Z = false;
	BranchConditionBackward(CPU::INS_BNE, !cpu.PS.Flags.Z, true);
}

TEST_F(M6502BranchTest, BCCForwardNoSucceed)
{
	cpu.PS.Flags.C = true;
	BranchConditionForward(CPU::INS_BCC, !cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCCForwardSucceed)
{
	cpu.PS.Flags.C = false;
	BranchConditionForward(CPU::INS_BCC, !cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCCForwardCrosspage)
{
	cpu.PS.Flags.C = false;
	BranchConditionForward(CPU::INS_BCC, !cpu.PS.Flags.C, true);
}

TEST_F(M6502BranchTest, BCCBackwardNoSucceed)
{
	cpu.PS.Flags.C = true;
	BranchConditionBackward(CPU::INS_BCC, !cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCCBackwardSucceed)
{
	cpu.PS.Flags.C = false;
	BranchConditionBackward(CPU::INS_BCC, !cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCCBackwardCrosspage)
{
	cpu.PS.Flags.C = false;
	BranchConditionBackward(CPU::INS_BCC, !cpu.PS.Flags.C, true);
}


TEST_F(M6502BranchTest, BCSForwardNoSucceed)
{
	cpu.PS.Flags.C = false;
	BranchConditionForward(CPU::INS_BCS, cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCSForwardSucceed)
{
	cpu.PS.Flags.C = true;
	BranchConditionForward(CPU::INS_BCS, cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCSForwardCrosspage)
{
	cpu.PS.Flags.C = true;
	BranchConditionForward(CPU::INS_BCS, cpu.PS.Flags.C, true);
}

TEST_F(M6502BranchTest, BCSBackwardNoSucceed)
{
	cpu.PS.Flags.C = false;
	BranchConditionBackward(CPU::INS_BCS, cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCSBackwardSucceed)
{
	cpu.PS.Flags.C = true;
	BranchConditionBackward(CPU::INS_BCS, cpu.PS.Flags.C, false);
}

TEST_F(M6502BranchTest, BCSBackwardCrosspage)
{
	cpu.PS.Flags.C = true;
	BranchConditionBackward(CPU::INS_BCS, cpu.PS.Flags.C, true);
}

TEST_F(M6502BranchTest, BVSForwardNoSucceed)
{
	cpu.PS.Flags.V = false;
	BranchConditionForward(CPU::INS_BVS, cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVSForwardSucceed)
{
	cpu.PS.Flags.V = true;
	BranchConditionForward(CPU::INS_BVS, cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVSForwardCrosspage)
{
	cpu.PS.Flags.V = true;
	BranchConditionForward(CPU::INS_BVS, cpu.PS.Flags.V, true);
}

TEST_F(M6502BranchTest, BVSBackwardNoSucceed)
{
	cpu.PS.Flags.V = false;
	BranchConditionBackward(CPU::INS_BVS, cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVSBackwardSucceed)
{
	cpu.PS.Flags.V = true;
	BranchConditionBackward(CPU::INS_BVS, cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVSBackwardCrosspage)
{
	cpu.PS.Flags.V = true;
	BranchConditionBackward(CPU::INS_BVS, cpu.PS.Flags.V, true);
}

TEST_F(M6502BranchTest, BVCForwardNoSucceed)
{
	cpu.PS.Flags.V = true;
	BranchConditionForward(CPU::INS_BVC, !cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVCForwardSucceed)
{
	cpu.PS.Flags.V = false;
	BranchConditionForward(CPU::INS_BVC, !cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVCForwardCrosspage)
{
	cpu.PS.Flags.V = false;
	BranchConditionForward(CPU::INS_BVC, !cpu.PS.Flags.V, true);
}

TEST_F(M6502BranchTest, BVCBackwardNoSucceed)
{
	cpu.PS.Flags.V = true;
	BranchConditionBackward(CPU::INS_BVC, !cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVCBackwardSucceed)
{
	cpu.PS.Flags.V = false;
	BranchConditionBackward(CPU::INS_BVC, !cpu.PS.Flags.V, false);
}

TEST_F(M6502BranchTest, BVCBackwardCrosspage)
{
	cpu.PS.Flags.V = false;
	BranchConditionBackward(CPU::INS_BVC, !cpu.PS.Flags.V, true);
}

TEST_F(M6502BranchTest, BMIForwardNoSucceed)
{
	cpu.PS.Flags.N = false;
	BranchConditionForward(CPU::INS_BMI, cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BMIForwardSucceed)
{
	cpu.PS.Flags.N = true;
	BranchConditionForward(CPU::INS_BMI, cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BMIForwardCrosspage)
{
	cpu.PS.Flags.N = true;
	BranchConditionForward(CPU::INS_BMI, cpu.PS.Flags.N, true);
}

TEST_F(M6502BranchTest, BMIBackwardNoSucceed)
{
	cpu.PS.Flags.N = false;
	BranchConditionBackward(CPU::INS_BMI, cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BMIBackwardSucceed)
{
	cpu.PS.Flags.N = true;
	BranchConditionBackward(CPU::INS_BMI, cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BMIBackwardCrosspage)
{
	cpu.PS.Flags.N = true;
	BranchConditionBackward(CPU::INS_BMI, cpu.PS.Flags.N, true);
}

TEST_F(M6502BranchTest, BPLForwardNoSucceed)
{
	cpu.PS.Flags.N = true;
	BranchConditionForward(CPU::INS_BPL, !cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BPLForwardSucceed)
{
	cpu.PS.Flags.N = false;
	BranchConditionForward(CPU::INS_BPL, !cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BPLForwardCrosspage)
{
	cpu.PS.Flags.N = false;
	BranchConditionForward(CPU::INS_BPL, !cpu.PS.Flags.N, true);
}

TEST_F(M6502BranchTest, BPLBackwardNoSucceed)
{
	cpu.PS.Flags.N = true;
	BranchConditionBackward(CPU::INS_BPL, !cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BPLBackwardSucceed)
{
	cpu.PS.Flags.N = false;
	BranchConditionBackward(CPU::INS_BPL, !cpu.PS.Flags.N, false);
}

TEST_F(M6502BranchTest, BPLBackwardCrosspage)
{
	cpu.PS.Flags.N = false;
	BranchConditionBackward(CPU::INS_BPL, !cpu.PS.Flags.N, true);
}