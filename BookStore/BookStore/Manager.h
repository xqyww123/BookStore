/* Manage.h
 * 
 * by Xero Essential (–Ï∆Ù‘¥ 3130000950)
 */
#pragma once
#pragma once
#include "stdafx.h"
#include "Role.h"
#include <typeindex>
#include "User.h"

namespace BookStore{

	// the Manager base class
	// Manager is a singleton in program which manage objects.
	class Manager{

		// the required authority to manipulate the manager. 
		Role requirement;
		// A static dictionary to store all manager.
		static unordered_map<type_index, Manager*> managers;

	protected:
		// driverd: the driver class
		Manager(type_index drived, Role Requirement);

	public:
		// return the requirment.
		Role Requirement();
		// Get the manager.
		// User is required to check if authority is enough.
		template<typename TManager> 
		static TManager* GetManager(User* user)
		{
			auto man = managers[type_index(typeid(TManager))];
			RoleManage::Validate(user, requirement);
			return (TManager*)man;
		}
	};

}