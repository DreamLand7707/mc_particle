#include "datapack_func_options.hpp"

namespace mc_particle
{
    namespace fs = boost::filesystem;
    bool datapack::create_fs() const {
        if (dp_i->safety_check) {
            if (!fs::exists(dp_i->dp_dir))
                return false;
        }

        path temp_path = get_datapack_path();
        if (!fs::exists(temp_path)) {
            fs::create_directories(temp_path);
        }
        else {
            if (dp_i->safety_check && !fs::is_directory(temp_path))
                return false;
        }

        temp_path /= "data";
        if (!fs::exists(temp_path)) {
            fs::create_directories(temp_path);
        }
        else {
            if (dp_i->safety_check && !fs::is_directory(temp_path))
                return false;
        }

        temp_path /= "../pack.mcmeta";
        if (!fs::exists(temp_path)) {
            fs::ofstream fout(temp_path, std::ios::out);
            fout << R"({
    "pack": {
        "pack_format": 8,
        "description": "Pack Description"
    }
}
)";
            fout.close();
        }
        else {
            if (dp_i->safety_check && !fs::is_regular_file(temp_path))
                return false;
        }
        return true;
    }
    bool datapack::delete_fs() const {
        if (dp_i->safety_check) {
            if (!fs::exists(dp_i->dp_dir))
                return false;
        }
        path temp_path = get_datapack_path();
        if (fs::exists(temp_path))
            fs::remove_all(temp_path);
        return true;
    }
    bool datapack::exists_fs() const {
        if (dp_i->safety_check) {
            if (!fs::exists(dp_i->dp_dir))
                return false;
        }
        path temp_path = get_datapack_path();
        if (!fs::exists(temp_path) || (dp_i->safety_check && !fs::is_directory(temp_path))) {
            return false;
        }
        temp_path /= "data";
        if (!fs::exists(temp_path) || (dp_i->safety_check && !fs::is_directory(temp_path))) {
            return false;
        }
        temp_path /= "../pack.mcmeta";
        if (!fs::exists(temp_path) || (dp_i->safety_check && !fs::is_regular_file(temp_path))) {
            return false;
        }
        return true;
    }
    bool datapack::clear_fs() const {
        if (dp_i->safety_check) {
            if (!fs::exists(dp_i->dp_dir))
                return false;
        }
        path temp_path = get_datapack_path() / "data";
        if (!fs::exists(temp_path) || (dp_i->safety_check && !fs::is_directory(temp_path))) {
            return false;
        }
        fs::directory_iterator di(temp_path);
        for (const auto &x : di)
            fs::remove_all(x);
        return true;
    }

    bool dp_namespace::create_fs() const {
        if (ns_i->safety_check) {
            datapack dp = get_datapack();
            if (!dp.exists_fs()) {
                if (ns_i->recursion) {
                    if (!dp.create_fs())
                        return false;
                }
                else
                    return false;
            }
        }
        else if (ns_i->recursion) {
            datapack dp = get_datapack();
            if (!dp.exists_fs()) {
                if (!dp.create_fs())
                    return false;
            }
        }

        path temp_path = get_namespace_path();
        if (!fs::exists(temp_path)) {
            fs::create_directories(temp_path);
        }
        else {
            if (ns_i->safety_check && !fs::is_directory(temp_path))
                return false;
        }

        temp_path /= "functions";
        if (!fs::exists(temp_path)) {
            fs::create_directories(temp_path);
        }
        else {
            if (ns_i->safety_check && !fs::is_directory(temp_path))
                return false;
        }

        return true;
    }
    bool dp_namespace::delete_fs() const {
        if (ns_i->safety_check) {
            datapack dp = get_datapack();
            if (!dp.exists_fs()) {
                return false;
            }
        }
        path temp_path = get_namespace_path();
        if (fs::exists(temp_path))
            fs::remove_all(temp_path);
        return true;
    }
    bool dp_namespace::exists_fs() const {
        if (ns_i->safety_check) {
            datapack dp = get_datapack();
            if (!dp.exists_fs()) {
                return false;
            }
        }
        path temp_path = get_namespace_path();
        if (!fs::exists(temp_path) || (ns_i->safety_check && !fs::is_directory(temp_path))) {
            return false;
        }
        temp_path /= "functions";
        if (!fs::exists(temp_path) || (ns_i->safety_check && !fs::is_directory(temp_path))) {
            return false;
        }

        return true;
    }
    bool dp_namespace::clear_fs() const {
        if (ns_i->safety_check) {
            datapack dp = get_datapack();
            if (!dp.exists_fs()) {
                return false;
            }
        }
        path temp_path = get_namespace_path() / "functions";
        if (!fs::exists(temp_path) || (ns_i->safety_check && !fs::is_directory(temp_path))) {
            return false;
        }
        fs::directory_iterator di(temp_path);
        for (const auto &x : di)
            fs::remove_all(x);
        return true;
    }

    bool mc_function::create_fs() const {
        if (func_i->safety_check) {
            dp_namespace ns = get_namespace();
            if (!ns.exists_fs()) {
                if (func_i->recursion) {
                    if (!ns.create_fs())
                        return false;
                }
                else
                    return false;
            }
        }
        else if (func_i->recursion) {
            dp_namespace ns = get_namespace();
            if (!ns.exists_fs()) {
                if (!ns.create_fs())
                    return false;
            }
        }
        path temp_path = get_function_path(), temp_path2 = temp_path.parent_path();
        if (!fs::exists(temp_path2)) {
            fs::create_directories(temp_path2);
        }
        else {
            if (func_i->safety_check && !fs::is_directory(temp_path2))
                return false;
        }
        if (!fs::exists(temp_path)) {
            if (open(std::ios::out))
                close();
            else
                return false;
        }
        else {
            if (func_i->safety_check && !fs::is_regular_file(temp_path))
                return false;
        }
        return true;
    }
    bool mc_function::delete_fs() const {
        if (func_i->safety_check) {
            dp_namespace ns = get_namespace();
            if (!ns.exists_fs()) {
                return false;
            }
        }
        path temp_path = get_function_path(), temp_path2 = get_namespace_path() / "functions";
        if (fs::exists(temp_path)) {
            if (!fs::is_regular_file(temp_path))
                return false;
            fs::remove(temp_path);
            temp_path = temp_path.parent_path();
        }
        while (temp_path != temp_path2 && fs::is_empty(temp_path)) {
            fs::remove(temp_path);
            temp_path = temp_path.parent_path();
        }
        return true;
    }
    bool mc_function::exists_fs() const {
        if (func_i->safety_check) {
            dp_namespace ns = get_namespace();
            if (!ns.exists_fs()) {
                return false;
            }
        }
        path temp_path = get_function_path();
        if (!fs::exists(temp_path) || (func_i->safety_check && !fs::is_regular_file(temp_path))) {
            return false;
        }
        return true;
    }
    bool mc_function::clear_fs() const {
        if (func_i->safety_check) {
            dp_namespace ns = get_namespace();
            if (!ns.exists_fs()) {
                return false;
            }
        }
        path temp_path = get_function_path();
        if (!fs::exists(temp_path) || (func_i->safety_check && !fs::is_regular_file(temp_path))) {
            return false;
        }
        if (open(std::ios::out))
            close();
        else
            return false;
        return true;
    }

    datapack datapack::clone() const { return datapack(dp_i); }
    dp_namespace dp_namespace::clone() const { return dp_namespace(ns_i); }
    mc_function mc_function::clone() const { return mc_function(func_i); }

    dp_namespace datapack::create_namespace(str name) { return {std::move(name), *this}; }
    mc_function dp_namespace::create_mc_function(path folder, str name) const { return {std::move(folder), std::move(name), *this}; }

    str mc_function::get_function_str() const {
        path func_p = fs::canonical(get_function_path()),
             func_g = fs::canonical(get_namespace_path() / "functions");
        return get_namespace().get_namespace_str() + ":" + func_p.lexically_relative(func_g).replace_extension().generic_string();
    }

    str mc_function::format(int time, format_mode mode) const {
        if (time <= 0)
            return std::format("function {}", get_function_str());
        switch (mode) {
        case APPEND:
            return std::format("schedule function {} {}t append", get_function_str(), time);
        case REPLACE:
            return std::format("schedule function {} {}t replace", get_function_str(), time);
        case CLEAR:
            return std::format("schedule clear {}", get_function_str());
        default:
            return std::format("schedule function {} {}t append", get_function_str(), time);
        }
    }


} // namespace mc_particle