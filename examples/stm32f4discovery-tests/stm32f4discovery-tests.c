#include "contiki.h"
#include "contiki-net.h"
#include "telnetd.h"
#include "shell.h"
#include "webserver-nogui.h"
#include "dev/temperature-sensor.h"

/*---------------------------------------------------------------------------*/
PROCESS(sensor_psock_server_process, "Sensor protosocket server");
PROCESS(shell_init_process, "Shell init process");
AUTOSTART_PROCESSES(&telnetd_process, &shell_init_process, &webserver_nogui_process,
					&sensor_psock_server_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_init_process, ev, data)
{
	PROCESS_BEGIN();
	shell_ps_init();
	shell_netstat_init();
	shell_memdebug_init();
	shell_text_init();
	shell_time_init();
	shell_blink_init();
	shell_rime_neighbors_init();
	shell_rime_netcmd_init();
	shell_rime_ping_init();
	shell_rime_sendcmd_init();
	shell_sendtest_init();
	shell_collect_view_init();
	shell_rime_init();
	shell_tcpsend_init();
	PROCESS_END();
}

/*
 * We define one protosocket since we've decided to only handle one
 * connection at a time. If we want to be able to handle more than one
 * connection at a time, each parallell connection needs its own
 * protosocket.
 */
static struct psock ps;

static char buffer[100];

struct etimer sendtimer;

static PT_THREAD(handle_connection(struct psock *p))
{
	static char buf[100];
	PSOCK_BEGIN(p);

	etimer_set(&sendtimer, 3000);

	PSOCK_WAIT_UNTIL(p, etimer_expired(&sendtimer));

	snprintf(buf, sizeof(buf), "%02X:%02X		temperature: %d\n\r",
			rimeaddr_node_addr.u8[0],
			rimeaddr_node_addr.u8[1],
			temperature_sensor.value(0)/10);

	PSOCK_SEND_STR(p, buf);

	PSOCK_END(p);
}

/*---------------------------------------------------------------------------*/
/*
 * The definition of the process.
 */
PROCESS_THREAD(sensor_psock_server_process, ev, data)
{
  PROCESS_BEGIN();

  tcp_listen(UIP_HTONS(1010));

  while(1) {

    PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);

    if(uip_connected()) {

      PSOCK_INIT(&ps, buffer, sizeof(buffer));

      while(!(uip_aborted() || uip_closed() || uip_timedout())) {

        PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event || ev == PROCESS_EVENT_TIMER);
        handle_connection(&ps);
      }
      PSOCK_CLOSE(&ps);
    }
  }

  PSOCK_CLOSE_EXIT(&ps);
  PROCESS_END();
}
