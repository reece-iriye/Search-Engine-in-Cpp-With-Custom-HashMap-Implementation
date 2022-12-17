//
// Created by Reece Iriye on 11/9/22.
//

#ifndef FINALPROJEXAMPLES_DOCUMENT_H
#define FINALPROJEXAMPLES_DOCUMENT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>

#include "DSAVLTree.h"


class TheDocument {
private:
    // Create string indicating the UUID
    std::string UUID;

    // File contents
    std::string title;
    std::list<string> person;
    std::list<string> organization;
    std::list<std::string> text;

public:
    // Rule of 3 and constructor
    TheDocument() = default;
    ~TheDocument() = default;
    TheDocument &operator=(const TheDocument & rhs);
    TheDocument(std::string UUID, std::string title, std::list<string> person, std::list<string> organization, std::list<std::string> text);


    // Getters and Setters
    const string &getUuid() const;
    void setUuid(const string &uuid);
    const string &getTitle() const;
    void setTitle(const string &title);
    const list<std::string> &getText() const;
    void setText(const list<std::string> &text);
    const list<string> &getPerson() const;
    void setPerson(const list<string> &person);
    const list<string> &getOrganization() const;
    void setOrganization(const list<string> &organization);

    // Normal Functions
    void addPerson(const std::string &);
    void addOrganization(const std::string &);
};


#endif //FINALPROJEXAMPLES_DOCUMENT_H
