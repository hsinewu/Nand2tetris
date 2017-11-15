#include <iostream>
#include <fstream>
#include "vm.cpp"
using namespace std;

string G_FILE_BASENAME;

void process_file(const string file_name) {

    // remove extension
    size_t dot = file_name.find_last_of(".");
    const string no_ext = file_name.substr(0, dot);

    // remove path
    size_t slash = no_ext.find_last_of("/");
    G_FILE_BASENAME = no_ext.substr( slash+1);

    ifstream vm_file( file_name);
    ofstream asm_file( no_ext + ".asm");
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