#include <string>
#include <hash_map>

extern "C"
{
	__declspec( dllexport ) size_t __stdcall HashString(const char* hash)
	{
		static const std::hash<std::string> hasher;

		std::string hashStr(hash);

		return hasher(hashStr);
	}
}