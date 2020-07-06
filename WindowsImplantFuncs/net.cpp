#include "net.h"

string net_GetLocalGroupNames()
{
	string group_names = "";

	LOCALGROUP_INFO_0 *lginfo;
	DWORD read;
	DWORD total;

	LOCALGROUP_INFO_1 *lginfo1;
	DWORD read1;
	DWORD total1;

	NetLocalGroupEnum(NULL, 0, (unsigned char **)&lginfo, MAX_PREFERRED_LENGTH, &read, &total, NULL);
	NetLocalGroupEnum(NULL, 1, (unsigned char **)&lginfo1, MAX_PREFERRED_LENGTH, &read1, &total1, NULL);

	group_names += "Group Names\n\n";

	for (int i = 0; i < read; i++)
	{
		group_names += CW2A(lginfo[i].lgrpi0_name);
		group_names += "\t";
		group_names += CW2A(lginfo1[i].lgrpi1_comment);
		group_names += "\n";
	}

	return group_names;
}

string net_GetLocalGroupNames(LPCWSTR server)
{
	string group_names = "";

	LOCALGROUP_INFO_0 *lginfo;
	DWORD read;
	DWORD total;

	LOCALGROUP_INFO_1 *lginfo1;
	DWORD read1;
	DWORD total1;

	NetLocalGroupEnum(server, 0, (unsigned char **)&lginfo, MAX_PREFERRED_LENGTH, &read, &total, NULL);
	NetLocalGroupEnum(server, 1, (unsigned char **)&lginfo1, MAX_PREFERRED_LENGTH, &read1, &total1, NULL);

	group_names += "Group Names\n\n";

	for (int i = 0; i < read; i++)
	{
		group_names += CW2A(lginfo[i].lgrpi0_name);
		group_names += "\t";
		group_names += CW2A(lginfo1[i].lgrpi1_comment);
		group_names += "\n";
	}

	NetApiBufferFree(lginfo);
	NetApiBufferFree(lginfo1);

	return group_names;
}

string net_GetMembersOfLocalGroup(LPCWSTR groupname)
{
	string group_mem;
	LPWSTR sid_string;
	NET_API_STATUS status;

	LOCALGROUP_MEMBERS_INFO_2 *lpmeminfo;
	DWORD read;
	DWORD total;

	status = NetLocalGroupGetMembers(NULL, groupname, 2, (unsigned char **)&lpmeminfo, MAX_PREFERRED_LENGTH, &read, &total, NULL);
	cout << status << endl;
	group_mem += "Group Members\n";

	for (int i = 0; i < read; i++)
	{
		sid_string = (LPWSTR)"";
		ConvertSidToStringSid((lpmeminfo[i].lgrmi2_sid), &sid_string);
		group_mem += CW2A(sid_string);
		group_mem += "\t";
		group_mem += (lpmeminfo[i].lgrmi2_sidusage);
		group_mem += "\t";
		group_mem += CW2A(lpmeminfo[i].lgrmi2_domainandname);
		group_mem += "\n";

	}

	NetApiBufferFree(lpmeminfo);

	return group_mem;
}

string net_GetMembersOfLocalGroup(LPCWSTR groupname, LPCWSTR server)
{
	string group_mem;
	LPWSTR sid_string;
	NET_API_STATUS status;

	LOCALGROUP_MEMBERS_INFO_2 *lpmeminfo;
	DWORD read;
	DWORD total;

	status = NetLocalGroupGetMembers(server, groupname, 2, (unsigned char **)&lpmeminfo, MAX_PREFERRED_LENGTH, &read, &total, NULL);
	cout << status << endl;
	group_mem += "Group Members\n";

	for (int i = 0; i < read; i++)
	{
		sid_string = (LPWSTR)"";
		ConvertSidToStringSid((lpmeminfo[i].lgrmi2_sid), &sid_string);
		group_mem += CW2A(sid_string);
		group_mem += "\t";
		group_mem += (lpmeminfo[i].lgrmi2_sidusage);
		group_mem += "\t";
		group_mem += CW2A(lpmeminfo[i].lgrmi2_domainandname);
		group_mem += "\n";

	}

	NetApiBufferFree(lpmeminfo);

	return group_mem;
}

string net_GetLocalAccountNames()
{
	string usernames = "";
	NET_API_STATUS status;

	LPUSER_INFO_0 uinfo0;
	DWORD filter;
	DWORD read;
	DWORD total;

	usernames += "Users:\n";

	//the flag "FILTER_NORMAL_ACCOUNT" can be used instead of the second 0 to get only users, not sure which is better necessarily
	status = NetUserEnum(NULL, 0, 0, (unsigned char **)&uinfo0, MAX_PREFERRED_LENGTH, &read, &total, NULL);
	cout << status << endl;

	for (int i = 0; i < read; i++)
	{
		usernames += CW2A(uinfo0[i].usri0_name);
		usernames += "\n";
	}

	return usernames;
}

string net_GetLocalAccountNames(LPCWSTR server)
{
	string usernames = "";
	NET_API_STATUS status;

	LPUSER_INFO_0 uinfo0;
	DWORD read;
	DWORD total;

	usernames += "Users:\n";

	//the flag "FILTER_NORMAL_ACCOUNT" can be used instead of the second 0 to get only users, not sure which is better necessarily
	status = NetUserEnum(server, 0, 0, (unsigned char **)&uinfo0, MAX_PREFERRED_LENGTH, &read, &total, NULL);
	cout << status << endl;
	//tmpBuf = uinfo0;

	for (int i = 0; i < read; i++)
	{
		usernames += CW2A(uinfo0[i].usri0_name);
		usernames += "\n";
	}

	return usernames;
}

