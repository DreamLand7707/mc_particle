#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <iostream>
#include "mc_particle.hpp"

namespace py = pybind11;
using namespace pybind11::literals;
#define TOINT(X) (reinterpret_cast<uint64_t>((X)))
struct Point {
    int x;
    int y;
};
int main() {
    using std::numbers::e;
    using std::numbers::egamma;
    using std::numbers::pi;

    py::scoped_interpreter python {};

    py::module sys = py::module::import("sys");
    py::list path = sys.attr("path");
    path.append(PROJECT_DIR);
    py::function print = py::eval("print");
    print(path);

    py::module cppplot = py::module::import("pymodule.cppplot");
    py::function plotVector = cppplot.attr("plotVector");
    py::function plotShow = cppplot.attr("plotShow");
    py::function fft = cppplot.attr("fft");
    py::function print_point = cppplot.attr("print_point");

    std::vector<double> x, y, amplitute(200);
    for (int i = 0; i < 200; i++) {
        x.push_back((i / 200.0) * 20 * pi);
        y.push_back(sin(x.back()));
    }
    Point p {1, 2};

    fft(TOINT(y.data()), TOINT(amplitute.data()), py::make_tuple(200, 200));

    plotVector(TOINT(amplitute.data()), py::none {}, py::make_tuple(200, 200));
    print_point(TOINT(&p));

    plotShow();

    return 0;
}