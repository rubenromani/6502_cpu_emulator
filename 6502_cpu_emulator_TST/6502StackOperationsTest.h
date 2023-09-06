#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502StackOperationTest : public testing::Test
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

TEST_F(M6502StackOperationTest, TSXCanTransferTheStackPointerToXRegister)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x01;
	mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x01);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
}

TEST_F(M6502StackOperationTest, TSXCanTransferTheAZeroStackPointerToXRegister)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x00;
	mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0x00);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
}

TEST_F(M6502StackOperationTest, TSXCanTransferTheANegativeStackPointerToXRegister)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.N = false;
	cpu.X = 0x00;
	cpu.SP = 0xFF;
	mem[0xFF00] = CPU::INS_TSX;
	constexpr s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.X, 0xFF);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.N);
}

TEST_F(M6502StackOperationTest, TXSCanTransferTheRegisterXToTheStackPoininterRegister)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.N = true;
	cpu.X = 0x01;
	cpu.SP = 0x00;
	mem[0xFF00] = CPU::INS_TXS;
	constexpr s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.SP, 0x01);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
}

TEST_F(M6502StackOperationTest, TXSCanTransferTheZeroRegisterXToTheStackPoininterRegister)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.N = true;
	cpu.X = 0x00;
	cpu.SP = 0x01;
	mem[0xFF00] = CPU::INS_TXS;
	constexpr s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.SP, 0x00);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
}

TEST_F(M6502StackOperationTest, TXSCanTransferTheNegativeRegisterXToTheStackPoininterRegister)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.N = false;
	cpu.X = 0xFF;
	cpu.SP = 0x00;
	mem[0xFF00] = CPU::INS_TXS;
	constexpr s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.SP, 0xFF);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.N);
}

TEST_F(M6502StackOperationTest, PHACanPushARegisterOntoTheStack)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x42;
	mem[0xFF00] = CPU::INS_PHA;
	constexpr s32 EXPECTED_CYCLES = 3;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[cpu.SPToAddress()+1], cpu.A);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
	EXPECT_EQ(cpu.SP, 0xFE);
}

TEST_F(M6502StackOperationTest, PHPCanPushPSRegisterOntoTheStack)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Reg = 0xCC;
	mem[0xFF00] = CPU::INS_PHP;
	constexpr s32 EXPECTED_CYCLES = 3;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[cpu.SPToAddress() + 1], 0xCC);
	EXPECT_EQ(cpu.PS.Reg, cpuCopy.PS.Reg);
	EXPECT_EQ(cpu.SP, 0xFE);
}

TEST_F(M6502StackOperationTest, PLACanPullAccumulatorFromTheStack)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.N = true;
	cpu.A = 0x00;
	cpu.SP--;
	mem[0x01FF] = 0x04;
	mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x04);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(M6502StackOperationTest, PLACanPullTheZeroAccumulatorFromTheStack)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.N = true;
	cpu.A = 0x01;
	cpu.SP--;
	mem[0x01FF] = 0x00;
	mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x00);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(M6502StackOperationTest, PLACanPullTheNegativeAccumulatorFromTheStack)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.N = false;
	cpu.A = 0x00;
	cpu.SP--;
	mem[0x01FF] = 0xFF;
	mem[0xFF00] = CPU::INS_PLA;
	constexpr s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0xFF);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.N);
	EXPECT_EQ(cpu.SP, 0xFF);
}

TEST_F(M6502StackOperationTest, PLPCanPullThePSRegisterFromTheStack)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Reg = 0x00;
	cpu.SP--;
	mem[0x01FF] = 0x42;
	mem[0xFF00] = CPU::INS_PLP;
	constexpr s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.PS.Reg, 0x42);
	EXPECT_EQ(cpu.SP, 0xFF);
}