#ifndef COMMONS_H
#define COMMONS_H

#include <string>
#include <vector>
#include <map>
using namespace std;

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

	class Symbol
	{
	private:
		// --- Variables ----------------------		
		std::string SymbolName;
		Vector3 scale;
		Vector3 position;
		bool IsActive;
		bool IsDrawable;			
	
		// --- Methods ------------------------

	public:
		// --- Variables ----------------------
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