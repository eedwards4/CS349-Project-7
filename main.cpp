//
// Created by Joe Coon and Ethan Edwards on 3/11/2023.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

// Globals

// Templates
// template<vector<typename>> lineReader(int numLines, ); TODO: Learn how to template this

// Function Prototypes
void initFiles(ifstream& infile, ofstream& outfile, int argc, char* argv[]);
vector<int> lineReaderInt(int numInputs, ifstream& in);
bool checkForIntersect(int r, int centerX, int centerY, int x1, int y1, int x2, int y2);
bool checkForLineIntersect(int firstX1, int firstY1, int firstX2, int firstY2, // First line
                           int secondX1, int secondY1, int secondX2, int secondY2, int rad, int cx, int cy); // Second line

// Main
int main(int argc, char* argv[]) {
    // Variable defs
    ifstream infile; ofstream outfile;
    string line;
    int numIntersects = 1, numIntersectsTmp = 0;
    vector<int> CI, LI; // case inputs; line inputs
    vector<vector<int>> lineStorage;
    // Open files
    initFiles(infile, outfile, argc, argv);
    // File read
    while (!infile.eof()){
        // Get each line of the initial input
        CI = lineReaderInt(4, infile); // Input order: rad, center x, center y, number of lines
        for (int i : CI)
            cout << i << " ";
        cout << "\n";

        // Loop through each line to check against the given circle
        for (int i = 0; i < CI.at(3); i++) {
            // Get the line coordinates
            LI = lineReaderInt(4, infile); // Input order: x1, y1, x2, y2
            for (int j : LI)
                cout << j << " ";
            cout << "\n";
            // Check for intersection (w/ circle)
            if (checkForIntersect(CI[0], CI[1], CI[2], LI[0], LI[1], LI[2], LI[3])){
                lineStorage.push_back(LI); // Store line coordinates for intersection checking
                numIntersectsTmp++;
            }
            // Check for intersection (w/ other lines)
            for (auto & j : lineStorage){
                if (checkForLineIntersect(LI[0], LI[1], LI[2], LI[3], j[0], j[1], j[2], j[3], CI[0], CI[1], CI[2])){
                    numIntersectsTmp *= 2;
                }
            }
            numIntersects += numIntersectsTmp;
            numIntersectsTmp = 0;
            LI.clear(); // Just in case
        }
        if (!(CI[0] == 0 && CI[1] == 0 && CI[2] == 0 && CI[3] == 0)){
            outfile << numIntersects << "\n";
        }
        CI.clear(); // Just in case
        numIntersects = 1;
        lineStorage.clear();
    }
    // Cleanup before program end
    infile.close(); outfile.close();
    return 0;
}

// Check if lines intersect with circle
bool checkForIntersect(int r, int centerX, int centerY, int x1, int y1, int x2, int y2){
    float area2 = abs((x2 - x1) * (centerY - y1) - (centerX - x1) * (y2 - y1)); // Triangle area * 2
    float dist = area2 / sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); // Distance as a function of area * 2 / segment length
    if (dist <= r) // Check if distance is smaller than radius
        return true;
    return false;
}

// Calculate the distance between two points
float getDist(int x1, int y1, int x2, int y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Check if lines intersect with each other
bool checkForLineIntersect(int firstX1, int firstY1, int firstX2, int firstY2, int secondX1, int secondY1, int secondX2, int secondY2, int rad, int cx, int cy){
    // https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
    // check if lines are parallel
    if ((firstX1 - firstX2) * (secondY1 - secondY2) == (firstY1 - firstY2) * (secondX1 - secondX2)) {
        return false;
    }
    // check if lines are colinear
    if ((firstX1 - firstX2) * (secondY1 - firstY1) == (firstY1 - firstY2) * (secondX1 - firstX1)) {
        return false;
    }
    // check if lines intersect
    if ((firstX1 - firstX2) * (secondY1 - firstY1) - (firstY1 - firstY2) * (secondX1 - firstX1) < 0) {
        pair<int, int> intersect;
        intersect.first = ((firstX1 * firstY2 - firstY1 * firstX2) * (secondX1 - secondX2) - (firstX1 - firstX2) * (secondX1 * secondY2 - secondY1 * secondX2)) / ((firstX1 - firstX2) * (secondY1 - secondY2) - (firstY1 - firstY2) * (secondX1 - secondX2));
        intersect.second = ((firstX1 * firstY2 - firstY1 * firstX2) * (secondY1 - secondY2) - (firstY1 - firstY2) * (secondX1 * secondY2 - secondY1 * secondX2)) / ((firstX1 - firstX2) * (secondY1 - secondY2) - (firstY1 - firstY2) * (secondX1 - secondX2));
        if (getDist(intersect.first, intersect.second, cx, cy) <= rad){
            return true;
        }
    }
    return false; // If none of the above conditions are met, the lines do not intersect
}

// ChatGPT's implementation of the problem :: TODO: REMOVE THIS AND WRITE OUR OWN ALGORITHM
int chadgptAlgo(int radius, int centerX, int centerY, int numLines, ifstream& in) {
    if(radius == 0 && centerX == 0 && centerY == 0 && numLines == 0) {
        return -1; // exit if input is all 0s
    }
    int cnt = 1; // initialize counter to 1 (for the circle itself)
    for(int i = 0; i < numLines; i++) {
        int x1, y1, x2, y2;
        in >> x1 >> y1 >> x2 >> y2;
        // check if line intersects with circle
        double a, b, c;
        double determinant = b * b - 4 * a * c;
        if(determinant >= 0) { // the roots are not imaginary; from quadratic formula (completed below)
            double rootA = (-b - sqrt(determinant)) / (2 * a);
            double rootB = (-b + sqrt(determinant)) / (2 * a);
            if(rootA <= 1 && rootA >= 0 || rootB <= 1 && rootB >= 0) {
                cnt++; // increment counter if line intersects with circle
            }
        }
    }
    return cnt;
}

// Read a line of variable length/inputs and return a vector of ints
vector<int> lineReaderInt(int numInputs, ifstream& in){
    vector<int> inputs;
    string line;
    getline(in, line);
    for (int i = 0; i < numInputs; i++) {
        if (line.size() == 1) inputs.push_back(stoi(line.substr(0, line.size())));
        inputs.push_back(stoi(line.substr(0, line.find(' '))));
        line.erase(0, line.find(' ') + 1);
    }
    return inputs;
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
