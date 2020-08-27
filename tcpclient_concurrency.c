#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define bufsize 1024

int main(argc, argv) int argc; char *argv[];{
  int sock, rval;
  struct hostent *host;
  struct sockaddr_in server;  // not a pointer.
  char buf[bufsize];
  
  if(argc != 4){
    printf("usage:\ntcpclient hostname port string\n\n");
    exit(0);
  }
  
  // look up hostname (server) using DNS
  if ((host = gethostbyname(argv[1])) == 0) {
    fprintf(stderr, "%s: unknown host\n", argv[1]); 
    exit(0); 
  }
  
  // Set up fields for socket to point to host and port
  bcopy(host->h_addr, &server.sin_addr, host->h_length);
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
 
  // running loop to fork process 5 times and connect to server 5 times concurrently
  for(int i = 0; i < 5; i++){  
    
    int id = fork(); // creating a child process running simultaneously to the parent process
    
    // when id == 0, this is the child and needs to go through the loop 
    // when if > 0, this is the parent, and it should just loop around,
    // when id < 0, we had an error.
    if(id > 0){
      continue;
    }
    else if(id < 0){
      printf("Error\n");
      exit(0);
    }    
    
    // Create socket
    // checking result not less than 0 (error)
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
      printf("Socket Creation Failed\n");
      exit(0);
    }
  
    // connect (3-way handshake)
    // checking result not less than 0 (error)
    if(connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) { 
      printf("Connection Failed\n"); 
      exit(0); 
    } 
  
    // Copy the arg into buf so we can send it to the server
    strncpy(buf, argv[3], bufsize);
    
    // print message that we're sending to the server
    printf("Message for server: %s\n", buf);
 
    // Send sentence to server
    send(sock, buf, strlen(buf), 0);
    printf("Message sent\n");
  
    // read response from server
    rval = read(sock, buf, bufsize);
  
    // print result to window
    printf("Message from server: %s\n", buf);
  
    close(sock);
    exit(0);
  }
  
  return 0;
}

