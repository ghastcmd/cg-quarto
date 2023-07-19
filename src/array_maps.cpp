#include "pch.h"

#include "enum_types.hpp"
#include "hash_array.hpp"

template <typename _ty>
struct hash_func_objtypes
{
    inline constexpr _ty operator()(const char *in_str) const noexcept
    {
        return in_str[0] << 8 | in_str[1];
    }
};

DECLARE_ARRAY_MAP(objtypes_array_map, 11, objtypes, std::uint16_t, hash_func_objtypes, 
    array_map::pair{"mt",   objtypes::mtllib},
    array_map::pair{"#\0",  objtypes::comment},
    array_map::pair{"o\0",  objtypes::object_name},
    array_map::pair{"v\0",  objtypes::vertex_coord},
    array_map::pair{"vn",   objtypes::vertex_normal},
    array_map::pair{"vt",   objtypes::vertex_texture},
    array_map::pair{"us",   objtypes::usemtl},
    array_map::pair{"f\0",  objtypes::face},
    array_map::pair{"l\0",  objtypes::line},
    array_map::pair{"s\0",  objtypes::smooth_shading},
    array_map::pair{"\n\0", objtypes::new_line}
)
