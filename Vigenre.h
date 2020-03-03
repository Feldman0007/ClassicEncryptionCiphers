#pragma once
#ifndef VIGENRE_H
#define VIGENRE_H

#include <vector>   /* For vectors */
#include <string>   /* For C++ strings */
#include <stdio.h>  /* For standard I/O */
#include <stdlib.h> /* For miscellenous C functions */
#include "CipherInterface.h"
#include <iostream>

using namespace std;

/**
 * This class implements the playfair cipher.
 * The class extends the abstract class
 * CipherInterface.
 */

class Vigenre : public CipherInterface
{
	/** The public members **/
public:

	Vigenre();
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

	string alphabet = "abcdefghijklmnopqrstuvwxyz";
	string capitalizationTracker = "";
	char vigenreMatrix[26][26];


};

#endif