#include "stdafx.h"
#include "Config.h"
#include "Core.h"
#include <iostream>
#include <iomanip>

const char* welcom = "A simple book store ~ \n\
writen by Xero Essential (ÐìÆôÔ´ 3130000950).\n\
\n\
Whenever met problem, type \'?\'\n\
Don't panic!\n\
\n\
";

namespace BookStore{

	BookMng* Core::BookMng;
	UserMng* Core::UserMng;
	StatusMng* Core::StatusMng;
	RoleManage* Core::RoleMng;
	StorageMng* Core::StorageMng;
	User* Core::User;
	Status Core::login_sta, Core::user_sta, Core::book_mng_sta, Core::user_mng_sta;

	void Core::Launch()
	{
		Core::StorageMng = new BookStore::StorageMng();
		bool init_require = !Core::StorageMng->DbExist();
		Core::UserMng = BookStore::UserMng::Load();
		Core::BookMng = BookStore::BookMng::Load();
		Core::StorageMng->CloseLoadDb();
		if (init_require) Core::UserMng->GlobalInit();
		if (init_require) Core::BookMng->GlobalInit();
		// UI Lauch
		Core::StatusMng = new BookStore::StatusMng();
		Core::RoleMng = RoleManage::Load();
		Core::StatusLoad();
		Core::StatusMng->init(login_sta);
		cout << welcom;
		Core::StatusMng->run_interactive();
	}
	void Core::Prepare(int argc, char* argv[])
	{
		Config::ConfigEnviron(argc, argv);
	}
	void Core::Terminate()
	{
		Core::UserMng->Save();
		Core::BookMng->Save();
		Core::StorageMng->CloseSaveDb();
	}


	Book* ui_select_book(ActionEvent& evt)
	{
		Book* bk = NULL; string name;
		if (evt.argu_contains("id")) bk = Core::BookMng->Find(to_i(evt.get("id")));
		if (!bk && evt.argu_contains("name")) bk = Core::BookMng->Find(name = evt.get("name"));
		bk = Core::BookMng->Find(name = StatusMng::ui_read_val("name"));
		if (!bk) { cout << "Not found the book named " << name << endl; return NULL; }
		return bk;
	}
	template<typename TVal>
	TVal throw_on_error(TVal val, string msg)
	{
		if (!val) throw OperationFail(msg);
		return val;
	}
	void PrintCopys(User* user)
	{
		cout << setw(4) << "id" << setw(20) << "name" << setw(26) << "bought time" << endl;
		user->EachMyCopy([&](Copy* copy) { 
			cout << setw(4) << copy->Id() << ' ' << setw(20) << copy->Kind()->Name() \
			<< ' ' << setw(26) << user->BoughtTime(copy) << endl; });
	}
	Copy* ui_select_copy(User* user)
	{
		PrintCopys(user);
		cout << "Select a copy by id : ";
		int id; cin >> id;
		return throw_on_error(user->FindMyCopy(id), "You don't have this copy!");
	}
	template<typename TKey, typename TVal>
	void PrintBooks(const BookRep<TKey, TVal>& books)
	{
		cout << setw(4) << "id" << setw(20) << "name" << setw(20) << "author" << endl;
		books.EachBook([](Book* book, ...){ cout << setw(4) << book->Id() << setw(20) << book->Name()
			<< setw(20) << book->Author() << endl; });
	}

