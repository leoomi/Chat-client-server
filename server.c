#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define BUFSIZE 1024

//parses xml message
void parseXml(char* recv_buf, char* buffer){
  xmlDoc *doc = NULL;
  xmlNode *root = NULL;
  xmlNode *cur_node = NULL;
  char *sender, *recipient, *msg;

  doc = xmlParseDoc(recv_buf);
  if (doc == NULL) {
    printf("error: could not parse file\n");
  }

  root = xmlDocGetRootElement(doc);
  if(strcmp(xmlGetProp(root, "type"), "SEND") == 0){
    for(cur_node = root->children; cur_node != NULL; cur_node = cur_node->next){
      if(strcmp(cur_node->name, "from") == 0){
	sender = xmlNodeGetContent(cur_node);
      }
      else if(strcmp(cur_node->name, "to") == 0){
	recipient = xmlNodeGetContent(cur_node);
      }
      else if(strcmp(cur_node->name, "msg") == 0){
	msg = xmlNodeGetContent(cur_node);
      }
    }
  }

  sprintf(buffer, "[%s->%s]: %s", sender, recipient, msg);
  
  xmlFreeDoc(doc);
  xmlCleanupParser();
}

void send_to_all(int j, int sockfd, int nbytes_recvd, char *send_buf, fd_set *master)
{
  if (FD_ISSET(j, master)){
    if (j != sockfd) {
      if (send(j, send_buf, nbytes_recvd, 0) < 0) {
	perror("send");
      }
    }
  }
}

void send_recv(int i, fd_set *master, int sockfd, int fdmax)
{
  int nbytes_recvd, j;
  char recv_buf[BUFSIZE];
  char send_buf[BUFSIZE];
	
  if ((nbytes_recvd = recv(i, recv_buf, BUFSIZE, 0)) <= 0) {
    if (nbytes_recvd == 0) {
      printf("socket %d disconnected\n", i);
    }
    else {
      perror("recv");
    }
    close(i);
    FD_CLR(i, master);
  }
  else {
    recv_buf[nbytes_recvd] = '\0';
    //printf("%s\n", recv_buf);
    parseXml(recv_buf, send_buf);
    for(j = 0; j <= fdmax; j++){
      send_to_all(j, sockfd, nbytes_recvd, send_buf, master);
    }
  }	
}

//Handles new connections
void connection_accept(fd_set *master, int *fdmax, int sockfd, struct sockaddr_in *client_addr)
{
  socklen_t addrlen;
  int newsockfd;
	
  addrlen = sizeof(struct sockaddr_in);
  if((newsockfd = accept(sockfd, (struct sockaddr *)client_addr, &addrlen)) < 0) {
    perror("accept");
    exit(1);
  }
  else {
    FD_SET(newsockfd, master);
    if(newsockfd > *fdmax){
      *fdmax = newsockfd;
    }
    printf("new connection from %s on port %d \n",inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
  }
}

//Handles socket initialization
void init_connection(int *sockfd, struct sockaddr_in *my_addr, int port)
{
  int yes = 1;
  
  if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0))  < 0) {
    perror("socket");
    exit(1);
  }
		
  my_addr->sin_family = AF_INET;
  my_addr->sin_port = htons(port);
  my_addr->sin_addr.s_addr = INADDR_ANY;
  memset(my_addr->sin_zero, '\0', sizeof my_addr->sin_zero);
		
  if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
    perror("setsockopt");
    exit(1);
  }
		
  if (bind(*sockfd, (struct sockaddr *)my_addr, sizeof(struct sockaddr)) < 0) {
    perror("bind");
    exit(1);
  }
  if (listen(*sockfd, 10) < 0) {
    perror("listen");
    exit(1);
  }
  printf("\nTCPServer Waiting for client on port %d\n", port);
  fflush(stdout);
}

int main(int argc, char* argv[]) {
  fd_set master;
  fd_set read_fds;
  int fdmax, i, port;
  int sockfd = 0;
  struct sockaddr_in my_addr, client_addr;

  if (argc == 2) {
    port = atoi(argv[1]);
  }
  else {
    fprintf(stderr, "usage: ./server port\n");
    exit(1);
  }

  //Select initialization
  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  init_connection(&sockfd, &my_addr, port);
  FD_SET(sockfd, &master);
	
  fdmax = sockfd;
  while(1){
    read_fds = master;
    if(select(fdmax+1, &read_fds, NULL, NULL, NULL) < 0){
      perror("select");
      exit(4);
    }
		
    for (i = 0; i <= fdmax; i++){
      if (FD_ISSET(i, &read_fds)){
	if (i == sockfd)
	  connection_accept(&master, &fdmax, sockfd, &client_addr);
	else
	  send_recv(i, &master, sockfd, fdmax);
      }
    }
  }
  return 0;
}
		
