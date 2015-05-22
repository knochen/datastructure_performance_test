#include <iostream>

using namespace std;
#include <array>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include "test_data_generator.h"

int main(int argc, const char* const argv[]) {
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("data_structure", boost::program_options::value<std::string>(), "data structure to test")
            ("data_set_size", boost::program_options::value<int>(), "size of the data set for testing")
            ;
    boost::program_options::variables_map vm;
    try {
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
    } catch (const std::exception& e) {
        cout << e.what() << endl;
    }
    boost::program_options::notify(vm);
    enum class data_structure_type : int {
        MAP,
        MULTI_INDEX,
        PROPERTY_TREE
    };
    int data_set_size = 2000;
    data_structure_type data_structure(data_structure_type::MAP);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("data_set_size")) {
        cout << "Data set size is set to  "
        << vm["data_set_size"].as<int>() << ".\n";
    } else {
        cout << "Use default data set size.\n";
    }

    if (vm.count("data_structure")) {
        string name(vm["data_structure"].as<string>());
        boost::to_upper(name);
        if(name == "MAP") {
            cout << "Use data structure  "
            << name << ".\n";
            data_structure = data_structure_type::MAP;
        } else if(name == "MULTI_INDEX") {
            cout << "Use data structure  "
            << name << ".\n";
            data_structure = data_structure_type::MULTI_INDEX;
        } else if(name == "PROPERTY_TREE") {
            cout << "Use data structure  "
            << name << ".\n";
            data_structure = data_structure_type::PROPERTY_TREE;
        } else {
            cout << "Unknown data structure '" << name << "'.\n";
            return 2;
        }
    } else {
        cout << "No data structure chosen.\n";
        return 2;
    }

    cout << "Start testing" << endl;

    // generate test data
    test_data_generator test_data;

    return 0;
}