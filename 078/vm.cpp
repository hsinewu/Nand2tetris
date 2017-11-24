#include <sstream>
#include <map>
#include <stdexcept>
using namespace std;

extern string G_FILE_BASENAME;

// D = val
string load_addr(string val) {

	return "@" + val + "\nD=A\n";
}

// D = M
string load_memory(string var) {

	return "@" + var + "\nD=M\n";
}

// D = PTR
string load_segment(string seg, string val) {

	const map< string, string> shorthand = {
		{"local", "LCL"},
		{"argument", "ARG"},
		{"this", "THIS"},
		{"that", "THAT"}
	};

	// TEMP is defined as @5 ~ @12
	if( seg == "temp")
		return load_addr( to_string( 5 + stoi(val)));

	// pointer 0/1 = THIS/THAT
	if( seg == "pointer")
		return load_addr( to_string( 3 + stoi(val)));

	// static i = @Foo.i where Foo is the name of vm file
	if( seg == "static")
		return load_addr( G_FILE_BASENAME + "." + val);

	if( shorthand.count(seg) == 0 )
		throw runtime_error( "Bad seg name: " + seg );

	// optimize for val == 0
	if( val == "0")
		return load_memory( shorthand.at(seg));

	return load_addr(val) + "@" + shorthand.at(seg) + "\nD=M+D\n";
}

// *SP++ = D
string push_stack() {

	return "@SP\nM=M+1\nA=M-1\nM=D\n";
}

// A = --SP
string pop_stack() {

	return "@SP\nM=M-1\nA=M\n";
}

string command_push(string seg, string val) {

	if( seg == "constant")
		return load_addr(val) + push_stack();

	return load_segment( seg, val) + "A=D\nD=M\n" + push_stack();
}

string command_pop(string seg, string val) {

	const string pop_stackA = pop_stack() + "A=M\n";
	const string swap_AD = "D=D+A\nA=D-A\nD=D-A\n";
	const string write_back = "M=D\n";

	return load_segment( seg, val) + pop_stackA + swap_AD + write_back;
}

// D = op2, M = op1 ( A = SP )
const string set_ops = "@SP\nM=M-1\nA=M\nD=M\n@SP\nA=M-1\n";
const string set_op = "@SP\nA=M-1\n";  // M = op ( A = SP )

// Arithmetics
const string command_add = set_ops + "M=M+D\n";
const string command_sub = set_ops + "M=M-D\n";
const string command_neg = set_op + "M=-M\n";

// Comparision
string uniq_name() {

	static int uniq_num;

	return "VM_" + G_FILE_BASENAME + to_string( ++uniq_num);
}

string test(string jump) {

	const string label = uniq_name();
	const string set_bits = "D=M\nM=-1\n";
	const string jump_exit_if = "@" + label + "\nD;" + jump + "\n";
	const string unset_bits = "@SP\nA=M-1\nM=0\n";
	const string exit_label = "(" + label + ")\n";

	return set_bits + jump_exit_if + unset_bits + exit_label;
}

string command_eq() {

	return command_sub + test("JEQ");
}

string command_gt() {

	return command_sub + test("JGT");
}

string command_lt() {

	return command_sub + test("JLT");
}

// Bit-wise
const string command_and = set_ops + "M=M&D\n";
const string command_or  = set_ops + "M=M|D\n";
const string command_not = set_op  + "M=!M\n" ;

// Jump
string command_label(string label) {

	return "(" + label + ")\n";
}

string command_goto(string label) {

	return "@" + label + "\n0;JMP\n";
}

string command_if_goto(string label) {

	return
	"@SP\nM=M-1\nA=M\nD=M\n"
	"@" + label + "\n"
	"D;JNE\n";
}

// Function
string command_function(string name, string nvars) {

	// This makes code grows n times
	// Better use jump?
	const string pad_local = "@SP\nM=M+1\nA=M-1\nM=0\n";
	string pad_locals = "";
	for( int n = stoi(nvars); n>0; n--)
		pad_locals += pad_local;

	return
	// label this function address
	command_label(name) + 

	// reserve zero initialized vars * n 
	pad_locals

	// course says push 0, but why?
	// + "@SP\nM=M+1\nA=M-1\nM=0\n"
	;
}

