	.global toggleLEDsAsm

toggleLEDsAsm:
	xor.b #0x01, &P1OUT	; Toggle LED1 (P1.0)
	xor.b #0x40, &P1OUT	; Toggle LED2 (P1.6)
	    reti
