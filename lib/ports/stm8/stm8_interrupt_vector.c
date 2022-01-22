/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */

/* COSMIC: Requires interrupt vector table */
#if defined(__CSMC__)

/* Import Atomthreads system tick ISR prototype */
#include "atomport-private.h"

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector
{
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt(void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

extern void _stext(); /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext},							// reset
	{0x82, (interrupt_handler_t)TRAP_IRQHandler},					// trap  - Software interrupt
	{0x82, (interrupt_handler_t)TLI_IRQHandler},					// irq0  - External Top Level interrupt (TLI)
	{0x82, (interrupt_handler_t)AWU_IRQHandler},					// irq1  - Auto Wake Up from Halt interrupt
	{0x82, (interrupt_handler_t)CLK_IRQHandler},					// irq2  - Clock Controller interrupt
	{0x82, (interrupt_handler_t)EXTI_PORTA_IRQHandler},			// irq3  - External interrupt 0 (GPIOA)
	{0x82, (interrupt_handler_t)EXTI_PORTB_IRQHandler},			// irq4  - External interrupt 1 (GPIOB)
	{0x82, (interrupt_handler_t)EXTI_PORTC_IRQHandler},			// irq5  - External interrupt 2 (GPIOC)
	{0x82, (interrupt_handler_t)EXTI_PORTD_IRQHandler},			// irq6  - External interrupt 3 (GPIOD)
	{0x82, (interrupt_handler_t)EXTI_PORTE_IRQHandler},			// irq7  - External interrupt 4 (GPIOE)
	{0x82, (interrupt_handler_t)CAN_RX_IRQHandler},				// irq8  - CAN Rx interrupt
	{0x82, (interrupt_handler_t)CAN_TX_IRQHandler},				// irq9  - CAN Tx/ER/SC interrupt
	{0x82, (interrupt_handler_t)SPI_IRQHandler},					// irq10 - SPI End of transfer interrupt
	{0x82, (interrupt_handler_t)TIM1_UPD_OVF_TRG_BRK_IRQHandler}, // irq11 - TIM1 Update/Overflow/Trigger/Break interrupt
	{0x82, (interrupt_handler_t)TIM1_CAP_COM_IRQHandler},			// irq12 - TIM1 Capture/Compare interrupt
	{0x82, (interrupt_handler_t)TIM2_UPD_OVF_BRK_IRQHandler},		// irq13 - TIM2 Update/Overflow/Break interrupt
	{0x82, (interrupt_handler_t)TIM2_CAP_COM_IRQHandler},			// irq14 - TIM2 Capture/Compare interrupt
	{0x82, (interrupt_handler_t)TIM3_UPD_OVF_BRK_IRQHandler},		// irq15 - TIM3 Update/Overflow/Break interrupt
	{0x82, (interrupt_handler_t)TIM3_CAP_COM_IRQHandler},			// irq16 - TIM3 Capture/Compare interrupt
	{0x82, (interrupt_handler_t)USART_TX_IRQHandler},				// irq17 - USART Tx complete interrupt
	{0x82, (interrupt_handler_t)USART_RX_IRQHandler},				// irq18 - USART Rx interrupt
	{0x82, (interrupt_handler_t)I2C_IRQHandler},					// irq19 - I2C interrupt
	{0x82, (interrupt_handler_t)LINUART_TX_IRQHandler},			// irq20 - LINUART Tx interrupt
	{0x82, (interrupt_handler_t)LINUART_RX_IRQHandler},			// irq21 - LINUART Rx interrupt
	{0x82, (interrupt_handler_t)ADC_IRQHandler},					// irq22 - ADC end of conversion interrupt
	{0x82, (interrupt_handler_t)TIM4_UPD_OVF_IRQHandler},			// irq23 - TIM4 Update/Overflow interrupt
	{0x82, (interrupt_handler_t)FLASH_IRQHandler},				// irq24 - FLASH interrupt
	{0x82, (interrupt_handler_t)NonHandledInterrupt},				// irq25 - Reserved
	{0x82, (interrupt_handler_t)NonHandledInterrupt},				// irq26 - Reserved
	{0x82, (interrupt_handler_t)NonHandledInterrupt},				// irq27 - Reserved
	{0x82, (interrupt_handler_t)NonHandledInterrupt},				// irq28 - Reserved
	{0x82, (interrupt_handler_t)NonHandledInterrupt},				// irq29 - Reserved

};

#endif /* __CSMC__ */
