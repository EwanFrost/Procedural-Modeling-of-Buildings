#ifndef COMMONS_H
#define COMMONS_H

#include <string>
#include <vector>

namespace comn
{
	struct Vector3
	{
	public:
		Vector3(){ Vector3(0, 0, 0); }
		Vector3(double _x, double _y, double _z){ x = _x; y = _y; z = _z; };
		double x;
		double y;
		double z;
	};

	class Node
	{
	private:
		// --- Variables ----------------------		

		// --- Methods ------------------------

	public:
		// --- Variables ----------------------
		std::string SymbolName;
		Vector3 Scale;
		Vector3 Position;
		bool IsActive;
		bool IsDrawable;
		Node* Parent;
		std::vector<Node> Children;

		// --- Methods ------------------------
		Node() : Node("unknown") {};
		Node(std::string symbolName){ IsActive = true; IsDrawable = true; SymbolName = symbolName; }
		~Node(){}
	};

	class Rule
	{
	private:
		// --- Variables ----------------------

		// --- Methods ------------------------

	public:
		// --- Variables ----------------------
		std::string functionName;
		std::vector<std::string> functionParameters;
		std::vector<std::string> childNames;
		double probability;

		// --- Methods ------------------------
		Rule()
		{
			functionName = "";
			probability = 1;
		}
		~Rule(){};
	};
}

#endif