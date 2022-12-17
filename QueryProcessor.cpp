//
// Created by Alden Cressy on 12/1/22.
//

#include "QueryProcessor.h"


void QueryProcessor::openGUI(Database & db) {
    cout << "Hello, here is how this search engine works." << endl;
    cout << "   First, you will be allowed to type in any individual words that you'd like to search through within these financial documents. "
         << "These words will search through the text within each document and find the documents that contain the words of interest." << endl
         << "      For example, typing in \"markets\" will cause the search engine to find all documents with the word \"markets\" in it. " << endl
         << "   Also, you can search multiple terms. Typing \"social network\" into the query should lead to the search engine finding documents with "
         << "both \"social\" and \"network\" inside of them." << endl
         << "   The search can be tailored much more specifically, putting PERSON: behind a search or ORG: to have a more specific search allowing the "
         << "search engine to hone in on people and organizations labeled within each JSON document as well." << endl
         << "   You may also omit words from your search using a \"-\" sign before any text. This will cause documents with those words in them to not"
         << " appear in the search." << endl << endl
         << "Have fun!" << endl << endl;

    while(true) {
        cout << "Please type in a search query: ";
        string searchQuery;
        getline(cin, searchQuery);

        search(searchQuery, db);

        string yesOrNo;
        do {
            yesOrNo = Y_or_N();
        } while(yesOrNo != "y" && yesOrNo != "Y" && yesOrNo != "n" && yesOrNo != "N");

        if (yesOrNo == "n" || yesOrNo == "N") {
            cout << "Search complete. Have a great day!" << "\n\n";
            break;
        }
    }
}

string QueryProcessor::Y_or_N() {
    cout << "Would you like to search again? (y/n): ";
    string yesOrNo = "";
    getline(cin, yesOrNo);
    return yesOrNo;
}

void QueryProcessor::search(string fullQuery, Database &db) {
    // Time the searching
    auto start1 = chrono::high_resolution_clock::now();

    // Separate queries into necessary subsections
    vector<string> queries = parse_string(fullQuery);

    // Perform NLP on text queries and store individual words into a list
    list<string> textQueries = NLP(queries[0]);
    list<string> negationQueries = NLP(queries[3]);

    // Perform NLP on people and organizations and store individual words into a list
    list<string> orgQuery = NLP_people_and_orgs(queries[1]);
    list<string> personQuery = NLP_people_and_orgs(queries[2]);

    // Find and output the document titles and ID's that pop up in our search
    db.findDocuments(textQueries, negationQueries, orgQuery, personQuery);

    // End Timer and Output time
    auto stop1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::seconds>(stop1 - start1);
    cout << "Search CPU Time: " << duration1.count() << " seconds" << "\n";

}


vector<string> QueryProcessor::parse_string(string fullQuery) {
    vector<string> queries = {"", "", "", ""};

    int mode = 0;
    for (int i = 0; i < fullQuery.size(); ++i) {

        switch(mode) {
            // TEXT CASE
            case 0:
                // Append the text query
                queries[0] = queries[0] + fullQuery[i];

                // Check other cases
                if (i>=3 && fullQuery[i]==':') {
                    if (fullQuery[i-3] == 'O' && fullQuery[i-2] == 'R' && fullQuery[i-1] == 'G') {
                        mode = 1;
                        for (int j = 0; j < 5; ++j) {
                            queries[0].pop_back();
                        }
                    }
                }

                if (i>=6 && fullQuery[i]==':') {
                    if (fullQuery[i-6] == 'P' && fullQuery[i-5] == 'E' && fullQuery[i-4] == 'R'
                    &&  fullQuery[i-3] == 'S' && fullQuery[i-2] == 'O' && fullQuery[i-1] == 'N') {
                        mode = 2;
                        for (int j = 0; j < 8; ++j) {
                            queries[0].pop_back();
                        }
                    }
                }

                else if (fullQuery[i] == '-') {
                    mode = 3;
                    queries[0].pop_back();
                    queries[0].pop_back();
                }


                break;
            // ORGANIZATION CASE
            case 1:
                // Append organization query
                queries[1] = queries[1] + fullQuery[i];

                // Check for person case
                if (i>=6 && fullQuery[i]==':') {
                    if (fullQuery[i-6] == 'P' && fullQuery[i-5] == 'E' && fullQuery[i-4] == 'R'
                        &&  fullQuery[i-3] == 'S' && fullQuery[i-2] == 'O' && fullQuery[i-1] == 'N') {
                        mode = 2;
                        for (int j = 0; j < 8; ++j) {
                            queries[1].pop_back();
                        }
                    }
                }
                else if (fullQuery[i] == '-') {
                    mode = 3;
                    queries[1].pop_back();
                    queries[1].pop_back();
                }

                break;
            // PERSON CASE
            case 2:
                // Append person query
                queries[2] = queries[2] + fullQuery[i];

                // Search other cases
                if (i>=3 && fullQuery[i]==':') {
                    if (fullQuery[i-3] == 'O' && fullQuery[i-2] == 'R' && fullQuery[i-1] == 'G') {
                        mode = 1;
                        for (int j = 0; j < 5; ++j) {
                            queries[2].pop_back();
                        }
                    }
                }
                else if (fullQuery[i] == '-') {
                    mode = 3;
                    queries[2].pop_back();
                    queries[2].pop_back();
                }

                break;
            case 3:
                // Append exclusion query
                queries[3] = queries[3] + fullQuery[i];

                // Check other cases
                if (i>=3 && fullQuery[i]==':') {
                    if (fullQuery[i-3] == 'O' && fullQuery[i-2] == 'R' && fullQuery[i-1] == 'G') {
                        mode = 1;
                        for (int j = 0; j < 5; ++j) {
                            queries[3].pop_back();
                        }
                    }
                }

                if (i>=6 && fullQuery[i]==':') {
                    if (fullQuery[i-6] == 'P' && fullQuery[i-5] == 'E' && fullQuery[i-4] == 'R'
                        &&  fullQuery[i-3] == 'S' && fullQuery[i-2] == 'O' && fullQuery[i-1] == 'N') {
                        mode = 2;
                        for (int j = 0; j < 8; ++j) {
                            queries[3].pop_back();
                        }
                    }
                }

                // Search other cases
                break;
        }
    }

    return queries;
}


