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
	string cipherText = "";
	string plaintext = p; //copy of the plaintext so we can modify it. 
	//First scan input text for positions of capital letters, then strip case
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

	//get the number of rails and the length of rails
	stringstream asciiToInt;
	asciiToInt << key;
	int numRails = 0;
	asciiToInt >> numRails;
	int railLength = ceil(plaintext.size() / double(numRails)); //number of columns if we treat rails as 2d Matrix 	
	
	//Create rail "matrix"
	memset(rails, '-', sizeof(rails[0][0]) * 100 * 100);
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
			cipherText += rails[i][j]; //read off letters rail by rail to produce ciphertext
		}
	}
	
	return cipherText;


}

string Railfence::decrypt(const string& ciphertext)
{
	if (ciphertext.size() == 0)
	{
		return "";
	}
	memset(showUnderstanding, '-', sizeof(showUnderstanding[0][0]) * 100 * 100);

	//To decrypt, we need to know the number of rails and the number of letter per rail
	string plaintext = "";
	stringstream asciiToInt;
	asciiToInt << key;
	int numRails = 0;
	asciiToInt >> numRails; 
	int railLength = ceil(ciphertext.size() / double(numRails)); //number of columns if we treat rails as 2d Matrix 	
	int filledRails = ciphertext.size() / numRails; //number of letters per rail, assuming we no partially filled columns 
	int remainingLetters = ciphertext.size() % numRails; // remaining letters


	//we will repopulate a rail matrix to show understanding of the decryption algorithm
	//these steps are only to show understanding of decryption
	int cursor = 0;
	for (int i = 0; i < numRails; i++) //read from all all rails except 
	{
		for (int j = 0; j < filledRails; j++) //fill out each rail 
		{
			showUnderstanding[i][j] = ciphertext[cursor];
			cursor++;
		}
		if (remainingLetters) //if there are remaining letters, add additional character to that rail
		{
			showUnderstanding[i][filledRails] = ciphertext[cursor];
			remainingLetters -= 1;
			cursor++;
		}
	}

	for (int i = 0; i < railLength; i++)
	{
		for (int j = 0; j < numRails; j++)
		{
			if (showUnderstanding[j][i] == '-')
			{
				break;
			}
			plaintext += showUnderstanding[j][i];
		}
	}
	return plaintext;
}