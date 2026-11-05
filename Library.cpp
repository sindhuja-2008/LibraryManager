#include "Library.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace std;

namespace {

    string csvQuote(const string& value) {
        string result = "\"";

        for (char ch : value) {
            if (ch == '"') {
                result += "\"\"";
            } else {
                result += ch;
            }
        }

        result += '"';
        return result;
    }

    vector<string> parseCSV(const string& line) {
        vector<string> fields;
        string field;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            char ch = line[i];

            if (ch == '"') {
                if (inQuotes && i + 1 < line.size()
                    && line[i + 1] == '"') {
                    field += '"';
                    ++i;
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (ch == ',' && !inQuotes) {
                fields.push_back(field);
                field.clear();
            } else {
                field += ch;
            }
        }

        if (inQuotes) {
            return {};
        }

        fields.push_back(field);
        return fields;
    }

    string lowerText(string value) {
        transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char ch) {
                       return static_cast<char>(tolower(ch));
                   });

        return value;
    }

    bool validDate(const string& date) {
        if (date.size() != 10 ||
            date[4] != '-' || date[7] != '-') {
            return false;
        }

        for (size_t i = 0; i < date.size(); ++i) {
            if (i != 4 && i != 7 &&
                !isdigit(static_cast<unsigned char>(date[i]))) {
                return false;
            }
        }

        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        if (year < 1 || month < 1 || month > 12) {
            return false;
        }

        const int daysInMonth[] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };

        int maxDay = daysInMonth[month - 1];

        bool leap = (year % 400 == 0) ||
                    (year % 4 == 0 && year % 100 != 0);

        if (month == 2 && leap) {
            maxDay = 29;
        }

        return day >= 1 && day <= maxDay;
    }

    bool validYear(int year) {
        return year >= 1 && year <= 9999;
    }

}

// SAVE BOOKS
bool Library::saveBooks() const {
    ofstream file(booksFile, ios::trunc);

    if (!file) {
        cerr << "Error: Could not save books.\n";
        return false;
    }

    file << "book_id,title,author,category,year,status,borrower_id,due_date\n";

    for (const Book& book : books) {
        string borrower;
        string dueDate;

        for (const Loan& loan : loans) {
            if (loan.getBookId() == book.getId() &&
                !loan.isReturned()) {
                borrower = loan.getMemberId();
                dueDate = loan.getDueDate();
                break;
            }
        }

        file << csvQuote(book.getId()) << ','
             << csvQuote(book.getTitle()) << ','
             << csvQuote(book.getAuthor()) << ','
             << csvQuote(book.getCategory()) << ','
             << book.getYear() << ','
             << (book.isAvailable() ? "available" : "borrowed") << ','
             << csvQuote(borrower) << ','
             << csvQuote(dueDate) << '\n';
    }

    file.close();
    return !file.fail();
}

// SAVE MEMBERS
bool Library::saveMembers() const {
    ofstream file(membersFile, ios::trunc);

    if (!file) {
        cerr << "Error: Could not save members.\n";
        return false;
    }

    file << "member_id,name,email\n";

    for (const Member& member : members) {
        file << csvQuote(member.getId()) << ','
             << csvQuote(member.getName()) << ','
             << csvQuote(member.getEmail()) << '\n';
    }

    file.close();
    return !file.fail();
}

