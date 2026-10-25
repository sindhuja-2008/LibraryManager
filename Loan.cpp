
#include "Loan.h"

Loan::Loan(const std::string& bookId,
           const std::string& memberId,
           const std::string& issueDate,
           const std::string& dueDate,
           bool returned)
    : bookId(bookId),
      memberId(memberId),
      issueDate(issueDate),
      dueDate(dueDate),
      returned(returned) {
}

std::string Loan::getBookId() const {
    return bookId;
}

std::string Loan::getMemberId() const {
    return memberId;
}

std::string Loan::getIssueDate() const {
    return issueDate;
}

std::string Loan::getDueDate() const {
    return dueDate;
}

bool Loan::isReturned() const {
    return returned;
}

void Loan::markReturned() {
    returned = true;
}
