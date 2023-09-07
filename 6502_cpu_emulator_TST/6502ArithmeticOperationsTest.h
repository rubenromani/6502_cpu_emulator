#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502ArithmeticOperationsTest : public testing::Test
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

	Word AddrZeroPage(
		Word fromAddress);
	
	Word AddrZeroPageOffset(
		Word fromAddress, 
		Byte CPU::*OffsetRegister);
	
	Word AddrAbsolute(
		Word fromAddress);
	
	Word AddrAbsoluteOffset(
		Word fromAddress, 
		Byte CPU::*OffsetRegister);
	
	Word AddrIndirectX(
		Word fromAddress);
	
	Word AddrIndirectY(
		Word fromAddress);

};

Word M6502ArithmeticOperationsTest::AddrZeroPage(
	Word fromAddress) 
{
	Word ZeroPageAddress = static_cast<Word>(mem[fromAddress]);
	return ZeroPageAddress;
}

Word M6502ArithmeticOperationsTest::AddrZeroPageOffset(
	Word fromAddress, 
	Byte CPU::* OffsetRegister) 
{
	Word ZeroPageAddr = static_cast<Word>(mem[fromAddress]);
	ZeroPageAddr += static_cast<Word>(cpu.*OffsetRegister);
	return ZeroPageAddr;
}

Word M6502ArithmeticOperationsTest::AddrAbsolute(
	Word fromAddress) 
{
	Word AbsoluteAddress = ((static_cast<Word>(mem[fromAddress+1])) << 8) |
		(static_cast<Word>(mem[fromAddress]));
	return AbsoluteAddress;
}

Word M6502ArithmeticOperationsTest::AddrAbsoluteOffset(
	Word fromAddress, 
	Byte CPU::* OffsetRegister) 
{
	Word AbsoluteAddress = ((static_cast<Word>(mem[fromAddress+1])) << 8) |
		(static_cast<Word>(mem[fromAddress]));
	AbsoluteAddress += static_cast<Word>(cpu.*OffsetRegister);
	return AbsoluteAddress;
}

Word M6502ArithmeticOperationsTest::AddrIndirectX(
	Word fromAddress) 
{
	Word ZPAddress = static_cast<Word>(mem[fromAddress]);
	ZPAddress += static_cast<Word>(cpu.X);
	Word IndirectXAddress = 
		((static_cast<Word>(mem[ZPAddress+1])) << 8) | static_cast<Word>(mem[ZPAddress]);
	return IndirectXAddress;
}

Word M6502ArithmeticOperationsTest::AddrIndirectY(
	Word fromAddress) 
{
	Word ZPAddress = static_cast<Word>(mem[fromAddress]);
	Word IndirectYAddress = 
		((static_cast<Word>(mem[ZPAddress+1])) << 8) | static_cast<Word>(mem[ZPAddress]);
	IndirectYAddress = IndirectYAddress + static_cast<Word>(cpu.Y);
	return IndirectYAddress;
}




TEST_F(M6502ArithmeticOperationsTest, ADCImmediateCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	mem[0xFF00] = CPU::INS_ADC_IM;
	mem[0xFF01] = 0x01;					
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADCImmediateCanAddWithCarry)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x00;
	mem[0xFF00] = CPU::INS_ADC_IM;
	mem[0xFF01] = 0x00;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = true;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x01);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADCImmediateCanAddSettingNegativeAndOverflowFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	mem[0xFF00] = CPU::INS_ADC_IM;
	mem[0xFF01] = 0x7F;
	cpu.PS.Flags.N = false;
	cpu.PS.Flags.V = false;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x80);
	EXPECT_TRUE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADCImmediateCanAddSettingZeroFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x00;
	mem[0xFF00] = CPU::INS_ADC_IM;
	mem[0xFF01] = 0x00;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = false;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x00);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADCImmediateCanAddSettingCarryAndOverflowFlag)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0xFF;
	mem[0xFF00] = CPU::INS_ADC_IM;
	mem[0xFF01] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = false;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 2;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x00);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_TRUE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.C);
}


TEST_F(M6502ArithmeticOperationsTest, ADC_ZPCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	mem[0xFF00] = CPU::INS_ADC_ZP;
	mem[0xFF01] = 0x80;
	mem[AddrZeroPage(0xFF01)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 3;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_ZPXCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0x05;
	mem[0xFF00] = CPU::INS_ADC_ZPX;
	mem[0xFF01] = 0x80;
	mem[AddrZeroPageOffset(0xFF01, &CPU::X)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_ABSCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0x05;
	mem[0xFF00] = CPU::INS_ADC_ABS;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[AddrAbsolute(0xFF01)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_ABSXCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0x05;
	mem[0xFF00] = CPU::INS_ADC_ABSX;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[AddrAbsoluteOffset(0xFF01, &CPU::X)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_ABSXPageCrossCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0xFF;
	mem[0xFF00] = CPU::INS_ADC_ABSX;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	mem[AddrAbsoluteOffset(0xFF01, &CPU::X)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 5;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_ABSYCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0x05;
	mem[0xFF00] = CPU::INS_ADC_ABSY;
	mem[0xFF01] = 0x00;
	mem[0xFF02] = 0x80;
	mem[AddrAbsoluteOffset(0xFF01, &CPU::Y)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 4;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_ABSYPageCrossCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0xFF;
	mem[0xFF00] = CPU::INS_ADC_ABSY;
	mem[0xFF01] = 0x02;
	mem[0xFF02] = 0x80;
	mem[AddrAbsoluteOffset(0xFF01, &CPU::Y)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 5;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_INXCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.X = 0x05;
	mem[0xFF00] = CPU::INS_ADC_INX;
	mem[0xFF01] = 0x80;
	mem[0x0085] = 0x00;
	mem[0x0086] = 0x80;
	mem[AddrIndirectX(0xFF01)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_INYCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0x05;
	mem[0xFF00] = CPU::INS_ADC_INY;
	mem[0xFF01] = 0x80;
	mem[0x0080] = 0x00;
	mem[0x0081] = 0x80;
	mem[AddrIndirectY(0xFF01)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 5;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}

TEST_F(M6502ArithmeticOperationsTest, ADC_INYPageCrossCanAddWithoutSettingFlags)
{
	// Given:
	cpu.Reset(mem, 0xFF00);
	cpu.A = 0x01;
	cpu.Y = 0xFF;
	mem[0xFF00] = CPU::INS_ADC_INY;
	mem[0xFF01] = 0x80;
	mem[0x0080] = 0x02;
	mem[0x0081] = 0x80;
	mem[AddrIndirectY(0xFF01)] = 0x01;
	cpu.PS.Flags.N = true;
	cpu.PS.Flags.V = true;
	cpu.PS.Flags.Z = true;
	cpu.PS.Flags.C = false;
	const s32 EXPECTED_CYCLES = 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(cpu.A, 0x02);
	EXPECT_FALSE(cpu.PS.Flags.N);
	EXPECT_FALSE(cpu.PS.Flags.V);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.C);
}