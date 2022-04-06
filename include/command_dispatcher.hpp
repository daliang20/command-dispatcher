#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include <functional>
#include <map>
#include <string>
#include "packet.hpp"
#include <utility>
#include "invoke.hpp"

template<typename T> T Read(Packet&) = delete;

template<> uint32_t Read<uint32_t>(Packet& packet) {
    return packet.ReadUInt32();
}

template<> int32_t Read<int32_t>(Packet& packet) {
    return packet.ReadInt32();
}

template<> float Read<float>(Packet& packet) {
    return packet.ReadFloat();
}

template<> std::string Read<std::string>(Packet& packet) {
    char* string = packet.ReadString();
    std::string s(string);
    free(string);

    return s;
}

class CommandDispatcher {
private:
    std::map<std::string, std::function<void(Packet&)>> commandMap;
    std::map<uint32_t, std::function<void(Packet&)>> binaryCommandMap;

public:
    void AddHandler(std::string command, void (*callback)(Packet& packet)) {
        commandMap.emplace(command, [callback](Packet& packet) {
            callback(packet);
        });
    }

    template <typename... Arguments> 
    void AddHandler(std::string command, void (*callback)(Arguments...)) {
        commandMap.emplace(command, [callback](Packet& packet) {
            auto arguments = std::make_tuple(Read<Arguments>(packet)...);

            // this is a cpp17 concept, use a backport
            nonstd::apply(callback, std::move(arguments));
        });
    }

    template <typename... Arguments> 
    void AddHandler(uint32_t command, void (*callback)(Arguments...)) {
        binaryCommandMap.emplace(command, [callback](Packet& packet) {
            auto arguments = std::make_tuple(Read<Arguments>(packet)...);

            // this is a cpp17 concept, use a backport
            nonstd::apply(callback, std::move(arguments));
        });
    }

    void DispatchPacket(SerialPacket& packet) {
        std::cout << "Dispatch Serial Packet" << std::endl;
        
        char *command = packet.ReadString();
        auto itr = commandMap.find(command); 
        if (itr != commandMap.end()) {
            itr->second(packet);
        }

        // cleanup 
        free(command);
    }

    void DispatchPacket(BinaryPacket& packet) {
        std::cout << "Dispatch Binary Packet" << std::endl;
        uint32_t command = packet.ReadUInt32();

        auto itr = binaryCommandMap.find(command); 
        if (itr != binaryCommandMap.end()) {
            itr->second(packet);
        }
    }
};

#endif // COMMAND_DISPATCHER_HPP