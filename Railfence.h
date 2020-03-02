#pragma once
#ifndef RAILFENCE_H
#define RAILFENCE_H

#include <vector>   /* For vectors */
#include <string>   /* For C++ strings */
#include <stdio.h>  /* For standard I/O */
#include <stdlib.h> /* For miscellenous C functions */
#include "CipherInterface.h"
#include <sstream>

using namespace std;

/**
 * This class implements the playfair cipher.
 * The class extends the abstract class
 * CipherInterface.
 */

class Railfence : public CipherInterface
{
	/** The public members **/
public:

	/**
	 * Sets the key to use
	 * @param key - the key to use
	 * @return - True if the key is valid and False otherwise
	 */
	bool setKey(const string& key);

	/**
	 * Encrypts a plaintext string
	 * @param plaintext - the plaintext string
	 * @return - the encrypted ciphertext string
	 */
	string encrypt(const string& plaintext);

	/**
	 * Decrypts a string of ciphertext
	 * @param ciphertext - the ciphertext
	 * @return - the plaintext
	 */
	string decrypt(const string& ciphertext);

	/**
	 * Prints the Playfair matrix
	 * @param fp - the file pointer
	 */
	void printMatrix(FILE* fp);

	/* The protected members */
protected:

	char rails[100][100];
	char showUnderstanding[100][100]; //not necessary, but will repopulate it using ciphertext to show understanding of decryption
	string capitalizationTracker = "";

};

#endif