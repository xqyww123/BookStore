// BookStore.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Core.h"
#include <fstream>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\archive\binary_iarchive.hpp>

using namespace BookStore;
using namespace std;

int main(int argc, char *argv[])
{
	Core::Prepare(argc, argv);
	Core::Launch();
	Core::Terminate();

	unordered_map<string, string> a, b;
	a["a"] = "b";
	a["c"] = "d";

	fstream f("txt", ios::out);
	boost::archive::binary_oarchive oar(f);
	oar << a;
	f.close();

	f.open("txt", ios::in);
	boost::archive::binary_iarchive iar(f);
	iar >> b;
	f.close();

	return 0;
}

