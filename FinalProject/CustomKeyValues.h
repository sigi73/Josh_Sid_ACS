#ifndef CUSTOM_KEY_VALUES_H
#define CUSTOM_KEY_VALUES_H

#include <string>
#include <iostream>
#include <fstream>

class CustomKey
{
  public:
    CustomKey(int in_rank, int in_tiebreaker) :
        rank(in_rank), tiebreaker(in_tiebreaker) {}

    bool operator==(const CustomKey& rhs) const
    {
        return (rank == rhs.rank)
        && (tiebreaker == rhs.tiebreaker);
    }
    friend std::ostream& operator<<(std::ostream& os, const CustomKey& key)
    {
        return os << "Rank: " << key.rank  << ", Tiebreak: " << key.tiebreaker;
    }

    int rank;
    int tiebreaker;
};

struct CustomKeyHash
{
    // Based off https://stackoverflow.com/questions/17016175/c-unordered-map-using-a-custom-class-type-as-the-key
    std::size_t operator()(const CustomKey &k) const
    {
        return std::hash<int>()(k.rank) ^ (std::hash<int>()(k.tiebreaker));
    }
};

class CustomKeySerializer
{
  public:
    static Json::Value serialize(CustomKey val)
    {
        Json::Value ret;
        ret["rank"] = val.rank;
        ret["tiebreaker"] = val.tiebreaker;
        return ret;
    }
    static CustomKey deserialize(Json::Value json_val)
    {
        CustomKey ret(json_val["rank"].asInt(), json_val["tiebreaker"].asInt());
        return ret;
    }
  private:
    CustomKeySerializer() {}
};

class CustomValue
{
  public:
    CustomValue(int in_num, std::string in_name, float in_value) :
        num(in_num), name(in_name), value(in_value) {}

    friend std::ostream& operator<<(std::ostream& os, const CustomValue& value)
    {
        return os << "Num: " << value.num << ", Name: " << value.name << ", Value: " << value.value;
    }

    int num;
    std::string name;
    float value;
};

class CustomValueSerializer
{
  public:
    static Json::Value serialize(CustomValue val)
    {
        Json::Value ret;
        ret["num"] = val.num;
        ret["name"] = val.name;
        ret["value"] = val.value;
        return ret;
    }
    static CustomValue deserialize(Json::Value json_val)
    {
        CustomValue ret(json_val["num"].asInt(), json_val["name"].asString(), json_val["value"].asFloat());
        return ret;
    }
  private:
    CustomValueSerializer() {}
};


#endif