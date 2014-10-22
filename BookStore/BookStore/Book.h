/* Book.h
 * 
 * by Xero Essential (–Ï∆Ù‘¥ 3130000950)
 */
#pragma once
#include "stdafx.h"

namespace BookStore{

	class BookMng;
	class User;
	class Core;

	class Book{
		Book();
		Book(string name, string abstracts, string author, float price);

		int id;
		string name;
		string title;
		string abstracts;
		string author;
		float price;

		friend BookMng;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) 
		{
			ar & id & name & title & abstracts & author & price;
		}

		friend Core;
	public:

		string Name();
		string Title();
		string Abstracts();
		string Author();
		float Price();
		int Id();
		
		class BookHash {
			size_t operator() (const Book& book) const;
		};
	};

	// Copy is , just a copy of one book.
	class Copy{
		Copy();
		Copy(Book* kind);
		
		// the time when the book recorded.
		time_t create_time;
		// the kind of the book.
		Book* kind;
		// the id of the copy
		int copy_id;

		friend BookMng;
		friend User;
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version) 
		{
			ar & create_time & kind & copy_id;
		}
	public:
		Book* Kind();
		time_t CreateTime();
		int   Id();
	};

}