string net_GetUserInfo(LPCWSTR username)
{
	LPGROUP_USERS_INFO_1 lpgroup1;
	LPUSER_INFO_1 uinfo1;
	NET_API_STATUS ustatus, gstatus;

	DWORD gread;
	DWORD gtotal;

	string netuser = "";

	gstatus = NetUserGetGroups(NULL, username, 1, (unsigned char **)&lpgroup1, MAX_PREFERRED_LENGTH, &gread, &gtotal);
	ustatus = NetUserGetInfo(NULL, username, 1, (unsigned char **)&uinfo1);

	cout << ustatus << endl;
	netuser += "Net User:\n";

	netuser += "Username:\t";
	netuser += CW2A(uinfo1->usri1_name);
	netuser += "\n";
	netuser += "Comment:\t";
	netuser += CW2A(uinfo1->usri1_comment);
	netuser += "\n";
	netuser += "Password Age:\t";
	netuser += (uinfo1->usri1_password_age);
	netuser += "\n";
	netuser += "Homedir:\t";
	netuser += CW2A(uinfo1->usri1_home_dir);
	netuser += "\n";
	netuser += "Privs:\t";
	netuser += (uinfo1->usri1_priv);
	netuser += "\n";
	netuser += "Script Path:\t";
	netuser += CW2A(uinfo1->usri1_script_path);
	netuser += "\n";
	netuser += "Flags:\t";
	netuser += (uinfo1->usri1_flags);
	netuser += "\n\n";

	netuser += "Groups:\n";
	
	for (int i = 0; i < gread; i++)
	{
		netuser += CW2A(lpgroup1[i].grui1_name);
		netuser += "\n";
		netuser += (lpgroup1[i].grui1_attributes);
		netuser += "\n";
	}

	return netuser;

}

string net_GetUserInfo(LPCWSTR username, LPCWSTR server)
{
	LPGROUP_USERS_INFO_1 lpgroup1;
	LPUSER_INFO_1 uinfo1;
	NET_API_STATUS ustatus, gstatus;

	DWORD gread;
	DWORD gtotal;

	string netuser = "";

	gstatus = NetUserGetGroups(server, username, 1, (unsigned char **)&lpgroup1, MAX_PREFERRED_LENGTH, &gread, &gtotal);
	ustatus = NetUserGetInfo(server, username, 1, (unsigned char **)&uinfo1);

	cout << ustatus << endl;
	netuser += "Net User:\n";

	netuser += "Username:\t";
	netuser += CW2A(uinfo1->usri1_name);
	netuser += "\n";
	netuser += "Comment:\t";
	netuser += CW2A(uinfo1->usri1_comment);
	netuser += "\n";
	netuser += "Password Age:\t";
	netuser += (uinfo1->usri1_password_age);
	netuser += "\n";
	netuser += "Homedir:\t";
	netuser += CW2A(uinfo1->usri1_home_dir);
	netuser += "\n";
	netuser += "Privs:\t";
	netuser += (uinfo1->usri1_priv);
	netuser += "\n";
	netuser += "Script Path:\t";
	netuser += CW2A(uinfo1->usri1_script_path);
	netuser += "\n";
	netuser += "Flags:\t";
	netuser += (uinfo1->usri1_flags);
	netuser += "\n\n";

	netuser += "Groups:\n";

	for (int i = 0; i < gread; i++)
	{
		netuser += CW2A(lpgroup1[i].grui1_name);
		netuser += "\n";
		netuser += (lpgroup1[i].grui1_attributes);
		netuser += "\n";
	}

	return netuser;

}

string net_GetSharedResources()
{
	string shares = "";
	PSHARE_INFO_1 sinfo, p;
	DWORD read;
	DWORD total;
	NET_API_STATUS status;

	status = NetShareEnum(NULL, 1, (LPBYTE *)&sinfo, MAX_PREFERRED_LENGTH, &read, &total, NULL);

	shares += "Shares:\n";

	p = sinfo;

	for (int i = 0; i < read; i++)
	{
		shares += CW2A( sinfo[i].shi1_netname);
		shares += "\t";
		shares += (sinfo[i].shi1_type);
		shares += "\t";
		shares += CW2A(sinfo[i].shi1_remark);
		shares += "\n";
	}

	return shares;
}

string net_GetSharedResources(LPWSTR server)
{
	string shares = "";
	PSHARE_INFO_1 sinfo, p;
	DWORD read;
	DWORD total;
	NET_API_STATUS status;

	status = NetShareEnum(server, 1, (LPBYTE *)&sinfo, MAX_PREFERRED_LENGTH, &read, &total, NULL);

	shares += "Shares:\n";

	p = sinfo;

	for (int i = 0; i < read; i++)
	{
		shares += CW2A(sinfo[i].shi1_netname);
		shares += "\t";
		shares += (sinfo[i].shi1_type);
		shares += "\t";
		shares += CW2A(sinfo[i].shi1_remark);
		shares += "\n";
	}

	return shares;
}