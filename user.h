#define NAME_SIZE 15
#define GROUP_SIZE 10

typedef struct user{
  int sockfd;
  char name[NAME_SIZE];
} user;

typedef struct group{
  char name[NAME_SIZE];
  user group_users[GROUP_SIZE];
} group;

int findUser(user* users, int nusers, char *name);
int findSockfd(user* users, int nusers, int sockfd);
