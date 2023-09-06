#pragma once
#include "pch.h"
#include "main_6502.h"

using namespace m6502;


static void VerifyUnmodifiedFlagsFromLoadRegister(
	const CPU& cpu,
	const CPU& cpuCopy)
{
	EXPECT_EQ(cpu.PS.Flags.C, cpuCopy.PS.Flags.C);
	EXPECT_EQ(cpu.PS.Flags.I, cpuCopy.PS.Flags.I);
	EXPECT_EQ(cpu.PS.Flags.D, cpuCopy.PS.Flags.D);
	EXPECT_EQ(cpu.PS.Flags.B, cpuCopy.PS.Flags.B);
	EXPECT_EQ(cpu.PS.Flags.V, cpuCopy.PS.Flags.V);
}

class M6502LoadRegisterTest : public testing::Test
{
public:
	Mem mem;
	CPU cpu;

	virtual void SetUp()
	{
		cpu.Reset(mem);
	}

	virtual void TearDown()
	{

	}

	void TestLoadRegisterImmediate(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

	void TestLoadRegisterZeroPage(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

	void TestLoadRegisterZeroPageOffset(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest,
		Byte CPU::* OffsetRegister);

	void TestLoadRegisterAbsolute(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest);

	void TestLoadRegisterAbsoluteOffset(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest,
		Byte CPU::* OffsetRegister);

	void TestLoadRegisterAbsoluteOffsetPageCross(
		Byte OpcodeToTest,
		Byte CPU::* RegisterToTest,
		Byte CPU::* OffsetRegister);

};

void M6502LoadRegisterTest::TestLoadRegisterImmediate(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest
)
{
	// Given :
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x84;
	CPU CPUCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.*RegisterToTest, 0x84);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_TRUE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTest::TestLoadRegisterZeroPage(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x42;
	mem[0x0042] = 0x37;
	CPU CPUCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 3;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.*RegisterToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTest::TestLoadRegisterZeroPageOffset(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest,
	Byte CPU::* OffsetRegister)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	cpu.*OffsetRegister = 5;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x42;
	mem[0x0047] = 0x37;
	CPU CPUCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then :
	EXPECT_EQ(cpu.*RegisterToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTest::TestLoadRegisterAbsolute(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x80;
	mem[0xFFFE] = 0x44;	//0x4480
	mem[0x4480] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.*RegisterToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTest::TestLoadRegisterAbsoluteOffset(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest,
	Byte CPU::* OffsetRegister)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	cpu.*OffsetRegister = 0x01;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x02;
	mem[0xFFFE] = 0x44;	//0x4402
	mem[0x4403] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 4;
	CPU CPUCopy = cpu;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.*RegisterToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

void M6502LoadRegisterTest::TestLoadRegisterAbsoluteOffsetPageCross(
	Byte OpcodeToTest,
	Byte CPU::* RegisterToTest,
	Byte CPU::* OffsetRegister)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	cpu.*OffsetRegister = 0xFF;
	mem[0xFFFC] = OpcodeToTest;
	mem[0xFFFD] = 0x02;
	mem[0xFFFE] = 0x44;	//0x4402
	mem[0x4501] = 0x37; //0x4402 + 0xFF crosses page boundary
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.*RegisterToTest, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

TEST_F(M6502LoadRegisterTest, TheCPUDoesNothingWhenWeExecuteZeroCycles)
{
	// Given
	constexpr s32 NUM_CYCLES = 0;

	// When
	s32 CyclesUsed = cpu.Execute(NUM_CYCLES, mem);

	// Then
	EXPECT_EQ(CyclesUsed, 0);
}

TEST_F(M6502LoadRegisterTest, CPUCanExecuteMoreCyclesThenRequestedIfRequiredByTheIntruction)
{
	// Given :
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x84;

	// When :
	s32 CyclesUsed = cpu.Execute(1, mem);

	// Then:
	EXPECT_EQ(CyclesUsed, 2);
}

TEST_F(M6502LoadRegisterTest, LDAImmediateCanLoadAValueIntoTheARegister)
{
	TestLoadRegisterImmediate(CPU::INS_LDA_IM, &CPU::A);
}

TEST_F(M6502LoadRegisterTest, LDXImmediateCanLoadAValueIntoTheXRegister)
{
	TestLoadRegisterImmediate(CPU::INS_LDX_IM, &CPU::X);
}

TEST_F(M6502LoadRegisterTest, LDYImmediateCanLoadAValueIntoTheYRegister)
{
	TestLoadRegisterImmediate(CPU::INS_LDY_IM, &CPU::Y);
}


TEST_F(M6502LoadRegisterTest, LDAImmediateCanAffectTheZeroFlag)
{
	// Given : 
	cpu.A = 0x44;
	mem[0xFFFC] = CPU::INS_LDA_IM;
	mem[0xFFFD] = 0x00;
	CPU CPUCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 2;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_TRUE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}



TEST_F(M6502LoadRegisterTest, LDAZeroPageCanLoadAValueIntoTheARegister)
{
	TestLoadRegisterZeroPage(CPU::INS_LDA_ZP, &CPU::A);
}

TEST_F(M6502LoadRegisterTest, LDXZeroPageCanLoadAValueIntoTheXRegister)
{
	TestLoadRegisterZeroPage(CPU::INS_LDX_ZP, &CPU::X);
}

TEST_F(M6502LoadRegisterTest, LDYZeroPageCanLoadAValueIntoTheYRegister)
{
	TestLoadRegisterZeroPage(CPU::INS_LDY_ZP, &CPU::Y);
}



TEST_F(M6502LoadRegisterTest, LDAZeroPageXCanLoadAValueIntoTheARegister)
{
	TestLoadRegisterZeroPageOffset(CPU::INS_LDA_ZPX, &CPU::A, &CPU::X);
}

TEST_F(M6502LoadRegisterTest, LDXZeroPageYCanLoadAValueIntoTheXRegister)
{
	TestLoadRegisterZeroPageOffset(CPU::INS_LDX_ZPY, &CPU::X, &CPU::Y);
}

TEST_F(M6502LoadRegisterTest, LDYZeroPageXCanLoadAValueIntoTheYRegister)
{
	TestLoadRegisterZeroPageOffset(CPU::INS_LDY_ZPX, &CPU::Y, &CPU::X);
}



TEST_F(M6502LoadRegisterTest, LDAZeroPageXCanLoadAValueIntoTheRegisterWhenItWraps)
{
	// Given : 
	cpu.X = 0xFF;
	mem[0xFFFC] = CPU::INS_LDA_ZPX;
	mem[0xFFFD] = 0x80;
	mem[0x007F] = 0x37;
	CPU CPUCopy = cpu;
	constexpr s32 EXPECTED_CYCLES = 4;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}


TEST_F(M6502LoadRegisterTest, LDAAbsoluteCanLoadAValueIntoTheARegister)
{
	TestLoadRegisterAbsolute(CPU::INS_LDA_ABS, &CPU::A);
}

TEST_F(M6502LoadRegisterTest, LDXAbsoluteCanLoadAValueIntoTheXRegister)
{
	TestLoadRegisterAbsolute(CPU::INS_LDX_ABS, &CPU::X);
}

TEST_F(M6502LoadRegisterTest, LDYAbsoluteCanLoadAValueIntoTheYRegister)
{
	TestLoadRegisterAbsolute(CPU::INS_LDY_ABS, &CPU::Y);
}



TEST_F(M6502LoadRegisterTest, LDAAbsoluteXCanLoadAValueIntoTheARegister)
{
	TestLoadRegisterAbsoluteOffset(CPU::INS_LDA_ABSX, &CPU::A, &CPU::X);
}

TEST_F(M6502LoadRegisterTest, LDXAbsoluteYCanLoadAValueIntoTheXRegister)
{
	TestLoadRegisterAbsoluteOffset(CPU::INS_LDX_ABSY, &CPU::X, &CPU::Y);
}

TEST_F(M6502LoadRegisterTest, LDYAbsoluteXCanLoadAValueIntoTheYRegister)
{
	TestLoadRegisterAbsoluteOffset(CPU::INS_LDY_ABSX, &CPU::Y, &CPU::X);
}



TEST_F(M6502LoadRegisterTest, LDAAbsoluteXCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary)
{
	TestLoadRegisterAbsoluteOffsetPageCross(CPU::INS_LDA_ABSX, &CPU::A, &CPU::X);
}

TEST_F(M6502LoadRegisterTest, LDXAbsoluteYCanLoadAValueIntoTheXRegisterWhenItCrossesAPageBoundary)
{
	TestLoadRegisterAbsoluteOffsetPageCross(CPU::INS_LDX_ABSY, &CPU::X, &CPU::Y);
}

TEST_F(M6502LoadRegisterTest, LDAAbsoluteXCanLoadAValueIntoTheYRegisterWhenItCrossesAPageBoundary)
{
	TestLoadRegisterAbsoluteOffsetPageCross(CPU::INS_LDY_ABSX, &CPU::Y, &CPU::X);
}

TEST_F(M6502LoadRegisterTest, LDAAbsoluteYCanLoadAValueIntoTheARegister)
{
	TestLoadRegisterAbsoluteOffset(CPU::INS_LDA_ABSY, &CPU::A, &CPU::Y);
}

TEST_F(M6502LoadRegisterTest, LDAAbsoluteYCanLoadAValueIntoTheARegisterWhenItCrossesAPageBoundary)
{
	TestLoadRegisterAbsoluteOffsetPageCross(CPU::INS_LDA_ABSY, &CPU::A, &CPU::Y);
}

TEST_F(M6502LoadRegisterTest, LDAIndirectXCanLoadAValueIntoTheARegister)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	cpu.X = 0x04;
	mem[0xFFFC] = CPU::INS_LDA_INDX;
	mem[0xFFFD] = 0x02;
	mem[0x0006] = 0x00;	//0x02 + 0x04
	mem[0x0007] = 0x80;
	mem[0x8000] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

TEST_F(M6502LoadRegisterTest, LDAIndirectYCanLoadAValueIntoTheARegister)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	cpu.Y = 0x04;
	mem[0xFFFC] = CPU::INS_LDA_INDY;
	mem[0xFFFD] = 0x02;
	mem[0x0002] = 0x00;
	mem[0x0003] = 0x80;
	mem[0x8004] = 0x37;
	constexpr s32 EXPECTED_CYCLES = 5;
	CPU CPUCopy = cpu;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

TEST_F(M6502LoadRegisterTest, LDAIndirectYCanLoadAValueIntoTheARegisterWhenItCrossesAPage)
{
	// Given : 
	cpu.PS.Flags.Z = 1, cpu.PS.Flags.N = true;
	cpu.Y = 0xFF;
	mem[0xFFFC] = CPU::INS_LDA_INDY;
	mem[0xFFFD] = 0x02;
	mem[0x0002] = 0x02;
	mem[0x0003] = 0x80;
	mem[0x8101] = 0x37;		// 0x8002 + 0xFF
	constexpr s32 EXPECTED_CYCLES = 6;
	CPU CPUCopy = cpu;

	// When :
	s32 CyclesUsed = cpu.Execute(EXPECTED_CYCLES, mem);

	// Then:
	EXPECT_EQ(cpu.A, 0x37);
	EXPECT_EQ(CyclesUsed, EXPECTED_CYCLES);
	EXPECT_FALSE(cpu.PS.Flags.Z);
	EXPECT_FALSE(cpu.PS.Flags.N);
	VerifyUnmodifiedFlagsFromLoadRegister(cpu, CPUCopy);
}

