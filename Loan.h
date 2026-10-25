
#ifndef LOAN_H
#define LOAN_H

#include <string>

class Loan {
private:
    std::string bookId;
    std::string memberId;
    std::string issueDate;
    std::string dueDate;
    bool returned;

public:
    Loan(const std::string& bookId,
         const std::string& memberId,
         const std::string& issueDate,
         const std::string& dueDate,
         bool returned = false);

    std::string getBookId() const;
    std::string getMemberId() const;
    std::string getIssueDate() const;
    std::string getDueDate() const;
    bool isReturned() const;

    void markReturned();
};

#endif
