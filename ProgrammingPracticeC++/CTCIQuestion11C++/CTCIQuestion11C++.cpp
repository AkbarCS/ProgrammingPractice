#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main()
{	
	string inputString;
	printf("Input String\n");
	getline(cin, inputString);

	vector<char> characters = vector<char>();

	for (size_t i = 0; i < inputString.size(); i++)
	{
		char currentCharacter = inputString[i];
		
		//can be replaced with std::find
		for (size_t j = 0; j < characters.size(); j++)
		{
			if (characters[j] == currentCharacter)
			{
				printf("String does not contain all unique characters\n");
				return 0;
			}
		}

		characters.push_back(inputString[i]);
	}

	printf("String has all unique characters\n");
}