#include "Railfence.h"

bool Railfence::setKey(const string& inputkey)
{
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

string Railfence::encrypt(const string& p)
{
	if (p.size() == 0)
	{
		return "";
	}
	string ciphertext = "";
	string plaintext = p; //copy of the plaintext so we can modify it. 
	//First scan input text for positions of capital letters, then strip case
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

	//get the number of rails and the length of rails
	stringstream asciiToInt;
	asciiToInt << key;
	int numRails = 0;
	asciiToInt >> numRails;
	int railLength = ceil(plaintext.size() / double(numRails)); //number of columns if we treat rails as 2d Matrix 	
	
	//Create rail "matrix"
	memset(rails, '-', sizeof(rails[0][0]) * MAX_MATRIX_COLS * numRails);
	int cursor = 0; //keeps track of position in plaintext 
	bool doneProcessing = false;
	for (int col = 0; col < railLength; col++)
	{
		for (int rail = 0; rail < numRails; rail++)
		{
			rails[rail][col] = plaintext[cursor];
			cursor++;
			if (cursor == plaintext.size())
			{
				doneProcessing = true;
				break;
			}
		}
		if (doneProcessing)
		{
			break;
		}
	}

	for (int i = 0; i < numRails; i++)
	{
		for (int j = 0; j < railLength; j++)
		{
			if (rails[i][j] == '-')
			{
				break;
			}
			ciphertext += rails[i][j]; //read off letters rail by rail to produce ciphertext
		}
	}
	
	//ENCODE ALL NECESSARY INFORMATION FOR DECRYPTION*****************************************************************************************************************************************************************
	string encryptionInformation; //this will contain our ciphertext as well as necessary information needed for decryption
	for (int i = 0; i < ciphertext.size(); i++)
	{
		encryptionInformation += ciphertext[i]; //the first line will contain the ciphertext,
	}
	encryptionInformation += ' '; //delimited by a space

	for (int i = 0; i < capitalizationTracker.size(); i++)
	{
		encryptionInformation += capitalizationTracker[i]; //this section will contain the positions of i's and j's
	}
	encryptionInformation += ' '; //delimited by a space

	return encryptionInformation;
}

string Railfence::decrypt(const string& p)
{
	if (p.size() == 0)
	{
		return "";
	}
	memset(rails, '-', sizeof(rails[0][0]) * 100 * 100);

	//Parse through encryption file for encryption information and ciphertext
	string encryptionInformation = p;// create copy so we can make modifications during processing
	char delimiter = ' ';

	int delimPos = encryptionInformation.find(delimiter);
	string ciphertext = encryptionInformation.substr(0, delimPos); // extract ciphertext
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());

	delimPos = encryptionInformation.find(delimiter);
	capitalizationTracker = encryptionInformation.substr(0, delimPos); // extract captalization information

	//To decrypt, we need to know the number of rails and the number of letter per rail
	string plaintext = "";
	stringstream asciiToInt;
	asciiToInt << key;
	int numRails = 0;
	asciiToInt >> numRails; 
	int railLength = ceil(ciphertext.size() / double(numRails)); //number of columns if we treat rails as 2d Matrix 	
	int filledRails = ciphertext.size() / numRails; //number of letters per rail, assuming we no partially filled columns 
	int remainingLetters = ciphertext.size() % numRails; // remaining letters


	//Repopulate a rail matrix to show understanding of the decryption algorithm
	int cursor = 0;
	for (int i = 0; i < numRails; i++) //read from all all rails except 
	{
		for (int j = 0; j < filledRails; j++) //fill out each rail 
		{
			rails[i][j] = ciphertext[cursor];
			cursor++;
		}
		if (remainingLetters) //if there are remaining letters, add additional character to that rail
		{
			rails[i][filledRails] = ciphertext[cursor];
			remainingLetters -= 1;
			cursor++;
		}
	}

	for (int i = 0; i < railLength; i++)
	{
		for (int j = 0; j < numRails; j++)
		{
			if (rails[j][i] == '-' || rails[j][i] == '\0')
			{
				break;
			}
			plaintext += rails[j][i];
		}
	}

	for (int i = 0; i < plaintext.size(); i++) //plaintext should be correct now, just have to adjust capitalization
	{
		if (capitalizationTracker[i] == 'u')
		{
			plaintext[i] = toupper(plaintext[i]);
		}
	}

	return plaintext;
}