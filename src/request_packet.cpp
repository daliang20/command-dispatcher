#include "request_packet.hpp"
#include <string.h>

#define NO_ERROR 0
#define INVALID 1

SerialRequestPacket::SerialRequestPacket(const char *packet) {
  char *copy = strdup(packet);
  const char *space = " ";

  // need to normalize string here
  char *token = strtok(copy, space);

  while (token) {
    tokens.push_back(token);
    token = strtok(NULL, space);
  }
}

bool SerialRequestPacket::hasNext() { return index < tokens.size(); }

std::string &SerialRequestPacket::getNext() { return tokens[index++]; }

Result<const char *> SerialRequestPacket::ReadString() {
  Result<const char *> result;

  if (hasNext()) {
    result.resultCode = NO_ERROR;
    result.value = getNext().c_str();
  } else {
    result.resultCode = INVALID;
  }

  return result;
}

Result<uint32_t> SerialRequestPacket::ReadUInt32() {
  Result<uint32_t> result;

  if (hasNext()) {
    auto token = getNext();
    char *end;

    result.value = std::strtoul(token.c_str(), &end, 0);

    if (errno == ERANGE) {
      result.resultCode = 1;
    }

    if (token.c_str() == end || *end) {
      result.resultCode = 1;
    }
  } else {
    result.resultCode = INVALID;
  }
  return result;
}

Result<float> SerialRequestPacket::ReadFloat() {
  Result<float> result;

  if (hasNext()) {
    auto token = getNext();
    char *end;

    result.value = std::strtof(token.c_str(), &end);

    if (errno == ERANGE) {
      result.resultCode = 1;
    }

    if (token.c_str() == end || *end) {
      result.resultCode = 1;
    }
  } else {
    result.resultCode = INVALID;
  }
  return result;
}

Result<int32_t> SerialRequestPacket::ReadInt32() {
  Result<int32_t> result;

  if (hasNext()) {
    auto token = getNext();
    char *end;

    result.value = std::strtol(token.c_str(), &end, 0);

    if (errno == ERANGE) {
      result.resultCode = 1;
    }

    if (token.c_str() == end || *end) {
      result.resultCode = 1;
    }
  } else {
    result.resultCode = INVALID;
  }

  return result;
}

template <> Result<uint32_t> Read(RequestPacket *packet) {
  return packet->ReadUInt32();
}
template <> Result<int32_t> Read(RequestPacket *packet) {
  return packet->ReadInt32();
}
template <> Result<float> Read(RequestPacket *packet) {
  return packet->ReadFloat();
}
template <> Result<const char *> Read(RequestPacket *packet) {
  return packet->ReadString();
}