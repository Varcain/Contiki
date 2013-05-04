#include "contiki.h"
#include "contiki-net.h"
#include "telnetd.h"
#include "shell.h"

/*---------------------------------------------------------------------------*/
PROCESS(shell_init_process, "Shell init process");
AUTOSTART_PROCESSES(&telnetd_process, &shell_init_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_init_process, ev, data)
{
	PROCESS_BEGIN();
	shell_ps_init();
	shell_netstat_init();
	shell_memdebug_init();
	shell_run_init();
	shell_text_init();
	shell_time_init();
	shell_ping_init();
	shell_blink_init();
	shell_netperf_init();
	shell_rime_init();
	shell_rime_debug_init();
	shell_rime_debug_runicast_init();
	shell_rime_neighbors_init();
	shell_rime_netcmd_init();
	shell_rime_ping_init();
	shell_rime_sendcmd_init();
	shell_rime_sniff_init();
	shell_rime_unicast_init();

	PROCESS_END();
}
