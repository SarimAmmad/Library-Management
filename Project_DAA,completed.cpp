#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

class Client {
private:
    string id;
    string username;
    string firstName;
    string fatherName;
    string password;
public:
    Client(string id, string username, string firstName, string fatherName, string password)
        : id(id), username(username), firstName(firstName), fatherName(fatherName), password(password) {}
    string getId() const {
        return id;
    }
    string getUsername() const {
        return username;
    }
    string getFirstName() const {
        return firstName;
    }
    string getFatherName() const {
        return fatherName;
    }
    string getPassword() const {
        return password;
    }
};

class Book {
private:
    string title;
    string bookId;
    bool borrowed;
public:
    Book(string title, string bookId) : title(title), bookId(bookId), borrowed(false) {}
    string getTitle() const {
        return title;
    }
    string getBookId() const {
        return bookId;
    }
    bool isBorrowed() const {
        return borrowed;
    }
    void borrow() {
        borrowed = true;
    }
    void returnBook() {
        borrowed = false;
    }
};

class Administrator {
private:
    string username;
    string password;
public:
    Administrator(string username, string password)
        : username(username), password(password) {}

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return password;
    }
};

class BorrowedBookNode {
public:
    string title;
    string bookId;
    BorrowedBookNode* left;
    BorrowedBookNode* right;
    BorrowedBookNode(string title, string bookId) : title(title), bookId(bookId), left(nullptr), right(nullptr) {}
};

class BookNode {
public:
    string title;
    string bookId;
    BookNode* left;
    BookNode* right;
    BookNode(string title, string bookId) : title(title), bookId(bookId), left(nullptr), right(nullptr) {}
};

class Library {
private:
    unordered_map<string, Client> clients;
    vector<Book> books;
    Administrator admin;
    BorrowedBookNode* borrowedBooksRoot;
    BookNode* booksRoot;

    BorrowedBookNode* insertBorrowedBook(BorrowedBookNode* root, string title, string bookId) {
        if (!root) return new BorrowedBookNode(title, bookId);
        if (title < root->title)
            root->left = insertBorrowedBook(root->left, title, bookId);
        else
            root->right = insertBorrowedBook(root->right, title, bookId);
        return root;
    }

    BookNode* insertBookNode(BookNode* root, string title, string bookId) {
        if (!root) return new BookNode(title, bookId);
        if (title < root->title)
            root->left = insertBookNode(root->left, title, bookId);
        else
            root->right = insertBookNode(root->right, title, bookId);
        return root;
    }

    void inOrderBorrowedBooks(BorrowedBookNode* root) const {
        if (root) {
            inOrderBorrowedBooks(root->left);
            cout << "Title: " << root->title << ", Book ID: " << root->bookId << endl;
            inOrderBorrowedBooks(root->right);
        }
    }

public:
    Library(Administrator admin) : admin(admin), borrowedBooksRoot(nullptr), booksRoot(nullptr) {
        // Pre-add some books
        addBook("The legends of islam", "011");
        addBook("The great warriors", "012");
        addBook("History of the universe", "013");
        addBook("The knowledge of programming", "014");
        addBook("Data Mining", "015");
    }

    void addClient(Client client) {
        clients.emplace(client.getId(), client);
    }

    void addBook(const string& title, const string& bookId) {
        books.emplace_back(title, bookId);
        booksRoot = insertBookNode(booksRoot, title, bookId);
    }

    bool validateClient(string clientId) const {
        return clients.find(clientId) != clients.end();
    }

    bool validateAdmin(string username, string password) const {
        return admin.getUsername() == username && admin.getPassword() == password;
    }

    void displayAllClients() const {
        cout << "All Registered Clients:\n";
        for (const auto& client : clients) {
            cout << "ID: " << client.first << ", Username: " << client.second.getUsername() << endl;
        }
    }

    void displayAllBooks() const {
        cout << "All Available Books:\n";
        for (const auto& book : books) {
            cout << "Title: " << book.getTitle() << ", Book ID: " << book.getBookId();
            if (book.isBorrowed()) {
                cout << " (Borrowed)";
            }
            cout << endl;
        }
    }

    void addClientProfile() {
        string id, username, firstName, fatherName, password;
        cout << "Enter Client ID: ";
        cin >> id;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter First Name: ";
        cin >> firstName;
        cout << "Enter Father's Name: ";
        cin >> fatherName;
        cout << "Enter Password: ";
        cin >> password;
        Client client(id, username, firstName, fatherName, password);
        addClient(client);
        cout << "Profile created successfully!\n";
    }

    void adminMenu() {
        system("cls");
        cout << "\nAdministrator Menu:\n";
        cout << "1. Add Book\n";
        cout << "2. Delete Book\n";
        cout << "3. View All Books\n";
        cout << "4. Add Client\n";
        cout << "5. Delete Client\n";
        cout << "6. View All Clients\n";
        cout << "7. View Borrowed Books\n";
        cout << "8. Exit\n";
    }

