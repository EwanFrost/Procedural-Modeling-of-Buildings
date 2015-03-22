#include "Parser.h"
#include <iostream>
#include <fstream>

using namespace std;

namespace pars
{
	Parser::Parser()
	{

	}

	Parser::~Parser()
	{

	}

	string trim(string str)
	{
		size_t endpos = str.find_last_not_of(" \t\n\r");
		if (string::npos != endpos)
		{
			str = str.substr(0, endpos + 1);
		}

		size_t startpos = str.find_first_not_of(" \t\n\r");
		if (string::npos != startpos)
		{
			str = str.substr(startpos);
		}

		return str;
	}

	void printRule(comn::Rule rule)
	{
		cout << "Function name: " << rule.functionName << endl;

		cout << "Function parameters: { ";
		for (int i = 0; i < rule.functionParameters.size(); i++)
		{
			cout << rule.functionParameters[i] << " ";
		}
		cout << "}" << endl;

		cout << "Children names: { ";
		for (int i = 0; i < rule.childNames.size(); i++)
		{
			cout << rule.childNames[i] << " ";
		}
		cout << "}" << endl;

		cout << "Probability: " << rule.probability << endl << endl;
	}

	void printMap(std::unordered_map < std::string, std::vector<comn::Rule>> ruleSet)
	{
		for (std::unordered_map<std::string, std::vector<comn::Rule>>::iterator it = ruleSet.begin(); it != ruleSet.end(); ++it)
		{
			cout << "KEY: " << it->first << endl << endl;
			for (int j = 0; j < (it->second).size(); j++)
			{
				printRule(it->second[j]);
			}

			cout << "------------------------------------------------" << endl;
		}
	}

	void Parser::Parse()
	{
		// parse the Rules.txt and fill the variable ruleSet

		string line;
		ifstream rulesFile("..\\BuildingGenerator\\Files\\Rules.txt");

		if (rulesFile.is_open())
		{
			while (getline(rulesFile, line))
			{
				if (line.length() < 3)
					continue;

				size_t pos = line.find("->");

				string key = line.substr(0, pos - 1);
				key = trim(key);
				string ruleBody = line.substr(pos + 3, string::npos);
				comn::Rule rule;
			
				pos = ruleBody.find("(");
				if (pos == string::npos)
				{
					rule.childNames.push_back(trim(ruleBody));
				}
				else
				{
					char charArray[100];

					rule.functionName = trim(ruleBody.substr(0, pos));
					string parameters = ruleBody.substr(pos+1, ruleBody.find(")") - pos - 1);

					strcpy(charArray, parameters.c_str());
					char *p = strtok(charArray, ",");
					while (p) 
					{
						rule.functionParameters.push_back(p);
						p = strtok(NULL, ",");
					}

					pos = ruleBody.find("{");

					if (pos != string::npos)
					{
						string children = ruleBody.substr(pos + 1, ruleBody.find("}") - pos - 1);

						strcpy(charArray, children.c_str());
						p = strtok(charArray, "|");
						while (p)
						{
							rule.childNames.push_back(p);
							p = strtok(NULL, "|");
						}
					}

					pos = ruleBody.find(":");
					if (pos != string::npos)
					{
						rule.probability = stod(ruleBody.substr(pos + 1, ruleBody.find("\n") - pos - 1));
					}
				}

				if (ruleSet.find(key) == ruleSet.end()) 
				{
					vector<comn::Rule> rules;
					rules.push_back(rule);
					ruleSet.insert(pair<string, vector<comn::Rule>>(key, rules));
				}
				else
				{
					ruleSet[key].push_back(rule);
				}
					
			}
			rulesFile.close();
			
		}

		else 
		{ 
			cout << "Unable to open Rules.txt\n"; 
		}

		//printMap(ruleSet);
		//system("pause");
	}

	std::unordered_map < std::string, std::vector<comn::Rule>> Parser::GenerateTestData()
	{
		std::unordered_map < std::string, std::vector<comn::Rule>> rules;
		return rules;
	}
}
