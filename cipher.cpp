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

	//Testing
	//CipherInterface* cipher = new Vigenre();
	//CipherInterface* cipher1 = new Caesar();
	//CipherInterface* cipher2 = new RowTransposition();
	//CipherInterface* cipher3 = new Playfair();
	//CipherInterface* cipher4 = new Railfence();

	//validateAndSetKey(cipher, "security");
	//validateAndSetKey(cipher1, "4");
	//validateAndSetKey(cipher2, "7654321");
	//validateAndSetKey(cipher3, "security");
	//validateAndSetKey(cipher4, "4");

	//performOperation(cipher, "enc", "small.txt", "vgencrypt.txt");
	//performOperation(cipher1, "enc", "small.txt", "csencrypt.txt");
	//performOperation(cipher2, "enc", "small.txt", "rtencrypt.txt");
	//performOperation(cipher3, "enc", "small.txt", "pfencrypt.txt");
	//performOperation(cipher4, "enc", "small.txt", "rfencrypt.txt");

	//performOperation(cipher, "dec", "vgencrypt.txt", "vgdecrypt.txt");
	//performOperation(cipher1, "dec", "csencrypt.txt", "csdecrypt.txt");
	//performOperation(cipher2, "dec", "rtencrypt.txt", "rtdecrypt.txt");
	//performOperation(cipher3, "dec", "pfencrypt.txt", "pfdecrypt.txt");
	//performOperation(cipher4, "dec", "rfencrypt.txt", "rfdecrypt.txt");

	//return 0;

	/*Make sure we have only 5 command line arguments before moving forward*/
	if (argc != 6)
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
		string encryptionInformation = cipher->encrypt(inputText); //we may want to store encryption information in addition to the ciphertext
		writeFile(outputFilename, encryptionInformation);
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
		cout << "Input file has no text!";
		exit(-1);
	}

	string fileContents; /*Buffer to hold file contents*/

	while (!fileReader.eof())
	{
		string buffer = "";
		fileReader >> buffer;
		for (int i = 0; i < buffer.size(); i++)
		{
			fileContents += buffer[i];
		}
		//Add a space between each peice of info. don't add a space at the end though

		if (!fileReader.eof())
		{
			fileContents += ' ';
		}
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
