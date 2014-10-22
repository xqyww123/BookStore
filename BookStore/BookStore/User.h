/* User.h
 *
 * by Xero Essential (–Ï∆Ù‘¥ 3130000950)
 */
#pragma once
#include "stdafx.h"
#include "Role.h"
#include "BookRep.h"
#include <time.h>

namespace BookStore{

	class UserMng;
	class User {

		// The info about user's book
		struct UserCopyInfo{
			// the time at buying.
			time_t bought_time;
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int version)
			{
				ar & bought_time;
			}
		};

		Role role;
		// as you see, every one have a reposity, no mater system or normal user.
		BookRep<UserCopyInfo> myrep;
		string name;
		string id;
		time_t create_time;
		float money;

		User();
		void AddCopy(Copy* copy);

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & role & myrep & name & id & create_time & money;
		}
		friend UserMng;
	public:
		static User* Login(string name, string pwd);
		static void Register(string name, string pwd);

		string Name();
		Role Role();

		// Buy book from system.
		Copy* Buy(Book* book);
		// Order books to system book reposity.
		// require Staff authority
		void Order(Book* book, int cnt);
		// Add books directly into user's reposity
		// require Admin authority
		void Add(Book* book, int cnt);
		void Return(Copy* copy);
		// Charge Money into user's profile
		// Only Admin can call the method
		// For normal user, please give administrator money fisrtly, 
		// and he will charge your money.
		void ChargeMoney(float monney);
		float MyMoney();

		// Find in my reposity
		Book* FindMyBook(string name);
		Book* FindMyBook(int id);
		Copy* FindMyCopy(int id);
		time_t BoughtTime(Copy* copy);
		int  CopyCount();
		int  BookCount();


		// implention of templates ( cause they must be put on 
		// caller visiable range, such as header file.
		template<typename TFunc>
		void EachMyBook(TFunc func)
		{
			myrep.EachBook([&](Book* book, ...){ func(book); });
		}
		template<typename TFunc>
		void EachMyCopy(TFunc func)
		{
			myrep.EachCopy([&](Copy* copy, ...){ func(copy); });
		}
	};
}

	