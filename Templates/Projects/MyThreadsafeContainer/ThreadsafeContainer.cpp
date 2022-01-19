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

#include "src/TSContainer.h"

int main()
{
    std::cout << "Main thread id = " << std::this_thread::get_id() << std::endl;
    {
        TSContainer<std::vector<double>> container;

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

        /*t1.join();
        t2.join();*/

        a1.wait();
        a2.wait();

        container->push_back(container->front() + container->back());

        std::cout << std::endl << std::endl;

        for (const auto& el : container.values())
            std::cout << "\t" << el << std::endl;

        std::cout << std::endl << std::endl;
    }
    {
        TSContainer<std::unordered_map<int, int>> container;

        auto a1 = std::async(std::launch::async,
            [&]()
            {
                auto value = 10;
                std::random_device r;
                for (int i = 0; i < 9; ++i)
                {
                    auto key = r();
                    container->emplace(key, value);
                    ::Sleep(200);
                }
            });

        auto a2 = std::async(std::launch::async,
            [&]()
            {
                auto value = 100;
                std::random_device r;
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
}
