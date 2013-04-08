#include <contiki.h>
#include <contiki-net.h>

#include <stdio.h>
#include <stddef.h>
#include <stm32f10x.h>

#include <dev/leds.h>
#include <dev/uart3.h>
#include <dev/slip.h>


PROCINIT(&etimer_process);

uip_ipaddr_t hostaddr;
uip_ipaddr_t netmask;
uip_ipaddr_t draddr;

static struct uip_fw_netif slipif =
{UIP_FW_NETIF(172,16,0,0, 255,255,0,0, slip_send)};

#undef putchar

int __attribute__(( weak )) putchar(int c)
{
  //uart3_writeb(c);
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
// Initialize serial first so the debug console is active first
  uart3_init(0);
  slip_arch_init(0);
  //printf("\rStarting Contiki...\n\r");
  clock_init();
  process_init();
  //serial_line_init();
  //uart3_set_input(serial_line_input_byte);
  uip_init();
  uip_fw_init();
  process_start(&tcpip_process, NULL);
  process_start(&slip_process, NULL);
  process_start(&uip_fw_process, NULL);
  uip_ipaddr(&hostaddr, 172, 16, 0, 1);
  uip_sethostaddr(&hostaddr);
  uip_ipaddr(&netmask, 255, 255, 0, 0);
  uip_setnetmask(&netmask);
  uip_ipaddr(&netmask, 172, 16, 0, 2);
  uip_setdraddr(&draddr);
  uip_fw_register(&slipif);
  uip_fw_default(&slipif);
  rtimer_init();
  leds_init();
  process_start(&etimer_process, NULL);
  ctimer_init();
  procinit_init();
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
