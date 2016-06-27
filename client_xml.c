#include "client_xml.h"

void xmlSend(char* buffer, char* msg, char* recipient){
  sprintf(buffer, "<request type=\"SEND\"><from>%s</from><to>%s</to><msg>%s</msg></request>", username, recipient, msg);
}

void xmlSendAll(char* buffer, char* msg){
  sprintf(buffer, "<request type=\"SENDALL\"><from>%s</from><msg>%s</msg></request>", username, msg);
}

void xmlWho(char* buffer){
  sprintf(buffer, "<request type=\"WHO\"><from>%s</from></request>", username);
}

void xmlCreateGroup(char* buffer, char* group){
  sprintf(buffer, "<request type=\"CREATEG\"><from>%s</from><group>%s</group></request>", username, group);
}

void xmlJoinGroup(char* buffer, char* group){
  sprintf(buffer, "<request type=\"JOING\"><from>%s</from><group>%s</group></request>", username, group);
}

void xmlSendGroup(char* buffer, char* group, char* msg){
  sprintf(buffer, "<request type=\"JOING\"><from>%s</from><group>%s</group><msg>%s</msg></request>", username, group, msg);
}

