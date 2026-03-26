# About the CPU
The MOS6502+ is an 8-bit CPU with ~50 instructions, 64Kb of memory and 5 main registers.
## The registers and status flags tables
### Registers
| Name | Function |
|---|---|
| A | Accumulator |
| X | Index register |
| Y | Index register |
| SP | Stack Pointer |
| PC (16-bit) | Program Counter |
| C, Z, I, D, B, V, N | Status flags |
### Status Flags
| Short Name | Full Name | Description |
|---|---|---|
| C | Carry | Set if an operation causes a carry |
| Z | Zero | Set if an operation result is zero |
| I | Interrupt |
| D | Decimal | Set when CPU is in Binary Coded Decimal  mode | Disables IRQ interrupts when set (1) |
| B | Break | Set when a BRK instruction is executed |
| V | Overflow | 	Set if a signed arithmetic operation overflows (+127/ -128) |
| N | Negative | Set if bit 7 of a result is 1 |

[<- Back](./Contents.md) +==+ [Next ->](./Instructions.md)