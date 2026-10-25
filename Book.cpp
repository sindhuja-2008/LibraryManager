
#include "Book.h"

Book::Book(const std::string& id,
           const std::string& title,
           const std::string& author,
           const std::string& category,
           int year,
           bool available)
    : id(id),
      title(title),
      author(author),
      category(category),
      year(year),
      available(available) {
}

std::string Book::getId() const {
    return id;
}

std::string Book::getTitle() const {
    return title;
}

std::string Book::getAuthor() const {
    return author;
}

std::string Book::getCategory() const {
    return category;
}

int Book::getYear() const {
    return year;
}

bool Book::isAvailable() const {
    return available;
}

void Book::setTitle(const std::string& title) {
    this->title = title;
}

void Book::setAuthor(const std::string& author) {
    this->author = author;
}

void Book::setCategory(const std::string& category) {
    this->category = category;
}

void Book::setYear(int year) {
    if (year > 0) {
        this->year = year;
    }
}

void Book::setAvailable(bool available) {
    this->available = available;
}
