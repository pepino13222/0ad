#include "precompiled.h"

#include "CmdLineArgs.h"

CmdLineArgs::CmdLineArgs(int argc, char* argv[])
{
	if (argc >= 1)
		m_Arg0 = argv[0];

	for (int i = 1; i < argc; ++i)
	{
		// Only accept arguments that start with '-'
		if (argv[i][0] != '-')
			continue;

		CStr name, value;

		// Check for "-arg=value"
		const char* eq = strchr(argv[i], '=');
		if (eq)
		{
			name = CStr(argv[i]+1, eq-argv[i]-1);
			value = CStr(eq+1);
		}
		else
		{
			name = CStr(argv[i]+1);
		}

		m_Args.push_back(make_pair(name, value));
	}
}

template<typename T>
struct first_equals
{
	T x;
	first_equals(const T& x) : x(x) {}
	template<typename S> bool operator()(const S& v) { return v.first == x; }
};

bool CmdLineArgs::Has(const char* name) const
{
	return find_if(m_Args.begin(), m_Args.end(), first_equals<CStr>(name)) != m_Args.end();
}

CStr CmdLineArgs::Get(const char* name) const
{
	ArgsT::const_iterator it = find_if(m_Args.begin(), m_Args.end(), first_equals<CStr>(name));
	if (it != m_Args.end())
		return it->second;
	else
		return "";
}

std::vector<CStr> CmdLineArgs::GetMultiple(const char* name) const
{
	std::vector<CStr> values;
	ArgsT::const_iterator it = m_Args.begin();
	while (1)
	{
		it = find_if(it, m_Args.end(), first_equals<CStr>(name));
		if (it == m_Args.end())
			break;
		values.push_back(it->second);
		++it; // start searching from the next one in the next iteration
	}
	return values;
}

CStr CmdLineArgs::GetArg0() const
{
	return m_Arg0;
}
