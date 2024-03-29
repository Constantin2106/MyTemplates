#ifndef FACTORY_METHOD
#define FACTORY_METHOD

#include <unordered_map>
#include <memory>

//---------------------------------------------------------------------------//
/*
Description
   auto* inst = FactoryFun<Class, argtype1, argtype2, ..., argtypen>(args...);
   Creates an instance of a Class by means function template

Arguments
   Arguments of the class constructor

Return

History
   Created by Kostiantyn Zhelieznov   09/01/2018
*/
template<typename T, typename... Arg>
T* FactoryFun(Arg&&... args)
{
   return new T(std::forward<Arg>(args)...);
}
template<typename T>
void DesroyFun(T* t)
{
	delete t;
}

/*
Description
   auto* inst = Factory<Class, argtype1, argtype2, ..., argntype>(args...);
   Creates the instance of the Class by means the static function of the class template

Arguments
   Arguments of the class constructor

Return

History
   Created by Kostiantyn Zhelieznov   09/01/2018
*/
template<typename T, typename... Arg>
class Factory
{
public:
   static T* Create(Arg&&... args)
   {
      return new T(std::forward<Arg>(args)...);
   }
   static void Destroy(T* t)
   {
	   delete t;
   }
};

//---------------------------------------------------------------------------//
/*
Description
   auto* inst = static_cast<Class*>(BaseClass::CreateObject("class"));
   Creates the instance of the Class using map of pair <"class name", pointer_to_creating_function>

Arguments

Return

History
   Created by Kostiantyn Zhelieznov   09/01/2018
*/
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