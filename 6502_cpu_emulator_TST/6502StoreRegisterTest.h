#pragma once
#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;

class M6502StoreRegisterTest : public testing::Test
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

	void ST_ZP(
		Byte OpcodeToTest,
		Byte CPU::*RegisterToTest,
		Byte CPU::*OffsetRegister);

	void ST_ABS(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest,
		Byte CPU::* OffsetRegister);

};

void M6502StoreRegisterTest::VerifyUnmodifiedFlagsFromStoreRegister()
{
	EXPECT_EQ(cpu.PS.Flags.C, cpuCopy.PS.Flags.C);
	EXPECT_EQ(cpu.PS.Flags.I, cpuCopy.PS.Flags.I);
	EXPECT_EQ(cpu.PS.Flags.D, cpuCopy.PS.Flags.D);
	EXPECT_EQ(cpu.PS.Flags.B, cpuCopy.PS.Flags.B);
	EXPECT_EQ(cpu.PS.Flags.V, cpuCopy.PS.Flags.V);
	EXPECT_EQ(cpu.PS.Flags.Z, cpuCopy.PS.Flags.Z);
	EXPECT_EQ(cpu.PS.Flags.N, cpuCopy.PS.Flags.N);
}

void M6502StoreRegisterTest::ST_ZP(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest,
	Byte CPU::* OffsetRegister)
{
	// Given:
	Byte offsetAddr = 0x0;
	s32 offsetCycles = 0;
	if (OffsetRegister != nullptr)
	{
		cpu.*OffsetRegister = 0x5;
		offsetAddr = 0x5;
		offsetCycles = 1;

	}
	cpu.*RegisterToTest = 0x2F;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;					// Zero Page Address
	mem[0x0080 + offsetAddr] = 0x00;
	const s32 EXPECTED_CYCLES = 3 + offsetCycles;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x0080 + offsetAddr], 0x2F);
	VerifyUnmodifiedFlagsFromStoreRegister();
}

void M6502StoreRegisterTest::ST_ABS(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest,
	Byte CPU::* OffsetRegister)
{
	// Given:
	Byte offsetAddr = 0x0;
	s32 offsetCycles = 0;
	if (OffsetRegister != nullptr)
	{
		cpu.*OffsetRegister = 0x5;
		offsetAddr = 0x5;
		offsetCycles = 1;
	}
	cpu.*RegisterToTest = 0x2F;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x01;					// Absolute Address : Low Byte
	mem[0xFFFE] = 0x80;					// Absolute Address : High Byte
	mem[0x8001 + offsetAddr] = 0x00;
	const s32 EXPECTED_CYCLES = 4 + offsetCycles;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x8001 + offsetAddr], 0x2F);
	VerifyUnmodifiedFlagsFromStoreRegister();
}

TEST_F(M6502StoreRegisterTest, STAZeroPageCanStoreARegisterIntoMemory)
{
	ST_ZP(CPU::INS_STA_ZP, &CPU::A, nullptr);
}

TEST_F(M6502StoreRegisterTest, STAZeroPageXCanStoreARegisterIntoMemory)
{
	ST_ZP(CPU::INS_STA_ZPX, &CPU::A, &CPU::X);
}

TEST_F(M6502StoreRegisterTest, STXZeroPageCanStoreXRegisterIntoMemory)
{
	ST_ZP(CPU::INS_STX_ZP, &CPU::X, nullptr);
}

TEST_F(M6502StoreRegisterTest, STXZeroPageYCanStoreXRegisterIntoMemory)
{
	ST_ZP(CPU::INS_STX_ZPY, &CPU::X, &CPU::Y);
}

TEST_F(M6502StoreRegisterTest, STYZeroPageCanStoreYRegisterIntoMemory)
{
	ST_ZP(CPU::INS_STY_ZP, &CPU::Y, nullptr);
}

TEST_F(M6502StoreRegisterTest, STYZeroPageXCanStoreYRegisterIntoMemory)
{
	ST_ZP(CPU::INS_STY_ZPX, &CPU::Y, &CPU::X);
}

TEST_F(M6502StoreRegisterTest, STAAbsoluteCanStoreARegisterIntoMemory)
{
	ST_ABS(CPU::INS_STA_ABS, &CPU::A, nullptr);
}

TEST_F(M6502StoreRegisterTest, STAAbsoluteXCanStoreARegisterIntoMemory)
{
	ST_ABS(CPU::INS_STA_ABSX, &CPU::A, &CPU::X);
}

TEST_F(M6502StoreRegisterTest, STAAbsoluteYCanStoreARegisterIntoMemory)
{
	ST_ABS(CPU::INS_STA_ABSY, &CPU::A, &CPU::Y);
}

TEST_F(M6502StoreRegisterTest, STXAbsoluteCanStoreXRegisterIntoMemory)
{
	ST_ABS(CPU::INS_STX_ABS, &CPU::X, nullptr);
}

TEST_F(M6502StoreRegisterTest, STYAbsoluteCanStoreYRegisterIntoMemory)
{
	ST_ABS(CPU::INS_STY_ABS, &CPU::Y, nullptr);
}

TEST_F(M6502StoreRegisterTest, STAIndirectXCanStoreARegisterIntoMemory)
{
	// Given:
	cpu.A = 0x2F;
	cpu.X = 0x0F;
	mem[0xFFFC] = CPU::INS_STA_INDX;
	mem[0xFFFD] = 0x80;
	mem[0x008F] = 0x30;			// Address : Low Byte
	mem[0x0090] = 0x50;			// Address : High Byte
	mem[0x5030] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x5030], 0x2F);
	VerifyUnmodifiedFlagsFromStoreRegister();
}

TEST_F(M6502StoreRegisterTest, STAIndirectYCanStoreARegisterIntoMemory)
{
	// Given:
	cpu.A = 0x2F;
	cpu.Y = 0x0F;
	mem[0xFFFC] = CPU::INS_STA_INDY;
	mem[0xFFFD] = 0x80;
	mem[0x0080] = 0x30;			// Address : Low Byte
	mem[0x0081] = 0x50;			// Address : High Byte
	mem[0x503F] = 0x00;
	constexpr s32 EXPECTED_CYCLES = 6;
	CpuMakeCopy();

	// When:
	const s32 ActualCycles = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(ActualCycles, EXPECTED_CYCLES);
	EXPECT_EQ(mem[0x503F], 0x2F);
	VerifyUnmodifiedFlagsFromStoreRegister();
}