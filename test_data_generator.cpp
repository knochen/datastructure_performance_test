//
// Created by martin on 21.05.15.
//

#include "test_data_generator.h"
#include <iostream>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/normal_distribution.hpp>

void test_data_generator::init() {
    parameter_names.reserve(TREE_SIZE);
    boost::random::uniform_int_distribution<> dist_tree_depth(0, TREE_DEPTH - 1);
    std::string name;
    while (parameter_names.size() < TREE_SIZE) {
        name.clear();
        int node_depth = dist_tree_depth(rng);
        add_parameter("", 0, node_depth);
    }
    generate_access_order();
}

std::string test_data_generator::generate_parameter_name() {
    const std::string chars("abcdefghijklmnopqrstuvwxyz");
    boost::random::uniform_int_distribution<> dist_char_index(0, static_cast<int>(chars.size() - 1));
    boost::random::normal_distribution<> dist_parameter_name_length(PARAMETER_LENGTH_AVG, 3);
    std::string name;
    int parameter_name_size = static_cast<int>(dist_parameter_name_length(rng));
    for(int name_index = 0; name_index < parameter_name_size; ++name_index) {
        name.push_back(chars[dist_char_index(rng)]);
    }
    return name;
}

void test_data_generator::add_parameter(std::string prefix, int layer, int node_depth) {
    std::string parameter_name;
    if(!prefix.empty()) {
        parameter_name.append(prefix);
        parameter_name.push_back('.');
    }
    parameter_name.append(generate_parameter_name());
    if(layer == node_depth) {
        parameter_names.push_back(parameter_name);
        return;
    }
    boost::random::normal_distribution<> dist_node_leafes(NODE_LEAFES_AVG, 1.5);
    int node_leafes = static_cast<int>(dist_node_leafes(rng));
    for(int node_leaf = 0; node_leaf < node_leafes; node_leaf++) {
        add_parameter(parameter_name, layer + 1, node_depth);
    }
}

const std::vector<std::string> &test_data_generator::get_parameter_names() const {
    return parameter_names;
}

const std::vector<std::size_t> &test_data_generator::get_access_order() const {
    return access_order;
}

void test_data_generator::generate_access_order() {
    boost::random::uniform_int_distribution<> dist_parameter_access(0, static_cast<int>(TREE_SIZE) - 1);
    std::size_t access_list_length = 1000000;
    access_order.clear();
    access_order.reserve(access_list_length);
    for(std::size_t i = 0; i < access_list_length; i++) {
        access_order.push_back(dist_parameter_access(rng));
    }
}
