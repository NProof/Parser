#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <stack>
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
				else if(!currentToken.compare("")){
					continue;
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
				else if(!currentToken.compare("")){
					continue;
				}
			}
		}
		start = rules.begin()->lhs;
		set<string> temp;
		std::set_difference (terminals.begin(), terminals.end(), non_terminals.begin(), non_terminals.end(), std::inserter(temp, temp.begin()));
		terminals = temp;
		
		DERIVESEMPTYSTRING();
	}
	
	set<Rule, Rule::compare> produtionsfor(string A){
		set<Rule, Rule::compare> produtionforSet;
		for(vector<struct Rule>::iterator rule=rules.begin(); rule!=rules.end(); rule++){
			if(!rule->lhs.compare(A))
				produtionforSet.insert(*rule);
		}
		return produtionforSet;
	}
	
	void DERIVESEMPTYSTRING(){
		for(set<string>::iterator symbol=non_terminals.begin(); symbol!=non_terminals.end(); symbol++){
			SymbolDerivesEmpty.insert(std::pair<string,bool>(*symbol, false));
		}
		for(vector<struct Rule>::iterator rule=rules.begin(); rule!=rules.end(); rule++){
			RuleDerivesEmpty[*rule] = false;
			count[*rule] = 0;
			for(vector<string>::iterator token = rule->rhs.begin(); token!=rule->rhs.end(); token++){
				count[*rule]++;
			}
			CHECKFOREMPTY(*rule);
		}
		while(!worklist.empty()){
			string X = worklist.front();
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
	}
	
	void CHECKFOREMPTY(Rule rule){
		if(count[rule] == 0){
			RuleDerivesEmpty[rule] = true;
			string symbol = rule.lhs;
			if(!SymbolDerivesEmpty[symbol]){
				SymbolDerivesEmpty[symbol] = true;
				worklist.push_back(symbol);
			}
		}
	}
	map<string, bool> visitedFirst;
	
	set<string> first(vector<string> vectorStr){
		for(set<string>::iterator symbol=non_terminals.begin(); symbol!=non_terminals.end(); symbol++){
			visitedFirst[*symbol] = false;
		}
		// for(map<string, bool>::iterator it = visitedFirst.begin(); it!= visitedFirst.end(); it++){
			// cout << it->first << " / " << it->second << endl;
		// }
		return internalFirst(vectorStr);
	}
	
	set<string> internalFirst(vector<string> vectorStr){
		if(vectorStr.empty()){
			return set<string>();
		}
		else{
			set<string> returnSet;
			string head = *(vectorStr.begin());
			if(terminals.count(head)){
				returnSet.insert(head);
				return returnSet;
			}
			if(!visitedFirst[head]){
				visitedFirst[head] = true;
				set<Rule, Rule::compare> produtionsforSet = produtionsfor(head);
				for(set<Rule, Rule::compare>::iterator rule=produtionsforSet.begin(); rule!= produtionsforSet.end(); rule++){
					vector<string> pr = rule->rhs;
					for(vector<string>::iterator token = pr.begin(); token!=pr.end(); token++){
						set<string> unset = internalFirst(rule->rhs);
						set<string> temp;
						std::set_union (returnSet.begin(), returnSet.end(), unset.begin(), unset.end(), std::inserter(temp, temp.begin()));
						returnSet = temp;
					}
				}
			}
			if(SymbolDerivesEmpty[head]){
				vector<string> vFollow(vectorStr.begin()+1, vectorStr.end());
				set<string> unset = internalFirst(vFollow);
				std::set_union (returnSet.begin(), returnSet.end(), unset.begin(), unset.end(), std::inserter(returnSet, returnSet.begin()));
			}
			return returnSet;
		}
	}
	
	map<string, bool> visitedFollow;
	set<string> follow(string A){
		for(set<string>::iterator symbol=non_terminals.begin(); symbol!=non_terminals.end(); symbol++){
			visitedFollow[*symbol] = false;
		}
		return internalFollow(A);
	}
	
	set<string> internalFollow(string A){
		set<string> ans;
		if(!visitedFollow[A]){
			visitedFollow[A] = true;
			for(vector<struct Rule>::iterator rule=rules.begin(); rule!=rules.end(); rule++){
				for(vector<string>::iterator token = rule->rhs.begin(); token!=rule->rhs.end(); token++){
					if(!A.compare(*token)){
						// cout << "\n" << rule->number << *token << endl;
						vector<string> tail(token+1, rule->rhs.end());
						set<string> temp = first(tail);
						set<string> temp2;
						std::set_union (ans.begin(), ans.end(), temp.begin(), temp.end(), std::inserter(temp2, temp2.begin()));
						ans = temp2;
						if(allDeriveEmpty(tail)){
							set<string> temp3 = internalFollow(rule->lhs);
							set<string> temp4;
							std::set_union (ans.begin(), ans.end(), temp3.begin(), temp3.end(), std::inserter(temp4, temp4.begin()));
							ans = temp4;
						}
					}
				}
			}
		}
		return ans;
	}
	
	bool allDeriveEmpty(vector<string> vStr){
		for(vector<string>::iterator vIterator = vStr.begin(); vIterator!=vStr.end(); vIterator++){
			if(!SymbolDerivesEmpty[*vIterator]||terminals.count(*vIterator))
				return false;
		}
		return true;
	}
	
	set<string> predict(Rule rule){
		set<string> ans = first(rule.rhs);
		if(RuleDerivesEmpty[rule]){
			set<string> temp = follow(rule.lhs);
			std::set_union (ans.begin(), ans.end(), temp.begin(), temp.end(), std::inserter(ans, ans.begin()));
		}
		return ans;
	}
	
};

