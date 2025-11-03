/*
COMSC-210 | Lab 28 | Rylan Der
IDE Used: Visual Studio Code
*/

#include <iostream>
#include <numeric>
#include <algorithm>
#include <string>
#include <fstream>
#include <iomanip>
#include <list>
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25;
const int STREAM_IGNORE_CHARS = 100;

int select_goat(list<Goat> trip);
void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, string [], string []);
void display_trip(list<Goat> trip);
int main_menu();

//add new goats by creating a new list and merging with existing list, using std::merge
void add_multiple(list<Goat> &trip, string names[], string colors[]);

//find total age of all goats usign std::accumulate
void total_age(const list<Goat> &trip);

//checks if there is a goat older than the specified age
void exists_older_than(const list<Goat> &trip);

//finds a goat with the specified name, using std::find_if
void find_name(const list<Goat> &trip);

//changes colors of all goats using std::for_each
void dye_goats(list<Goat> &trip);

//increments ages of all goats using std::transform
void age_goats(list<Goat> &trip);

//removes all goats above specified age from the trip using std::remove_if
void retire_goats(list<Goat> &trip);

//removes all goats with duplicated names using std::unique
void unique_names(list<Goat> &trip);

int validate_input(int min, int max, string prompt);
int validate_input(int min, string prompt);

int main() {
    srand(time(0));
    bool again;

    // read & populate arrays for names and colors
    ifstream fin("names.txt");
    string names[SZ_NAMES];
    int i = 0;
    while (fin >> names[i++]);
    fin.close();
    ifstream fin1("colors.txt");
    string colors[SZ_COLORS];
    i = 0;
    while (fin1 >> colors[i++]);
    fin1.close();

    // create & populate a trip of Goats using std::list of random size 8-15
    int tripSize = rand() % 8 + 8;
    list<Goat> trip;
    int age;
    string name, color;
    for (int i = 0; i < tripSize; i++) {
        age = rand() % MAX_AGE;  // defined in Goat.h
        name = names[rand() % SZ_NAMES];
        color = colors[rand() % SZ_COLORS];
        Goat tmp(name, age, color);
        trip.push_back(tmp);
    }

   // sort(trip.begin(), trip.end());
    
    enum MENU_OPTIONS {ADD = 1, REMOVE = 2, DISPLAY = 3, ADD_MULTIPLE = 4, ADD_AGE = 5, EXISTS_OLDER = 6, FIND = 7, DYE = 8, AGE = 9, RETIRE = 10, UNIQUE_NAMES = 11, EXIT = 12};
    // Goat Manager 3001 Engine
    int sel = main_menu();
    while (sel != EXIT) {
        switch (sel) {
            case ADD:
                cout << "Adding a goat.\n";
                add_goat(trip, names, colors);
                break;
            case REMOVE:    
                cout << "Removing a goat.\n";
                delete_goat(trip);
                break;
            case DISPLAY:    
                cout << "Displaying goat data.\n";
                display_trip(trip);
                break;
            case ADD_MULTIPLE:
                cout << "Adding new goats to the trip.\n";
                add_multiple(trip, names, colors);
                break;
            case ADD_AGE:
                cout << "Adding ages of all goats. \n";
                total_age(trip);
                break;
            case EXISTS_OLDER:
                cout << "Seeing if a goat above a specified age exists. \n";
                exists_older_than(trip);
                break;
            case FIND:
                cout << "Finding goat of a specify name.\n";
                //find_name(trip);
                break;
            case DYE:
                cout << "Recoloring all goats. \n";
                //dye_goats(trip);
                break;
            case AGE:
                cout << "Incrementing age of all goats. \n";
                //age_goats(trip);
                break;
            case RETIRE:
                cout << "Removing goats above specified age from the trip. \n";
                //retire_goats(trip);
                break;
            case UNIQUE_NAMES:
                cout << "Removing all goats with duplicate names. \n";
                //unique_names(trip);
                break;
            default:
                cout << "Invalid selection.\n";
                break;
        }
        trip.sort();
        cout << endl;
        sel = main_menu();
    }

    return 0;
}

int main_menu() {
    cout << "*** GOAT MANAGER 3001 ***\n";
    cout << "[1] Add a goat\n";
    cout << "[2] Delete a goat\n";
    cout << "[3] List goats\n";
    cout << "[4] Add a number of random goats\n";
    cout << "[5] Find total age of all goats\n";
    cout << "[6] See whether there is a goat older than a given age\n";
    cout << "[7] Find information of a goat\n";
    cout << "[8] Recolor all goats\n";
    cout << "[9] Age all goats by one year\n";
    cout << "[10] Retire goats above a certain age\n";
    cout << "[11] Remove goats with duplicated names\n";
    cout << "[12] Quit\n";
    cout << "Choice --> ";
    int choice;
    cin >> choice;
    while (choice < 1 || choice > 12) {
        cout << "Invalid, again --> ";
        cin >> choice;
    }
    return choice;
}

void delete_goat(list<Goat> &trip) {
    cout << "DELETE A GOAT\n";
    int index = select_goat(trip);
    auto it = trip.begin();
    advance(it, index-1);
    trip.erase(it);
    cout << "Goat deleted. New trip size: " << trip.size() << endl;
}

