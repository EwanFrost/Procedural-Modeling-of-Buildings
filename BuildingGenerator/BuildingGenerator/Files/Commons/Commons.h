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
		Vector3(Vector3& other) { x = other.x; y = other.y; z = other.z; }
		Vector3(float _x, float _y, float _z){ x = _x; y = _y; z = _z;};
		float x;
		float y;
		float z;
	};

	class Symbol
	{
	private:
		// --- Variables ----------------------		
		std::string SymbolName;
		std::string Texture = ""; 
		float Opacity = 1.0f;
		Vector3 scale;
		Vector3 position;
		bool IsActive;
		bool IsDrawable;			
	
		// --- Methods ------------------------

	public:
		// --- Variables ----------------------
		std::vector<Symbol*> Children;
			
		// --- Methods ------------------------		
		void setTexture(string texture) { Texture = texture; }
		string getTexture() { return Texture; }
		void setOpacity(float opacity) { Opacity = opacity; }
		float getOpacity() { return Opacity; }
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