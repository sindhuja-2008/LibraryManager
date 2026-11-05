
#ifndef LIBRARY_H
#define LIBRARY_H

#include "Book.h"
#include "Member.h"
#include "Loan.h"

#include <string>
#include <vector>

class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;
    std::vector<Loan> loans;

    const std::string booksFile = "books.csv";
    const std::string membersFile = "members.csv";
    const std::string loansFile = "loans.csv";

    bool saveBooks() const;
    bool saveMembers() const;
    bool saveLoans() const;

public:
    bool loadData();

    bool addBook(const Book& book);
    bool addMember(const Member& member);

    void listBooks() const;
    void listMembers() const;

    void searchBook(const std::string& keyword) const;
    void binarySearchBook(const std::string& id) const;
    void sortBooksByTitle();
    void sortBooksById();
    bool updateBook(const std::string& id,
                    const std::string& title,
                    const std::string& author,
                    const std::string& category,
                    int year);

    bool issueBook(const std::string& bookId,
                   const std::string& memberId,
                   const std::string& issueDate,
                   const std::string& dueDate);

    bool returnBook(const std::string& bookId,
                    const std::string& memberId);

    void listAvailableBooks() const;
};

#endif
