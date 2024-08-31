Ticket Reservation System
This project implements a simple ticket reservation system using C++. The system allows users and admins to perform various operations related to event management and ticket booking. It includes functionality for user registration, login, booking, and cancelling tickets, as well as admin capabilities for adding and removing events.

Features
User Operations:

Register User: Create a new user account with a unique username and password.
Login User: Authenticate users by checking their credentials against stored data.
Book Ticket: Book a ticket for an event if seats are available.
Cancel Ticket: Cancel a reservation and free up a seat for the event.
View Tickets: Display a list of tickets reserved by the user.
Admin Operations:

Login Admin: Authenticate admin credentials for managing events.
Add Event: Create a new event with a specified number of seats and a unique event ID.
Remove Event: Delete an existing event from the system.
Classes
Event: Manages event details and seat availability. Provides methods for booking and cancelling seats.
Reservation: Handles reservations made by users. Provides methods for generating unique reservation IDs and managing seat cancellations.
User: Handles user-specific operations including registration, login, booking, and cancellation of tickets.
Admin: Manages admin-specific operations including event management (adding and removing events.

File Structure
users.txt: Stores user data (username and hashed password).
events.txt: Stores event data (event ID, name, total seats, available seats).
reservations.txt: Stores reservation data (reservation ID, user ID, event ID).
Compilation and Execution
Compile: Use a C++ compiler like g++ to compile the source code:

g++ main.cpp -o main

Run: Execute the compiled program:
./main
