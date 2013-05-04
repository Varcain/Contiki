#include "contiki.h"
#include "contiki-net.h"
#include "telnetd.h"
#include "shell.h"

/*---------------------------------------------------------------------------*/
PROCESS(shell_init_process, "Shell init process");
AUTOSTART_PROCESSES(&shell_init_process, &telnetd_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(shell_init_process, ev, data)
{
	PROCESS_BEGIN();

	shell_rime_ping_init();

	PROCESS_END();
}
