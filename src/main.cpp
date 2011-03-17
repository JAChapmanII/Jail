#include <iostream>
using std::cout;
using std::endl;

int main(int argc, char** argv) {
    if(argc < 1) { // impossible, argv[0] is how the program was invoked
        cout << "Usage: " << argv[0] << endl;
        return 1;
    }

    cout << "We do nothing so far!" << endl;
    return 1;
}

// vim:ts=4 et sw=4
