#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

/**
 * @class Event
 * @brief Represents an event with methods for booking and cancellation of seats.
 */
class Event {
public:
    int eventID;              ///< Unique event ID
    string eventName;         ///< Name of the event
    int totalSeats;           ///< Total number of seats available
    int availableSeats;       ///< Number of seats currently available

    Event() : eventID(0), totalSeats(0), availableSeats(0) {}

    /**
     * @brief Attempts to book a seat for the event.
     * @return True if booking is successful, otherwise false.
     */
    bool bookSeat() {
        if (availableSeats > 0) {
            availableSeats--;
            return true;
        }
        return false;
    }

    /**
     * @brief Attempts to cancel a seat for the event.
     * @return True if cancellation is successful, otherwise false.
     */
    bool cancelSeat() {
        if (availableSeats < totalSeats) {
            availableSeats++;
            return true;
        }
        return false;
    }
};

/**
 * @class Reservation
 * @brief Manages reservations made by users for events.
 */
class Reservation {
public:
    int reservationID;      ///< Unique reservation ID
    string userID;          ///< ID of the user who made the reservation
    int eventID;            ///< ID of the event for which the reservation was made

    Reservation() : reservationID(0), eventID(0) {}

    /**
     * @brief Generates a unique reservation ID by checking the last used ID.
     * @return A new unique reservation ID.
     */
    int generateReservationID() {
        ifstream reservationFile("reservations.txt");
        int lastID = 0;
        string line;

        // Get the last reservation ID from the file
        while (getline(reservationFile, line)) {
            stringstream ss(line);
            int idTemp;
            ss >> idTemp;
            lastID = idTemp;
        }

        reservationFile.close();
        return lastID + 1;
    }

    /**
     * @brief Cancels the reservation and frees up a seat in the event.
     * @return True if cancellation is successful, otherwise false.
     */
    bool cancelSeat() {
        Event event;
        event.eventID = eventID;
        return event.cancelSeat();
    }
};

/**
 * @class User
 * @brief Handles user operations such as registration, login, booking, and cancellation of tickets.
 */
class User {
public:
    string username;                ///< Stores the username of the user
    string password;                ///< Stores the password of the user
    vector<int> reservationIDs;     ///< Stores the list of reservation IDs associated with the user

    User() : username(""), password("") {}

    /**
     * @brief Constructor to initialize a User object.
     * @param user The username of the user.
     * @param pass The password of the user.
     */
    User(string user, string pass) : username(user), password(pass) {}

    /**
     * @brief Registers a new user by saving the username and hashed password to a file.
     */
    void registerUser() {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        // Check if the username is already taken
        if (!isUsernameAvailable(username)) {
            cout << "Username already taken. Please choose another one." << endl;
            return;
        }

        // Save the new user's data to the users.txt file
        ofstream userFile("users.txt", ios::app);
        if (userFile.is_open()) {
            userFile << username << " " << hashPassword(password) << endl;
            userFile.close();
            cout << "User registered successfully!" << endl;
        } else {
            cout << "Error: Unable to open file." << endl;
        }
    }

    /**
     * @brief Logs in a user by checking the entered credentials against the stored data.
     * @return True if login is successful, otherwise false.
     */
    bool loginUser() {
        string inputUsername, inputPassword;
        cout << "Enter username: ";
        cin >> inputUsername;
        cout << "Enter password: ";
        cin >> inputPassword;

        // Check the entered credentials against the stored data
        ifstream userFile("users.txt");
        string fileUsername, filePassword;
        while (userFile >> fileUsername >> filePassword) {
            if (fileUsername == inputUsername && filePassword == hashPassword(inputPassword)) {
                username = inputUsername;
                return true;
            }
        }

        cout << "Invalid username or password." << endl;
        return false;
    }

    /**
     * @brief Displays the list of reservations made by the user.
     */
    void viewTickets() const {
        if (reservationIDs.empty()) {
            cout << "No reservations found." << endl;
            return;
        }
        cout << "Your reservations:" << endl;
        for (int id : reservationIDs) {
            cout << "Reservation ID: " << id << endl;
        }
    }

