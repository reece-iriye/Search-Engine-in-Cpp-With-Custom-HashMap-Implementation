//
// Created by Alden Cressy on 11/28/22.
// Owner: Alden Cressy
//

#include "database.h"


////////
// Getters and Setters
///////
//
//const DSMap<string, list<string>> &Database::getWordDatabase() const {
//    return wordDatabase;
//}
//
//void Database::setWordDatabase(const DSMap<string, list<string>> &wordDatabase) {
//    Database::wordDatabase = wordDatabase;
//}
//
//const DSMap<string, list<string>> &Database::getPeopleDatabase() const {
//    return peopleDatabase;
//}
//
//void Database::setPeopleDatabase(const DSMap<string, list<string>> &peopleDatabase) {
//    Database::peopleDatabase = peopleDatabase;
//}
//
//const DSMap<string, list<string>> &Database::getOrganizationDatabase() const {
//    return organizationDatabase;
//}
//
//void Database::setOrganizationDatabase(const DSMap<string, list<string>> &organizationDatabase) {
//    Database::organizationDatabase = organizationDatabase;
//}
//
//
//const DSHashTable<string, unordered_set<string>> &Database::getWordDatabaseHash() const {
//    return wordDatabase_hash;
//}
//
//void Database::setWordDatabaseHash(const DSHashTable<string, unordered_set<string>>) &wordDatabaseHash) {
//    wordDatabase_hash = wordDatabaseHash;
//}
//
//const DSHashTable<string, unordered_set<string>> &Database::getPeopleDatabaseHash() const {
//    return peopleDatabase_hash;
//}
//
//void Database::setPeopleDatabaseHash(const DSHashTable<string, list<string>> &peopleDatabaseHash) {
//    peopleDatabase_hash = peopleDatabaseHash;
//}
//
//const DSHashTable<string, list<string>> &Database::getOrganizationDatabaseHash() const {
//    return organizationDatabase_hash;
//}
//
//void Database::setOrganizationDatabaseHash(const DSHashTable<string, list<string>> &organizationDatabaseHash) {
//    organizationDatabase_hash = organizationDatabaseHash;
//}
//
//const DSHashTable<string, string> &Database::getUuidTitleMapHash() const {
//    return uuidTitleMap_hash;
//}
//
//void Database::setUuidTitleMapHash(const DSHashTable<string, string> &uuidTitleMapHash) {
//    uuidTitleMap_hash = uuidTitleMapHash;
//}


////////
// Actual Methods
///////


