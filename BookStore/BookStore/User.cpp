#include "stdafx.h"
#include "base.h"
#include "Core.h"
#include "Config.h"
#include "User.h"

namespace BookStore{

	Copy* User::Buy(Book* book)
	{
		RoleManage::Valid(this, Role::Guest);
		if (book->Price() > money) throw OperationFail("You have no enough money");
		Copy* re = Core::BookMng->books.FetchCopy(book);
		if (!re) throw OperationFail("Can't buy the book, books has already sell out.");
		Core::BookMng->books.Remove(re);
		AddCopy(re);
		money -= book->Price();
		return re;
	}
	void User::Order(Book* book, int cnt)
	{
		RoleManage::Valid(this, Role::Staff);
		Core::BookMng->Order(book, cnt);
	}
	void User::Add(Book* book, int cnt)
	{
		RoleManage::Valid(this, Role::Admin);
		for (int i = 0; i < cnt; ++i)
			AddCopy(&Copy(book));
	}
	void User::Return(Copy* copy)
	{
		RoleManage::Valid(this, Role::Guest);
		auto info = myrep.Seek(copy);
		if (!info) throw OperationFail("You didn't bought the book!");
		time_t curt; time(&curt);
		if (difftime(curt, info->bought_time) > Config::Returnable_sec)
			throw OperationFail("Too old to return the book = =.");
		myrep.Remove(copy);
		money += copy->Kind()->Price();
		Core::BookMng->Return(copy);
	}
	void User::AddCopy(Copy* copy)
	{
		UserCopyInfo info;
		time(&info.bought_time);
		myrep.Add(copy, info);
	}

	string User::Name()
	{
		return name;
	}
	Role User::Role()
	{
		return role;
	}

	User* User::Login(string name, string pwd)
	{
		return Core::UserMng->Login(name, pwd);
	}
	void User::Register(string name, string pwd)
	{
		Core::UserMng->Register(name, pwd);
	}
	void User::ChargeMoney(float _monney)
	{
		RoleManage::Valid(this, Role::Admin);
		money += _monney;
	}
	float User::MyMoney()
	{
		return money;
	}
	time_t User::BoughtTime(Copy* copy)
	{
		auto info = myrep.Seek(copy);
		if (!info) throw OperationFail("You didn't bought the book!");
		return info->bought_time;
	}
	int  User::CopyCount() { return myrep.CopyCount(); }
	int  User::BookCount() { return myrep.BookCount(); 
	}

	Copy* User::FindMyCopy(int id)
	{
		return myrep.FindCopy(id);
	}
	Book* User::FindMyBook(string name)
	{
		return myrep.Find(name);
	}
	Book* User::FindMyBook(int id)
	{
		return myrep.Find(id);
	}
	User::User() : role(Role::Empty), create_time(time(NULL)), money(0.0) {}
}