#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    Byte Data[MAX_MEM];

    void Init() {
        for (u32 i = 0; i < MAX_MEM; i++)
        {
            Data[i] = 0;
        }
    }

    // read a byte
    Byte operator[](u32 Address) const {
        return Data[Address];
    }
    // write a byte
    Byte& operator[](u32 Address) {
        return Data[Address];
    }
};

struct CPU {
    Word PC; // program counter
    Word SP; // stack pointer

    Byte A, X, Y; // registers

    // Status flags 
    Byte C : 1;
    Byte Z : 1;
    Byte I : 1;
    Byte D : 1;
    Byte B : 1;
    Byte V : 1;
    Byte N : 1;

    void Reset(Mem& mem) {
        PC = 0xFFFC;
        SP = 0xFF;
        C = Z = I = D = B = V = N = 0;
        A = X = Y = 0;
        mem.Init();
    }

    static constexpr Byte 
        INS_LDA_IM = 0xA9,
        INS_LDA_ZP = 0xA5,
        INS_LDA_ZPX = 0xB5,
        INS_LDA_ABS = 0xAD,
        INS_LDX_IM = 0xA2,
        INS_LDX_ABS = 0xAE,
        INS_LDY_IM = 0xA0,
        INS_LDY_ABS = 0xAC,
        INS_STA_ABS = 0x8D,
        INS_STX_ABS = 0x8E,
        INS_STY_ABS = 0x8C,
        INS_JMP_ABS = 0x4C,
        INS_ADC_IM = 0x69,
        INS_SBC_IM = 0xE9,
        INS_DEC_ABS = 0xCE,
        INS_INC_ABS = 0xEE,
        INS_INC_ZP = 0xE6,
        INS_DEC_ZP = 0xC6,
        INS_INX = 0xE8,
        INS_DEX = 0xCA,
        INS_INY = 0xC8,
        INS_DEY = 0x88,
        INS_TXA = 0x8A,
        INS_TAX = 0xAA,
        INS_TYA = 0x98,
        INS_TAY = 0xA8,
        INS_CLC = 0x18,
        INS_SEC = 0x38,
        INS_BCC = 0x90,
        INS_BCS = 0xB0,
        INS_BEQ = 0xF0,
        INS_BNE = 0xD0,
        INS_PLA = 0x68,
        INS_PHA = 0x48,
        INS_JSR = 0x20,
        INS_RTS = 0x60,
        INS_AND_IM = 0x26,
        INS_ORA_IM = 0x09,
        INS_EOR_IM = 0x49,
        INS_AND_ABS = 0x2D,
        INS_ORA_ABS = 0x0D,
        INS_EOR_ABS = 0x4D,
        // My own opcodes
        INS_PLX = 0x2B,
        INS_PHX = 0x3B,
        INS_PLY = 0x4B,
        INS_PHY = 0x5B,
        INS_MIN = 0x0F,
        INS_MAX = 0x1F,
        INS_PRT = 0xA7,
        INS_BRK = 0x02;

    void LDASetStatus() {
        Z = (A == 0);
        N = (A & 0b10000000) > 0;
    }

    Byte FetchByte(u32& Cycles, Mem& mem) {
        Byte Data = mem[PC];
        PC++;
        Cycles--;

        return Data;
    }

    Word FetchWord(u32& Cycles, Mem& mem) {
        Word Data = mem[PC];
        PC++;

        Data |= (mem[PC] << 8);
        PC++;
        Cycles-=2;

        return Data;
    }

    Byte ReadByte(u32& Cycles, Byte Addr, Mem& mem) {
        Byte Data = mem[Addr];
        Cycles--;

        return Data;
    }

