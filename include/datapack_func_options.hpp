#pragma once

#include "basic_particle.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/stream.hpp>
#include <utility>

namespace mc_particle
{
    using path = boost::filesystem::path;
    class dp_namespace;
    class mc_function;

    class datapack {
     private:
        struct datapack_internal {
            path dp_dir;
            str dp_name;
            bool safety_check = true;
        };
        ptr_s<datapack_internal> dp_i;

        explicit datapack(const ptr_s<datapack_internal> &x) : dp_i(x) {}

     public:
        friend class dp_namespace;
        friend class mc_function;

        datapack() = default;
        datapack(const datapack &) = default;
        datapack(path directory, str name)
            : dp_i(std::make_shared<datapack_internal>(std::move(directory), std::move(name), true)) {}

        [[nodiscard]] bool create_fs() const;
        [[nodiscard]] bool exists_fs() const;
        [[nodiscard]] bool clear_fs() const;
        [[nodiscard]] bool delete_fs() const;

        [[nodiscard]] path get_datapack_dir() const { return dp_i->dp_dir; }
        [[nodiscard]] str get_datapack_name() const { return dp_i->dp_name; }
        [[nodiscard]] bool get_safety_check() const { return dp_i->safety_check; }
        [[nodiscard]] path get_datapack_path() const { return dp_i->dp_dir / dp_i->dp_name; }

        void set_datapack_dir(path p) const { dp_i->dp_dir = std::move(p); }
        void set_datapack_name(str p) const { dp_i->dp_name = std::move(p); }
        void set_safety_check(bool s) { dp_i->safety_check = s; }

        dp_namespace create_namespace(str name);

        [[nodiscard]] datapack clone() const;
    };

    class dp_namespace {
        struct dp_namespace_internal {
            ptr_s<datapack::datapack_internal> dp_i;
            str ns;
            bool safety_check = true;
            bool recursion = true;
        };

        ptr_s<dp_namespace_internal> ns_i;

        explicit dp_namespace(const ptr_s<dp_namespace_internal> &x) : ns_i(x) {}

     public:
        friend class datapack;
        friend class mc_function;

        dp_namespace() = default;
        dp_namespace(const dp_namespace &) = default;
        dp_namespace(dp_namespace &&) = default;
        dp_namespace &operator=(const dp_namespace &) = default;
        dp_namespace &operator=(dp_namespace &&) = default;

        dp_namespace(str ns_str, const datapack &datapack)
            : ns_i(std::make_shared<dp_namespace_internal>(datapack.dp_i, std::move(ns_str), true, true)) {}

        [[nodiscard]] bool create_fs() const;
        [[nodiscard]] bool exists_fs() const;
        [[nodiscard]] bool clear_fs() const;
        [[nodiscard]] bool delete_fs() const;

        [[nodiscard]] path get_namespace_path() const { return get_datapack().get_datapack_path() / "data" / ns_i->ns; }
        [[nodiscard]] path get_datapack_path() const { return ns_i->dp_i->dp_dir / ns_i->dp_i->dp_name; }
        [[nodiscard]] bool get_safety_check() const { return ns_i->safety_check; }
        [[nodiscard]] bool get_recursion() const { return ns_i->recursion; }
        [[nodiscard]] str get_namespace_str() const { return ns_i->ns; }
        [[nodiscard]] datapack get_datapack() const { return datapack(ns_i->dp_i); }

        void set_safety_check(bool s) const { ns_i->safety_check = s; }
        void set_recursion(bool s) const { ns_i->recursion = s; }
        void set_namespace_str(str ns_s) const { ns_i->ns = std::move(ns_s); }

        [[nodiscard]] dp_namespace clone() const;
        [[nodiscard]] mc_function create_mc_function(path folder, str name) const;
    };

    class mc_function {
        struct mc_function_internal {
            path f_folder;
            str f_name;
            ptr_s<dp_namespace::dp_namespace_internal> ns;
            ptr_s<f_stream> fout;
            std::ios::openmode openmode;
            bool safety_check;
            bool recursion;
        };

        ptr_s<mc_function_internal> func_i;
        explicit mc_function(const ptr_s<mc_function_internal> &x) : func_i(x) {}

     public:
        enum format_mode {
            APPEND,
            REPLACE,
            CLEAR
        };

        friend class datapack;
        friend class dp_namespace;

        mc_function() = default;
        mc_function(const mc_function &) = default;
        mc_function(mc_function &&) = default;
        mc_function &operator=(const mc_function &) = default;
        mc_function &operator=(mc_function &&) = default;

        mc_function(path folder, str name, const dp_namespace &ns)
            : func_i(std::make_shared<mc_function_internal>(std::move(folder), std::move(name), ns.ns_i, std::make_shared<f_stream>(), std::ios::out, true, true)) {}

        [[nodiscard]] dp_namespace get_namespace() const { return dp_namespace(func_i->ns); }
        [[nodiscard]] datapack get_datapack() const { return get_namespace().get_datapack(); }
        [[nodiscard]] path get_datapack_path() const { return get_namespace().get_datapack_path(); }
        [[nodiscard]] path get_namespace_path() const { return get_namespace().get_namespace_path(); }
        [[nodiscard]] path get_function_path() const { return get_namespace_path() / "functions" / func_i->f_folder / (func_i->f_name + ".mcfunction"); }
        [[nodiscard]] str get_function_str() const;
        [[nodiscard]] path get_func_folder() const { return func_i->f_folder; }
        [[nodiscard]] str get_func_name() const { return func_i->f_name; }
        [[nodiscard]] bool get_safety_check() const { return func_i->safety_check; }
        [[nodiscard]] bool get_recursion() const { return func_i->recursion; }
        [[nodiscard]] std::ios::openmode get_openmode() const { return func_i->openmode; }
        [[nodiscard]] f_stream &get_stream() const { return *(func_i->fout); }

        [[nodiscard]] bool create_fs() const;
        [[nodiscard]] bool exists_fs() const;
        [[nodiscard]] bool clear_fs() const;
        [[nodiscard]] bool delete_fs() const;

        void set_func_folder(path folder) const { func_i->f_folder = std::move(folder); }
        void set_func_name(str name) const { func_i->f_name = std::move(name); }

        void set_safety_check(bool s) const { func_i->safety_check = s; }
        void set_recursion(bool s) const { func_i->recursion = s; }
        void set_openmode(int s) const { func_i->openmode = s; }

        void close() const { get_stream().close(); }
        [[nodiscard]] bool is_open() const { return get_stream().is_open(); }
        [[nodiscard]] mc_function clone() const;
        [[nodiscard]] str format(int time = 0, format_mode mode = APPEND) const;
        [[nodiscard]] bool open() const {
            get_stream().open(get_function_path().string(), func_i->openmode);
            return get_stream().good();
        }
        [[nodiscard]] bool open(std::ios_base::openmode openmode) const {
            get_stream().open(get_function_path().string(), openmode);
            return get_stream().good();
        }
        template <class T>
        const mc_function &operator<<(T &&arg) const {
            get_stream() << std::forward<T &&>(arg);
            return *this;
        }
        template <class T>
        mc_function &operator<<(T &&arg) {
            get_stream() << std::forward<T &&>(arg);
            return *this;
        }
        [[nodiscard]] mc_function create_temp_file(const str &file_name) const { return get_namespace().create_mc_function(get_func_folder() / "temp" / (get_func_name() + "_temp"), file_name); }
        [[nodiscard]] bool delete_temp_file(const str &file_name) const { return create_temp_file(file_name).delete_fs(); }
        [[nodiscard]] bool delete_temp_folder() const;
    };
} // namespace mc_particle