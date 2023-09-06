#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502LoadProgramTest : public testing::Test
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


TEST_F(M6502LoadProgramTest, TestLoadAProgramIntoThecorrectAreaOfTheMemory)
{
	Byte prg[] = { 0x00, 0x10, 0xa9, 0xff, 0x85, 0x90, 0x8d,
					0x00, 0x80, 0x49, 0xcc, 0x4c, 0x00, 0x10 };

	// when: 
	cpu.LoadPrg(prg, 14, mem);

	// then:
	EXPECT_EQ(mem[0x0FFF], 0x00);
	EXPECT_EQ(mem[0x1000], 0xA9);
	EXPECT_EQ(mem[0x1001], 0xFF);
	EXPECT_EQ(mem[0x1002], 0x85);
	//....
	EXPECT_EQ(mem[0x1009], 0x4C);
	EXPECT_EQ(mem[0x100A], 0x00);
	EXPECT_EQ(mem[0x100B], 0x10);

	/*
	//when
	for (u32 Clock = 10000; Clock > 0;)
	{
		Clock -= cpu.Execute(20, mem);
	}

	// then: ???
	*/
}

TEST_F(M6502LoadProgramTest, TestLoadAProgramAndExecuteIt)
{
	Byte prg[] = { 0x00, 0x10, 0xa9, 0xff, 0x85, 0x90, 0x8d,
					0x00, 0x80, 0x49, 0xcc, 0x4c, 0x00, 0x10 };

	// when: 
	Word Address = cpu.LoadPrg(prg, 14, mem);
	cpu.PC = Address;

	//when
	for (s32 Clock = 10000; Clock > 0;)
	{
		Clock -= cpu.Execute(20, mem);
		//printf("A: %d\t PC: %d\n", cpu.A, cpu.PC);
	}

	// then: ???
}