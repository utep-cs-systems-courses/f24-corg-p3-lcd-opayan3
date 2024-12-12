	.global reaction_time_calc_asm

	reaction_time_calc_asm:
	MOV #10, R12
	Call #show_result
	RET
