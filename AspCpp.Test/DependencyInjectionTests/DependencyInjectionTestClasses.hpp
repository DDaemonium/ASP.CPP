#pragma once

#include "ServiceCollection.hpp"

class DependencyInjectionTestClass;
class DependencyInjectionTestClassC;
class DependencyInjectionTestClassB;

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
	explicit DependencyInjectionTestClassB(
		AspCpp::Extensions::DependencyInjection::Dependency<DependencyInjectionTestClass> dependency,
		AspCpp::Extensions::DependencyInjection::Dependency<DependencyInjectionTestClassC> dependencyC)
	{
	}
};

class DependencyInjectionTestClassC
{
private:
public:
	explicit DependencyInjectionTestClassC(
		AspCpp::Extensions::DependencyInjection::Dependency<DependencyInjectionTestClass> dependency)
	{
	}
};
