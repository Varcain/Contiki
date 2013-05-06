#include <contiki.h>
#include <contiki-net.h>

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"


#include <stdio.h>
#include <stddef.h>

#include <dev/leds.h>
#include <dev/uart1.h>
#include <dev/uart0.h>

#include "serial-line.h"

#include <shell.h>

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

uip_ipaddr_t hostaddr,netmask, draddr;

#include "net/uip-fw-drv.h"
#include "net/uip-over-mesh.h"

static struct uip_fw_netif meshif =
  {UIP_FW_NETIF(172,16,0,0, 255,255,0,0, uip_over_mesh_send)};

#define UIP_OVER_MESH_CHANNEL 8
static uint8_t is_gateway;

#undef putchar

int __attribute__(( weak )) putchar(int c)
{
  //uart0_writeb(c);
  return c;
}

void __io_putchar(char c)
{
  putchar(c);
}


/*---------------------------------------------------------------------------*/
int
main(void)
{
    //
    // Enable lazy stacking for interrupt handlers.  This allows floating-point
    // instructions to be used within interrupt handlers, but at the expense of
    // extra stack usage.
    //
    ROM_FPUEnable();
    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                       SYSCTL_XTAL_16MHZ);

	leds_init();
	uart0_init(0);
	printf("\rStarting Contiki on LM4F120 Launchpad...\n\r");

	clock_init();
	process_init();

	process_start(&etimer_process, NULL);
	ctimer_init();

	/* Networking stack. */
	NETSTACK_RADIO.init();
	NETSTACK_RDC.init();
	NETSTACK_MAC.init();
	NETSTACK_NETWORK.init();
	{
		rimeaddr_t rimeaddr;

		rimeaddr.u8[0] = 0x00;
		rimeaddr.u8[1] = 0x03;
		rimeaddr_set_node_addr(&rimeaddr);
	}

	process_start(&tcpip_process, NULL);
	process_start(&uip_fw_process, NULL);

	uip_init();

	uip_ipaddr(&hostaddr, 172, 16, 0, 3);
	uip_ipaddr_copy(&meshif.ipaddr, &hostaddr);
	uip_sethostaddr(&hostaddr);
	uip_ipaddr(&netmask, 255, 255, 0, 0);
	uip_setnetmask(&netmask);

	uip_over_mesh_set_net(&hostaddr, &netmask);
	uip_fw_default(&meshif);
	//uip_fw_register(&slipif);
	uip_over_mesh_init(UIP_OVER_MESH_CHANNEL);

	autostart_start(autostart_processes);

	while (1)
	{
		process_run();
	}

	return 0;
}
/*---------------------------------------------------------------------------*/
void
log_message(char *m1, char *m2)
{
	//printf("%s%s\n", m1, m2);
}
/*---------------------------------------------------------------------------*/
void
uip_log(char *m)
{
	//printf("%s\n", m);
}
/*---------------------------------------------------------------------------*/
