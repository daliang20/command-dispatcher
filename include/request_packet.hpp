#ifndef REQUEST_PACKET
#define REQUEST_PACKET

#include "result.hpp"
#include <string>
#include <vector>

class RequestPacket {
public:
  virtual Result<const char *> ReadString() = 0;
  virtual Result<uint32_t> ReadUInt32() = 0;
  virtual Result<float> ReadFloat() = 0;
  virtual Result<int32_t> ReadInt32() = 0;
  virtual bool hasNext() = 0;
};

template <typename T> Result<T> Read(RequestPacket *packet);

class SerialRequestPacket : public RequestPacket {
private:
  std::vector<std::string> tokens;
  size_t index = 0;

  std::string &getNext();

public:
  SerialRequestPacket(const char *packet);

  bool hasNext();
  Result<const char *> ReadString();
  Result<uint32_t> ReadUInt32();
  Result<float> ReadFloat();
  Result<int32_t> ReadInt32();
};

#endif // REQUEST_PACKET
