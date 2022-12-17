//
// Created by Reece Iriye on 11/6/22.
//

#ifndef FINALPROJEXAMPLES_DOCUMENTPARSER_H
#define FINALPROJEXAMPLES_DOCUMENTPARSER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <list>
#include <cstring>

#include "Document.h"
#include "porter2_stemmer.h"

// Document parsers needed for our JSON files
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/document.h"


class DocumentParser {
public:
    DocumentParser() = default;
    ~DocumentParser() = default;
    DocumentParser &operator=(const DocumentParser & rhs);

    TheDocument read_JSON(std::string filename, std::unordered_set<string> & stopWords);
    unordered_set<std::string> tokenize_toSet(std::string text, char dlm);
    std::list<std::string> NLP(std::string text, std::unordered_set<string> & stopWordsSet);
    std::list<std::string> NLP_people_and_orgs(std::string text);
    std::list<TheDocument> iterate_JSON(const std::string & folderName);


};


#endif //FINALPROJEXAMPLES_DOCUMENTPARSER_H
