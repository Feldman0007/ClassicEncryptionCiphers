#include "Vigenre.h"

bool Vigenre::setKey(const string& inputkey)
{
	for (int i = 0; i < inputkey.size(); i++)
	{
		if (!isalpha(inputkey[i]))
		{
			return false;
		}
	}
	key = inputkey;
	return true;
}

string Vigenre::encrypt(const string& p)
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

	//First create the Vigenre table
	for(int i = 0; i < 26; i++)
	{
		for (int j = 0; j < 26; j++)
		{
			vigenreMatrix[i][j] = alphabet[j];
		}
		alphabet.insert(26, 1, alphabet[0]); // insert the 4th character at the beginning
		alphabet.erase(0, 1);
	}
	//Stretch key out to meet the length of the plaintext 
	if (key.size() > plaintext.size())
	{
		string useableKey = key.substr(0, plaintext.size());
		key = useableKey;
	}
	else
	{
		int cursor = 0;
		int originalkeysize = key.size(); //repeating key
		while (key.size() < plaintext.size())
		{
			if (cursor == originalkeysize)
			{
				cursor = 0;
			}
			key += key[cursor];
			cursor++;
		}
		//int cursor = 0;             //autokey
		//while (key.size() < plaintext.size())
		//{
		//	key += plaintext[cursor];
		//	cursor++;
		//}
	}

	//Table and Key built, perform translation

	for (int i = 0; i < plaintext.size(); i++) 
	{
		int x = -1;
		int y = -1;

		//get letters' position in the alphabet.
		for (int j = 0; j < alphabet.size(); j++)
		{
			if (plaintext[i] == alphabet[j])
			{
				x = j;
			}
			if (key[i] == alphabet[j])
			{
				y = j;
			}
			if (x != -1 && y != -1) //short circuit
			{
				break;
			}
		}
		cipherText += vigenreMatrix[x][y];
	}

	return cipherText;
}

string Vigenre::decrypt(const string& ciphertext)
{
	if (ciphertext.size() == 0)
	{
		return "";
	}
	string plaintext = "";
	for (int i = 0; i < ciphertext.size(); i++)
	{
		int row = -1; //get letters' position in the alphabet.
		for (int j = 0; j < alphabet.size(); j++)
		{
			if (key[i] == alphabet[j])
			{
				row = j;
				break;
			}
		}
		int col = -1;
		for (int j = 0; j < 26; j++) // we have the row number now, so seach for the ciphertext letter in that row
		{
			if (vigenreMatrix[row][j] == ciphertext[i])
			{
				col = j;
				break;
			}
		}
		plaintext += alphabet[col];
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

void Vigenre::printMatrix(FILE* fp)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << vigenreMatrix[i][j] << " ";
		}
		cout << endl;
	}
}