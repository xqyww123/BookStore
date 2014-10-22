/* Shell.h
 * A light-weight status machine based Command Line framwork.
 * author   : Xero Essential.
 * study on : Zhejiang University.
 * Stu - id : 3130000950
 * 
 * Document:
 * At each time point, program keep on an identical status, which 
 * an instance of Status class. Status have some action, which 
 * user could invoke. Action can also call Status::tran function,
 * which make status transform so that user could visti each status
 * through the action.
 * The status and actions, consists an status machine.
 *
*/
#pragma once
#include "base.h"
#include "Manager.h"
#include <vector>
#include <string>
#include <functional>
#include "User.h"

using namespace std;

namespace BookStore {
	class Core;

	unordered_map<string, string>* parse_command(string command);

	// occurred when user typed an unknown action command.
	class ActionNotFound : public runtime_error
	{
	public:
		ActionNotFound() : runtime_error("Action not found") {}
		ActionNotFound(const string& str) : runtime_error(str) {}
	};

	class Status;
	class ActionEvent;

	class Action
	{
	public:
		Action() {}
		// to simplify the programming
		typedef function<void(ActionEvent&)> ActFunc;
		// not recommanded, cause you must convert lambda to ActFunc manual, 
		// please use create function which is more easy.
		Action(string _name, ActFunc _f) : name(_name), func(_f) {}
		Action(string _name, ActFunc _f, string _comment) :comment(_comment), name(_name), func(_f) {}

		string name;
		// will appear on help page
		string comment;
		ActFunc func;
		// the parameters list.
		// before invoke the function, all parameters will be acquired though UI
		vector<string> parameters;
		// invoke an action.
		void invoke(ActionEvent& evt);
		void invoke(Status& status);
		// use template to simplify programming
		template<typename TFunc> static Action create(string name, TFunc func, const vector<string>& params)
		{
			auto re = Action(name, (Action::ActFunc)func);
			re.parameters = params;
			return re;
		}
		template<typename TFunc> static Action create(string name, TFunc func, const vector<string>& params, string comment)
		{
			auto re = Action(name, (Action::ActFunc)func, comment);
			re.parameters = params;
			return re;
		}
	};
	// Provide information about the invocation
	// mind that Action::ActFunc only receive ActionEvent argument.
	class ActionEvent{
	public:
		// The invocation happend on which status
		Status* who;
		// the arguments, at least including Action::parameters
		unordered_map<string, string> arguments;
		// get an arguement by name
		string get(string name);
		// if an argument named NAME not provided, return _default.
		string get(string name, string _default);
		// get an argument by user input index
		string get(int index);
		// if arguments contains name
		bool argu_contains(string name);
	};
	class Status
	{
	public:
		Status() {}
		Status(string _name) : name(_name) {}
		Status(string _name, vector<Action> acts) : name(_name), actions(acts) {}
		vector<Action> actions;
		string name;

		void invoke(string name);
		void add_action(const Action& act);
	};
	class StatusMng : Manager
	{
		StatusMng();
		friend Core;
	public:

		static string program_name;
		static Status* current;
		// specials is a serial of action, invoked only when user try to
		// call an nonexist action, such as help and '?' action.
		// If user call an action but the action isn't found, then 
		// program will search the action in specials and try to invoke.
		static vector<Action> specials;

		// input the default status and init status machine
		static void init(Status& _default);
		// transform current status to identical status.
		static void trans(Status& status);
		// invoke a function on current status
		static void invoke(string name);
		// read a user input command.
		static void read_act();
		// start UI interactive
		static void run_interactive();
		// end the interactive
		static void end_interactive();
		//show help information about current status
		static void show_help();

		static string ui_read_val(string name);
		static string ui_read_long_val(string name);
	};

	template<typename TFunc>
	void ui_run(ActionEvent &evt, TFunc func)
	{
		try { func(evt); }
		catch (const OperationFail& oe)
		{
			cout << "Fail : " << oe.what() << endl;
		}
		catch (const UnauthoritedError& ue)
		{
			cout << "Fail : " << ue.what() << endl;
		}
		catch (const format_error& fe)
		{
			cout << "Fail : " << fe.what() << endl; 
		}
	}
	// convert a general function to a UI-friend function
	// which imported exception handing.
	template<typename TFunc> Action::ActFunc ToUIFunc(TFunc func)
	{
		return (Action::ActFunc)[=](ActionEvent& evt){ui_run(evt, func);  };
	}
}
