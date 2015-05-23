#include <iostream>

using namespace std;
#include <array>
#include <chrono>
#include <string>
#include <map>
#include <unordered_map>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/format.hpp>
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

    // test with map
    std::cout << "Start map..." << std::endl;
    std::map<std::string, std::size_t> parameter_map; // the second parameter is the index in the vector

    for(std::size_t index = 0; index < test_data.get_parameter_names().size(); index++) {
        parameter_map.insert({test_data.get_parameter_names().at(index), index});
    }

    boost::accumulators::accumulator_set<unsigned long, boost::accumulators::stats<boost::accumulators::tag::mean> > acc_map;
    for(int i = 0; i < 5; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        for(int index : test_data.get_access_order()) {
            auto iter = parameter_map.find(test_data.get_parameter_names().at(index));
            if(iter == parameter_map.end()) {
                std::cout << "Something is wrong" << std::endl;
                return 3;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << "time consumed: " <<
        std::to_string(time_in_ms) << "ms"  << std::endl;
        acc_map(time_in_ms);
        std::cout << "mean: " << std::to_string(boost::accumulators::mean(acc_map)) << "ms" << std::endl;
    }

    // test with unordered
    std::cout << "Start unordered..." << std::endl;
    std::unordered_map<std::string, std::size_t> parameter_map_unordered; // the second parameter is the index in the vector

    std::cout << "Buckets: " << std::to_string(parameter_map_unordered.bucket_count()) << std::endl;
    for(std::size_t index = 0; index < test_data.get_parameter_names().size(); index++) {
        parameter_map_unordered.insert({test_data.get_parameter_names().at(index), index});
    }
    std::cout << "Buckets: " << std::to_string(parameter_map_unordered.bucket_count()) << std::endl;
    parameter_map_unordered.rehash(10000);
    std::cout << "Buckets: " << std::to_string(parameter_map_unordered.bucket_count()) << std::endl;

    boost::accumulators::accumulator_set<unsigned long, boost::accumulators::stats<boost::accumulators::tag::mean> > acc_map_unordered;
    for(int i = 0; i < 5; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        for(int index : test_data.get_access_order()) {
            auto iter = parameter_map_unordered.find(test_data.get_parameter_names().at(index));
            if(iter == parameter_map_unordered.end()) {
                std::cout << "Something is wrong" << std::endl;
                return 3;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << "time consumed: " <<
        std::to_string(time_in_ms) << "ms"  << std::endl;
        acc_map_unordered(time_in_ms);
        std::cout << "mean: " << std::to_string(boost::accumulators::mean(acc_map_unordered)) << "ms" << std::endl;
    }

    // test with multi-index-container
    std::cout << "Start multi_index..." << std::endl;
    typedef boost::multi_index_container<std::string,
                boost::multi_index::indexed_by<
                        boost::multi_index::sequenced< >,
                        boost::multi_index::ordered_unique<boost::multi_index::identity<std::string> >
                        >
            > string_container;
    boost::accumulators::accumulator_set<unsigned long, boost::accumulators::stats<boost::accumulators::tag::mean> > acc_multi_index;
    string_container string_container1;
    std::copy(test_data.get_parameter_names().begin(),test_data.get_parameter_names().end(),
              std::back_inserter(string_container1));
    for(int i = 0; i < 5; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        for(int index : test_data.get_access_order()) {
            auto iter = string_container1.get<1>().find(test_data.get_parameter_names().at(index));
            if(iter == string_container1.get<1>().end()) {
                std::cout << "Something is wrong" << std::endl;
                return 3;
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto time_in_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << "time consumed: " <<
        std::to_string(time_in_ms) << "ms"  << std::endl;
        acc_multi_index(time_in_ms);
        std::cout << "mean: " << std::to_string(boost::accumulators::mean(acc_multi_index)) << "ms" << std::endl;
    }

    boost::format summary(
        "\n"
        "Summary:\n"
        "         map: 100%%\n"
        "   unordered: %f%%\n"
        " multi-index: %f%%\n");
    summary % ((boost::accumulators::mean(acc_map)/boost::accumulators::mean(acc_map_unordered))*100.0)
            % ((boost::accumulators::mean(acc_map)/boost::accumulators::mean(acc_multi_index))*100.0);

    std::cout << summary.str() << std::endl;
    return 0;
}