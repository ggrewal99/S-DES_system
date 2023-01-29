#include<iostream>
#include<map>

#include "sdes.h"

using namespace std;

bool isBinary(string in);
bool isValid(string in, int n);

int main(){
	string in, k;
	cout << "**** S-DES SYSTEM ****\n" << endl;
	cout << "Enter an option from below:" << endl;

	cout << "1. Encryption" << endl;
	cout << "2. Decryption" << endl;

	int opt;
	do{
		cout << "[option]: ";
		cin >> opt;
		if(opt != 1 && opt != 2)
			cout << "Invalid. Please select an option number from above." << endl;
		else
			break;
	}while(true);

	do{
		cout << "Enter an 8 bit ";
		if(opt == 1)
			cout << "Plaintext: ";
		else
			cout << "Ciphertext: ";
		cin >> in;
		if(isValid(in, 8))	// Checks if the input is binary and is the correct size.
			break;
	}while(true);
	do{
		cout << "Enter the 10 bit key (k): ";
		cin >> k;
		if(isValid(k, 10))
			break;
	}while(true);

	Sdes test(in, k, opt);
	test.run();

	return 0;
}

bool isBinary(string in){
	for(int i = 0; i < in.size(); i++)
			if(in[i] != '0' && in[i] != '1')
				return false;
	return true;
}

bool isValid(string in, int n){
	if(in.size() < n || in.size() > n)
			cout << "Error: Binary value is not the correct size!" << endl;
	else if(!isBinary(in))
			cout << "Error: Not a valid binary value!" << endl;
	else
		return true;
	return false;
}
