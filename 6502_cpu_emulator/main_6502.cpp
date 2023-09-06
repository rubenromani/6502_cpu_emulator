#include "main_6502.h"

m6502::Word m6502::CPU::AddrZeroPage(s32& Cycles, const Mem& memory)
{
	Byte ZeroPageAddr = FetchByte(Cycles, memory);
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrZeroPageOffset(s32& Cycles, const Mem& memory, Byte Offset)
{
	Byte ZeroPageAddr = FetchByte(Cycles, memory);
	ZeroPageAddr += Offset;
	Cycles--;
	return ZeroPageAddr;
}

m6502::Word m6502::CPU::AddrAbsolute(s32& Cycles, const Mem& memory)
{
	Word AbsAddr = FetchWord(Cycles, memory);
	return AbsAddr;
}

m6502::Word m6502::CPU::AddrAbsoluteOffset(s32& Cycles, const Mem& memory, Byte Offset)
{
	Word AbsAddr = FetchWord(Cycles, memory);
	Word AbsAddrX = AbsAddr + Offset;
	if ((AbsAddrX & 0x00FF) < (AbsAddr & 0x00FF))
	{
		Cycles--;
	}
	return AbsAddrX;
}

m6502::Word m6502::CPU::AddrIndirectX(s32& Cycles, const Mem& memory)
{
	Byte ZPAddress = FetchByte(Cycles, memory);
	Byte ZPAddressX = ZPAddress + X;
	Cycles--;
	Word EffectiveAddress = ReadWord(Cycles, ZPAddressX, memory);
	return EffectiveAddress;
}

m6502::Word m6502::CPU::AddrIndirectY(s32& Cycles, const Mem& memory)
{
	Word ZPAddress = FetchByte(Cycles, memory);
	Word EffectiveAddress = ReadWord(Cycles, ZPAddress, memory);
	Word EffectiveAddressY = EffectiveAddress + Y;
	if ((EffectiveAddressY & 0x00FF) < (EffectiveAddress & 0x00FF))
	{
		Cycles--;
	}
	return EffectiveAddressY;
}

m6502::Word m6502::CPU::AddrAbsoluteOffsetStore(s32& Cycles, const Mem& memory, Byte Offset)
{
	Word AbsAddr = FetchWord(Cycles, memory);
	Word AbsAddrX = AbsAddr + Offset;
	Cycles--;
	return AbsAddrX;
}

m6502::Word m6502::CPU::AddrIndirectYStore(s32& Cycles, const Mem& memory)
{
	Word ZPAddress = FetchByte(Cycles, memory);
	Word EffectiveAddress = ReadWord(Cycles, ZPAddress, memory);
	Word EffectiveAddressY = EffectiveAddress + Y;
	Cycles--;
	return EffectiveAddressY;
}

void m6502::CPU::BranchCondition(s32& Cycles, const Mem& memory, bool condition)
{
	Byte Offset = FetchByte(Cycles, memory);
	if (condition) {
		Word Address = PC + static_cast<sByte>(Offset);
		if ((Address & 0xFF00) != (PC & 0xFF00))
		{
			Cycles -= 2;
		}
		PC = Address;
		Cycles--;
	}
}

m6502::Word m6502::CPU::LoadPrg(Byte* Program, u32 nBytes, Mem& memory)
{
	m6502::Word LoadAddress = 0x0000;
	if (Program && nBytes >  2)
	{
		u32 At = 0x00;
		LoadAddress = ((m6502::Word)Program[At]) | (((m6502::Word)Program[At+1]) << 8);
		At += 2;
		for(int i = LoadAddress; i < LoadAddress+nBytes-2; i++)
		{
			memory[i] = Program[At++];
		}
	}
	return LoadAddress;

}

/* @return the number of cycles that were used */
m6502::s32 m6502::CPU::Execute(s32 Cycles, Mem& memory)
{
	auto LoadRegister = [this, &Cycles, &memory](Word Address, Byte& Register)
	{
		Register = ReadByte(Cycles, Address, memory);
		LoadRegisterSetStatus(Register);
	};

	const u32 CycleRequested = Cycles;
	while (Cycles > 0)
	{
		Byte Ins = FetchByte(Cycles, memory);
		switch (Ins)
		{
		case INS_LDA_IM:
		{
			A = FetchByte(Cycles, memory);
			LoadRegisterSetStatus(A);
		} break;
		case INS_LDX_IM:
		{
			X = FetchByte(Cycles, memory);
			LoadRegisterSetStatus(X);
		} break;
		case INS_LDY_IM:
		{
			Y = FetchByte(Cycles, memory);
			LoadRegisterSetStatus(Y);
		} break;
		case INS_LDA_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			LoadRegister(Address, A);
		}break;
		case INS_LDX_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			LoadRegister(Address, X);
		}break;
		case INS_LDY_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			LoadRegister(Address, Y);
		}break;
		case INS_LDA_ZPX:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, X);
			LoadRegister(Address, A);
		}break;
		case INS_LDX_ZPY:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, Y);
			LoadRegister(Address, X);
		}break;
		case INS_LDY_ZPX:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, X);
			LoadRegister(Address, Y);
		}break;
		case INS_LDA_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			LoadRegister(Address, A);
		}break;
		case INS_LDX_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			LoadRegister(Address, X);
		}break;
		case INS_LDY_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			LoadRegister(Address, Y);
		}break;
		case INS_LDA_ABSX:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, X);
			LoadRegister(Address, A);
		}break;
		case INS_LDA_ABSY:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, Y);
			LoadRegister(Address, A);
		}break;
		case INS_LDX_ABSY:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, Y);
			LoadRegister(Address, X);
		}break;
		case INS_LDY_ABSX:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, X);
			LoadRegister(Address, Y);
		}break;
		case INS_LDA_INDX:
		{
			Word Address = AddrIndirectX(Cycles, memory);
			LoadRegister(Address, A);
		}break;
		case INS_LDA_INDY:
		{
			Word Address = AddrIndirectY(Cycles, memory);
			LoadRegister(Address, A);
		}break;
		case INS_STA_ZP:
		{
			Byte Address = AddrZeroPage(Cycles, memory);
			WriteByte(A, Address, Cycles, memory);
		}break;
		case INS_STA_ZPX:
		{
			Byte Address = AddrZeroPageOffset(Cycles, memory, X);
			WriteByte(A, Address, Cycles, memory);
		}break;
		case INS_STA_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			WriteByte(A, Address, Cycles, memory);
		}break;
		case INS_STA_ABSX:
		{
			Word Address = AddrAbsoluteOffsetStore(Cycles, memory, X);
			WriteByte(A, Address, Cycles, memory);
		}break;
		case INS_STA_ABSY:
		{
			Word Address = AddrAbsoluteOffsetStore(Cycles, memory, Y);
			WriteByte(A, Address, Cycles, memory);
		}break;
		case INS_STX_ZP:
		{
			Byte Address = AddrZeroPage(Cycles, memory);
			WriteByte(X, Address, Cycles, memory);
		}break;
		case INS_STX_ZPY:
		{
			Byte Address = AddrZeroPageOffset(Cycles, memory, Y);
			WriteByte(X, Address, Cycles, memory);
		}break;
		case INS_STY_ZP:
		{
			Byte Address = AddrZeroPage(Cycles, memory);
			WriteByte(Y, Address, Cycles, memory);
		}break;
		case INS_STY_ZPX:
		{
			Byte Address = AddrZeroPageOffset(Cycles, memory, X);
			WriteByte(Y, Address, Cycles, memory);
		}break;
		case INS_STX_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			WriteByte(X, Address, Cycles, memory);
		}break;
		case INS_STY_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			WriteByte(Y, Address, Cycles, memory);
		}break;
		case INS_STA_INDX:
		{
			Word Address = AddrIndirectX(Cycles, memory);
			WriteByte(A, Address, Cycles, memory);
		}break;
		case INS_STA_INDY:
		{
			Word Address = AddrIndirectYStore(Cycles, memory);
			WriteByte(A, Address, Cycles, memory);
		}break;
		case INS_TSX:
		{
			X = SP;
			LoadRegisterSetStatus(X);
			Cycles--;
		}break;
		case INS_TXS:
		{
			SP = X;
			LoadRegisterSetStatus(SP);
			Cycles--;
		}break;
		case INS_TYA:
		{
			A = Y;
			LoadRegisterSetStatus(A);
			Cycles--;
		}break;
		case INS_TAY:
		{
			Y = A;
			LoadRegisterSetStatus(Y);
			Cycles--;
		}break;
		case INS_TXA:
		{
			A = X;
			LoadRegisterSetStatus(A);
			Cycles--;
		}break;
		case INS_TAX:
		{
			X = A;
			LoadRegisterSetStatus(X);
			Cycles--;
		}break;
		case INS_PHA:
		{
			PushByteOnTheStack(A, Cycles, memory);
		}break;
		case INS_PHP:
		{
			PushByteOnTheStack(PS.Reg, Cycles, memory);
		}break;
		case INS_PLA:
		{
			A = PopByteFromStack(Cycles, memory);
			LoadRegisterSetStatus(A);
		}break;
		case INS_PLP:
		{
			PS.Reg = PopByteFromStack(Cycles, memory);
		}break;
		case INS_INX:
		{
			X += 1;
			LoadRegisterSetStatus(X);
			Cycles--;
		}break;
		case INS_INY:
		{
			Y += 1;
			LoadRegisterSetStatus(Y);
			Cycles--;
		}break;
		case INS_DEX:
		{
			X -= 1;
			LoadRegisterSetStatus(X);
			Cycles--;
		}break;
		case INS_DEY:
		{
			Y -= 1;
			LoadRegisterSetStatus(Y);
			Cycles--;
		}break;
		case INS_INC_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			memory[Address] += 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_INC_ABSX:
		{
			Word Address = AddrAbsoluteOffsetStore(Cycles, memory, X);
			memory[Address] += 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_INC_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			memory[Address] += 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_INC_ZPX:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, X);
			memory[Address] += 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_DEC_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			memory[Address] -= 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_DEC_ABSX:
		{
			Word Address = AddrAbsoluteOffsetStore(Cycles, memory, X);
			memory[Address] -= 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_DEC_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			memory[Address] -= 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_DEC_ZPX:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, X);
			memory[Address] -= 1;
			LoadRegisterSetStatus(memory[Address]);
			Cycles -= 3;
		}break;
		case INS_AND_IM:
		{
			Byte Value = FetchByte(Cycles, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_AND_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_AND_ZPX:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, X);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_AND_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_AND_ABSX:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, X);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_AND_ABSY:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, Y);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_AND_INDX:
		{
			Word Address = AddrIndirectX(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_AND_INDY:
		{
			Word Address = AddrIndirectY(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A & Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_IM:
		{
			Byte Value = FetchByte(Cycles, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_ZPX:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, X);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_ABSX:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, X);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_ABSY:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, Y);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_INDX:
		{
			Word Address = AddrIndirectX(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_XOR_INDY:
		{
			Word Address = AddrIndirectY(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A ^ Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_IM:
		{
			Byte Value = FetchByte(Cycles, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_ZPX:
		{
			Word Address = AddrZeroPageOffset(Cycles, memory, X);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_ABSX:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, X);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_ABSY:
		{
			Word Address = AddrAbsoluteOffset(Cycles, memory, Y);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_INDX:
		{
			Word Address = AddrIndirectX(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_OR_INDY:
		{
			Word Address = AddrIndirectY(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			A = (A | Value);
			LoadRegisterSetStatus(A);
		}break;
		case INS_BIT_ZP:
		{
			Word Address = AddrZeroPage(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			Byte Result = (A & Value);
			PS.Flags.Z = (Result == 0x00);
			PS.Flags.N = ((Value & 0b10000000) == 0b10000000);
			PS.Flags.V = ((Value & 0b01000000) == 0b01000000);
		}break;
		case INS_BIT_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			Byte Value = ReadByte(Cycles, Address, memory);
			Byte Result = (A & Value);
			PS.Flags.Z = (Result == 0x00);
			PS.Flags.N = ((Value & 0b10000000) == 0b10000000);
			PS.Flags.V = ((Value & 0b01000000) == 0b01000000);
		}break;
		case INS_BEQ:
		{
			BranchCondition(Cycles, memory, PS.Flags.Z == 1);
		}break;
		case INS_BNE:
		{
			BranchCondition(Cycles, memory, PS.Flags.Z == 0);
		}break;
		case INS_BPL:
		{
			BranchCondition(Cycles, memory, PS.Flags.N == 0);
		}break;
		case INS_BMI:
		{
			BranchCondition(Cycles, memory, PS.Flags.N == 1);
		}break;
		case INS_BVC:
		{
			BranchCondition(Cycles, memory, PS.Flags.V == 0);
		}break;
		case INS_BVS:
		{
			BranchCondition(Cycles, memory, PS.Flags.V == 1);
		}break;
		case INS_BCC:
		{
			BranchCondition(Cycles, memory, PS.Flags.C == 0);
		}break;
		case INS_BCS:
		{
			BranchCondition(Cycles, memory, PS.Flags.C == 1);
		}break;
		case INS_JSR:
		{
			Word SubAddr = FetchWord(Cycles, memory);
			PushPCToStack(Cycles, memory);
			PC = SubAddr;
			Cycles --;
		}break;
		case INS_RTS:
		{
			Word ReturnAddress = PopWordFromStack(Cycles, memory);
			PC = ReturnAddress;
			Cycles -= 2;
		}break;
		case INS_JMP_ABS:
		{
			Word Address = AddrAbsolute(Cycles, memory);
			PC = Address;
		}break;
		case INS_JMP_IND:
		{
			Word IndAddress = AddrAbsolute(Cycles, memory);
			Word Address = ReadWord(Cycles, IndAddress, memory);
			PC = Address;
		}break;
		default:
		{
			printf("Intruction not handled, Ins: %d\tCycles: %d\n", Ins, Cycles);
			throw - 1;
		}break;
		}
	}
	const s32 NumCyclesUsed = CycleRequested - Cycles;
	return NumCyclesUsed;
}