    /**
     * @brief Books a ticket for the user by associating an event with the user.
     */
    void bookTicket() {
        int eventID;
        cout << "Enter event ID: ";
        cin >> eventID;

        Event event;
        event.eventID = eventID;
        if (event.bookSeat()) {
            Reservation reservation;
            reservation.reservationID = reservation.generateReservationID();
            reservation.userID = username;
            reservation.eventID = eventID;

            // Save the reservation details to the reservations.txt file
            ofstream reservationFile("reservations.txt", ios::app);
            reservationFile << reservation.reservationID << " " << reservation.userID << " " << reservation.eventID << endl;
            reservationFile.close();

            cout << "Ticket booked successfully!" << endl;
            reservationIDs.push_back(reservation.reservationID);
        } else {
            cout << "Event is fully booked or does not exist." << endl;
        }
    }

    /**
     * @brief Cancels a ticket by removing the reservation from the user's list.
     */
    void cancelTicket() {
        int reservationID;
        cout << "Enter reservation ID: ";
        cin >> reservationID;

        Reservation reservation;
        reservation.reservationID = reservationID;
        if (reservation.cancelSeat()) {
            auto it = find(reservationIDs.begin(), reservationIDs.end(), reservationID);
            if (it != reservationIDs.end()) {
                reservationIDs.erase(it);
            }
            cout << "Ticket cancelled successfully!" << endl;
        } else {
            cout << "Reservation does not exist." << endl;
        }
    }

private:
    /**
     * @brief Checks if the username is available by comparing it against stored usernames.
     * @param user The username to check.
     * @return True if the username is available, otherwise false.
     */
    bool isUsernameAvailable(const string& user) {
        ifstream userFile("users.txt");
        string fileUsername, filePassword;
        while (userFile >> fileUsername >> filePassword) {
            if (fileUsername == user) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Hashes the user's password for secure storage.
     * @param pass The password to hash.
     * @return The hashed password.
     */
    string hashPassword(const string& pass) {
        string hash = "";
        for (char c : pass) {
            hash += to_string(int(c) * 5); // Simple hash function (for demonstration purposes)
        }
        return hash;
    }
};

/**
 * @class Admin
 * @brief Handles admin operations such as adding, viewing, and removing events.
 */
class Admin {
public:
    string adminUsername;   ///< Admin username for login
    string adminPassword;   ///< Admin password for login

    Admin() : adminUsername(""), adminPassword("") {}

    /**
     * @brief Constructor to initialize an Admin object.
     * @param user The admin username.
     * @param pass The admin password.
     */
    Admin(string user, string pass) : adminUsername(user), adminPassword(pass) {}

    /**
     * @brief Logs in the admin by checking credentials.
     * @return True if login is successful, otherwise false.
     */
    bool loginAdmin() {
        string inputUsername, inputPassword;
        cout << "Enter admin username: ";
        cin >> inputUsername;
        cout << "Enter admin password: ";
        cin >> inputPassword;

        // Check credentials
        if (inputUsername == adminUsername && inputPassword == adminPassword) {
            return true;
        }
        cout << "Invalid admin credentials." << endl;
        return false;
    }

    /**
     * @brief Adds a new event to the system.
     */
    void addEvent() {
        Event event;
        cout << "Enter event name: ";
        cin >> event.eventName;
        cout << "Enter total seats: ";
        cin >> event.totalSeats;
        event.availableSeats = event.totalSeats;
        event.eventID = generateEventID();

        // Save the event details to the events.txt file
        ofstream eventFile("events.txt", ios::app);
        eventFile << event.eventID << " " << event.eventName << " " << event.totalSeats << " " << event.availableSeats << endl;
        eventFile.close();

        cout << "Event added successfully!" << endl;
    }

private:
    /**
     * @brief Generates a unique event ID.
     * @return A new unique event ID.
     */
    int generateEventID() {
        ifstream eventFile("events.txt");
        int lastID = 0;
        string line;

        // Get the last event ID from the file
        while (getline(eventFile, line)) {
            stringstream ss(line);
            int idTemp;
            ss >> idTemp;
            lastID = idTemp;
        }

        eventFile.close();
        return lastID + 1;
    }
};

int main() {
    // Example usage
    User user("exampleUser", "examplePass");
    Admin admin("adminUser", "adminPass");

    // Registration and login example
    user.registerUser();
    if (user.loginUser()) {
        cout << "Login successful!" << endl;
    }

    // Admin adding an event example
    if (admin.loginAdmin()) {
        admin.addEvent();
    }

    // Booking and viewing tickets
    user.bookTicket();
    user.viewTickets();

    return 0;
}
