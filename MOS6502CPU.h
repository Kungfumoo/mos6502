//Author: Aaron Winter
//Date: 7/9/2014
/*Description:
 *Class acting as the MOS 6502 CPU.
 *Emulation is done via interpretation.
 */

#include <string>

typedef unsigned char byte;

#ifndef _MOS_6502_CPU_H_
#define _MOS_6502_CPU_H_
namespace MOS_6502
{
    //forward decs
    class StatusRegister;
    class Memory;
    class Compiler;
    class Stack;

    class MOS6502CPU
    {
        //Constants
        static byte MAX_CLOCK_SPEED_MHZ;
        static byte NEGATIVE; //represents the number where the CPU starts interpreting as negative

        //Registers
        unsigned short _programCounter;
        byte _accumulator;
        byte _x;
        byte _y;
        StatusRegister* _status; //initial state 00100000
        byte _stackPointer;

        //Memory
        Memory* _memory;
        Stack* _stack;

        //Variables
        bool _running;
        unsigned int _clockSpeed;
        Compiler* _compiler;
        unsigned int _cycles;
        byte* _cycleLookup; //array storing the cycles that each procedure should take
        bool _debug;

        //--STATE METHODS
        void saveProgramCounter(); //pushes program counter to stack
        void getProgramCounter(); //retreives program counter and restores it
        void saveCurrentState(); //will save the current state of the status reg and program counter and push it on the stack
        void getLastState(); //will obtain the last state of the above and restore it

        //--ADDRESSING MODE METHODS
        unsigned short getAbsolute(); //will get an absolute address based on the next two bytes from the program counter.
        unsigned short getPreIndirect(); //will get a pre-indexed indirect address from the next byte from the program counter.
        unsigned short getPostIndirect(); //will get a post-indexed indirext address from the next byte from the pc
        unsigned short getZeroPageIndexed(byte regValue); //will get a zero-page indexed address from the next byte from the program counter.

        /*Ok so relative...
         *Basically used for all branch instructions, to work out the start of the next 'branch'
         *The value passed is treated as a sign number, eg -127 ~ 127.
         *If the value treated is a negative number, then 'add' that value to the pc, which will take it back x spots
         *Same for if its a positive, add it, and it will move upto x spots.
         *This is displacement.
         *This uses SIGNED MAGNITUDE, eg 1011 = -3; 0011 = +3
         */
        unsigned short getRelative(byte value);

        //--ASSEMBLY PROCEDURES(private). Formatted as <operation><addressing mode>
        /*Addressing Modes:
         * 1 - Immediate
         * 2 - Absolute
         * 3 - zeropage
         * 4 - implied
         * 5 - accumulator
         * 6 - indexed - eg: LDA $0000, Y; addressing: absoulute, <reg>
         * 7 - zeropage indexed - eg: LDA $00, X; addressing: zeropage, <reg>
         * 8 - indirect
         * 9 - pre-indexed indrect - eg: LDA ($3E, X)
         * 10 - post-indexed indirect - eg: LDA($4C), Y
         * 11 - relative
         *
         *
         *Legend:
            -*add 1 to cycles if page boundery is crossed

            -** add 1 to cycles if branch occurs on same page
                add 2 to cycles if branch occurs to different page


             Legend to Flags:  + .... modified
                               - .... not modified
                               1 .... set
                               0 .... cleared
                              M6 .... memory bit 6
                              M7 .... memory bit 7
         */

        /*
             ADC  Add Memory to Accumulator with Carry

             A + M + C -> A, C                N Z C I D V
                                              + + + - - +

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     ADC #oper     69    2     2
             zeropage      ADC oper      65    2     3
             zeropage,X    ADC oper,X    75    2     4
             absolute      ADC oper      6D    3     4
             absolute,X    ADC oper,X    7D    3     4*
             absolute,Y    ADC oper,Y    79    3     4*
             (indirect,X)  ADC (oper,X)  61    2     6
             (indirect),Y  ADC (oper),Y  71    2     5*

             1 3 7 2 6 6 9 10

             AARON NOTES:
             -Add given operand to what is currently loaded in the accumilator

             Notes on BCD:
                BCD is where 4 bits represents a digit in a number, eg 0001-0001 = 11
         */
         void ADC(byte operand); //Method that declares basic ADC logic used by all versions
         void ADC1(); //TODO & NOTE: Operands are obtained from WITHIN the method.
         void ADC3();
         void ADC7();
         void ADC2();
         void ADC6_X();
         void ADC6_Y();
         void ADC9();
         void ADC10();

