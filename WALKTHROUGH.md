# Project Walkthrough Script (about 2 minutes)

Hello, my name is Sindhuja. This is my C++ Library Manager application.

The project uses separate classes for Book, Member, and Loan. The Library class contains the main library operations and handles CSV data storage. The `main.cpp` file provides the menu and validates user input.

The application supports adding and updating books, registering members, issuing and returning books, searching, sorting, and listing available books. The records are stored in three CSV files, so they can be loaded again when the application restarts.

One useful feature is the Library Analytics Report. I select option 14 to display total books, available books, borrowed books, registered members, loan records, overdue loans, and the borrowing rate. Overdue loans are identified by comparing unreturned loan due dates with the computer's current date.

I also added build instructions, a test checklist, and a GitHub Actions workflow to compile the project with both GCC and Clang. This makes the project easier for another developer to build and evaluate.

Thank you.
