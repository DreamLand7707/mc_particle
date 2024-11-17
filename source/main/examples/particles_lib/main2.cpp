#include "mc_particle.hpp"
#include <Eigen/Eigen>
#include <boost/math/special_functions.hpp>
#include <iostream>
#include <locale>
#include <numbers>
#include <iomanip>

using namespace mc_particle;

int main() {
    Eigen::Vector3<real_number> A, B, C, D;
    A = {1.0, 2.0, 3.0};
    B = {4.0, 6.0, 9.0};
    C = {1.2, 3.6, 9.5};
    D = {9.6, 3.2, 6.5};
    Eigen::Matrix<real_number, 3, 4> points;
    points << A, B, C, D;
    std::cout << points << std::endl;
    cube_bezier_integral_polynomial pol(points.transpose());
    real_number ans = 0;
    for (int i = 0; i < 10000; i++) {
        ans += pol((1.0 / 10000) * i) * (1.0 / 10000);
    }
    std::cout.setf(std::ios::fixed, std::ios::floatfield);
    std::cout.precision(12);
    std::cout << "Exactly: " << ans << std::endl;

    real_number res;
    res = gauss_legender_integral<8>(pol, 0, 1);
    std::cout << "level :" << 8 << ' ' << std::setw(16) << res << " -> " << std::setw(16) << (res - ans) / ans << std::endl;
}