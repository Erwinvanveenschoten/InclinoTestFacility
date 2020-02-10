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

/**
 * A vector storing the result angle data of the complement filter.
 */
typedef struct
{
	double x,y,z;
} complement_filter_angle_vector2;

/**
 * Struct to store relevant sensor data
 */
/*
typedef struct
{
	uint32_t pointTotal;
	uint32_t forwardOffset;
	complement_filter_angle_vector2 result_angle_mat;
	uint16_t pointDistance[4000];
} points;*/

typedef struct
{
	uint8_t packetA;
	uint8_t packetB;
	uint8_t packetC;
	uint8_t packetD;
	uint8_t packetE;
	uint8_t packetF;
	uint8_t packetG;
	uint8_t packetH;
} pakketje;

void udp_initialize();
void udp_fillBuffer(uint16_t, pakketje*, uint8_t, uint8_t);
err_t udp_printStruct(pakketje *po);
void udp_receive();



#endif
