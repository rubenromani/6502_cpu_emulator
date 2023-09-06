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

TEST_F(M6502StatusFlagsTest, BVSBackwardCrosspage)
{
	
}