// SAVE LOANS
bool Library::saveLoans() const {
    ofstream file(loansFile, ios::trunc);

    if (!file) {
        cerr << "Error: Could not save loans.\n";
        return false;
    }

    file << "book_id,member_id,issue_date,due_date,returned\n";

    for (const Loan& loan : loans) {
        file << csvQuote(loan.getBookId()) << ','
             << csvQuote(loan.getMemberId()) << ','
             << csvQuote(loan.getIssueDate()) << ','
             << csvQuote(loan.getDueDate()) << ','
             << (loan.isReturned() ? "true" : "false") << '\n';
    }

    file.close();
    return !file.fail();
}
// LOAD DATA
bool Library::loadData() {
    books.clear();
    members.clear();
    loans.clear();

    // LOAD BOOKS
    ifstream bookFile(booksFile);

    if (bookFile) {
        string line;
        getline(bookFile, line);

        while (getline(bookFile, line)) {
            if (line.find_first_not_of(" \t\r\n") == string::npos) {
                continue;
            }

            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            vector<string> f = parseCSV(line);

            if (f.size() != 8) {
                cerr << "Warning: Skipping malformed book row.\n";
                continue;
            }

            try {
                int year = stoi(f[4]);

                if (f[0].empty() || f[1].empty() ||
                    f[2].empty() || f[3].empty() ||
                    !validYear(year) ||
                    (f[5] != "available" && f[5] != "borrowed")) {
                    cerr << "Warning: Skipping invalid book row.\n";
                    continue;
                }

                auto duplicate = find_if(
                    books.begin(), books.end(),
                    [&](const Book& b) {
                        return b.getId() == f[0];
                    });

                if (duplicate != books.end()) {
                    cerr << "Warning: Duplicate book ID skipped: "
                         << f[0] << '\n';
                    continue;
                }

                books.emplace_back(
                    f[0], f[1], f[2], f[3], year,
                    f[5] == "available");

            } catch (const exception&) {
                cerr << "Warning: Skipping book row with invalid year.\n";
            }
        }
    }

    // LOAD MEMBERS
    ifstream memberFile(membersFile);

    if (memberFile) {
        string line;
        getline(memberFile, line);

        while (getline(memberFile, line)) {
            if (line.find_first_not_of(" \t\r\n") == string::npos) {
                continue;
            }

            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            vector<string> f = parseCSV(line);

            if (f.size() != 3 || f[0].empty() || f[1].empty()) {
                cerr << "Warning: Skipping invalid member row.\n";
                continue;
            }

            auto duplicate = find_if(
                members.begin(), members.end(),
                [&](const Member& m) {
                    return m.getId() == f[0];
                });

            if (duplicate == members.end()) {
                members.emplace_back(f[0], f[1], f[2]);
            } else {
                cerr << "Warning: Duplicate member ID skipped: "
                     << f[0] << '\n';
            }
        }
    }

    // LOAD LOANS
    ifstream loanFile(loansFile);

    if (loanFile) {
        string line;
        getline(loanFile, line);

        while (getline(loanFile, line)) {

            // Skip blank lines
            if (line.find_first_not_of(" \t\r\n") == string::npos) {
                continue;
            }

            // Remove carriage return
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            vector<string> f = parseCSV(line);

            if (f.size() != 5 ||
                f[0].empty() || f[1].empty() ||
                !validDate(f[2]) || !validDate(f[3]) ||
                f[3] < f[2] ||
                (f[4] != "true" && f[4] != "false")) {
                cerr << "Warning: Skipping invalid loan row.\n";
                continue;
            }

            bool bookExists = any_of(
                books.begin(), books.end(),
                [&](const Book& b) {
                    return b.getId() == f[0];
                });

            bool memberExists = any_of(
                members.begin(), members.end(),
                [&](const Member& m) {
                    return m.getId() == f[1];
                });

            bool returned = f[4] == "true";

            if (!bookExists || !memberExists) {
                cerr << "Warning: Loan refers to a missing book or member.\n";
                continue;
            }

            bool activeDuplicate = false;

            if (!returned) {
                for (const Loan& existing : loans) {
                    if (!existing.isReturned() &&
                        (existing.getBookId() == f[0] ||
                         (existing.getMemberId() == f[1] &&
                          existing.getBookId() == f[0]))) {
                        activeDuplicate = true;
                        break;
                    }
                }
            }

            if (activeDuplicate) {
                cerr << "Warning: Duplicate active loan skipped.\n";
                continue;
            }

            loans.emplace_back(f[0], f[1], f[2], f[3], returned);
        }
    }

    // UPDATE BOOK AVAILABILITY
    for (Book& book : books) {
        bool borrowed = any_of(
            loans.begin(), loans.end(),
            [&](const Loan& loan) {
                return loan.getBookId() == book.getId() &&
                       !loan.isReturned();
            });

        book.setAvailable(!borrowed);
    }

    cout << "Loaded " << books.size() << " book(s), "
         << members.size() << " member(s), and "
         << loans.size() << " loan record(s).\n";

    return true;
}
// ADD BOOK
bool Library::addBook(const Book& book) {
    if (book.getId().empty() || book.getTitle().empty() ||
        book.getAuthor().empty() || book.getCategory().empty() ||
        !validYear(book.getYear())) {
        cout << "Invalid book details.\n";
        return false;
    }

    for (const Book& existing : books) {
        if (existing.getId() == book.getId()) {
            cout << "A book with that ID already exists.\n";
            return false;
        }
    }

    books.push_back(book);

    if (!saveBooks()) {
        books.pop_back();
        return false;
    }

    cout << "Book added successfully.\n";
    return true;
}

