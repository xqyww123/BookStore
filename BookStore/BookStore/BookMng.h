/* BookMng.h
 * Manage all book in the system
 * Manage was designed as singletion rather than static class
 * cause it's more easy to control the life cycle.
 *
 * User could trans into book_mng status to get the access 
 * of methods in the manage.
 * But only staff could get the access to the manage, 
 * so all methods here could be invoke by staff only.
*/
#pragma once
#include "stdafx.h"
#include "Book.h"
#include "BookRep.h"
#include "User.h"
#include "Manager.h"

namespace BookStore{
	class Core;

	class BookMng : public Manager{

		class BookMngData;
		BookMng(BookMngData* data);

		// Manager's book infomation
		struct MngCopyInfo{
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int version) {}
		};
		struct MngBookInfo
		{
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int version) {}
		};

		int book_ids, copy_ids;
		friend Core;
	public:
		// system's book reposity.
		BookRep<MngCopyInfo, MngBookInfo> books;

		void Order(Book* book, int cnt);
		// though name or id find book or copy
		Book* Find(string name);
		Book* Find(int id);
		Copy* FindCopy(int id);
		// asign a unique book or copy id.
		int AsignBookId();
		int AsignCopyId();
		// Add a book to the system, with copy_num copys.
		void Add(Book* book, int copy_num);
		// return a copy to system.
		void Return(Copy* copy);

		static BookMng* Load();
		void Save();
		// If it's the first time to run the program on certain machine,
		// call Global init to generate intial infomartion.
		void GlobalInit();
	};
}