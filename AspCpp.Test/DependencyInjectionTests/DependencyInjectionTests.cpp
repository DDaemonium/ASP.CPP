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
	decltype(auto) instance1 = serviceProvider->ResolveSingleton<DependencyInjectionTestClass>();
	decltype(auto) instance2 = serviceProvider->ResolveSingleton<DependencyInjectionTestClass>();
	decltype(auto) instance3 = serviceProvider->ResolveSingleton<DependencyInjectionTestClass>();

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
	decltype(auto) instance1 = serviceProvider->ResolveTransient<DependencyInjectionTestClass>();
	decltype(auto) instance2 = serviceProvider->ResolveTransient<DependencyInjectionTestClass>();
	decltype(auto) instance3 = serviceProvider->ResolveTransient<DependencyInjectionTestClass>();

	// Assert
	EXPECT_EQ(ctorCallsCounter, 3);
}
