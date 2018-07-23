#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct rule{
	int number;
	string lhs;
	vector<string> rhs;
};

int main(int argc, char *argv[]){
	for(int index = 1; index < argc; index++){
		// cout << " [ " << index << " ] : " << argv[index] << endl;
		char body[256];
		std::ifstream ifs(argv[index]);
		vector<struct rule> rules;
		while(ifs.getline (body,256)){
			rules.push_back(rule());
			string stringvalues(body);
			std::istringstream iss (stringvalues);
			iss >> rules.rbegin()->number;
			string currentToken;
			while(iss >> currentToken){
				if(!currentToken.compare(">")||!currentToken.compare("|")){
					if(!currentToken.compare("|")){
						rules.rbegin()->lhs = (rules.rbegin()+1)->lhs;
					}
					break;
				}
				else{
					rules.rbegin()->lhs = currentToken;
				}
			}
			while(iss >> currentToken){
				if(!currentToken.compare(">")||!currentToken.compare("|"))
					throw 1;
				else{
					rules.rbegin()->rhs.push_back(currentToken);
				}
			}
		}
		
		cout << "rules.size() = " << rules.size() << endl;
		
		for(vector<struct rule>::iterator it=rules.begin(); it!=rules.end(); it++){
			cout << "number : " << it->number << endl;
			cout << "lhs : " << it->lhs << endl;
			cout << "rhs : " ;
			for(vector<string>::iterator it2= it->rhs.begin(); it2!= it->rhs.end(); it2++){
				cout << *it2 << " ";
			}
			cout << endl;
		}
	}
	return 0;
}
