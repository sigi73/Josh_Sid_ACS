#ifndef DEFAULT_SERIALIZER_H
#define DEFAULT_SERIALIZER_H

#include "json/json.h"
#include <string>

template <class T>
class DefaultSerializer
{
  public:
    static Json::Value serialize(T val);
    static T deserialize(Json::Value json_val);

  private:
    DefaultSerializer() {}
};

// TODO: Default for int, String, float

template <>
Json::Value DefaultSerializer<int>::serialize(int val)
{
    Json::Value ret(val);
    return ret;
}
template<>
int DefaultSerializer<int>::deserialize(Json::Value json_val)
{
    return json_val.asInt();
}

template <>
Json::Value DefaultSerializer<float>::serialize(float val)
{
    Json::Value ret(val);
    return ret;
}
template<>
float DefaultSerializer<float>::deserialize(Json::Value json_val)
{
    return json_val.asFloat();
}

template <>
Json::Value DefaultSerializer<std::string>::serialize(std::string val)
{
    Json::Value ret(val);
    return ret;
}
template<>
std::string DefaultSerializer<std::string>::deserialize(Json::Value json_val)
{
    return json_val.asString();
}

#endif