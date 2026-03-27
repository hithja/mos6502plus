#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <cstdint>

#include "memory.hpp"
#include "opcodes.hpp"

using Byte = std::uint8_t;
using Word = std::uint16_t;
using u32  = std::uint32_t;

struct CPU {
    Word PC; // program counter
    Byte SP; // stack pointer

    Byte A, X, Y; // registers

    // Status flags 
    Byte P;
    Byte C = 1 << 0,
        Z = 1 << 1,
        I = 1 << 2,
        D = 1 << 3,
        B = 1 << 4,
        U = 1 << 5,
        V = 1 << 6,
        N = 1 << 7;

    void Reset(Mem& mem) {
        PC = mem[0xFFFC] | (mem[0xFFFD] << 8);
        SP = 0xFF;
        P = 0;
        A = X = Y = 0;
    }

    void SetFlag(Byte Flag, bool Value) {
        if (Value)
            P |= Flag;
        else
            P &= ~Flag;
    }

    void SetStatus(Byte Value) {
        SetFlag(Z, (Value == 0));
        SetFlag(N, (Value & 0b10000000) > 0);
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
                        SetStatus(A);
                    } break;
                case INS_LDA_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        A = mem[AbsAddr];
                        SetStatus(A);
                    } break;
                case INS_LDA_ZP:
                    {
                        Byte ZPAddress = FetchByte(Cycles, mem);
                        A = ReadByte(Cycles, ZPAddress, mem);
                        SetStatus(A);
                    } break;
                case INS_LDA_ZPX:
                    {
                        Byte ZPAddress = FetchByte(Cycles, mem);
                        ZPAddress += X;
                        Cycles--;
                        A = ReadByte(Cycles, ZPAddress, mem);
                        SetStatus(A);
                    } break;
                case INS_LDX_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        X = Value;
                        SetStatus(X);
                    } break;
                case INS_LDX_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        X = mem[AbsAddr];
                        SetStatus(X);
                        Cycles-=2;
                    } break;
                case INS_LDY_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        Y = Value;
                        SetStatus(Y);
                    } break;
                case INS_LDY_ABS:
                    {
                        Word AbsAddr = FetchWord(Cycles, mem);
                        Y = mem[AbsAddr];
                        SetStatus(Y);
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
                        Word Sum = A + Value + (P & C);
                        SetFlag(C, (Sum > 0xFF));
                        A = Sum & 0xFF;
                        SetStatus(A);
                    } break;
                case INS_SBC_IM:
                    {
                        Byte Value = FetchByte(Cycles, mem);
                        Word Sum = A - Value - (1 - (P & C));
                        SetFlag(C, (A >= Value + (1 - (P & C))));
                        A = Sum & 0xFF;
                        SetStatus(A);
                    } break;
                case INS_INC_ABS:
                    {
                        Word Addr = FetchWord(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value++;
                        mem[Addr] = Value;

                        SetStatus(Value);

                        Cycles -= 4;
                    } break;
                case INS_DEC_ABS:
                    {
                        Word Addr = FetchWord(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value--;
                        mem[Addr] = Value;

                        SetStatus(Value);

                        Cycles -= 4;
                    } break;
                case INS_INC_ZP:
                    {
                        Byte Addr = FetchByte(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value++;
                        mem[Addr] = Value;

                        SetStatus(Value);

                        Cycles -= 3;
                    } break;
                case INS_DEC_ZP:
                    {
                        Byte Addr = FetchByte(Cycles, mem);
                        Byte Value = mem[Addr];

                        Value--;
                        mem[Addr] = Value;

                        SetStatus(Value);

                        Cycles -= 3;
                    } break;
                case INS_INX:
                    {
                        X++;

                        SetStatus(X);
                        Cycles-=2;
                    } break;
                case INS_INY:
                    {
                        Y++;
                        
                        SetStatus(Y);
                        Cycles-=2;
                    } break;
                case INS_DEX:
                    {
                        X--;

                        SetStatus(X);
                        Cycles-=2;
                    } break;
                case INS_DEY:
                    {
                        Y--;

                        SetStatus(Y);
                        Cycles-=2;
                    } break;
                case INS_TXA: 
                    {
                        A = X; 
                        SetStatus(A);
                        Cycles--;
                    } break;
                case INS_TAX: 
                    {
                        X = A; 
                        SetStatus(X);
                        Cycles--;
                    } break;
                case INS_TYA: 
                    {
                        A = Y; 
                        SetStatus(A);
                        Cycles--;
                    } break;
                case INS_TAY: 
                    {
                        Y = A; 
                        SetStatus(Y);
                        Cycles--;
                    } break;
                case INS_CLC:
                    {
                        SetFlag(C, 0);
                        Cycles--;
                    } break;
                case INS_SEC:
                    {
                        SetFlag(C, 1);
                        Cycles--;
                    } break;
                case INS_BCS:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (P & C) {
                            if (Offset < 0x80)
                                PC += Offset;
                            else
                                PC += Offset - 0x100;
                        }
                    } break;
                case INS_BCC:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (!(P & C)) {
                            if (Offset < 0x80)
                                PC += Offset;
                            else
                                PC += Offset - 0x100;
                        }
                    } break;
                case INS_BEQ:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (P & Z) {
                            if (Offset < 0x80)
                                PC += Offset;
                            else
                                PC += Offset - 0x100;
                        }
                    } break;
                case INS_BNE:
                    {
                        Byte Offset = FetchByte(Cycles, mem);

                        if (!(P & Z)) {
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
                        SetStatus(A);
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
                        SetStatus(A);
                    } break;
                case INS_ORA_IM:
                    {
                        Byte value = FetchByte(Cycles, mem);
                        A = A | value;
                        SetStatus(A);
                    } break;
                case INS_EOR_IM:
                    {
                        Byte value = FetchByte(Cycles, mem);
                        A = A ^ value;
                        SetStatus(A);
                    } break;
                case INS_AND_ABS:
                    {
                        Word addr = FetchWord(Cycles, mem);
                        A = A & mem[addr];
                        SetStatus(A);
                        Cycles-=2;
                    } break;
                case INS_ORA_ABS:
                    {
                        Word addr = FetchWord(Cycles, mem);
                        A = A | mem[addr];
                        SetStatus(A);
                        Cycles-=2;
                    } break;
                case INS_EOR_ABS:
                    {
                        Word addr = FetchWord(Cycles, mem);
                        A = A ^ mem[addr];
                        SetStatus(A);
                        Cycles-=2;
                    } break;
                case INS_BRK:
                    {
                        mem[0x0100 + SP] = (PC+1 >> 8) & 0xFF;
                        SP--;
                        mem[0x0100 + SP] = PC & 0xFF;
                        SP--;
                        mem[0x0100 + SP] = P;
                        SP--;

                        SetFlag(B, true);

                        PC = mem[0xFFFE] | (mem[0xFFFF] << 8);
                        Cycles-=7;
                    } break;
                case INS_RTI:
                    {
                        SP++;
                        P = mem[0x0100 + SP];
                        SetFlag(B, false);

                        SP++;
                        Byte low = mem[0x0100 + SP];
                        SP++;
                        Byte high = mem[0x0100 + SP];
                        PC = ((Word)high << 8) | low;
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
                        SetStatus(X);
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
                        SetStatus(Y);
                        Cycles-=4;
                    } break;
                case INS_MIN:
                    {
                        A = (X < Y) ? X : Y;
                        Cycles--;
                        SetStatus(A);
                    } break;
                case INS_MAX:
                    {
                        A = (X > Y) ? X : Y;
                        SetStatus(A);
                        Cycles--;
                    } break;
                case INS_PRT:
                    {
                        printf("A: %02X\nX: %02X\nY: %02X\n", A, X, Y);
                    } break;
                case INS_EXT:
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
    memory.Init();
    // start
    memory[0xFFFC] = 0x40;
    memory[0xFFFD] = 0x42;
    
    memory[0x4240] = INS_LDA_IM;
    memory[0x4241] = 0x67;
    memory[0x4242] = INS_PHA;
    memory[0x4243] = INS_LDA_IM;
    memory[0x4244] = 0x68;
    memory[0x4245] = INS_PRT;
    memory[0x4246] = INS_PLA;
    memory[0x4247] = INS_PRT;
    memory[0x4248] = INS_EXT; 
    // end

    cpu.Reset(memory);
    cpu.Execute(50, memory);
    return 0;
}