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
                           int secondX1, int secondY1, int secondX2, int secondY2); // Second line

// Main
int main(int argc, char* argv[]) {
    // Variable defs
    ifstream infile; ofstream outfile;
    string line;
    int numIntersects = 0, numIntersectsTmp = 0;
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
            lineStorage.push_back(LI); // Store line coordinates for intersection checking
            for (int j : LI)
                cout << j << " ";
            cout << "\n";
            // Check for intersection (w/ circle)
            if (checkForIntersect(CI[0], CI[1], CI[2], LI[0], LI[1], LI[2], LI[3])){
                numIntersectsTmp++;
            }
            // Check for intersection (w/ other lines)
            for (int j = 0; j < lineStorage.size(); j++){
                if (checkForLineIntersect(LI[0], LI[1], LI[2], LI[3], lineStorage[j][0], lineStorage[j][1], lineStorage[j][2], lineStorage[j][3])){
                    numIntersectsTmp *= 2;
                }
            }
            numIntersects += numIntersectsTmp;
            numIntersectsTmp = 0;
            LI.clear(); // Just in case
        }
        outfile << numIntersects << "\n";
        CI.clear(); // Just in case
        numIntersects = 0;
        lineStorage.clear();
    }
    // Cleanup before program end
    infile.close(); outfile.close();
    return 0;
}

// Vector manipulation functions
pair<int, int> vAdd(pair<int, int> v1, pair<int, int> v2){ // Add
    pair<int, int> returnPair;
    returnPair.first = v1.first + v2.first;
    returnPair.second = v1.second + v2.second;
    return returnPair;
}
pair<int, int> vSub(pair<int, int> v1, pair<int, int> v2){ // Subtract
    pair<int, int> returnPair;
    returnPair.first = v1.first - v2.first;
    returnPair.second = v1.second - v2.second;
    return returnPair;
}
float dot(pair<int, int> v1, pair<int, int> v2){ // Dot product
    return (v1.first * v2.first) + (v1.second * v2.second);
}
float hypot2(pair<int, int> v1, pair<int, int> v2){ // Hypotenuse
    return dot(vSub(v1, v2), vSub(v1, v2));
}
pair<int, int> proj(pair<int, int> v1, pair<int, int> v2){ // Projection
    float k = dot(v1, v2) / dot(v2, v2);
    return make_pair(k * v2.first, k * v2.second);
}

// Check if lines intersect with circle
bool checkForIntersect(int r, int centerX, int centerY, int x1, int y1, int x2, int y2){
    // https://stackoverflow.com/a/1079478
    // Use projection to determine dist between line point n and centerpoint
    pair<int, int> AC = vSub(make_pair(x1, y1), make_pair(centerX, centerY));
    pair<int, int> AB = vSub(make_pair(x2, y2), make_pair(x1, y1));
    // Get point D on line AB closest to centerpoint
    pair<int, int> D = vAdd(proj(AC, AB), make_pair(x1, y1));
    // Reduce the chance of divide by zero errors
    pair<int, int> AD = vSub(D, make_pair(x1, y1));
    int dist = 0;
    float k = abs(AD.first) > abs(AB.second) ? AD.first / AB.first : AD.second / AB.second;
    // Check if D is within the line segment
    if (k <= 0.0){
        dist = sqrt(hypot2(make_pair(centerX, centerY), make_pair(x1, y1)));
    } else if (k >= 1.0){
        dist = sqrt(hypot2(make_pair(centerX, centerY), make_pair(x2, y2)));
    } else{
        dist = sqrt(hypot2(make_pair(centerX, centerY), D));
    }
    // Check if we're within the circle
    if (r >= dist) return true;
    // We aren't within the circle =(
    return false;
}

// Check if lines intersect with each other
bool checkForLineIntersect(int firstX1, int firstY1, int firstX2, int firstY2, int secondX1, int secondY1, int secondX2, int secondY2){
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
        return true;
    }
    return false; // If none of the above conditions are met, the lines do not intersect
}

// ChatGPT's implementation of the problem :: TODO: REMOVE THIS AND WRITE OUR OWN ALGORITHM
#include <cmath>
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
