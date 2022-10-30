// ThreadsafeContainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vld.h>
#include <Windows.h>

#include <iostream>
#include <vector>
#include <future>
#include <unordered_map>
#include <random>
#include <chrono>
#include <map>


#include "src/TSContainer.h"

int main()
{
    std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;
    
    {
        std::cout << "<------- std::vector ------->" << std::endl;
        TSContainer<std::vector<double>> container(10, -1);

        //std::thread t1(
        auto a1 = std::async(std::launch::async,
            [&]()
            {
                double val = 1.1;
                for (int i = 0; i < 9; ++i)
                {
                    container->push_back(val);
                    val += 0.1;
                    ::Sleep(200);
                }
            });

        //std::thread t2(
        auto a2 = std::async(std::launch::async,
            [&]()
            {
                double val = 2.1;
                for (int i = 0; i < 9; ++i)
                {
                    container->push_back(val);
                    val += 0.1;
                    //container->push_back(container->back() + 10);
                    ::Sleep(500);
                }
            });

        //t1.join();
        //t2.join();

        a1.wait();
        a2.wait();

        container->push_back(container->front() + container->back());

        std::cout << std::endl << std::endl;

        for (const auto& el : container.values())
            std::cout << "\t" << el << std::endl;

        std::cout << std::endl << std::endl;
    }
    {
        std::cout << "<------- std::unordered_map ------->" << std::endl;
        TSContainer<std::unordered_map<int, int>> container;

        auto a1 = std::async(std::launch::async,
            [&]()
            {
                auto value = 10;
                std::random_device r("123");
                for (int i = 0; i < 9; ++i)
                {
                    auto key = r();
                    container->emplace(key, value);
                    ::Sleep(500);
                }
            });

        auto a2 = std::async(std::launch::async,
            [&]()
            {
                auto value = 100;
                std::random_device r("456");
                for (int i = 0; i < 9; ++i)
                {
                    auto key = r();
                    container->emplace(key, value);
                    ::Sleep(200);
                }
            });

        a1.wait();
        a2.wait();

        for (const auto& el : container.values())
            std::cout << "\t" << el.first << "\t" << el.second << std::endl;

        std::cout << std::endl << std::endl;
    } 
    {
        std::cout << "<------- std::unordered_multimap ------->" << std::endl;
        TSContainer<std::unordered_multimap<std::thread::id, long long>> container;
        auto start = std::chrono::high_resolution_clock::now();

        auto a1 = std::async(std::launch::async,
            [&]()
            {
                auto value = 10;
                for (int i = 0; i < 9; ++i)
                {
                    auto elapsed = std::chrono::high_resolution_clock::now() - start;
                    container->emplace(std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count());
                    ::Sleep(500);
                }
            });

        auto a2 = std::async(std::launch::async,
            [&]()
            {
                auto value = 100;
                for (int i = 0; i < 9; ++i)
                {
                    auto elapsed = std::chrono::high_resolution_clock::now() - start;
                    container->emplace(std::this_thread::get_id(), std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count());
                    ::Sleep(200);
                }
            });

        a1.wait();
        a2.wait();

        for (const auto& el : container.values())
            std::cout << "\t" << el.first << "\t" << el.second << std::endl;

        std::cout << std::endl << std::endl;
    }
    {
        std::cout << "<------- std::map ------->" << std::endl;
        TSContainer<std::map<std::string, std::pair<std::string, int>>> safeMap;
        auto func = [](decltype(safeMap)& safeMap)
        {
            safeMap->emplace("apple", std::make_pair<std::string, int>("fruit", 0));
            safeMap->emplace("potato", std::make_pair<std::string, int>("vegetable", 0));

            for (size_t i = 0; i < 10000; ++i)
            {
                safeMap->at("apple").second++;
                safeMap->at("potato").second++;
            }

            //auto const readOnlySafeMap = safeMap;
            std::cout << "potato is " << safeMap->at("potato").first <<
                " " << safeMap->at("potato").second <<
                ", apple is " << safeMap->at("apple").first <<
                " " << safeMap->at("apple").second << std::endl;
        };
    
        std::vector<std::thread> threads(10);

        for (auto& i : threads)
        {
            i = std::move(std::thread([&]() 
                {
                    func(safeMap);
                }));
        }

        for (auto& i : threads) i.join();
    }
}
