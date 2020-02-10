#include <DAVE.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "Headers/UDP.h"


uint8_t packetCount = 0;				/** Counter to indicate how many UDP packets are needed to transmit the whole struct through UDP **/

uint8_t cycleNr = 0;					/** Variable to store the ID of the current distance cycle **/


void udp_echo_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, struct ip_addr *addr, u16_t port){
      if (p != NULL) {
            udp_sendto(pcb, p, &returnaddr, 20000); //dest port
            memcpy(UDPBuffer, p->payload, 11);
            uint8_t test = UDPBuffer[3];
            pbuf_free(p);
      }
}


/**
 * This is a function to initialize some variables which are necessary for UDP communication.
 */
void udp_initialize()
{
	uint16_t test = 1337;
	IP_ADDR4(&returnaddr, ip1,ip2,ip3,ip4);
	com_pcb = udp_new();

    udp_bind(com_pcb, IP_ADDR_ANY , PORT_COM_OUT);
    udp_recv(com_pcb, udp_echo_recv, test);
}


/**
 * This function is used to fill a UDP packet buffer with a header and part of the struct.
 * The header contains the cycleID that the struct is part of, the total number of packets and the current packet number.
 * This is done so the receiving end can reconstruct the whole struct.
 */
void udp_fillBuffer(uint16_t bufferSize, pakketje* po, uint8_t packetNr, uint8_t totalPackets)
{
	uint8_t* structPtr = (uint8_t*)po;
	UDPBuffer[0] = cycleNr;
	UDPBuffer[1] = totalPackets;
	UDPBuffer[2] = packetNr;
	for(int l = UDP_OFFSET; l < bufferSize; l++)
	{
		UDPBuffer[l] = *((structPtr + ((maxPayloadSize - 3) * packetNr))+(l-UDP_OFFSET));
	}
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
err_t udp_printStruct(pakketje *po)
{
	sys_check_timeouts();
	packetCount = 0;
	packetCount = (sizeof(pakketje)) / ((maxPayloadSize-3)); // - UDP_OFFSET
	uint16_t dataLeft = ((sizeof(pakketje)) % (maxPayloadSize - 3));

	struct pbuf* b;
	err_enum_t error; //= ERR_OK;

	pbuf_init();

	uint8_t sampleCount = packetCount + (dataLeft!=0);
	for(int k = 0; k < packetCount; k++)
	{
		UDPBuffer = (uint8_t*)calloc(maxPayloadSize, 1);
		udp_fillBuffer((maxPayloadSize-3), po, (uint8_t)k, sampleCount);
		b = pbuf_alloc(PBUF_TRANSPORT, maxPayloadSize, PBUF_RAM);
		memcpy(b->payload, UDPBuffer, maxPayloadSize);
		error = udp_sendto(com_pcb,  b,  &returnaddr, PORT_COM_OUT);
		pbuf_free(b);
		free(UDPBuffer);
	}
	if(dataLeft != 0)
	{
		UDPBuffer = (uint8_t*)calloc(maxPayloadSize, 1);
		udp_fillBuffer((dataLeft + UDP_OFFSET), po, packetCount, sampleCount);//allocate extra space for the header
		b = pbuf_alloc(PBUF_TRANSPORT, (dataLeft + UDP_OFFSET), PBUF_RAM);
		memcpy(b->payload, UDPBuffer, (dataLeft + UDP_OFFSET));
		error = udp_sendto(com_pcb,  b,  &returnaddr, PORT_COM_OUT);
		pbuf_free(b);
		free(UDPBuffer);
	}

	sys_check_timeouts();
	return error;
}


