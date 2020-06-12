#ifndef HASHER_HPP
#define HASHER_HPP
#include <cstdint>
#include <string_view>
#include <tuple>

#include "extern/SpookyV2.h"

class Hasher : protected SpookyHash
{
  public:
    using Hash128_t = std::pair<uint64_t, uint64_t>;
    using Hash64_t = uint64_t;
    using Hash32_t = uint32_t;

    Hasher(Hash128_t seed = {0, 0}) : SpookyHash()
    {
        SpookyHash::Init(seed.first, seed.second);
    }

    void Update(std::string_view sv)
    {
        SpookyHash::Update(sv.data(), sv.length());
    }

    Hash128_t Final()
    {
        Hash128_t ret;
        SpookyHash::Final(&ret.first, &ret.second);
        return ret;
    }

    static Hash128_t Hash128(std::string_view sv, Hash128_t seed = {0, 0})
    {
        SpookyHash::Hash128(sv.data(), sv.length(), &seed.first, &seed.second);
        return seed;
    }

    static Hash64_t Hash64(std::string_view sv, uint64_t seed = 0)
    {
        return SpookyHash::Hash64(sv.data(), sv.length(), seed);
    }

    static Hash32_t Hash32(std::string_view sv, uint32_t seed = 0)
    {
        return SpookyHash::Hash32(sv.data(), sv.length(), seed);
    }

    using SpookyHash::Final;
    using SpookyHash::Hash128;
    using SpookyHash::Hash32;
    using SpookyHash::Hash64;
    using SpookyHash::Update;
};

inline Hasher &operator<<(Hasher &h, std::string_view sv)
{
    h.Update(sv);
    return h;
}

template <> struct std::hash<Hasher::Hash128_t>
{
    std::size_t operator()(const Hasher::Hash128_t &val) const
    {
        return (val.first);
    }
};

#endif // HASHER_HPP