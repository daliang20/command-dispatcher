#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include <functional>
#include <map>
#include <string>
#include "packet.hpp"

class CommandDispatcher {
    std::map<std::string, std::function<void(Packet&)>> commandMap;
public:
    void AddHandler(std::string command, void (*function)(int, float)) {
        commandMap.emplace(command, [function](Packet& packet) {
            auto param1 = packet.ReadInt32();
            auto param2 = packet.ReadFloat();

            function(param1, param2);
        });
    }

    void DispatchPacket(SerialPacket& packet) {
        std::cout << "Dispatch Serial Packet" << std::endl;
        
        std::string command = packet.ReadString();

        auto itr = commandMap.find(command); 

        if (itr != commandMap.end()) {
            itr->second(packet);
        }
    }
};

#endif // COMMAND_DISPATCHER_HPP