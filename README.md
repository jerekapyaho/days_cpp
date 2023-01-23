# Days

Days is a UNIX-style utility for reporting about events and their relation 
to the current date. It shows both past and future events with a count of 
days since or until an event.

The events have a date, a category and a description. They are stored in a 
CSV file called `events.csv` in a subdirectory called `.days` inside the 
user's home directory.

Days can also congratulate the user on their birthday if the current date 
is the same as their birthdate, set as an enviroment variable called 
`BIRTHDATE`, with the value in YYYY-MM-DD format, for example, `1988-08-16`.

## The event file

The program reads the events to display from a CSV file. Create the `~/.days` directory if it doesn't already exist. Add a file called `events.csv` 
inside that directory, with contents in the following format:

    date,category,description
    2020-12-15,computing,C++20 released
    2023-01-10,computing,Rust 1.66.1 released
    2022-09-20,computing,Java SE 19 released
    2014-11-12,computing,.NET Core released

The format of each line should be `date,category,description` in the CSV (comma-separated value) style. Here `date` is the date in ISO 8601 standard
format `YYYY-MM-DD`, while `category` and `description` are free text.

Note that the first line in the file is a column header, and should be included
as such.

Ideally, the value of the `category` column is a single word like `history`, 
`personal` or `computing`, but there are no predefined categories.

*NOTE*: If your description contains commas, you will need to put it in
quotation marks, because otherwise the commas would mess up the CSV parsing.
For example, a line with a description containing commas could be:

    2010-02-14,personal,"Signed, sealed, and delivered"

Users can edit this file with a text editor. Later on this program may get
features that allow you to add or delete events and update this file.
The program will reject any lines that are not in the correct format.

## Compiling the program

Days is written in C++20, so you will need a C++ compiler that supports 
that standard.

### Windows

In Microsoft Windows the use of the Microsoft C/C++ Compiler is preferred. 
If you have Visual Studio installed, and have also installed the "Desktop 
Development with C/C++" workload, you have the necessary tools installed. 
Open "Developer Command Prompt for Microsoft Visual Studio 2022" (or whatever
version you have, like 2019) from the Start menu, navigate to the directory 
where you cloned this repository, and use the command

    cl /std:c++20 /EHsc days.cpp event.cpp

to compile the program. The result is an executable file called `days.exe`, 
which you can run with the command `days` in the Command Prompt.

### macOS

In macOS you can use either the `clang` compiler installed with Xcode, or 
the GNU C/C++ compiler installed with Homebrew. For example, if you have 
Xcode installed, you should be able to compile the program with

    clang++ -std=c++20 -o days days.cpp event.cpp

which produces an executable file called `days`. Run the program with 
`./days` (the `./` prefix is needed because you should never have the 
current directory in your PATH).

### Linux

In a standard Linux environment you probably already have development tools 
installed, so you should be able to compile the program using the GNU C++ 
compiler:

    g++ -std=c++20 -o days days.cpp event.cpp

which produces an executable file called `days`. Run the program with 
`./days` (the `./` prefix is needed because you should never have the 
current directory in your PATH).

Please use at least GCC 11 to enjoy the C++20 features. Note that you might need to update your distro to a newer version, eg. WSL2 Ubuntu users need to update from 20.04 to 22.04 so they you can use GCC 11. Instructions how to update are [here](https://askubuntu.com/questions/1428423/upgrade-ubuntu-in-wsl2-from-20-04-to-22-04).

## The BIRTHDATE environment variable

If the program environment contains the `BIRTHDATE` variable, and its value 
is a date in the `YYYY-MM-DD` format, this application will use the value 
to show a birthday greeting to the user.

If you are a Linux user, you should set the `BIRTHDATE` environment variable 
in your `.bashrc` file (or `~/.bash_profile`, or `~/.profile`, or whatever 
your shell uses).

Windows users need to open the old style Control Panel, then navigate to 
System Properties, Advanced, Environment Variables. Set a user variable,
not a system variable.

Note that changes to environment variables will not be picked up by a 
running shell. After you have defined the environment variable, close the 
shell window, then open it again.
