#ifndef PACKET_HPP
#define PACKET_HPP

#include <assert.h>
#include <vector>
#include <string.h>
#include <iostream>

class Packet {
public:
    Packet() {}

    virtual int32_t ReadInt32() = 0;
    virtual float   ReadFloat() = 0;
    virtual char*   ReadString() = 0;
};

// all strings, tokenize the string
class SerialPacket : public Packet {
    std::vector<std::string> tokens;
    int index = 0;

public:
    SerialPacket(const char* string) {
        const char* delimiters = " ";
        char* duplicate = strdup(string);

        // Tokenize split on space
        char* token = strtok(duplicate, delimiters);

        while (token != NULL) {
            tokens.push_back(token);

            token = strtok(NULL, delimiters);
        }

        free(duplicate);
    }

    int32_t ReadInt32()  override {
        assert(index < tokens.size());
        std::string& value = tokens[index++];

        return std::stoi(value);
    }

    float   ReadFloat()  override {
        assert(index < tokens.size());
        std::string& value = tokens[index++];

        return std::stof(value);
    }

    char*   ReadString() override{
        assert(index < tokens.size());
        std::string& value = tokens[index++];

        return strdup(value.c_str());
    }
};

#endif // PACKET_HPP