#pragma once

#include <type_traits>
#include <tuple>



namespace dd99::EIS1
{
    // base template.
    // each type must have a corresponding specialization of this template in order to be supported by the serialization framework.
    template <class T> struct Serializable_Traits;
    template <class T> constexpr bool is_serializable = requires { sizeof(Serializable_Traits<std::remove_cvref_t<T>>); };
    template <class... T> constexpr bool is_serializable<std::tuple<T...>> = (is_serializable<T> && ...);
    template <class T, class U> constexpr bool is_serializable<std::pair<T, U>> = (is_serializable<T> && is_serializable<U>);
    template <class T> concept Serializable = is_serializable<T>;

    // Serializable_Traits functions that can be defined:
    // 
    //  - Serializable_Traits::to_tuple(value);
    // This function is used to decompose a value into a tuple of references to it's internal data elements.
    // It is the easiest way to implement Serializable_Traits for user-defined structs.
    // 
    //  - Serializable_Traits::buffer(value);
    // This function is defined for values that can be directly sent/received
    // It returns a buffer usable in the read/write functions
    // 
    //  - Serializable_Traits::next(value);
    // This function returns the next data level to read/write
}
