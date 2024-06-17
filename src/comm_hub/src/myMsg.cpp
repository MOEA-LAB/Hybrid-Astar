#include "myMsg.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <vector>

#define MSG_HEAD "@ZDX"

std::string send_msg_package(std::string json)
{
    auto msgType = intToBytes(0, 2);
    auto jsonLen = intToBytes(json.length(), 4);

    std::string str;
    str += MSG_HEAD;
    str += std::string(msgType.begin(), msgType.end());
    str += std::string(jsonLen.begin(), jsonLen.end());
    str += json;

    return str;
}

int recv_msg_parse(char *data, int len, std::string &res)
{
    std::string msg;
    for (size_t i = 0; i < len; i++)
    {
        msg += data[i];
    }
    // std::cout << msg << std::endl;
    // std::cout << msg.size() << std::endl;
    // std::cout << sizeof(data) << std::endl;

    if (msg.substr(0, 4) != MSG_HEAD)
    {
        std::cout << "MSG_HEAD error!" << std::endl;
        return -1;
    }

    if (0 != bytesToInt(msg.substr(4, 2)))
    {
        std::cout << "MSG_TYPE is not 0!" << std::endl;
        return -1;
    }

    auto jsonLen = bytesToInt(msg.substr(6, 4));

    res = msg.substr(10, jsonLen);

    return 0;
}

int recv_msg_parse_1(char *data, int len, std::vector<std::string> &res)
{
    std::string msg;
    for (size_t i = 0; i < len; i++)
    {
        msg += data[i];
    }

    int ptr = 0;
    while (ptr < len)
    {
        if (msg.substr(0, 4) != MSG_HEAD)
        {
            std::cout << "MSG_HEAD error!" << std::endl;
            return ptr;
        }

        if (0 != bytesToInt(msg.substr(4, 2)))
        {
            std::cout << "MSG_TYPE is not 0!" << std::endl;
            return ptr;
        }

        auto jsonLen = bytesToInt(msg.substr(6, 4));

        res.push_back(msg.substr(10, jsonLen));

        ptr += 10 + jsonLen;
        msg = msg.substr(10 + jsonLen, len - 10 - jsonLen);
    }

    return ptr;
}

// common
std::vector<unsigned char> intToBytes(const int paramInt, const int len)
{
    std::vector<unsigned char> arrayOfByte(len);
    for (int i = 0; i < len; i++)
        arrayOfByte[i] = (paramInt >> (i * 8));
    return arrayOfByte;
}

int bytesToInt(const std::string &buffer)
{
    if (4 == buffer.size())
    {
        return int((unsigned char)(buffer[3]) << 24 |
                   (unsigned char)(buffer[2]) << 16 |
                   (unsigned char)(buffer[1]) << 8 |
                   (unsigned char)(buffer[0]));
    }
    else if (2 == buffer.size())
    {
        return int((unsigned char)(buffer[1]) << 8 |
                   (unsigned char)(buffer[0]));
    }

    return -1;
}

std::string StringToHex(const std::string &data)
{
    const std::string hex = "0123456789ABCDEF";
    std::stringstream ss;

    for (std::string::size_type i = 0; i < data.size(); ++i)
        ss << hex[(unsigned char)data[i] >> 4] << hex[(unsigned char)data[i] & 0xf];
    // std::cout << ss.str() << std::endl;
    return ss.str();
}

std::string HexToStr(const std::string &str)
{
    std::string result;
    for (size_t i = 0; i < str.length(); i += 2)
    {
        std::string byte = str.substr(i, 2);
        char chr = (char)(int)strtol(byte.c_str(), NULL, 16);
        result.push_back(chr);
    }
    return result;
}

std::string getCurrentTime()
{
    auto timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    auto timeStamp = std::to_string(timeNow.count());

    return timeStamp;
}