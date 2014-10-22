#include "stdafx.h"
#include "Manager.h"
#include <exception>
#include "User.h"

namespace BookStore{

	unordered_map<type_index, Manager*> Manager::managers;
	Manager::Manager(type_index drived, Role req)
	{
		if (managers.find(drived) != managers.end()) throw logic_error("asdas");
		managers[drived] = this;
		requirement = req;
	}
	Role Manager::Requirement()
	{
		return requirement;
	}
}