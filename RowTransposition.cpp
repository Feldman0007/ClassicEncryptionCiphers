#include "RowTransposition.h"

bool RowTransposition::setKey(const string& inputkey)
{
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
	}
	key = inputkey;
	return true;
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
	bool tableComplete = false; //use to short circuit
	for (int i = 0; i < 100; i++) //Populate transposition matrix
	{
		for (int j = 0; j < 7; j++)
		{
			transpositionMatrix[i][j] = plaintext[cursor];
			cursor++;
			if (cursor == plaintext.size())
			{
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
	return ciphertext;
}

string RowTransposition::decrypt(const string& ciphertext)
{
	if (ciphertext.size() == 0)
	{
		return "";
	}

	string plaintext = "";
	int numberRows = ceil(ciphertext.size() / 7.0); //need to the number of rows by dividing the ciphertext length by key length
	int cursor = 0;

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