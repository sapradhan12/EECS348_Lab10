#include "matrix.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

int main() {
    cout << "Enter the file name: ";
    string filename;
    cin >> filename;
    ifstream infile(filename.c_str());
    if (!infile) {
        cout << "Cannot open file: " << filename << endl;
        return 1;
    }
    string line;
    string addend = "-123.456";
    while (getline(infile, line)) {
        while (!line.empty() && std::isspace(line[0]))
            line.erase(line.begin());
        while (!line.empty() && std::isspace(line[line.size()-1]))
            line.erase(line.end()-1);
        if (line == "")
            continue;
        if (isValidDouble(line)) {
            string result = addDoubleStrings(line, addend);
            cout << "Valid number: " << line << endl;
            cout << "After adding " << addend << ", result: " << result << endl;
        } else {
            cout << "Invalid number: " << line << endl;
        }
    }
    infile.close();
    return 0;
}