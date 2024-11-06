#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdint>
#include <format>

struct proj_depend {
    std::map<uint32_t, uint8_t> depends;
};

struct bimap {
    std::unordered_map<std::string, uint32_t> left;
    std::unordered_map<uint32_t, std::string> right;
};

bimap &get_projname_map() {
    static bimap projname_map;
    return projname_map;
}

int main(int argc, char **argv) {

    std::string dependence(argv[1]);
    std::string custom_dependence(argv[2]);
    std::string all_projs(argv[3]);

    std::fstream
        fin1(dependence, std::ios::in),
        fin2(all_projs, std::ios::in),
        fin3(custom_dependence, std::ios::in);


    std::vector<proj_depend> dependence_graph;

    std::string str1, str2, str3, str4;
    uint32_t temp1 = 0;
    for (std::getline(fin2, str1); fin2; std::getline(fin2, str1)) {
        if (get_projname_map().left.find(str1) == get_projname_map().left.end()) {
            get_projname_map().left.insert({str1, temp1});
            get_projname_map().right.insert({temp1, str1});
            dependence_graph.emplace_back();
            ++temp1;
        }
    }

    std::stringstream ssin;
    bool get_depend = false, temp2 = false;
    std::set<uint32_t> target;
    uint32_t line_number = 0;
    std::getline(fin1, str1);
    while (fin1 || fin3) {
        line_number++;
        ssin.clear();
        ssin.str(str1);
        get_depend = false;

        for (ssin >> str2; ssin; ssin >> str2) {
            if (str2 == "-") {
                get_depend = true;
                temp2 = false;
                continue;
            }

            if (!get_depend) {
                try {
                    if (!temp2) {
                        target.clear();
                        temp2 = true;
                    }
                    target.insert(get_projname_map().left.at(str2));
                }
                catch (const std::out_of_range &e) {
                    std::cerr << std::format("[Error] At line {} : Can't Find Project \"{}\"\n",
                                             line_number, str2)
                              << std::flush;

                    return -1;
                }
            }
            else {
                try {
                    for (auto x : target) {
                        dependence_graph[x].depends[get_projname_map().left.at(str2)] = 0;
                    }
                }
                catch (const std::out_of_range &e) {
                    std::cerr << std::format("[Error] At line {} : Can't Find Project \"{}\"\n",
                                             line_number, str2)
                              << std::flush;
                    return -1;
                }
            }
        }
        if (fin1) {
            std::getline(fin1, str1);
        }
        else if (fin3) {
            std::getline(fin3, str1);
        }
    }

    std::vector<uint32_t> route;
    std::set<uint32_t> route_set;
    std::vector<std::pair<uint32_t, uint32_t>> weight(dependence_graph.size());
    route.reserve(dependence_graph.size() + 1);
    for (uint32_t i = 0; i < weight.size(); i++)
        weight[i] = {i, 0};

    for (uint32_t i = 0; i < dependence_graph.size(); i++) {
        route.push_back(i);
        route_set.insert(i);

        for (; route.size() > 0;) {
            bool have_not = false;
            uint32_t curr = route.back();
            for (auto &x : dependence_graph.at(curr).depends) {
                if (route_set.find(x.first) == route_set.end()) { // don't have circle
                    if (x.second == 0) {
                        have_not = true;
                        if (weight[x.first].second < (weight[curr].second + 1)) {
                            weight[x.first].second = (weight[curr].second + 1);
                        }
                        route.push_back(x.first);
                        route_set.insert(x.first);
                        x.second = 1;
                        break;
                    }
                    else {
                        continue;
                    }
                }
                else { // have circle
                    std::cerr << std::format("[Error] Graph of Dependencies have a circle\n")
                              << std::format("Dependencies Path is : \n");

                    for (uint32_t k = 0; k < route.size(); k++) {
                        std::cerr << get_projname_map().right.at(route[k]) << " -> ";
                    }
                    std::cerr << get_projname_map().right.at(x.first) << std::endl;

                    return -1;
                }
            }
            if (!have_not) {
                for (auto &x : dependence_graph.at(curr).depends) {
                    x.second = 0;
                }
                route_set.erase(curr);
                route.pop_back();
            }
        }
    }

    std::sort(weight.begin(), weight.end(),
              [](const std::pair<uint32_t, uint32_t> &lhs, const std::pair<uint32_t, uint32_t> &rhs) -> bool
              {
                  if (lhs.second > rhs.second)
                      return true;
                  return false;
              });

    for (auto p = weight.begin(); p != weight.end(); p++) {
        if (p == weight.begin()) {
            std::cout << get_projname_map().right.at(p->first);
        }
        else {
            std::cout << ';' << get_projname_map().right.at(p->first);
        }
    }
    std::cout << std::flush;


    return 0;
}