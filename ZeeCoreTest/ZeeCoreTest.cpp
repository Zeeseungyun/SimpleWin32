﻿#include <iostream>
#include "core/core.h"
#include "math/vec/vec.h"
#include "math/to_string.h"
//#include "log/log.h"

using namespace zee;
using namespace std;
#include <thread>
int main() {
    constexpr math::vec2f a0 = { 2,-3 };
    constexpr math::vec2f a1 = { 2,3 };
    constexpr math::vec2f b0 = { 1,0 };
    constexpr math::vec2f b1 = { 3,0 };
    constexpr auto va = a1 - a0;
    constexpr auto vb = b1 - b0;
    constexpr auto a0_b0 = a0 - b0;
    constexpr auto b0_a0 = -a0_b0;
    constexpr auto vb_dot_a0_b0 = vb.dot(a0_b0);
    constexpr auto va_dot_b0_a0 = va.dot(b0_a0);
    constexpr auto va_dot_vb = va.dot(vb);
    constexpr auto denom = va.length_sq() * vb.length_sq() - va_dot_vb * va_dot_vb;
    constexpr auto fa = (va_dot_vb * vb_dot_a0_b0 + va_dot_b0_a0 * vb.length_sq()) / denom;
    constexpr auto fb = (va_dot_vb * math::saturate(fa) + vb_dot_a0_b0) / vb.length_sq();
    constexpr bool is_intersect = fa >= 0 && fa <= 1.0f;
    std::vector<int> vi1 = { 1,2,3 };
    std::vector<int> vi2 = { 1,2,3 };
    std::cout << (vi1.begin() == vi2.begin());

    //print_test();
    //ZEE_LOG(normal, TEXT("asdfasdf"));
    return 0;
}
