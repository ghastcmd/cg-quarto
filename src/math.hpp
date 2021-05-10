#pragma once

template <typename _ty>
constexpr _ty PI = (_ty)3.14159265358979323846;

template <typename _ty>
_ty radians(_ty val)
{
    return val * (PI<_ty> / (_ty)180.0f);
}