; hello-os
; TAB=4


start:
    ORG   0X7c00
    JMP   entry
    DB    0x90

		DB		"HELLOIPL"	
		DW		512				
		DB		1				
		DW		1				
		DB		2				
		DW		224				
		DW		2880			
		DB		0xf0		
		DW		9			
		DW		18	
		DW		2		
		DD		0	
		DD		2880			
		DB		0,0,0x29
		DD		0xffffffff	
		DB		"HELLO-OS   "
		DB		"FAT12   "	
		RESB	18				

; ºËÐÄ²¿·Ö

entry:
    MOV   AX,0
    MOV   SS,AX
    MOV   SP,0x7c00
    MOV   DS,AX
    MOV   ES,AX

readloop:
    MOV		AX,0x0820
		MOV		ES,AX
		MOV		CH,0		
		MOV		DH,0	
		MOV		CL,2

		MOV		AH,0x02		
		MOV		AL,1		
		MOV		BX,0
		MOV		DL,0x80	
		INT		0x13		
		JC		error

next:
    MOV   AX,ES
    ADD   AX,0x0020
    MOV   ES,AX
    ADD   CL,1
    CMP   CL,8
    JBE   readloop

fin:
		JMP		0x8200	

error:
		MOV		SI,errmsg
putloop:
		MOV		AL,[SI]
		ADD		SI,1		
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e	
		MOV		BX,15		
		INT		0x10		
		JMP		putloop

errmsg:
		DB		0x0a, 0x0a		
		DB		"error happen"
		DB		0x0a			
		DB		0

msg:
		DB		0x0a, 0x0a		
		DB		"hello, kitty~~cc"
		DB		0x0a			
		DB		0

marker:
    RESB	0x1fe-(marker-start)
		DB		0x55, 0xaa

