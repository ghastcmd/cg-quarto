#pragma once

#include <array>

namespace array_map
{

template <typename _enum>
struct pair
{
    std::string_view literal;
    _enum value;
    constexpr pair(std::string_view ll, _enum vv)
        : literal(ll), value(vv)
    {
        
    }
};

template <typename _hash_ty, template <typename> class _hash_func, typename _ty, std::size_t _size>
constexpr std::size_t max_const_array(const std::array<_ty, _size>& arr) noexcept
{
    std::size_t max = 0, value = 0;
    constexpr _hash_func<_hash_ty> hash_func;
    for (const auto &val : arr)
    {
        value = static_cast<std::size_t>(hash_func(val.literal.data()));
        if (value > max)
        {
            max = value;
        }
    }

    return max + 1;
}

template <typename _hash_ty, template<typename> class _hash, typename _enum, std::size_t _arr_size>
struct wrapper
{
    inline constexpr _enum operator[](const char* sv) const noexcept
    {
        return m_arr[m_get_key(sv)];
    }

    _hash<_hash_ty> m_get_key;
    std::array<_enum, _arr_size> m_arr {_enum::invalid};
};

template <
    typename                    _hash_ty,
    template<typename> class    _hash,
    std::size_t                 _arr_size,
    typename                    _enum,
    std::size_t                 _in_size,
    template<typename> class    _pair_type
>
constexpr wrapper<_hash_ty, _hash, _enum, _arr_size>
// constexpr auto
create_thing(const std::array<_pair_type<_enum>, _in_size> &arr)
{
    _hash<_hash_ty> get_key;
    
    // constexpr std::size_t array_size = max_const_array<_hash_ty, _hash>(arr);

    wrapper<_hash_ty, _hash, _enum, _arr_size> ret_wrapper {};
    // wrapper<_hash_ty, _hash, _enum, array_size> ret_wrapper {};
    auto &wrapper_arr = ret_wrapper.m_arr;

    for (const auto &val : arr)
    {
        wrapper_arr[get_key(val.literal.data())] = val.value;
    }

    return ret_wrapper;
}

}

/// MACRO for declaring the hash_array

#define DECLARE_ARRAY_MAP(name, array_size, enum_type, hash_type, hash_class, ...) \
namespace name { \
static constexpr std::array<::array_map::pair<::enum_type>, array_size> name ## _arr { \
    __VA_ARGS__ \
}; \
static constexpr std::size_t name ## _size = array_map::max_const_array<hash_type, hash_class>(name ## _arr); \
static constexpr array_map::wrapper<hash_type, hash_class, ::enum_type, name ## _size> name = array_map::create_thing<hash_type, hash_class, name ## _size, ::enum_type>(name ## _arr); \
const ::enum_type get(const char *str) { \
    return name[str]; \
} \
}

#define DECLARE_GLOBAL_ARRAY_MAP(name, enum_type) \
namespace name { \
extern const ::enum_type get(const char *str); \
}
