#include "RowTransposition.h"

bool RowTransposition::setKey(const string& inputkey)
{
	bool numberEncountered[100]; //I'm assuming here that we won't receive a key greater than size 100

	// The key value
	int keyValue = -1;

	// The resturn value
	bool retVal = false;

	for (int i = 0; i < inputkey.size(); i++)
	{
		if (!isdigit(inputkey[i]))
		{
			break;
		}
		else //ensure that the key is a permutation on numbers 1-keysize
		{

			keyValue = inputkey[i] - 48;

			numberEncountered[keyValue - 1] = true;
		}
	}
	bool validKey = true;

	for (int i = 0; i < inputkey.size(); i++)
	{
		if(numberEncountered[i] == false)
		{
			validKey = false;
		}
	}

	if (validKey)
	{
		key = inputkey;
		retVal = true;
	}
	else
	{
		retVal = false;
	}
	return retVal;
}

string RowTransposition::encrypt(const string& p)
{
	if (p.size() == 0)
	{
		return "";
	}

	//initialize transposition matrix to a default value
	memset(transpositionMatrix, '-', sizeof(transpositionMatrix[0][0]) * MAX_MATRIX_COLS * key.size());
	memset(rearrangedTranspositionMatrix, '-', sizeof(transpositionMatrix[0][0]) * MAX_MATRIX_COLS * key.size());

	//First scan input text for positions of capital letters, then strip case
	string plaintext = p;
	for (int i = 0; i < plaintext.size(); i++)
	{
		if (isupper(plaintext[i])) //scan for case
		{
			capitalizationTracker += "u";
			plaintext[i] = tolower(plaintext[i]);
		}
		else
		{
			capitalizationTracker += "l";
		}
	}

	int cursor = 0;
	int numRows = 0;
	bool tableComplete = false; //use to short circuit
	for (int i = 0; i < MAX_MATRIX_COLS; i++) //Populate transposition matrix
	{
		for (int j = 0; j < key.size(); j++)
		{
			transpositionMatrix[i][j] = plaintext[cursor];
			cursor++;
			if (cursor == plaintext.size())
			{
				numRows = i + 1; //used later for encoding the matrix
				tableComplete = true;
				break;
			}
		}
		if (tableComplete)
		{
			break;
		}
	}
	//perform encryption
	string ciphertext = "";
	for (int i = 0; i < key.size(); i++) //for each column rearrange based on order of key
	{
		char c = key[i];
		int col = -1 + c - '0'; //get column number. -1 so we can get the proper index.
		int row = 0;
		while(transpositionMatrix[row][col] != '-' && transpositionMatrix[row][col] != '\0'  && row < MAX_MATRIX_COLS)
		{
			ciphertext += transpositionMatrix[row][col]; //generate ciphertext
			rearrangedTranspositionMatrix[row][i] = transpositionMatrix[row][col]; //rearrange columns
			row++;
		}
	}

	//ENCODE ALL NECESSARY INFORMATION FOR DECRYPTION*****************************************************************************************************************************************************************
	string encryptionInformation = ""; //this will contain our ciphertext as well as necessary information needed for decryption
	for (int i = 0; i < ciphertext.size(); i++)
	{
		encryptionInformation += ciphertext[i]; //the first line will contain the ciphertext
	}
	encryptionInformation += ' '; //delimited by a space

	stringstream intTostring;
	string strNumRows;
	intTostring << numRows;
	intTostring >> strNumRows;
	encryptionInformation += strNumRows; //store the number of rows we used

	encryptionInformation += ' ';

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < key.size(); j++)
		{
			encryptionInformation += rearrangedTranspositionMatrix[i][j]; //next section will contain the encoded rearranged transposition matrix
		}
	}
	encryptionInformation += ' '; // delimited by space

	for (int i = 0; i < capitalizationTracker.size(); i++)
	{
		encryptionInformation += capitalizationTracker[i];  //this section will contain the capitalization of the original plaintext
	}

	return encryptionInformation;
}

string RowTransposition::decrypt(const string& p)
{
	if (p.size() == 0)
	{
		return "";
	}
	memset(rearrangedTranspositionMatrix, '-', sizeof(transpositionMatrix[0][0]) * MAX_MATRIX_COLS * key.size());

	//Parse through encryption for encryption information and ciphertext
	string encryptionInformation = p;// create copy so we can make modifications during processing
	char delimiter = ' ';

	int delimPos = encryptionInformation.find(delimiter);
	string ciphertext = encryptionInformation.substr(0, delimPos); // extract ciphertext
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());

	delimPos = encryptionInformation.find(delimiter);
	string token = encryptionInformation.substr(0, delimPos); // extract number of rows in table
	stringstream asciiToInt;
	asciiToInt << token;
	int numRows = 0;
	asciiToInt >> numRows;
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());

	delimPos = encryptionInformation.find(delimiter);
	token = encryptionInformation.substr(0, delimPos); // extract table info
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());
	int cursor = 0;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < key.size(); j++)
		{
			rearrangedTranspositionMatrix[i][j] = token[cursor]; //populate plaifair matrix
			cursor++;
		}
	}

	delimPos = encryptionInformation.find(delimiter);
	capitalizationTracker = encryptionInformation.substr(0, delimPos); // extract capitalization info
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());

	//Perform decryption
	string plaintext = "";
	double keysizeAsDouble = double(key.size());
	int numberRows = ceil(ciphertext.size() / keysizeAsDouble); //need to the number of rows by dividing the ciphertext length by key length
	cursor = 0;

	//retreive the plaintext by using the key to rearrange columns

	for (int i = 0; i < numberRows; i++)//for every row
	{
		for (int j = 0; j < key.size(); j++)//for every column
		{
			for (int k = 0; k < key.size(); k++) //for each number in the key
			{
				char c = key[k];
				int keyValue = -1 + c - '0';
				if (keyValue == j) //find column position 1,2,3,...n
				{
					if (rearrangedTranspositionMatrix[i][k] != '-')
					{
						plaintext += rearrangedTranspositionMatrix[i][k]; //read off the ordered rows to obtain plaintext
					}
				}
			}
		}
	}

	for (int i = 0; i < capitalizationTracker.size(); i++)
	{
		if (capitalizationTracker[i] == 'u')
		{
			plaintext[i] = toupper(plaintext[i]);
		}
	}
	return plaintext;
}

void RowTransposition::printMatrix(FILE* fp)
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < key.size(); j++)
		{
			cout << transpositionMatrix[i][j] << " ";
		}
		cout << endl;
		if (transpositionMatrix[i+1][0] == '-')
		{
			break;
		}
	}
}
