# Days

Days ia a UNIX-style utility for reporting about events and their relation to the current date.
It shows both past and future events with a count of days since or until an event.

The events have a date, a category and a description. They are stored in a CSV file called `events.csv`
in a subdirectory called `.days` inside the user's home directory.

Days can also congratulate the user on their birthday if the current date is the same as their
birthdate, set as an enviroment variable called `BIRTHDATE`, with the value in YYYY-MM-DD format,
for example, `1988-08-16`.

## Compiling the program

Days is written in C++20, so you will need a C++ compiler that supports that standard.

### Windows

In Microsoft Windows the use of the Microsoft C/C++ Compiler is preferred. If you have Visual Studio
installed, and have also installed the "Desktop Development with C/C++" workload, you have the
necessary tools installed. Open "Developer Command Prompt for Microsoft Visual Studio 2022" (or whatever
version you have, like 2019) from the Start menu, navigate to the directory where you cloned this repository,
and use the command

    cl days.cpp

to compile the program. The result is an executable file called `days.exe`, which you can run with
the command `days` in the Command Prompt.

### macOS

In macOS you can use either the `clang` compiler installed with Xcode, or the GNU C/C++ compiler installed
with Homebrew. For example, if you have Xcode installed, you should be able to compile the program with

    clang++ -o days days.cpp

which produces an executable file called `days`. Run the program with `./days` (the `./` prefix is needed
because you should never have the current directory in your PATH).

### Linux

In a standard Linux environment you probably already have development tools installed, so you should
be able to compile the program using the GNU C++ compiler:

    g++ -o days days.cpp

which produces an executable file called `days`. Run the program with `./days` (the `./` prefix is needed
because you should never have the current directory in your PATH).
