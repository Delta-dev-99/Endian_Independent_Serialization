#pragma once

#include <dd99/EIS1/eis_implementation.hpp>
#include <optional>



namespace dd99::EIS1
{

    template <Serializable T>
    struct Serializable_Traits<std::optional<T>>
    {
        using value_type = std::optional<T>;

        template <class OutputIt>
        static constexpr std::size_t serialize(OutputIt out, const value_type & v)
        {
            auto n = EIS1::serialize(out, v.has_value());
            if (v.has_value())
                n += EIS1::serialize(out, *v);
            return n;
        }

        template <class InputIt>
        static constexpr std::pair<std::size_t, value_type> deserialize(InputIt in)
        {
            auto [n, b] = EIS1::deserialize<decltype(std::declval<value_type>().has_value())>(in);
            if (!b) return {n, {}};
            else
            {
                in += n;
                auto [n2, x] = EIS1::deserialize<T>(in);
                return {n + n2, std::move(x)};
            }
        }
    };
}
