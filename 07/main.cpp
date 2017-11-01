#include <iostream>
#include <fstream>
#include "vm.cpp"
using namespace std;

string set_extension(string file_name) {

    size_t index = file_name.find_last_of(".");
    if( index!= -1)
        file_name = file_name.substr(0, index);
    return file_name + ".asm";
}

void process_file(string file_name) {

    string output_name = set_extension( file_name);
    ifstream vm_file( file_name);
    ofstream asm_file( output_name);
    string line;
    int num = 0;

    while( getline(vm_file, line)) {
        line.erase( line.find_last_not_of("\t\r") + 1 );
        if( line == "" || line.substr(0,2) == "//") continue;
        cerr << ++num << ": " << line << endl;
        asm_file << parse_line( line) << endl;
    }
}

int main(int argc, char* argv[]) {

    if( argc == 1) {
        cout << "No argv provided, end program." << endl;
        return 0;
    }

    for( int i=1; i<argc; i++)
        process_file(argv[i]);

    cout << "Program finished successfully." << endl;
    return 0;
}