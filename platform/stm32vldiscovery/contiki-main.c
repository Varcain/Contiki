#include <contiki.h>

#include <stdio.h>
#include <stddef.h>
#include <stm32f10x.h>

#include <dev/leds.h>
#include <dev/uart3.h>
#include <dev/serial-line.h>

PROCINIT(&etimer_process);

/*---------------------------------------------------------------------------*/
int
main(void)
{
// Initialize serial first so the debug console is active first
  uart3_init(0);
  printf("\rStarting Contiki...\n\r");
  clock_init();
  process_init();
  serial_line_init();
  uart3_set_input(serial_line_input_byte);
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
