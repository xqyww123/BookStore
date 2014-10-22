/* Role.h
 * main file about role and autorization control.
 * 
 * by Xero Essential (–Ï∆Ù‘¥ 3130000950)
*/
#pragma once
#include "stdafx.h"
#include <exception>
#include <iostream>

namespace BookStore {

	class User;
	enum Role;

	class UnauthoritedError : public runtime_error {
	public:
		UnauthoritedError(Role now, Role Requirement);
		UnauthoritedError(User* user, Role Requirement);
	};

	// Role class : Main object of role control.
	// Each function in user interface will check if user have enough priority
	// by checking user's role property
	enum Role
	{
		Empty = 0,
		Guest = 1,
		Admin = 4,
		Staff = 2,
		All	  = 7
	};

	ostream& operator<< (ostream& in, Role role);

	// Role Info : provide rich info about each role
	// Main point is that you can only transmit light Role rather than heavy 
	// and senseless big object.
	class RoleInfo
	{
	public:
		Role role;
		string explain;
	};

	// Provide some api about role,
	// such as the transformation between role and its info object. 
	class RoleManage
	{
		map<Role, RoleInfo> info;
		RoleManage();
	public:
		// Explain the role to a human readable string
		string Explain(Role role);
		Role Parse(string str);
		// Check if now have enough authority for requirement
		static bool Check(Role now, Role Requirement);
		static bool Check(User* user, Role Requirement);
		// Validate if enough authority provided else UnanthoritedError occurred.
		static void Valid(Role now, Role Requirement);
		static void Valid(User* user, Role Requirement);

		static RoleManage* Load();
	};
}