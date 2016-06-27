#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include "interface.h"
#include "client_xml.h"
#define PARSE_ERROR 2
#define PARSE_QUIT 1
#define PARSE_OK 0
#define MSG_SIZE 300

int parseString(char* msg_buf, char* send_buf){
  char* first;
  char msg[MSG_SIZE];
  strcpy(msg, msg_buf);

  if(strlen(msg_buf) != 0){
    first = strtok(msg_buf, " ");
    if(strcmp(first, "SEND") == 0){
      char *recipient;
      recipient = strtok(NULL, " ");
    
      xmlSend(send_buf, msg+6+strlen(recipient), recipient);
      return PARSE_OK;
    }
    else if(strcmp(first, "SENDALL") == 0){
      xmlSendAll(send_buf, msg+8);
      return PARSE_OK;
    }
    else if(strcmp(first, "WHO") == 0){
      xmlWho(send_buf);
      return PARSE_OK;
    }
    else if(strcmp(first, "CREATEG") == 0){
      char *group;
      group = strtok(NULL, " ");
    
      xmlCreateGroup(send_buf, group);
      return PARSE_OK;
    }
    else if(strcmp(first, "JOING") == 0){
      char *group;
      group = strtok(NULL, " ");
      xmlJoinGroup(send_buf, group);
      return PARSE_OK;
    }
    else if(strcmp(first, "SENDG") == 0){
      char *group;
      group = strtok(NULL, " ");
    
      xmlSendGroup(send_buf, group, msg+7+strlen(group));
      return PARSE_OK;
    }
    else if(strcmp(first, "EXIT") == 0){
      return PARSE_QUIT;
    }
    else{
      return PARSE_ERROR;
    }
  }
  else
    return PARSE_ERROR;
}

void send_recv(int i, int sockfd)
{
  char msg_buf[MSG_SIZE];
  char send_buf[BUFSIZE];
  char recv_buf[BUFSIZE];
  int nbyte_recvd, code;
  
  if (i == 0){
    in_scan(msg_buf);
    code = parseString(msg_buf, send_buf);

    if (code == PARSE_QUIT) {
      exit(0);
    }
    else if(code == PARSE_OK){
      send(sockfd, send_buf, strlen(send_buf), 0);
    }
    else{
      out_printf("%s\n", "Parse error");
    }
  }
  else {
    nbyte_recvd = recv(sockfd, recv_buf, BUFSIZE, 0);
    recv_buf[nbyte_recvd] = '\0';
    
    out_printf("%s\n" , recv_buf);
    fflush(stdout);
  }
}
		
int main(int argc, char* argv[])
{
  int sockfd, fdmax, i, port;
  char *host;
  struct hostent *hp;
  struct sockaddr_in server_addr;
  fd_set master;
  fd_set read_fds;
  
  if (argc==4) {
    host = argv[1];
    port = atoi(argv[2]);
    if(strlen(argv[3]) > NAME_SIZE){
      fprintf(stderr, "User name too long");
      exit(1);
    }
    else{
      strcpy(username, argv[3]);
      printf("%s\n", username);
    }
  }
  else {
    fprintf(stderr, "usage: ./client <host> <port> <username>\n");
    exit(1);
  }

  /* translate host name into peerâ€™s IP address */
  hp = gethostbyname(host);

  if (!hp) {
    fprintf(stderr, "unknown host: %s\n", host);
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket");
    exit(1);
  }
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);
	
  if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
    perror("connect");
    exit(1);
  }

  printf("%s\n", username);
  if(send(sockfd, username,sizeof(username), 0) < 0){
    perror("name send");
    exit(1);
  }
  
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  FD_SET(0, &master);
  FD_SET(sockfd, &master);
  fdmax = sockfd;

  init_interface();
  
  while(1){
    read_fds = master;
    if(select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1){
      perror("select");
      exit(4);
    }
		
    for(i=0; i <= fdmax; i++ )
      if(FD_ISSET(i, &read_fds))
	send_recv(i, sockfd);
  }
  out_printf("client-quited\n");
  close(sockfd);
  return 0;
}
