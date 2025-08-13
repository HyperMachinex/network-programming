// S O C K E T
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

// D E F A U L T
#include <stdio.h>
#include <string.h>
#include <time.h>

// M A C R O S
#define ISVALIDSOCKET(s) ((s) >=0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int // socket() returns negative on failure
#define GETSOCKETERRNO() (errno)

#define LINE "==============================\n"

int main(int argc, char *argv[]){
  
  printf("Configuring local address...\n%s", LINE);
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  struct addrinfo *bind_address;
  getaddrinfo(0, "8080", &hints, &bind_address);

  printf("Creating socket...\n%s", LINE); 
  SOCKET socket_listen;
  socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);
  if(!ISVALIDSOCKET(socket_listen)){
    fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }
  
  printf("Binding socket to local address...\n%s", LINE);
  if(bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)){
    fprintf(stderr, "bind() failed (%d)\n", GETSOCKETERRNO());
    return 1;
  }

  freeaddrinfo(bind_address);

  printf("Listening...\n%s", LINE);
  if(listen(socket_listen, 10) < 0) {
    fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }

  printf("Waiting for connection...\n%s", LINE);
  struct sockaddr_storage client_address;
  socklen_t client_len = sizeof(client_address);
  SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
  if(!(socket_client >= 0)){ // or if(!ISVALIDSOCKET(socket_client)) 
    fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
    return 1;
  }

  printf("Client is connected:");
  char address_buffer[100];
  getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
  printf("%s\n%s", address_buffer, LINE);

  printf("Reading request..\n%s", LINE);
  char request[1024];
  int bytes_received = recv(socket_client, request, 1024, 0);
  printf("Received %d bytes.\n%s", bytes_received, LINE);
  //printf("%.*s", bytes_received, request);

  printf("Sending response...\n%s", LINE);
  const char* response = 
                        "HTTP/1.1 200 OK\r\n"
                        "Connection: close\r\n"
                        "Content-Type: text/plain\r\n\r\n"
                        "Local time is: ";

  int bytes_sent = send(socket_client, response, strlen(response), 0);
  printf("Sent 200OK %d of %d bytes.\n", bytes_sent, (int)strlen(response));

  time_t timer;
  time(&timer);
  char *time_msg = ctime(&timer);
  bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);
  printf("Sent time_msg %d of %d bytes.\n%s", bytes_sent, (int)strlen(time_msg), LINE);

  printf("Closing connection...\n%s", LINE);
  CLOSESOCKET(socket_client); // or close(socket_client)

  printf("Closing listening socket...\n%s", LINE);
  CLOSESOCKET(socket_listen); // or close(socket_listen)

  printf("F I N I S H E D\n");

  return 0;
}
