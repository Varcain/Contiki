#include "dev/xbee.h"
#include "dev/uart1.h"
#include "lib/ringbuf.h"
#include "net/netstack.h"
#include "net/packetbuf.h"
#include <string.h> /* for memcpy() */

#define XBEE_DELIMITER 0x7E
#define XBEE_API_RX		0x81
#define XBEE_API_TX		0x01

enum xbee_rcv_states  {
	xbee_state_start,
	xbee_state_length,
	xbee_state_api_id,
	xbee_state_metadata,
	xbee_state_data,
	xbee_state_checksum
};

int c;
/* macro for obtaining next byte for state machine */
#define xbee_getByte()	c = ringbuf_get(&rxbuf); \
						if(c == -1) \
						{ \
							/* Buffer empty, wait for poll */ \
							PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_POLL); \
							continue; \
						}

#define XBEE_PACKET_SIZE	100
#define XBEE_BUFSIZE		128

static struct ringbuf rxbuf;
static uint8_t rxbuf_data[XBEE_BUFSIZE];

int xbee_input_handler(unsigned char c);

PROCESS(xbee_process, "XBee driver");

/*---------------------------------------------------------------------------*/
static int
init(void)
{
	uart1_init(0);
	uart1_set_input(xbee_input_handler);
	ringbuf_init(&rxbuf, rxbuf_data, sizeof(rxbuf_data));
	process_start(&xbee_process, NULL);
	return 0;
}
/*---------------------------------------------------------------------------*/
static int
prepare(const void *payload, unsigned short payload_len)
{
	unsigned char txBuf[108]; // 108 is the maximum buffer size needed
	int ptr;
	unsigned char chksum;

	/* drop the packet if payload is bigger than max XBee packet size */
	if(payload_len > XBEE_PACKET_SIZE)
	{
		/* loop forever for debug purpose */
		while(1);
	}

	/* Prepare XBee API frame here */
	ptr = 0;
	txBuf[ptr++] = XBEE_DELIMITER;
	txBuf[ptr++] = 0;
	txBuf[ptr++] = payload_len + 5;
	txBuf[ptr++] = XBEE_API_TX;
	txBuf[ptr++] = 0; // Frame ID 0, ACK disabled
	txBuf[ptr++] = 0xFF; // broadcast address
	txBuf[ptr++] = 0xFF; // broadcast address
	txBuf[ptr++] = 0;
	memcpy(txBuf+ptr,payload,payload_len);
	ptr += payload_len;
	/* calculate checksum */
	chksum =  0;
	for(int i=3; i<ptr;i++)
	{
		chksum += txBuf[i];
	}
	chksum = 0xFF - chksum;
	txBuf[ptr++] = chksum;
	for(int i=0; i<ptr; i++)
	{
		uart1_writeb(txBuf[i]);
	}
	return 1;
}
/*---------------------------------------------------------------------------*/
static int
transmit(unsigned short transmit_len)
{
	/* We assume XBee hardware handled all the MAC related mumbo-jumbo
	 * correctly, hence we return OK
	 */

	return RADIO_TX_OK;
}
/*---------------------------------------------------------------------------*/
static int
send(const void *payload, unsigned short payload_len)
{
	prepare(payload, payload_len);
	return transmit(payload_len);
}
/*---------------------------------------------------------------------------*/
static int
read(void *buf, unsigned short buf_len)
{
	/* This is not implemented since XBee modules are handling stuff like
	 * ACK in hardware automatically while in API mode.
	 *
	 * All the packet parsing happens in xbee_process.
	 * In general this should never be called so loop forever for debug
	 * purpose.
	 */
	while(1);
	return 0;
}
/*---------------------------------------------------------------------------*/
static int
channel_clear(void)
{
	return 1;
}
/*---------------------------------------------------------------------------*/
static int
receiving_packet(void)
{
	return 0;
}
/*---------------------------------------------------------------------------*/
static int
pending_packet(void)
{
	return 0;
}
/*---------------------------------------------------------------------------*/
static int
on(void)
{
	return 0;
}
/*---------------------------------------------------------------------------*/
static int
off(void)
{
	return 0;
}
/*---------------------------------------------------------------------------*/
const struct radio_driver xbee_radio_driver =
{
		init,
		prepare,
		transmit,
		send,
		read,
		channel_clear,
		receiving_packet,
		pending_packet,
		on,
		off,
};
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(xbee_process, ev, data)
{
	static char buf[XBEE_PACKET_SIZE];
	static int ptr = 0;
	static int rcvState = xbee_state_start;
	static int rcvCnt = 0;
	static unsigned char ourChksum = 0;
	static unsigned int msgLen = 0;
	PROCESS_BEGIN();

	while(1)
	{
		/* This might look ugly since I can't use switch statements inside a protothread
		* state machine is used to handle fragmented packets from XBee module
		* (rare but can occur)
		*/
		if(rcvState == xbee_state_start)
		{
			xbee_getByte();
			if(c == XBEE_DELIMITER)
			{
				rcvCnt++;
				rcvState = xbee_state_length;
			}
		}

		if(rcvState == xbee_state_length)
		{

			if(rcvCnt < 2)
			{
				xbee_getByte();
				msgLen = (unsigned char)c << 8;
				rcvCnt++;
			}
			else
			{
				xbee_getByte();
				msgLen += (unsigned char)c;
				rcvCnt++;
				rcvState = xbee_state_api_id;
			}
		}

		if(rcvState == xbee_state_api_id)
		{
			xbee_getByte();
			if(c == XBEE_API_RX)
			{
				rcvCnt++;
				ourChksum += c;
				rcvState = xbee_state_metadata;
			}
			else
			{
				ptr = 0;
				rcvCnt = 0;
				ourChksum = 0;
				msgLen = 0;
				rcvState = xbee_state_start;
			}
		}

		if(rcvState == xbee_state_metadata)
		{
			/* ignore rf packet metadata for now */
			while(ringbuf_size(&rxbuf) > 0 && rcvCnt < 8)
			{
				xbee_getByte(); // src address high
				ourChksum += c;
				rcvCnt++;
			}
			if(rcvCnt == 8)
			{
				rcvState = xbee_state_data;
			}
		}

		if(rcvState == xbee_state_data)
		{
			while(ringbuf_size(&rxbuf) > 0 && ((rcvCnt - 3) < msgLen))
			{
				xbee_getByte(); // data byte
				ourChksum += c;
				buf[ptr++] = c;
				rcvCnt++;
			}
			if((rcvCnt - 3) == msgLen)
			{
				rcvState = xbee_state_checksum;
			}
		}

		if(rcvState == xbee_state_checksum)
		{
			xbee_getByte();
			ourChksum += c;
			if(ourChksum == 0xFF) /* Checksum is ok */
			{
				packetbuf_clear();
				//packetbuf_set_attr(PACKETBUF_ATTR_TIMESTAMP, last_packet_timestamp);
				memcpy(packetbuf_dataptr(), buf, ptr);
				packetbuf_set_datalen(ptr);
				NETSTACK_RDC.input();
			}
			else
			{
				ourChksum = 0;
			}
			// cleanup
			ptr = 0;
			rcvCnt = 0;
			ourChksum = 0;
			msgLen = 0;
			rcvState = xbee_state_start;
		}
	}

	PROCESS_END();
}

int xbee_input_handler(unsigned char c)
{
	static uint8_t overflow = 0;

	if(!overflow)
	{
		/* Add character */
		if(ringbuf_put(&rxbuf, c) == 0)
		{
			overflow = 1;
		}
	}
	else
	{
		/* Buffer overflowed, no handling at the moment */
		while(1);
		overflow = 0;
	}

	/* Wake up consumer process */
	process_poll(&xbee_process);
	return 1;
}
