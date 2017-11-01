#include <sstream>
#include <map>
#include <stdexcept>
using namespace std;

using F = string (*)(string);
static int uniq_num = 0;

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

// Arithmetics
string command_add() {

	const string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	const string op2_add = "@SP\nA=M-1\nM=M+D\n";
	return op1 + op2_add;
}

string command_sub() {
	const string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	const string op2_sub = "@SP\nA=M-1\nM=M-D\n";
	return op1 + op2_sub;
}

string command_neg() {
	return "@SP\nA=M-1\nM=-M";
}

// Comparision
string command_eq() {
	const string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	const string op2_sub = "@SP\nA=M-1\nM=M-D\n";
	// M = M==0? 1: 0
	const string label = "VM_EQ_" + to_string(++uniq_num);
	const string test = "D=M\nM=-1\n"  // Guess true
		"@" + label + "\nD;JEQ\n"     // Jump/skip if correct
		"@SP\nA=M-1\nM=0\n"                  // Set false
		"(" + label + ")\n";          // Label for exit
	return op1 + op2_sub + test;
}

string command_gt() {
	const string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	const string op2_sub = "@SP\nA=M-1\nM=M-D\n";
	const string label = "VM_GT_" + to_string(++uniq_num);
	const string test = "D=M\nM=-1\n@" + label + "\nD;JGT\n@SP\nA=M-1\nM=0\n(" + label + ")\n";
	return op1 + op2_sub + test;
}

string command_lt() {
	const string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	const string op2_sub = "@SP\nA=M-1\nM=M-D\n";
	const string label = "VM_LT_" + to_string(++uniq_num);
	const string test = "D=M\nM=-1\n@" + label + "\nD;JLT\n@SP\nA=M-1\nM=0\n(" + label + ")\n";
	return op1 + op2_sub + test;
}

// Bit-wise
string command_and() {
	const string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	const string op2_and = "@SP\nA=M-1\nM=M&D\n";
	return op1 + op2_and;
}

string command_or() {
	const string op1 = "@SP\nM=M-1\nA=M\nD=M\n";
	const string op2_or = "@SP\nA=M-1\nM=M|D\n";
	return op1 + op2_or;
}

string command_not() {
	return "@SP\nA=M-1\nM=!M";
}

string parse_line(string line) {

	stringstream tokens( line);
	string cmd, seg, val;
	tokens >> cmd >> seg >> val;
	if( cmd == "push")
		return command_push(seg, val);
	if( cmd == "add")
		return command_add();
	if( cmd == "sub")
		return command_sub();
	if( cmd == "neg")
		return command_neg();
	if( cmd == "eq")
		return command_eq();
	if( cmd == "gt")
		return command_gt();
	if( cmd == "lt")
		return command_lt();
	if( cmd == "and")
		return command_and();
	if( cmd == "or")
		return command_or();
	if( cmd == "not")
		return command_not();
	throw runtime_error("Unknown command [" + cmd + "]");
}