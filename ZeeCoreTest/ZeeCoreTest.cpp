#include <iostream>
#include "core/core.h"
#include "math/vec/vec.h"
#include "math/to_string.h"

using namespace zee;
using namespace std;
bool check(math::vec2f b0, math::vec2f e0, math::vec2f b1, math::vec2f e1) {
    math::vec2f b0_e0 = b0 - e0;
    math::vec2f b1_e1 = b1 - e1;
    math::vec2f b0_b1 = b0 - b1;
    if (math::is_near_not_zero(ccw(b0_e0, b1_e1), math::epsilon<float>() * 10)) {
        math::vec2f e0_b1 = e0 - b1;
        const auto n1_dot_b0 = b1_e1.ccw(b0_b1);
        const auto n1_dot_e0 = b1_e1.ccw(e0_b1);
        if ((n1_dot_b0 <= 0 && n1_dot_e0 >= 0) || (n1_dot_e0 <= 0 && n1_dot_b0 >= 0)) {
            math::vec2f b1_b0 = b1 - b0;
            math::vec2f e1_b0 = e1 - b0;
            const auto n0_dot_b1 = b0_e0.ccw(b1_b0);
            const auto n0_dot_e1 = b0_e0.ccw(e1_b0);
            if ((n0_dot_b1 <= 0 && n0_dot_e1 >= 0) || (n0_dot_e1 <= 0 && n0_dot_b1 >= 0)) {
                return true;
            } 
        }

    } else { //평행
        if(math::is_near_zero(b0_b1.ccw(b0_e0), math::epsilon<float>()*10)) {
            const auto len = (b0 - e0).length_sq();
            return len > (b0 - b1).length_sq() || len > (b0 - e1).length_sq();
        }
    }

    return false;
}


int main()
{
    math::vec2f b0 ;//= { -0.270000,5.470000+1 };
    math::vec2f e0 ;//= { 1.67, 4.65 };
    constexpr math::vec2f b1 = { 0.7,5.06 };
    constexpr math::vec2f e1 = { 1.67,4.65 };
    constexpr math::vec2f temp = b1 - e1;
    auto tn = temp.normalize();
    b0 = b1 + tn ;
    e0 = b1 + tn * -2;
    //b0 += math::vec2f::constants::unit_y;
    //e0 += math::vec2f::constants::unit_y;
    //cout << to_string(temp) << ", \n"; //<< to_string(e0) << std::endl;
    cout << to_string(b0) <<", "<< to_string(e0) << std::endl;
    cout << to_string(b1) <<", "<< to_string(e1) << std::endl;
    cout << check(b0, e0, b1, e1) << std::endl;
}
