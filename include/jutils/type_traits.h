// Copyright © 2021-2023 Leonov Maksim. All Rights Reserved.

#pragma once

#include <type_traits>

namespace jutils
{
    template<typename T1, typename T2>
    constexpr bool is_same_class = std::is_same_v<std::remove_cvref_t<T1>, std::remove_cvref_t<T2>>;

    template<typename T>
    constexpr bool is_abstract_class = std::is_abstract_v<std::remove_cvref_t<T>>;

    template<typename Base, typename Child>
    constexpr bool is_base_class = std::is_base_of_v<std::remove_cvref_t<Base>, std::remove_cvref_t<Child>>;
    template<typename Base, typename Child>
    constexpr bool is_derived_from_class = is_base_class<Base, Child> && !is_same_class<Base, Child>;
    template<typename Base, typename Child>
    constexpr bool is_base_and_not_abstract_class = is_base_class<Base, Child> && !is_abstract_class<Child>;
}