    void adminTasks() {
        int choice;
        while (true) {
            adminMenu();
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice) {
                case 1:
                    addBook();
                    break;
                case 2:
                    deleteBook();
                    break;
                case 3:
                    displayAllBooks();
                    break;
                case 4:
                    addClientProfile();
                    break;
                case 5:
                    deleteClient();
                    break;
                case 6:
                    displayAllClients();
                    break;
                case 7:
                    displayBorrowedBooks();
                    break;
                case 8:
                    cout << "Exiting Administrator Menu...\n";
                    return;
                default:
                    cout << "Invalid choice. Please enter a valid option.\n";
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    void addBook() {
        string title, bookId;
        cout << "Enter Book Title: ";
        cin.ignore();
        getline(cin, title);
        cout << "Enter Book ID: ";
        getline(cin, bookId);
        addBook(title, bookId);
        cout << "Book added successfully!\n";
    }

    void deleteBook() {
        string title;
        cout << "Enter Book Title to delete: ";
        cin.ignore();
        getline(cin, title);
        auto it = find_if(books.begin(), books.end(), [&title](const Book& book) {
            return book.getTitle() == title;
        });
        if (it != books.end()) {
            books.erase(it);
            cout << "Book deleted successfully!\n";
        } else {
            cout << "Book not found!\n";
        }
    }

    void deleteClient() {
        string id;
        cout << "Enter Client ID to delete: ";
        cin >> id;
        auto it = clients.find(id);
        if (it != clients.end()) {
            clients.erase(it);
            cout << "Client deleted successfully!\n";
        } else {
            cout << "Client not found!\n";
        }
    }

    void clientMenu(const string& clientId) {
        int clientChoice;
        do {
            system("cls"); 
            cout << "\nClient Menu:\n";
            cout << "1. Borrow a Book\n";
            cout << "2. View All Books\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> clientChoice;
            switch (clientChoice) {
                case 1:
                    borrowBookMenu(clientId);
                    break;
                case 2:
                    displayAllBooks();
                    break;
                case 3:
                    cout << "Exiting Client Menu...\n";
                    break;
                default:
                    cout << "Invalid choice. Please enter a valid option.\n";
            }
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
        } while (clientChoice != 3);
    }

    void borrowBookMenu(const string& clientId) {
        cout << "Choose how you want to search for the book to borrow:\n";
        cout << "1. By Title\n";
        cout << "2. By Book ID\n";
        int searchChoice;
        cin >> searchChoice;
        switch (searchChoice) {
            case 1:
                borrowBookByTitle(clientId);
                break;
            case 2:
                borrowBookById(clientId);
                break;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
    }

    void borrowBookByTitle(const string& clientId) {
        cout << "Enter the title of the book you want to borrow: ";
        string bookTitle;
        cin.ignore();
        getline(cin, bookTitle);
        auto it = find_if(books.begin(), books.end(), [&bookTitle](const Book& book) {
            return book.getTitle() == bookTitle && !book.isBorrowed();
        });
        if (it != books.end()) {
            it->borrow();
            borrowedBooksRoot = insertBorrowedBook(borrowedBooksRoot, it->getTitle(), it->getBookId());
            cout << "Book \"" << bookTitle << "\" borrowed successfully!\n";
        } else {
            cout << "Book \"" << bookTitle << "\" is not available for borrowing or not found!\n";
        }
    }

    void borrowBookById(const string& clientId) {
        cout << "Enter the ID of the book you want to borrow: ";
        string bookId;
        cin.ignore();
        getline(cin, bookId);
        auto it = find_if(books.begin(), books.end(), [&bookId](const Book& book) {
            return book.getBookId() == bookId && !book.isBorrowed();
        });
        if (it != books.end()) {
            it->borrow();
            borrowedBooksRoot = insertBorrowedBook(borrowedBooksRoot, it->getTitle(), it->getBookId());
            cout << "Book with ID \"" << bookId << "\" borrowed successfully!\n";
        } else {
            cout << "Book with ID \"" << bookId << "\" is not available for borrowing or not found!\n";
        }
    }

    void displayBorrowedBooks() const {
        cout << "Borrowed Books:\n";
        if (!borrowedBooksRoot) {
            cout << "No books borrowed yet.\n";
        } else {
            inOrderBorrowedBooks(borrowedBooksRoot);
        }
    }
};

int main() {
    Administrator admin("admin", "123");
    Library library(admin);
    bool isAdmin = false;
    string username, password;
    string clientId;
    while(true) {
        system("cls"); 
        cout << "Welcome to Library Management System\n";
        cout << "1. Login as Client\n";
        cout << "2. Login as Administrator\n";
        cout << "3. Exit\n";
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter your Client ID: ";
                cin >> clientId;
                if (library.validateClient(clientId)) {
                    cout << "Login successful!\n";
                    library.clientMenu(clientId);
                } else {
                    cout << "Client ID not found!\n";
                    cout << "Create a new profile:\n";
                    library.addClientProfile();
                }
                break;
            case 2:
                cout << "Enter Administrator Username: ";
                cin >> username;
                cout << "Enter Administrator Password: ";
                cin >> password;
                if (library.validateAdmin(username, password)) {
                    isAdmin = true;
                    cout << "Login successful!\n";
                    library.adminTasks();
                } else {
                    cout << "Invalid username or password!\n";
                }
                break;
            case 3:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }
        cout << "Press Enter to continue...";
        cin.ignore();
        cin.get();
    }
    return 0;
}
