#include <regex>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    const std::string s = argv[1];
    std::regex words_regex("[^\\s\t\n]+");
    auto words_begin = std::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end = std::sregex_iterator();

    auto c = 0;
    for (std::sregex_iterator i = words_begin; i != words_end; ++i)
//        just in case if we want to print tokens
//        std::cout << (*i).str() << '\n';
        c++;
    std::cout << c << '\n';
}