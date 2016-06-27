#define NAME_SIZE 15

typedef struct user{
  int sockfd;
  char name[NAME_SIZE];
} user;

int findUser(user* users, int nusers, char *name);
int findSockfd(user* users, int nusers, int sockfd);
