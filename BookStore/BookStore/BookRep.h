#pragma once
#include "stdafx.h"
#include "Book.h"
#include <vector>
#include <exception>
#include <boost\serialization\unordered_map.hpp>
#include <boost\serialization\unordered_set.hpp>

namespace BookStore{

	// as you see, it's empty
	struct EmptyStruct
	{
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) {}
	};

	// The book reposity, like a dictionary
	// BookInfoT is user specialized infomation about each book
	// CopyInfoT is the same as BookInfoT.
	template<typename CopyInfoT = EmptyStruct, typename BookInfoT = EmptyStruct>
	class BookRep {

		struct BookInfo{
			// count of copy of the book.
			int count;
			BookInfoT info;
			unordered_set<Copy*> copys;
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int version)
			{
				ar & count & info & copys;
			}
		};
		struct CopyInfo{
			CopyInfoT info;
			friend class boost::serialization::access;
			template<class Archive>
			void serialize(Archive & ar, const unsigned int version)
			{
				ar & info;
			}
		};

		unordered_map<Book*, BookInfo> books;
		unordered_map<Copy*, CopyInfo> copys;

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & books;
			ar & copys;
		}

		void add_no_copy(Book* book, const BookInfoT& info)
		{
			books[book] = { 0, info, unordered_set<Copy*>() };
		}
	public:
		BookRep() {}
		BookRep(const BookRep& rep) : books(rep.books), copys(rep.copys) {}

		// Register (or update) a book to the reposity
		// return a new book stored in the rep
		Book* Add(Book* book, const BookInfoT& info)
		{
			book = new Book(*book); // copy the book to avoid pointer deleted unexpected
			add_no_copy(book, info);
			return book;
		}
		void Add(Copy* copy, const CopyInfoT& info)
		{
			copy = new Copy(*copy); // copy the book to avoid pointer deleted unexpected
			if (books.find(copy->Kind()) == books.end())
				add_no_copy(copy->Kind(), BookInfoT());
			copys[copy] = { info };
			books[copy->Kind()].count++;
			books[copy->Kind()].copys.insert(copy);
		}
		void Remove(Copy* copy)
		{
			if (books.find(copy->Kind()) == books.end()) throw logic_error("not existed in reposity");
				copys.erase(copy);
			books[copy->Kind()].count--;
			books[copy->Kind()].copys.erase(copy);
		}
		// Seek the related information about a book or copy
		BookInfoT* Seek(Book* book) const
		{
			if (books.find(book) == books.end()) return NULL;
			return const_cast<BookInfoT*>(&books.at(book).info);
		}
		CopyInfoT* Seek(Copy* copy) const
		{
			if (copys.find(copy) == copys.end()) return NULL;
			return const_cast<CopyInfoT*>(&copys.find(copy)->second.info);
		}
		// the count of one book; count is one of specialized infomation.
		int Count(Book* book) const
		{
			return books.find(book)->count;
		}

		// Enumerate each book and invoke the function.
		// Func : void (Book* book, const BookInfoT& info)
		template<typename Func> void EachBook(Func f) const
		{
			for (auto a : books)
				f(a.first, a.second.info);
		}
		// Enumerate each copy and invoke the function.
		// Func : void (Copy* copy, const CopyInfoT& info)
		template<typename Func> void EachCopy(Func f) const
		{
			for (auto a : copys)
				f(a.first, a.second.info);
		}
		const unordered_set<Copy*>& Copys(Book* book) const
		{
			if (books.find(book) == books.end()) throw out_of_range("The book not exist in the reposity");
			return books[book].copys;
		}
		// Fetch a copy in kind of book
		// return NULL if no enough copy
		Copy* FetchCopy(Book* book) const
		{
			auto info = books.at(book);
			if (!info.count) return NULL;
			auto re = info.copys.begin();
			Copy* ret = *re;
			info.copys.erase(re);
			info.count--;
			return ret;
		}
		int CopyCount()
		{
			return copys.size();
		}
		int BookCount()
		{
			return books.size();
		}

		Copy* FindCopy(int id) const
		{
			for (auto a : copys)
			if (a.first->Id() == id)
				return a.first;
			return NULL;
		}
		Book* Find(string name) const
		{
			for (auto a : books)
			if (a.first->Name() == name)
				return a.first;
			return NULL;
		}
		Book* Find(int id) const
		{
			for (auto a : books)
			if (a.first->Id() == id)
				return a.first;
			return NULL;
		}
		// Find a book which satisfy the prediction
		// TPredict : function<bool(Book*)>
		template<typename TPredict>
		Book* Find(TPredict predict) const
		{
			for (auto a : books)
			if (predict(a))
				return a;
		}
	};
}