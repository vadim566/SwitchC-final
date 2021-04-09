#pragma once
#ifndef STDIO_H
#include <stdio.h>
#endif // !STDIO_H

#ifndef STDLIB_H
#include <stdlib.h>
#endif // !STDLIB_H

#ifndef STRING_H
#include <string.h>
#endif // !STRING_H

#ifndef STRUCTERS_H
#include "stucters.h"
#endif // !STRUCTERS_H


/*function decleration*/

void freeNode(S_node *node);//Null arguments of the node and free the heap memory


/*function definitions*/
void packet_read(FILE *fp, packet *pkt)
{
	FILE *thisFile = fp;//file to read
	packet *thisPacket = pkt;//packet to write in




	char tmp_DataHandle[100];

	fscanf(thisFile, "%u %d %d %d %d %[^\n]%*c", &thisPacket->time, &thisPacket->da, &thisPacket->sa, &thisPacket->prio, &thisPacket->data_length, tmp_DataHandle);

	thisPacket->data = (char*)calloc(thisPacket->data_length, sizeof(char));//exactly memory allocation by the number of the object in the data
	if (feof(fp))
	{
		return;
	}
	/*split the tmp_dataHandle by spaces  to data and checksum*/

	/*some variables for simplicty*/
	char *thisData = thisPacket->data;

	/*define token for strtok to split the tmp_dataHandle*/
	char *token = strtok(tmp_DataHandle, " ");

	// loop through the string to extract all data and checsums from tokens
	int i = 0;
	for (i; i < thisPacket->data_length + 1; i++)
	{
		char tmp[20];//tmpory array for handling the token
		strcpy(tmp, token);//copy the token into tmp
		thisData[i] = atoi(tmp);//convert tmp into int and store as a charcter


		if (i == thisPacket->data_length)//in the last i
		{
			char checksum[20];//define another tmp array for handling token for the checksum
			strcpy(checksum, token);//copy the token into cheksum
			thisPacket->checksum = atoi(checksum);//convert tmp into int and store as a charcter


			thisData[i] = '\0';//close the string of thisData
		}
		token = strtok(NULL, (" "));//move foward to next space the token


	}

}
void packet_write(FILE * fp, packet * pkt)
{
	FILE *thisFile = fp;//file to out
	packet *thisPacket = pkt;//packet to write


	/*print into file until the data*/
	fprintf(thisFile, "%u %d %d %d %d", thisPacket->time, thisPacket->da, thisPacket->sa, thisPacket->prio, thisPacket->data_length);

	/*print the data as integers*/
	int i = 0;
	for (i; i < thisPacket->data_length; i++)
	{
		fprintf(thisFile, " %d", thisPacket->data[i]);
	}

	/*print the check sum*/
	fprintf(thisFile, " %d\n", thisPacket->checksum);


}
Bool checksum_check(const packet * pkt)
{


	const Packet *thisPacket = pkt;//simplicity

	int checkSum_tmp = thisPacket->da ^ thisPacket->sa ^ thisPacket->prio ^thisPacket->data_length;//xor between the number


	int i = 0;

	for (i; i < thisPacket->data_length; i++)
	{
		checkSum_tmp ^= thisPacket->data[i];//keep xor
	}






	if (checkSum_tmp == (int)(thisPacket->checksum))//if true
	{

		return True;
	}
	else
	{

		return False;
	}

}
void freePacket(packet *pkt)
{
	/*Null value and free*/
	pkt->checksum = NULL;
	pkt->da = NULL;
	pkt->sa = NULL;
	pkt->prio = NULL;
	int i = 0;
	for (i; i < pkt->data_length; i++)
	{
		pkt->data[i] = NULL;
	}
	pkt->data_length = NULL;
	pkt->time = NULL;

	free(pkt);

}
void freePacket_withoutmem(packet *pkt)
{
	/*Null value and free*/
	pkt->checksum = NULL;
	pkt->da = NULL;
	pkt->sa = NULL;
	pkt->prio = NULL;
	int i = 0;
	for (i; i < pkt->data_length; i++)
	{
		pkt->data[i] = NULL;
	}
	pkt->data_length = NULL;
	pkt->time = NULL;

	//free(pkt);

}
S_node * add_route(S_node * root, char da, char output_port)
{

	/*simplicty*/
	S_node *thisRoot = root;
	unsigned char thisDa = (unsigned char)da;
	unsigned char thisOutPort = (unsigned char)output_port;


	/*if this tree root is empty*/
	if (thisRoot == NULL)
	{
		thisRoot = (S_node*)calloc(1, sizeof(S_node));
		thisRoot->da = (unsigned char)thisDa;
		thisRoot->output_port = (unsigned char)thisOutPort;
		return thisRoot;
	}

	/*if the tree isnt empty*/

	/*if its the same as da in the root*/
	if (thisRoot->da == thisDa)
	{
		return thisRoot;
	}

	/*if the da is smaller then the root */
	if (thisRoot->da > thisDa)
	{
		thisRoot->left = add_route(thisRoot->left, thisDa, thisOutPort);//recourse to left
		return thisRoot;
	}
	/*if the da is larger then the root */
	if (thisRoot->da < thisDa)
	{
		thisRoot->right = add_route(thisRoot->right, thisDa, thisOutPort);//recourse to right
		return thisRoot;
	}




}
S_node * delete_route(S_node * root, char da)
{

	/*simplicty*/
	S_node *thisRoot = root;
	unsigned char thisDa = (unsigned char)da;


	/*if the tree is empty*/
	if (thisRoot == NULL)
		return NULL;

	/*if the tree isnt empty*/

	/*if its the same as da in the root*/
	if (thisRoot->da == thisDa)
	{
		S_node *thisLeft = thisRoot->left;//holding the point of left
		S_node *thisRight = thisRoot->right;//holding the pont of right

		if (thisLeft != NULL) //only if the left child exist
		{
			/*if left dont have a right child */
			if (thisLeft->right == NULL)
				thisLeft->right = thisRight;



			/*if left have right child*/
			else
			{
				while (thisLeft->right != NULL)
				{
					thisLeft = thisLeft->right;//go until the last child in the tree of the right brench
				}
				thisLeft->right = thisRight;//put the right child in the end of the right brench

				thisLeft = thisRoot->left;

			}

			/*free memory*/
			freeNode(root);

			return thisLeft;
		}
		else
		{
			if (thisRight != NULL)//only if the right child exist
			{
				/*free memory*/
				freeNode(root);

				return thisRight;

			}
			root->da = NULL;
			root->output_port = NULL;
			free(root);
			return NULL;//if there are no childrens
		}
	}
	/*if the da is smaller then the root */
	if (thisRoot->da > thisDa)
	{
		thisRoot->left = delete_route(thisRoot->left, thisDa);
		return thisRoot;
	}
	/*if the da is larger then the root */
	if (thisRoot->da < thisDa)
	{
		thisRoot->right = delete_route(thisRoot->right, thisDa);
		return thisRoot;
	}


}
void freeNode(S_node * node)
{
	node->da = NULL;
	node->output_port = NULL;
	node->right = NULL;
	node->left = NULL;
	free(node);

}
S_node * search_route(const S_node * root, char da)
{
	/*simplicty*/
	S_node *thisRoot = (S_node*)root;
	unsigned char thisDa = (unsigned char)da;

	/*if this tree root is empty*/
	if (thisRoot == NULL)
	{
		return NULL;
	}

	/*if its the same as da in the root*/
	if (thisRoot->da == thisDa)
	{
		return (S_node*)thisRoot;
	}

	/*if the da is smaller then the root */
	if (thisRoot->da > thisDa)
	{

		return search_route(thisRoot->left, thisDa);
	}
	/*if the da is larger then the root */
	if (thisRoot->da < thisDa)
	{

		return search_route(thisRoot->right, thisDa);
	}

}
void print_routing_table(const S_node * root)
{
	if (root == NULL)
	{
		printf("\n\t");
		return;

	}
	/*in order print Binary tree*/
	print_routing_table(root->left);
	printf("DA: %d ,Port_out:%d", root->da, root->output_port);
	print_routing_table(root->right);

}
S_node * build_route_table(FILE * fp, S_node * root)
{
	/*simplicty*/
	FILE *thisFile = fp;
	S_node * thisRoot = root;
	root = (S_node*)calloc(1, sizeof(S_node));
	unsigned char lineHandle[6];
	int i = 0, num_lines = 0;

	/*add objects in the tree*/
	while (!feof(thisFile))
	{

		fscanf(thisFile, " %c %u %u", &lineHandle[0], &lineHandle[1], &lineHandle[2]);
		num_lines++;
		if (lineHandle[0] == 'a')
		{
			thisRoot = add_route(thisRoot, lineHandle[1], lineHandle[2]);
		}
		if (lineHandle[0] == 'd')
		{
			thisRoot = delete_route(thisRoot, lineHandle[1]);

		}

	}



	/*relocate and check the root of the tree*/

	for (i; i < num_lines; i++)
	{
		lineHandle[0] = thisRoot->da;//keep the information of the root
		lineHandle[1] = thisRoot->output_port;


		thisRoot = delete_route(thisRoot, thisRoot->da);//remove the root
		thisRoot = add_route(thisRoot, lineHandle[0], lineHandle[1]);//return the root to its matched position in the tree
	}

	return thisRoot;
}
void enque_pkt(S_Out_Qs_mgr * QM_ptr, packet * pkt)
{
	/*simplicty*/
	S_Out_Qs_mgr *thisQueuMgr = QM_ptr;
	packet *thisPacket = pkt;


	/*build the queue*/
	if (thisPacket->prio == 0)//classify by the priority 0
	{
		if (checksum_check(thisPacket) == True)//if the checksum is TRUE
		{
			//check if the queue is mngr exist but emptyof nodes
			if (!thisQueuMgr->head_p0)
			{
				pkt_node *thisQ0 = (pkt_node*)calloc(1, sizeof(pkt_node));//create the first node

				thisQ0->pkt = thisPacket;

				thisQ0->next = NULL;

				thisQueuMgr->head_p0 = thisQ0;//set head this q0
				thisQueuMgr->tail_p0 = thisQ0;//set tail this q0
				thisQueuMgr->tail_p0->next = NULL;



			}
			else//not empty of nodes
			{
				pkt_node *thisQ0 = (pkt_node*)calloc(1, sizeof(pkt_node));//create the new node

				//add to end of the queue

				thisQ0->pkt = thisPacket;//put this packet value in the queue1
				thisQ0->next = NULL;


				thisQueuMgr->tail_p0->next = thisQ0;//connect thisQ0 to the tail
				thisQueuMgr->tail_p0 = thisQ0;//set tail








			}

		}
		else//if the CHECKSUM IS FALSE
		{
			freePacket(thisPacket);//free memory function
			return;
		}
	}



	if (thisPacket->prio == 1)//classify by the priority 1 
	{
		if (checksum_check(thisPacket) == True)//if the checksum is TRUE
		{

			//check if the queue is mngr exist but emptyof nodes
			if (!thisQueuMgr->head_p1)
			{

				pkt_node *thisQ1 = (pkt_node*)calloc(1, sizeof(pkt_node));//create the first node

				thisQ1->pkt = thisPacket;

				thisQ1->next = NULL;

				thisQueuMgr->head_p1 = thisQ1;//set head this q1
				thisQueuMgr->tail_p1 = thisQ1;//set tail this q1
				thisQueuMgr->tail_p1->next = NULL;



			}
			else//not empty of nodes
			{
				pkt_node *thisQ1 = (pkt_node*)calloc(1, sizeof(pkt_node));//create the new node

				//add to end of the queue

				thisQ1->pkt = thisPacket;//put this packet value in the queue1
				thisQ1->next = NULL;


				thisQueuMgr->tail_p1->next = thisQ1;//connect thisQ1 to the tail
				thisQueuMgr->tail_p1 = thisQ1;//set tail






			}
		}
		else//if the CHECKSUM IS FALSE
		{
			freePacket(thisPacket);//free memory function
			return;
		}

	}


}
packet * deque_pkt(S_Out_Qs_mgr * QM_ptr, char priority)
{
	/*simplicty*/
	S_Out_Qs_mgr *thisQueuMgr = QM_ptr;
	char thisPriority = priority;
	packet *tmpPacket;


	if (thisPriority == 0)//which priority queue to deque, prio=0
	{
		if (thisQueuMgr->head_p0 == thisQueuMgr->tail_p0->next)//if the head is next of the tail its as the same the queue is empty
			return NULL;

		tmpPacket = thisQueuMgr->head_p0->pkt;//release the first to return
		thisQueuMgr->head_p0 = thisQueuMgr->head_p0->next;//set head as the next one in the list
		return tmpPacket;
	}

	if (thisPriority == 1)//which priority queue to deque, prio=1
	{
		if (thisQueuMgr->head_p1 == thisQueuMgr->tail_p1->next)//if the head is next of the tail its as the same the queue is empty
			return NULL;

		tmpPacket = thisQueuMgr->head_p1->pkt;//release the first to return
		thisQueuMgr->head_p1 = thisQueuMgr->head_p1->next;//set head as the next one in the list
		return tmpPacket;

		/*free() of these returned packets data types will be in the main function*/
	}


}
void copyPacket(const packet * pkt, packet *dstPkt)
{
	dstPkt->time = pkt->time;
	dstPkt->da = pkt->da;
	dstPkt->sa = pkt->sa;
	dstPkt->prio = pkt->prio;
	dstPkt->data_length = pkt->data_length;
	dstPkt->checksum = pkt->checksum;

	dstPkt->data = (char*)calloc(dstPkt->data_length, sizeof(char));
	char *desData = dstPkt->data;
	char *souData = pkt->data;


	int i = 0;

	for (i; i < dstPkt->data_length; i++)
	{
		desData[i] = souData[i];
	}
	desData[i] = '\0';



}