    void Execute(u32 Cycles, Mem& mem) {
        while (Cycles > 0) {
            Byte Ins = FetchByte(Cycles, mem);
            
            switch (Ins) {
                case INS_LDA_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        A = Value;
                        LDASetStatus();
                    } break;
                case INS_LDA_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        A = mem[AbsAddr];
                        LDASetStatus();
                    } break;
                case INS_LDA_ZP:
                    {
                        Byte ZPAddress = FetchByte(Cycles, mem);
                        A = ReadByte(Cycles, ZPAddress, mem);
                        LDASetStatus();
                    } break;
                case INS_LDA_ZPX:
                    {
                        Byte ZPAddress = FetchByte(Cycles, mem);
                        ZPAddress += X;
                        Cycles--;
                        A = ReadByte(Cycles, ZPAddress, mem);
                        LDASetStatus();
                    } break;
                case INS_LDX_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        X = Value;
                        Z = (X == 0);
                        N = (X & 0b10000000) > 0;
                    } break;
                case INS_LDX_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        X = mem[AbsAddr];
                        Z = (X == 0);
                        N = (X & 0b10000000) > 0;
                        Cycles-=2;
                    } break;
                case INS_LDY_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        Y = Value;
                        Z = (Y == 0);
                        N = (Y & 0b10000000) > 0;
                    } break;
                case INS_LDY_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        Y = mem[AbsAddr];
                        Z = (Y == 0);
                        N = (Y & 0b10000000) > 0; 
                        Cycles-=2;                       
                    } break;
                case INS_STA_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        mem[AbsAddr] = A;
                        Cycles-=2;
                    } break;
                case INS_STX_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        mem[AbsAddr] = X;
                        Cycles--;
                    } break;
                case INS_STY_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        mem[AbsAddr] = Y;
                        Cycles--;
                    } break;
                case INS_JMP_ABS:
                    {
                        Word SubAddr = FetchWord(Cycles, mem);
                        PC = SubAddr;
                        Cycles--;
                    } break;
                case INS_ADC_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        Word Sum = A + Value + C;
                        C = (Sum > 0xFF);
                        A = Sum & 0xFF;
                        LDASetStatus();
                    } break;
                case INS_SBC_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        Word Sum = A - Value - (1 - C);
                        C = (A >= Value + (1 - C));
                        A = Sum & 0xFF;
                        LDASetStatus();
                    } break;
                case INS_INC_ABS:
                    {
                        Word Addr = FetchWord(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value++;
                        mem[Addr] = Value;

                        Z = (Value == 0);
                        N = (Value & 0b10000000) > 0;

                        Cycles -= 4;
                    } break;
                case INS_DEC_ABS:
                    {
                        Word Addr = FetchWord(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value--;
                        mem[Addr] = Value;

                        Z = (Value == 0);
                        N = (Value & 0b10000000) > 0;

                        Cycles -= 4;
                    } break;
                case INS_INC_ZP:
                    {
                        Byte Addr = FetchByte(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value++;
                        mem[Addr] = Value;

                        Z = (Value == 0);
                        N = (Value & 0b10000000) > 0;

                        Cycles -= 3;
                    } break;
                case INS_DEC_ZP:
                    {
                        Byte Addr = FetchByte(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value--;
                        mem[Addr] = Value;

                        Z = (Value == 0);
                        N = (Value & 0b10000000) > 0;

                        Cycles -= 3;
                    } break;
                case INS_INX:
                    {
                        X++;

                        Z = (X == 0);
                        N = (X & 0b10000000) > 0;

                        Cycles-=2;
                    } break;
                case INS_INY:
                    {
                        Y++;
                        Z = (Y == 0);
                        N = (Y & 0b10000000) > 0;
                        Cycles-=2;
                    } break;
                case INS_DEX:
                    {
                        X--;

                        Z = (X == 0);
                        N = (X & 0b10000000) > 0;

                        Cycles-=2;
                    } break;
                case INS_DEY:
                    {
                        Y--;
                        Z = (Y == 0);
                        N = (Y & 0b10000000) > 0;
                        Cycles-=2;
                    } break;
                case INS_TXA: 
                    {
                        A = X; 
                        LDASetStatus(); 
                        Cycles--;
                    } break;
                case INS_TAX: 
                    {
                        X = A; 
                        Z = (X==0); N = (X&0x80); 
                        Cycles--;
                    } break;
                case INS_TYA: 
                    {
                        A = Y; 
                        LDASetStatus(); 
                        Cycles--;
                    } break;
                case INS_TAY: 
                    {
                        Y = A; 
                        Z = (Y==0); N = (Y&0x80); 
                        Cycles--;
                    } break;
                case INS_CLC:
                    {
                        C = 0;
                        Cycles--;
                    } break;
                case INS_SEC:
                    {
                        C = 1;
                        Cycles--;
                    } break;
                case INS_BCS:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (C == 1) {
                            if (Offset < 0x80)
                                PC += Offset;
                            else
                                PC += Offset - 0x100;
                        }
                    } break;
                case INS_BCC:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (C == 0) {
                            if (Offset < 0x80)
                                PC += Offset;
                            else
                                PC += Offset - 0x100;
                        }
                    } break;
                case INS_BEQ:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (Z == 1) {
                            if (Offset < 0x80)
                                PC += Offset;
                            else
                                PC += Offset - 0x100;
                        }
                    } break;
                case INS_BNE:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (Z == 0) {
                            if (Offset < 0x80)
                                PC += Offset;
                            else
                                PC += Offset - 0x100;
                        }
                    } break;
                case INS_PHA:
                    {
                        mem[0x0100+SP] = A;
                        SP--;
                        Cycles-=3;
                    } break;
                case INS_PLA:
                    {
                        SP++;
                        A = mem[0x0100 + SP];
                        LDASetStatus();
                        Cycles-=4;
                    } break;
                case INS_JSR:
                    {
                        Word Adrr = FetchWord(Cycles, mem);
                        mem[0x0100 + SP] = ((PC-1) >> 8) & 0xFF;
                        SP--;
                        mem[0x0100 + SP] = (PC-1) & 0xFF;
                        SP--;
                        Cycles-=4;
                    } break;
                case INS_RTS:
                    {   
                        SP++;
                        Byte low = mem[0x0100 + SP];
                        SP++;
                        Byte high = mem[0x0100 + SP];
                        PC = ((Word)high << 8) | low;
                        PC++;
                    } break;
                case INS_AND_IM:
                    {
                        Byte value = FetchByte(Cycles, mem);
                        A = A & value;
                        LDASetStatus();
                    } break;
                case INS_ORA_IM:
                    {
                        Byte value = FetchByte(Cycles, mem);
                        A = A | value;
                        LDASetStatus();
                    } break;
                case INS_EOR_IM:
                    {
                        Byte value = FetchByte(Cycles, mem);
                        A = A ^ value;
                        LDASetStatus();
                    } break;
                case INS_AND_ABS:
                    {
                        Word addr = FetchWord(Cycles, mem);
                        A = A & mem[addr];
                        LDASetStatus();
                        Cycles-=2;
                    } break;
                case INS_ORA_ABS:
                    {
                        Word addr = FetchWord(Cycles, mem);
                        A = A | mem[addr];
                        LDASetStatus();
                        Cycles-=2;
                    } break;
                case INS_EOR_ABS:
                    {
                        Word addr = FetchWord(Cycles, mem);
                        A = A ^ mem[addr];
                        LDASetStatus();
                        Cycles-=2;
                    } break;
                case INS_PHX:
                    {
                        mem[0x0100+SP] = X;
                        SP--;
                        Cycles-=3;
                    } break;
                case INS_PLX:
                    {
                        SP++;
                        X = mem[0x0100 + SP];
                        Z = (X == 0);
                        N = (X & 0b10000000) > 0;
                        Cycles-=4;
                    } break;
                case INS_PHY:
                    {
                        mem[0x0100+SP] = Y;
                        SP--;
                        Cycles-=3;
                    } break;
                case INS_PLY:
                    {
                        SP++;
                        Y = mem[0x0100 + SP];
                        Z = (Y == 0);
                        N = (Y & 0b10000000) > 0;
                        Cycles-=4;
                    } break;
                case INS_MIN:
                    {
                        A = (X < Y) ? X : Y;
                        Cycles--;
                        LDASetStatus();
                    } break;
                case INS_MAX:
                    {
                        A = (X > Y) ? X : Y;
                        LDASetStatus();
                        Cycles--;
                    } break;
                case INS_PRT:
                    {
                        printf("A: %02X\nX: %02X\nY: %02X\n", A, X, Y);
                    } break;
                case INS_BRK:
                    {
                        exit(0);
                    } break;
                default:
                    {
                        printf("Instruction not given %d \n", Ins);
                    } break;
            }
        }
    }
};

int main() {
    Mem memory;
    CPU cpu;
    cpu.Reset(memory);
    // start
    memory[0xFFFC] = CPU::INS_JMP_ABS;
    memory[0xFFFD] = 0x40;
    memory[0xFFFE] = 0x42;

    memory[0x4240] = CPU::INS_LDA_IM;
    memory[0x4241] = 0x67;
    memory[0x4242] = CPU::INS_PHA;
    memory[0x4243] = CPU::INS_LDA_IM;
    memory[0x4244] = 0x68;
    memory[0x4245] = CPU::INS_PRT;
    memory[0x4246] = CPU::INS_PLA;
    memory[0x4247] = CPU::INS_PRT;
    memory[0x4248] = CPU::INS_BRK; 
    
    // end
    cpu.Execute(50, memory);
    return 0;
}