// ADD MEMBER
bool Library::addMember(const Member& member) {
    if (member.getId().empty() || member.getName().empty()) {
        cout << "Invalid member details.\n";
        return false;
    }

    for (const Member& existing : members) {
        if (existing.getId() == member.getId()) {
            cout << "A member with that ID already exists.\n";
            return false;
        }
    }

    members.push_back(member);

    if (!saveMembers()) {
        members.pop_back();
        return false;
    }

    cout << "Member added successfully.\n";
    return true;
}

// DISPLAY BOOKS
void Library::listBooks() const {
    if (books.empty()) {
        cout << "No books found.\n";
        return;
    }

    cout << "\nBOOK LIST\n";

    cout << left << setw(10) << "ID"
         << setw(32) << "TITLE"
         << setw(24) << "AUTHOR"
         << setw(20) << "CATEGORY"
         << setw(8) << "YEAR"
         << "STATUS\n";

    for (const Book& book : books) {
        cout << left << setw(10) << book.getId()
             << setw(32) << book.getTitle()
             << setw(24) << book.getAuthor()
             << setw(20) << book.getCategory()
             << setw(8) << book.getYear()
             << (book.isAvailable() ? "Available" : "Borrowed")
             << '\n';
    }
}

// DISPLAY MEMBERS
void Library::listMembers() const {
    if (members.empty()) {
        cout << "No members found.\n";
        return;
    }

    cout << "\nMEMBER LIST\n";

    for (const Member& member : members) {
        cout << member.getId() << " | "
             << member.getName() << " | "
             << member.getEmail() << '\n';
    }
}

// LINEAR SEARCH
void Library::searchBook(const string& keyword) const {
    string key = lowerText(keyword);
    bool found = false;

    for (const Book& book : books) {
        if (lowerText(book.getId()).find(key) != string::npos ||
            lowerText(book.getTitle()).find(key) != string::npos ||
            lowerText(book.getAuthor()).find(key) != string::npos ||
            lowerText(book.getCategory()).find(key) != string::npos) {

            cout << book.getId() << " | " << book.getTitle()
                 << " | " << book.getAuthor()
                 << " | "
                 << (book.isAvailable() ? "Available" : "Borrowed")
                 << '\n';

            found = true;
        }
    }

    if (!found) {
        cout << "No matching books found.\n";
    }
}

// BINARY SEARCH
void Library::binarySearchBook(const string& id) const {
    if (books.empty()) {
        cout << "No books available to search.\n";
        return;
    }

    // Create a sorted copy of the books.
    vector<Book> sortedBooks = books;

    sort(sortedBooks.begin(), sortedBooks.end(),
         [](const Book& a, const Book& b) {
             return a.getId() < b.getId();
         });

    // Perform Binary Search.
    auto it = lower_bound(
        sortedBooks.begin(), sortedBooks.end(), id,
        [](const Book& book, const string& searchId) {
            return book.getId() < searchId;
        });

    if (it != sortedBooks.end() && it->getId() == id) {
        cout << "Book found using Binary Search:\n";

        cout << it->getId() << " | " << it->getTitle()
             << " | " << it->getAuthor()
             << " | "
             << (it->isAvailable() ? "Available" : "Borrowed")
             << '\n';
    } else {
        cout << "Book ID not found.\n";
    }
}