/*
 * Performs all necessary Natural Language Processing functions
 */
list<string> QueryProcessor::NLP(string text) {
    // Return type
    list<string> storedWords;

    // Read in stop-words file
    ifstream stopwordReader("stopwords.csv");
    if (!stopwordReader.is_open()) {
        throw runtime_error("Stop words file is not open!");
    }

    // Read through the file and take the stop words
    stringstream buffer;
    buffer << stopwordReader.rdbuf(); // input file data into buffer
    unordered_set<string> stopWordsSet = tokenize_toSet(buffer.str(), ',');
    stopwordReader.close();

    // Iterate through the entire text to fill our stored words
    string bufferStr = "";
    unordered_map<string, string> stemChecker;
    size_t n = text.size();
    for (int i = 0; i < n; ++i) {
        // Make capital letters lowercase
        if (text[i] >= 'A' && text[i] <= 'Z') {
            // ASCII number for 'a' is 97 and for 'A' is 65
            // Therefore, adding 32 to any lowercase letter will make an upper-case letter lower-case
            text[i] = text[i] + 32;
        }

        // Ignore characters outside the alphabetical letters, the delimiter, and comma
        if (text[i] >= 'a' && text[i] <= 'z' || text[i] == '\n' || text[i] == ' ') {
            // In the case of a space or \n, start evaluating bufferStr
            if (text[i] == ' ' || text[i] == '\n' || i == n-1) {
                if (i == n-1)
                    bufferStr = bufferStr + text[i];
                if (stopWordsSet.find(bufferStr) == stopWordsSet.end() && bufferStr != "") {
                    string temp = bufferStr;
                    // If the word has been seen and stemmed before
                    if (stemChecker.find(temp) == stemChecker.end()) {
                        Porter2Stemmer::stem(bufferStr);
                        stemChecker.insert({temp, bufferStr});
                        storedWords.push_front(bufferStr);
                    }
                    else {
                        storedWords.push_front(stemChecker.at(temp));
                    }
                }

                bufferStr.clear();
            }
                // Append our buffer string within this else statement
            else {
                bufferStr = bufferStr + text[i];
            }
        }
    }


    return storedWords;
}

//string QueryProcessor::NLP_people_and_orgs(string text) {
//    string newText = "";
//    for (int i = 0; i < text.size(); ++i) {
//
//        if (text[i] >= 'A' && text[i] <= 'Z') {
//            // ASCII number for 'a' is 97 and for 'A' is 65
//            // Therefore, adding 32 to any lowercase letter will make an upper-case letter lower-case
//            text[i] = text[i] + 32;
//        }
//
//        if (text[i] == '\n')
//            text[i] = ' ';
//
//        // Ignore characters outside the alphabetical letters, the delimiter, and comma
//        if ((text[i] >= 'a' && text[i] <= 'z' || text[i] == ' ')) {
//            if (i<1)
//                newText = newText + text[i];
//            else if (text[i-1] != ' ' || text[i] != ' ')
//                newText = newText + text[i];
//        }
//
//    }
//
//    return newText;
//
//}

list<string> QueryProcessor::NLP_people_and_orgs(string text) {
    list<string> listOfItems;
    string newText = "";
    for (int i = 0; i < text.size(); ++i) {

        if (text[i] >= 'A' && text[i] <= 'Z') {
            // ASCII number for 'a' is 97 and for 'A' is 65
            // Therefore, adding 32 to any lowercase letter will make an upper-case letter lower-case
            text[i] = text[i] + 32;
        }

        if (text[i] == '\n')
            text[i] = ' ';

        // Ignore characters outside the alphabetical letters, the delimiter, and comma
        if (text[i] >= 'a' && text[i] <= 'z') {
//            if (i<1)
//                newText = newText + text[i];
//            else if (text[i-1] != ' ' || text[i] != ' ')
            newText = newText + text[i];
        }
        if (text[i] == ' ' || i == text.size()-1) {
//            if (i == text.size()-1)
//                newText = newText + text[i];
            listOfItems.push_front(newText);
            newText.clear();
        }

    }

    return listOfItems;

}


/*
 * Tokenize to a set.
 */
unordered_set<string> QueryProcessor::tokenize_toSet(string text, char dlm) {
    unordered_set<string> tmpSet;
    string tmpString;

    for (size_t i = 0; i < text.size(); ++i) {
        if (text[i] != dlm) {
            tmpString = tmpString + text[i];
        }
        else {
            tmpSet.insert(tmpString);
            tmpString.clear();
        }
    }


    return tmpSet;

}



