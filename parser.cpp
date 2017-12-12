#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <regex>

class Nonterminal{
	std::string variable;
};

class Terminal{
	std::string variable;
};

class Rule{
	private:
	std::vector<std::string> left;
	std::vector<std::string> right;
	public:
	Rule(std::string);
	std::vector<std::string> getleft();
	std::vector<std::string> getright();
};
 // continue write ////////////////
class ContextFreeGrammar{
	std::set<Nonterminal> V; // nonterminal set
	std::set<Terminal> T; // terminal set
	// const std::vector<Rule> R; // rules, productions 
	const Rule R[]; // rules, productions 
	Nonterminal S; // start symbol
};

std::vector<std::string> split(const std::string&, const std::string&);

void show(std::vector<std::string> myvector){
	for (std::vector<std::string>::iterator it = myvector.begin() ; it != myvector.end(); ++it)
		std::cout << " & " << *it;
	std::cout << '\n';
}

Rule::Rule(std::string stringRule){
	std::string derives = ">";
	size_t find = stringRule.find(derives);
	size_t rfind = stringRule.rfind(derives);
	if(find == rfind){
		// std::cout << "find == rfind\n";
		left = split(stringRule.substr(0, find), " ");
		right = split(stringRule.substr(find + derives.length()), " ");
	}
	else {
		std::cout << "cfg error\n";
		exit(1);
	}
};

std::vector<std::string> Rule::getleft(){
	return left;
}

std::vector<std::string> Rule::getright(){
	return right;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiters){
	std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delimiters, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delimiters.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

int main(int argc, char *argv[]){
	std::ifstream ifs("CFG\\1.cfg", std::ifstream::in);
	
	std::string newline;
	std::getline (ifs, newline);
	
	Rule rule(newline);
	show(rule.getleft());
	show(rule.getright());
	return 0;
}

	// if (std::regex_match ("subject", std::regex("(sub)(.*)") ))
		// std::cout << "string literal matched\n";