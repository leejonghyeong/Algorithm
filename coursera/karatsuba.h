#pragma once
#include <vector>

using namespace std;

vector<int> intToVec(int num);
vector<int> stringToVec(string num);
string intToString(int num);
string vecToString(vector<int> vec);


void regroup(vector<int>& num);

vector<int> product(const vector<int>& a, const vector<int>& b);

void karatsubaAdd(vector<int>& a, const vector<int>& b, int k);
void karatsubaAdd(string& a, const string& b);

void karatsubaSub(vector<int>& a, const vector<int>& b);
void karatsubaSub(string& a, const string& b);

vector<int> karatsubaProduct(const vector<int>& a, const vector<int>& b);
string karatsubaProduct(const string& a, const string& b);

