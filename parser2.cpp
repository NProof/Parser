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
	public:
string start;
vector<struct rule> rules;
set<string> non_terminals;
set<string> terminals;

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
		set<string> temp;
		std::set_difference (terminals.begin(), terminals.end(), non_terminals.begin(), non_terminals.end(), std::inserter(temp, temp.begin()));
		terminals = temp;
	}
	
	set<int> produtionsfor(string A){
		set<int> produtionforSet;
		for(vector<struct rule>::iterator it=rules.begin(); it!=rules.end(); it++){
			if(!it->lhs.compare(A))
				produtionforSet.insert(it->number);
		}
		return produtionforSet;
	}
};

int main(int argc, char *argv[]){
	for(int index = 1; index < argc; index++){
		CFG cfg = CFG(argv[index]);
		set<int> produtionforSet = cfg.produtionsfor("E");
		cout << produtionforSet.size() << endl;
		for(set<int>::iterator it=produtionforSet.begin(); it!=produtionforSet.end(); it++){
			cout << " " << *it ;
		}
		cout << endl;
	}
	return 0;
}
