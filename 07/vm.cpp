#include <sstream>
#include <map>
using namespace std;

using F = string (*)(string);
string load_constant(string val) {

	return "@" + val + "\nD=A\n";
}

string inc_stack() {

	return "@SP\nM=M+1\n";
}

string write_stack() {

	return "@SP\nA=M\nM=D\n";
}

string command_push(string seg, string val) {

	static map< string, F> funcs = {
		{"constant", load_constant}
	};
	return funcs[seg](val) + write_stack() + inc_stack();
}

string command_add() {

	static string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	static string op2_add = "@SP\nA=M-1\nM=M+D\n";
	return op1 + op2_add;
}

string parse_line(string line) {

	stringstream tokens( line);
	string cmd, seg, val;
	tokens >> cmd >> seg >> val;
	if( cmd == "push")
		return command_push(seg, val);
	if( cmd == "add")
		return command_add();
	return "Command not implemented: [" + cmd + "]";
}