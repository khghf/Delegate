#pragma once
#include <typeindex>
namespace DM
{
    template <typename T>
    inline size_t hash_combine(const size_t& seed, const T& val) {
        std::hash<T> hasher;
        size_t res = hasher(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return res;
    }
    template<typename T>
    struct FTypeHash
    {
        size_t operator()()const
        {
            return typeid(T).hash_code();
        }
    };
    template<typename T>
    struct  FFunHash
    {
        size_t operator()(T CallObj)const
        {
            return std::hash<void*>()(reinterpret_cast<void*>(CallObj));
        }
    };
    template<typename T>
    struct FLambdaHash
    {
        size_t operator()(const T& lambda)const
        {
            size_t TypeHash = FTypeHash<T>()();
            size_t Obj_Hash = std::hash<std::uintptr_t>()(reinterpret_cast<std::uintptr_t>(&lambda));
            return hash_combine(TypeHash, Obj_Hash);
        }
    };

    template<typename Class, typename Ret, typename ...Args>
    struct FMebFunHash
    {
        size_t operator()(Ret(Class::* MebFun)(Args...))const
        {
            size_t seed = FTypeHash<Class>()();
            using MebFunType = Ret(Class::*)(Args...);
            const void*funAddress= reinterpret_cast<const void *&>(MebFun);
            hash_combine(seed, funAddress);
            return seed;
        }
        size_t operator()(Ret(Class::* MebFun)(Args...) const) const {
            size_t seed = FTypeHash<Class>()();
            using MebFunType = Ret(Class::*)(Args...) const;
			const void* funAddress = reinterpret_cast<const void*&>(MebFun);
			hash_combine(seed, funAddress);
            return seed;
        }
    };
}

