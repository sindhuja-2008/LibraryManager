
#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    std::string id;
    std::string title;
    std::string author;
    std::string category;
    int year;
    bool available;

public:
    Book(const std::string& id,
         const std::string& title,
         const std::string& author,
         const std::string& category,
         int year,
         bool available = true);

    std::string getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getCategory() const;
    int getYear() const;
    bool isAvailable() const;

    void setTitle(const std::string& title);
    void setAuthor(const std::string& author);
    void setCategory(const std::string& category);
    void setYear(int year);
    void setAvailable(bool available);
};

#endif
