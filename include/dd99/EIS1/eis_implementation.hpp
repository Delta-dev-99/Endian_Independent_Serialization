#pragma once

#include <dd99/EIS1/serializable_traits.hpp>


namespace dd99::EIS1
{

    namespace detail
    {
        template <class> struct is_tuple : std::false_type {};
        template <class ...T> struct is_tuple<std::tuple<T...>> : std::true_type {};
        template <class T, class U> struct is_tuple<std::pair<T, U>> : std::true_type {};
        // template <class T, std::size_t N> struct is_tuple<std::array<T, N>> : std::true_type {};
        template <class T> inline constexpr bool is_tuple_v = is_tuple<T>::value;

        template <class> struct is_pair : std::false_type {};
        template <class T, class U> struct is_pair<std::pair<T, U>> : std::true_type {};
        template <class T> inline constexpr bool is_pair_v = is_pair<T>::value;
    }



    template <Serializable T, class OutputIt> std::size_t serialize(OutputIt out, const T & v)
    {
        using value_type = std::remove_cvref_t<T>;
        using traits = Serializable_Traits<value_type>;

        if constexpr ( detail::is_tuple_v<T> )
            return std::apply([out](const auto & ... x){ return (serialize(out, x) + ...); }, v);
        else if constexpr ( requires { traits::to_tuple(v); } )
            return serialize(out, Serializable_Traits<T>::to_tuple(v));
        // else if constexpr ( requires { traits::serialize(out, v); } )
        else // assumed: constexpr ( requires { traits::serialize(out, v); } )
            return traits::serialize(out, v);
    }

    template <Serializable ... Args, class OutputIt> std::size_t serialize(OutputIt out, const Args & ... args)
    {
        return (serialize(out, args) + ...);
    }



    template <Serializable T, class InputIt>
    std::pair<std::size_t, T> deserialize(InputIt in);

    template <class T, class InputIt, std::size_t N = 0>
    auto deserialize_tuple(InputIt in)
    {
        if constexpr ( detail::is_pair_v<T> )
        {
            auto [n1, r1] = deserialize<std::tuple_element_t<0, T>>(in);
            in += n1;
            auto [n2, r2] = deserialize<std::tuple_element_t<1, T>>(in);
            return std::make_pair(n1 + n2, std::make_pair(std::move(r1), std::move(r2)));
        }
        else if constexpr ( std::tuple_size_v<T> - N == 0 )
            return std::pair<std::size_t, T>{0, {}};
        else if constexpr ( std::tuple_size_v<T> - N == 1 )
        {
            auto [n, r] = deserialize<std::tuple_element_t<N, T>>(in);
            return std::pair{n, std::make_tuple(std::move(r))};
        }
        else
        {
            auto [n, r] = deserialize<std::tuple_element_t<N, T>>(in);
            in += n;
            auto [n2, r2] = deserialize_tuple<T, InputIt, N + 1>(in);
            auto rt = std::make_tuple(std::move(r));
            return std::make_pair(n + n2, std::tuple_cat(std::move(rt), std::move(r2)));
        }
    }

    template <Serializable T, class InputIt>
    std::pair<std::size_t, T> deserialize(InputIt in)
    {
        using value_type = std::remove_cvref_t<T>;
        using traits = Serializable_Traits<value_type>;

        if constexpr ( detail::is_tuple_v<T> )
            return deserialize_tuple<T>(in);
        else if constexpr ( requires { traits::from_tuple(std::declval<typename traits::tuple_type>()); } )
        {
            // typename traits::tuple_type r;
            // std::size_t n;
            // auto tmp = traits::to_tuple(r);
            // std::tie(n, tmp) = deserialize<decltype(traits::to_tuple(r))>(in);
            auto [n, r] = deserialize<typename traits::tuple_type>(in);
            return {std::move(n), traits::from_tuple(std::move(r))};
        }
        else // assumed: constexpr ( requires { traits::deserialize(out); } )
            return traits::deserialize(in);
    }

}
