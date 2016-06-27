#include <stdio.h>
#include "interface.h"

void xmlSend(char* buffer, char* msg, char* recipient);
void xmlSendAll(char* buffer, char* msg);
void xmlWho(char* buffer);
void xmlCreateGroup(char* buffer, char* group);
void xmlJoinGroup(char* buffer, char* group);
void xmlSendGroup(char* buffer, char* group, char* msg);
