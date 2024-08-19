#pragma once

// EIS stands for "Endian Independent Serialization"
// The number one ('1') represents the concept version
// There are many other ways to implement serialization

// This library provides a framework for (de)serialization of arbitrary types
// And defines standard (de)serialization traits for basic types

// If you need more freedom regarding basic types, do not include this header; use the individual headers below instead

#include <dd99/EIS1/serializable_traits.hpp>
#include <dd99/EIS1/eis_implementation.hpp>
#include <dd99/EIS1/basic_type_support/eis_integral.hpp>
#include <dd99/EIS1/basic_type_support/eis_container.hpp>
#include <dd99/EIS1/basic_type_support/eis_optional.hpp>
