#ifndef RESPONSE_PACKET
#define RESPONSE_PACKET

#include "result.hpp"
#include <string>
#include <vector>

class ResponsePacket {
public:
  virtual void SetResponse(std::string message) = 0;
  virtual const char *GetResponse() = 0;
};

class SerialResponsePacket : public ResponsePacket {
private:
  std::string buffer;

public:
  SerialResponsePacket() {}

  void SetResponse(std::string message) { buffer = message; }
  const char *GetResponse() { return buffer.c_str(); }
};

#endif // RESPONSE_PACKET
