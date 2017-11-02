#include <sstream>
#include <map>
#include <stdexcept>
using namespace std;

static int uniq_num = 0;

// D = val
string load_constant(string val) {

	return "@" + val + "\nD=A\n";
}

// D = PTR
string load_address(string seg, string val) {

	const map< string, string> shorthand = {
		{"local", "@LCL\n"},
		{"argument", "@ARG\n"},
		{"this", "@THIS\n"},
		{"that", "@THAT\n"}
	};

	// TEMP is defined as @5 ~ @12
	if( seg == "temp")
		return load_constant( to_string( 5 + stoi(val)));

	if( shorthand.count(seg) == 0 )
		throw runtime_error( "Bad seg name: " + seg );

	// optimize for val == 0
	if( val == "0")
		return shorthand.at(seg) + "D=M\n";

	return load_constant(val) + shorthand.at(seg) + "D=M+D\n";
}

// SP++
string inc_stack() {

	return "@SP\nM=M+1\n";
}

// SP--
string dec_stack() {

	return "@SP\nM=M-1\n";
}

// *SP = D
string write_stack() {

	return "@SP\nA=M\nM=D\n";
}

string command_push(string seg, string val) {

	if( seg == "constant")
		return load_constant(val) + write_stack() + inc_stack();

	return load_address( seg, val) + "A=D\nD=M\n" + write_stack() + inc_stack();
}

string command_pop(string seg, string val) {

	const string load_stack = dec_stack() + "A=M\n" "A=M\n";
	const string swap_AD = "D=D+A\nA=D-A\nD=D-A\n";
	const string write_back = "M=D\n";

	return load_address( seg, val) + load_stack + swap_AD + write_back;
}

// D = op2, M = op1 ( A = SP )
const string set_ops = "@SP\nM=M-1\nA=M\nD=M\n@SP\nA=M-1\n";
const string set_op = "@SP\nA=M-1\n";  // M = op ( A = SP )

// Arithmetics
string command_add() {

	return set_ops + "M=M+D\n";
}

string command_sub() {

	return set_ops + "M=M-D\n";
}

string command_neg() {

	return set_op + "M=-M\n";
}

// Comparision
string command_eq() {

	// M = M==0? -1: 0
	const string label = "VM_EQ_" + to_string( ++uniq_num);
	const string test = "D=M\nM=-1\n"  // Guess true
		"@" + label + "\nD;JEQ\n"      // Jump/skip if correct
		"@SP\nA=M-1\nM=0\n"            // Set false
		"(" + label + ")\n";           // Label for exit

	return command_sub() + test;
}

string command_gt() {

	const string label = "VM_GT_" + to_string( ++uniq_num);
	const string test = "D=M\nM=-1\n@" + label + "\nD;JGT\n@SP\nA=M-1\nM=0\n(" + label + ")\n";

	return command_sub() + test;
}

string command_lt() {

	const string label = "VM_LT_" + to_string( ++uniq_num);
	const string test = "D=M\nM=-1\n@" + label + "\nD;JLT\n@SP\nA=M-1\nM=0\n(" + label + ")\n";

	return command_sub() + test;
}

// Bit-wise
string command_and() {

	return set_ops + "M=M&D\n";
}

string command_or() {

	return set_ops + "M=M|D\n";
}

string command_not() {

	return set_op + "M=!M\n";
}

string parse_line(string line) {

	stringstream tokens( line);
	string cmd, seg, val;
	tokens >> cmd >> seg >> val;
	if( cmd == "push")
		return command_push(seg, val);
	if( cmd == "pop")
		return command_pop(seg, val);
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