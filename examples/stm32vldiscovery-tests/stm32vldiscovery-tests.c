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
  //shell_netstat_init();
  //shell_wget_init();
  //shell_memdebug_init();
  //shell_file_init();
  //shell_httpd_init();
  //shell_irc_init();
  //shell_run_init();
  //shell_text_init();
  //shell_time_init();
  shell_ping_init();
  //shell_netperf_init();

  PROCESS_END();
}