         /*
             AND  AND Memory with Accumulator

             A AND M -> A                     N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     AND #oper     29    2     2
             zeropage      AND oper      25    2     3
             zeropage,X    AND oper,X    35    2     4
             absolute      AND oper      2D    3     4
             absolute,X    AND oper,X    3D    3     4*
             absolute,Y    AND oper,Y    39    3     4*
             (indirect,X)  AND (oper,X)  21    2     6
             (indirect),Y  AND (oper),Y  31    2     5*

             1 3 7 2 6 6 9 10
         */

         void AND(byte operand);
         void AND1();
         void AND3();
         void AND7();
         void AND2();
         void AND6_X();
         void AND6_Y();
         void AND9();
         void AND10();

         /*
             ASL  Shift Left One Bit (Memory or Accumulator)

             C <- [76543210] <- 0             N Z C I D V
                                              + + + - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             accumulator   ASL A         0A    1     2
             zeropage      ASL oper      06    2     5
             zeropage,X    ASL oper,X    16    2     6
             absolute      ASL oper      0E    3     6
             absolute,X    ASL oper,X    1E    3     7

             5 3 7 2 6
         */

         void ASL(byte& operand); //reference to the operand, not the actual operand itself(as it performs bit shift on the value)
         void ASL5();
         void ASL3();
         void ASL7();
         void ASL2();
         void ASL6();

         /*
             BCC  Branch on Carry Clear

             branch on C = 0                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BCC oper      90    2     2**

             11
         */

         void BCC11();

         /*
             BCS  Branch on Carry Set

             branch on C = 1                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BCS oper      B0    2     2**

             11
         */

         void BCS11();

         /*
             BEQ  Branch on Result Zero

             branch on Z = 1                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BEQ oper      F0    2     2**

             11
         */

         void BEQ11();

         /*
             BIT  Test Bits in Memory with Accumulator

             bits 7 and 6 of operand are transfered to bit 7 and 6 of SR (N,V);
             the zeroflag is set to the result of operand AND accumulator.

             A AND M, M7 -> N, M6 -> V        N Z C I D V
                                             M7 + - - - M6

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             zeropage      BIT oper      24    2     3
             absolute      BIT oper      2C    3     4

             3 2
         */

         void BIT(byte operand);
         void BIT3();
         void BIT2();

         /*
             BMI  Branch on Result Minus

             branch on N = 1                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BMI oper      30    2     2**

             11
         */

         void BMI11();

         /*
             BNE  Branch on Result not Zero

             branch on Z = 0                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BNE oper      D0    2     2**

             11
         */

         void BNE11();

         /*
             BPL  Branch on Result Plus

             branch on N = 0                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BPL oper      10    2     2**

             11
         */

         void BPL11();

         /*
             BRK  Force Break

             interrupt,                       N Z C I D V
             push PC+2, push SR               - - - 1 - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       BRK           00    1     7

             4
         */

         /*Notes on BRK:
          * -Although it is a single byte operation, it needs an extra byte for padding,
          *  so treat it as two.
          *
          * -The B bit in the status register will need to be tested in the STACKED record,
          *  not the status register itself, as a result it needs to be set before you save the state.
          *
          * Reference: http://6502.org/tutorials/interrupts.html#2.2
          */
         void BRK4();

         /*
             BVC  Branch on Overflow Clear

             branch on V = 0                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BVC oper      50    2     2**

             11
         */

         void BVC11();

         /*
             BVS  Branch on Overflow Set

             branch on V = 1                  N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             relative      BVC oper      70    2     2**

             11
         */

         void BVS11();

         /*
             CLC  Clear Carry Flag

             0 -> C                           N Z C I D V
                                              - - 0 - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       CLC           18    1     2

             4
         */

         void CLC4();

         /*
             CLD  Clear Decimal Mode

             0 -> D                           N Z C I D V
                                              - - - - 0 -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       CLD           D8    1     2

             4
         */

         void CLD4();

         /*
             CLI  Clear Interrupt Disable Bit

             0 -> I                           N Z C I D V
                                              - - - 0 - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       CLI           58    1     2

             4
         */

         void CLI4();

         /*
             CLV  Clear Overflow Flag

             0 -> V                           N Z C I D V
                                              - - - - - 0

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       CLV           B8    1     2

             4
         */

         void CLV4();

         /*
             CMP  Compare Memory with Accumulator

             A - M                            N Z C I D V
                                              + + + - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     CMP #oper     C9    2     2
             zeropage      CMP oper      C5    2     3
             zeropage,X    CMP oper,X    D5    2     4
             absolute      CMP oper      CD    3     4
             absolute,X    CMP oper,X    DD    3     4*
             absolute,Y    CMP oper,Y    D9    3     4*
             (indirect,X)  CMP (oper,X)  C1    2     6
             (indirect),Y  CMP (oper),Y  D1    2     5*

             1 3 7 2 6 6 9 10
         */

