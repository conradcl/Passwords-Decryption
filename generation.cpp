#pragma comment(lib, "crypt32")
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <fstream>
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

using namespace std;

void generation(int length, char password[], int index, ofstream& outfile, RSA* rsa);
void capitalization(int length, char password[], ofstream& outfile, RSA* rsa);
string sha256(const string& str);
string rsa_encrypt(RSA* rsa, const string& str);
RSA* generate_rsa_key();

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

    ofstream outfile("output1.txt");

    RSA* rsa = generate_rsa_key();
    if (!rsa) {
        cerr << "Failed to generate RSA key pair" << endl;
        return 1;
    }

    cout << "PLAIN TEXT                     SHA256                                                          RSA" << endl;

    for (int length = 1; length <= maxLength; length++) {
        generation(length, password, 0, outfile, rsa);
    }

    outfile.close();
    RSA_free(rsa);
    return 0;
}

void generation(int length, char password[], int index, ofstream& outfile, RSA* rsa) {
    if (index == length) {
        string pass(password);
        string hash = sha256(pass);
        string encrypted = rsa_encrypt(rsa, pass);
        cout << pass << " " << hash << " " << encrypted << endl;
        outfile << pass << " " << hash << " " << encrypted << endl;
        capitalization(length, password, outfile, rsa);
        return;
    }

    for (int i = 0; i < 26; i++) {
        password[index] = 97 + i;
        generation(length, password, index + 1, outfile, rsa);
    }
}

void capitalization(int length, char password[], ofstream& outfile, RSA* rsa) {
    if (length == 1) {
        password[0] = toupper(password[0]);
        string pass(password);
        string hash = sha256(pass);
        string encrypted = rsa_encrypt(rsa, pass);
        cout << pass << " " << hash << " " << encrypted << endl;
        outfile << pass << " " << hash << " " << encrypted << endl;
    }
    else if (length == 2) {
        password[0] = toupper(password[0]);
        string pass(password);
        string hash = sha256(pass);
        string encrypted = rsa_encrypt(rsa, pass);
        cout << pass << " " << hash << " " << encrypted << endl;
        outfile << pass << " " << hash << " " << encrypted << endl;

        password[0] = tolower(password[0]);
        password[1] = toupper(password[1]);
        pass = string(password);
        hash = sha256(pass);
        encrypted = rsa_encrypt(rsa, pass);
        cout << pass << " " << hash << " " << encrypted << endl;
        outfile << pass << " " << hash << " " << encrypted << endl;

        password[0] = toupper(password[0]);
        pass = string(password);
        hash = sha256(pass);
        encrypted = rsa_encrypt(rsa, pass);
        cout << pass << " " << hash << " " << encrypted << endl;
        outfile << pass << " " << hash << " " << encrypted << endl;

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
            string encrypted = rsa_encrypt(rsa, pass);
            cout << pass << " " << hash << " " << encrypted << endl;
            outfile << pass << " " << hash << " " << encrypted << endl;
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
            string encrypted = rsa_encrypt(rsa, pass);
            cout << pass << " " << hash << " " << encrypted << endl;
            outfile << pass << " " << hash << " " << encrypted << endl;
        }
        password[0] = tolower(password[0]);
        password[1] = tolower(password[1]);
        password[2] = tolower(password[2]);
        password[3] = tolower(password[3]);
    }
}

string sha256(const string& str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

RSA* generate_rsa_key() {
    int bits = 2048;
    unsigned long e = RSA_F4;
    RSA* rsa = RSA_new();
    BIGNUM* bne = BN_new();
    if (BN_set_word(bne, e) != 1) {
        RSA_free(rsa);
        BN_free(bne);
        return nullptr;
    }
    if (RSA_generate_key_ex(rsa, bits, bne, NULL) != 1) {
        RSA_free(rsa);
        BN_free(bne);
        return nullptr;
    }
    BN_free(bne);
    return rsa;
}

string rsa_encrypt(RSA* rsa, const string& str) {
    int rsa_len = RSA_size(rsa);
    unsigned char* encrypted_text = new unsigned char[rsa_len];
    int result = RSA_public_encrypt(str.length(), (unsigned char*)str.c_str(), encrypted_text, rsa, RSA_PKCS1_OAEP_PADDING);
    if (result == -1) {
        cerr << "RSA encryption failed: " << ERR_error_string(ERR_get_error(), NULL) << endl;
        delete[] encrypted_text;
        return "";
    }

    stringstream ss;
    for (int i = 0; i < result; i++) {
        ss << hex << setw(2) << setfill('0') << (int)encrypted_text[i];
    }

    delete[] encrypted_text;
    return ss.str();
}
