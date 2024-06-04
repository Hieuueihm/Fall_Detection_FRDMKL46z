#include "timer.h"


void pit_timer_init(float expected_time, uint8_t STATE){
	
	SIM->SCGC6 |= (1<<23);
	PIT->MCR = 0x00U;
	
	switch (STATE){
		case STATE_ACTIVE:
			
			PIT->CHANNEL[0].TCTRL = 0x00U;
			PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
			PIT->CHANNEL[0].LDVAL = expected_time/BUSCLOCK_PERIOD_SEC - 1;
	
			PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);
			break;
		case STATE_FALL:
			PIT->CHANNEL[1].TCTRL = 0x00U;
			PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
			PIT->CHANNEL[1].LDVAL = expected_time/BUSCLOCK_PERIOD_SEC - 1;

			PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);

			break;
	}
			NVIC_SetPriority(22, 14);
			NVIC_EnableIRQ(22);
		
	
}