         void CMP(byte operand);
         void CMP1();
         void CMP3();
         void CMP7();
         void CMP2();
         void CMP6_X();
         void CMP6_Y();
         void CMP9();
         void CMP10();

         /*
             CPX  Compare Memory and Index X

             X - M                            N Z C I D V
                                              + + + - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     CPX #oper     E0    2     2
             zeropage      CPX oper      E4    2     3
             absolute      CPX oper      EC    3     4

             1 3 2
         */

         void CPX(byte operand);
         void CPX1();
         void CPX3();
         void CPX2();

         /*
             CPY  Compare Memory and Index Y

             Y - M                            N Z C I D V
                                              + + + - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     CPY #oper     C0    2     2
             zeropage      CPY oper      C4    2     3
             absolute      CPY oper      CC    3     4

             1 3 2
         */

         void CPY(byte operand);
         void CPY1();
         void CPY3();
         void CPY2();

         /*
             DEC  Decrement Memory by One

             M - 1 -> M                       N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             zeropage      DEC oper      C6    2     5
             zeropage,X    DEC oper,X    D6    2     6
             absolute      DEC oper      CE    3     3
             absolute,X    DEC oper,X    DE    3     7

             3 7 2 6
         */

         void DEC(unsigned short address);
         void DEC3();
         void DEC7();
         void DEC2();
         void DEC6();

         /*
             DEX  Decrement Index X by One

             X - 1 -> X                       N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       DEC           CA    1     2

             4
         */

         void DEX4();

         /*
             DEY  Decrement Index Y by One

             Y - 1 -> Y                       N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       DEC           88    1     2

             4
         */

         void DEY4();

         /*
             EOR  Exclusive-OR Memory with Accumulator

             A EOR M -> A                     N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     EOR #oper     49    2     2
             zeropage      EOR oper      45    2     3
             zeropage,X    EOR oper,X    55    2     4
             absolute      EOR oper      4D    3     4
             absolute,X    EOR oper,X    5D    3     4*
             absolute,Y    EOR oper,Y    59    3     4*
             (indirect,X)  EOR (oper,X)  41    2     6
             (indirect),Y  EOR (oper),Y  51    2     5*

             1 3 7 2 6 6 9 10
         */

         void EOR(byte operand);
         void EOR1();
         void EOR3();
         void EOR7();
         void EOR2();
         void EOR6_X();
         void EOR6_Y();
         void EOR9();
         void EOR10();

         /*
             INC  Increment Memory by One

             M + 1 -> M                       N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             zeropage      INC oper      E6    2     5
             zeropage,X    INC oper,X    F6    2     6
             absolute      INC oper      EE    3     6
             absolute,X    INC oper,X    FE    3     7

             3 7 2 6
         */

         void INC(unsigned short address);
         void INC3();
         void INC7();
         void INC2();
         void INC6();

         /*
             INX  Increment Index X by One

             X + 1 -> X                       N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       INX           E8    1     2

             4
         */

         void INX4();

         /*
             INY  Increment Index Y by One

             Y + 1 -> Y                       N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       INY           C8    1     2

             4
         */

         void INY4();

         /*
             JMP  Jump to New Location

             (PC+1) -> PCL                    N Z C I D V
             (PC+2) -> PCH                    - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             absolute      JMP oper      4C    3     3
             indirect      JMP (oper)    6C    3     5

             2 8
         */

         void JMP2();
         void JMP8();

         /*
             JSR  Jump to New Location Saving Return Address

             push (PC+2),                     N Z C I D V
             (PC+1) -> PCL                    - - - - - -
             (PC+2) -> PCH

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             absolute      JSR oper      20    3     6

             2
         */

         void JSR2();

         /*
             LDA  Load Accumulator with Memory

             M -> A                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     LDA #oper     A9    2     2
             zeropage      LDA oper      A5    2     3
             zeropage,X    LDA oper,X    B5    2     4
             absolute      LDA oper      AD    3     4
             absolute,X    LDA oper,X    BD    3     4*
             absolute,Y    LDA oper,Y    B9    3     4*
             (indirect,X)  LDA (oper,X)  A1    2     6
             (indirect),Y  LDA (oper),Y  B1    2     5*

             1 3 7 2 6 6 9 10
         */

         void LDA(byte operand);
         void LDA1();
         void LDA3();
         void LDA7();
         void LDA2();
         void LDA6_X();
         void LDA6_Y();
         void LDA9();
         void LDA10();

