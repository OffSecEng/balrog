#pragma once
#include <iostream>
#include <vector>
#include <string>

/*
https://stackoverflow.com/questions/5689003/how-to-implode-a-vector-of-strings-into-a-string-the-elegant-way
*/
std::string join(const std::vector<std::string>& sequence, const std::string& separator)
{
    std::string result;
    for (size_t i = 0; i < sequence.size(); ++i)
        result += sequence[i] + ((i != sequence.size() - 1) ? separator : "");
    return result;
}

/*
https://stackoverflow.com/questions/27512066/conversion-between-wchar-t-to-string
*/
std::string wchar2string(wchar_t* str)
{
    std::string mystring;
    while (*str)
        mystring += (char)*str++;
    return  mystring;
}