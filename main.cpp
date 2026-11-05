
#include "Library.h"

#include <iostream>
#include <limits>
#include <string>

using namespace std;

string readText(const string& prompt) {
    string value;

    while (true) {
        cout << prompt;

        if (!getline(cin, value)) {
            return "";
        }

        if (value.find_first_not_of(" \t\r\n") != string::npos) {
            return value;
        }

        cout << "Input cannot be empty. Try again.\n";
    }
}

int readNumber(const string& prompt, int minimum, int maximum) {
    string line;

    while (true) {
        cout << prompt;

        if (!getline(cin, line)) {
            return -1;
        }

        try {
            size_t position = 0;
            int number = stoi(line, &position);

            if (position == line.size() &&
                number >= minimum && number <= maximum) {
                return number;
            }
        } catch (...) {
            // Invalid input is handled below.
        }

        cout << "Invalid choice. Enter a number from "
             << minimum << " to " << maximum << ".\n";
    }
}

int main() {
    Library library;
    library.loadData();

    while (true) {
        cout << "\n========== LIBRARY MANAGER ==========\n"
             << "1. Add Book\n"
             << "2. List Books\n"
             << "3. Search Book (Linear Search)\n"
             << "4. Update Book\n"
             << "5. Register Member\n"
             << "6. List Members\n"
             << "7. Issue Book\n"
             << "8. Return Book\n"
             << "9. List Available Books\n"
             << "10. Exit\n"
             << "11. Binary Search by Book ID\n"
             << "12. Sort Books by Title\n"
             << "13. Sort Books by ID\n"
             << "14. Library Analytics Report\n"
             << "=====================================\n";

        int choice = readNumber("Enter your choice: ", 1, 14);
        if (choice == -1) {
            cout << "\nInput closed. Exiting Library Manager.\\n";
            break;
        }

        if (choice == 1) {
            string id = readText("Book ID: ");
            string title = readText("Title: ");
            string author = readText("Author: ");
            string category = readText("Category: ");
            int year = readNumber("Year (1-9999): ", 1, 9999);

            library.addBook(Book(id, title, author, category, year));

        } else if (choice == 2) {
            library.listBooks();

        } else if (choice == 3) {
            library.searchBook(
                readText("Search by ID, title, author, or category: "));

        } else if (choice == 4) {
            string id = readText("Book ID to update: ");
            string title = readText("New title: ");
            string author = readText("New author: ");
            string category = readText("New category: ");
            int year = readNumber("New year (1-9999): ", 1, 9999);

            library.updateBook(id, title, author, category, year);

        } else if (choice == 5) {
            string id = readText("Member ID: ");
            string name = readText("Member name: ");
            string email = readText("Member email: ");

            library.addMember(Member(id, name, email));

        } else if (choice == 6) {
            library.listMembers();

        } else if (choice == 7) {
            string bookId = readText("Book ID: ");
            string memberId = readText("Member ID: ");
            string issueDate = readText("Issue date (YYYY-MM-DD): ");
            string dueDate = readText("Due date (YYYY-MM-DD): ");

            library.issueBook(bookId, memberId, issueDate, dueDate);

        } else if (choice == 8) {
            string bookId = readText("Book ID: ");
            string memberId = readText("Member ID: ");

            library.returnBook(bookId, memberId);

        } else if (choice == 9) {
            library.listAvailableBooks();

        } else if (choice == 10) {
            cout << "Thank you for using Library Manager!\n";
            break;

        } else if (choice == 11) {
            string id = readText("Enter exact Book ID: ");
            library.binarySearchBook(id);

        } else if (choice == 12) {
            library.sortBooksByTitle();

        } else if (choice == 13) {
            library.sortBooksById();

        } else if (choice == 14) {
            library.displayAnalyticsReport();
        }
    }

    return 0;
}
