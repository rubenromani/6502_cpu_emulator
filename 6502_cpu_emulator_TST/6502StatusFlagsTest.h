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
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0080 + offsetAddr], zero ? 0x00 : (negative ? 0x80 : 0x02));
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);

}
