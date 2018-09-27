// Templates.cpp: определяет точку входа для консольного приложения.
//
#include <stdlib.h>
#include <crtdbg.h>

#include <conio.h>
#include <vector>
#include <iostream>
#include <fstream>
//#include "json.hpp"

#include "src/Factory Method/FactoryMethodTest.h"
#include "src/Singleton/SingletonTest.h"
#include "src/Thread/ThreadTest.h"
#include "src/PImp/PImplTest.h"
#include "src/Strategy/StrategyTest.h"
#include "src/Callback/CallbackTest.h"
#include "src/Visitor/VisitorTest.h"
#include "src/Delegate/DelegateTest.h"
#include "src/Delegate/MyDelegateTest.h"
#include "src/Decorator/DecoratorTest.h"

#include <vld.h>


using namespace std;
//using json = nlohmann::json;


int main()
{
	//FactoryMethodTest();
	//SingletonTest();
	ThreadTest();
    //PImplTest();
    //StrategyTest();
    LambdaTest();
    //VisitorTest();
	//DelegateTest();
   //MyDelegateTest();
   //DecoratorTest();

	/*
	json Json;
	ifstream input_file("items.json");
	
	Json << input_file;
	input_file.close();

	if (Json.is_object())
	{
		cout << "object" << endl;
		cout << Json << endl;
	}
	else
	{
		cout << "not object" << endl;
		if (Json.is_array())
		{
			cout << "array" << endl;
			string str;
			string key, val, group, name, length, width;
			float l, w;

			cout << "key: ";
			cin >> key;
			cout << "val: ";
			cin >> val;

			for (auto j : Json)
			{
				if (j[key] == val)
				{
					group = j["group"];
					name = j["name"];
					cout << group << "  " << name << endl;

					json j1 = j["properties"];
					for (auto j2 : j1)
					{
						if (j2["name"] == "length")
						{
							str = j2["value"];
							cout << "length = " << str << endl;
							l = atof(str.c_str());
						}
						else if (j2["name"] == "width")
						{
							str = j2["value"];
							cout << "width = " << str << endl;
							w = atof(str.c_str());
						}
					}

					break;
				}
			}

			string dataFileName = "./" + group + "/" + name;
			ifstream dataFile(dataFileName);
			if (dataFile.is_open())
			{
				char data[20];
				dataFile.getline(data, 20);
				dataFile.close();
			
				cout << data;
			}
		}
	}
	_getch();

	//Json::Value root;
	*/

	return 0;
}