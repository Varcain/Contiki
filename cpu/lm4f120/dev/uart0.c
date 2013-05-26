#include "dev/uart0.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"


static int (*uart0_input_handler)(unsigned char c);

/*---------------------------------------------------------------------------*/
uint8_t
uart0_active(void)
{
	return 0;
}
/*---------------------------------------------------------------------------*/
void
uart0_set_input(int (*input)(unsigned char c))
{
	uart0_input_handler = input;
}
/*---------------------------------------------------------------------------*/
void
uart0_writeb(unsigned char c)
{
	//ROM_UARTCharPutNonBlocking(UART0_BASE, c);
	UARTCharPut(UART0_BASE,c);
}
/*---------------------------------------------------------------------------*/
/**
 * Initalize the RS232 port.
 *
 */
void
uart0_init(unsigned long ubr)
{
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	//
	// Enable the GPIO port that is used for the on-board LED.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//
	// Enable the GPIO pins for the LED (PF2).
	//
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

	ROM_IntMasterEnable();

	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
					UART_CONFIG_PAR_NONE));

	ROM_IntEnable(INT_UART0);
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
}


/*---------------------------------------------------------------------------*/
void UART0IntHandler(void)
{
	unsigned long ulStatus;

	ulStatus = ROM_UARTIntStatus(UART0_BASE, true);
	ROM_UARTIntClear(UART0_BASE, ulStatus);

	while(ROM_UARTCharsAvail(UART0_BASE))
	{
		uart0_input_handler(ROM_UARTCharGetNonBlocking(UART0_BASE));
	}
}
