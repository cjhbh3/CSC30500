#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct Book {
  char title[100]; // possible spaces
  string author; // no spaces
  string isbn; // no spaces
  int year;
};

struct Library {
  string name; // no spaces
  string city; // no spaces
  int zipCode; // no spaces
};

struct Hold {
  string isbn; // no spaces
  string library; // no spaces
  // represents how many holds has been put on this isbn at this library
  int copyNumber;
};

Book createBook(string isbn);
Library createLibrary(string name);

int main() {
  // ios_base::app forces you to append text to the file.
  ofstream dataOut("data.txt", ios_base::app);
  list <Book> books;
  list <Library> libraries;
  list <Hold> holds;
  string input = "";

  while (input != "q") {
    ifstream dataIn("data.txt");

    cin >> input;
    // possible inputs a,l,f,q

    // *** Pulling existing data from file ***
    books.clear();
    libraries.clear();
    holds.clear();
    while(!dataIn.eof()) {
      
      string fragment;
      
      
      dataIn >> fragment;
      
      if (fragment == "b") {
        Book newBook;
        dataIn >>  newBook.isbn;
        dataIn >> newBook.year;
        dataIn >> newBook.author;
        dataIn.getline(newBook.title, sizeof(newBook.title));
        books.push_back(newBook);
      }
      else if (fragment == "l") {
        Library newLibrary;
        dataIn >> newLibrary.name;
        dataIn >> newLibrary.city;
        dataIn >> newLibrary.zipCode;
        libraries.push_back(newLibrary);
      }
      else if (fragment == "h") {
        Hold newHold;
        dataIn >> newHold.isbn;
        dataIn >> newHold.library;
        newHold.copyNumber = 1;
        for (Hold hold : holds) {
          if (newHold.isbn == hold.isbn && newHold.library == hold.library) {
            newHold.copyNumber++;
          }
        }
        holds.push_back(newHold);
      }
    }
    // *** End of the existing section ***

    // a: add
    if (input == "a") {
      cin >> input;

      if (input == "b") {
        string isbn;
        bool isbnFound = false;
        cin >> isbn;
        for (Book book : books) {
          if (book.isbn == isbn) {
            isbnFound == true;
          }
        }
        if (!isbnFound) {
          Book book = createBook(isbn);
          dataOut << "b " << book.isbn << " " << book.year << " " << book.author << " " << book.title << endl;
        }
        else {
          cout << "Book not added: ISBN already exists." << endl;
        }
      }
      else if (input == "l") {
        string name;
        bool nameFound = false;
        cin >> name;
        for (Library library : libraries) {
          if (library.name == name) {
            nameFound = true;
          }
        }
        if (!nameFound) {
          Library library = createLibrary(name);
          dataOut << "l " << library.name << " " << library.city << " " << library.zipCode << endl;
        }
        else  {
          cout << "Library not added: Library Name already exists." << endl;
        }
      }
      else if (input == "h") {
        // Add new hold on a book
        // isbn then library name
        string isbn;
        string name;
        bool isbnFound = false;
        bool nameFound = false;

        cin >> isbn;
        cin >> name;

        for (Book book : books) {
          if (book.isbn == isbn) {
            isbnFound = true;
          }
        }

        for (Library libary : libraries) {
          if (libary.name == name) {
            nameFound = true;
          }
        }

        if (isbnFound && nameFound) {
          dataOut << "h " << isbn << " " << name << endl;
        }
        else if (!isbnFound || !nameFound) {
          cout << "Hold not placed: ISBN or Library Name not found." << endl;
        }
        // We'll have to check 1. library exists 2. isbn exists
      }
    }
    // l: list
    else if (input == "l") {
      cin >> input; 

      if (input == "b") {
        for (Book book : books) {
          cout << book.isbn << " " << book.year << " " << book.author << " " << book.title << endl;
        }
      }
      else if (input == "l") {
        for (Library library : libraries) {
          cout << library.name <<  " " << library.city << " " << library.zipCode << endl;
        }
      }
    }
    // f: find
    else if (input == "f") {
      // search through hold for matching isbn
      // display library name, isbn number, date, author, title, and copy number
      string isbn;
      string author;
      string library;
      char * title;
      int year;
      int copyNumber;

      cin >> isbn; // isbn;

      // Check if book exists
      for (Book book : books) {
        if (book.isbn == isbn) {
          author = book.author;
          title = book.title;
          year = book.year;
        }
      }

      // Check if a hold exists on this isbn
      for (Hold hold : holds) {
        if (hold.isbn == isbn) {
          library = hold.library;
          copyNumber = hold.copyNumber;
          cout << library << ": " << isbn << " " << year << " " << author << " " 
          << title << " Copy Number: " << copyNumber << endl;
        }
      }
      // Delete and reallocate memory used on pointer
      title =  nullptr;
      delete title;
    }
    // q: quit
    dataIn.close();
  }

  // while input, branch to different possible inputs
  
  dataOut.close();
  return 0;
}

Book createBook(string isbn) {
  Book newBook;
  newBook.isbn = isbn;
  cin >> newBook.year;
  cin >> newBook.author;
  cin.getline(newBook.title, sizeof(newBook.title));
  return newBook;
}

Library createLibrary(string name) {
  Library newLibrary;
  newLibrary.name = name;
  cin >> newLibrary.city;
  cin >> newLibrary.zipCode;
  return newLibrary;
}