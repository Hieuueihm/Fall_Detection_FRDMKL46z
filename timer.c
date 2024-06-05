#include "timer.h"

#define SIM_SCGC6_PIT_EN (1 << 23)

void pit_timer_init(float expected_time, uint8_t STATE)
{

	SIM->SCGC6 |= SIM_SCGC6_PIT_EN;
	PIT->MCR = 0x00U;

	NVIC_ClearPendingIRQ(22);
	switch (STATE)
	{
	case STATE_ACTIVE:

		PIT->CHANNEL[0].TCTRL = 0x00U;
		PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK; // time out has occured
		PIT->CHANNEL[0].LDVAL = expected_time / BUSCLOCK_PERIOD_SEC - 1;
		// enable timer and interrupt
		PIT->CHANNEL[0].TCTRL |= (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);
		break;
	case STATE_FALL:
		PIT->CHANNEL[1].TCTRL = 0x00U;
		PIT->CHANNEL[1].TFLG = PIT_TFLG_TIF_MASK;
		PIT->CHANNEL[1].LDVAL = expected_time / BUSCLOCK_PERIOD_SEC - 1;
		PIT->CHANNEL[1].TCTRL |= (PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK);

		break;
	}
	NVIC_SetPriority(22, 14);
	NVIC_EnableIRQ(22);
}