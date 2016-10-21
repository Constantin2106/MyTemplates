#ifndef FACTORY_METHOD
#define FACTORY_METHOD

#include <unordered_map>

//---------------------------------------------------------------------------//
class TBaseClass
{
public:
	static TBaseClass* CreateObject(std::string name);

	virtual bool Initialize() { return true; }
	virtual bool Free() { return true; }

	virtual std::string GetClassName() = 0;
	virtual ~TBaseClass();

private:
	typedef TBaseClass*(*creator)();
	typedef std::unordered_map<std::string, creator> TCreators;

	static TCreators Creators;
};

//---------------------------------------------------------------------------//
class TClass_1 : public TBaseClass
{
public:
	std::string GetClassName() { return className; }

private:
	friend TBaseClass;

	static TBaseClass* Create() { return new TClass_1(); }

	TClass_1() {}
	TClass_1(const TClass_1&) = delete;
	TClass_1& operator = (const TClass_1&) = delete;

	const std::string className = "Class_1";
};

//---------------------------------------------------------------------------//
class TClass_2 : public TBaseClass
{
public:
	bool Initialize() override;
	bool Free() override;

	std::string GetClassName() { return className; }
	size_t GetSize() { return data.size(); }
	int GetData(size_t n) { return data[n]; }

private:
	friend TBaseClass;

	static TBaseClass* Create() { return new TClass_2(); }

	TClass_2() {}
	TClass_2(const TClass_2&) = delete;
	TClass_2& operator = (const TClass_2&) = delete;

	const std::string className = "Class_2";
	std::vector<int> data;
};



#endif