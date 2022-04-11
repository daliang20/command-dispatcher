#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "request_packet.hpp"
#include "response_packet.hpp"
#include <functional>

struct Parameters {
  RequestPacket *request;
  ResponsePacket *response;
};

template <typename T>
bool AssertParameter(T *variable, Parameters &params, const char *error) {
  Result<T> result = Read<T>(params.request);
  if (result.resultCode != 0) {
    params.response->SetResponse(error);

    return true;
  }

  *variable = result.value;
  return false;
}

bool AssertParametersEmpty(Parameters &params) {
  bool hasNext = params.request->hasNext();

  if (hasNext) {
    params.response->SetResponse("Too Many Parameters");
  }

  return params.request->hasNext();
}

#endif // PARAMETERS_H