#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "DSMap.h"

// add includes


TEST_CASE("Testing DSMap") {
    DSMap<int, string> myMap;
    myMap.insert(1, "Reece");
    myMap.insert(2, "Alden");
    myMap.insert(3, "Alejandro");
    myMap.insert(4, "Alex");
    myMap.insert(5, "Fondren");

    //Tests to make sure the key returns the correct value
    REQUIRE(myMap.get(1) == "Reece");
    REQUIRE(myMap.get(2) == "Alden");
    REQUIRE(myMap.get(3) == "Alejandro");
    REQUIRE(myMap.get(4) == "Alex");
    REQUIRE(myMap.get(5) == "Fondren");

    //myMap.get(6);

    //If the get() function is used with a key that doesn't exist, an error is returned
    try {
        myMap.get(6);
    }

    catch (std::runtime_error &e) {
        cerr << e.what() << endl;
    }

    //Makes sure the contains() function works
    REQUIRE(myMap.contains(1) == true);
    REQUIRE(myMap.contains(2) == true);
    REQUIRE(myMap.contains(3) == true);
    REQUIRE(myMap.contains(4) == true);
    REQUIRE(myMap.contains(5) == true);
    REQUIRE(myMap.contains(6) == false);

    //Makes sure the getSize() Function works
    REQUIRE(myMap.getSize() == 5);

    myMap.remove(3);

    //Makes sure the remove() function works
    REQUIRE(myMap.getSize() == 4);
    REQUIRE(myMap.contains(3) == false);


    try {
        myMap.get(3);
    }

    catch (std::runtime_error &e) {
        cerr << e.what() << endl;
    }

    //If key that doesnt exists is tried to be removed, an error is thrown
    try {
        myMap.remove(7);
    }

    catch (std::runtime_error &e) {
        cerr << e.what() << endl;
    }

    REQUIRE(myMap.isEmpty() == false);


    //Tests to make sure the empty functions work
    myMap.makeEmpty();
    REQUIRE(myMap.isEmpty() == true);
    REQUIRE(myMap.getSize() == 0);

}


// you can run the test using Run CTest in the task bar or by running the tests executable. 