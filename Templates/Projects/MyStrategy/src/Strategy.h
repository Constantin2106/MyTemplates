#pragma once

#include <memory>
#include <iostream>

class BaseStrategy
{
public:
    BaseStrategy() { std::cout << "Constructor Base Strategy" << std::endl; }
    virtual ~BaseStrategy() { std::cout << "Destructor Base Strategy" << std::endl; }

    virtual bool Execute() = 0;

protected:

private:

};

class Strategy_1 : public BaseStrategy
{
public:
    Strategy_1() { std::cout << "Constructor Strategy 1" << std::endl; }
    ~Strategy_1() override 
    { 
        std::cout << "Destructor Strategy 1" << std::endl; 
        // TODO: resource free
    }

    bool Execute() override;

protected:

private:

};

class Strategy_2 : public BaseStrategy
{
public:
    Strategy_2() { std::cout << "Constructor Strategy 2" << std::endl; }
    ~Strategy_2() override 
    { 
        std::cout << "Destructor Strategy 2" << std::endl; 
        // TODO: resource free
    }

    bool Execute() override;

protected:

private:

};

class StrategyExecutor
{
public:
    StrategyExecutor();

    StrategyExecutor(const StrategyExecutor&) = delete;
    StrategyExecutor(StrategyExecutor&&) = delete;
    StrategyExecutor& operator=(const StrategyExecutor&) = delete;
    StrategyExecutor&& operator=(StrategyExecutor&&) = delete;

    ~StrategyExecutor();
    
    bool SetStrategy(BaseStrategy* _strategy);
    bool Execute();

protected:

private:
    BaseStrategy* m_strategy;
};
