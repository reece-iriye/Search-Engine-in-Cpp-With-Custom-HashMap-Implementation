//
// Created by Reece Iriye on 11/9/22.
//

#include "Document.h"

using namespace std;


TheDocument::TheDocument(std::string UUID, std::string title, std::list<string> person, std::list<string> organization, list<string> text) {
    this->UUID = UUID;
    this->title = title;
    this->person = person;
    this->organization = organization;
    this->text = text;
}


// Getters and setters
const string &TheDocument::getUuid() const {
    return UUID;
}

void TheDocument::setUuid(const string &uuid) {
    UUID = uuid;
}

const string &TheDocument::getTitle() const {
    return title;
}

void TheDocument::setTitle(const string &title) {
    TheDocument::title = title;
}

const list<string> &TheDocument::getText() const {
    return text;
}

void TheDocument::setText(const list<string> &text) {
    TheDocument::text = text;
}

const list<string> &TheDocument::getPerson() const {
    return person;
}

void TheDocument::setPerson(const list<string> &person) {
    TheDocument::person = person;
}

const list<string> &TheDocument::getOrganization() const {
    return organization;
}

void TheDocument::setOrganization(const list<string> &organization) {
    TheDocument::organization = organization;
}





// Normal Functions

void TheDocument::addPerson(const string & s) {
    person.push_front(s);
}

void TheDocument::addOrganization(const string & s) {
    organization.push_front(s);
}