// SORT BY TITLE
void Library::sortBooksByTitle() {
    sort(books.begin(), books.end(),
         [](const Book& a, const Book& b) {
             return lowerText(a.getTitle()) <
                    lowerText(b.getTitle());
         });

    cout << "Books sorted by title.\n";
    listBooks();
}

// SORT BY ID
void Library::sortBooksById() {
    sort(books.begin(), books.end(),
         [](const Book& a, const Book& b) {
             return a.getId() < b.getId();
         });

    cout << "Books sorted by ID.\n";
    listBooks();
}
// UPDATE BOOK
bool Library::updateBook(const string& id,
                         const string& title,
                         const string& author,
                         const string& category,
                         int year) {

    if (title.empty() || author.empty() || category.empty() ||
        !validYear(year)) {
        cout << "Invalid book details.\n";
        return false;
    }

    for (Book& book : books) {
        if (book.getId() == id) {
            Book oldBook = book;

            book.setTitle(title);
            book.setAuthor(author);
            book.setCategory(category);
            book.setYear(year);

            if (!saveBooks()) {
                book = oldBook;
                cout << "Could not save update.\n";
                return false;
            }

            cout << "Book updated successfully.\n";
            return true;
        }
    }

    cout << "Book ID not found.\n";
    return false;
}

// ISSUE BOOK
bool Library::issueBook(const string& bookId,
                        const string& memberId,
                        const string& issueDate,
                        const string& dueDate) {

    if (!validDate(issueDate) || !validDate(dueDate) ||
        dueDate < issueDate) {
        cout << "Invalid dates. Use YYYY-MM-DD and ensure due date is not earlier.\n";
        return false;
    }

    Book* selectedBook = nullptr;
    bool memberExists = false;

    for (Book& book : books) {
        if (book.getId() == bookId) {
            selectedBook = &book;
            break;
        }
    }

    for (const Member& member : members) {
        if (member.getId() == memberId) {
            memberExists = true;
            break;
        }
    }

    if (selectedBook == nullptr) {
        cout << "Book ID not found.\n";
        return false;
    }

    if (!memberExists) {
        cout << "Member ID not found.\n";
        return false;
    }

    if (!selectedBook->isAvailable()) {
        cout << "Book is already borrowed.\n";
        return false;
    }

    loans.emplace_back(bookId, memberId, issueDate, dueDate, false);
    selectedBook->setAvailable(false);

    if (!saveLoans() || !saveBooks()) {
        loans.pop_back();
        selectedBook->setAvailable(true);

        saveLoans();
        saveBooks();

        cout << "Could not save loan. Operation cancelled.\n";
        return false;
    }

    cout << "Book issued successfully.\n";
    return true;
}

// RETURN BOOK
bool Library::returnBook(const string& bookId,
                         const string& memberId) {

    for (Loan& loan : loans) {
        if (loan.getBookId() == bookId &&
            loan.getMemberId() == memberId &&
            !loan.isReturned()) {

            loan.markReturned();

            Book* selectedBook = nullptr;

            for (Book& book : books) {
                if (book.getId() == bookId) {
                    selectedBook = &book;
                    break;
                }
            }

            if (selectedBook != nullptr) {
                selectedBook->setAvailable(true);
            }

            if (!saveLoans() || !saveBooks()) {
                cout << "Warning: Could not save return operation.\n";
                return false;
            }

            cout << "Book returned successfully.\n";
            return true;
        }
    }

    cout << "No active loan found for that book and member.\n";
    return false;
}

// DISPLAY AVAILABLE BOOKS
void Library::listAvailableBooks() const {
    bool found = false;

    cout << "\nAVAILABLE BOOKS\n";

    for (const Book& book : books) {
        if (book.isAvailable()) {
            cout << book.getId() << " | "
                 << book.getTitle() << " | "
                 << book.getAuthor() << '\n';

            found = true;
        }
    }

    if (!found) {
        cout << "No books are currently available.\n";
    }
}