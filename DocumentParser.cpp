//
// Owner: Reece Iriye
//

#include "DocumentParser.h"

using namespace rapidjson;
using namespace std;


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// Actual Functions
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


/*
 * Reads in the JSON File
 */
TheDocument DocumentParser::read_JSON(string filename, unordered_set<string> & stopWords) {
    // File io
    ifstream input(filename);

    // use filesystem to iterate through every single file
    IStreamWrapper isw(input);

    // Create RapidJSON Document object to parse IStreamWrapper
    rapidjson::Document d;
    d.ParseStream(isw);

    // Create document object
    TheDocument doc;

    // Use RapidJSON Document to capture person, organization, UUID, and title
    /// Load all the people into the document
    auto tmp1 = d["entities"]["persons"].GetArray();
    for (auto & x : tmp1) {
        // Parse each name into parts of the name
        string thePerson = x["name"].GetString();
        list<string> parsedName = NLP_people_and_orgs(x["name"].GetString());

        // Add each individual part of the org name into the organization
        for (auto & part : parsedName) {
            doc.addPerson(part);
        }
    }

    /// Load all the organizations into the document
    auto tmp2 = d["entities"]["organizations"].GetArray();
    for (auto & x : tmp2) {
        // Parse each name into parts of the name
        string theOrg = x["name"].GetString();
        list<string> parsedName = NLP_people_and_orgs(x["name"].GetString());

        // Add each individual part of the org name into the organization
        for (auto & part : parsedName) {
            doc.addOrganization(part);
        }
    }

    /// Load the UUID and Title into the document
    doc.setUuid(d["uuid"].GetString());
    doc.setTitle(d["title"].GetString());

    /// Clean the text then load it into the document
    doc.setText(NLP(d["text"].GetString(), stopWords));

    // Close file
    input.close();


    return doc;

}


/*
 * Tokenize to a set.
 */
unordered_set<string> DocumentParser::tokenize_toSet(string text, char dlm) {
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



/*
 * Performs all necessary Natural Language Processing functions
 */
list<string> DocumentParser::NLP(string text, unordered_set<string> & stopWordsSet) {
    // Return type
    list<string> storedWords;

    // Iterate through the entire text to fill our stored words
    string bufferStr = "";
    unordered_map<string, string> stemChecker;
    for (int i = 0; i < text.size(); ++i) {
        // Make capital letters lowercase
        if (text[i] >= 'A' && text[i] <= 'Z') {
            // ASCII number for 'a' is 97 and for 'A' is 65
            // Therefore, adding 32 to any lowercase letter will make an upper-case letter lower-case
            text[i] = text[i] + 32;
        }

        // Ignore characters outside the alphabetical letters, the delimiter, and comma
        if (text[i] >= 'a' && text[i] <= 'z' || text[i] == '\n' || text[i] == ' ') {
            // In the case of a space or \n, start evaluating bufferStr
            if (text[i] == ' ' || text[i] == '\n') {
                if (stopWordsSet.find(bufferStr) == stopWordsSet.end() && bufferStr != "") {
                    // If the word has been seen and stemmed before
                    if (stemChecker.find(bufferStr) == stemChecker.end()) {
                        string temp = bufferStr;
                        Porter2Stemmer::stem(bufferStr);
                        stemChecker.insert({temp, bufferStr});
                        storedWords.push_front(bufferStr);
                    }
                    else {
                        storedWords.push_front(stemChecker.at(bufferStr));
                    }
                }

                bufferStr.clear();
            }
            // Append our buffer string within this else statement
            else {
                bufferStr.push_back(text[i]);
            }
        }
    }


    return storedWords;
}

list<string> DocumentParser::NLP_people_and_orgs(string text) {
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
        if ((text[i] >= 'a' && text[i] <= 'z')) {
            if (i<1)
                newText = newText + text[i];
            else if (text[i-1] != ' ' || text[i] != ' ')
                newText = newText + text[i];
        }
        else if (text[i] == ' ' || i == text.size()-1) {
            if (i == text.size()-1)
                newText = newText + text[i];
            listOfItems.push_front(newText);
            newText.clear();
        }

    }

    return listOfItems;

}


/*
 * Iterates through all JSON files in a folder and puts all
 * their information into a Document class
 */
list<TheDocument> DocumentParser::iterate_JSON(const string & folderName) {
    // recursive_director_iterator used to "access" folder at parameter -path-
    // we are using the recursive iterator so it will go into subfolders.
    // see: https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
    auto it = filesystem::recursive_directory_iterator(folderName);

    // Create Document library
    list<TheDocument> library;

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

    int i = 0;
    // Loop over every single JSON file
    for (const auto &entry : it) {
        // We only want to attempt to parse files that end with .json
        if (entry.is_regular_file() && entry.path().extension().string() == ".json")
            library.push_front(read_JSON(entry.path().string(), stopWordsSet));

        if (i % 500 == 0)
            cout << "   " << "Document Number (" << i << ")..." << endl;

        i++;
    }


    return library;
}






