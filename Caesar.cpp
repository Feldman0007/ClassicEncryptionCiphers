#include "Caesar.h"

bool Caesar::setKey(const string& inputkey) 
{
	//Key should be a number, check if there are any non-numeric characters
	int i = 0;
	if (inputkey[0] == '-') //we can technically have a negative key, so if there is a negative key ignore it for now
	{
		i++;
	}
	for (; i < inputkey.size(); i++)
	{
		if (!isdigit(inputkey[i]))
		{
			cout << "Caesar Cipher key should be a positive integer";
			return false;
		}
	}
	key = inputkey;
	return true;
}

string Caesar::encrypt(const string& plaintext) 
{
	if (plaintext.size() == 0)
	{
		return "";
	}
	int intKey = std::stoi(key); //convert key to integer
	string cipherText = "";

	//transform each letter in original phrase
	for (int i = 0; i < plaintext.size(); i++)
	{
		//Handle uppercase letters
		if (isupper(plaintext[i]))
		{
			cipherText += char(int(plaintext[i] + intKey - 65) % 26 + 65);
		}
		else
		{
			cipherText += char(int(plaintext[i] + intKey - 97) % 26 + 97);
		}
	}
	return cipherText;
}

string Caesar::decrypt(const string& cipherText) 
{
	if (cipherText.size() == 0)
	{
		return "";
	}
	int intKey = std::stoi(key); //convert key to integer
	string plainText = "";

	//transform each letter in original phrase
	for (int i = 0; i < cipherText.size(); i++)
	{
		//Handle uppercase letters
		if (isupper(cipherText[i]))
		{
			plainText += char(int(abs((cipherText[i] - intKey - 65 + 26))) % 26 + 65);
		}
		else
		{
			plainText += char(int(abs((cipherText[i] - intKey - 97 + 26))) % 26 + 97);
		}
	}
	return plainText;
}
