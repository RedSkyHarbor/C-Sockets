#include <stdio.h> 	/* input/output */
#include <string.h>	/* memset */
#include <stdlib.h>	/* exit() */
#include <unistd.h>	/* ??? */
#include <sys/types.h> 	/* Data types used in system calls ??? */
#include <sys/socket.h>	/* For creating, binding, and listening to sockets */
#include <netinet/in.h>	/* sockaddr_in */


using namespace std;

int main(int argc, char *argv[]){

	
	short int portNum;
	int serverFD, receiver;
	int messageCount = 0;
	int buffSize = 2048;

	//sockaddr_in is used to specify an address to connect a socket to
	struct sockaddr_in serverAddr, clientAddr;

	socklen_t addrLen = sizeof(clientAddr); //length of adresses

	//Reads chars from server to buffer
	char buffer[buffSize]; //unsigned 
	memset((char *)&buffer, 0, sizeof(buffer));

	//Check that a port number is provided from the user
	if (argc < 2){
		printf("%s%s%s", "Correct usage: ", argv[0], " [port number]\n");
		exit(1);
	}

	//Create UDP socket and check that it opened
	serverFD = socket(AF_INET, SOCK_DGRAM, 0);
	if (serverFD < 0 ) {
		printf("%s", "Error openining socket!\n");
		exit(1);
	}

	//The port number the server will listen to for connections
	portNum = atoi(argv[1]);

	//Declare values for serverAddr
	memset((char *)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET; //address family
	serverAddr.sin_port = htons(portNum); //port number/transport address
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //socket address
	
	//Bind socket
	if (bind(serverFD, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
		printf("%s","Error on binding!");
		exit(1);
	}
	
	//Inifinite loop receiving data and print what is received
	while (true){
		
		printf("%s", "Waiting on port...\n");
		receiver = recvfrom(serverFD, buffer, buffSize, 0, (struct sockaddr *)&clientAddr, &addrLen);
		printf("received %d bytes\n", receiver);
	
		if (receiver > 0){
			buffer[receiver] = 0;
			printf("received message: \"%s\"\n", buffer);
		} else {
			printf("Error receiving message!\n");
		}

		//TODO Read what is in the buffer, handle two cases
		//check header: is it a request or an update?
		//handle each case.


		//Fill the buffer
		//TODO This is not what needs to be sent back, this is temporary!
		sprintf(buffer, "ACK %d", messageCount++); 

		//Send a message (the buffer) back to the client
		printf("sending response \"%s\"\n", buffer);
		if(sendto(serverFD, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, addrLen) < 0){
			printf("Error of sending message to client!\n");
		}

		//TODO Temporary soluion
		if (messagecount > 2){
			break;
		}
	}

	//TODO Connectionless calls
	//read "datagram" from serverFD
	//Write reply to serverFD specifying client address and port number
	//while true read from serverFD to receive message
	//send message back to client from serverFD

	//Close
	close(serverFD);

	return 0;
}


//1 Server prints its ouwn initial routing table

//2 Server waits for at least two messages from client
//  Message may be either a request for a routing update from the server or an asynchronous update
//  The information in the message header should allow the server to distinguish btwn the two cases

//3 Server prints the update it received from client

//4 Include "trace" output analogous to that for the client specified before.
