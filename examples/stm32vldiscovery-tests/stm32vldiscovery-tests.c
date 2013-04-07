#include "contiki.h"
#include "shell.h"
#include "serial-shell.h"

#include "dev/leds.h"

#include <stdio.h>
#include <string.h>

static struct etimer timer;
/*---------------------------------------------------------------------------*/
PROCESS(example_shell_process, "Contiki shell");
AUTOSTART_PROCESSES(&example_shell_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_shell_process, ev, data)
{
  PROCESS_BEGIN();
  
  serial_shell_init();
  shell_base64_init();
  shell_blink_init();
  shell_memdebug_init();
  shell_ps_init();
  shell_text_init();
  shell_time_init();
  shell_vars_init();
  //shell_run_init();
  //shell_checkpoint_init();
  //shell_coffee_init();
  //shell_collect_view_init();
  //shell_download_init();
  //shell_exec_init();
  //shell_file_init();
  //shell_httpd_init();
  //shell_irc_init();
  //shell_netfile_init();
  //shell_netperf_init();
  //shell_netstat_init();
  //shell_ping_init();
  //shell_power_init();
  //shell_powertrace_init();
  //shell_profile_init();
  //shell_reboot_init();
  //shell_rime_init();
  //shell_rime_debug_init();
  //shell_rime_debug_runicast_init();
  //shell_rime_neighbors_init();
  //shell_rime_netcmd_init();
  //shell_rime_ping_init();
  //shell_rime_sendcmd_init();
  //shell_rime_sniff_init();
  //shell_rime_unicast_init();
  //shell_rsh_init();
  //shell_sendtest_init();
  //shell_sensortweet_init();
  //shell_tcpsend_init();
  //shell_tweet_init();
  //shell_udpsend_init();
  //shell_wget_init();
  
  
//  while(1)
//  {
//	etimer_set(&timer, CLOCK_SECOND);
//    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
//	leds_toggle(LEDS_RED);
//	leds_toggle(LEDS_GREEN);
//  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