         /*
             LDX  Load Index X with Memory

             M -> X                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     LDX #oper     A2    2     2
             zeropage      LDX oper      A6    2     3
             zeropage,Y    LDX oper,Y    B6    2     4
             absolute      LDX oper      AE    3     4
             absolute,Y    LDX oper,Y    BE    3     4*

             1 3 7 2 6
         */

         void LDX(byte operand);
         void LDX1();
         void LDX3();
         void LDX7();
         void LDX2();
         void LDX6();

         /*
             LDY  Load Index Y with Memory

             M -> Y                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     LDY #oper     A0    2     2
             zeropage      LDY oper      A4    2     3
             zeropage,X    LDY oper,X    B4    2     4
             absolute      LDY oper      AC    3     4
             absolute,X    LDY oper,X    BC    3     4*

             1 3 7 2 6
         */

         void LDY(byte operand);
         void LDY1();
         void LDY3();
         void LDY7();
         void LDY2();
         void LDY6();

         /*
             LSR  Shift One Bit Right (Memory or Accumulator)

             0 -> [76543210] -> C             N Z C I D V
                                              - + + - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             accumulator   LSR A         4A    1     2
             zeropage      LSR oper      46    2     5
             zeropage,X    LSR oper,X    56    2     6
             absolute      LSR oper      4E    3     6
             absolute,X    LSR oper,X    5E    3     7

             5 3 7 2 6
         */

         byte LSR(byte operand); //return result of right shift
         void LSR5();
         void LSR3();
         void LSR7();
         void LSR2();
         void LSR6();

         /*
             NOP  No Operation

             ---                              N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       NOP           EA    1     2

             4

             NOTE: Implemented in the intepreter as a blank return, no point making a method for it.
         */

         /*
             ORA  OR Memory with Accumulator

             A OR M -> A                      N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     ORA #oper     09    2     2
             zeropage      ORA oper      05    2     3
             zeropage,X    ORA oper,X    15    2     4
             absolute      ORA oper      0D    3     4
             absolute,X    ORA oper,X    1D    3     4*
             absolute,Y    ORA oper,Y    19    3     4*
             (indirect,X)  ORA (oper,X)  01    2     6
             (indirect),Y  ORA (oper),Y  11    2     5*

             1 3 7 2 6 6 9 10
         */

         void ORA1();
         void ORA3();
         void ORA7();
         void ORA2();
         void ORA6_X();
         void ORA6_Y();
         void ORA9();
         void ORA10();

         /*
             PHA  Push Accumulator on Stack

             push A                           N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       PHA           48    1     3

             4
         */

         void PHA4();

         /*
             PHP  Push Processor Status on Stack

             push SR                          N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       PHP           08    1     3

             4
         */

         void PHP4();

         /*
             PLA  Pull Accumulator from Stack

             pull A                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       PLA           68    1     4

             4
         */

         void PLA4();

         /*
             PLP  Pull Processor Status from Stack

             pull SR                          N Z C I D V
                                              from stack

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       PHP           28    1     4

             4
         */

         void PLP4();

         /*
             ROL  Rotate One Bit Left (Memory or Accumulator)

             C <- [76543210] <- C             N Z C I D V
                                              + + + - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             accumulator   ROL A         2A    1     2
             zeropage      ROL oper      26    2     5
             zeropage,X    ROL oper,X    36    2     6
             absolute      ROL oper      2E    3     6
             absolute,X    ROL oper,X    3E    3     7

             5 3 8 2 6
         */

         void ROL5();
         void ROL3();
         void ROL7();
         void ROL2();
         void ROL6();

         /*
             ROR  Rotate One Bit Right (Memory or Accumulator)

             C -> [76543210] -> C             N Z C I D V
                                              + + + - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             accumulator   ROR A         6A    1     2
             zeropage      ROR oper      66    2     5
             zeropage,X    ROR oper,X    76    2     6
             absolute      ROR oper      6E    3     6
             absolute,X    ROR oper,X    7E    3     7

             5 3 8 2 6
         */

         void ROR5();
         void ROR3();
         void ROR8();
         void ROR2();
         void ROR6();

         /*
             RTI  Return from Interrupt

             pull SR, pull PC                 N Z C I D V
                                              from stack

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       RTI           40    1     6

             4
         */

         void RTI4();

         /*
             RTS  Return from Subroutine

             pull PC, PC+1 -> PC              N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       RTS           60    1     6

             4
         */

         void RTS4();

