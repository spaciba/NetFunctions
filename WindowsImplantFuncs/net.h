#pragma once
#include <Windows.h>
#include <LMaccess.h>
#include <LM.h>
#include <string>
#include <cstring>
#include <atlstr.h>
#include <lmapibuf.h>
#include <sddl.h>
#include <iostream>

using namespace std;

string net_GetLocalGroupNames();
string net_GetMembersOfLocalGroup(LPCWSTR groupname);
string net_GetLocalAccountNames();
string net_GetUserInfo(LPCWSTR username);
string net_GetSharedResources();

string net_GetLocalGroupNames(LPCWSTR server);
string net_GetMembersOfLocalGroup(LPCWSTR groupname, LPCWSTR server);
string net_GetLocalAccountNames(LPCWSTR server);
string net_GetUserInfo(LPCWSTR username, LPCWSTR server);
string net_GetSharedResources(LPCWSTR server);

