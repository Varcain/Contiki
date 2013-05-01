#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#include <stdint.h>

#define CC_CONF_REGISTER_ARGS          1
#define CC_CONF_FUNCTION_POINTER_ARGS  1
#define CC_CONF_FASTCALL
#define CC_CONF_VA_ARGS                1
#define CC_NO_VA_ARGS					0

#define WITH_LOGGING 1
#define WITH_CLIENT 1
#define WITH_DNS 1
#define WITH_PFS 1
#define CONNECTIONS 2
#define MTU_SIZE 500

#define WITH_UIP 1
#define WITH_UIP6 0
#define UIP_CONF_IPV6 0

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
#define UIP_CONF_TCP      		  1
#define UIP_CONF_TCP_SPLIT       1
#define UIP_CONF_LOGGING         1
#define UIP_CONF_UDP_CHECKSUMS   1
#define UIP_CONF_LLH_LEN		  0
#define UIP_CONF_IP_FORWARD		  1

typedef unsigned long clock_time_t;

#define CLOCK_CONF_SECOND 1000

#define LOG_CONF_ENABLED 1

/* Not part of C99 but actually present */
int strcasecmp(const char*, const char*);

#define CCIF
#define CLIF

#endif /* __CONTIKI_CONF_H__ */