void Database::findDocuments(list<string> text, list<string> negation, list<string> org, list<string> person) {

    // Master Set of All UUIDs
    vector<string> grandMasterSet;

    // Text Query Data Structures
    list<vector<string>> uuidsForEachWord;
    unordered_set<string> negationUUIDs;
    vector<string> masterSetOfTextUUIDs;

    // Org Query Data Structures
    list<vector<string>> uuidsForEachOrg;
    vector<string> masterSetOfOrgUUIDs;

    // Person Query Data Structures
    list<vector<string>> uuidsForEachPerson;
    vector<string> masterSetOfPersonUUIDs;

    // Create unordered_set for negations
    if (!negation.empty()) {
        // Obtain UUIDs where negation query exists and insert into unordered_set
        for (auto &word: negation) {
            if (wordDatabase_hash.contains(word)) {
                for (auto &uuid: wordDatabase_hash.get(word)) {
                    negationUUIDs.insert(uuid);
                }
            }
        }
    }

    // Iterate through each text search query
    for (auto & word : text) {
        vector<string> onlyTextUUIDs;
        if (wordDatabase_hash.contains(word)) {
            for (auto &uuid: wordDatabase_hash.get(word)) {
                if (negationUUIDs.find(uuid) == negationUUIDs.end()) {
                    onlyTextUUIDs.push_back(uuid);
                }
            }
        }
        // Sort the array so we can find intersection
        std::sort(onlyTextUUIDs.begin(), onlyTextUUIDs.end());
        uuidsForEachWord.push_front(onlyTextUUIDs);
    }

    // In the case that there's only 1 search query for text
    if (uuidsForEachWord.size() == 1) {
        for (auto & uuidsForThisWord : uuidsForEachWord)
            masterSetOfTextUUIDs = uuidsForThisWord;
    }
    // In the case that there's 2 search queries for text
    else if (uuidsForEachWord.size() >= 2) {
        for (auto & uuidsForThisWord : uuidsForEachWord) {
            if (masterSetOfTextUUIDs.empty())
                masterSetOfTextUUIDs = uuidsForThisWord;
            else {
                vector<string> bufferVector;
                std::set_intersection(masterSetOfTextUUIDs.begin(), masterSetOfTextUUIDs.end(),
                                      uuidsForThisWord.begin(), uuidsForThisWord.end(),
                                      std::back_inserter(bufferVector));
                masterSetOfTextUUIDs = bufferVector;
            }
        }
    }

    // Populate grandmaster set
    grandMasterSet = masterSetOfTextUUIDs;
    std::sort(grandMasterSet.begin(), grandMasterSet.end());


    // Now do the same for the organizations
    if (!org.empty()) {
        for (auto &word: org) {
            vector<string> onlyOrgUUIDs;
            if (organizationDatabase_hash.contains(word)) {
                for (auto &uuid: organizationDatabase_hash.get(word)) {
                    onlyOrgUUIDs.push_back(uuid);
                }
            }
            // Sort the array so we can find intersection
            std::sort(onlyOrgUUIDs.begin(), onlyOrgUUIDs.end());
            uuidsForEachOrg.push_front(onlyOrgUUIDs);
        }

        // In the case that there's only 1 search query for text
        if (uuidsForEachOrg.size() == 1) {
            for (auto & uuidsForThisOrg : uuidsForEachOrg)
                masterSetOfOrgUUIDs = uuidsForThisOrg;
        }
        // In the case that there's 2 search queries for text
        else if (uuidsForEachOrg.size() >= 2) {
            for (auto & uuidsForThisOrg : uuidsForEachOrg) {
                if (masterSetOfOrgUUIDs.empty())
                    masterSetOfOrgUUIDs = uuidsForThisOrg;
                else {
                    vector<string> bufferVector;
                    std::set_intersection(masterSetOfOrgUUIDs.begin(), masterSetOfOrgUUIDs.end(),
                                          uuidsForThisOrg.begin(), uuidsForThisOrg.end(),
                                          std::back_inserter(bufferVector));
                    masterSetOfOrgUUIDs = bufferVector;
                }
            }
        }

        // Intersect grandmaster set and org set if there exists queries for both
        if (!grandMasterSet.empty()) {
            vector<string> bufferVector;
            std::set_intersection(grandMasterSet.begin(), grandMasterSet.end(),
                                  masterSetOfOrgUUIDs.begin(), masterSetOfOrgUUIDs.end(),
                                  std::back_inserter(bufferVector));
            grandMasterSet = bufferVector;
        }
        // Populate grandmaster set with org set if we don't have a standalone text query
        else {
            grandMasterSet = masterSetOfOrgUUIDs;
        }

    }

    // Now do the same for the organizations
    if (!person.empty()) {
        for (auto &word: person) {
            vector<string> onlyPersonUUIDs;
            if (peopleDatabase_hash.contains(word)) {
                for (auto &uuid: peopleDatabase_hash.get(word)) {
                    onlyPersonUUIDs.push_back(uuid);
                }
            }
            // Sort the array so we can find intersection
            std::sort(onlyPersonUUIDs.begin(), onlyPersonUUIDs.end());
            uuidsForEachPerson.push_front(onlyPersonUUIDs);
        }

        // In the case that there's only 1 search query for text
        if (uuidsForEachPerson.size() == 1) {
            for (auto & uuidsForThisPerson : uuidsForEachPerson)
                masterSetOfPersonUUIDs = uuidsForThisPerson;
        }
            // In the case that there's 2 search queries for text
        else if (uuidsForEachPerson.size() >= 2) {
            for (auto & uuidsForThisPerson : uuidsForEachPerson) {
                if (masterSetOfPersonUUIDs.empty())
                    masterSetOfPersonUUIDs = uuidsForThisPerson;
                else {
                    vector<string> bufferVector;
                    std::set_intersection(masterSetOfPersonUUIDs.begin(), masterSetOfPersonUUIDs.end(),
                                          uuidsForThisPerson.begin(), uuidsForThisPerson.end(),
                                          std::back_inserter(bufferVector));
                    masterSetOfPersonUUIDs = bufferVector;
                }
            }
        }

        // Intersect grandmaster set and org set if there exists queries for both
        if (!grandMasterSet.empty()) {
            vector<string> bufferVector;
            std::set_intersection(grandMasterSet.begin(), grandMasterSet.end(),
                                  masterSetOfPersonUUIDs.begin(), masterSetOfPersonUUIDs.end(),
                                  std::back_inserter(bufferVector));
            grandMasterSet = bufferVector;
        }
            // Populate grandmaster set with org set if we don't have a standalone text query
        else {
            grandMasterSet = masterSetOfPersonUUIDs;
        }

    }


    cout << "    Documents Associated with ALL Our Queries (" << grandMasterSet.size() << " Documents):" << endl;
    if (!grandMasterSet.empty()) {
        for (auto &uuid: grandMasterSet) {
            cout << "        " << "TITLE: " << uuidTitleMap_hash.get(uuid) << endl
                 << "          " << "└──" << " UUID: " << uuid << endl;
        }
    }
    else {
        cout << "        " << "A document that intersects with every single one of our search queries does not exist." << endl;
    }

    // Only output this statement if multiple query types are included
    if (!(person.empty() && org.empty())) {
        // Print this statement out only if we have a text query
        if (!text.empty()) {
            cout << "    Documents Associated with Our Text Query (" << masterSetOfTextUUIDs.size() << " Documents):" << endl;

            if (!masterSetOfTextUUIDs.empty()) {
                for (auto &uuid: masterSetOfTextUUIDs)
                    cout << "        " << "TITLE: " << uuidTitleMap_hash.get(uuid) << endl
                         << "          " << "└──" << " UUID: " << uuid << endl;
            } else {
                cout << "        " << "A document that intersects with all of our text queries does not exist." << endl;
            }
        }
    }

    // Only output this statement if multiple query types are included
    if (!(text.empty() && person.empty())) {
        // Print this statement out only if we have an org query
        if (!org.empty()) {
            cout << "    Documents Associated with Our ORG Query (" << masterSetOfOrgUUIDs.size() << " Documents):" << endl;

            if (!masterSetOfOrgUUIDs.empty()) {
                for (auto &uuid: masterSetOfOrgUUIDs)
                    cout << "        " << "TITLE: " << uuidTitleMap_hash.get(uuid) << endl
                         << "          " << "└──" << " UUID: " << uuid << endl;
            } else {
                cout << "        " << "A document that intersects with all of our ORG queries does not exist." << endl;
            }
        }
    }

    // Only output this statement if multiple query types are included
    if (!(text.empty() && org.empty())) {
        // Print this statement out only if we have a text query
        if (!person.empty()) {
            cout << "    Documents Associated with Our PERSON Query (" << masterSetOfPersonUUIDs.size() << " Documents):" << endl;

            if (!masterSetOfPersonUUIDs.empty()) {
                for (auto &uuid: masterSetOfPersonUUIDs)
                    cout << "        " << "TITLE: " << uuidTitleMap_hash.get(uuid) << endl
                         << "          " << "└──" << " UUID: " << uuid << endl;
            } else {
                cout << "        " << "A document that intersects with all of our PERSON queries does not exist."
                     << endl;
            }
        }
    }

}


