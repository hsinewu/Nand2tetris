#include <sstream>
#include <map>
#include <functional>
using namespace std;

// typedef string(*F)(string);
using F = string (*)(string);
string load_constant(string val) {

	return "@" + val + "\nD=A\n";
}

string inc_stack() {

	return "@SP\nM=M+1\n";
}

string command_push(string seg, string val) {

	// static map< string, function< string (string)>> funcs = {};
	static map< string, F> funcs = {
		{"constant", load_constant}
	};
	return funcs[seg](val) + inc_stack();
}

string parse_line(string line) {

	stringstream tokens( line);
	string cmd, seg, val;
	tokens >> cmd >> seg >> val;
	if( cmd == "push")
		return command_push(seg, val);
	return "I parse this line: " + cmd;
}