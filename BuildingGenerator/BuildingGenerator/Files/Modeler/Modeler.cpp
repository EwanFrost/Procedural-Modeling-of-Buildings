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

	vector<Symbol*>queue;

	//Symbol* tree;
	
	void Modeler::init(){	
				//initialize root of Tree with parameters - name, position, scale
			
		        tree = new Symbol("start");
				tree->setPosition(0.0, 0.0, 0.0);
				tree->setScale(1.0,1.0,1.0);				
				queue.push_back(tree);			
				
	}

	void Modeler::printTree(){
		
				//after building derivation tree, print is being done, by starting from root node and using BFS logic

				/*ofstream cout("Files\\output.txt");				

				queue.push_back(tree);			
				
				cout << "------------------------------------------------" << endl;
				while(queue.size()){
					Symbol* cur_item = queue[0];				
				    queue.erase(queue.begin());	
					cout<<"Symbol Name: "<<cur_item->getSymbolName()<<endl;
					Vector3 position = cur_item->getPosition();
					cout<<"Position: ("<<position.x<<", "<<position.y<<", "<<position.z<<")"<<endl;
					Vector3 scale = cur_item->getScale();
					cout<<"Scale: ("<<scale.x<<", "<<scale.y<<", "<<scale.z<<")"<<endl;
					cout<<"Children: { ";
					for(int i = 0;i<cur_item->Children.size();i++){
						Symbol *child = cur_item->Children[i];
						cout<<child->getSymbolName()<<" ";
						queue.push_back(child);
					}
					cout << "}"<<endl;
				cout << "------------------------------------------------" << endl;
					
				}
				
				cout.close();*/
	
	}

	double createRandom(){	
			//generates random number between 0 and 1		
			srand(time(NULL));
			return ((double)(rand()%100)+1)/100.0;
	}

	void Modeler::Model(std::unordered_map < std::string, std::vector<comn::Rule>> ruleSet)
	{
				/*
				 By using BFS approach, each time one element is being selected and children are created from it.
				 Those nodes are components of shape.
				 */

				double  probability = createRandom();	

				int index = (probability<=0.40)?1:0;	
		
				init();		
		
				int pre_index = index;

				while(queue.size()){				    
					Symbol *cur_item = queue[0];				
				    queue.erase(queue.begin());		
					
					string shape_name = cur_item->getSymbolName();
					

					if(ruleSet.find(shape_name)!=ruleSet.end()){  //searchs for shape name in given map
					vector<Rule>rules = ruleSet.at(shape_name);
					index = (rules.size()>1)?index:0;
					string function_name = rules[index].functionName;		
					
					if(function_name.compare("comp")==0){						
						comp(rules[index].childNames,cur_item);
					}else if(function_name.compare("S")==0){						
						S(rules[index].functionParameters, rules[index].childNames, cur_item);					
					}else if(function_name.compare("repeat")==0){
						repeat(rules[index].functionParameters, rules[index].childNames,cur_item);
					}else if(function_name.compare("subDiv")==0){
						subDiv(rules[index].functionParameters, rules[index].childNames,cur_item);
					}else{
						nullFunction(rules[index].childNames,cur_item);
					}					
					}
					index = pre_index;
			   }  			
					printTree();
	}


	comn::Symbol* Modeler::GenerateTestData()
	{   
		comn::Symbol* node = new Symbol();
		return node;
	}
	
	void Modeler::print(Symbol* item){
		/*
				cout<<item->getSymbolName()<<"-> ";
				cout<<"Position: ";
				Vector3 pos = item->getPosition();
				for(int k = 0;k<pos.size();k++)cout<<pos[k]<<" ";
				cout<<endl;
				cout<<"Scale: ";
				pos = item->getScale();
				for(int k = 0;k<pos.size();k++)cout<<pos[k]<<" ";
				cout<<endl<<endl<<endl;
				*/
		
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


	void Modeler::comp(vector<string> childSymbolNames,Symbol* Parent){
		//create Children

		for(int i = 0;i<childSymbolNames.size();i++){		

			Symbol* item = new Symbol(childSymbolNames[i]);		
			Vector3 parentPos = Parent->getPosition();
			Vector3 parentScale = Parent->getScale();

			if(childSymbolNames[i].compare("front")==0){				
				item->setPosition(parentPos.x, parentPos.y, parentPos.z);
				item->setScale(parentScale.x, parentScale.y,0.0);
			}
			if(childSymbolNames[i].compare("left_facade")==0){
				item->setPosition(parentPos.x, parentPos.y, parentPos.z);
				item->setScale(0, parentScale.y,parentScale.z);
			}
			if(childSymbolNames[i].compare("right_facade")==0){
				item->setPosition(parentPos.x+parentScale.x, parentPos.y, parentPos.z);
				item->setScale(0, parentScale.y,parentScale.z);
			}
			if(childSymbolNames[i].compare("back")==0){
				item->setPosition(parentPos.x, parentPos.y, parentPos.z+parentScale.z);
				item->setScale(parentScale.x, parentScale.y,0.0);
			}
			if(childSymbolNames[i].compare("bottom")==0){
				item->setPosition(parentPos.x, parentPos.y, parentPos.z);
				item->setScale(parentScale.x,0.0 ,parentScale.z);
			}
			if(childSymbolNames[i].compare("top")==0){
				item->setPosition(parentPos.x, parentPos.y+parentScale.y, parentPos.z);
				item->setScale(parentScale.x,0.0 ,parentScale.z);
			}	
				
			Parent->Children.push_back(item);	
			queue.push_back(item);
			//print(item);
		
			
		}
		
	}


	void Modeler::insertMap1(int id, string symbolName){
		map<string, vector<int>>::iterator it;
		it = map1.find(symbolName);
		if(it!=map1.end()){			
			it->second.push_back(id);
		}else{
			vector<int>v_map1;
			v_map1.push_back(id);
			map1.insert(pair<string,vector<int>>(symbolName,v_map1));
		}
	}
	
	void Modeler::subDiv(vector<string> parameterValues, vector<string> childSymbolNames, Symbol* Parent){
		//regard to which coordination divide
		//with which sizes
		//children names
		//scales and positions of children
		

		string axis = parameterValues[0];		
		Vector3 parentPos = Parent->getPosition();
		Vector3 parentScale = Parent->getScale();
		if(axis.compare("\"X\"")==0){	

		double prepos= parentPos.x;	

		for(int i = 0;i<childSymbolNames.size();i++){				
			Symbol* item = new Symbol(childSymbolNames[i]);				
			item->setPosition(prepos,parentPos.y, parentPos.z);
			item->setScale(nextDouble(parameterValues[i+1]), parentScale.y, parentScale.z);
			prepos+=nextDouble(parameterValues[i+1]);
			
			Parent->Children.push_back(item);	
			queue.push_back(item);

		}
		}else if(axis.compare("\"Y\"")==0){		
			double prepos = parentPos.y;
			
			for(int i = 0;i<childSymbolNames.size();i++){
				Symbol* item = new Symbol(childSymbolNames[i]);					
				item->setPosition(parentPos.x,prepos, parentPos.z);
				item->setScale(parentScale.x, nextDouble(parameterValues[i+1]), parentScale.z);
			
				prepos+=nextDouble(parameterValues[i+1]);
				
				Parent->Children.push_back(item);	
				queue.push_back(item);
				
			}
		}else{
			double prepos = parentPos.z;	

			for(int i = 0;i<childSymbolNames.size();i++){		
				Symbol* item = new Symbol(childSymbolNames[i]);		
				item->setPosition(parentPos.x, parentPos.y,prepos);
				item->setScale(parentScale.x, parentScale.y, nextDouble(parameterValues[i+1]));		
				prepos+=nextDouble(parameterValues[i+1]);

				
				Parent->Children.push_back(item);		
				queue.push_back(item);
				
			}
		}
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