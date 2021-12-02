// Name: CJ Hess
// Date: 11/12/2021

#include <iostream>
#include <mysql/jdbc.h>
#include <string>

using namespace std;

// https://dev.mysql.com/doc/connector-cpp/1.1/en/connector-cpp-connect-options.html
// https://dev.mysql.com/doc/dev/connector-cpp/8.0/jdbc_ref.html
// https://stackoverflow.com/questions/27480741/which-execute-function-should-i-use-in-mysql-connector-c
int main() {
    string hostName, userName, password, databaseName;// hold user input for host, username, password, database
    string input; // hold user input for interaction
    int portNo; // hold user input for port
    bool tablesCreated = false;

    cout << "What is the hostname?" << endl;
    cin >> hostName;
    cout << "What is the port number? (0 is default)" << endl;
    cin >> portNo;
    cout << "What is the username?" << endl;
    cin >> userName;
    cout << "What is the password?" << endl;
    cin >> password;
    cout << "What is the database name?" << endl;
    cin >> databaseName;

	// Sets up connection to SQL server
	sql::Driver* driver = sql::mysql::get_driver_instance();
    sql::ConnectOptionsMap connection_properties;
    connection_properties["hostName"] = hostName;
    connection_properties["userName"] = userName;
    connection_properties["password"] = password;
    if (portNo != 0) {
        connection_properties["port"] = portNo;
    }

    try {
        // Attempts to connect with specified properties
        unique_ptr<sql::Connection> con(driver->connect(connection_properties));

        // Check if the connection is  live
        if (con->isValid()) {
            cout << "Connection successful" << endl;
        }

        con->setSchema(databaseName);

        unique_ptr<sql::Statement> stmnt(con->createStatement());

        if (!tablesCreated) {
            try {
                stmnt->execute("CREATE TABLE Library (name VARCHAR(20), city VARCHAR(20), zipCode INT);");
                stmnt->execute("CREATE TABLE Book (isbn VARCHAR(20), year INT, author VARCHAR(20), title VARCHAR(20));");
                stmnt->execute("CREATE TABLE Holds (isbn VARCHAR(20), libraryName VARCHAR(20), copyNumber INT);");
                tablesCreated = true;
                cout << "Tables created." << endl;
            }
            catch (sql::SQLException& e) {
                tablesCreated = true;
                cout << "Tables already exist." << endl;
            }
        }

        while (input != "q") {
            cin >> input;

            // a: add
            if (input == "a") {
                cin >> input;

                if (input == "b") {
                    // prompt structure isbn, year, author, title
                    string isbn, year, author;
                    char title[100];

                    cin >> isbn;
                    
                    unique_ptr<sql::ResultSet> res(stmnt->executeQuery("SELECT * FROM Book WHERE isbn = " + isbn + ";"));
                    
                    if (!res->next()) {
                        cout << "ISBN not found" << endl;
                        cin >> year;
                        cin >> author;
                        cin.getline(title, sizeof(title));
                        string values = "(\"" + isbn + "\"," + year + ",\"" + author + "\",\"" + title + "\")";
                        cout << values << endl;
                        int numRows = stmnt->executeUpdate("INSERT INTO Book VALUES " + values);
                        if (numRows > 0) {
                            cout << "Successfully added Book" << endl;
                        }
                    }
                    else {
                        cout << "Book not added: ISBN already exists." << endl;
                    }
                }
                else if (input == "l") {
                    // prompt structure name, city, zipcode
                    string name, city, zipCode;
                    

                    cin >> name;

                    unique_ptr<sql::ResultSet> res(stmnt->executeQuery("SELECT * FROM Library WHERE name = \"" + name + "\";"));

                    if (!res->next()) {
                        cout << "Name not found" << endl;
                        cin >> city;
                        cin >> zipCode;
                        string values = "(\"" + name + "\",\"" + city + "\","  + zipCode + ")";
                        cout << values << endl;
                        int numRows = stmnt->executeUpdate("INSERT INTO Library VALUES " + values);
                        if (numRows > 0) {
                            cout << "Successfully added Library" << endl;
                        }
                    }
                    else {
                        cout << "Library not added: Name already exists." << endl;
                    }
                }
                else if (input == "h") {
                    // prompt structure isbn, name, copyNumber
                    string isbn, name;
                    int copyNo = 1;

                    cin >> isbn;
                    cin >> name;
                    
                    unique_ptr<sql::ResultSet> bookCheck(stmnt->executeQuery("SELECT * FROM Book WHERE isbn = " + isbn + ";"));
                    unique_ptr<sql::ResultSet> libraryCheck(stmnt->executeQuery("SELECT * FROM Library WHERE name = \"" + name + "\";"));

                    if (bookCheck->next() && libraryCheck->next()) {
                        cout << "ISBN and Library found." << endl;
                        unique_ptr<sql::ResultSet> holdCheck(stmnt->executeQuery("SELECT * FROM Holds WHERE isbn = " + isbn + " AND libraryName = \"" + name + "\";"));
                        if (holdCheck->next()) {
                            copyNo++;
                            while (holdCheck->next()) {
                                copyNo++;
                            }
                        }
                        string values = "(" + isbn + ",\"" + name + "\"," + to_string(copyNo) + ")";
                        cout << values << endl;
                        int numRows = stmnt->executeUpdate("INSERT INTO Holds VALUES " + values);
                        if (numRows > 0) {
                            cout << "Successfully added Hold" << endl;
                        }
                    }
                    else {
                        cout << "Hold not added: ISBN or Library does not exist." << endl;
                    }
                    // We'll have to check 1. library exists 2. isbn exists
                }
            }
            // l: list
            else if (input == "l") {
                cin >> input;

                if (input == "b") {
                    unique_ptr<sql::ResultSet> res(stmnt->executeQuery("SELECT * FROM Book;"));
                    while (res->next()) {
                        cout << res->getString("isbn") << " " << to_string(res->getInt("year")) << " " << res->getString("author") << " " << res->getString("title") << endl;
                    }
                }
                else if (input == "l") {
                    unique_ptr<sql::ResultSet> res(stmnt->executeQuery("SELECT * FROM Library;"));
                    while (res->next()) {
                        cout << res->getString("name") << " " << res->getString("city") << " " << to_string(res->getInt("zipCode")) << endl;
                    }
                }
            }
            // f: find
            else if (input == "f") {
                // search through hold for matching isbn
                // display library name, isbn number, year, author, title, and copy number
                string isbn, year, author, title, libraryName;
                int copyNumber;
                
                cin >> isbn; // isbn;
                // Check if book exists
                unique_ptr<sql::ResultSet> bookCheck(stmnt->executeQuery("SELECT * FROM Book WHERE isbn = " + isbn + ";"));
                while (bookCheck->next()) {
                    // Check if a hold exists on this isbn
                    year = to_string(bookCheck->getInt("year"));
                    author = bookCheck->getString("author");
                    title = bookCheck->getString("title");
                }

                // Check if holds on book exists
                unique_ptr<sql::ResultSet> holdCheck(stmnt->executeQuery("SELECT * FROM Holds WHERE isbn = " + isbn + ";"));
                while (holdCheck->next()) {
                    libraryName = holdCheck->getString("libraryName");
                    copyNumber = holdCheck->getInt("copyNumber");

                    cout << libraryName << " " << isbn << " " << year << " " << author << " " << title << " " << to_string(copyNumber) << endl;
                }
                
            }
            // d: delete
            else if (input == "d") {
                string isbn, libraryName;

                cin >> isbn;
                cin >> libraryName;

                int numRows = stmnt->executeUpdate("DELETE FROM Holds WHERE isbn = " + isbn + " AND libraryName = \"" + libraryName + "\";");
                if (numRows > 0) {
                    cout << "Deleted " << to_string(numRows) << " holds." << endl;
                }
                else {
                    cout << "No rows deleted." << endl;
                }
            }
            // q: quit
        }
        // while input, branch to different possible inputs     
    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << "EXAMPLE_FUNCTION" << ") on line " << __LINE__ << endl;

        /* Use what() (derived from std::runtime_error) to fetch the error message */

        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }

    return 0;
}