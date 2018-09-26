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
    ~Strategy_1() override { std::cout << "Destructor Strategy 1" << std::endl; }

    bool Execute() override;
protected:

private:

};

class Strategy_2 : public BaseStrategy
{
public:
    Strategy_2() { std::cout << "Constructor Strategy 2" << std::endl; }
    ~Strategy_2() override { std::cout << "Destructor Strategy 2" << std::endl; }

    bool Execute() override;
protected:

private:

};

class ExecuteStrategy
{
public:
    ExecuteStrategy(BaseStrategy* strategy) 
    { 
        std::cout << "Constructor Execute Strategy" << std::endl;
        m_strategy = strategy; 
    };
    ~ExecuteStrategy() 
    { 
        std::cout << "Destructor Execute Strategy" << std::endl; 
        delete m_strategy; 
    };

    bool Execute();

protected:

private:
    BaseStrategy* m_strategy;
};
