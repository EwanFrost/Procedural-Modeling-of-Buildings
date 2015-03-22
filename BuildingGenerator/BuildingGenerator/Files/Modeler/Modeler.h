#ifndef MODELING_H
#define MODELING_H

#include "..\Commons\Commons.h"
#include "..\Source.h"
#include <vector>
#include <unordered_map>
#include <map>
#include <string>

namespace modl
{
	class Modeler
	{
	private:
		// --- Variables ----------------------
		comn::Node tree;

		// --- Methods ------------------------
		comn::Node GenerateTestData();
	public:
		Modeler();
		~Modeler();
		void Model(std::unordered_map < std::string, std::vector<comn::Rule>> ruleSet);
		comn::Node GetDeviationTree() { 
			if (USE_TEST_DATA) 
				return GenerateTestData(); 
			else 
				return tree;
		}
	};
}

#endif