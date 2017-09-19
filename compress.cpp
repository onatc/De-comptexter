//--------------------------------------------------------
// You can only use these headers

#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>

#include "treap_bst.h"

//--------------------------------------------------------


/*
*  Purpose of program
*  This program compresses and decompresses a text file.
*  Compress: filename inputfile outputfile
*  Decompress: filename "-d" inputfile outputfile
*
*  Programmer: Onat Calik
*  Date: December 07, 2016
*
*  Honor Code: I have neither given nor received any unauthorized assistance with this program.
*/

int main(int argc, char *argv[])
{

	if (argc > 4 || argc <= 0)
	{
		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "-d") == 0)       //decompress
	{
		std::ifstream din(argv[2], std::ios::binary);
		std::ofstream dout(argv[3]);

		if (!din)
		{
			EXIT_FAILURE;
		}

		if (!dout)
		{
			EXIT_FAILURE;
		}
		TreapBST<long, std::string> dictionary;
		bool f;
		std::vector<long> keys = {};

		uint32_t key;
		char value;
		din.read(reinterpret_cast<char*>(&key), sizeof key);

		while (key != 0)
		{
			din.read(reinterpret_cast<char*>(&value), sizeof value);
			std::string myStr = "";
			while (value != 0)
			{
				myStr += value;
				din.read(reinterpret_cast<char*>(&value), sizeof value);
			}

			dictionary.insert(key, myStr);
			din.read(reinterpret_cast<char*>(&key), sizeof key);
		}

		uint32_t order;
		din.read(reinterpret_cast<char*>(&order), sizeof order);

		while (!din.fail())
		{
			bool f;
			std::string finalStr = dictionary.search(order, f);

			if (f)
			{
				dout << finalStr << " ";
			}

			din.read(reinterpret_cast<char*>(&order), sizeof order);

		}



	}

	else                 //compress
	{
		std::ifstream in(argv[1]);
		std::ofstream out(argv[2], std::ios::binary);

		if (!in)
		{
			EXIT_FAILURE;
		}

		if (!out)
		{
			EXIT_FAILURE;
		}

		std::string str;
		bool f;
		std::vector<long> keys = { 0 };
		TreapBST<std::string, long> dictionary;
		uint32_t count = 1;

		in >> str;
		while (!in.fail())
		{

			long temp = dictionary.search(str, f);

			if (!f) //if not in dictionary add to the dictionary, write, add to vector
			{
				uint32_t num1 = count;
				out.write(reinterpret_cast<const char*>(&num1), sizeof num1);

				for (int i = 0; i < str.length(); i++)
				{
					uint8_t num2 = str[i];
					out.write(reinterpret_cast<const char*>(&num2), sizeof num2);
				}

				uint8_t num3 = 0;
				out.write(reinterpret_cast<const char*>(&num3), sizeof num3);
				keys.push_back(count);
				dictionary.insert(str, count);
				count++;
			}

			else  //if in dictionary add to vector
			{
				keys.push_back(temp);
			}

			in >> str;
		}

		for (int i = 0; i < keys.size(); i++) //order of the words 
		{
			uint32_t num = keys.at(i);
			out.write(reinterpret_cast<const char*>(&num), sizeof num);
		}
	}

	return EXIT_SUCCESS;
}

