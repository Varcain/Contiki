#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#include <stdint.h>

#define CC_CONF_REGISTER_ARGS          1
#define CC_CONF_FUNCTION_POINTER_ARGS  1
#define CC_CONF_FASTCALL
#define CC_CONF_VA_ARGS                1

#define CCIF
#define CLIF

typedef uint8_t   u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef  int32_t s32_t;
typedef unsigned short uip_stats_t;

#define UIP_CONF_UDP             1
#define UIP_CONF_MAX_CONNECTIONS 40
#define UIP_CONF_MAX_LISTENPORTS 40
#define UIP_CONF_BUFFER_SIZE     420
#define UIP_CONF_BYTE_ORDER      UIP_LITTLE_ENDIAN
#define UIP_CONF_TCP      		 1
#define UIP_CONF_TCP_SPLIT       1
#define UIP_CONF_LOGGING         0
#define UIP_CONF_UDP_CHECKSUMS   1

#if UIP_CONF_IPV6
#define UIP_CONF_IPV6_CHECKS     1
#define UIP_CONF_IPV6_QUEUE_PKT  1
#define UIP_CONF_IPV6_REASSEMBLY 0
#define UIP_CONF_NETIF_MAX_ADDRESSES  3
#define UIP_CONF_ND6_MAX_PREFIXES     3
#define UIP_CONF_ND6_MAX_NEIGHBORS    4
#define UIP_CONF_ND6_MAX_DEFROUTERS   2
#define UIP_CONF_ICMP6           1
#endif /* UIP_CONF_ICMP6 */

typedef unsigned long clock_time_t;

#define CLOCK_CONF_SECOND 1000

#define LOG_CONF_ENABLED 1

/* Not part of C99 but actually present */
int strcasecmp(const char*, const char*);

#endif /* __CONTIKI_CONF_H__ */
