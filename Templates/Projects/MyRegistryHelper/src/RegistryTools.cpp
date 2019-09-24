
#include <Windows.h>
#include <string>

using RetType = std::optional<std::pair<std::wstring, LSTATUS>>;

#define ExitIfKeyInvalid														\
	if (nullptr == key.GetBase() || key.GetSubkey().empty())					\
	{																			\
		std::pair<std::wstring, LSTATUS>										\
			res(GetErrorMessage(ERROR_PATH_NOT_FOUND), ERROR_PATH_NOT_FOUND);	\
		return std::make_optional<decltype(res)>(res);							\
	}

#define ExitIfError(status)														\
	std::pair<std::wstring, LSTATUS> res(GetErrorMessage(status), status);		\
	return std::make_optional<decltype(res)>(res)									

class RKey
{
	HKEY			m_key{};
	HKEY			m_base{};
	std::wstring	m_subkey{};

public:
	RKey(HKEY baseKey = HKEY_CURRENT_USER, std::wstring subKey = L"") 
		: m_base(baseKey), m_subkey(subKey) {}
	~RKey() { Close(); }

	void Close()
	{
		if (nullptr != m_key)
			::RegCloseKey(m_key);
		m_key = nullptr;
	}
	void SetBase(const HKEY& root) { m_base = root; }
	auto GetBase() const { return m_base; }
	void SetSubkey(const std::wstring& subkey) { m_subkey.assign(subkey); }
	auto GetSubkey() const { return m_subkey; }
	void SetKey(const HKEY& key) { m_key = key; }
	auto& GetKey() const { return m_key; }
	operator bool() { return nullptr != m_key; }
};

std::wstring GetErrorMessage(LSTATUS status)
{
	// Receive the system message
	LPWSTR buffer{};
	//HMODULE hModule{};
	std::wstring message{};

	//hModule = LoadLibrary(L"wininet.dll");
	//if (hModule)
	{
		auto nTCHARs = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,//hModule,
			status,
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			(LPWSTR)&buffer,
			0,
			NULL);
		//FreeLibrary(hModule);
		if (nTCHARs)
		{
			message.assign(L"Error code: " + std::to_wstring(status) + L'\t');
			message.append(buffer, nTCHARs - 2); // -2 to remove \r\n symbols

			::LocalFree(buffer);
		}
	}

	return message;
}

LSTATUS GetKey(RKey& key, Registry::eAccessAndMask access)
{
	using namespace Registry;

	HKEY hKey;
	auto createStatus = CreateKey(key.GetBase(), key.GetSubkey().c_str(), &hKey, eOptions::Store, access);
	if (ERROR_SUCCESS == createStatus)
		key.SetKey(hKey);

	return createStatus;
}

RetType IsKeyExist(RKey& key)
{
	using namespace Registry;

	ExitIfKeyInvalid

	HKEY hKey;
	LSTATUS openStatus = OpenKey(key.GetBase(), key.GetSubkey().c_str(), &hKey, eAccessAndMask::ReadSubKey);
	if (ERROR_SUCCESS == openStatus)
	{
		CloseKey(hKey);
		return{};
	}

	ExitIfError(openStatus);
}

RetType OpenKey(RKey& key)
{
	ExitIfKeyInvalid

	HKEY hKey;
	auto openStatus = Registry::OpenKey(key.GetBase(), key.GetSubkey().c_str(), 
		&hKey, Registry::eAccessAndMask::Full);
	if (ERROR_SUCCESS == openStatus)
	{
		key.SetKey(hKey);
		return{};
	}

	ExitIfError(openStatus);
}

RetType CloseKey(RKey& key)
{
	ExitIfKeyInvalid

	auto closeStatus = Registry::CloseKey(key.GetKey());
	if (ERROR_SUCCESS == closeStatus)
	{
		key.SetKey(nullptr);
		return{};
	}

	ExitIfError(closeStatus);
}

RetType DeleteKey(RKey& key)
{
	ExitIfKeyInvalid

	using namespace Registry;

	if (key)
	{
		auto delStatus = DeleteKey(key.GetKey());
		if (ERROR_SUCCESS == delStatus)
			return{};

		ExitIfError(delStatus);
	}

	auto delStatus = DeleteKey(key.GetBase(), key.GetSubkey().c_str());
	if (ERROR_SUCCESS == delStatus)
		return{};

	ExitIfError(delStatus);
}

RetType DeleteKeysTree(RKey& key)
{
	ExitIfKeyInvalid

	if (!key)
	{
		auto errorStatus = OpenKey(key);
		if (errorStatus)
			return errorStatus;
	}

	auto delStatus = Registry::DeleteKeysTree(key.GetKey());
	if (ERROR_SUCCESS == delStatus)
	{
		key.SetKey(nullptr);
		return{};
	}

	auto closeStatus = CloseKey(key);
	ExitIfError(delStatus);
}
