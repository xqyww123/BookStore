#include "stdafx.h"
#include "UserMng.h"
#include "base.h"
#include <boost\uuid\sha1.hpp>
#include "Config.h"
#include "Core.h"

namespace BookStore{

	struct UserMng::UserMngData
	{
		UserMngData() {}
		UserMngData(UserMng* um) : users(&um->users), name_index(&um->name_index) {}
		unordered_map<User*, UserInfo> *users;
		unordered_map<string, User*> *name_index;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & *users;
			ar & *name_index;
		}

		static UserMngData* GenerateNew()
		{
			auto re = new UserMngData();
			re->users = new unordered_map<User*, UserInfo>();
			re->name_index = new unordered_map<string, User*>();
			return re;
		}
		void Save()
		{
			StorageMng::oarchive& ar = Core::StorageMng->SaveDb();
			ar << *this;
		}
		static UserMngData* Load()
		{
			if (!StorageMng::DbExist())
				return GenerateNew();
			StorageMng::iarchive& ar = Core::StorageMng->LoadDb();
			UserMngData* re = new UserMngData();
			re->users = new unordered_map<User*, UserInfo>();
			re->name_index = new unordered_map<string, User*>();
			ar >> *re;
			return re;
		}
	};

	UserMng::UserMng(UserMngData* data) : Manager(type_index(typeid(UserMng)), Role::Admin), \
		users(*data->users), name_index(*data->name_index) {}

	User* UserMng::Login(string name, string pwd)
	{
		auto u = GetUser(name);
		if (!u) throw OperationFail("User not exist = =");
		auto info = GetInfo(u);
		if (info->pwd != Passwd(pwd)) throw OperationFail("Bad passwd = = ");
		time(&info->last_login_time);
		return u;
	}
	User* UserMng::Register(string name, string pwd)
	{
		if (GetUser(name)) throw OperationFail("user with the same name exist!");
		User* u = new User();
		u->name = name;
		u->role = Role::Guest;
		time_t curt; time(&curt);
		UserInfo info = UserInfo{ Passwd(pwd), curt };
		AddUser(u, info);
		return u;
	}
	UserMng::UserInfo* UserMng::GetInfo(User* user)
	{
		if (users.find(user) == users.end()) return NULL;
		return &users[user];
	}
	UserMng::UserInfo* UserMng::GetInfo(string name)
	{
		auto user = GetUser(name);
		if (user) return GetInfo(user);
		else return NULL;
	}
	User* UserMng::GetUser(string name)
	{
		if (name_index.find(name) == name_index.end()) return NULL;
		return name_index[name];
	}
	void UserMng::AddUser(User* user, const UserInfo& info)
	{
		users[user] = info;
		name_index[user->name] = user;
	}
	string UserMng::Passwd(string pwd)
	{
		auto t = string(Config::UserPwdSalt) + pwd;
		boost::uuids::detail::sha1 s;
		s.process_bytes(t.c_str(), t.size());
		unsigned int re[5];
		s.get_digest(re);
		return string((char*)re);
	}
	time_t UserMng::LastLoginTime(User* user)
	{
		auto info = GetInfo(user);
		if (!info) throw OperationFail("User not exist = =");
		return info->last_login_time;
	}
	time_t UserMng::LastLoginTime(string name)
	{
		auto info = GetInfo(name);
		if (!info) throw OperationFail("User not exist = =");
		return info->last_login_time;
	}


	UserMng* UserMng::Load()
	{
		auto data = UserMngData::Load();
		auto re = new UserMng(data);
		delete data;
		return re;
	}
	void UserMng::Save()
	{
		auto a = new UserMngData(this);
		a->Save();
		delete a;
	}

	void UserMng::Grant(User* user, Role role)
	{
		user->role = role;
	}
	void UserMng::ChargeMoney(User* user, float money)
	{
		user->money += money;
	}
	void UserMng::GlobalInit()
	{
		auto adm = Register("admin", "admin");
		adm->role = Role::All;
		adm->money = 1000.0;
		auto staff = Register("staff", "staff");
		staff->role = (Role) (Role::Staff | Role::Guest);
		staff->money = 100.0;
	}
}