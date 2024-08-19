#pragma once

#include <dd99/EIS1/serializable_traits.hpp>
#include <algorithm>
#include <concepts>
#include <bit>



namespace dd99::EIS1
{
    // add some exceptions that can also be handled by the Serializable_Traits specialization for integrals
    template <class T>
    concept extended_integral = std::integral<T> || std::same_as<T, std::byte>;


    template <extended_integral T>
    struct Serializable_Traits<T>
    {
        using value_type = T;

        template <class OutputIt>
        static constexpr std::size_t serialize(OutputIt out, const value_type v)
        {
            if constexpr (std::endian::native == std::endian::little)
                std::copy(reinterpret_cast<const char *>(&v), reinterpret_cast<const char *>(&v) + sizeof(v), out);
            else
                std::reverse_copy(reinterpret_cast<const char *>(&v), reinterpret_cast<const char *>(&v) + sizeof(v), out);
            
            return sizeof(v);
        }

        template <class InputIt>
        static constexpr auto deserialize(InputIt in)
        {
            value_type r;
            if constexpr (std::endian::native == std::endian::little)
                std::copy_n(in, sizeof(r), reinterpret_cast<char *>(&r));
            else
                std::copy_n(in, sizeof(r), std::make_reverse_iterator(reinterpret_cast<char *>(&r) + sizeof(r)));
                // std::reverse_copy(reinterpret_cast<const char *>(&v), reinterpret_cast<const char *>(&v) + sizeof(v), out);
            
            return std::make_pair(sizeof(r), std::move(r));
        }
    };
}
