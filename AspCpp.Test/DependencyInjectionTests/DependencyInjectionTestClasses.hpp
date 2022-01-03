#pragma once

#include "ServiceCollection.hpp"

class DependencyInjectionTestClass
{
private:
	int _value;
public:
	explicit DependencyInjectionTestClass(int& ctorCallsCounter, int storedValue) : _value(storedValue)
	{
		++ctorCallsCounter;
	}

	int Value()
	{ 
		return _value;
	}
};

class DependencyInjectionTestClassB
{
private:
public:
	explicit DependencyInjectionTestClassB(int& ctorCallsCounter, int storedValue) : _value(storedValue)
	{
		++ctorCallsCounter;
	}

	int Value()
	{ 
		return _value;
	}
};
