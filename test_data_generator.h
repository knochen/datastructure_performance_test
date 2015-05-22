//
// Created by martin on 21.05.15.
//

#ifndef DATASTRUCTURE_PERFORMANCE_TEST_TEST_DATA_GENERATOR_H
#define DATASTRUCTURE_PERFORMANCE_TEST_TEST_DATA_GENERATOR_H

#include <vector>
#include <string>
#include <boost/random/random_device.hpp>

class test_data_generator {
    private:
    const int TREE_DEPTH;
    const int NODE_LEAFES_AVG;
    const int PARAMETER_LENGTH_AVG;
    const unsigned long TREE_SIZE;
    std::vector<std::string> parameter_names;
    boost::random::random_device rng;

    protected:
    void init();
    std::string generate_parameter_name();
    void add_parameter(std::string prefix, int layer, int node_depth);

    public:
    explicit test_data_generator(int tree_depth,
                                 int node_members_avg,
                                 int parameter_name_length_avg,
                                 unsigned long tree_size) :
            TREE_DEPTH(tree_depth),
            NODE_LEAFES_AVG(node_members_avg),
            PARAMETER_LENGTH_AVG(parameter_name_length_avg),
            TREE_SIZE(tree_size),
            parameter_names(TREE_SIZE),
            rng() { init(); }
    explicit test_data_generator() :
            TREE_DEPTH(3),
            NODE_LEAFES_AVG(5),
            PARAMETER_LENGTH_AVG(9),
            TREE_SIZE(2500),
            parameter_names(TREE_SIZE),
            rng() { init(); }
    test_data_generator(const test_data_generator&) = delete;
    virtual ~test_data_generator() { }
};


#endif //DATASTRUCTURE_PERFORMANCE_TEST_TEST_DATA_GENERATOR_H