void Database::addEverything_hash(string filepath) {
    // Create a library of Document objects in a list data structure using DocumentParser

    // Parse Documents using a DocumentParser object
    DocumentParser dp;
    cout << "Parsing Documents:" << endl;
    auto start1 = chrono::high_resolution_clock::now();

    list<TheDocument> library = dp.iterate_JSON(filepath);
    cout << "Document Parsing Complete! ";

    auto stop1 = chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::seconds>(stop1 - start1);
    cout << "Document Parsing took " << duration1.count() << " seconds." << endl;


    // Create DSHashTables containing Document data and the associated UUID
    unordered_set<string> bufferList;

    cout << "Filling DSHashTables with data." << endl;
    auto start2 = chrono::high_resolution_clock::now();

    // Initialize DSHashTable sizes
    wordDatabase_hash.resize(16000183);
    peopleDatabase_hash.resize(4000037);
    organizationDatabase_hash.resize(4000037);
    uuidTitleMap_hash.resize(1000003);

    // Isolate each word in each document in a nested loop
    cout << "Filling word database:" << endl;
    int i = 0;
    for (auto &doc: library) {
        // Iterate through words
        for (auto &word: doc.getText()) {
            // If the word doesn't exist in the dictionary, add it to the map as a key and
            // set the value as a list<string> with its associated ID number
            if (!wordDatabase_hash.contains(word)) {
                bufferList.insert(doc.getUuid());
                wordDatabase_hash.insert(word, bufferList);
                bufferList.clear();
            }
            // If it's already in the dictionary, add the ID occurance if it hasn't seen the
            // word in the document yet.
            else {
                wordDatabase_hash.push_toList(word, doc.getUuid());
            }
        }

        // Iterate through people
        for (auto &word: doc.getPerson()) {

            // If the word doesn't exist in the dictionary, add it to the map as a key and
            // set the value as a list<string> with its associated ID number
            if (!peopleDatabase_hash.contains(word)) {
                bufferList.insert(doc.getUuid());
                peopleDatabase_hash.insert(word, bufferList);
                bufferList.clear();
            }
            // If it's already in the dictionary, add the ID occurance if it hasn't seen the
            // word in the document yet.
            else {
                peopleDatabase_hash.push_toList(word, doc.getUuid());
            }
        }

        // Iterate through organizations
        for (auto &word: doc.getOrganization()) {

            // If the word doesn't exist in the dictionary, add it to the map as a key and
            // set the value as a list<string> with its associated ID number
            if (!organizationDatabase_hash.contains(word)) {
                bufferList.insert(doc.getUuid());
                organizationDatabase_hash.insert(word, bufferList);
                bufferList.clear();
            }
            // If it's already in the dictionary, add the ID occurance if it hasn't seen the
            // word in the document yet.
            else {
                organizationDatabase_hash.push_toList(word, doc.getUuid());
            }

        }

        // Add title and UUID as well
        //-------------
        string theTitle = doc.getTitle();
        remove_commas(theTitle);
        uuidTitleMap_hash.insert(doc.getUuid(), theTitle);
        //-------------

        if (i % 3000 == 0)
            cout << "   " << "Documents Uploaded to DSHashTables (" << i << ")..." << endl;
        i++;

    }

    auto stop2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::seconds>(stop2 - start2);
    cout << "Loading DSHashTables took " << duration2.count() << " seconds." << endl;

}

