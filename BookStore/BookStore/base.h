#pragma once
#include "stdafx.h"
#include <exception>

namespace BookStore{

	class OperationFail : public runtime_error {
	public:
		OperationFail();
		OperationFail(const char* msg);
		OperationFail(const string& str);
	};
	class format_error : public runtime_error
	{
	public:
		format_error() : runtime_error("Bad format.") {}
		format_error(const string& str) : runtime_error(str) {}
	};

	bool like(string a, string b);

	int to_i(string& str);
	int to_i(string& str, int defaul);
	bool to_b(string str);
	bool to_b(string str, bool defaul);
	double to_d(string& str);
	double to_d(string& str, double defaul);

}