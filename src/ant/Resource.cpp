#include <ant/resources/Resource.hpp>
#include <ant/ant_std.hpp>
#include <list>
#include <map>
#include <cctype>			// for std::tolower

using namespace ant;

ant::Resource::Resource(const std::string& name)
	:m_name(name)
{
	// Transform to lower case to unify
	std::transform(m_name.begin(), m_name.end(), m_name.begin(), (int(*)(int)) std::tolower);
}

ant::Resource::~Resource()
{

}

