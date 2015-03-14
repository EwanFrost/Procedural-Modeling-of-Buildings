#ifndef PARSER_H
#define PARSER_H

#include "..\Commons\Commons.h"
#include "..\Source.h"
#include <vector>
#include <unordered_map>
#include <string>

namespace pars
{
	class Parser
	{
	private:
		// --- Variables ----------------------
		// pair < SymbolID, vector<Node> >
		std::unordered_map < std::string, std::vector<comn::Rule>> ruleSet;

		// --- Methods ------------------------
		std::unordered_map < std::string, std::vector<comn::Rule>> GenerateTestData();
	public:
		Parser();
		~Parser();
		void Parse();
		std::unordered_map < std::string, std::vector<comn::Rule>> GetRuleSet() {
			if (USE_TEST_DATA) 
				return GenerateTestData(); 
			else 
				return ruleSet; }

	};
}

#endif