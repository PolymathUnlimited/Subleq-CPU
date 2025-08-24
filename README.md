# Subleq-CPU
This repository contains files relating to my subleq processor build.

To use the processor, simply load the .circ file into Logisim (make sure that ticks are enabled and the tick rate is set to the desired frequency).  Then, turn on the "reset" switch (located on the bottom right, near RAM), and enter (or paste) a machine code program into RAM.  Then, to run the program, simply turn off the "reset" switch.  Both the output register and the teletype display are memory-mapped to memory address 0xff.

Also included is an assembler I wrote to assemble programs for this processor (so that you don't have to write machine code programs directly).  The assembler is limited, but it works well enough.  I provided the raw C++ file for the assembler, so just copy and paste it into the compiler of your choice.  Before running the assembler, make sure you make a file in the same directory as the assembler called "code.asm" and put your assembly code in that file.  When you run the assembler, "code.asm" will get assembled and the hexadecimal machine code will get written to the console.  You can then copy and paste it into the Logisim circuit's RAM block.

I have included the assembly code for a "hello world" program.  This program is designed to work with a standard 1602 LCD display, and it also works with the logisim circuit.
