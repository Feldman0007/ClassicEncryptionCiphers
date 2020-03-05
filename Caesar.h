#pragma once
#ifndef CAESAR_H
#define CAESAR_H

#include "CipherInterface.h"
#include <iostream>
#inlcude <sstream>

class Caesar : public CipherInterface
{
	public:

		bool setKey(const string& key);

		string encrypt(const string& plaintext);

		string decrypt(const string& ciphertext);
};

#endif
