#ifndef PACKET_HPP
#define PACKET_HPP

#include <assert.h>
#include <vector>
#include <string.h>
#include <iostream>

enum class CommunicationInterface 
{
    NONE,
    UART,
    I2C,
};

class Packet {
public:
    Packet(CommunicationInterface interface) : interface(interface) 
    {

    }

    CommunicationInterface interface;

    virtual uint32_t ReadUInt32() = 0;
    virtual int32_t  ReadInt32()  = 0;
    virtual float    ReadFloat()  = 0;
    virtual char*    ReadString() = 0;
};

// all strings, tokenize the string
class SerialPacket : public Packet {
    std::vector<std::string> tokens;
    int index = 0;

public:
    SerialPacket(const char* string) : Packet(CommunicationInterface::UART) {
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

    uint32_t ReadUInt32() override {
        assert(static_cast<size_t>(index) < tokens.size());
        std::string& value = tokens[index++];

        return static_cast<uint32_t>(std::stoi(value));
    }

    int32_t ReadInt32()  override {
        assert(static_cast<size_t>(index) < tokens.size());
        std::string& value = tokens[index++];

        return std::stoi(value);
    }

    float   ReadFloat()  override {
        assert(static_cast<size_t>(index) < tokens.size());
        std::string& value = tokens[index++];

        return std::stof(value);
    }

    char*   ReadString() override{
        assert(static_cast<size_t>(index) < tokens.size());
        std::string& value = tokens[index++];

        return strdup(value.c_str());
    }
};

class BinaryPacket : public Packet {
private:
    const char* data;
    size_t  length;
    char* current; // pointer to the data object
    size_t bytesRead;

    template<typename T> T Read() {
        T value; 
        
        size_t numBytes = sizeof(T);
        memcpy(&value, current, numBytes);
        current += numBytes;

        return value;
    }

public: 
    BinaryPacket(const char* bytes, uint16_t length) 
        : Packet(CommunicationInterface::I2C), data(bytes), length(length) {
        current = const_cast<char*>(bytes);
        bytesRead = 0;
    }

    uint32_t ReadUInt32() override {
        return this->Read<uint32_t>();
    }

    int32_t ReadInt32()  override {
        return this->Read<int32_t>();
    }

    float   ReadFloat()  override {
        return this->Read<float>();
    }

    char*   ReadString() override {
        int length = static_cast<int>(this->Read<uint16_t>());
        char* s = static_cast<char*>(malloc((length + 1) * sizeof(char)));

        for (int i = 0; i < length; i++) {
            s[i] = this->Read<char>();
        }

        s[length] = '\0';

        return s;
    }
};



#endif // PACKET_HPP