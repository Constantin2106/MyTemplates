#pragma once

#include <string>
#include <iostream>>

namespace StrategyClasses
{
	class Strategy_1
	{
		std::string mName = "Strategy_1";
		std::string mData{};

	public:
		Strategy_1(std::string data) : mData(data)
		{
			std::cout << "Constructor " << mName << " - " << mData << std::endl;
		}

		~Strategy_1()
		{
			std::cout << "Destructor " << mName << std::endl;
			if(!mData.empty())
				mData.clear();
		}

		const std::string GetName() const { return mName; }

		void Step_1()
		{
			std::cout << "Step 1 " << mName << std::endl;
		}
		void Step_2()
		{
			std::cout << "Step 2 " << mName << std::endl;
		}		
		void Step_3()
		{
			std::cout << "Step 3 " << mName << std::endl;
		}
	};

	class Strategy_2
	{
		std::string mName = "Strategy_2";
		std::string mData{};

	public:
		Strategy_2(std::string data) : mData(data)
		{
			std::cout << "Constructor " << mName << " - " << mData << std::endl;
		}

		~Strategy_2()
		{
			std::cout << "Destructor " << mName << std::endl;
			if (!mData.empty())
				mData.clear();
		}

		const std::string GetName() const { return mName; }

		void Step_1()
		{
			std::cout << "Step 1 " << mName << std::endl;
		}
		void Step_2()
		{
			std::cout << "Step 2 " << mName << std::endl;
		}
		void Step_3()
		{
			std::cout << "Step 3 " << mName << std::endl;
		}
	};

}
