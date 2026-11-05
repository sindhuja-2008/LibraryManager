# Build and Test Evidence

## Clean build

Build from the repository root (not from an IDE's cached build):

```bash
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp Library.cpp Book.cpp Member.cpp Loan.cpp -o library_manager
```

Expected result: compilation completes without errors. Warnings, if any, should be reviewed.

## Manual test checklist

Run `./library_manager` (Windows: `./library_manager.exe`) and verify:

| Test | Action | Expected result |
|---|---|---|
| Startup | Launch with the CSV files present | Existing books, members, and loans load |
| List | Choose 2 | Book list and availability are displayed |
| Linear search | Choose 3 and enter a title fragment | Matching book(s) are displayed |
| Binary search | Choose 11 and enter an existing exact ID | Matching book is displayed |
| Missing ID | Choose 11 and enter an unknown ID | “Book ID not found.” |
| Sort | Choose 12, then 13 | Books display in the selected order |
| Analytics | Choose 14 | Counts, overdue loans, and borrowing rate are displayed |
| Invalid menu input | Enter text or a number outside 1–14 | Application asks for a valid choice |
| Exit | Choose 10 | Application exits normally |

## Second compiler / environment

A GitHub Actions workflow in `.github/workflows/build.yml` builds the same source with GCC and Clang on Ubuntu. After pushing the repository, open the **Actions** tab and confirm both jobs pass. This provides a reproducible second-compiler check independent of the local Windows build.

## Evidence to capture

After running the program, take screenshots of:
1. Successful clean build command.
2. Main menu and book listing.
3. Analytics report (option 14).
4. GitHub Actions showing successful GCC and Clang jobs.

Only mark tests as passed after actually running them in your environment.
