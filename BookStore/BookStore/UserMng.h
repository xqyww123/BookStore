/* UserMng.h
 * Manage all user in the system
 * Manage was designed as singletion rather than static class
 * cause it's more easy to control the life cycle.
 *
 * User could trans into user_mng status to get the access 
 * of methods in the manage.
 * But only administrator could get the access to the manage, 
 * so all methods here could be invoke by admin only.
*/
#pragma once
#include "stdafx.h"
#include "User.h"
#include "Manager.h"
#include <typeindex>
#include <boost\archive\binary_iarchive.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\serialization\unordered_map.hpp>

namespace BookStore{
	class Core;

	// The Manager class of users
	class UserMng : public Manager {
		struct UserMngData;
		UserMng(UserMngData* data);

		struct UserInfo
		{
			// all password was encryped by Passwd function
			string pwd;
			time_t last_login_time;

			// methods to support serialization
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int version)
			{
				ar & pwd & last_login_time;
			}
		};
		unordered_map<User*, UserInfo> users;
		// an index table
		unordered_map<string, User*> name_index;

		// return NULL if no user in the datas
		UserInfo* GetInfo(User* user);
		UserInfo* GetInfo(string name);
		// low level function to add user
		void AddUser(User* user, const UserInfo& info);

		friend Core;
	public:

		User* Login(string name, string pwd);
		User* Register(string name, string pwd);
		User* GetUser(string name);
		time_t LastLoginTime(User* user);
		time_t LastLoginTime(string name);
		// change user's role.
		void Grant(User* user, Role role);
		void ChargeMoney(User* user, float money);

		// encryp user password, with sha1 algorithm, 
		// a single way encrpytion algorithm
		static string Passwd(string pwd);

		static UserMng* Load();
		void Save();
		// If it's the first time to run the program on certain machine,
		// call Global init to generate intial infomartion.
		void GlobalInit();
	};
}