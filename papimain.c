#include "papimain.h"

/*
 * p4p1 Main program loop
 ***/
 int main_loop(struct server_info * inf)
 {
 	pid_t pID;
 	int fd[2];
 	bnlisten(inf);

 	pipe(fd);
 	pID = fork();

 	if(pID == 0){	// child has to receive all of the variables
 		close(fd[1]);
		printFirstScreen(inf);

		char readBuf[80];
		char readSock[15];
		int sock = 0;
		serverThread.allDone = 1;
		while (1){

			//Clear the buffer!
			for(int i = 0; i < 80; i++){readBuf[i] = '\0';}
			//Clear reading socket buffer
			for(int i = 0; i < 15; i++){ readSock[i] = '\0'; }

			//read the client IP
			read(fd[0], readBuf, sizeof(readBuf));

			printAcceptedConnection(inf, readBuf);

			//read the client Socket
			read(fd[0], readSock, sizeof(readSock));
			*(serverThread.saved_sockets + sock) = atoi(readSock);
			serverThread.cliNum++;	//update the numofclient character
			if(serverThread.cliNum == '1'){
				pthread_join(serverThread.onConnect[serverThread.connectedTo], NULL);
			}
			sock++;	// keep track of wich pointer to update!
		}
 	} else if (pID < 0) {

 		error("Cant fork", -1);

 	} else { // father
 		close(fd[0]);

		char sockstr[15];

 		int c,  new_s, sock = 0;
 		char * sessionid = "5";


 		c = sizeof(struct sockaddr_in);
 		while( (new_s = accept(inf->s, (struct sockaddr *)&inf->client, (socklen_t*)&c)) && (sock < NUMOCLIENTS) ){

			sprintf(sockstr, "%d", new_s);	// save socket in string

 			inf->hostaddrp = inet_ntoa(inf->client.sin_addr);
 			if(inf->hostaddrp == NULL){
 				error("inet_ntoa()", 1);
 			}

			// host ip for other part of fork !
			sleep(1);
			write(fd[1], inf->hostaddrp, strlen(inf->hostaddrp));

			// socket string for other part of fork !
			sleep(1);
			write(fd[1], sockstr, strlen(sockstr));

			//write to socket the session id to tell p4p1-o if in server or netcat
 			write(new_s, sessionid, strlen(sessionid));
			sock++; // keep track of how many clients connected
 		}
 	}

 	return 0;
 }

/*
 * p4p1 connection handler for user
 ***/
void *connection_handler(void * sock)
{

	int leaveloop = 0;
	//int pbs;

	int t = *(int *) sock;
	int s = *(serverThread.saved_sockets+t);
	char buf[BUFSIZE];
	printf("\n\n\n%d\n\n\n%d\n\n\n", t, serverThread.connectedTo);
	printConHandler(t);

	while(! leaveloop){
		if(t == serverThread.connectedTo){
			printf("IM HERE %d", s);
		}
	}

	return 0;
}

/*
 * Bind and listening function
 **/
void bnlisten(struct server_info * inf)
{

	if( bind(inf->s, (struct sockaddr *)&inf->server, sizeof(inf->server)) < 0 ){
		error("bind failed", 1);
	}

	listen(inf->s, 3);

}

void printFirstScreen(struct server_info * inf)
{

	if(inf->argo.ncr){
		getmaxyx(stdscr, inf->win.row, inf->win.col);

		printlogo(inf);
		mvprintw(((inf->win.row) / 2)-1, (inf->win.col-30)/2, "Hello %s You are", inf->username);
		mvprintw((inf->win.row) / 2, (inf->win.col-35)/2,"listening on %s:%d", inf->ip, inf->portno);
		mvprintw( (inf->win.row)-1, 0, "" );
		refresh();
	} else if(inf->argo.cli){
		printlogo(inf);
		printf("[!] Listening on %s:%d\n", inf->ip, inf->portno);
	}

}

void printAcceptedConnection(struct server_info * inf, char * readBuf)
{
	if(inf->argo.ncr){

		clear();
		printlogo(inf);
		mvprintw(6, 0, "[*] Connection from %s:%d\n", readBuf, inf->portno);
		refresh();

	} else if(inf->argo.cli){

		printf("[*] Connection from %s:%d\n", readBuf, inf->portno);

	}
}

void printConHandler(int t)
{
	if(serverThread.ncurses){
		getmaxyx(stdscr, row, col);
		mvprintw(3, col - (col/2), "[ Initializing Client Conectors no: %d ]\n", t);
		mvprintw(7, 0, "");
		refresh();
	} else if(serverThread.cmd){
		printf("[ Initializing Client Conectors no: %d ]\n", t);
	}
}

void printPrompt(int s)
{
	if(serverThread.ncurses){
		getmaxyx(stdscr, row, col);
		clastrow();
		mvprintw(row-1, 0, "<p4p1 -%d-> ", s);
		refresh();
	} else if (serverThread.cmd) {
		printf("\n<p4p1 -%d-> ", s);
	}
}
