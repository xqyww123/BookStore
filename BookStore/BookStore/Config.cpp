#include "stdafx.h"
#include "Config.h"
#include <fstream>

typedef boost::filesystem::path path;

namespace BookStore{

	const double Config::Returnable_sec = 14 * 24 * 3600;
	const char* Config::UserPwdSalt = "0fDZRo%FFdBM8$2Wb&*b";
	boost::filesystem::path Config::DbAddress;
	boost::filesystem::path Config::BaseDirectory;
	boost::filesystem::path Config::ProgramAddress;

	void Config::ConfigEnviron(int argc, char* argv[])
	{
		ProgramAddress = boost::filesystem::system_complete(argv[0]);
		BaseDirectory = ProgramAddress.parent_path();

		DbAddress = BaseDirectory / "db";

	}
}