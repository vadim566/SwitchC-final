#pragma once
enum Bool_t { True, False };//boolean type true/false 
typedef enum Bool_t Bool;

typedef struct packet
{
	unsigned int time;//time the packet in portX.in 
	unsigned char da;//Distation address, must converted from integer while recived from the portX.in
	unsigned char sa;//Source address, must converted from integer while recived from the portX.in
	unsigned char prio;//priority of the packet 0 is High 1 is Low
	unsigned char data_length;//length (posetive intger) of data frame in the packet
	char *data;//data array in the length of data_length size, need mem allocation by the size of data_length
	unsigned char checksum;//validation tool of the packet , also xor between da^sa^prio^data_length^{data0^data1^...^datan}

}Packet;//packets struct

typedef struct route_node
{
	unsigned char da;//Distation address,
	unsigned char output_port;//Distation address,
	struct route_node *left;//left brench of the root
	struct route_node *right;//right brench of the root
}S_node;//node struct in binary tree

typedef struct pkt_node //queue of packets
{
	packet *pkt;
	struct pkt_node *next;
} S_pkt;

typedef struct Out_Qs_mgr //queue of packets by priority field of packets struct
{
	struct pkt_node *head_p1;
	struct pkt_node *tail_p1;
	struct pkt_node *head_p0;
	struct pkt_node *tail_p0;
} S_Out_Qs_mgr;


