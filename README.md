# **Creating a Search Engine Using a Custom HashTable Class**
## **Summary**
This project is authored by both Reece Iriye and Alden Cressy for CS 2341: Data Structures taught by Dr. Michael Hahsler at SMU. This program was a journey, as it required for us to work around one another to ensure that we could complete the project in time. We managed the difficulty of not only completing the project, but optimizing runtime and ensuring that the program could run as efficiently as possible with the 300,000 file `.json` financial document folder we were given from Kaggle. This documentation marks the description of our completed project, and it outlines how our project works and why picked certain design choices.


*NOTE:* Our team was ranked #1 for the fastest search time for 300,000 documents out of ~100 competing students in the class. 

## **UML Diagram**
### **Core Program**
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/UML_SearchEngine.png)
### **Custom Data Structures (Hash Map, AVL Tree, and AVLTree-Based Map)**
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/CustomDataStructures.png)
## **Pseudocode for Each Component of the Program**
### **QueryProcessor.cpp**
#### **openGUI()**
This method is what we’d like to call the home base of the program. For all searching processes, until the user indicates that they’d like to exit the supersearch program, the program will loop indefinitely through the `search()` method continuing to ask a user what query they’d like to type into the program and find within our financial document database.
#### **parse_string()**
This method parses the full query sent in by the user into four separate search queries. The four different queries are words, people, organizations, and negations. Based off whether the `PERSON:`, `ORG:`, or `-` operators are used, the substring is placed into a vector of size 4. The index of the vector the substring is placed in determines which type of query it is.
#### **search()**
Once the full search query is parsed into four separate search queries, the search method is called. In this method, all the search queries are parsed and stemmed, just like the words of the documents are parsed and stemmed in document parser. Then, the `finddocuments()` method is called with the four search queries as parameters. We iterate through this method continuously until the user indicates that they’d like to stop running the program.
### **database.cpp**
#### **create()**
This method decides between one of two ways to create the word, people, and organization indexes. If text files that indicate the persistency index already exist, the DSHashTables will be created from the pre-existing text files. This process involves reading from the text files and inserting the word and a list of UUIDs as a key-value pair into DSHashTable. If the text files do not exist, the `addEverything_hash()` method is called to create the hash tables from TheDocument objects.
#### **addEverything_hash()**
This method  populates the three hash maps based off of TheDocument objects obtained from parsing the documents. First, the document parser is called, which returns a list of all the documents. Each document has a list of words, people, and organizations referenced in that document along with a title and UUID. Once this is completed, a double for loop starts. The first for loop loops through the list of document objects. The inner loop loops through the lists of words, people, and organizations in the document object. These terms are then inserted into a hash table with the word being the key, and the UUID being the value. If a word comes up that is already in the hash table, the UUID of the document will be pushed_back to the list of UUIDs associated with that word. A fourth hash table is also created which pairs the document UUID as the key, and title as the value.
#### **write_fromHash()**
Once the four DSHashTables are populated, this write method is called. What this method does is write the four tables to text files for the Persistency Index. This method iterates through the entire hash table and prints to the file the key (word) and the value (list of UUIDs). Each UUID is separated by a comma with no space. This is because the `create()` method uses a comma as a delimiter between each UUID.
#### **findDocuments()**
This method finds the intersections of documents that contain all of the search queries indicated by the user. This action is done by first getting a vector of document UUIDs that are in each of the search queries. Then, we use `std::set_intersection()` to find the intersections of all four of these lists. Then, the master list containing the intersections of all is printed out for the user. 
### **DocumentParser.cpp**
#### **iterate_JSON()**
This method takes advantage of the `recursive_director_iterator` that we obtained from using `#include <filesystem>`. It loops through `read_JSON()` for every single `.json` document in the indicated file path given by the user, then it pushes a TheDocument object with private variables filled with all the information we need to a `std::list`. We use this `list<TheDocument>` type later immediately then to store our DSHashTables with the information in a more organized fashion.
#### **read_JSON()**
This method takes in the filename of a .json file and returns a TheDocument object with the UUID, Title, Text, Organizations, and People indicated in the `.json` file. For storing the People and Organizations, we pass the information from the .json file through the `NLP_people_and_orgs()` method, which essentially makes all the text lowercase, ignores characters outside the alphabet, and tokenizes each part to an individual word. For storing Text, we use a similar function `NLP()`, except this function removes stop words and stems each word individually, on top of maintaining all the functionality of `NLP_people_and_orgs()`. We made the Natural Language Processing methods different for the separate categories, because people and organizations don’t need to be stemmed or removed at all, meanwhile standard words that appear in text do.
## **Document.cpp**
This file contains code for `TheDocument` objects, which essentially store all pertinent information from each `.json` document necessary for the search engine to operate.
## **Planned Timeline**
| Task                  | Planned completion | Actual completion.     | Time spent in hrs by Alden Cressy | Time spent in hrs by Reece Iriye |
| --------------------- | :-----------------:| :--------------------: | :-------------------------------: | :------------------------------: |
| UML Diagram           | Nov 5              | Nov 5                  | 3 hours                           | 3 hours                          |
| DSAVLTree.h           | Nov 14             | Nov 13                 | 7 hours                           | 3 hours                          |
| DSMap.h               | Nov 14             | Nov 14                 | 4 hours                           | 2 hours                          |
| DSHashTable.h         | Nov 14             | Nov 13                 | 7 hours                           | 3 hours                          |
| DocumentParser.cpp    | Nov 18             | Nov 28 (Thanksgiving)  | 4 hours                           | 17 hours                         |
| database.cpp          | Nov 29             | Dec 1                  | 36 hours                          | 36 hours                         |
| QueryProcessor.cpp    | Dec 2              | Dec 4                  | 12 hours                          | 12 hours                         |
| Documentation.        | Dec 4              | Dec 4                  | 5 hours                           | 5 hours                          |
 
