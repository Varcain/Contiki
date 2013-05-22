#include "contiki.h"
#include "dev/leds.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_1);
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
	 return	((GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3)) ? LEDS_GREEN : 0);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
	((leds & LEDS_GREEN) ? GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3) :
						   GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0));
}
/*---------------------------------------------------------------------------*/
