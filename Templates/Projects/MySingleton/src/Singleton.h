#ifndef SINGLETON
#define SINGLETON

#include <string>

class TSingleton
{
public:
	static TSingleton* Instance();
	static bool Destroy(TSingleton* singleton);
	~TSingleton() {}

	bool Initialize();
	bool Free();
	std::string GetClassName() { return className; }
	int GetSize() { return size; }

private:
	static TSingleton* instance;
	const std::string className = "Singleton";

	int size;
	int* m_data;

	TSingleton() : m_data(nullptr), size(0) {}
	TSingleton(const TSingleton&) = delete;
	TSingleton& operator = (const TSingleton&) = delete;

};

#endif
