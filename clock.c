#include "clock.h"

#define MCG_C1_EX_CLK_FLL_DIV_256 (0b00011000)
#define MCG_C2_OSC_SL (1 << 2)
#define MCG_C2_HIGH_FRE_SL (1 << 4)
#define MCG_C5_PRDIV0_DIV_4 (0b00011);
#define MCG_C6_VDIV0_MUL_24 0
#define SIM_CLKDIV1_OUTDIV1_DIV_2 (1 << 28)
#define SIM_CLKDIV1_OUTDIV4_DIV_2 (1 << 16)

#define SIM_RTC_EN (1 << 29)
#define RTC_CR_OSC_EN (1 << 8)

void mcg_clock_config(void)
{
	// add the 8pF - 16pF to the oscillator load
	OSC0->CR = OSC_CR_SC16P_MASK | OSC_CR_SC8P_MASK;
	// Select Osc clock: high frequency range, request osc
	// lựa chọn high frequency range 01 : 3Mhz - 8MHz - high-gain -> 8MHz
	MCG->C2 = MCG_C2_HIGH_FRE_SL | MCG_C2_OSC_SL | MCG_C2_HGO0_MASK;
	// Output from FLL is selected //0x18 // 0b00011000
	// vì nguồn tới FLL trong khoảng 31.25 kHz tới 39.0625 kHz nne ta chia cho 256 // OSCSEL = 0
	MCG->C1 = MCG_C1_EX_CLK_FLL_DIV_256; // FLL = 31.25 kHz
	//	enable MCGIRCLK and ICLK is enabled in Stop Mode
	MCG->C1 |= MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK;
	// select slow internal clock
	MCG->C2 &= ~MCG_C2_IRCS_MASK;
	// MCGIRCLK = 32 kHz
	// check whether osc init successfully or not
	while (!(MCG->S & MCG_S_OSCINIT0_MASK))
	{
	}
	// wait source for the PFF change to external reference clock
	while (MCG->S & MCG_S_IREFST_MASK)
	{
	}
	// while ((MCG->S & MCG_S_IREFST_MASK)) {}
	// PRDIV0 / 4 that's because pll from 2 -> 4 MHz -> 8 / 4 = 2
	MCG->C5 = MCG_C5_PRDIV0_DIV_4;
	// Multiply by 24 (2MHz * 24 = 48MHz)
	// select PLL source
	MCG->C6 = MCG_C6_VDIV0_MUL_24 | MCG_C6_PLLS_MASK;
	while (!(MCG->S & MCG_S_PLLST_MASK))
	{
	}
	while (!(MCG->S & MCG_S_LOCK0_MASK))
	{
	}

	// OUTDIV1 / 2 and OUTDIV4 / 2
	SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1_DIV_2 | SIM_CLKDIV1_OUTDIV4_DIV_2;
	// 24MHz // 12MHz
	SystemCoreClockUpdate();
}
