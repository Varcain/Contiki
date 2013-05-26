#ifndef __CONTIKI_CONF_H__
#define __CONTIKI_CONF_H__

#include <stdint.h>

#define	 QUEUEBUF_CONF_NUM						20

#define NETSTACK_CONF_RDC_CHANNEL_CHECK_RATE	32

#define CSMA_CONF_MAX_MAC_TRANSMISSIONS			10
#define CSMA_CONF_MAX_NEIGHBOR_QUEUES			10

#define NETSTACK_CONF_RADIO		xbee_radio_driver
#define NETSTACK_CONF_NETWORK	rime_driver
#define NETSTACK_CONF_FRAMER	framer_nullmac
#define NETSTACK_CONF_MAC		csma_driver

#define	 RIME_CONF_BROADCAST_ANNOUNCEMENT_BUMP_TIME	CLOCK_SECOND * 1
#define RIME_CONF_BROADCAST_ANNOUNCEMENT_MIN_TIME	CLOCK_SECOND * 8
#define RIME_CONF_BROADCAST_ANNOUNCEMENT_MAX_TIME  CLOCK_SECOND * 8

#define CONNECTIONS 					4

#define MTU_SIZE 						512
#define UIP_CONF_LLH_LEN 				0
//#define PACKETBUF_CONF_SIZE
//#define PACKETBUF_CONF_HDR_SIZE
//#define UIP_CONF_TCP_MSS
//#define UIP_CONF_RECEIVE_WINDOW

#define WITH_UIP 						1
#define WITH_UIP6 						0
#define UIP_CONF_IPV6 					0

#define UIP_CONF_UDP             1
#define UIP_CONF_MAX_CONNECTIONS 20
#define UIP_CONF_MAX_LISTENPORTS 20
#define UIP_CONF_BUFFER_SIZE     (UIP_CONF_LLH_LEN + MTU_SIZE)
#define UIP_CONF_BYTE_ORDER      UIP_LITTLE_ENDIAN
#define UIP_CONF_TCP      		  1
#define UIP_CONF_TCP_SPLIT       1
#define UIP_CONF_LOGGING         0
#define UIP_CONF_UDP_CHECKSUMS   1
#define UIP_CONF_IP_FORWARD		  1
#define UIP_CONF_REASSEMBLY		  1

typedef unsigned long clock_time_t;

#define CLOCK_CONF_SECOND 1000

#define LOG_CONF_ENABLED 1

/* Not part of C99 but actually present */
int strcasecmp(const char*, const char*);

typedef uint8_t   u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef  int32_t s32_t;
typedef unsigned short uip_stats_t;

#define CC_CONF_REGISTER_ARGS          1
#define CC_CONF_FUNCTION_POINTER_ARGS  1
#define CC_CONF_FASTCALL
#define CC_CONF_VA_ARGS                1
#define CC_NO_VA_ARGS					0

#define CCIF
#define CLIF

#endif /* __CONTIKI_CONF_H__ */
