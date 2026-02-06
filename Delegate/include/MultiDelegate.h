#pragma once
#include"BaseDelegate.h"
#include"DelegateHash.h"
#include<unordered_map>
namespace DM
{
	template<typename FunType, typename T = void>
	class MultiDelegate;
	template<typename Ret, typename...Args, typename T>
	class MultiDelegate<Ret(Args...), T>
	{
	private:
		using FunType = Ret(*)(Args...);
		std::unordered_map<size_t, BaseDelegate<Ret(Args...)>>Delegates;
	public:
		MultiDelegate() = default;
		//普通函数
		void Add(FunType Fun)
		{
			BaseDelegate<Ret(Args...)>BD;
			BD.Bind(std::forward<FunType>(Fun));
			if (Delegates.find(BD.Key()) != Delegates.end())
			{
				std::cout << "MultiDelegate:repeat add" << std::endl;
				return;
			}
			Delegates[BD.Key()] = std::move(BD);
		}
		//成员函数
		template<typename Class>
		void Add(Class*Obj, Ret(Class::* MebFunType)(Args...))
		{
			BaseDelegate<Ret(Args...)>BD;
			BD.Bind(Obj, std::forward<Ret(Class::*)(Args...)>(MebFunType));
			if (Delegates.find(BD.Key()) != Delegates.end())
			{
				std::cout << "MultiDelegate:repeat add" << std::endl;
				return;
			}
			Delegates[BD.Key()] = std::move(BD);
		}
		//Lambda 表达式
		template<typename CallObj>
		void Add(CallObj&& Obj)
		{
			BaseDelegate<Ret(Args...)>BD;
			BD.Bind(std::forward<CallObj>(Obj));
			if (Delegates.find(BD.Key()) != Delegates.end())
			{
				std::cout << "MultiDelegate:repeat add" << std::endl;
				return;
			}
			Delegates[BD.Key()] = std::move(BD);
		}
		void Add(BaseDelegate<Ret(Args...)>&&BD)
		{
			if (Delegates.find(BD.Key()) != Delegates.end())
			{
				LOG_CORE_ERROR("MultiDelegate:repeat add. HashKey:{}",BD.Key());
				return;
			}
			Delegates[BD.Key()] = std::move(BD);
		}
		void Remove(BaseDelegate<Ret(Args...)>BD)
		{
			Delegates.erase(BD.Key());
		}
		void BroadCast(Args...arg)
		{
			for (auto it = Delegates.begin(); it != Delegates.end();)
			{
				const auto& delegate = it->second;
				if (delegate.IsValid())
				{
					delegate.Execute(std::forward<Args>(arg)...);
					++it; 
				}
				else
				{
					it = Delegates.erase(it);
					std::cout <<":found and removed invalid delegate" << std::endl;
				}
			}
		}
		int Count()const
		{
			return Delegates.size();
		}
	};
}
