#pragma once
#include "stdafx.h"
#include <boost\filesystem.hpp>

namespace BookStore{

	class Config{

	public:
		// 用户可以归还书籍的 时间上限，超过此时间，用户不能归还
		static const double Returnable_sec;
		static const char* UserPwdSalt;
		static boost::filesystem::path DbAddress; // = db
		static boost::filesystem::path BaseDirectory;
		static boost::filesystem::path ProgramAddress;

		static void ConfigEnviron(int argc, char* argv[]);

	};

}