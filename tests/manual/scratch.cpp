#include <dd99/EIS1/EIS1.hpp>
#include <vector>
#include <optional>
#include <iterator>
#include <string>
#include <map>


struct something
{
    int x;
    char y;
    std::uint64_t z;
    std::vector<int> v;
    std::string s;
    std::map<int, int> m;
    std::optional<int> o;
};

template <>
struct dd99::EIS1::Serializable_Traits<something>
{
    using value_type = something;
    using tuple_type = std::tuple<int, char, std::uint64_t, std::vector<int>, std::string, std::map<int, int>, std::optional<int>>;

    static auto to_tuple(const value_type & v)
    { return std::tie(v.x, v.y, v.z, v.v, v.s, v.m, v.o); }

    static auto from_tuple(tuple_type && v)
    { return std::make_from_tuple<something>(std::move(v)); }
};

int main()
{
    int x = 5;

    std::vector<char> buffer;
    buffer.reserve(1024);

    auto r = dd99::EIS1::serialize(std::back_inserter(buffer), something{1, 2, 3, {4, 5, 6}, "Armando", {{1,2},{3,4},{5,6}}, std::nullopt});

    auto k = dd99::EIS1::deserialize<something>(buffer.data());
}
