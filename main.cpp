#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>

#include "database.h"
#include "QueryProcessor.h"

using namespace std;


int main(int args, char** argv) {

    auto start1 = chrono::high_resolution_clock::now();

    Database db;
    string filepath = argv[1];
    db.create(filepath);
    QueryProcessor qp;
    qp.openGUI(db);

    auto stop1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::seconds>(stop1 - start1);
    cout << "supersearch has been terminated after " << duration1.count() << " seconds." << "\n";



    return 0;
}
