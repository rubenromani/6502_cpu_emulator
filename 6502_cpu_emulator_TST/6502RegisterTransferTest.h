#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502RegisterTransefTest : public testing::Test
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

	void TransferRegister(
		Byte OpCodeToTest,
		Byte CPU::* TrasferFromRegister,
		Byte CPU::* TransferToRegister,
		bool negative,
		bool zero);

};

void M6502RegisterTransefTest::TransferRegister(
	Byte OpCodeToTest,
	Byte CPU::* TrasferFromRegister,
	Byte CPU::* TransferToRegister,
	bool negative,
	bool zero)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.PS.Flags.Z = zero ? false : true;
	cpu.PS.Flags.N = negative ? false : true;
	cpu.*TrasferFromRegister = zero ? 0x00 : (negative ? 0xFF : 0x01);
	cpu.*TransferToRegister = zero ? 0x01 : 0x00;
	mem[0xFF00] = OpCodeToTest;
	constexpr s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.*TransferToRegister, zero ? 0x00 : (negative ? 0xFF : 0x01));
	EXPECT_EQ(cpu.PS.Flags.Z, zero ? true : false);
	EXPECT_EQ(cpu.PS.Flags.N, negative ? true : false);
}

TEST_F(M6502RegisterTransefTest, TYACanTransferTheRegisterYToTheARegister)
{
	TransferRegister(CPU::INS_TYA, &CPU::Y, &CPU::A, false, false);
}

TEST_F(M6502RegisterTransefTest, TYACanTransferTheZeroRegisterYToTheARegister)
{
	TransferRegister(CPU::INS_TYA, &CPU::Y, &CPU::A, false, true);
}

TEST_F(M6502RegisterTransefTest, TYACanTransferTheNegativeRegisterYToTheARegister)
{
	TransferRegister(CPU::INS_TYA, &CPU::Y, &CPU::A, true, false);
}


TEST_F(M6502RegisterTransefTest, TXACanTransferTheRegisterXToTheARegister)
{
	TransferRegister(CPU::INS_TXA, &CPU::X, &CPU::A, false, false);
}

TEST_F(M6502RegisterTransefTest, TXACanTransferTheZeroRegisterXToTheARegister)
{
	TransferRegister(CPU::INS_TXA, &CPU::X, &CPU::A, false, true);
}

TEST_F(M6502RegisterTransefTest, TXACanTransferTheNegativeRegisterXToTheARegister)
{
	TransferRegister(CPU::INS_TXA, &CPU::X, &CPU::A, true, false);
}


TEST_F(M6502RegisterTransefTest, TAYCanTransferTheRegisterAToTheYRegister)
{
	TransferRegister(CPU::INS_TAY, &CPU::A, &CPU::Y, false, false);
}

TEST_F(M6502RegisterTransefTest, TAYCanTransferTheZeroRegisterAToTheYRegister)
{
	TransferRegister(CPU::INS_TAY, &CPU::A, &CPU::Y, false, true);
}

TEST_F(M6502RegisterTransefTest, TAYCanTransferTheNegativeRegisterAToTheYRegister)
{
	TransferRegister(CPU::INS_TAY, &CPU::A, &CPU::Y, true, false);
}


TEST_F(M6502RegisterTransefTest, TAXCanTransferTheRegisterAToTheXRegister)
{
	TransferRegister(CPU::INS_TAX, &CPU::A, &CPU::X, false, false);
}

TEST_F(M6502RegisterTransefTest, TAXCanTransferTheZeroRegisterAToTheXRegister)
{
	TransferRegister(CPU::INS_TAX, &CPU::A, &CPU::X, false, true);
}

TEST_F(M6502RegisterTransefTest, TAXCanTransferTheNegativeRegisterAToTheXRegister)
{
	TransferRegister(CPU::INS_TAX, &CPU::A, &CPU::X, true, false);
}