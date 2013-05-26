#include <contiki.h>
#include <contiki-net.h>

#include <stdio.h>
#include <stddef.h>
#include <stm32f4xx.h>

#include "lib/sensors.h"

#include <dev/leds.h>
#include <dev/uart1.h>
#include <dev/uart3.h>
#include <dev/slip.h>
#include <dev/temperature-sensor.h>

#include <shell.h>

#include "stm32f4xx_dbgmcu.h"

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

SENSORS(&temperature_sensor);

uip_ipaddr_t hostaddr,netmask, draddr;

#include "net/uip-fw-drv.h"
#include "net/uip-over-mesh.h"
static struct uip_fw_netif slipif =
  {UIP_FW_NETIF(192,168,1,2, 255,255,255,255, slip_send)};
static struct uip_fw_netif meshif =
  {UIP_FW_NETIF(172,16,0,0, 255,255,0,0, uip_over_mesh_send)};

#define UIP_OVER_MESH_CHANNEL 8
static uint8_t is_gateway;

#undef putchar

int __attribute__(( weak )) putchar(int c)
{
  //uart1_writeb(c);
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
	//DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, ENABLE);

	uart3_init(0);
	//uart1_init(0);
	slip_arch_init(0);
	leds_init();
	printf("\rStarting Contiki on STM32F4 Discovery...\n\r");

	clock_init();
	process_init();

	process_start(&etimer_process, NULL);
	ctimer_init();

	process_start(&sensors_process, NULL);

	/* Networking stack. */
	NETSTACK_RADIO.init();
	NETSTACK_RDC.init();
	NETSTACK_MAC.init();
	NETSTACK_NETWORK.init();
	{
		rimeaddr_t rimeaddr;

		rimeaddr.u8[0] = 0x00;
		rimeaddr.u8[1] = 0x01;
		rimeaddr_set_node_addr(&rimeaddr);
	}

	process_start(&tcpip_process, NULL);
	process_start(&slip_process, NULL);
	process_start(&uip_fw_process, NULL);

	uip_init();

	uip_ipaddr(&hostaddr, 172, 16, 0, 1);
	uip_ipaddr_copy(&meshif.ipaddr, &hostaddr);
	uip_sethostaddr(&hostaddr);
	uip_ipaddr(&netmask, 255, 255, 0, 0);
	uip_setnetmask(&netmask);
	uip_over_mesh_set_net(&hostaddr, &netmask);
	uip_over_mesh_set_gateway_netif(&slipif);
	uip_fw_default(&meshif);
	//uip_fw_register(&slipif);
	uip_over_mesh_init(UIP_OVER_MESH_CHANNEL);

	uip_over_mesh_set_gateway(&rimeaddr_node_addr);
	uip_over_mesh_make_announced_gateway();
	is_gateway = 1;

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
	printf("%s%s\n", m1, m2);
}
/*---------------------------------------------------------------------------*/
void
uip_log(char *m)
{
	printf("%s\n", m);
}
/*---------------------------------------------------------------------------*/
