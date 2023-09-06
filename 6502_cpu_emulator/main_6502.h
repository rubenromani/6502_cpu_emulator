#pragma once

#include <stdio.h>
#include <stdlib.h>

/* www.c64-wiki.com */

namespace m6502
{
	using Byte = unsigned char;
	using Word = unsigned short;
	using sByte = char;

	using u32 = unsigned int;
	using s32 = signed int;

	struct Mem;
	struct CPU;
	struct StatusFlags;

}

struct m6502::Mem
{
	static constexpr u32 MAX_MEM = 1024 * 64;
	Byte Data[MAX_MEM];

	void Initialise()
	{
		for (u32 i = 0; i < MAX_MEM; i++) {
			Data[i] = 0;
		}
	}

	/* read 1 byte */
	Byte operator[](u32 Address) const
	{
		//assert here Address <  MAX_MEM
		return Data[Address];
	}

	/* write 1 byte */
	Byte& operator[](u32 Address)
	{
		//assert here Address <  MAX_MEM
		return Data[Address];
	}
};

struct m6502::StatusFlags
{
	Byte C : 1;			//	status flag
	Byte Z : 1;			//	status flag
	Byte I : 1;			//	status flag
	Byte D : 1;			//	status flag
	Byte B : 1;			//	status flag
	Byte Unused : 1;
	Byte V : 1;			//	status flag
	Byte N : 1;			//	status flag
};

union PSUnion
{
	m6502::Byte Reg;
	m6502::StatusFlags Flags;
};

struct m6502::CPU
{

	Word PC;			// Program counter
	Byte SP;			// Stack Pointer

	Byte A, X, Y;		// Accumulator, Index register X, Index register Y

	PSUnion PS;

	void Reset(Mem& memory, Word InitAddress = 0xFFFC)
	{
		PC = InitAddress;
		SP = 0xFF;
		PS.Flags.C = PS.Flags.Z = PS.Flags.I = PS.Flags.D = PS.Flags.B = PS.Flags.V = PS.Flags.N = 0;
		A = X = Y = 0;
		memory.Initialise();
	}

	Byte FetchByte(s32& Cycles, const Mem& memory)
	{
		Byte Data = memory[PC];
		PC++;
		Cycles--;
		return Data;
	}


	// 6502 is little endian
	Word FetchWord(s32& Cycles, const Mem& memory)
	{
		// read less significant byte
		Word Data = memory[PC];
		PC++;

		// read most significant byte
		Data |= (memory[PC] << 8);
		PC++;

		Cycles -= 2;

		// if you want to handle endianness
		// you would have to swap bytes here
		// if(PLATFORM_BIG_ENDIAN)
		// SwapBytesInPlace(Data)

		return Data;
	}

	Byte ReadByte(s32& Cycles, Word Address, const Mem& memory)
	{
		Byte Data = memory[Address];
		Cycles--;
		return Data;
	}

	Word ReadWord(s32& Cycles, Word Address, const Mem& memory)
	{
		Byte LowByte = ReadByte(Cycles, Address, memory);
		Byte HighByte = ReadByte(Cycles, Address+1, memory);
		Word Data = ((Word)HighByte << 8) | LowByte;
		return Data;
	}
	
	/* write 1 byte to memory */
	void WriteByte(Byte Value, Word Address, s32& Cycles, Mem& memory)
	{
		memory[Address] = Value;
		Cycles--;
	}

	/* write 1 word to memory*/
	void WriteWord(Word Value, u32 Address, s32& Cycles, Mem& memory)
	{
		memory[Address] = Value & 0xFF;
		memory[Address + 1] = (Value >> 8);
		Cycles -= 2;
	}

	/* @return the stack pointer as a full 16-bit address (in the first page)*/
	Word SPToAddress() const
	{
		return 0x0100 | SP;
	}

	/* Push the PC-1 onto the stack*/
	void PushPCToStack(s32& Cycles, Mem& memory)
	{
		WriteWord(PC, SPToAddress()-1, Cycles, memory);
		SP -= 2;
	}

	/* Pop a word from the stack*/
	Word PopWordFromStack(s32& Cycles, Mem& memory)
	{
		Word Value = ReadWord(Cycles, SPToAddress()+1, memory);
		SP += 2;
		Cycles--;
		return Value;
	}

	/* Pop a byte from the stack*/
	Byte PopByteFromStack(s32& Cycles, Mem& memory)
	{
		Word StackAddress = SPToAddress() + 1;
		Byte Value = ReadByte(Cycles, SPToAddress()+1, memory);
		Cycles--;
		SP++;
		Cycles--;
		return Value;
	}

	void PushByteOnTheStack(Byte Value, s32& Cycles, Mem& memory)
	{
		memory[SPToAddress()] = Value;
		Cycles--;
		SP--;
		Cycles--;
	}


