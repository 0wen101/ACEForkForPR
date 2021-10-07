/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "fs.h"
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
#  if !defined(NOMINMAX) || !defined(WIN32_LEAN_AND_MEAN)
#    define NOMINMAX
#    define WIN32_LEAN_AND_MEAN
#    include <windows.h>
#    undef WIN32_LEAN_AND_MEAN
#    undef NOMINMAX
#  else
#    include <windows.h>
#  endif
#endif

namespace nFs {

bool dirCreate(const std::string &szPath)
{
	#if defined(_WIN32)
		auto Error = CreateDirectoryA(szPath.c_str(), 0);
	#else
		auto Error = mkdir(szPath.c_str(),0733);
	#endif

	return Error != 0;
}

bool isDir(const std::string &szPath)
{
	struct stat info;

	if(stat( szPath.c_str(), &info ) != 0) {
			return false;
	}
	if(info.st_mode & S_IFDIR) {  // S_ISDIR() doesn't exist on my windows
			return true;
	}
	return false;
}

std::string getExt(const std::string &szPath)
{
	auto DotPos = szPath.find_last_of('.');
	if(DotPos == std::string::npos) {
		return "";
	}
	return szPath.substr(DotPos + 1);
}

std::string trimExt(const std::string &szPath)
{
	auto DotPos = szPath.find_last_of('.');
	if(DotPos == std::string::npos) {
		return "";
	}
	return szPath.substr(0, DotPos);
}

std::string getBaseName(const std::string &szPath)
{
	auto LastSlash = szPath.find_last_of('/');
	if(LastSlash == std::string::npos) {
		LastSlash = 0;
	}
	auto LastBackslash = szPath.find_last_of('\\');
	if(LastBackslash == std::string::npos) {
		LastBackslash = 0;
	}
	return szPath.substr(std::max(LastSlash, LastBackslash));
}

} // namespace nFs
