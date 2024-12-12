	.global toggleLEDS
	.text
toggleLEDS:
	;;  Toggle LED1 (P1.0)
	BIS     #0x01, &P1OUT ; Set P1.0 to toggle (LED1 ON)
	BIC     #0x01, &P1OUT ; Clear P1.0 to toggle (LED1 OFF)

	;;  Toggle LED2 (P1.6)
	BIS     #0x40, &P1OUT ; Set P1.6 to toggle (LED2 ON)
	BIC     #0x40, &P1OUT ; Clear P1.6 to toggle (LED2 OFF)

	    RET
