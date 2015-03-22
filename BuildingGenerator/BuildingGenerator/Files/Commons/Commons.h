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
		Vector3(Vector3& other) { x = other.x; y = other.y; z = other.z; }
		Vector3(float _x, float _y, float _z){ x = _x; y = _y; z = _z;};
		float x;
		float y;
		float z;
	};

	class Node
	{
	private:
		// --- Variables ----------------------		
<<<<<<< HEAD
		std::string SymbolName;
		std::string Texture = "";
		Vector3 scale;
		Vector3 position;
		bool IsActive;
		bool IsDrawable;			
	
=======

>>>>>>> 0bccdc6449e680e5c82b40141274fcec35ca960f
		// --- Methods ------------------------

	public:
		// --- Variables ----------------------
<<<<<<< HEAD
		std::vector<Symbol*> Children;
			
		// --- Methods ------------------------		
	
		void setSymbolName(string name){
			SymbolName = name;
		}	
		string getSymbolName(){
			return SymbolName;
		}
		void setScale(double sx, double sy, double sz){
			scale.x = sx;
			scale.y = sy;
			scale.z = sz;
		}
		Vector3 getScale(){
			return scale;
		}	
		void setPosition(double sx, double sy, double sz){
			position.x = sx;
			position.y = sy;
			position.z = sz;
	
		}	
		Vector3 getPosition(){
			return position;
		}
		void setActiveness(bool state){
			IsActive = state;
		}
		bool getActiveness(){
			return IsActive;
		}
		void setDrawableness(bool state){
			IsDrawable = state;
		}
		bool getDrawableness(){
			return IsDrawable;
		}
		Symbol() {};
		Symbol(std::string symbolName){ IsActive = true; IsDrawable = true; SymbolName = symbolName;}
		~Symbol(){}
=======
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
>>>>>>> 0bccdc6449e680e5c82b40141274fcec35ca960f
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