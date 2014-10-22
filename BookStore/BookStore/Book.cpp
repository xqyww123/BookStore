#include "stdafx.h"
#include "Book.h"
#include <ctime>
#include "Core.h"

namespace BookStore{


	string Book::Name() { return name; }
	string Book::Title() { return title; }
	string Book::Abstracts() { return abstracts; }
	string Book::Author() { return author; }
	float Book::Price() { return price; }
	time_t Copy::CreateTime() { return create_time; }
	Book* Copy::Kind() { return kind; }
	int Copy::Id() { return copy_id; }
	Copy::Copy(Book* _kind)
	{
		kind = _kind;
		copy_id = Core::BookMng->AsignCopyId();
		time(&create_time);
	}
	size_t Book::BookHash::operator() (const Book& book) const
	{
		return book.id;
	}
	Book::Book(string name, string abstracts, string author, float price)
	{
		this->name = name;
		this->abstracts = abstracts;
		this->author = author;
		this->price = price;
		this->id = Core::BookMng->AsignBookId();
	}
	Book::Book() {}
	Copy::Copy() {}
	int Book::Id() { return id; }
}