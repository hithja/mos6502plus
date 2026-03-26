# Instructions
There are about 40 instructions and 50 opcodes.

### Address modes
| Short name | Full name | Description |
|---|---|---|
| IM | immediate | Operand is a byte |
| ZP | zero page | Operand is address in $00-$FF |
| ZPX | zero page,X | Zero page address + X register |
| ABS | absolute | Full 16-bit address |
| IMPL | implied | No operand, instruction implied |

### Instructions and Opcodes
| Instruction | Address mode | Opcode | Description |
|---|---|---|---|
| LDA | IM | 0xA9 | Load A with immediate value |
| LDA | ZP | 0xA5 | Load A from zero page |
| LDA | ZPX | 0xB5 | Load A from zero page + X |
| LDA | ABS | 0xAD | Load A from absolute address |
| LDX | IM | 0xA2 | Load X with immediate value |
| LDX | ABS | 0xAE | Load X from absolute address |
| LDY | IM | 0xA0 | Load Y with immediate value |
| LDY | ABS | 0xAC | Load Y from absolute address |
| STA | ABS | 0x8D | Store A to absolute address |
| STX | ABS | 0x8E | Store X to absolute address |
| STY | ABS | 0x8C | Store Y to absolute address |
| JMP | ABS | 0x4C | Jump to absolute address |
| ADC | IM | 0x69 | Add with carry immediate |
| SBC | IM | 0xE9 | Subtract with carry immediate |
| DEC | ABS | 0xCE | Decrement memory at absolute address |
| INC | ABS | 0xEE | Increment memory at absolute address |
| INC | ZP | 0xE6 | Increment memory at zero page |
| DEC | ZP | 0xC6 | Decrement memory at zero page |
| INX | IMPL | 0xE8 | Increment X register |
| DEX | IMPL | 0xCA | Decrement X register |
| INY | IMPL | 0xC8 | Increment Y register |
| DEY | IMPL | 0x88 | Decrement Y register |
| TXA | IMPL | 0x8A | Transfer X to A |
| TAX | IMPL | 0xAA | Transfer A to X |
| TYA | IMPL | 0x98 | Transfer Y to A |
| TAY | IMPL | 0xA8 | Transfer A to Y |
| CLC | IMPL | 0x18 | Clear carry flag |
| SEC | IMPL | 0x38 | Set carry flag |
| BCC | IMPL | 0x90 | Branch if carry clear |
| BCS | IMPL | 0xB0 | Branch if carry set |
| BEQ | IMPL | 0xF0 | Branch if equal (zero set) |
| BNE | IMPL | 0xD0 | Branch if not equal (zero clear) |
| PLA | IMPL | 0x68 | Pull accumulator from stack |
| PHA | IMPL | 0x48 | Push accumulator to stack |
| JSR | ABS | 0x20 | Jump to subroutine |
| RTS | IMPL | 0x60 | Return from subroutine |
| AND | IM | 0x26 | AND |
| ORA | IM | 0x09 | OR |
| EOR | IM | 0x49 | XOR |
| AND | ABS | 0x2D | AND |
| ORA | ABS | 0x0D | OR |
| EOR | ABS | 0x4D | XOR |
| PLX | IMPL | 0x2B | Pull X from stack |
| PHX | IMPL | 0x3B | Push X to stack |
| PLY | IMPL | 0x4B | Pull Y from stack |
| PHY | IMPL | 0x5B | Push Y to stack |
| MIN | IMPL | 0x0F | Store min(X,Y) in A |
| MAX | IMPL | 0x1F | Store max(X,Y) in A |
| PRT | IMPL | 0xA7 | Print A, X and Y |
| BRK | IMPL | 0x02 | Break / stop |

[<- Back](./About.md) +==+ [Next ->](./Examples.md)