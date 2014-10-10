#ifndef RegEx_h
#define RegEx_h

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

//  Example:
//	map< string, vector<string> > result = RegExSearch("(\\w+) (\\w+)", "This is a test");
//  result.begin()->first == "This is";
//  result.begin()->second[0] = "This";
//  result.begin()->second[1] = "is";
//  result.end()->first == "a test";
//  result.end()->second[0] = "a";
//  result.end()->second[1] = "test";
map< string, vector<string> > RegExSearch(const char *pattern, const char *target);

//  Expression $var =~ s/(\\w)\\.(\\w)/$2.$1/gx is equivalent to
//  var = RegExSubstitude("(\\w)\\.(\\w)", "$2.$1", "first.second"); (var == "second.first")
string RegExSubstitude(const char *pattern_from, const char *pattern_to, const char* target);

#endif
