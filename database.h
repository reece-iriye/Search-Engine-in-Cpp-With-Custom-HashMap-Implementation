//
// Created by Alden Cressy on 11/28/22.
// Owner: Alden Cressy
//

#ifndef FINALPROJEXAMPLES_DATABASE_H
#define FINALPROJEXAMPLES_DATABASE_H

#include <functional>
//#include <unordered_set>

#include "DSHashTable.h"
#include "DocumentParser.h"
#include "DSMap.h"


class Database {
private:
    DSMap<string, list<string>> wordDatabase;
    DSMap<string, list<string>> peopleDatabase;
    DSMap<string, list<string>> organizationDatabase;

    DSHashTable<string, unordered_set<string>> wordDatabase_hash;
    DSHashTable<string, unordered_set<string>> peopleDatabase_hash;
    DSHashTable<string, unordered_set<string>> organizationDatabase_hash;

    DSHashTable<string, string> uuidTitleMap_hash;

    /*
     * TO DO:
     *  Use DSHashTable to store individual words, people, and organizations,
     *  and map them to the documents in which they belong. Also, specifically
     *  for text, ensure that we use std::pair to indicate the number of appearances
     *  for each word
     */

public:

    void remove_commas(string &);

    void addEverything(string filepath);
    void addEverything_hash(string filepath);

    void create(string fileSuffix);
    void write(string filetitle);
    void write_fromHash(string filetitle);
    void findDocuments(list<string>, list<string>, list<string>, list<string>);


};

#endif //FINALPROJEXAMPLES_DATABASE_H
