#pragma once

#include <dd99/EIS1/eis_implementation.hpp>
#include <stdexcept>
#include <limits>



namespace dd99::EIS1
{

    template <class T>
    concept serializable_container =
        requires (T & t, std::size_t n)
        {
            typename T::value_type;
            { t.size() };
            { t.begin() };
            { t.end() };
        }
        && Serializable<std::remove_cvref_t<typename T::value_type>>;


    template <serializable_container T>
    struct Serializable_Traits<T>
    {
        using value_type = T;
        using size_type = std::uint32_t;

        static constexpr auto is_mapping = requires { typename value_type::mapped_type; };

        using element_type = decltype([](){
            if constexpr (is_mapping)
                return std::pair<typename value_type::key_type, typename value_type::mapped_type>{};
            else
                return std::remove_cvref_t<typename value_type::value_type>{};
        }());

        template <class OutputIt>
        static constexpr std::size_t serialize(OutputIt out, const value_type & v)
        {
            if (v.size() > std::numeric_limits<size_type>::max())
                throw std::runtime_error{"Container serialization error: Too many elements in container"};

            auto n = EIS1::serialize(out, static_cast<size_type>(v.size()));
            for (const auto & x : v)
                n += EIS1::serialize(out, x);
            return n;
        }

        template <class InputIt>
        static constexpr auto deserialize(InputIt in)
        {
            value_type r;
            auto [n, size] = EIS1::deserialize<size_type>(in);
            in += n;
            if constexpr ( requires { r.reserve(size); } )
                r.reserve(size);
            for (size_type i = 0; i < size; ++i)
            {
                auto [n2, elem] = EIS1::deserialize<element_type>(in);
                in += n2; n += n2;
                if constexpr ( requires { r.emplace_back(std::move(elem)); } )
                    r.emplace_back(std::move(elem));
                else if constexpr ( requires { r.emplace_hint(r.end(), std::move(elem)); } )
                    r.emplace_hint(r.end(), std::move(elem));
                else if constexpr ( requires { r.emplace(std::move(elem)); } )
                    r.emplace(std::move(elem));
                else r.push_back(std::move(elem));
            }
            
            return std::make_pair(n, std::move(r));
        }
    };
}
