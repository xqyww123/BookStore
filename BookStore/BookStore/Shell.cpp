#include "stdafx.h"
#include "base.h"
#include "Shell.h"
#include <iostream>
#include "Role.h"
#include <iomanip>

using namespace std;

namespace BookStore{

	Status* StatusMng::current;
	vector<Action> StatusMng::specials;
	string StatusMng::program_name;
	void StatusMng::init(Status& _default)
	{
		trans(_default);
		specials.push_back(Action("exit", [](ActionEvent& a){StatusMng::end_interactive(); }));
		specials.push_back(Action("?", [](ActionEvent& a){StatusMng::show_help(); }));
		specials.push_back(Action("help", [](ActionEvent& a){StatusMng::show_help(); }));
	}
	void Action::invoke(ActionEvent& evt) 
	{ 
		func(evt); 
	}
	void Action::invoke(Status& status)
	{
		ActionEvent evt;
		evt.who = &status;
		string line; getline(cin, line);
		auto dic = parse_command(line);
		evt.arguments = *dic;
		for (auto a : parameters)
		{
			if (dic->find(a) == dic->end())
				evt.arguments.insert(make_pair(a, StatusMng::ui_read_val(a)));
		}
		delete dic;
		invoke(evt);
	}
	void Status::invoke(string name)
	{
		int may = -1;
		for (size_t i = 0; i < actions.size(); ++i)
		{
			Action& a = actions[i];
			if (a.name == name) { a.invoke(*this); return; }
			if (like(a.name, name)) may = (may == -1 ? i : -2);
		}
		if (may >= 0) { actions[may].invoke(*this); return; }
		throw ActionNotFound((string("Action ") + name + " not found = =!"));
	}
	void StatusMng::trans(Status& status)
	{
		current = &status;
	}
	void StatusMng::invoke(string name)
	{
		int may = -1;
		try { current->invoke(name); }
		catch (const ActionNotFound& e) {
			for (size_t i = 0; i < specials.size(); ++i)
			{
				Action& a = specials[i];
				if (a.name == name) { a.invoke(*current); return; }
				if (like(a.name, name)) may = (may == -1 ? i : -2);
			}
			if (may >= 0)
			{
				specials[may].invoke(*current); return;
			}
			throw e;
		}
	}
	void StatusMng::read_act()
	{
		cout << program_name << " : " << current->name << " > ";
		string name;
		cin >> name;
		invoke(name);
	}
	bool interactive_break;
	void StatusMng::end_interactive() { interactive_break = true; }
	void StatusMng::run_interactive()
	{
		interactive_break = false;
		while (!interactive_break)
		{
			try { read_act(); }
			catch (const ActionNotFound& e)
			{
				puts(e.what());
			}
		}
	}
	void StatusMng::show_help()
	{
		puts("Usable method:");
		for (auto a : current->actions)
			cout << left << setw(10) << a.name << " " << left << setw(50) << a.comment << endl;
		puts("");
	}
	void Status::add_action(const Action& act)
	{
		for (size_t i = 0; i < actions.size(); ++i)
		if (actions[i].name == act.name)
		{
			actions[i] = act; return;
		}
		actions.push_back(act);
	}
	StatusMng::StatusMng() : Manager(type_index(typeid(StatusMng)), Role::Admin) {}

	string ActionEvent::get(string name)
	{
		return arguments[name];
	}
	string ActionEvent::get(int index)
	{
		return arguments[string("@") + to_string(index)];
	}
	bool ActionEvent::argu_contains(string name)
	{
		return arguments.find(name) != arguments.end();
	}
	string ActionEvent::get(string name, string _default)
	{
		if (!argu_contains(name))
			return _default;
		return get(name);
	}


	unordered_map<string, string>* parse_command(string command)
	{
		auto re = new unordered_map<string, string>();
		
		int sti, vsti, cli, segi, vsted, len = command.length(), mode;
		sti = vsti = cli = segi = mode = vsted = 0;

		for (int i = 0; i <= len; ++i)
		{
			if (mode & 1 && command[i] == ',' && !(mode & 4) || i == len)
			{
					string key, val;
					val = command.substr(vsti, vsted - vsti);
					if (cli)
					{
						key = command.substr(sti, cli - sti);
						val = command.substr(cli + 1, vsted - cli - 1);
						re->insert(make_pair(key, val));
					}
					key = string("@") + to_string(segi++);
					re->insert(make_pair(key, val));
					mode = cli = 0;
					continue;
			}
			if (command[i] != ' ' && command[i] != '\t' && command[i] != '\r' && command[i] != '\n')
			{
				if (!(mode & 1)) sti = vsti = i;
				mode |= 1, vsted = i+1;
			}
			if (mode & 1)
			{
				if (command[i] == ':' && !cli && !(mode&2) ) cli = i;
				if (command[i] == '"')
				{
					mode |= 2, mode ^= 4;
					if (!(mode & 8) && cli) vsti = i;
					if (!(mode & 4)) vsted = i;
					mode |= 8;
				}
			}
		}
		return re;
	}

	string StatusMng::ui_read_val(string name)
	{
		cout << name << " : ";
		string line;
		getline(cin, line);
		return line;
	}
	string StatusMng::ui_read_long_val(string name)
	{
		cout << name << " (empty line to stop) : ";
		string re, line;
		while (true)
		{
			getline(cin, line);
			if (line.empty()) break;
			re += line + "\n";
		}
		return re;
	}
}
