#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    std::string current_exec_name = argv[0]; // Name of the current exec program
    std::vector<std::string> allWords;
    if (argc > 1) {
        allWords.assign(argv + 1, argv + argc);
    } else {
        cout << "No args given" << endl;
        return 0;
    }

    int i;
    for (i = 1; i < allWords.size(); i++) {}

    cout << i << endl;
    return 0;
}