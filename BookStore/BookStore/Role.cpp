#include "stdafx.h"
#include "Role.h"
#include "User.h"
#include <boost\format.hpp>
#include "Core.h"
#include <boost\algorithm\string.hpp>

namespace BookStore{

	RoleManage::RoleManage() : info({ { Role::Admin, RoleInfo{ Role::Admin, "Administrator" } },
	{ Role::Guest, RoleInfo{ Role::Guest, "Guest" } }, { Role::Staff, RoleInfo{ Role::Staff, "Staff" } } }) {}
	RoleManage* RoleManage::Load()
	{
		return new RoleManage();
	}
	string RoleManage::Explain(Role role)
	{
		string re;
		for (auto a : info)
		{
			if (Check(role, a.first))
			{
				if (re.size()) re += ", ";
				re += a.second.explain;
			}
		}
		if (!re.size()) re = "Empty";
		return re;
	}
	Role RoleManage::Parse(string str)
	{
		vector<string> sps;
		boost::split(sps, str, [](char a){return a == '|'; });
		Role re = Role::Empty;
		for (auto a : sps)
		{
			boost::trim(a);
			bool succ = false;
			for (auto b : info)
			if (b.second.explain == a)
				re = (Role)(re | b.first), succ = true;
			if (!succ)
				throw format_error("Format Error : Bad Role");
		}
		return re;
	}
	bool RoleManage::Check(Role now, Role Requirement)
	{
		return (now & Requirement) == Requirement;
	}
	bool RoleManage::Check(User* user, Role Requirement)
	{
		return RoleManage::Check(user->Role(), Requirement);
	}
	void RoleManage::Valid(Role now, Role Requirement)
	{
		if (!RoleManage::Check(now, Requirement)) throw UnauthoritedError(now, Requirement);
	}
	void RoleManage::Valid(User* user, Role Requirement)
	{
		if (!RoleManage::Check(user, Requirement)) throw UnauthoritedError(user, Requirement);
	}
	UnauthoritedError::UnauthoritedError(Role now, Role Requirement) :
		runtime_error(str(boost::format("Verification failed : require %s but provided %s") % \
		Core::RoleMng->Explain(Requirement) % Core::RoleMng->Explain(now))) { }
	UnauthoritedError::UnauthoritedError(User* user, Role Requirement) : 
		runtime_error(str(boost::format("Verification failed : require %s but the user %s only have %s") % \
		Core::RoleMng->Explain(Requirement) % user->Name() % Core::RoleMng->Explain(user->Role()))) { }

	ostream& operator<< (ostream& in, Role role)
	{
		return in << Core::RoleMng->Explain(role);
	}
}