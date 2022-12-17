//
// Created by Alden Cressy on 12/1/22.
//

#ifndef FINALPROJEXAMPLES_QUERYPROCESSOR_H
#define FINALPROJEXAMPLES_QUERYPROCESSOR_H

using namespace std;
#include <string>
#include <vector>
#include <list>
#include <unordered_set>
#include "database.h"


class QueryProcessor {
private:

public:
    QueryProcessor() = default;

    // Query manipulation
    void openGUI(Database &); // PROGRAM HOME BASE
    string Y_or_N();
    void search(string fullQuery, Database &);
    vector<string> parse_string(string fullQuery);

    // Natural Language Processing
    list<string> NLP(string text);
    list<string> NLP_people_and_orgs(string text);
    unordered_set<string> tokenize_toSet(string text, char dlm);


};


#endif //FINALPROJEXAMPLES_QUERYPROCESSOR_H
