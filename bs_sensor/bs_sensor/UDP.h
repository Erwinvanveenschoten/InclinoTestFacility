#define ENABLE_UDP

#ifdef ENABLE_UDP

#ifndef UDP_H_
#define UDP_H_

#define ip1 192							/** Firstbyte of the IP address **/
#define ip2 168							/** Second byte of the IP address **/
#define ip3 0							/** Third byte of the IP address **/
#define ip4 9							/** Fourth byte of the IP address **/

#define UDP_OFFSET 3					/** UDP packet header size **/

#define maxPayloadSize (1536-72)		/** The maximum amount of bytes an UDP packet can contain **/

#define PORT_COM_OUT    52256			/** The port at which the receiver receives the UDP packets **/
#define PORT_COM_IN     62509			/** The port at which the chip transmits the UDP packets **/

struct udp_pcb*  com_pcb;				/** Struct for storing necessary data for UDP **/

uint8_t* UDPBuffer;						/** Array to contain the UDP packet data **/

ip_addr_t returnaddr;					/** Variable to contain the address of the chip which is sent with and UDP packet **/

extern uint8_t packetCount;				/** Counter to indicate how many UDP packets are needed to transmit the whole struct through UDP **/

extern uint8_t cycleNr;					/** Variable to store the ID of the current distance cycle **/

void udp_initialize();
void udp_fillBuffer(uint16_t bufferSize, void * po, uint8_t packetNr, uint8_t totalPackets);
err_t udp_printStruct(void * po, uint32_t size);
void udp_receive();



#endif

#endif
