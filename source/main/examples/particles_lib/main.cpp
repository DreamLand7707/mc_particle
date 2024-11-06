#include "mc_particle.hpp"
#include <iostream>
#include <filesystem>
int main() {

    using namespace mc_particle;
    namespace bf = boost::filesystem;

    datapack dp(path(ROOT_DIR) / "resource/public/datapacks", "my_datapack");
    dp_namespace ns = dp.create_namespace("example");
    mc_function func = ns.create_mc_function("", "test_func");

    if (!dp.create_fs())
        std::cout << "Error: Can't Create DataPack!" << std::endl;
    if (!ns.create_fs())
        std::cout << "Error: Can't Create Namespace!" << std::endl;
    if (!func.create_fs())
        std::cout << "Error: Can't Create Function!" << std::endl;

    particle_parameter_options para_opti(
        {1.0, 1.0, 1.0, 1.0},
        parameter_particle_options(
            {0.0, 10.0},
            200,
            0.05,
            false,
            particle_basic_options()));

    para_opti.set_para_counting_period(0.01);
    para_opti.confirm_particle_num();

    linear_particle lp;

    vector3D<real_number> points[] = {{177, 95, -221}, {184, 100, -215}, {190, 106, -209}};

    std::cout << lp.solid_color_expand_spiral_instantaneous_file(para_opti, func,
                                                                 points[0], points[2], true, 0, 6,
                                                                 5, 0.2);

    return 0;
}
