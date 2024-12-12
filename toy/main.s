	.global _asmFunction

_asmFunction:
	;;  Example function that does something simple, like toggling a pin or delay
	BIS.B #0x01, &P1OUT	; Set P1.0 (LED) high
	NOP			; No operation (just for delay)
	BIC.B #0x01, &P1OUT	; Set P1.0 (LED) low
	RET
