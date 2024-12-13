	.global toggle_LED_on   ; Make the function accessible globally
	.global toggle_LED_off ; Make the function accessible globally


toggle_LED_on:
	bis.b   #0x40, &P1OUT ; Turn on LED (P1OUT |= BIT6)
	ret

toggle_LED_off:
	bic.b   #0x40, &P1OUT ; Turn off LED (P1OUT &= ~BIT6)
	ret

	
