#pragma once

#include <typeindex>
#include <typeinfo>
#include <any>
#include <functional>
#include <unordered_map>
#include <memory>
#include <utility>
#include <exception>
#include <string>

namespace AspCpp
{
	namespace Extensions
	{
		namespace DependencyInjection
		{
            class ServiceProvider;
            class ServiceCollection;

            template<class TClass>
            using Dependency = std::shared_ptr<TClass>;

            template<class TClass>
            using DependencyConfiguration = std::function<TClass* (ServiceProvider&)>;

            namespace Internal
            {
                enum class ServiceLifetimeType
                {
                    Scoped,
                    Singleton,
                    Trnasient,
                };

                struct ServiceDependency
                {
                    ServiceLifetimeType ServiceLifetimeType;
                    std::any ServiceConfigurationRule;
                };

                class ScopeGuard
                {
                private:
                    std::function<void()> m_cleanup;

                public:
                    ~ScopeGuard() { m_cleanup(); }

                    ScopeGuard(std::function<void()> cleanup)
                        : m_cleanup(cleanup)
                    {}
                };
            }

            class ServiceCollection
            {
            private:
                friend class ServiceProvider;
                std::unordered_map<std::type_index, Internal::ServiceDependency> m_servicies;

            public:
                template<class TClass>
                void AddSingleton(DependencyConfiguration<TClass> conf);

                template<class TClass>
                void AddScoped(DependencyConfiguration<TClass> conf);

                template<class TClass>
                void AddTransient(DependencyConfiguration<TClass> conf);

                std::unique_ptr<ServiceProvider> BuildServiceProvider() const
                {
                    return std::make_unique<ServiceProvider>(this);
                }

            private:
                const Internal::ServiceDependency& GetService(const std::type_index& classId) const
                {
                    if (m_servicies.find(classId) == m_servicies.end())
                    {
                        throw std::exception((std::string("Unable to find dependency: ") + classId.name()).c_str());
                    }

                    return m_servicies.at(classId);
                }
            };

            template<class TClass>
            void ServiceCollection::AddScoped(DependencyConfiguration<TClass> conf)
            {
                m_servicies[typeid(TClass)] = { Internal::ServiceLifetimeType::Scoped, conf };
            }

            template<class TClass>
            void ServiceCollection::AddTransient(DependencyConfiguration<TClass> conf)
            {
                m_servicies[typeid(TClass)] = { Internal::ServiceLifetimeType::Trnasient, conf };
            }

            template<class TClass>
            void ServiceCollection::AddSingleton(DependencyConfiguration<TClass> conf)
            {
                m_servicies[typeid(TClass)] = { Internal::ServiceLifetimeType::Singleton, conf };
            }

            class ServiceProvider
            {
            private:
                const ServiceCollection* m_collection;
                std::unordered_map<std::type_index, std::any> m_singletonValues;

                bool m_isFirstScopedResolved = false;
                std::unordered_map<std::type_index, std::any> m_scopedValues;
            public:
                ServiceProvider(const ServiceCollection* collection) :m_collection(collection)
                {}

                template<class TClass>
                Dependency<TClass> Resolve();

            private:
                template<class TClass>
                Dependency<TClass> ResolveWithScopedBuffering();

                template<class TClass>
                Dependency<TClass> ResolveSingleton(const std::type_index& classId, DependencyConfiguration<TClass> conf);

                template<class TClass>
                Dependency<TClass> ResolveScoped(const std::type_index& classId, DependencyConfiguration<TClass> conf);

                template<class TClass>
                Dependency<TClass> ResolveTransient(DependencyConfiguration<TClass> conf);
            };

            template<class TClass>
            Dependency<TClass> ServiceProvider::ResolveWithScopedBuffering()
            {
                decltype(auto) classId = typeid(TClass);
                const Internal::ServiceDependency& serviceDependency = m_collection->GetService(classId);
                const auto& serviceConfigurationRule = std::any_cast<DependencyConfiguration<TClass>>(serviceDependency.ServiceConfigurationRule);
                switch (serviceDependency.ServiceLifetimeType)
                {
                case Internal::ServiceLifetimeType::Singleton:
                    return ResolveSingleton<TClass>(classId, serviceConfigurationRule);
                case Internal::ServiceLifetimeType::Scoped:
                    return ResolveScoped<TClass>(classId, serviceConfigurationRule);
                case Internal::ServiceLifetimeType::Trnasient:
                    return ResolveTransient<TClass>(serviceConfigurationRule);
                }

                throw std::exception((std::string("Unable to resolve dependency: ") + classId.name()).c_str());
            }

            template<class TClass>
            Dependency<TClass> ServiceProvider::Resolve()
            {
                if (!m_isFirstScopedResolved)
                {
                    m_isFirstScopedResolved = true;

                    Internal::ScopeGuard scopeGuard([&]()
                        {
                            m_scopedValues.clear();
                            m_isFirstScopedResolved = false;
                        });

                    return ResolveWithScopedBuffering<TClass>();
                }
               
                return ResolveWithScopedBuffering<TClass>();
            }

            template<class TClass>
            Dependency<TClass> ServiceProvider::ResolveSingleton(const std::type_index& classId, DependencyConfiguration<TClass> conf)
            {
                decltype(auto) searchIter = m_singletonValues.find(classId);
                if (searchIter == m_singletonValues.end())
                {
                    decltype(auto) resolvedValue = Dependency<TClass>(conf(*this));
                    m_singletonValues[classId] = resolvedValue;
                    return resolvedValue;
                }

                return std::any_cast<Dependency<TClass>>(searchIter->second);
            }

            template<class TClass>
            Dependency<TClass> ServiceProvider::ResolveTransient(DependencyConfiguration<TClass> conf)
            {
                decltype(auto) resolvedValue = conf(*this);
                return Dependency<TClass>(resolvedValue);
            }

            template<class TClass>
            Dependency<TClass> ServiceProvider::ResolveScoped(const std::type_index& classId, DependencyConfiguration<TClass> conf)
            {
                auto searchIter = m_scopedValues.find(classId);
                if (searchIter == m_scopedValues.end())
                {
                    decltype(auto) resolvedValue = Dependency<TClass>(conf(*this));
                    m_scopedValues[classId] = resolvedValue;
                    return resolvedValue;
                }

                return std::any_cast<Dependency<TClass>>(searchIter->second);
            }
		}
	}
}