void add_goat(list<Goat> &trip, string nms[], string cls[]) {
    cout << "ADD A GOAT\n";
    int age = rand() % MAX_AGE;
    string nm = nms[rand() % SZ_NAMES];
    string cl = cls[rand() % SZ_COLORS];
    Goat tmp(nm, age, cl);
    trip.push_back(tmp);
    cout << "Goat added. New trip size: " << trip.size() << endl;
}

void display_trip(list<Goat> trp) {
    int i = 1;
    for (auto gt: trp)
        cout << "\t" 
             << "[" << i++ << "] "
             << gt.get_name() 
             << " (" << gt.get_age() 
             << ", " << gt.get_color() << ")\n";
}

int select_goat(list<Goat> trp) {
    int input;
    cout << "Make a selection:\n";
    display_trip(trp);
    cout << "Choice --> ";
    cin >> input;
    while (input < 1 || input > trp.size()) {
        cout << "Invalid choice, again --> ";
        cin >> input;
    }
    return input;
}

/**
 * Retrieves an integer in specified range from console input
 * @param prompt String to display before prompting input
 * @param min Minimum range of integers to accept (inclusive)
 * @param max Maximum range of integers to accept (inclusive)
 * @return Integer in the range of min, max inclusive
 */
int validate_input(int min, int max, string prompt) {
    int input;
    do {
        cout << prompt << endl;
        cout << " < ";
        cin >> input;

        if (cin.fail()) {
            cout << "Input must be an integer" << endl;
            input = min - 1;
        } else if (input < min || input > max) {
            cout << "Input must be between " << min << " and " << max << endl;
        }
        cin.clear();
        cin.ignore(STREAM_IGNORE_CHARS, '\n');
    } while (input < min || input > max);
    return input;
}

/**
 * Retrieves an integer above the specified minimum from console input
 * @param prompt String to display before prompting input
 * @param min Minimum range of integers to accept (inclusive)
 * @return Integer above the minimum (inclusive)
 */
int validate_input(int min, string prompt) {
    int input;
    do {
        cout << prompt << endl;
        cout << " > ";
        cin >> input;

        if (cin.fail()) {
            cout << "Input must be an integer" << endl;
            input = min - 1;
        } else if (input < min) {
            cout << "Input must be greater than " << min << endl;
        }
        cin.clear();
        cin.ignore(STREAM_IGNORE_CHARS, '\n');
    } while (input < min);
    return input;
}

/**
 * Add new goats by creating a new list and merging with existing list, using std::merge
 * @param trip List of goats to add to
 * @param names Array of names to retrieve new goat names from
 * @param colors Array of colors to retrieve new goat colors from
 */
void add_multiple(list<Goat> &trip, string names[], string colors[]) {
    //retrieve number of goats from input
    int num = validate_input(1, "Enter the number of goats to add:");

    //fill a new list of goats
    list<Goat> newGoats;
    int age;
    string name, color;
    for (int i = 0; i < num; i++) {
        age = rand() % MAX_AGE;
        name = names[rand() % SZ_NAMES];
        color = colors[rand() % SZ_COLORS];
        newGoats.push_back(Goat(name, age, color));
    }

    //sort new list and existing list
    trip.sort();
    newGoats.sort();

    //merge both lists into a new temp list
    list<Goat> mergedTrip(trip.size() + newGoats.size());
    merge(trip.begin(), trip.end(), newGoats.begin(), newGoats.end(), mergedTrip.begin());

    //copy the new list to the existing list
    trip = mergedTrip;
}

/**
 * Sums the ages of all goats using std::accumulate
 * @param trip List of goats
 */
void total_age(const list<Goat> &trip) {
    //uses accumulate with lambda function that adds age to an accumulator
    cout << "Total age of all goats: " << accumulate(trip.begin(), trip.end(), 0, [](int a, Goat goat) { return a + goat.get_age(); }) << endl;
}

/**
 * Checks if there is a goat older than an age received from input (exclusive)
 * @param trip List of goats
 */
void exists_older_than(const list<Goat> &trip) {
    //retrieves age from input as a nonnegative number
    int age = validate_input(0, "Enter age to search for:");
    
    //calls any_of to with function that checks if a goat's age is above (not equal to) input age
    bool exists = any_of(trip.begin(), trip.end(), [age](Goat goat) { return goat.get_age() > age; });

    //outputs message accordingly if a goat is/ is not found
    if (exists) {
        cout << "There is a goat older than " << age << " years old (exclusive)" << endl;
    } else {
        cout << "There is not a goat older than " << age << " years old (exclusive)" << endl;
    }
}

//finds a goat with the specified name, using std::find_if
//Goat find_name(const list<Goat> &trip, string name);

//changes colors of all goats using std::for_each
//void dye_goats(list<Goat> &trip);

//increments ages of all goats using std::transform
//void age_goats(list<Goat> &trip);

//removes all goats above specified age from the trip using std::remove_if
//void retire_goats(list<Goat> &trip, int age);

//removes all goats with duplicated names using std::unique
//void unique_names(list<Goat> &trip);