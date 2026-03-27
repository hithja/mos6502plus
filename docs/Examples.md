# Examples
There are some examples of MOS6502+ code. All code is written at `main()` function. At future i'll add much more examples.

### Storing a value in A
At this example we'll write a program that stores A value and prints it.
```C++
memory[0xFFFC] = 0x40; // Low byte
memory[0xFFFD] = 0x42; // High byte

memory[0x4240] = CPU::INS_LDA_IM;
memory[0x4241] = 0x42; // Storing 0x42 value in A
memory[0x4242] = CPU::INS_PRT; // Printing A, X, Y registers
memory[0x4243] = CPU::INS_BRK; // Stop the program
```
Output:
```
A: 42
X: 00
Y: 00
```
Congratulations! You've written your first program on MOS6502+ ASM! 🎉

### Storing the maximum of X and Y in A
Now, we'll write a program that stores maximum of X and Y in A register. 
```C++
memory[0xFFFC] = 0x40; // Low byte
memory[0xFFFD] = 0x42; // High byte

memory[0x4240] = CPU::INS_LDX_IM;
memory[0x4241] = 0x42; // Storing 0x42 value in X
memory[0x4242] = CPU::INS_LDY_IM; // Storing 0x43 value in Y
memory[0x4243] = 0x43;
memory[0x4244] = CPU::INS_MAX; // Storing the maximum of X and Y in A
memory[0x4245] = CPU::INS_PRT; // Printing A, X, Y registers
memory[0x4246] = CPU::BRK; // Stop the program 
```
Output:
```
A: 43
X: 42
Y: 43
```

### Pushing A value to the stack then pulling it from here
This program will push A value to the stack, then it'll get it from the stack.
```C++
memory[0xFFFC] = 0x40; // Low byte
memory[0xFFFD] = 0x42; // High byte

memory[0x4240] = CPU::INS_LDA_IM; // Storing 0x67 value in A
memory[0x4241] = 0x67;
memory[0x4242] = CPU::INS_PHA; // Pushing A value to the stack

memory[0x4243] = CPU::INS_LDA_IM; // Storing new A value (0x68)
memory[0x4244] = 0x68;
memory[0x4245] = CPU::INS_PRT; // Printing it

memory[0x4246] = CPU::INS_PLA; // Pulling A value from the stack
memory[0x4247] = CPU::INS_PRT; // Printing

memory[0x4248] = CPU::INS_BRK; 
```
Output:
```
A: 68
X: 00
Y: 00
A: 67
X: 00
Y: 00
```

[<- Back](./Instructions.md)