	static constexpr Byte
		// LDA
		INS_LDA_IM =	0xA9,
		INS_LDA_ZP =	0xA5,
		INS_LDA_ZPX =	0xB5,
		INS_LDA_ABS =	0xAD,
		INS_LDA_ABSX =	0xBD,
		INS_LDA_ABSY =	0xB9,
		INS_LDA_INDX =	0xA1,
		INS_LDA_INDY =	0xB1,
		// LDX
		INS_LDX_IM =	0xA2,
		INS_LDX_ZP =	0xA6,
		INS_LDX_ZPY =	0xB6,
		INS_LDX_ABS =	0xAE,
		INS_LDX_ABSY =	0xBE,
		// LDY
		INS_LDY_IM =	0xA0,
		INS_LDY_ZP =	0xA4,
		INS_LDY_ZPX =	0xB4,
		INS_LDY_ABS =	0xAC,
		INS_LDY_ABSX =	0xBC,
		// STA
		INS_STA_ZP =	0x85,
		INS_STA_ZPX =	0x95,
		INS_STA_ABS =	0x8D,
		INS_STA_ABSX =	0x9D,
		INS_STA_ABSY =	0x99,
		INS_STA_INDX =	0x81,
		INS_STA_INDY =	0x91,
		// STX
		INS_STX_ZP =	0x86,
		INS_STX_ZPY =	0x96,
		INS_STX_ABS =	0x8E,
		// STY
		INS_STY_ZP =	0x84,
		INS_STY_ZPX =	0x94,
		INS_STY_ABS =	0x8C,
		// Stack Operations
		INS_TSX =		0xBA,
		INS_TXS =		0x9A,
		INS_PHA =		0x48,
		INS_PHP =		0x08,
		INS_PLA =		0x68,
		INS_PLP =		0x28,
		// Transfer Register
		INS_TYA =		0x98,
		INS_TXA =		0x8A,
		INS_TAY =		0xA8,
		INS_TAX =		0xAA,
		// Incremente And Decrement
		INS_INX =		0xE8,
		INS_INY =		0xC8,
		INS_DEX =		0xCA,
		INS_DEY =		0x88,
		INS_INC_ABS =	0xEE,
		INS_INC_ABSX =	0xFE,
		INS_INC_ZP =	0xE6,
		INS_INC_ZPX =	0xF6,
		INS_DEC_ABS =	0xCE,
		INS_DEC_ABSX =	0xDE,
		INS_DEC_ZP =	0xC6,
		INS_DEC_ZPX =	0xD6,
		// AND OP
		INS_AND_IM =	0x29,
		INS_AND_ZP =	0x25,
		INS_AND_ZPX =	0x35,
		INS_AND_ABS =	0x2D,
		INS_AND_ABSX =	0x3D,
		INS_AND_ABSY =	0x39,
		INS_AND_INDX =	0x21,
		INS_AND_INDY =	0x31,
		// XOR OP
		INS_XOR_IM =	0x49,
		INS_XOR_ZP =	0x45,
		INS_XOR_ZPX =	0x55,
		INS_XOR_ABS =	0x4D,
		INS_XOR_ABSX =	0x5D,
		INS_XOR_ABSY =	0x59,
		INS_XOR_INDX =	0x41,
		INS_XOR_INDY =	0x51,
		// OR OP
		INS_OR_IM =		0x09,
		INS_OR_ZP =		0x05,
		INS_OR_ZPX =	0x15,
		INS_OR_ABS =	0x0D,
		INS_OR_ABSX =	0x1D,
		INS_OR_ABSY =	0x19,
		INS_OR_INDX =	0x01,
		INS_OR_INDY =	0x11,
		// BIT OP
		INS_BIT_ZP =	0x24,
		INS_BIT_ABS =	0x2C,
		// Branch
		INS_BEQ =		0xF0,
		INS_BNE =		0xD0,
		INS_BCS =		0xB0,
		INS_BCC =		0x90,
		INS_BVS =		0x70,
		INS_BVC =		0x50,
		INS_BMI =		0x30,
		INS_BPL =		0x10,
		// Flags
		INS_CLC =		0x18,
		INS_SEC =		0x38,
		INS_CLI =		0x58,
		INS_SEI =		0x78,
		INS_CLV =		0xB8,
		INS_CLD =		0xD8,
		INS_SED =		0xF8,
		// JSR, RTS
		INS_JSR =		0x20,
		INS_RTS =		0x60,
		INS_JMP_ABS =	0x4C,
		INS_JMP_IND =	0x6C;
		
	
	Word LoadPrg(Byte* Program, u32 nBytes, Mem& memory);

	/** Sets the correct Process status after a load register instruction
	*	- LDA, LDX, LDY
	*	@Register The A, X or Y Register */
	void LoadRegisterSetStatus(Byte Register)
	{
		PS.Flags.Z = (Register == 0);
		PS.Flags.N = (Register & 0b10000000) > 0;
	}
	
	/** @return the number of cycles that were used */
	s32 Execute(s32 Cycles, Mem& memory);

	/* Addressing mode - Zero Page */
	Word AddrZeroPage(s32& Cycles, const Mem& memory);

	/* Addressing mode - Zero Page with offset */
	Word AddrZeroPageOffset(s32& Cycles, const Mem& memory, const Byte Offset);

	/* Addressing mode - Absolute */
	Word AddrAbsolute(s32& Cycles, const Mem& memory);

	/* Addressing mode - Absolute with offset */
	Word AddrAbsoluteOffset(s32& Cycles, const Mem& memory, const Byte Offset);

	/* Addressing mode - Indirect X indexing offset */
	Word AddrIndirectX(s32& Cycles, const Mem& memory);

	/* Addressing mode - Indirect Y indexing offset */
	Word AddrIndirectY(s32& Cycles, const Mem& memory);

	/* Addressing mode - Indirect Y indexing offset for store Operations (TODO: check why is that) */
	Word AddrIndirectYStore(s32& Cycles, const Mem& memory);

	/* Addressing mode - Absolute with X offset for store operations (TODO: check why is that) */
	Word AddrAbsoluteOffsetStore(s32& Cycles, const Mem& memory, const Byte Offset);

	/* Branches on a given condition*/
	void BranchCondition(s32& Cycles, const Mem& memory, bool condition);



};
