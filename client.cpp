#include <stdio.h>	/* input/output */
#include <string.h> 	/* memset */
#include <stdlib.h> 	/* exit() */
#include <sys/types.h>	/* Data types used in system calls ???  */
#include <sys/socket.h>	/* For creating, binding, and listening to sockets */
#include <netinet/in.h> /* sockaddr_in */

using namespace std;

/* Print the IP address in a standard deimal dotted format */
void printAddress(unsigned char *inp){
	printf("%d.%d.%d.%d\n", inp[0], inp[1], inp[2], inp[3]);
}

int main(int argc, char * argv[]){

	int clientFD, serverPort, serverIP, receiver;
	int buffLength = 2056;
	//int versionNum, idSeq;

	char route[] = "0 1 3 7";
	char buffer[buffLength];

	//sockaddr_in is used to specify an address to connect a socket to
	struct sockaddr_in clientAddr;
	struct sockaddr_in serverAddr; 

	socklen_t addrLen = sizeof(serverAddr); //length of address

	//Check that the destination address
	if (argc < 4){
		printf("%s%s%s", "Correct usage: ", argv[0], " [destination IP] [portnum], [request/response/update]\n");
		exit(1);
	}
	
	//Take the command line argument
	serverIP = atoi(argv[1]);
	serverPort = atoi(argv[2]);
	
	//Format message
	//Version Number, seq id, Type of message (request, response, update), 
	char *type = argv[3];
	char message[] = "1234 0 ";
	strcat(message, type);

	//Create UDP socket and check that it opened
	clientFD = socket(AF_INET, SOCK_DGRAM, 0);
	if (clientFD < 0){
		printf("%s", "Error opening socket!\n");
		exit(1);
	}
	

	//Fill in the client's address and data
	memset((char *)&clientAddr, 0, sizeof(clientAddr));
	clientAddr.sin_family = AF_INET; //Address family
	clientAddr.sin_port = htons(0); //Doesnt need a well defined port
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //socket address = Ip addr of this machine

	//Fill in the server's address and data
	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_addr.s_addr = htonl(serverIP);

	//Bind socket
	if (bind(clientFD, (struct sockaddr *)&clientAddr, sizeof(clientAddr)) < 0){
		printf("%s", "Error on binding!\n");
		exit(1);
	}
	
	//Print out own initial routing table
	printf("Router0 initial routing table %s\n", route);


	//Send message to server (router0)
	printf("Sending Routing table request to R1 at [<%i>, <%i>]\n", serverIP, serverPort);
	if (sendto(clientFD, message, strlen(message), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		printf("%s", "Send to failed!\n");
		exit(1);
	}
	
	//Receive an acknowledgement / response from the server (router1)
	
	receiver = recvfrom(clientFD, buffer, buffLength, 0, (struct sockaddr *)&serverAddr, &addrLen);
	if (receiver >=0){
		polling = false;
		buffer[receiver]=0;
	}

	printf("received message:\n \"%s\" \n", buffer);

	//Print routing table from Router1
	printf("Received routing table response from R1 at [<%i>, <%i>]\n", serverIP, serverPort);

	

	return 0;
}





//1 Client proints its own initial routing table

//2 Client requests routing update from server, Server responds with its initial table. 
//  Client prints servers response.

//3 Client advertises its routing update to the server asynchronously.

//4 Include "trace" output on the screen prior to the client sending requests and after receiving
//  Responses so tthat it is clearer what the client is doing. (read document!)
