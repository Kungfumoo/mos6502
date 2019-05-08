=== COMPILING ASM ===
- install the xa65 package
- run: 'xa -o test.bin -bt 1536 <name>.asm'
	- '-bt 1536' compiles the program to start at address 0x0600 (which is where my emulator starts)
		- it compiles at 0x1000 by default
