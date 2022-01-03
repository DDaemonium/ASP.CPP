// ASP.CPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Core/DependencyInjection/ServiceCollection.hpp"
#include <iostream>

using namespace std;
using namespace AspCpp::Extensions::DependencyInjection;

class Foo {
private:
	int _v;
public:
	Foo(int v):_v(v) {
		cout << "\nFooCreated!\n";
	}

	void print() {
		cout << _v << "foo";
	}
};

class Bar {
private:
	Singleton<Foo> m_foo;
public:
	Bar(Singleton<Foo> ptr): m_foo(ptr) {
		cout << "\nBarCreated!\n";
	}
	void print() {
		cout << '\n';
		m_foo->print();
		cout << "bar";
	}
};

int main()
{
	ServiceCollection collection;
	collection.AddSingleton<Foo>([](ServiceProvider& sp) { return new Foo(2); });
	collection.AddTransient<Bar>([](ServiceProvider& sp) { return new Bar(sp.ResolveSingleton<Foo>()); });

	decltype(auto) serviceProvider = collection.BuildServiceProvider();
	auto bar = serviceProvider->ResolveTransient<Bar>();
	bar->print();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