         /*
             SBC  Subtract Memory from Accumulator with Borrow

             A - M - C -> A                   N Z C I D V
                                              + + + - - +

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             immidiate     SBC #oper     E9    2     2
             zeropage      SBC oper      E5    2     3
             zeropage,X    SBC oper,X    F5    2     4
             absolute      SBC oper      ED    3     4
             absolute,X    SBC oper,X    FD    3     4*
             absolute,Y    SBC oper,Y    F9    3     4*
             (indirect,X)  SBC (oper,X)  E1    2     6
             (indirect),Y  SBC (oper),Y  F1    2     5*

             1 4 7 2 6 6 9 10
         */

         void SBC1();
         void SBC4();
         void SBC7();
         void SBC2();
         void SBC6_X();
         void SBC6_Y();
         void SBC9();
         void SBC10();

         /*
             SEC  Set Carry Flag

             1 -> C                           N Z C I D V
                                              - - 1 - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       SEC           38    1     2

             4
         */

         void SEC4();

         /*
             SED  Set Decimal Flag

             1 -> D                           N Z C I D V
                                              - - - - 1 -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       SED           F8    1     2

             4
         */

         void SED4();

         /*
             SEI  Set Interrupt Disable Status

             1 -> I                           N Z C I D V
                                              - - - 1 - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       SEI           78    1     2

             4
         */

         void SEI4();

         /*
             STA  Store Accumulator in Memory

             A -> M                           N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             zeropage      STA oper      85    2     3
             zeropage,X    STA oper,X    95    2     4
             absolute      STA oper      8D    3     4
             absolute,X    STA oper,X    9D    3     5
             absolute,Y    STA oper,Y    99    3     5
             (indirect,X)  STA (oper,X)  81    2     6
             (indirect),Y  STA (oper),Y  91    2     6

             3 7 2 6 6 9 10
         */

         void STA3();
         void STA7();
         void STA2();
         void STA6_X();
         void STA6_Y();
         void STA9();
         void STA10();

         /*
             STX  Store Index X in Memory

             X -> M                           N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             zeropage      STX oper      86    2     3
             zeropage,Y    STX oper,Y    96    2     4
             absolute      STX oper      8E    3     4

             3 7 2
         */

         void STX3();
         void STX7();
         void STX2();

         /*
             STY  Sore Index Y in Memory

             Y -> M                           N Z C I D V
                                              - - - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             zeropage      STY oper      84    2     3
             zeropage,X    STY oper,X    94    2     4
             absolute      STY oper      8C    3     4

             3 7 2
         */

         void STY3();
         void STY7();
         void STY2();

         /*
             TAX  Transfer Accumulator to Index X

             A -> X                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       TAX           AA    1     2

             4
         */

         void TAX4();

         /*
             TAY  Transfer Accumulator to Index Y

             A -> Y                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       TAY           A8    1     2

             4
         */

         void TAY4();

         /*
             TSX  Transfer Stack Pointer to Index X

             SP -> X                          N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       TSX           BA    1     2

             4
         */

         void TSX4();

         /*
             TXA  Transfer Index X to Accumulator

             X -> A                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       TXA           8A    1     2

             4
         */

         void TXA4();

         /*
             TXS  Transfer Index X to Stack Register

             X -> SP                          N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       TXS           9A    1     2

             4
         */

         void TXS4();

         /*
             TYA  Transfer Index Y to Accumulator

             Y -> A                           N Z C I D V
                                              + + - - - -

             addressing    assembler    opc  bytes  cyles
             --------------------------------------------
             implied       TYA           98    1     2

             4
         */

         void TYA4();

         //--General(private)
         void setupCycleLookup();
         void run(); //will house the main loop for emulating the CPU
         void runCommand(byte opcode);

        public:
            //--Friends:
            friend class Stack;
            friend class Compiler;
            friend class CpuTest;

            //--General Methods:
            void execute(std::string program); //Executes a series of instructions given in the string
            void reset(); //resets cpu state and memory
            void start(); //Starts the CPU
            void stop(); //Stops it

            //--Debugging Methods:
            void setPC(unsigned short address); //set pc to point to address
            void runNext(bool status); //runs command the PC is pointing to, and moves to the next one, if status is true, show it
            void status(); //prints a report on the status of the CPU
            void status(std::string header); //like above but replaces STATUS with the chosen header

            //--Constructors:
            /*Constructor 1
             *clockSpeedMhz = the clockspeed that this should operate on in Mhz(max is 2)
             *memory = A pointer to an instanciated Memory object(represents RAM)
             */
            MOS6502CPU(unsigned int clockSpeedMhz, Memory* memory, bool debug);

            //Destructor
            ~MOS6502CPU();
    };
}
#endif
