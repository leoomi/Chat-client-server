#include "client_xml.h"

void xmlSend(char* buffer, char* msg, char* recipient){
  sprintf(buffer, "<request type=\"SEND\"><from>%s</from><to>%s</to><msg>%s</msg></request>", username, recipient, msg);
}

void xmlSendAll(char* buffer, char* msg, char* recipient){
  sprintf(buffer, "<request type=\"SEND\"><from>%s</from><to>%s</to><msg>%s</msg></request>", username, recipient, msg);
}
