#pragma once
#include "stdafx.h"
#include <boost\filesystem.hpp>

namespace BookStore{

	class Config{

	public:
		// �û����Թ黹�鼮�� ʱ�����ޣ�������ʱ�䣬�û����ܹ黹
		static const double Returnable_sec;
		static const char* UserPwdSalt;
		static boost::filesystem::path DbAddress; // = db
		static boost::filesystem::path BaseDirectory;
		static boost::filesystem::path ProgramAddress;

		static void ConfigEnviron(int argc, char* argv[]);

	};

}