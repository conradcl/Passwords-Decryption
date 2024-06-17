#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <openssl/sha.h>

using namespace std;

void generation(int length, char password[], int index, ofstream& outfile);
void capitalization(int length, char password[], ofstream& outfile);
string sha256(const string& str);

const char binaryArraysThree[][4] = {
    "111", "001", "011", "101",
    "010", "110", "100"
};

const char binaryArraysFour[][5] = {
    "1111", "0001", "0010", "0011",
    "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011",
    "1100", "1101", "1110"
};

int main() {
    const int maxLength = 4;
    char password[maxLength + 1] = { 0 };

    ofstream outfile("output.txt");

    for (int length = 1; length <= maxLength; length++) {
        generation(length, password, 0, outfile);
    }

    outfile.close();
    return 0;
}

void generation(int length, char password[], int index, ofstream& outfile) {
    if (index == length) {
        string pass(password);
        string hash = sha256(pass);
        cout << pass << " " << hash << endl;
        outfile << pass << " " << hash << endl;
        capitalization(length, password, outfile);
        return;
    }

    for (int i = 0; i < 26; i++) {
        password[index] = 97 + i;
        generation(length, password, index + 1, outfile);
    }
}

void capitalization(int length, char password[], ofstream& outfile) {
    if (length == 1) {
        password[0] = toupper(password[0]);
        string pass(password);
        string hash = sha256(pass);
        cout << pass << " " << hash << endl;
        outfile << pass << " " << hash << endl;
    }
    else if (length == 2) {
        password[0] = toupper(password[0]);
        string pass(password);
        string hash = sha256(pass);
        cout << pass << " " << hash << endl;
        outfile << pass << " " << hash << endl;
        password[0] = tolower(password[0]);

        password[1] = toupper(password[1]);
        pass = string(password);
        hash = sha256(pass);
        cout << pass << " " << hash << endl;
        outfile << pass << " " << hash << endl;

        password[0] = toupper(password[0]);
        pass = string(password);
        hash = sha256(pass);
        cout << pass << " " << hash << endl;
        outfile << pass << " " << hash << endl;

        password[0] = tolower(password[0]);
        password[1] = tolower(password[1]);
    }
    else if (length == 3) {
        for (int i = 0; i < 7; ++i) {
            for (int j = 0; j < length; ++j) {
                if (binaryArraysThree[i][j] == '1') {
                    password[j] = toupper(password[j]);
                }
                else {
                    password[j] = tolower(password[j]);
                }
            }
            string pass(password);
            string hash = sha256(pass);
            cout << pass << " " << hash << endl;
            outfile << pass << " " << hash << endl;
        }
        password[0] = tolower(password[0]);
        password[1] = tolower(password[1]);
        password[2] = tolower(password[2]);
    }
    else if (length == 4) {
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < length; ++j) {
                if (binaryArraysFour[i][j] == '1') {
                    password[j] = toupper(password[j]);
                }
                else {
                    password[j] = tolower(password[j]);
                }
            }
            string pass(password);
            string hash = sha256(pass);
            cout << pass << " " << hash << endl;
            outfile << pass << " " << hash << endl;
        }
        password[0] = tolower(password[0]);
        password[1] = tolower(password[1]);
        password[2] = tolower(password[2]);
        password[3] = tolower(password[3]);
    }
}

string sha256(const string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}