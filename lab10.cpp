// Name: Josh Tran
// Assignment: Lab 10
// Date: 11/3/25
// Description: Adds and subtracts numbers taken as strings from a file 
// External Sources: ChatGPT 
#include <iostream>   // For input and output
#include <fstream>    // For file input
#include <string>     // For using strings
using namespace std;

// Function to check if a string is a valid number (integer or decimal)
bool ValidDouble(string s) {
    if (s.empty()) return false;         // empty string is invalid
    int start = 0;                       // index to start checking characters
    if (s[0] == '+' || s[0] == '-') {   // skip sign if present
        start = 1;
        if (s.length() == 1) return false; // "+" or "-" alone is invalid
    }

    bool hasDecimal = false;  // flag to check if decimal exists
    int digitsBefore = 0;     // digits before decimal
    int digitsAfter = 0;      // digits after decimal

    for (size_t i = start; i < s.length(); i++) { // iterate over each character
        if (s[i] >= '0' && s[i] <= '9') {        // if digit
            if (!hasDecimal) digitsBefore++;
            else digitsAfter++;
        }
        else if (s[i] == '.') { // decimal point
            if (hasDecimal) return false; // only one decimal allowed
            hasDecimal = true;
        }
        else return false; // invalid character
    }

    if (hasDecimal) {           // if decimal exists, must have digits on both sides
        if (digitsBefore == 0 || digitsAfter == 0) return false;
    } else {                    // integer must have digits
        if (digitsBefore == 0) return false;
    }

    return true; // valid number
}

// Function to split a number into integer and fractional parts
void splitNumber(string num, string &intPart, string &fracPart) {
    size_t dot = num.find('.');       // find position of decimal
    if (dot == string::npos) {        // if no decimal
        intPart = num;                // all is integer
        fracPart = "";
    } else {
        intPart = num.substr(0,dot);          // before decimal
        fracPart = num.substr(dot+1);         // after decimal
    }
}

// Function to remove leading zeros from a string
string removeLeadingZeros(string s) {
    while (s.length() > 1 && s[0] == '0') s = s.substr(1);
    return s;
}

// Function to remove trailing zeros from fractional part
string removeTrailingZeros(string s) {
    while (!s.empty() && s[s.length()-1] == '0') s = s.substr(0,s.length()-1);
    return s;
}

//adds two numbers as strings 
string addStrings(string num1, string num2) {
    int carry = 0; //calls int variable carry 
    string result = ""; //calls string variable result 

    int i = num1.length()-1; //starts from the end of num 1 
    int j = num2.length()-1; //same thing with num 2 

    while (i >= 0 || j >= 0 || carry > 0) { //while within both lengths and carry is not 0
        int digit1 = (i >= 0) ? num1[i]-'0' : 0; // casts num 1 digit to int 
        int digit2 = (j >= 0) ? num2[j]-'0' : 0; //casts num2 digit to int 
        int sum = digit1 + digit2 + carry; //adds them together 
        result = char(sum%10 + '0') + result; //adds sum modulo 10 to the result 
        carry = sum/10; //updates the carry 
        i--; j--; //moves to next digits (previous ones )
    }

    return result;
}
//substracts num 1 from num 2 
string subtractStrings(string num1, string num2) {
    int borrow = 0; //casts int borrow to 0 
    string result = ""; //casts string result to empty string 

    int i = num1.length()-1; //starts from the end 
    int j = num2.length()-1;

    while (i >= 0) { //loops until all digits have been iterated through 
        int digit1 = num1[i]-'0' - borrow; //subtracts borrow from digit 1 
        int digit2 = (j >= 0) ? num2[j]-'0' : 0; //gets digit from num 2 
        if (digit1 < digit2) { //checks to see if borrow is needed 
            digit1 += 10; //borrows from next digit 
            borrow = 1; //sets borrow 
        } else borrow = 0;

        result = char(digit1 - digit2 + '0') + result; //adds the difference to result 
        i--; j--; //moves to next digits 
    }

    return removeLeadingZeros(result); //returns results 
}

