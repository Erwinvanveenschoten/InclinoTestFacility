

#include <DAVE.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <delay.h>

#include "UDP.h"
#include "BUS_IO_GP.h"

#ifdef ENABLE_UDP
uint8_t packetCount = 0;				/** Counter to indicate how many UDP packets are needed to transmit the whole struct through UDP **/

uint8_t cycleNr = 0;					/** Variable to store the ID of the current distance cycle **/

static uint32_t desiredTemp = 0;

void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port){
      if (p != NULL) {
            //udp_sendto(pcb, p, &returnaddr, 20000); //dest port
            memcpy(UDPBuffer, p->payload, 1);
            desiredTemp = UDPBuffer[0];
            pbuf_free(p);
      }
}

int getDesiredTemp(){
	return desiredTemp;
}

/**
 * This is a function to initialize some variables which are necessary for UDP communication.
 */
void udp_initialize()
{
	// Disable reset of ethernet chip (active-low)
	BUS_IO_GP_set(PHY_RESET_PIN);

	// Make sure netif is properly set up
	netif_set_link_up(netif_default);
	netif_set_up(ETH_LWIP_0.xnetif);

	// Set ip address
	IP_ADDR4(&returnaddr, IP1,IP2,IP3,IP4);

	// Create new UDP program control block
	com_pcb = udp_new();

	// Bind ip address and port to UDP channel
	err_t udp_bind_err = udp_bind(com_pcb, IP_ADDR_ANY , PORT_COM_OUT);
	if ( udp_bind_err != ERR_OK)
	{
		printf("UDP binding returned with error status: %d\n\r", udp_bind_err);
	}

	// Register receive callback
	udp_recv(com_pcb, (udp_recv_fn)&udp_echo_recv, NULL);

	// Check system timeouts
	sys_check_timeouts();
}


/**
 * This function is used to fill a UDP packet buffer with a header and part of the struct.
 * The header contains the cycleID that the struct is part of, the total number of packets and the current packet number.
 * This is done so the receiving end can reconstruct the whole struct.
 */
void udp_fillBuffer(uint16_t bufferSize, void* po, uint8_t packetNr, uint8_t totalPackets)
{
	UDPBuffer[0] = cycleNr;
	UDPBuffer[1] = totalPackets;
	UDPBuffer[2] = packetNr;
	memcpy(&UDPBuffer[3],po,bufferSize);
}


/**
 * This function is used to transmit a struct via the UDP protocol
 * The fuction first calculates how many packets the data has to be split into to be send over UDP.
 * This has to be done because each UDP packet cannot go over 1464 bytes due to limitations in the IP protocol.
 * After calculating the amount of packets needed to send the whole struct over, the function start transmits the data using UDP.
 * This is done by allocating enough space for 1 packet and filling this packets before transmitting it through UDP.
 * After each packet the allocated gets freed.
 * After all packets are transmitted the sys_check_timeouts function is called to make sure they are pushed onto the network.
 */
err_t udp_printStruct(void * po, uint32_t size)
{
	sys_check_timeouts();
	packetCount = 0;
	packetCount = (size) / ((maxPayloadSize-3)); // - UDP_OFFSET
	uint16_t dataLeft = (size % (maxPayloadSize - 3));

	struct pbuf* b;
	err_enum_t error; //= ERR_OK;

	uint8_t sampleCount = packetCount + (dataLeft!=0);
	for(int k = 0; k < packetCount; k++)
	{
		udp_fillBuffer((maxPayloadSize-3), po, (uint8_t)k, sampleCount);
		b = pbuf_alloc(PBUF_TRANSPORT, maxPayloadSize, PBUF_RAM);
		b->payload = UDPBuffer;
		error = udp_sendto(com_pcb,  b,  &returnaddr, PORT_COM_OUT);
		pbuf_free(b);
	}
	if(dataLeft != 0)
	{
		udp_fillBuffer((dataLeft + UDP_OFFSET), po, packetCount, sampleCount);
		b = pbuf_alloc(PBUF_TRANSPORT, (dataLeft + UDP_OFFSET), PBUF_RAM);
		b->payload = UDPBuffer;
		error = udp_sendto(com_pcb,  b,  &returnaddr, PORT_COM_OUT);
		pbuf_free(b);
	}

	sys_check_timeouts();
	return error;
}

err_t send_messages(void * po, uint32_t size, uint8_t nrof_elements)
{
	cycleNr=nrof_elements;
	return udp_printStruct(po, size);
}



#endif