int solve(stack<string> &start, map<string, map<string, Rule> > &table, stack<string> &target){
	while(!(start.empty()||target.empty())){
		string input = start.top(), output = target.top();
		// cout << input << " , " << output << endl;
		start.pop();
		if(input.compare(output)){
			if(table.count(input)&&table[input].count(output)){
				cout << " " << table[input][output].number;
				vector<string> derivations = table[input][output].rhs;
				for(vector<string>::reverse_iterator rit=derivations.rbegin(); rit!=derivations.rend(); ++rit){
					start.push(*rit);
				}
			}
			else return 0;
		}
		else{
			// cout << " (- " << output << ") " ;
			target.pop();
		}
	}
	return start.empty()&&target.empty();
}

int main(int argc, char *argv[]){
	for(int index = 1; index < argc; index += 2){
		// cout << index << endl;
		CFG cfg = CFG(argv[index]);
		map<string, map<string, Rule> > table;
		vector<string> orderT, orderNT;
		set<string> otherT = cfg.terminals, otherNT = cfg.non_terminals;
		for(vector<struct Rule>::iterator iRule=cfg.rules.begin(); iRule!=cfg.rules.end(); iRule++){
			if(otherNT.count(iRule->lhs)){
				otherNT.erase(iRule->lhs);
				orderNT.push_back(iRule->lhs);
			}
			for(vector<string>::iterator it=iRule->rhs.begin(); it!=iRule->rhs.end(); ++it){
				if(otherT.count(*it)){
					otherT.erase(*it);
					orderT.push_back(*it);
				}
			}
			set<string> ans = cfg.predict(*iRule);
			for(set<string>::iterator it=ans.begin(); it!=ans.end(); it++){
				table[iRule->lhs][*it] = *iRule;
			}
		}
		cout << " N.T. \\  T.";
		for(vector<string>::iterator it=orderT.begin(); it!=orderT.end(); ++it){
			cout << setw(15) << *it ;
		}
		cout << endl;
		for(vector<string>::iterator iNT=orderNT.begin(); iNT!=orderNT.end(); ++iNT){
			cout << setw(9) << *iNT ;
			for(vector<string>::iterator iT=orderT.begin(); iT!=orderT.end(); ++iT){
				if(table[*iNT][*iT].number){
					cout << setw(15) << table[*iNT][*iT].number ;
				}
				else
					cout << setw(15) << "-" ;
			}
			cout << endl;
		}
		cout << endl;
		cout << " The Outputs :" << endl;
		std::ifstream inputStringFile(argv[index+1]);
		char buffer[256];
		while(inputStringFile.getline(buffer, 256)){
			// cout << "qustion : \t" << buffer << endl;
			std::istringstream iss (buffer);
			vector<string> inputToken;
			string token;
			while(iss >> token){
				inputToken.push_back(token);
			}
			stack<string> target, start;
			while(!inputToken.empty()){
				string last = *inputToken.rbegin();
				target.push(last);
				inputToken.pop_back();
			}
			start.push(cfg.start);
			if(!solve(start, table, target)){
				cout << " Error" << endl;
			}
			// else cout << " Accept" << endl;
			else cout << endl;
		}
		cout << endl;
	}
	return 0;
}
