
/******************************************************************************************/
/***********************switch-emulator**Exam-Project*************************************/
/****************************************************************************************/
/*author1:David Musaev 319455119********************************************************/
/*author2:Shaked Spector 308132281*****************************************************/
/*Version 1.0 date:15/07/2020*********************************************************/
/*The program simulate a switch that recive a packet file to each port ,while in this
/*version there are 4 input ports, after the switch emulator will read the 4 input ports
/*it will build a routing table and will deside in which port each packet from input
/*ports will route to. each packet have a QOS element /priority element that will 
/*decide in which queue to put the packet in the output port.
/*In addition the the emulator will check each packet if the packet is not corputed
, it will happend threw XOR on all packet element except the checkSum frame.
after that the emulator will send all the packets by the Time Line (TIME FIFO) and by 
the QOS/priority elemt when the first to out will be these with 
priority 0 and X time after that priority 1 with X time, the product will be exported to files Port i.out .

the emulator can be run from command line by the command EXAMPLE:C:\Users\David\source\repos\switch\Debug\switch.exe route.txt port1.in port2.in port3.in port4.in
while 
switch.exe - name of the EXE file
route.txt - logic of the routing table - 1st column:a= add ,d =delete , 2nd column:da elemnt/frame ,3rd column(exist only while cloumn 1==a) output port
porti.in- i=1 or 2 or 3 or 4 the input file that simulate packet in the in ports 
each packet has frame of  [Time][Source Adress][Destination Adress][Priority][Data Length][Data(size of Data Length)][checksum]

after each use the emulator will free all the memory that was heap located for the emulator*/


#define _CRT_SECURE_NO_WARNINGS
#define TEST_TIME 3000
#define MAX_SIZE 1000
#define MAX_TIME 10000000000
#ifndef STRUCTERS_H
#include "stucters.h"
#endif // !STRUCTERS_H

#ifndef FUNCTIONS_H
#include "functions.h"
#endif // !FUNCTIONS_H

#ifndef STDIO_H
#include <stdio.h>
#endif // !STDIO_H

#ifndef STDLIB_H
#include <stdlib.h>
#endif // !STDLIB_H

#ifndef STRING_H
#include <string.h>
#endif // !STRING_H








/*functions decelerations*/

void packet_read(FILE *fp, packet *pkt);//read a line from a file portX.in
void packet_write(FILE *fp, packet *pkt);//write a line from a file portX.in
Bool checksum_check(const packet *pkt);//checking xor on all particals of the packet and compare it to checksum field if even so the value will true else false


S_node * add_route(S_node *root, char da, char output_port);//add a node to a tree for route table
S_node *delete_route(S_node *root, char da);//delete a node from a tree
void freeNode(S_node *node);//Null arguments of the node and free the heap memory
S_node *search_route(const S_node *root, char da);//search a node in binary tree by DA verfication
void print_routing_table(const S_node *root);//print a binary tree by in order
S_node * build_route_table(FILE *fp, S_node *root);//build route table matched to binary tree from route file
void freePacket(packet *pkt);//free memory of packet data type
void freePacket_withoutmem(packet * pkt);//free memory of packet data type wihtout FREE function

void enque_pkt(S_Out_Qs_mgr *QM_ptr, packet *pkt);//put a packet by priority in the last place in the queue
packet *deque_pkt(S_Out_Qs_mgr *QM_ptr, char priority);//export and clear the packet
void copyPacket(const packet * pkt, packet * dstPkt);//copy packet structer



