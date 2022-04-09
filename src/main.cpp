#include "command_dispatcher.hpp"
#include "parameters.hpp"
#include "request_packet.hpp"

#include <stdio.h>

#define ASSERT_PARAMETER(name)

int flash(Parameters &params) {
  uint32_t address;
  uint32_t size;

  // The order of this function matters!
  if (AssertParameter(&address, params, "Address Error") ||
      AssertParameter(&size, params, "Size Error") ||
      AssertParametersEmpty(params)) {
    printf("An Error Occured %s\n", __PRETTY_FUNCTION__);
    return 1;
  }

  printf("%s => (%d)\n", __PRETTY_FUNCTION__, address);

  return 0;
}

int main() {
  CommandDispatcher dispatcher;

  dispatcher.addCommand("flash", &flash);

  dispatcher.DispatchCommand("flash 1000 1001");

  return 0;
}