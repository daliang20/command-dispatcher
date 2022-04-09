#ifndef COMMAND_DISPATCHER_HPP
#define COMMAND_DISPATCHER_HPP

#include "parameters.hpp"
#include <functional>
#include <map>

class CommandDispatcher {
private:
  std::map<std::string, std::function<int(Parameters &)>> commandMap;

public:
  CommandDispatcher &addCommand(std::string opcode,
                                int (*function)(Parameters &)) {
    commandMap.emplace(opcode, function);

    return *this;
  }

  int DispatchCommand(const char *command) {
    SerialRequestPacket request(command);
    SerialResponsePacket response;

    Parameters params;
    params.request = &request;
    params.response = &response;

    // Does not matter if this fails
    const char *opcode = request.ReadString().value;

    auto itr = commandMap.find(opcode);

    if (itr != commandMap.end()) {
      int rc = itr->second(params);

      if (rc != 0) {
        printf("[ERR] %s\n", params.response->GetResponse());
      }

      return rc;
    } else {
      return 1;
    }
  }
};

#endif // COMMAND_DISPATCHER_HPP