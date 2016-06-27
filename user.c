#include "user.h"
#include <string.h>
#include <stdlib.h>
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

int findGroup(group* groups, int ngroups, char *name){
  int i;
  
  for(i = 0; i < ngroups; i++){
    if(strcmp(groups[i].name, name) == 0)
      return i;
  }
  return -1;
}

int isInGroup(int userPos, group grp){
  int i;

  for(i = 0; i < grp.nusers; i++){
    if(userPos == grp.users[i])
      return 1;
  }

  return 0;
}

char* who(user* users, int nusers){
  int i;
  char *output = malloc(1024*sizeof(char));
  char on[] = "online";
  char off[] = "offline";
  output[0] = '\0';
  strcat(output, "|            user |  status |\n");
  
  for(i = 0; i < nusers; i++){
    char line[50];
    if(users[i].sockfd < 0){
      sprintf(line, "| %15s | %7s |\n", users[i].name, off);
    }
    else{
      sprintf(line, "| %15s | %7s |\n", users[i].name, on);
    }
    strcat(output, line);
  }
  return output;
}
