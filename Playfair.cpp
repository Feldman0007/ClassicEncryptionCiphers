#include "Playfair.h"
#include <unordered_map>


//Helper function used to help us find the coordinates of a letter in the playfair matrix
pair<int, int> findPosition(char matrix[][5], char letter)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (matrix[i][j] == letter)
			{

				return make_pair(i, j);
			}
		}
	}
	return make_pair(-1, -1);
}

bool Playfair::setKey(const string& inputkey)
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

string Playfair::encrypt(const string& p)
{
	if (p.size() == 0)
	{
		return "";
	}
	//Variables used to implement logic for populating our matrix
	unordered_map<char, bool> encounteredLetter;
	int rowPosition = 0;
	int columnPosition = 0;

	string plaintext = p; //creating a duplicate of plaintext so we can modify
	string ciphertext; 
	
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

	bool isNewBlock = false; //flag variable used to make sure we look at blocks two letters at a time when searching for two identical letters 
	for (int i = 0; i < plaintext.size(); i++) //then scan for i/j's, duplicate letters, odd input size padding so we can preserve plaintext through encryption and decryption
	{
		if (plaintext[i] == 'j' || plaintext[i] == 'i') //scan for i's and j's
		{
			wasAnIorJ += plaintext[i];
		}
		else
		{
			wasAnIorJ += '-'; //placeholder
		}

		if (isNewBlock) //ensures a block is every two letters
			isNewBlock = false;
		else
			isNewBlock = true;

		if (isNewBlock) //we're starting a new block so look at the next letter
		{
			if (i + 1 == plaintext.size()) //If we're last letter and input is odd, pad at the end
			{
				bufferInsertedAt += '-';
				bufferInsertedAt += 'x';
				wasAnIorJ += '-';
				isNewBlock = false;
			}
			else
			{
				if ((plaintext[i] == 'i' || plaintext[i] == 'j') && (plaintext[i + 1] == 'i' || plaintext[i + 1] == 'j')) 
				{
					bufferInsertedAt += '-';
					bufferInsertedAt += 'x';
					wasAnIorJ += '-';
					isNewBlock = false;
				}
				else if (plaintext[i] == plaintext[i + 1]) //scan for duplicate letters in each block
				{
					bufferInsertedAt += '-';
					bufferInsertedAt += 'x';
					wasAnIorJ += '-'; 
					isNewBlock = false;
				}
				else
				{
					bufferInsertedAt += '-';
					bufferInsertedAt += '-';
				}
			}
		}		
	}

	//Create matrix using key
	for (int i = 0; i < key.size(); i++)
	{
		char letter = tolower(key[i]); //strip case
		if (!encounteredLetter[letter]) //If we haven't encountered the letter before
		{
			if (letter == 'i' || letter == 'j') //first check if it's an i or a j
			{
				encounteredLetter['i'] = true;
				encounteredLetter['j'] = true;
				playfairMatrix[rowPosition][columnPosition] = 'i';
			}
			else 
			{
				encounteredLetter[letter] = true;
				playfairMatrix[rowPosition][columnPosition] = letter;
			}

			columnPosition++;
			if (columnPosition == 5)     //populate all cells in table
			{
				columnPosition = 0;
				rowPosition++;
			}
		}
	}
	//If key is less than 26 characters long, run through the alphabet to populate the rest of the matrix
	while (rowPosition != 5) 
	{
		for (int i = 0; i < 26; i++)
		{
			if (!encounteredLetter[alphabet[i]])
			{
				encounteredLetter[alphabet[i]] = true;
				playfairMatrix[rowPosition][columnPosition] = alphabet[i];
				columnPosition++;
				if (columnPosition == 5)
				{
					columnPosition = 0;
					rowPosition++;
				}
			}
		}
	}

	//The matrix has been built, now we must use it to perform translation
	for (int i = 0; i < plaintext.size();)
	{
		bool paddingUsed = false;
		
		char firstLetter;
		if(plaintext[i] == 'i' || plaintext[i] == 'j')
		{
			firstLetter = 'i';
		}
		else
		{
			firstLetter = plaintext[i];
		}
		pair<int, int> pos1 = findPosition(playfairMatrix, firstLetter);
		char secondLetter; //may be padding, so defer assignment
		pair<int, int> pos2;

		if(((i+1) == plaintext.size())) //If there is an odd amount of characters in input and we are on last letter, pad it with an x
		{
			if (firstLetter == 'x') 
			{
				secondLetter = 'z';
			}
			else
			{
				secondLetter = 'x';
			}
			paddingUsed = true;
		}
		else
		{
			if (plaintext[i+1] == 'i' || plaintext[i+1] == 'j')
			{
				secondLetter = 'i';
			}
			else
			{
				secondLetter = plaintext[i+1]; //may be replaced with padding
			}
		}

		if (firstLetter == secondLetter)
		{
			if (firstLetter == 'x') 
			{
				secondLetter = 'z';
			}
			else
			{
				secondLetter = 'x';
			}
			paddingUsed = true;
		}
		
		pos2 = findPosition(playfairMatrix, secondLetter); //we set pos2 here because we needed to know whether or not we're using a padded letter for secondLetter

		if (pos1.first == pos2.first) //if letters are in the same row
		{
			int col = (pos1.second + 1) % 5; //replace letter with letter to the right of it, wrapping horizontally
			ciphertext += playfairMatrix[pos1.first][col]; //and append it to the ciphertext

			int col2 = (pos2.second + 1) % 5;
			ciphertext += playfairMatrix[pos2.first][col2];
		}
		else if (pos1.second == pos2.second) //if letters are in the same column
		{
			int row = (pos1.first + 1) % 5; //replace letter with letter beneath it, wrapping vertically
			ciphertext += playfairMatrix[row][pos1.second]; //and append it to the ciphertext
			
			int row2 = (pos2.first + 1) % 5;
			ciphertext += playfairMatrix[row2][pos2.second];
		}
		else //if letters are in differing columns and rows
		{
			ciphertext += playfairMatrix[pos1.first][pos2.second]; //first letter is replaced by the letter that falls in the row of the first letter and column of the second letter
			ciphertext += playfairMatrix[pos2.first][pos1.second]; //second letter is replaced by the letter that falls in the row of the second letter and column of the first letter
		}

		if (paddingUsed) 
		{
			i += 1; //processed only one actual letter of the plaintext since we inserted a buffer letter
		}
		else 
		{
			i += 2; //we processed to two letters of the plaintext. no padding was used.
		}
	}

	//ENCODE ALL NECESSARY INFORMATION FOR DECRYPTION*****************************************************************************************************************************************************************
	string encryptionInformation; //this will contain our ciphertext as well as necessary information needed for decryption
	for (int i = 0; i < ciphertext.size(); i++)
	{
		encryptionInformation += ciphertext[i]; //the first line will contain the ciphertext,
	}
	encryptionInformation += ' '; //delimited by a space
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			encryptionInformation += playfairMatrix[i][j]; //first section will contain the encoded playfair matrix
		}
	}
	encryptionInformation += ' '; // delimited by space
	for (int i = 0; i < wasAnIorJ.size(); i++)
	{
		encryptionInformation += wasAnIorJ[i]; //this section will contain the positions of i's and j's
	}
	encryptionInformation += ' '; //delimited by a space
	for (int i = 0; i < capitalizationTracker.size(); i++)
	{
		encryptionInformation += capitalizationTracker[i];  //this section will contain the capitalization of the original plaintext
	}
	encryptionInformation += ' '; //delimited by a space
	for (int i = 0; i < bufferInsertedAt.size(); i++) 
	{
		encryptionInformation += bufferInsertedAt[i]; //this section will contain the location of buffers inserted during playfair encryption
	}

	return encryptionInformation;
}

