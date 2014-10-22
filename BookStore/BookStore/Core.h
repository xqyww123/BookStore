/* Core.h
 * Main Core of the book store
 * provide lifecycle manage.
 * 
 * by Xero Essential (–Ï∆Ù‘¥ 3130000950);
*/
#pragma once
#include "stdafx.h"
#include "BookMng.h"
#include "UserMng.h"
#include "Shell.h"
#include "Role.h"
#include "StorageMng.h"

namespace BookStore{

	class Core{
		
		static Status login_sta, user_sta, book_mng_sta, user_mng_sta;
		static void StatusLoad();

	public:
		static BookMng* BookMng;
		static UserMng* UserMng;
		static StatusMng* StatusMng;
		static RoleManage* RoleMng;
		static StorageMng* StorageMng;

		static User* User;

		// Launch the Core, must after Prepare.
		static void Launch();
		// Do some prepare like environment build.
		// It should be some simple quickly and global required initialize.
		static void Prepare(int argc, char* argv[]);
		// Terminate the program, save all data to file system, 
		// release resouce.
		static void Terminate();

	};
}