#define CATCH_CONFIG_MAIN

#ifndef EMP_TRACK_MEM
#define EMP_TRACK_MEM
#endif

#include "third-party/Catch/single_include/catch2/catch.hpp"

#include "emp/data/DataNode.hpp"
#include "emp/data/DataManager.hpp"
#include "emp/data/DataInterface.hpp"
#include "emp/data/DataFile.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

// This function will tell us if the file generated by DataFile is identitcal
// to the expected file.
bool compareFiles(const std::string& p1, const std::string& p2) {
    // From mtrw's answer to https://stackoverflow.com/questions/6163611/compare-two-files
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; //file problem
    }

    if (f1.tellg() != f2.tellg()) {
        return false; //size mismatch
    }

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                        std::istreambuf_iterator<char>(),
                        std::istreambuf_iterator<char>(f2.rdbuf()));
}
int test_fun() {
  static int val = 10;
  return val += 3;
}

<<<<<<< HEAD
TEST_CASE("Test DataFile", "[data]") {
     int test_int = 5;

     emp::DataFile dfile("new_test_file.dat");

     REQUIRE(dfile.GetFilename() == "new_test_file.dat");

     emp::DataMonitor<double> data_fracs;
     emp::DataMonitor<int> data_squares;
     emp::DataMonitor<uint64_t> data_cubes;

     dfile.AddCurrent(data_fracs);
     dfile.AddCurrent(data_squares);
     dfile.AddCurrent(data_cubes);
     dfile.AddMean(data_cubes);
     dfile.AddTotal(data_cubes);
     dfile.AddMin(data_cubes);
     dfile.AddMax(data_cubes);
     dfile.AddStandardDeviation(data_cubes);
     dfile.AddSkew(data_cubes);
     dfile.AddKurtosis(data_cubes);
     dfile.AddFun<int>(test_fun);
     dfile.AddVar<int>(test_int);

     double frac = 0.0;
     for (size_t i = 0; i < 10; i++) {
         test_int += i;
         data_fracs.Add(frac += 0.01);
         data_squares.Add((int)(i*i));
         data_cubes.Add(i*i*i);
         dfile.Update();

         std::cout << i << '\n';
     }

     dfile.SetupLine("[[",":", "]]\n");
     for (size_t i = 10; i < 20; i++) {
         data_fracs.Add(frac += 0.01);
         data_squares.Add((int)(i*i));
         data_cubes.Add(i*i*i);
         dfile.Update();
         std::cout << i << '\n';
     }

     REQUIRE(compareFiles("new_test_file.dat", "test_file.dat"));
 }

TEST_CASE("Test Container DataFile", "[data]") {

    emp::vector<int> cool_data({1,2,3});
    std::function<emp::vector<int>(void)> get_data = [&cool_data](){return cool_data;};
    emp::ContainerDataFile<emp::vector<int>> dfile("new_test_container_file.dat");

    dfile.SetUpdateContainerFun(get_data);

    std::function<int(const int&)> return_val = [](const int& i){return i;};
    std::function<int(const int&)> square_val = [](const int& i){return i*i;};

    dfile.AddContainerFun(return_val, "value", "value");
    dfile.AddContainerFun(square_val, "squared", "value squared");

    dfile.PrintHeaderKeys();
    dfile.Update();
    cool_data.push_back(5);
    dfile.Update();

    // Since update is virtual, this should work on a pointer to a base datafile
    emp::DataFile * data_ptr = & dfile;

    cool_data.push_back(6);
    data_ptr->Update();

    dfile.SetTimingRepeat(2);
    cool_data.clear();
    cool_data.push_back(7);
    cool_data.push_back(3);
    dfile.Update(2);
    dfile.Update(3);
    cool_data.push_back(2);
    data_ptr->Update(4);
    data_ptr->Update(5);

    REQUIRE(compareFiles("new_test_container_file.dat", "test_container_file.dat"));

    auto dfile2 = emp::MakeContainerDataFile(get_data, "new_test_make_container_file.dat");
    dfile2.AddContainerFun(return_val, "value", "value");
    dfile2.AddContainerFun(square_val, "squared", "value squared");

    dfile2.PrintHeaderKeys();
    dfile2.Update();

    REQUIRE(compareFiles("new_test_make_container_file.dat", "test_make_container_file.dat"));
}

TEST_CASE("Test timing", "[data]") {
    int test_int = 5;

    emp::DataFile dfile("new_test_timing_file.dat");

    emp::DataMonitor<double> data_fracs;
    emp::DataMonitor<int> data_squares;
    emp::DataMonitor<uint64_t> data_cubes;

    dfile.AddVar<int>(test_int);
    dfile.AddCurrent(data_fracs);
    dfile.AddCurrent(data_squares);
    dfile.AddCurrent(data_cubes);
    dfile.AddMean(data_cubes);
    dfile.AddTotal(data_cubes);
    dfile.AddMin(data_cubes);
    dfile.AddMax(data_cubes);
    dfile.AddFun<int>(test_fun);

    double frac = 0.0;

    dfile.SetTimingRepeat(2);

    for (size_t i = 0; i < 10; i++) {
        test_int = i;
        data_fracs.Add(frac += 0.01);
        data_squares.Add((int)(i*i));
        data_cubes.Add(i*i*i);
        dfile.Update(i);

        std::cout << i << '\n';
    }

    dfile.SetTimingOnce(5);

    for (size_t i = 0; i < 10; i++) {
        test_int = i;
        data_fracs.Add(frac += 0.01);
        data_squares.Add((int)(i*i));
        data_cubes.Add(i*i*i);
        dfile.Update(i);
        std::cout << i << '\n';
    }

    dfile.SetTimingRange(2, 3, 9);

    for (size_t i = 0; i < 10; i++) {
        test_int = i;
        data_fracs.Add(frac += 0.01);
        data_squares.Add((int)(i*i));
        data_cubes.Add(i*i*i);
        dfile.Update(i);

        std::cout << i << '\n';
    }

    dfile.SetTiming([](size_t ud){return (bool)floor(sqrt((double)ud) == ceil(sqrt((double)ud)));});

    for (size_t i = 0; i < 10; i++) {
        test_int = i;
        data_fracs.Add(frac += 0.01);
        data_squares.Add((int)(i*i));
        data_cubes.Add(i*i*i);
        dfile.Update(i);

        std::cout << i << '\n';
     }

     REQUIRE(compareFiles("new_test_timing_file.dat", "test_timing_file.dat"));
 }

TEST_CASE("Test DataFile AddVal", "[data]") {

    emp::DataFile dfile("new_test_file_val.dat");

    dfile.AddVal("Value 1");
    dfile.AddVal("Value 2", "Key 2");
    dfile.Update();
    dfile.AddVal("Value 3", "Key 3", "Description 3");


    dfile.AddVal(32);
    dfile.Update();

    REQUIRE(compareFiles("new_test_file_val.dat", "test_file_val.dat"));
}
