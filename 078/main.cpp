#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <dirent.h> // ls_vm
#include <sys/stat.h> // is_directory
#include "vm.cpp"
using namespace std;

string G_FILE_BASENAME;
int G_FLAGS = 1;

// use c++17 filesystem?
int is_directory(const string path) {

    const char* c_path = path.c_str();
    struct stat path_stat;
    stat( c_path, &path_stat);
    return S_ISDIR( path_stat.st_mode);
}

// Orz
bool has_suffix(const std::string &str, const std::string &suffix)
{
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

vector<string> ls_vm(const string path) {

    vector<string> ret;
    DIR* dir;
    struct dirent* ent;

    dir = opendir(path.c_str());
    if( dir != NULL) {
        while( (ent = readdir(dir)) != NULL ) {
            
            string name(ent->d_name);
            if( has_suffix( name, ".vm"))
                ret.push_back( name);
        }
    }
    return ret;
}

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

    // init code specified in course
    if( G_FLAGS & 1)
        asm_file << command_init() << endl;

    while( getline(vm_file, line)) {

        line.erase( line.find_last_not_of("\t\r") + 1 );
        if( line == "" || line.substr(0,2) == "//")
            continue;

        asm_file << parse_line( line) << endl;
    }
}

void process_directory(const string path) {

    cout << path << " is directory" << endl;
}

void process_entries(vector<string> entries) {

    for(auto& e: entries) {

        if( is_directory(e)) { 
            process_directory(e);
        } else {
            process_file(e);
        }
    }
}

int main(int argc, char* argv[]) {

    if( argc == 1) {

        cout << "No argv provided, end program." << endl;
        return 0;
    }

    vector<string> entries;
    for( int i=1; i<argc; i++) {

        if( strcmp( argv[i], "--noinit") == 0) {

            G_FLAGS &= 0xfffffffe;
            continue;
        }
        entries.push_back( argv[i]);
    }

    process_entries( entries);

    cout << "Program finished successfully." << endl;
    return 0;
}