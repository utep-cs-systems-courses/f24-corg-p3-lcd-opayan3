	.global reaction_time_calc_asm

	reaction_time_calc_asm:
	MOV &reaction_time, R12
	MOV @R12, R12
	Call #show_result
	RET