// Add two decimal numbers as strings
string addDecimals(string num1, string num2) {
    string int1, frac1, int2, frac2;
    splitNumber(num1,int1,frac1); // split first number
    splitNumber(num2,int2,frac2); // split second number

    // Make fractional parts same length
    while (frac1.length() < frac2.length()) frac1 += '0';
    while (frac2.length() < frac1.length()) frac2 += '0';

    string fracSum = addStrings(frac1, frac2); // add fractional parts
    int carry = 0;
    if (fracSum.length() > frac1.length()) { // carry from fraction
        carry = 1;
        fracSum = fracSum.substr(1);
    }

    // Add integer parts
    string intSum = addStrings(int1, int2);
    if (carry) intSum = addStrings(intSum, "1"); // add carry if needed

    fracSum = removeTrailingZeros(fracSum);
    intSum = removeLeadingZeros(intSum);

    if (fracSum.empty()) return intSum;
    return intSum + "." + fracSum;
}

// Subtract two decimals 
string subtractDecimals(string num1, string num2) {
    string int1, frac1, int2, frac2;
    splitNumber(num1,int1,frac1);
    splitNumber(num2,int2,frac2);

    // Make fractional parts same length
    while (frac1.length() < frac2.length()) frac1 += '0';
    while (frac2.length() < frac1.length()) frac2 += '0';

    // If fractional part of num1 < num2, borrow 1 from integer
    if (frac1 < frac2) {
        int1 = subtractStrings(int1,"1");
        frac1 = addStrings(frac1, string(frac1.length(),'0') + "10"); // borrows 
    }

    string fracDiff = subtractStrings(frac1, frac2); //subtracts fractions 
    string intDiff = subtractStrings(int1, int2); //subtracts integers 

    fracDiff = removeTrailingZeros(fracDiff); //removes zeros 
    intDiff = removeLeadingZeros(intDiff); //removes zeros 

    if (fracDiff.empty()) return intDiff; //returns integer if there are no fractions 
    return intDiff + "." + fracDiff; //returns final number 
}

// Main function
int main() {
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "File given is invalid..." << endl;
        return 1;
    }

    string num1, num2;
    while (file >> num1 >> num2) { // read two numbers per line
        if (!ValidDouble(num1) || !ValidDouble(num2)) { //checks to make sure both numbers are valid 
            cout << "Invalid number: " << num1 << " " << num2 << endl; //if not, prints the line that is not 
            continue;
        }

        // Determine signs
        bool neg1 = false, neg2 = false;
        if (num1[0] == '-') { neg1 = true; num1 = num1.substr(1); }
        else if (num1[0] == '+') num1 = num1.substr(1);

        if (num2[0] == '-') { neg2 = true; num2 = num2.substr(1); }
        else if (num2[0] == '+') num2 = num2.substr(1);

        string sum, diff;

        // Handle addition
        if (neg1 == neg2) {
            sum = addDecimals(num1, num2);
            if (neg1) sum = "-" + sum;
        } else { // different signs -> subtraction
            if (num1 >= num2) sum = subtractDecimals(num1, num2);
            else sum = subtractDecimals(num2, num1);
            if ((num1 < num2 && !neg1) || (num1 >= num2 && neg1)) sum = "-" + sum;
        }

        // Handle subtraction (num1 - num2)
        if (neg1 != neg2) { // different signs -> addition
            diff = addDecimals(num1, num2);
            if (neg1) diff = "-" + diff;
        } else { // same signs -> subtraction
            if (num1 >= num2) diff = subtractDecimals(num1, num2);
            else diff = subtractDecimals(num2, num1);
            if ((num1 < num2 && !neg1) || (num1 >= num2 && neg1)) diff = "-" + diff;
        }

        // Output results
        cout << num1 << " + " << num2 << " = " << sum << endl;
        cout << num1 << " - " << num2 << " = " << diff << endl;
    }

    file.close(); // close file
    return 0;
}
