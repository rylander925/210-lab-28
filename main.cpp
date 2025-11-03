/*
COMSC-210 | Lab 28 | Rylan Der
IDE Used: Visual Studio Code
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25;

int select_goat(list<Goat> trip);
void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, string [], string []);
void display_trip(list<Goat> trip);
int main_menu();

//add new goats by creating a new list and merging with existing list, using std::merge
void add_multiple(list<Goat> &trip, int num);

//sort goats by age using std::sort
void sort_age(list<Goat> &trip);

//checks if there is a goat older than the specified age
bool exists_older_than(const list<Goat> &trip, int age);

//finds a goat with the specified name, using std::find_if
Goat find_name(const list<Goat> &trip, string name);

//changes colors of all goats using std::for_each
void dye_goats(list<Goat> &trip);

//increments ages of all goats using std::transform
void age(list<Goat> &trip);

//removes all goats above specified age from the trip using std::remove_if
void retire_goats(list<Goat> &trip, int age);

//removes all goats with duplicated names using std::unique
void unique_names(list<Goat> &trip);

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
    
    enum MENU_OPTIONS {ADD = 1, REMOVE = 2, DISPLAY = 3, ADD_MULTIPLE = 4, SORT_AGE = 5, EXISTS_OLDER = 6, FIND = 7, DYE = 8, AGE = 9, RETIRE = 10, UNIQUE_NAMES = 11, EXIT = 12};
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
                add_multiple(trip);
                break;
            case SORT_AGE:
                cout << "Sorting goats by age. \n";
                sort_age(trip);
                break;
            case EXISTS_OLDER:
                exists_older_than(trip);
                break;
            case FIND:
                find_name(trip);
                break;
            case DYE:
                dye_goats(trip);
            default:
                cout << "Invalid selection.\n";
                break;
        }
        sel = main_menu();
    }
    

    return 0;
}

int main_menu() {
    cout << "*** GOAT MANAGER 3001 ***\n";
    cout << "[1] Add a goat\n";
    cout << "[2] Delete a goat\n";
    cout << "[3] List goats\n";
    cout << "[4] Quit\n";
    cout << "Choice --> ";
    int choice;
    cin >> choice;
    while (choice < 1 || choice > 4) {
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