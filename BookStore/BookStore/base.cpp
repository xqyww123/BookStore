#include "stdafx.h"
#include "base.h"
#include <algorithm>
#include <sstream>

namespace BookStore{

	OperationFail::OperationFail() : runtime_error("operation failed") {}
	OperationFail::OperationFail(const char* msg) : runtime_error(msg) {}
	OperationFail::OperationFail(const string& str) : runtime_error(str) {}

	bool like(string a, string b)
	{
		return a.find(b) != string::npos || b.find(a) != string::npos;
	}
	int to_i(string& str)
	{
		int re;
		if (1 != sscanf(str.c_str(), "%d", &re))
			throw format_error("Bad Integer!");
		return re;
	}
	int to_i(string& str, int defaul)
	{
		int re;
		if (1 != sscanf(str.c_str(), "%d", &re))
			return defaul;
		return re;
	}
	bool to_b(string str)
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		if (str == "true" || str == "t") return true;
		else if (str == "false" || str == "f") return false;
		throw format_error("Bad bool = =");
	}
	bool to_b(string str, bool defaul)
	{
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		if (str == "true" || str == "t") return true;
		else if (str == "false" || str == "f") return false;
		return defaul;
	}
	double to_d(string& str)
	{
		double re;
		if (1 != sscanf(str.c_str(), "%lf", &re))
			throw format_error("Bad Double!");
		return re;
	}
	double to_d(string& str, double defaul)
	{
		double re;
		if (1 != sscanf(str.c_str(), "%lf", &re))
			return defaul;
		return re;
	}
}
