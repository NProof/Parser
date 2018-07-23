#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

struct rule{
	int number;
	string lhs;
	vector<string> rhs;
};

class CFG{
string start;
vector<struct rule> rules;
set<string> non_terminals;
set<string> terminals;

	public:
	CFG(char* file){
		char body[256];
		std::ifstream ifs(file);
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
					non_terminals.insert(currentToken);
				}
			}
			while(iss >> currentToken){
				if(!currentToken.compare(">")||!currentToken.compare("|"))
					throw 1;
				else{
					rules.rbegin()->rhs.push_back(currentToken);
					terminals.insert(currentToken);
				}
			}
		}
		start = rules.begin()->lhs;
		
		// set<string> temp(terminals);
		// terminals.erase(terminals.begin(), terminals.end());
		// std::set_difference (temp.begin(), temp.end(), non_terminals.begin(), non_terminals.end(), std::inserter(terminals, terminals.begin()));
		
		set<string> temp;
		std::set_difference (terminals.begin(), terminals.end(), non_terminals.begin(), non_terminals.end(), std::inserter(temp, temp.begin()));
		terminals = temp;
	}
	
};

int main(int argc, char *argv[]){
	for(int index = 1; index < argc; index++){
		CFG cfg = CFG(argv[index]);
		cout << "terminals.size() : " << cfg.terminals.size() << endl;
	}
	return 0;
}
