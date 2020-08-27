#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h> 
#include <string.h> 
#include <sys/wait.h> 
  
#define bufsize 1024

void eatZombies(int n){
  wait3(NULL,WNOHANG,NULL); 
}

// this function converts the c string passed to it to uppercase
char* uppercase(char* buffer){
  static char uppercase_buffer[bufsize] = {0};
  
  for(int i = 0; i < strlen(buffer); i++){
    char c = *(buffer + i);
    	
    if(97 <= c && c <= 122) // checking that the character is a lower case character
      uppercase_buffer[i] = c - 32; // converting to upper case as all upper case letters are 32 characters below lower case letters on ASCII table
    else
      uppercase_buffer[i] = c;
  }
  
  return uppercase_buffer;
}

int main(int argc, char *argv[]){
  int sock, length, msgsock, status;
  struct sockaddr_in server;

  pid_t id;       
  signal(SIGCHLD, &eatZombies);
 

  // Creates a socket
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Creates the socket addressing structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(atoi(argv[1])); // this time 1st arg is port#
  
  // BINDs the socket.
  // checking result not less than 0 (error)
  if(bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0){
   printf("Error binding the socket\n");
   exit(0);
  }
  
  // Main server loop.

  // Listening
  // checking result not less than 0 (error)
  if(listen(server_fd, SOMAXCONN) < 0){
    printf("Error listening for connections\n");
    exit(0);
  }
  
  char buffer[bufsize] = {0}; 
  int addrlen = sizeof(server);
  int client_fd;
      
  printf("\nWaiting for connection...\n");      
  
  while(1){
    
    // Accepts the connection request
    // checking result not less than 0 (error)
    if((client_fd = accept(server_fd, (struct sockaddr *)&server, (socklen_t*)&addrlen)) < 0){
      printf("Error accepting connection\n");
      exit(0);
    }
    
    printf("Connection made\n\n");    

    // the next call makes a new child process that will actually handle the client.
    id = fork();
    
    // when id == 0, this is the child and needs to do the work for the server. 
    // when if > 0, this is the parent, and it should just loop around,
    // when id < 0, we had an error.
    if(id > 0){
      continue;
    }
    else if(id < 0){
      printf("Error\n");
      exit(0);
    }
    
    // read message from client
    read(client_fd, buffer, bufsize); 
    
    // print message from client
    printf("Message from client: %s\n", buffer);
    
    // convert message from client to upper case
    char* uppercase_buffer = uppercase(buffer);
    
    // print upper case message that we're sending back to the client
    printf("Message for client: %s\n", uppercase_buffer);
    
    // sending upper case message to 
    send(client_fd, uppercase_buffer, strlen(uppercase_buffer), 0);
    
    printf("\nWaiting for connection...\n"); // this will print for the parent process      
    
    exit(0); // closing child process
  }
  
  return 0;
}

