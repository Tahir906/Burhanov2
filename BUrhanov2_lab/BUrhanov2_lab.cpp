#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Data structures
struct Pipe {
    int id;
    string name;
    double length;
    int diameter;
    bool inRepair;
};

struct CompressorStation {
    int id;
    string name;
    int totalWorkshops;
    int workingWorkshops;
};

// Global variables
vector<Pipe> pipes;
vector<CompressorStation> stations;
int nextPipeId = 1;
int nextStationId = 1;

// Input validation functions
int inputPositiveNumber(const string& prompt) {
    int number;
    while (true) {
        cout << prompt;
        if (cin >> number && number > 0) {
            return number;
        }
        else {
            cout << "Error! Please enter a positive number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

double inputPositiveDouble(const string& prompt) {
    double number;
    while (true) {
        cout << prompt;
        if (cin >> number && number > 0) {
            return number;
        }
        else {
            cout << "Error! Please enter a positive number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

bool inputYesNo(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && (value == 0 || value == 1)) {
            return value;
        }
        else {
            cout << "Error! Please enter 0 or 1: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// Pipe functions
void addPipe() {
    Pipe p;
    p.id = nextPipeId++;

    cout << "Pipe name: ";
    getline(cin >> ws, p.name);

    p.length = inputPositiveDouble("Length: ");
    p.diameter = inputPositiveNumber("Diameter: ");
    p.inRepair = inputYesNo("In repair (0-no, 1-yes): ");

    pipes.push_back(p);
    cout << "Pipe added (ID: " << p.id << ")\n";
}

void editPipeRepair(int id, bool repairStatus) {
    for (auto& pipe : pipes) {
        if (pipe.id == id) {
            pipe.inRepair = repairStatus;
            cout << "Pipe " << id << " updated\n";
            return;
        }
    }
    cout << "Pipe not found\n";
}

// Station functions
void addStation() {
    CompressorStation cs;
    cs.id = nextStationId++;

    cout << "Station name: ";
    getline(cin >> ws, cs.name);

    cs.totalWorkshops = inputPositiveNumber("Total workshops: ");


    // Validate working workshops
    while (true) {
        cs.workingWorkshops = inputPositiveNumber("Working workshops: ");
        if (cs.workingWorkshops <= cs.totalWorkshops) {
            break;
        }
        cout << "Error! Working workshops cannot exceed total workshops\n";
    }

    stations.push_back(cs);
    cout << "Station added (ID: " << cs.id << ")\n";
}

// Search functions
void findPipes() {
    cout << "Search by:\n1 - name\n2 - repair status\nChoice: ";
    int choice = inputPositiveNumber("");

    if (choice == 1) {
        string name;
        cout << "Name: ";
        getline(cin >> ws, name);

        bool found = false;
        for (const auto& pipe : pipes) {
            if (pipe.name.find(name) != string::npos) {
                cout << "ID: " << pipe.id << " | " << pipe.name << " | Length: " << pipe.length
                    << " | Repair: " << (pipe.inRepair ? "yes" : "no") << endl;
                found = true;
            }
        }
        if (!found) cout << "No pipes found\n";
    }
    else if (choice == 2) {
        bool status = inputYesNo("Status (0-working, 1-under repair): ");

        bool found = false;
        for (const auto& pipe : pipes) {
            if (pipe.inRepair == status) {
                cout << "ID: " << pipe.id << " | " << pipe.name << " | Length: " << pipe.length << endl;
                found = true;
            }
        }
        if (!found) cout << "No pipes found\n";
    }
    else {
        cout << "Invalid choice\n";
    }
}

void findStations() {
    cout << "Search by:\n1 - name\n2 - unused workshops percentage\nChoice: ";
    int choice = inputPositiveNumber("");
    if (choice == 1) {
        string name;
        cout << "Name: ";
        getline(cin >> ws, name);

        bool found = false;
        for (const auto& station : stations) {
            if (station.name.find(name) != string::npos) {
                double unusedPercent = (1.0 - (double)station.workingWorkshops / station.totalWorkshops) * 100;
                cout << "ID: " << station.id << " | " << station.name
                    << " | Unused: " << unusedPercent << "%" << endl;
                found = true;
            }
        }
        if (!found) cout << "No stations found\n";
    }
    else if (choice == 2) {
        double minPercent;
        while (true) {
            minPercent = inputPositiveDouble("Minimum unused percentage: ");
            if (minPercent >= 0) break;
            cout << "Error! Percentage cannot be negative\n";
        }

        bool found = false;
        for (const auto& station : stations) {
            double unusedPercent = (1.0 - (double)station.workingWorkshops / station.totalWorkshops) * 100;
            if (unusedPercent >= minPercent) {
                cout << "ID: " << station.id << " | " << station.name
                    << " | Unused: " << unusedPercent << "%" << endl;
                found = true;
            }
        }
        if (!found) cout << "No stations found\n";
    }
    else {
        cout << "Invalid choice\n";
    }
}

// Batch editing
void batchEditPipes() {
    vector<int> foundIds;

    cout << "Search criteria:\n1 - by name\n2 - by status\nChoice: ";
    int choice = inputPositiveNumber("");

    if (choice == 1) {
        string name;
        cout << "Name: ";
        getline(cin >> ws, name);

        for (const auto& pipe : pipes) {
            if (pipe.name.find(name) != string::npos) {
                foundIds.push_back(pipe.id);
            }
        }
    }
    else if (choice == 2) {
        bool status = inputYesNo("Status (0-working, 1-under repair): ");

        for (const auto& pipe : pipes) {
            if (pipe.inRepair == status) {
                foundIds.push_back(pipe.id);
            }
        }
    }
    else {
        cout << "Invalid choice\n";
        return;
    }

    if (!foundIds.empty()) {
        cout << "Found pipes: " << foundIds.size() << endl;
        bool newStatus = inputYesNo("Change repair status to (0-working, 1-under repair): ");

        for (int id : foundIds) {
            editPipeRepair(id, newStatus);
        }
        cout << "Batch editing completed\n";
    }
    else {
        cout << "No pipes found\n";
    }
}

// View all objects
void viewAll() {
    cout << "\n=== PIPES ===\n";
    if (pipes.empty()) {
        cout << "No pipes\n";
    }
    else {
        for (const auto& pipe : pipes) {
            cout << "ID: " << pipe.id << " | " << pipe.name << " | Length: " << pipe.length
                << " | Repair: " << (pipe.inRepair ? "yes" : "no") << endl;
        }
    }

    cout << "\n=== COMPRESSOR STATIONS ===\n";
    if (stations.empty()) {
        cout << "No stations\n";
    }
    else {
        for (const auto& station : stations) {
            double unusedPercent = (1.0 - (double)station.workingWorkshops / station.totalWorkshops) * 100;
            cout << "ID: " << station.id << " | " << station.name
                << " | Workshops: " << station.workingWorkshops << "/" << station.totalWorkshops
                << " | Unused: " << unusedPercent << "%" << endl;
        }
    }
}

// File operations
void saveToFile() {
    string filename;
    cout << "Filename: ";
    cin >> filename;

    ofstream file(filename);
    if (file.is_open()) {
        // Save pipes
        file << "PIPES " << pipes.size() << endl;
        for (const auto& pipe : pipes) {
            file << pipe.id << " " << pipe.name << " " << pipe.length << " "
                << pipe.diameter << " " << pipe.inRepair << endl;
        }
        // Save stations
        file << "STATIONS " << stations.size() << endl;
        for (const auto& station : stations) {
            file << station.id << " " << station.name << " " << station.totalWorkshops
                << " " << station.workingWorkshops << endl;
        }

        file.close();
        cout << "Data saved\n";
    }
    else {
        cout << "Error saving file\n";
    }
}

void loadFromFile() {
    string filename;
    cout << "Filename: ";
    cin >> filename;

    ifstream file(filename);
    if (file.is_open()) {
        pipes.clear();
        stations.clear();

        string type;
        int count;

        // Load pipes
        file >> type >> count;
        for (int i = 0; i < count; i++) {
            Pipe p;
            file >> p.id >> p.name >> p.length >> p.diameter >> p.inRepair;
            pipes.push_back(p);
            if (p.id >= nextPipeId) nextPipeId = p.id + 1;
        }

        // Load stations
        file >> type >> count;
        for (int i = 0; i < count; i++) {
            CompressorStation cs;
            file >> cs.id >> cs.name >> cs.totalWorkshops >> cs.workingWorkshops;
            stations.push_back(cs);
            if (cs.id >= nextStationId) nextStationId = cs.id + 1;
        }

        file.close();
        cout << "Data loaded\n";
    }
    else {
        cout << "Error loading file\n";
    }
}

// Menu
void showMenu() {
    cout << "\n=== MANAGEMENT SYSTEM ===\n";
    cout << "1. Add pipe\n";
    cout << "2. Add station\n";
    cout << "3. View all\n";
    cout << "4. Find pipes\n";
    cout << "5. Find stations\n";
    cout << "6. Batch edit pipes\n";
    cout << "7. Save\n";
    cout << "8. Load\n";
    cout << "9. Exit\n";
    cout << "Choice: ";
}

int main() {
    int choice;

    do {
        showMenu();
        choice = inputPositiveNumber("");

        switch (choice) {
        case 1: addPipe(); break;
        case 2: addStation(); break;
        case 3: viewAll(); break;
        case 4: findPipes(); break;
        case 5: findStations(); break;
        case 6: batchEditPipes(); break;
        case 7: saveToFile(); break;
        case 8: loadFromFile(); break;
        case 9: cout << "Exit\n"; break;
        default: cout << "Invalid choice\n";
        }
    } while (choice != 0);

    return 0;
}