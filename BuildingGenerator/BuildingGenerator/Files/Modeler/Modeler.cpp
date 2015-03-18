#include "Modeler.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <ctime>

using namespace std;
using namespace comn;

/*
Modeler.cpp reads output of Parsing and generates Derivation Tree. 
*/

namespace modl
{
	Modeler::Modeler()
	{

	}
	Modeler::~Modeler()
	{

	}
	void Modeler::Model(std::unordered_map < std::string, std::vector<comn::Rule>> ruleSet)
	{
		// uses the ruleSet to construct the deviation tree
	}
	
	comn::Symbol* Modeler::GenerateTestData()
	{   
		comn::Symbol* node = new Symbol();
		return node;
	}
	
	//  helper function to parse string
	int Modeler::nextInt(string s){
		return stoi(s);
	}
	//  helper function to parse string
	double Modeler::nextDouble(string s){
		return stod(s);
	}

	/*
		nullFunction, comp, subDiv, S functions were defined. Each of them does different operation on building component and creates new 
		children for given shape object.
	*/
	void Modeler::nullFunction(vector<string>childSymbolNames, Symbol* Parent ){	
				/*
					When no function was specified in ruleSet, this function is being used. 
				*/
				Vector3 parentPos = Parent->getPosition();
				Vector3 parentScale = Parent->getScale();
				Symbol* item = new Symbol(childSymbolNames[0]);						
				item->setPosition(parentPos.x,parentPos.y, parentPos.z);
				item->setScale(parentScale.x, parentScale.y, parentScale.z);			
				Parent->Children.push_back(item);
				queue.push_back(item);
				//print(item);
	}

	void Modeler::repeat(vector<string> parameterValues, vector<string> childSymbolNames, Symbol* Parent){
	 //regard to which coordinate
		string axis = parameterValues[0];
		int width = nextInt(parameterValues[1]);
		int repetation;
		
		
		Vector3 parentPos = Parent->getPosition();
		Vector3 parentScale = Parent->getScale();
		
		
		if(axis.compare("\"X\"")==0){		
			repetation = Parent->getScale().x/width;			
			for(int i = 0;i<repetation;i++){		 
			 Symbol* item = new Symbol(childSymbolNames[0]);						 
			 item->setPosition(i*width, parentPos.y, parentPos.z);
			 item->setScale(width, parentScale.y, parentScale.z);
			 
			    Parent->Children.push_back(item);	
				queue.push_back(item);
				//print(item);
			}
		}else{			
			repetation = Parent->getScale().z/width;			
			for(int i = 0;i<repetation;i++){			
			 Symbol* item = new Symbol(childSymbolNames[0]);			
			 item->setPosition(parentPos.x, parentPos.y, i*width);
			 item->setScale(parentScale.x, parentScale.y,width);			 
			 Parent->Children.push_back(item);	
			 queue.push_back(item);
		     
			}
		}
     //how many times
	 //childName
	 //scales and coordinates for children
	 //do map operations
	}

	void Modeler::S(vector<string> parameterValues, vector<string> childSymbolNames, Symbol* Parent){
		
	 //new sizes for new object
		Vector3 parentPos = Parent->getPosition();		
		if(childSymbolNames.size()==0){		
				Parent->setScale(nextDouble(parameterValues[0]), nextDouble(parameterValues[1]), nextDouble(parameterValues[2]));				
		}else{				
				Symbol* item = new Symbol(childSymbolNames[0]);					
				item->setPosition(parentPos.x,parentPos.y, parentPos.z);
				item->setScale(nextDouble(parameterValues[0]), nextDouble(parameterValues[1]), nextDouble(parameterValues[2]));			
				Parent->Children.push_back(item);		
				queue.push_back(item);		   		
		}			
	 
	}
	
}