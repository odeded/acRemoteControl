#pragma once

#include <string>

class MailItem
{
public:
    int         index;

    std::string subject;
    std::string body;

public:
    MailItem() : index(0)
        {}

    MailItem(int _index, std::string _subject, std::string _body) :
        index(_index), subject(_subject), body(_body)
        {}
    
};
