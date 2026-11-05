# Library Manager (C++)

A console-based library management application built with standard C++. It stores books, members, and loan records in CSV files so data remains available between runs.

## Features

- Add and update books; list all and currently available books.
- Register and list library members.
- Issue and return books with validated `YYYY-MM-DD` dates.
- Search by ID, title, author, or category (linear search).
- Search by exact book ID (binary search on a sorted copy).
- Sort books by title or ID.
- View a library analytics report: total, available and borrowed books, members, loan records, overdue loans, and borrowing rate.
- CSV persistence with quoted-field handling, duplicate checks, and warnings for invalid records.

## Project structure

| File | Responsibility |
|---|---|
| `main.cpp` | Console menu and input validation |
| `Library.h/.cpp` | Library operations, CSV persistence, searching, sorting, analytics |
| `Book.h/.cpp` | Book data model |
| `Member.h/.cpp` | Member data model |
| `Loan.h/.cpp` | Loan data model |
| `books.csv` | Book records |
| `members.csv` | Member records |
| `loans.csv` | Loan records |

The application uses separate model classes and a `Library` service class. The menu is kept in `main.cpp`; file persistence and business rules are handled by `Library`.

## Requirements

- A C++11-compatible compiler (GCC or Clang).
- Windows: MSYS2 MinGW/UCRT64 GCC, MinGW-w64, or another C++ compiler.
- Linux/macOS: GCC or Clang.

## Build and run

Run these commands from the repository root:

### GCC

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp Library.cpp Book.cpp Member.cpp Loan.cpp -o library_manager
./library_manager
```

### Windows (MSYS2 UCRT64)

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp Library.cpp Book.cpp Member.cpp Loan.cpp -o library_manager.exe
./library_manager.exe
```

Keep the three CSV files in the same working directory as the executable. The program reads and writes these files using relative paths.

## Using the analytics report

Run the application and choose menu option **14 – View Library Analytics Report**. The report calculates the borrowing rate as:

`borrowed books / total books × 100`

An overdue loan is an unreturned loan whose due date is earlier than the computer's current local date. The report shows zero borrowing rate when the library has no books.

## Data and safety notes

- Back up the CSV files before manually editing them.
- Dates must use `YYYY-MM-DD`.
- The program skips malformed or duplicate records with warnings rather than treating them as valid data.
- `.gitignore` excludes the generated Windows executable from version control.

## Testing

See [TESTING.md](TESTING.md) for build and manual test evidence. See [WALKTHROUGH.md](WALKTHROUGH.md) for a short demonstration script.