string command_call(string name, string nargs) {

	const string label = uniq_name();

	return
	// memorize new arg where arg = sp - nargs\n
	load_addr(nargs) + "@SP\nD=M-D\n" "@VM_ARG\nM=D\n" +

	// push return address\n
	load_addr(label) + push_stack() +

	// push pointers\n
	// *sp++ = ptr for ptr in [LCL, ARG, THIS, THAT]
	 "@LCL\nD=M\n" + push_stack() +
	 "@ARG\nD=M\n" + push_stack() +
	"@THIS\nD=M\n" + push_stack() +
	"@THAT\nD=M\n" + push_stack() +
	
	// set new arg, lcl\n
	// arg = sp - nargs - 5
	load_memory("VM_ARG") + "@ARG\nM=D\n" +

	// lcl = sp
	"@SP\nD=M\n" "@LCL\nM=D\n" +

	// transfer control\n
	command_goto(name) +

	// mark return address
	command_label(label)
	;
}

string command_return() {

	return
	// record where SP should be afterwards
	// tmp = arg+1
	"@ARG\nD=M+1\n@VM_SP\nM=D\n"

	// write return value
	// *arg = *sp
	// Is this safe when no return value/arg?
	// "@SP\nA=M-1\nD=M\n" "@ARG\nA=M\nM=D\n"

	// record return value
	"@SP\nA=M-1\nD=M\n" "@VM_VAL\nM=D\n"

	// pop locals
	// sp = lcl
	"@LCL\nD=M\n" "@SP\nM=D\n"

	// pop pointers
	// *--sp = ptr for ptr in [THAT, THIS, ARG, LCL]
	"@SP\nM=M-1\nA=M\nD=M\n" "M=0\n" "@THAT\nM=D\n"
	"@SP\nM=M-1\nA=M\nD=M\n" "M=0\n" "@THIS\nM=D\n"
	"@SP\nM=M-1\nA=M\nD=M\n" "M=0\n"  "@ARG\nM=D\n"
	"@SP\nM=M-1\nA=M\nD=M\n" "M=0\n"  "@LCL\nM=D\n"

	// pop return address
	+ pop_stack() + "D=M\n" "M=0\n" "@VM_RET\nM=D\n"

	// restore sp
	// sp = tmp
	"@VM_SP\nD=M\n@SP\nM=D\n" +

	// write return value
	load_memory("VM_VAL") + "@SP\nA=M-1\nM=D\n" +
	// goto return address
	"@VM_RET\nA=M\n0;JMP\n"
	// command_goto("VM_RET")
	;
}

string command_init() {

	const string init_SP = "@256\nD=A\n@SP\nM=D\n";
	const string thth = "@7513\nD=A\n@THIS\nM=D\n" "@7547\nD=A\n@THAT\nM=D\n";

	// return init_SP + command_call("Sys.init", "0");
	return init_SP + thth + command_call("Sys.init", "0");
	// return init_SP + command_goto("Sys.init");
	// return init_SP + thth + command_goto("Sys.init");
}

string parse_line(string line) {

	stringstream tokens( line);
	string cmd, a, b;
	tokens >> cmd >> a >> b;

	// commands that always generate same string
	const static map<string, string> static_cmd = {
		{"add", command_add},
		{"sub", command_sub},
		{"neg", command_neg},
		{"and", command_and},
		{"or" , command_or },
		{"not", command_not},
	};
	if( static_cmd.count( cmd))
		return static_cmd.at(cmd);

	// commands that don't always generate same string
	if( cmd == "push")
		return command_push(a, b);
	if( cmd == "pop")
		return command_pop(a, b);

	if( cmd == "eq")
		return command_eq();
	if( cmd == "gt")
		return command_gt();
	if( cmd == "lt")
		return command_lt();

	if( cmd == "label")
		return command_label(a);
	if( cmd == "goto")
		return command_goto(a);
	if( cmd == "if-goto")
		return command_if_goto(a);

	if( cmd == "function")
		return command_function(a, b);
	if( cmd == "call")
		return command_call(a, b);
	if( cmd == "return")
		return command_return();

	throw runtime_error("Unknown command [" + cmd + "]");
}