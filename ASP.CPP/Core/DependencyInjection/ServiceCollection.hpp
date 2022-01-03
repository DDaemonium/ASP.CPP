#pragma once

#include <typeindex>
#include <typeinfo>
#include <any>
#include <functional>
#include <unordered_map>
#include <memory>
#include <utility>

namespace AspCpp
{
	namespace Extensions
	{
		namespace DependencyInjection
		{
            template<class TClass>
            using Singleton = std::shared_ptr<TClass>;

            template<class TClass>
            using Scoped = std::shared_ptr<TClass>;

            template<class TClass>
            using Transient = std::shared_ptr<TClass>;

            class ServiceProvider;
            class ServiceCollection;

            template<class TClass>
            using SingletonConfiguration = std::function<TClass*(ServiceProvider&)>;

            template<class TClass>
            using ScopedConfiguration = std::function<TClass*(ServiceProvider&)>;

            template<class TClass>
            using TransientConfiguration = std::function<TClass*(ServiceProvider&)>;

            class ServiceCollection
            {
            private:
                std::unordered_map<std::type_index, std::any> m_singletons;
                std::unordered_map<std::type_index, std::any> m_scopeds;
                std::unordered_map<std::type_index, std::any> m_transients;

            public:
                template<class TClass>
                void AddSingleton(SingletonConfiguration<TClass> conf);

                template<class TClass>
                void AddScoped(ScopedConfiguration<TClass> conf);

                template<class TClass>
                void AddTransient(TransientConfiguration<TClass> conf);

                std::unique_ptr<ServiceProvider> BuildServiceProvider() const
                {
                    return std::make_unique<ServiceProvider>(this);
                }

                const std::unordered_map<std::type_index, std::any>& GetSingletons() const
                {
                    return m_singletons;
                }

                const std::unordered_map<std::type_index, std::any>& GetScopeds() const
                {
                    return m_scopeds;
                }

                const std::unordered_map<std::type_index, std::any>& GetTransients() const
                {
                    return m_transients;
                }

            };

            template<class TClass>
            void ServiceCollection::AddScoped(ScopedConfiguration<TClass> conf)
            {
                m_scopeds[typeid(TClass)] = conf;
            }

            template<class TClass>
            void ServiceCollection::AddTransient(TransientConfiguration<TClass> conf)
            {
                m_transients[typeid(TClass)] = conf;
            }

            template<class TClass>
            void ServiceCollection::AddSingleton(SingletonConfiguration<TClass> conf)
            {
                m_singletons[typeid(TClass)] = conf;
            }

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
                Singleton<TClass> ResolveSingleton();

                template<class TClass>
                Scoped<TClass> ResolveScoped();

                template<class TClass>
                Transient<TClass> ResolveTransient();

            private:
                template<class TClass>
                Scoped<TClass> ResolveScopedWithBuffering();
            };

            template<class TClass>
            Singleton<TClass> ServiceProvider::ResolveSingleton()
            {
                decltype(auto) classId = typeid(TClass);
                auto searchIter = m_singletonValues.find(classId);
                if (searchIter == m_singletonValues.end())
                {
                    decltype(auto) resolveConfigurationAny = m_collection->GetSingletons().at(classId);
                    auto resolveConfiguration = std::any_cast<SingletonConfiguration<TClass>>(resolveConfigurationAny);
                    auto resolvedValue = Singleton<TClass>(resolveConfiguration(*this));
                    m_singletonValues[classId] = resolvedValue;
                    return resolvedValue;
                }

                return std::any_cast<Singleton<TClass>>(searchIter->second);
            }

            template<class TClass>
            Transient<TClass> ServiceProvider::ResolveTransient()
            {
                decltype(auto) classId = typeid(TClass);
                decltype(auto) resolveConfigurationAny = m_collection->GetTransients().at(classId);
                auto resolveConfiguration = std::any_cast<TransientConfiguration<TClass>>(resolveConfigurationAny);
                auto resolvedValue = resolveConfiguration(*this);
                return Transient<TClass>(resolvedValue);
            }

            template<class TClass>
            Scoped<TClass> ServiceProvider::ResolveScoped()
            {
                if (!m_isFirstScopedResolved)
                {
                    m_isFirstScopedResolved = true;

                    ScopeGuard([&]()
                        {
                            m_scopedValues.clear();
                            m_isFirstScopedResolved = false;
                        });

                    return ResolveScopedWithBuffering<TClass>();
                }

                return ResolveScopedWithBuffering<TClass>();
            }

            template<class TClass>
            Scoped<TClass> ServiceProvider::ResolveScopedWithBuffering()
            {
                decltype(auto) classId = typeid(TClass);
                auto searchIter = m_scopedValues.find(classId);
                if (searchIter == m_scopedValues.end())
                {
                    decltype(auto) resolveConfigurationAny = m_collection->GetScopeds().at(classId);
                    auto resolveConfiguration = std::any_cast<ScopedConfiguration<TClass>>(resolveConfigurationAny);
                    auto resolvedValue = Scoped<TClass>(resolveConfiguration(*this));
                    m_scopedValues[classId] = resolvedValue;
                    return resolvedValue;
                }

                return std::any_cast<Scoped<TClass>>(searchIter->second);
            }
		}
	}
}