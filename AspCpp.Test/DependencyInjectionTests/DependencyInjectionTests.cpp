#include "gtest/gtest.h"

#include "ServiceCollection.hpp"
#include "DependencyInjectionTestClasses.hpp"

using namespace AspCpp::Extensions::DependencyInjection;

TEST(SingletonResolutionTest, ShouldCreateSingleInstance) {
	// Arrange
	int ctorCallsCounter = 0;
	ServiceCollection collection;
	collection.AddSingleton<DependencyInjectionTestClass>([&](ServiceProvider& sp) { return new DependencyInjectionTestClass(ctorCallsCounter, 2); });
	decltype(auto) serviceProvider = collection.BuildServiceProvider();

	// Act
	decltype(auto) instance1 = serviceProvider->Resolve<DependencyInjectionTestClass>();
	decltype(auto) instance2 = serviceProvider->Resolve<DependencyInjectionTestClass>();
	decltype(auto) instance3 = serviceProvider->Resolve<DependencyInjectionTestClass>();

	// Assert
	EXPECT_EQ(ctorCallsCounter, 1);
}

TEST(TransientResolutionTest, ShouldCreate3Instances) {
	// Arrange
	int ctorCallsCounter = 0;
	ServiceCollection collection;
	collection.AddTransient<DependencyInjectionTestClass>([&](ServiceProvider& sp) { return new DependencyInjectionTestClass(ctorCallsCounter, 2); });
	decltype(auto) serviceProvider = collection.BuildServiceProvider();

	// Act
	decltype(auto) instance1 = serviceProvider->Resolve<DependencyInjectionTestClass>();
	decltype(auto) instance2 = serviceProvider->Resolve<DependencyInjectionTestClass>();
	decltype(auto) instance3 = serviceProvider->Resolve<DependencyInjectionTestClass>();

	// Assert
	EXPECT_EQ(ctorCallsCounter, 3);
}

// B ctor A, C
// C -> A
// A - scoped lifetime
TEST(ScopedResolutionTest, ShouldCreate2ScopedInstances) {
	// Arrange
	int ctorCallsCounter = 0;
	ServiceCollection collection;
	collection.AddScoped<DependencyInjectionTestClass>([&](ServiceProvider& sp) { return new DependencyInjectionTestClass(ctorCallsCounter, 2); });
	collection.AddScoped<DependencyInjectionTestClassB>([&](ServiceProvider& sp)
		{
			return new DependencyInjectionTestClassB(sp.Resolve<DependencyInjectionTestClass>(),
													 sp.Resolve<DependencyInjectionTestClassC>());
		});
	collection.AddScoped<DependencyInjectionTestClassC>([&](ServiceProvider& sp)
		{
			return new DependencyInjectionTestClassC(sp.Resolve<DependencyInjectionTestClass>());
		});
	decltype(auto) serviceProvider = collection.BuildServiceProvider();

	// Act
	decltype(auto) instance1 = serviceProvider->Resolve<DependencyInjectionTestClassB>();
	decltype(auto) instance2 = serviceProvider->Resolve<DependencyInjectionTestClassB>();

	// Assert
	EXPECT_EQ(ctorCallsCounter, 2);
}
