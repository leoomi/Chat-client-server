#define NAME_SIZE 15
#define GROUP_SIZE 10

typedef struct user{
  int sockfd;
  char name[NAME_SIZE];
} user;

typedef struct group{
  char name[NAME_SIZE];
  int users[GROUP_SIZE];
  int nusers;
} group;

int findUser(user* users, int nusers, char *name);
int findSockfd(user* users, int nusers, int sockfd);
int findGroup(group* groups, int ngroups, char *name);
int isInGroup(int userPos, group grp); 
char* who(user* users, int nusers);
