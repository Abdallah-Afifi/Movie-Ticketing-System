#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

class User {
public:
    std::string username;
    std::string password;

    User() = default;
    User(const std::string& u, const std::string& p) : username(u), password(p) {}
};

class Seat {
public:
    int row;
    int col;
    double price;
    bool reserved;

    Seat(int r, int c, double pr) : row(r), col(c), price(pr), reserved(false) {}
};

class TicketingSystem {
private:
    std::vector<std::vector<Seat>> seats;
    std::unordered_map<std::string, User> users;
    User currentUser;

public:
    TicketingSystem(int numRows, int numCols) : seats(numRows, std::vector<Seat>(numCols, Seat(0, 0, 0.0))) {
        // Initialize users (for demonstration purposes)
        users["user1"] = User("user1", "pass1");
        users["user2"] = User("user2", "pass2");

        // Initialize seats with prices
        double basePrice = 10.0;
        for (int i = 0; i < numRows; ++i) {
            for (int j = 0; j < numCols; ++j) {
                seats[i][j] = Seat(i + 1, j + 1, basePrice + (i * 2.0)); // Adjust pricing based on row
            }
        }

        // Load state from file
        loadState();
    }

    void displayMenu() {
        std::cout << "1. Purchase Ticket\n"
                  << "2. Reserve Seat\n"
                  << "3. Cancel Reservation\n"
                  << "4. Display Seats\n"
                  << "5. Exit\n";
    }

    bool login() {
        std::string username, password;
        std::cout << "Enter username: ";
        std::cin >> username;
        std::cout << "Enter password: ";
        std::cin >> password;

        auto it = users.find(username);
        if (it != users.end() && it->second.password == password) {
            currentUser = it->second;
            return true;
        } else {
            std::cout << "Invalid username or password. Login failed.\n";
            return false;
        }
    }

    void purchaseTicket() {
        displaySeats();
        int row, col;
        std::cout << "Enter row and column to purchase ticket (e.g., 1 2): ";
        std::cin >> row >> col;

        if (isValidSeat(row, col) && !seats[row - 1][col - 1].reserved) {
            seats[row - 1][col - 1].reserved = true;
            std::cout << "Ticket purchased for seat (" << row << ", " << col << ") at $" << seats[row - 1][col - 1].price << ".\n";
        } else {
            std::cout << "Invalid seat or seat already reserved. Please try again.\n";
        }
    }

    void reserveSeat() {
        displaySeats();
        int row, col;
        std::cout << "Enter row and column to reserve seat (e.g., 1 2): ";
        std::cin >> row >> col;

        if (isValidSeat(row, col) && !seats[row - 1][col - 1].reserved) {
            seats[row - 1][col - 1].reserved = true;
            std::cout << "Seat (" << row << ", " << col << ") reserved at $" << seats[row - 1][col - 1].price << ".\n";
        } else {
            std::cout << "Invalid seat or seat already reserved. Please try again.\n";
        }
    }

    void cancelReservation() {
        displaySeats();
        int row, col;
        std::cout << "Enter row and column to cancel reservation (e.g., 1 2): ";
        std::cin >> row >> col;

        if (isValidSeat(row, col) && seats[row - 1][col - 1].reserved) {
            seats[row - 1][col - 1].reserved = false;
            std::cout << "Reservation for seat (" << row << ", " << col << ") canceled.\n";
        } else {
            std::cout << "Invalid seat or no reservation to cancel. Please try again.\n";
        }
    }

    void displaySeats() {
        std::cout << "------------------------\n";
        std::cout << "      Seating Plan      \n";
        std::cout << "------------------------\n";
        for (int i = 0; i < seats.size(); ++i) {
            for (int j = 0; j < seats[i].size(); ++j) {
                if (seats[i][j].reserved) {
                    std::cout << "[X] ";
                } else {
                    std::cout << "[ ] ";
                }
            }
            std::cout << '\n';
        }
        std::cout << "------------------------\n";
    }

    bool isValidSeat(int row, int col) const {
        return row >= 1 && row <= seats.size() && col >= 1 && col <= seats[0].size();
    }

    void saveState() {
        std::ofstream outFile("state.txt");
        if (outFile.is_open()) {
            for (const auto& row : seats) {
                for (const auto& seat : row) {
                    outFile << seat.reserved << " ";
                }
                outFile << '\n';
            }
            outFile.close();
            std::cout << "State saved successfully.\n";
        } else {
            std::cout << "Error saving state.\n";
        }
    }

    void loadState() {
        std::ifstream inFile("state.txt");
        if (inFile.is_open()) {
            for (auto& row : seats) {
                for (auto& seat : row) {
                    int reserved;
                    inFile >> reserved;
                    seat.reserved = static_cast<bool>(reserved);
                }
            }
            inFile.close();
            std::cout << "State loaded successfully.\n";
        } else {
            std::cout << "No saved state found.\n";
        }
    }
};

int main() {
    int numRows, numCols;
    std::cout << "Enter the number of rows and columns for the theater: ";
    std::cin >> numRows >> numCols;

    TicketingSystem ticketSystem(numRows, numCols);

    if (!ticketSystem.login()) {
        std::cout << "Exiting program.\n";
        return 1;
    }

    int choice;
    do {
        ticketSystem.displayMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                ticketSystem.purchaseTicket();
                break;
            case 2:
                ticketSystem.reserveSeat();
                break;
            case 3:
                ticketSystem.cancelReservation();
                break;
            case 4:
                ticketSystem.displaySeats();
                break;
            case 5:
                std::cout << "Saving state and exiting program.\n";
                ticketSystem.saveState();
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}

