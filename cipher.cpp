#include <string>
#include <iostream>
#include <fstream>
#include "CipherInterface.h"
#include "Playfair.h"
#include "Caesar.h"
#include "Vigenre.h"
#include "RowTransposition.h"
#include "Railfence.h"

using namespace std;

/*Case insensitive string comparison helper function used in processing command line arguments*/
bool iequals(const string&, const string&);
/*Error checking to see if we have correctly set up an instance of our cipher*/
void assertValidCipherAssignment(const CipherInterface*);
/*Read input file ciphertext*/
string readFile(const string&);
/*Write results of operation to output file*/
void writeFile(const string&, const string&);
/*Validate the key against the type of cypher selected and assign the cipher's key*/
void validateAndSetKey(CipherInterface* const, const string&);
/*Perform either encryption or decryption and write to output file*/
void performOperation(CipherInterface* const, const string&, const string&, const string&);

int main(int argc, char** argv)
{
	
	////Testing
	//CipherInterface* cipher = new Railfence();
	//validateAndSetKey(cipher, "3");
	//performOperation(cipher, "e", "small.txt", "railfenceTest.txt");
	//performOperation(cipher, "d", "railfenceTest.txt", "decrypt.txt");
	//return 0;

	/*Make sure we have only 5 command line arguments before moving forward*/
	if (argc != 5)
	{
		cout << "cipher.exe only accepts 5 arguments: <CIPHER NAME> <KEY> <ENC/DEC> <INPUTFILE> <OUTPUT FILE>" << endl;
		exit(-1);
	}

	/*Variables used to parse the command line argument and execute the ciphers dynamically*/
	string cipherName = argv[1];
	string key = argv[2];
	string operation = argv[3];
	string inputFileName = argv[4];
	string outputFileName = argv[5];

	CipherInterface* cipher = nullptr; /*pointer to an instance of our cipher*/

	if (iequals(cipherName, "PLF")) 
	{
		cipher = new Playfair();
	}
	else if (iequals(cipherName, "RFC")) 
	{
		cipher = new Railfence();
	}
	else if (iequals(cipherName, "CES"))
	{
		cipher = new Caesar();
	}
	else if (iequals(cipherName, "RTS"))
	{
		cipher = new RowTransposition();
	}
	else if (iequals(cipherName, "VIG"))
	{
		cipher = new Vigenre();
	}
	else
	{
		cout << "Invalid cipher type!\n";
		exit(-1);
	}

	assertValidCipherAssignment(cipher);
	validateAndSetKey(cipher, key);
	performOperation(cipher, operation, inputFileName, outputFileName);

	return 0;
}

void validateAndSetKey(CipherInterface* const cipher, const string& key)
{
	bool validKey = cipher->setKey(key);
	if (!validKey)
	{
		cout << "This key is not valid for the selected cipher!" << endl;
		exit(-1);
	}
}

void performOperation(CipherInterface* const cipher, const string& operation, const string& inputFilename, const string& outputFilename)
{
	/* Perform encryption */
	string inputText = readFile(inputFilename);
	if (iequals(operation, "ENC"))
	{
		string cipherText = cipher->encrypt(inputText);
		writeFile(outputFilename, cipherText);
	}
	/*Perform decryption */
	else if (iequals(operation, "DEC"))
	{
		string plainText = cipher->decrypt(inputText);
		writeFile(outputFilename, plainText);
	}
	else
	{
		cout << "This key is not a valid operation!" << endl;
		exit(-1);
	}
}

string readFile(const string& filename) 
{
	fstream fileReader;
	fileReader.open(filename, ios::in);
	if (fileReader.fail()) {
		cout << "Error opening input file!" << endl;
			exit(-1);
	}
	if (fileReader.peek() == std::ifstream::traits_type::eof())
	{
		exit(-1);
	}

	string fileContents; /*Buffer to hold file contents*/

	while (!fileReader.eof()) 
	{
		fileReader >> fileContents;
	}

	fileReader.close();
	return fileContents;
}

void writeFile(const string& filename, const string& results)
{
	fstream fileWriter;
	fileWriter.open(filename, ios::out | ios::trunc);
	if (fileWriter.fail()) {
		cout << "Error opening output file!" << endl;
		exit(-1);
	}

	for (int i = 0; i < results.size(); i++) 
	{
		fileWriter << results[i];
	}
	fileWriter.close();
}

bool iequals(const string& a, const string& b) //case insensitive comparison
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

void assertValidCipherAssignment(const CipherInterface* cipher) 
{
	/*Error checking*/
	if (!cipher)
	{
		fprintf(stderr, "ERROR [%s %s %d]: could not allocate memory\n",
			__FILE__, __FUNCTION__, __LINE__);
		exit(-1);
	}
}
