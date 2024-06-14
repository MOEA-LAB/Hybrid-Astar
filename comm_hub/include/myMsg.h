#ifndef MYMSG_H
#define MYMSG_H

#include <string>
#include <vector>

std::string send_msg_package(std::string json);

int recv_msg_parse(char *msg, int len, std::string &res);
int recv_msg_parse_1(char *data, int len, std::vector<std::string> &res);

// common
std::vector<unsigned char> intToBytes(const int paramInt, const int len);
int bytesToInt(const std::string &buffer);
std::string StringToHex(const std::string &data);
std::string HexToStr(const std::string &str);
std::string getCurrentTime();
#endif