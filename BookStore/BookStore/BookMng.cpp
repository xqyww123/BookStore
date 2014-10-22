#include "stdafx.h"
#include "BookMng.h"
#include <exception>
#include "Manager.h"
#include <fstream>
#include <boost\archive\binary_iarchive.hpp>
#include <boost\archive\binary_oarchive.hpp>
#include <boost\serialization\unordered_map.hpp>
#include "Config.h"
#include "StorageMng.h"
#include "Core.h"

namespace BookStore{
	class BookMng::BookMngData {
	public:
		BookRep<BookMng::MngCopyInfo, BookMng::MngBookInfo>* books;
		int book_ids;
		int copy_ids;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar &book_ids;
			ar &copy_ids;
			ar & *books;
		}

		static BookMngData* GenerateNew(){
			auto re = new BookMngData();
			re->books = new BookRep<BookMng::MngCopyInfo, BookMng::MngBookInfo>();
			return re;
		}
		void Save()
		{
			auto& ar = Core::StorageMng->SaveDb();
			ar << *this;
		}
		static BookMngData* Load()
		{
			if (!StorageMng::DbExist())
				return GenerateNew();
			auto& ar = Core::StorageMng->LoadDb();
			BookMngData* re = new BookMngData();
			re->books = new BookRep<BookMng::MngCopyInfo, BookMng::MngBookInfo>();
			ar >> *re;
			return re;
		}
	};

	BookMng::BookMng(BookMngData* data) : Manager(type_index(typeid(BookMng)), Role::Staff), \
		book_ids(data->book_ids), copy_ids(data->copy_ids), books(*data->books) {}
	void BookMng::Order(Book* book, int cnt)
	{
		if (!books.Seek(book)) book = books.Add(book, MngBookInfo());
		for (int i = 0; i < cnt; ++i)
		{
			books.Add(new Copy(book), MngCopyInfo());
		}
	}

	Book* BookMng::Find(string name)
	{
		return books.Find(name);
	}
	Book* BookMng::Find(int id)
	{
		return books.Find(id);
	}
	Copy* BookMng::FindCopy(int id)
	{
		return books.FindCopy(id);
	}
	int BookMng::AsignBookId() { return book_ids++; }
	int BookMng::AsignCopyId() { return copy_ids++; }


	/// serialization :

	BookMng* BookMng::Load()
	{
		auto data = BookMngData::Load();
		auto re = new BookMng(data);
		delete data;
		return re;
	}
	void BookMng::Save()
	{
		auto a = new BookMngData{ &books, book_ids, copy_ids };
		a->Save();
		delete a;
	}
	void BookMng::GlobalInit()
	{
		Add(new Book("测试书籍1", "其实没什么内容", "xqy", 10.0),  10);
		Add(new Book("测试书籍2", "有很多无聊的内容", "xero", 10.0), 10);
		Add(new Book("测试书籍3", "asdasdadasda", "xero", 50.0), 1);
	}
	void BookMng::Add(Book* book, int copy_num)
	{
		book = books.Add(book, MngBookInfo());
		for (int i = 0; i < copy_num; ++i)
			books.Add(new Copy(book), MngCopyInfo());
	}
	void BookMng::Return(Copy* copy)
	{
		books.Add(copy, MngCopyInfo());
	}
}

