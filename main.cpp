//
// Created by Joe Coon and Ethan Edwards on 3/11/2023.
//

#include <iostream>
#include <fstream>

using namespace std;

// Globals

// Function Prototypes
void initFiles(ifstream& infile, ofstream& outfile, int argc, char* argv[]);
int chadgptAlgo(int r, int x, int y, int n, ifstream& in);

// Main
int main(int argc, char* argv[]) {
    // Variable defs
    ifstream infile; ofstream outfile;
    string line;
    int rad, centerX, centerY, numLines;
    // Open files
    initFiles(infile, outfile, argc, argv);
    // File read
    while (!infile.eof()){
        getline(infile, line);
        rad = stoi(line.substr(0, line.find(' ')));
        line.erase(0, line.find(' ') + 1);
        centerX = stoi(line.substr(0, line.find(' ')));
        line.erase(0, line.find(' ') + 1);
        centerY = stoi(line.substr(0, line.find(' ')));
        line.erase(0, line.find(' ') + 1);
        numLines = stoi(line.substr(0, line.size()));
        cout << chadgptAlgo(rad, centerX, centerY, numLines, infile) << "\n";
        getline(infile, line); // Read the newline character
    }
    // Cleanup before program end
    infile.close(); outfile.close();
    return 0;
}

// ChatGPT's implementation of the problem :: TODO: REMOVE THIS AND WRITE OUR OWN ALGORITHM
#include <cmath>
int chadgptAlgo(int r, int x, int y, int n, ifstream& in){
    if(r == 0 && x == 0 && y == 0 && n == 0) {
        return -1; // exit if input is all 0s
    }
    int cnt = 1; // initialize counter to 1 (for the circle itself)
    for(int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        in >> x1 >> y1 >> x2 >> y2;
        // check if line intersects with circle
        double dx = x2 - x1;
        double dy = y2 - y1;
        double a = dx*dx + dy*dy;
        double b = 2*dx*(x1-x) + 2*dy*(y1-y);
        double c = x*x + y*y + x1*x1 + y1*y1 - 2*(x*x1 + y*y1) - r*r;
        double det = b*b - 4*a*c;
        if(det >= 0) { // line intersects with circle
            double t1 = (-b - sqrt(det)) / (2*a);
            double t2 = (-b + sqrt(det)) / (2*a);
            if(t1 <= 1 && t1 >= 0 || t2 <= 1 && t2 >= 0) {
                cnt++; // increment counter if line intersects with circle
            }
        }
    }
    return cnt;
}

/*
 *  This function receives user arguments and assigns them to the ifstream and ofstream objects which manage I/O
 *  for the program. If the user supplies no arguments, this function tries to resolve the issue.
 *
 *  If the user has no input file available to pass, then they must enter ## when prompted to exit the program.
 */
void initFiles(ifstream& infile, ofstream& outfile, int argc, char* argv[]){
    char confirm = 'n';
    string fname;
    // Check for inputs
    if (argc == 3){
        infile.open(argv[1]);
        outfile.open(argv[2]);
    }
    else if (argc == 2) {
        fname = argv[1];
        cout << "Found " << fname << " for input file. Continue using " << fname << " as input? (y/n) ";
        cin >> confirm;
        if (confirm == 'n') {
            cout << "Enter the new input filename: \n";
            cin >> fname;
        }
        infile.open(fname);
    }
    else {
        cout << "No input or output filename entered. Please run the program as ./ShipPather.out <input file> <output file>\n";
        exit( EXIT_FAILURE);
    }

    // Confirm that the files are actually open & ask for reentry if not valid
    while (!outfile.is_open()){
        cout << "Please provide an existing filename to overwrite, or a new filename to create: ";
        cin >> fname;
        outfile = ofstream(fname, ios::out);
    }
    while (!infile.is_open() && fname != "##"){
        cout << "No input file found. Please provide a relative path and filename for an input file or enter ## to close the program: ";
        cin >> fname;
        infile.open(fname);
    }
}
