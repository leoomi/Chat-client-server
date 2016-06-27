#include "user.h"
#include <string.h>
#include <stdio.h>

int findUser(user* users, int nusers, char *name){
  int i;
  
  for(i = 0; i < nusers; i++){
    if(strcmp(users[i].name, name) == 0)
      return i;
  }
  return -1;
}

int findSockfd(user* users, int nusers, int sockfd){
  int i;
  
  for(i = 0; i < nusers; i++){
    if(users[i].sockfd == sockfd)
      return i;
  }
  return -1;
}
