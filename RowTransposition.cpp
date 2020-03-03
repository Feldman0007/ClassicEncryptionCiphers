#include "RowTransposition.h"

bool RowTransposition::setKey(const string& inputkey)
{
	bool numberEncountered[7];
	if (inputkey.size() != 7)
	{
		return false;
	}
	for (int i = 0; i < key.size(); i++)
	{
		if (!isdigit(inputkey[i])) 
		{
			return false;
		}
		else //ensure that the key is a permutation of numbers 1-7
		{
			numberEncountered[-1 + int(inputkey[i])] = true;
		}
	}
	bool validKey = true;
	for (int i = 0; i < 7; i++)
	{
		if(numberEncountered[i] == false)
		{
			validKey = false;
		}
	}
	if (validKey)
	{
		key = inputkey;
		return true;
	}
	else
	{
		return false;
	}
}

string RowTransposition::encrypt(const string& p)
{
	if (p.size() == 0)
	{
		return "";
	}

	//initialize transposition matrix to a default value
	memset(transpositionMatrix, '-', sizeof(transpositionMatrix[0][0]) * 100 * 7);
	memset(rearrangedTranspositionMatrix, '-', sizeof(transpositionMatrix[0][0]) * 100 * 7);

	//First scan input text for positions of capital letters, then strip case
	string plaintext = p;
	for (int i = 0; i < plaintext.size(); i++)
	{
		if (isupper(plaintext[i])) //scan for case
		{
			capitalizationTracker += "u";
		}
		else
		{
			capitalizationTracker += "l";
		}
	}
	for (int i = 0; i < plaintext.size(); i++)
	{
		plaintext[i] = tolower(plaintext[i]);
	}

	string ciphertext = "";
	int cursor = 0;
	int numRows = 0;
	bool tableComplete = false; //use to short circuit
	for (int i = 0; i < 100; i++) //Populate transposition matrix
	{
		for (int j = 0; j < 7; j++)
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
	for (int i = 0; i < key.size(); i++) 
	{
		char c = key[i];
		int col = -1 + c - '0'; //rearrange columns based on order of key
		int row = 0;
		while(transpositionMatrix[row][col] != '-' && row < 100)
		{
			ciphertext += transpositionMatrix[row][col]; //generate ciphertext
			rearrangedTranspositionMatrix[row][i] = transpositionMatrix[row][col]; //rearrange columns
			row++;
		}
	}

	//ENCODE ALL NECESSARY INFORMATION FOR DECRYPTION*****************************************************************************************************************************************************************
	string encryptionInformation; //this will contain our ciphertext as well as necessary information needed for decryption
	for (int i = 0; i < ciphertext.size(); i++)
	{
		encryptionInformation += ciphertext[i]; //the first line will contain the ciphertext,
	}
	encryptionInformation += ' '; //delimited by a space

	encryptionInformation += '0' + numRows; //store the number of rows we used 

	encryptionInformation += ' ';

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < 7; j++)
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
		for (int j = 0; j < 7; j++)
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
	int numberRows = ceil(ciphertext.size() / 7.0); //need to the number of rows by dividing the ciphertext length by key length
	cursor = 0;

	//retreive the plaintext by using the key to rearrange columns

	for (int i = 0; i < numberRows; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			for (int k = 0; k < 7; k++)
			{
				char c = key[k];
				int keyValue = -1 + c - '0'; 
				if (keyValue == j) //find column position 1,2,3,...7
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
		for (int j = 0; j < 7; j++)
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