void Database::remove_commas(string & s) {
    int j = 0;

    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ',') {
            continue;
        } else {
            s[j] = s[i];
            j++;
        }
    }

    s[j] = '\0';
}

void Database::create(string fileSuffix) {
    // Initialize DSHashTable sizes
    cout << "Creating DSHashTables:" << endl;
    cout << "   wordDatabase_hash creating..." << endl;
    wordDatabase_hash.resize(16000183);
    cout << "   peopleDatabase_hash creating..." << endl;
    peopleDatabase_hash.resize(4000037);
    cout << "   organizationsDatabase_hash creating..." << endl;
    organizationDatabase_hash.resize(4000037);
    cout << "   uuidTitleMap_hash creating..." << endl;
    uuidTitleMap_hash.resize(1000003);
    cout << "Finished creating DSHashTables!" << endl;

    cout << "Checking if persistency index exists:" << endl;
    auto start1 = chrono::high_resolution_clock::now();
    for (int i = 0; i < 4; ++i) {
        ifstream file;
        string theFilename;
        switch (i) {
            case 0:
                theFilename = fileSuffix + "_words.txt";
                file.open(theFilename);
                break;
            case 1:
                theFilename = fileSuffix + "_people.txt";
                file.open(theFilename);
                break;
            case 2:
                theFilename = fileSuffix + "_orgs.txt";
                file.open(theFilename);
                break;
            case 3:
                theFilename = fileSuffix + "_uuid.txt";
                file.open(theFilename);
            default:
                break;
        }

        // PERSISTENCY INDEX DOES NOT EXIST
        if (!file.is_open()) {
            cout << "Persistency index does not exist for " + theFilename << endl
                 << "Program will parse documents now and store them into created DSHashTables." << endl;

            // Create DSHashTables
            auto start1 = chrono::high_resolution_clock::now();
            addEverything_hash(fileSuffix);
            auto stop1 = chrono::high_resolution_clock::now();
            auto duration1 = chrono::duration_cast<chrono::seconds>(stop1 - start1);
            cout << "   " << "DSHashTables created from Document objects in " << duration1.count() << " seconds." << "\n";

            // Create persistency index from DSHashTables
            cout << "Writing information from DSHashTables to Persistency Index:" << endl;
            auto start2 = chrono::high_resolution_clock::now();
            write_fromHash(fileSuffix);
            auto stop2 = chrono::high_resolution_clock::now();
            auto duration2 = chrono::duration_cast<chrono::seconds>(stop2 - start2);
            cout << "   " << "Persistency index complete in " << duration2.count() << " seconds." << "\n";


            break;
        }

        cout << "   Persistency index exists! Filling DSHashTable from " << theFilename << "..." << endl;
        auto start1 = chrono::high_resolution_clock::now();


        // Buffer variables
        string temp;
        string temp1;

        // Extract informtion from the file
        while (!file.eof()) {
            getline(file, temp,',');
            unordered_set<string> tempList;
            while (getline(file, temp1,',')) {
                tempList.insert(temp1);
                if(file.peek() == '\n')
                    break;
            }

            // Eliminate the \n
            char buffer;
            file.get(buffer);

            // Populate the database of focus
            if (!temp.empty()) {
                // Create certain database depending on the iteration
                switch (i) {
                    case 0:
                        wordDatabase_hash.insert(temp, tempList);
                        break;
                    case 1:
                        peopleDatabase_hash.insert(temp, tempList);
                        break;
                    case 2:
                        organizationDatabase_hash.insert(temp, tempList);
                        break;
                    case 3:
                        string s;
                        for (auto & k : tempList) {
                            s = k;
                        }
                        uuidTitleMap_hash.insert(temp, s);
                        break;
                }
            }

        }

        // Close the file after every iteration
        file.close();


        auto stop1 = chrono::high_resolution_clock::now();
        auto duration1 = chrono::duration_cast<chrono::seconds>(stop1 - start1);
        cout << "          " << theFilename << " created in " << duration1.count() << " seconds." << "\n";

    }

}

void Database::write(string filetitle) {
    wordDatabase.download(filetitle, "_words.txt");
    peopleDatabase.download(filetitle, "_people.txt");
    organizationDatabase.download(filetitle, "_orgs.txt");
}


void Database::write_fromHash(string filetitle) {
    cout << "Writing " + filetitle + "_words.txt :" << endl;
    wordDatabase_hash.download(filetitle, "_words.txt");
    cout << "Writing " + filetitle + "_people.txt :" << endl;
    peopleDatabase_hash.download(filetitle, "_people.txt");
    cout << "Writing " + filetitle + "_orgs.txt :" << endl;
    organizationDatabase_hash.download(filetitle, "_orgs.txt");
    cout << "Writing " + filetitle + "_uuid.txt :" << endl;
    uuidTitleMap_hash.download2(filetitle, "_uuid.txt");

    cout << "File writing complete!" << endl;
}
















