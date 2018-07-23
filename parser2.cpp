#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

struct Rule{
	struct compare{
		public:
		bool operator()(const Rule x,const Rule y) { return x.number<y.number; }
	};
	
	int number;
	string lhs;
	vector<string> rhs;
};

class CFG{
	public:
string start;
vector<struct Rule> rules;
set<string> non_terminals;
set<string> terminals;

map<string, bool> SymbolDerivesEmpty;
map<Rule, bool, Rule::compare> RuleDerivesEmpty;
map<Rule, int, Rule::compare> count;
list<string> worklist;

	CFG(char* file){
		char body[256];
		std::ifstream ifs(file);
		while(ifs.getline (body,256)){
			rules.push_back(Rule());
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
				else if(currentToken.compare("lamda")){
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
		for(vector<struct Rule>::iterator it=rules.begin(); it!=rules.end(); it++){
			if(!it->lhs.compare(A))
				produtionforSet.insert(it->number);
		}
		return produtionforSet;
	}
	
	// OCCURRENCES(X);
	// PRODUCTION(y);
	// TAIL(y);
	
	void DERIVESEMPTYSTRING(){
		// cout << "DERIVESEMPTYSTRING : " << endl;
		for(set<string>::iterator symbol=non_terminals.begin(); symbol!=non_terminals.end(); symbol++){
			// SymbolDerivesEmpty[*symbol] = false;
			SymbolDerivesEmpty.insert(std::pair<string,bool>(*symbol, false));
			// cout << SymbolDerivesEmpty[*symbol] << endl;
		}
		for(vector<struct Rule>::iterator rule=rules.begin(); rule!=rules.end(); rule++){
			RuleDerivesEmpty[*rule] = false;
			// cout << "false : " << rule->number << endl;
			count[*rule] = 0;
			for(vector<string>::iterator token = rule->rhs.begin(); token!=rule->rhs.end(); token++){
				count[*rule]++;
			}
			CHECKFOREMPTY(*rule);
		}
		while(!worklist.empty()){
			string X = worklist.front();
			// cout << "X : " << X << endl;
			worklist.pop_front();
			for(vector<struct Rule>::iterator rule=rules.begin(); rule!=rules.end(); rule++){
				for(vector<string>::iterator token = rule->rhs.begin(); token!=rule->rhs.end(); token++){
					if(!X.compare(*token)){
						count[*rule]--;
						CHECKFOREMPTY(*rule);
					}
				}
			}
		}
		// for(map<string, bool>::iterator it = SymbolDerivesEmpty.begin(); it!= SymbolDerivesEmpty.begin(); it++){
			// cout << it->first << " / " << it->second << endl;
		// }
		for(set<string>::iterator symbol=non_terminals.begin(); symbol!=non_terminals.end(); symbol++){
			cout << *symbol << " " << SymbolDerivesEmpty[*symbol] << endl;
		}
	}
	
	void CHECKFOREMPTY(Rule rule){
		if(count[rule] == 0){
			RuleDerivesEmpty[rule] = true;
			string symbol = rule.lhs;
			// cout << "BOOL : " << SymbolDerivesEmpty[symbol] << endl;
			if(!SymbolDerivesEmpty[symbol]){
				SymbolDerivesEmpty[symbol] = true;
				worklist.push_back(symbol);
			}
		}
	}
	
};

int main(int argc, char *argv[]){
	for(int index = 1; index < argc; index++){
		CFG cfg = CFG(argv[index]);
		cfg.DERIVESEMPTYSTRING();
	}
	return 0;
}