string Playfair::decrypt(const string& p)
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
	string token = encryptionInformation.substr(0, delimPos); // extract playfair table information
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());
	int cursor = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			playfairMatrix[i][j] = token[cursor]; //populate plaifair matrix
			cursor++;
		}
	}
	delimPos = encryptionInformation.find(delimiter);
	wasAnIorJ = encryptionInformation.substr(0, delimPos); // extract i/j posistioning info
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());

	delimPos = encryptionInformation.find(delimiter);
	capitalizationTracker = encryptionInformation.substr(0, delimPos); // extract capitalization info
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());

	delimPos = encryptionInformation.find(delimiter);
	bufferInsertedAt = encryptionInformation.substr(0, delimPos); // extract buffer positioning info
	encryptionInformation = encryptionInformation.substr(delimPos + 1, encryptionInformation.size());

	//Perform decryption
	string plaintext = "";

	for (int i = 0; i < ciphertext.size();) //perform a rough translation, we will correct padded characters and capitalization later 
	{
		char firstLetter = ciphertext[i];
		pair<int, int> pos1 = findPosition(playfairMatrix, firstLetter);
		char secondLetter = ciphertext[i+1];
		pair<int, int> pos2 = findPosition(playfairMatrix, secondLetter);

		if (pos1.first == pos2.first) //if letters are in the same row
		{
			int col = (pos1.second + 4) % 5; //replace letter with letter to the left of it, wrapping horizontally
			plaintext += playfairMatrix[pos1.first][col]; //and append it to the ciphertext

			int col2 = (pos2.second + 4) % 5;
			plaintext += playfairMatrix[pos2.first][col2];
		}
		else if (pos1.second == pos2.second) //if letters are in the same column
		{
			int row = (pos1.first + 4) % 5; //replace letter with letter beneath it, wrapping vertically
			plaintext += playfairMatrix[row][pos1.second]; //and append it to the ciphertext

			int row2 = (pos2.first + 4) % 5;
			plaintext += playfairMatrix[row2][pos2.second];
		}
		else //if letters are in differing columns and rows
		{
			plaintext += playfairMatrix[pos1.first][pos2.second]; //first letter is replaced by the letter that falls in the row of the first letter and column of the second letter
			plaintext += playfairMatrix[pos2.first][pos1.second]; //second letter is replaced by the letter that falls in the row of the second letter and column of the first letter
		}
		i += 2; //move to next block
	}
	
	//We now have the unofficial plaintext that we must process to retreive original plaintext
	for (int i = 0; i < wasAnIorJ.size(); i++) //indicate i's and j's
	{
		if (plaintext[i] == 'i')
		{
			plaintext[i] = wasAnIorJ[i];
		}
	}

	for (int i = 0; i < bufferInsertedAt.size(); i++) //remove padding for duplicate letters
	{
		if (bufferInsertedAt[i] == 'x')
		{
			plaintext[i] = '-'; 
		}
	}
	plaintext.erase(std::remove(plaintext.begin(), plaintext.end(), '-'), plaintext.end());

	for (int i = 0; i < plaintext.size(); i++) //plaintext should be correct now, just have to adjust capitalization
	{
		if (capitalizationTracker[i] == 'u')
		{
			plaintext[i] = toupper(plaintext[i]);
		}
	}
	
	return plaintext;
}

void Playfair::printMatrix()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			cout << playfairMatrix[i][j] << " ";
		}
		cout << endl;
	}
}