## **Used Data Structures**
### **std::list**
Used in: `Document Parser`, `Database`, and `DSHashTable`. We used lists to store the words, organizations, and people in each document. The main reason we chose a list for this is because of the $O(1)$ insertion. We considered using vector for this, but we ended up choosing a list because we knew the list would be getting pretty big. If we used a vector, the `resize()` method would eventually have to be called which takes $O(n)$ steps. It is also very easy to iterate through an entire list, which is what we needed to do for indexing. List is also used in DSHashtable for chaining when collisions occur. 
### **std::vector**
Used in: `Database` `findDocuments()`. We use a vector to store the UUIDs for the documents that contain the queries sent in by the user. The reason we chose vector instead of list is because >std::set_intersection()< requires the data to be put in sorted vectors. We need to use std::sort to sort the vectors, which is $O(n\log(n))$.  Iterating through a vector is very easy which $O(n)$ time complexity. Inserting to a vector takes $O(1)$ time. 
### **std::unordered_set**
Used in: `DocumentParser` to remove stop words. We chose to use an unordered set here because it takes $O(1)$ time complexity to check if a word is a stop word. Insertion also takes $O(1)$ time complexity. We also use an unordered set to store all the UUIDs associated with a word. This was useful when checking if the set already contained that UUID because of the $O(1)$ search time. 
### **DSHashTable (custom made data structure)**
Used in: `Database` for indexing. We chose to use a Hash table because of the $O(1)$ insertion and lookup time. We originally were using an AVL tree, but the $O(\log(n))$ insertion took way too long when we were running the massive dataset. Instead, we made the decision to customize our bin numbers to minimize collisions with the search engine implementation and ensure that the $O(1)$ lookup time could be as accurate as possible.
## **How the Program Works**
First, compile and run the program. It is necessary that this program is ran with the `supersearch` executable indicated, then followed by a command-line argument indicated the filepath of the `.json` documents or the beginning of the `.txt` documents if a persistency index has already been created. Once the files are indexed, our user interface will pop up. Here are the rules for searching in our search engine. First, you will be allowed to type in any individual words that you'd like to search through within these financial documents. These words will search through the text within each document and find the documents that contain the words of interest. For example, typing in “markets” will cause the search engine to find all documents with the word “markets” in it. Also, you can search multiple terms. Typing “social network” into the query should lead to the search engine finding documents with both “social” and “network” inside of them. The search can be tailored much more specifically, putting PERSON: behind a search or ORG: to have a more specific search allowing the search engine to hone in on people and organizations labeled within each JSON document as well. You may also omit words from your search using a "-" sign before any text. This will cause documents with those words in them to not appear in the search.
## **Example Queries**
We tested out some example queries from a simple 2000 `.json`-file document. Here are our results.
### **Example Query 1**
• bitcoin ethereum korea  

After receiving this full query, the program will output the documents intersecting with the three of these individual text queries. 
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Query_Examples/bitcoin_ethereum_korea.png)
### **Example Query 2**
• bitcoin ORG:wsj  

This query will cause the program to out the 4 most relevant files, which contain bitcoin in the text and WSJ as an organization. Then, the program will output the files that contain only bitcoin and only WSJ.
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Query_Examples/bitcoin_wsj1.png)
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Query_Examples/bitcoin_wsj2.png)
### **Example Query 3**
• network PERSON:trump  

This query has no intersecting document within the 2000 documents that contains network as a text query and Trump as a person query. So, we decided it would be best to let the user know about that but also inform them about what documents would show up for the Text query individually and the Person query individually.
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Query_Examples/network_trump1.png)
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Query_Examples/network_trump2.png)
### **Example Query 4**
• fake -news

This query searches for the documents that contain the word "fake" but ensure that the documents fetched do NOT contain the word "news". 
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Query_Examples/fake_news.png)
## **Performance Statistics**
The following statistics are testing against a 300,000 `.json` file folder of financial documents.
### **Document Parsing**
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Section_Runtimes/DocumentParsing_Time.png)
Runtime: 884 seconds, or 14 minutes & 43 seconds
### **Shifting Data from TheDocument Objects to DSHashTable Data Structures**
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Section_Runtimes/Upload_to_DSHashTable.png)
Runtime: 119 seconds, or 1 minute & 59 seconds
### **Creating Persistency Index from DSHashTables**
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Section_Runtimes/PersistencyIndex_Time.png)
Runtime: 18 seconds
### **Filling DSHashTables when Persistency Index Already Exists**
![alt text](https://github.com/SMUCS2341/assignment-4-search-engine-alden-and-reece/blob/main/Section_Runtimes/Fill_Index_if_exists.png)
Runtime: 109 seconds, or 1 minute & 49 seconds
## **Automatic Tests and Memory Tests for DSMap**
### **DSMap**
We tested insertion, the `get()` method after insertion, accessing indices using a Key that has not yet been inserted yet, the `contains()` function, the `size()` function, the `remove()` function and the resulting size afterwards, exception handling using the `std::what()` method, and the `isEmpty()` method. Based on our results, the DSMap() seemed to have been working perfectly, so we moved forward with confidence until ultimately scrapping the DSAVLTree idea later. 
## **Conclusion**
After this program, we learned to work together as a team to map out a month-long project, creating arbitrary deadlines for one another throughout the process. If we were to have managed our time slightly better and/or had more time for this project, we would have done a much more extensive relevancy ranking process including UUID counts instead of disregarding them entirely, so that the search engine program could prioritize terms that appear more often.
