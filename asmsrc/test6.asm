SORT8    LDY #$00
         STY $32
         LDA ($30),Y
         TAX        
         INY        
         DEX        
NXTEL    LDA ($30),Y
         INY
         CMP ($30),Y
         BCC CHKEND
         BEQ CHKEND          
         PHA       
         LDA ($30),Y
         DEY        
         STA ($30),Y
         PLA        
         INY        
         STA ($30),Y
         LDA #$FF   
         STA $32
CHKEND   DEX    
         BNE NXTEL
         BIT $32  
         BMI SORT8
         RTS