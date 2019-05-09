    LDX #05
    STX $30
    TXA
    CMP $30
    BEQ stuff
    BRK
    LDY #$a
stuff:
    LDA #$ff
    BRK