/*main function*/
int main(int argc, char *argv[])
{
	/*file handle input  argv[0]=filename , argv[1]= file argumnet 1 ,,,,*/

	printf("\n%s", argv[1]);
	printf("\n%s", argv[2]);
	printf("\n%s", argv[3]);
	printf("\n%s", argv[4]);
	printf("\n%s", argv[5]);
	

	 FILE *route = fopen(argv[1], "r+");//open route.txt
	FILE *port1In = fopen(argv[2], "r+");//open port1.in
	FILE *port2In = fopen(argv[3], "r+");//open port2.in
	FILE *port3In = fopen(argv[4], "r+");//open port3.in
	FILE *port4In = fopen(argv[5], "r+");//open port4.in

	/*building routing table*/
	S_node *rootTree=NULL;
	rootTree=build_route_table(route, rootTree);
	printf("\nbuilding the tree");
	printf("\nprinted the tree");
	print_routing_table(rootTree);
	

	/*input and logic*/
	/*array of packets that holding the information*/
	packet *infoIn[MAX_SIZE];

	/*inputs for the ports that reciving info from port1.in port2.in etc..*/
	infoIn[0]= (packet*)calloc(1, sizeof(packet));
	infoIn[1] = (packet*)calloc(1, sizeof(packet));
	infoIn[2] = (packet*)calloc(1, sizeof(packet));
	infoIn[3] = (packet*)calloc(1, sizeof(packet));
	
	//create 4 queue for each port out and dedicate a memory
	S_Out_Qs_mgr
		*QueuePo0=(Out_Qs_mgr*)calloc(1, sizeof(S_Out_Qs_mgr)),
		*QueuePo1 = (Out_Qs_mgr*)calloc(1, sizeof(S_Out_Qs_mgr)), 
		*QueuePo2 = (Out_Qs_mgr*)calloc(1, sizeof(S_Out_Qs_mgr)), 
		*QueuePo3 = (Out_Qs_mgr*)calloc(1, sizeof(S_Out_Qs_mgr));

	//initialize counters 
	int i = 0, k = -1,j=4;
	
	
	while (port1In || port2In || port3In || port4In)//while 1 of these is still not null and exist
	{

		/* initilization for time check*/
		/*the check is a competition between 4 in ports that the one who was sent earlier will get in the queue first, 
		only the one port that sent the packet is refreshing and now have a new packet to put in the time competition*/
		infoIn[j] = (packet*)calloc(1, sizeof(packet));//j is 4 for the infoin(first round only after this j++) that will be sent first to the route table and will be exported to the file if the checksum is TRUE
		infoIn[j]->time = MAX_TIME;//max value
		
		//read 4 ports in parrler,k=-1 is initial state and k=0 ,1,2,3 is the matched number of port that it was enter from
		//if the file of ports is at the end of file put max time(MAX_TIME) for elimnate these ports from check

		if (port1In && (k==0 || k==-1))
		{
			if (feof(port1In))
			{
				port1In = NULL;
				infoIn[0]->time = MAX_TIME;
			}
			else
				packet_read(port1In, infoIn[0]);

		}
		if (port2In &&  (k == 1 || k == -1))
		{
			if (feof(port2In))
			{
				port2In = NULL;
				infoIn[1]->time = MAX_TIME;
			}
			else
			packet_read(port2In, infoIn[1]);
		}
		if (port3In &&  (k == 2 || k == -1))
		{
			if (feof(port3In))
			{
				port3In = NULL;
				infoIn[2]->time = MAX_TIME;
			}
			else
			packet_read(port3In, infoIn[2]);
		}
		if (port4In && (k == 3 || k == -1))
		{
			if (feof(port4In))
			{
				port4In = NULL;
				infoIn[3]->time = MAX_TIME;
			}
			else
			packet_read(port4In, infoIn[3]);
		}

		
			if (k == -1)//initial state for creating first obj to compare it
		{
			copyPacket(infoIn[0], infoIn[j]);//first time only
		
			k = 0;
		}
		


		/*find in which time the packet was sent*/

		//compare time and save the index with minimal time bettween the 4 cells and keep it on the 4th index
		for (i = 0; i < 4; i++)
		{
			
				
			if (infoIn[j]->time > infoIn[i]->time)
			{
				
				copyPacket(infoIn[i], infoIn[j]);//copy save the index that matched to infoin array
				k = i;
				
			}
			
			
		}
		
		S_node *Route_P_IN;//node for entering the queue 
		Route_P_IN =search_route(rootTree, infoIn[j]->da) ;//find its route to out port number




		/*PUT IN QUEUE 1,2,3,4 and priority 0,1*/
	if(Route_P_IN!=NULL)
	{
		switch (Route_P_IN->output_port)
		{
		case 1://port out 1
			enque_pkt(QueuePo0, infoIn[j]);//enque to the matched priority queue
			if (infoIn[j]->da != infoIn[k]->da )
			{
				j--;
			}
			j++;
			break;

		case 2://port out 2
			enque_pkt(QueuePo1, infoIn[j]);
			if (infoIn[j]->da != infoIn[k]->da)
			{
				j--;
			}
			j++;
			break;

		case 3://port out 3
			enque_pkt(QueuePo2, infoIn[j]);
			if (infoIn[j]->da != infoIn[k]->da)
			{
				j--;
			}
			j++;
			break;

		case 4://port out 4
			enque_pkt(QueuePo3, infoIn[j]);
			if (infoIn[j]->da != infoIn[k]->da)
			{
				j--;
			}
			j++;
		

		
		default:
			break;
		}
	}


	
	}

	

	/*OUT PUT FILES*/

	/*create files to out*/
	FILE *port1Out = fopen("port1.out", "w+");//open port1.out
	FILE *port2Out = fopen("port2.out", "w+");//open port2.out
	FILE *port3Out = fopen("port3.out", "w+");//open port3.out
	FILE *port4Out = fopen("port4.out", "w+");//open port4.out

	i = 0;
	while (i<TEST_TIME)//test time can be modify in head of the code
		{
		i++;

		
		//export priority 0 queue in time=i
		if (QueuePo0->head_p0 && i == QueuePo0->head_p0->pkt->time)
		{
			packet_write(port1Out, deque_pkt(QueuePo0, 0));

		}
		if (QueuePo1->head_p0 && i == QueuePo1->head_p0->pkt->time)
		{
			packet_write(port2Out, deque_pkt(QueuePo1, 0));
		}
		if (QueuePo2->head_p0 && i == QueuePo2->head_p0->pkt->time)
		{
			packet_write(port3Out, deque_pkt(QueuePo2, 0));
		}
		if (QueuePo3->head_p0 && i == QueuePo3->head_p0->pkt->time)
		{
			packet_write(port4Out, deque_pkt(QueuePo3, 0));
		}

		
		////export priority 1 queue in time=i
		if (QueuePo0->head_p1 && i == QueuePo0->head_p1->pkt->time)
		{
			packet_write(port1Out, deque_pkt(QueuePo0, 1));
		}
		if (QueuePo1->head_p1 && i == QueuePo1->head_p1->pkt->time)
		{
			packet_write(port2Out, deque_pkt(QueuePo1, 1));
		}
		if (QueuePo2->head_p1 && i == QueuePo2->head_p1->pkt->time)
		{
			packet_write(port3Out, deque_pkt(QueuePo2, 1));
		}
		if (QueuePo3->head_p1 && i == QueuePo3->head_p1->pkt->time)
		{
			packet_write(port4Out, deque_pkt(QueuePo3, 1));
		}


	}

	//save and close the files
	fclose(port1Out);
	fclose(port2Out);
	fclose(port3Out);
	fclose(port4Out);


	
	/*memory free*/
	for (i = 0; i < MAX_SIZE; i++)
	{
	
		if ((infoIn[i]->time > TEST_TIME) && (i > 5))
			break;
		free(infoIn[i]);
	
	}
	freePacket(infoIn[i]);
	free(QueuePo0);
	free(QueuePo1);
	free(QueuePo2);
	free(QueuePo3);
}