	void Core::StatusLoad()
	{
		Core::StatusMng->program_name = "Book store";
		login_sta = Status("Home Page");
		user_mng_sta = Status("User manage page");
		book_mng_sta = Status("Book manage page");

		login_sta.add_action(Action::create("login", ToUIFunc([](ActionEvent& evt)
		{
			Core::User = Core::UserMng->Login(evt.get("name"), evt.get("passwd"));
			if (!Core::User) return;
			Core::StatusMng->trans(Core::user_sta);
			Core::user_sta.name = string("User ") + Core::User->Name();
		}),
		{ "name", "passwd" }, "Login into system"));

		login_sta.add_action(Action::create("register", ToUIFunc([](ActionEvent& evt)
		{ Core::UserMng->Register(evt.get("name"), evt.get("passwd")); }),
		{ "name", "passwd" }, "Register a user"));

		user_sta = Status("User page");
		user_sta.add_action(Action::create("buy", ToUIFunc([](ActionEvent& evt)
		{
			Core::User->Buy(throw_on_error(ui_select_book(evt), "Terminated."));
		}), vector<string>(), "Buy a book."));

		user_sta.add_action(Action::create("order", ToUIFunc([](ActionEvent& evt)
		{
			auto book = throw_on_error(ui_select_book(evt), "Termianted.");
			Core::User->Order(book, to_i(evt.get("num", "1")));
		}), vector<string>(), "Order a book, Staff only, optional parameter : num (number of copys)"));

		user_sta.add_action(Action::create("add", ToUIFunc([](ActionEvent& evt)
		{
			Core::User->Add(throw_on_error(ui_select_book(evt), "Termianted."), to_i(evt.get("num", "1")));
		}), vector<string>(), "Add a book to your reposity, Administrator only, optional parameter : num (number of copys)"));

		user_sta.add_action(Action::create("return", ToUIFunc([](ActionEvent& evt)
		{
			Core::User->Return(ui_select_copy(Core::User));
		}), vector<string>(), "Return a book"));

		user_sta.add_action(Action::create("charge", ToUIFunc([](ActionEvent& evt){
			try { Core::User->ChargeMoney((float)to_d(evt.get("money"))); }
			catch (const UnauthoritedError& ue) 
			{ cout << "Please contact admnistrator to charge the money!" << endl
			  << "I mean, give admin monney and admin will charge your account."<< endl; }
		}), { "money" }, "Charge money~"));

		user_sta.add_action(Action::create("seek", ToUIFunc([](ActionEvent& evt){
			cout << setw(10) << "User : " << Core::User->Name() << endl
			<< setw(10) << "Money : " << Core::User->MyMoney() << endl
			<< setw(10) << "Role : " << Core::User->Role() << endl;
		}), vector<string>(), "Seek my information."));

		user_sta.add_action(Action::create("to_book_manager", ToUIFunc([](ActionEvent& evt){
			RoleManage::Valid(Core::User, Role::Staff);
			Core::StatusMng->trans(Core::book_mng_sta);
		}), vector<string>(), "Open book manager"));

		user_sta.add_action(Action::create("to_user_manager", ToUIFunc([](ActionEvent& evt){
			RoleManage::Valid(Core::User, Role::Admin);
			Core::StatusMng->trans(Core::user_mng_sta);
		}), vector<string>(), "Open user manager"));

		user_sta.add_action(Action::create("list", ToUIFunc([](ActionEvent& evt){
			PrintCopys(Core::User);
		}), vector<string>(), "Print the list of your copy"));

		user_sta.add_action(Action::create("look_all", ToUIFunc([](ActionEvent& evt){
			PrintBooks(Core::BookMng->books);
		}), vector<string>(), "Print all book of the system"));

		book_mng_sta.add_action(Action::create("order", ToUIFunc([](ActionEvent& evt){
			auto book = throw_on_error(ui_select_book(evt), "Termianted.");
			Core::BookMng->Order(book, to_i(evt.get("num", "1")));
		}), vector<string>(), "Order books into system."));

		book_mng_sta.add_action(Action::create("register", ToUIFunc([](ActionEvent& evt){
			auto book = new Book(evt.get("name"), StatusMng::ui_read_long_val("abstract"),
				evt.get("author"), to_d(evt.get("price")));
			Core::BookMng->Add(book, to_i(evt.get("num", "0")));
		}), { "name", "author", "price" }, "Register book information."));

		user_mng_sta.add_action(Action::create("grant", ToUIFunc([](ActionEvent& evt){
			Core::UserMng->Grant(Core::UserMng->GetUser(evt.get("name")), 
				Core::RoleMng->Parse(evt.get("role"))); 
		}), { "name", "role" }, "Change the user's role."));

		user_mng_sta.add_action(Action::create("charge", ToUIFunc([](ActionEvent& evt){
			Core::UserMng->ChargeMoney(Core::UserMng->GetUser(evt.get("name")), to_d(evt.get("money")));
		}), { "name", "money" }, "Charge the user's money"));

		user_mng_sta.add_action(Action::create("register", ToUIFunc([](ActionEvent& evt)
		{ Core::UserMng->Register(evt.get("name"), evt.get("passwd")); }),
		{ "name", "passwd" }, "Register a user"));

		Action& back_user = Action::create("back", ToUIFunc([](ActionEvent& evt){
			Core::StatusMng->trans(Core::user_sta);
		}), vector<string>(), "Back to user page");

		user_mng_sta.add_action(back_user);
		book_mng_sta.add_action(back_user);
	}
}