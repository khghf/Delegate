#pragma once
#include<iostream>
#include<assert.h>
namespace DM
{
	template<typename Ret, typename...Args>
	class IDelegateInstance
	{
	public:
		~IDelegateInstance() = default;
		virtual Ret Execute(Args...arg) = 0;
		virtual IDelegateInstance<Ret, Args...>* Clone() = 0;
	};
	//普通函数
	template<typename Ret, typename...Args>
	class FunDelegateInst :public IDelegateInstance<Ret, Args...>
	{
		Ret(* Fun)(Args...);
	public:
		FunDelegateInst(Ret(*fun)(Args...)):Fun(fun) {}
		virtual Ret Execute(Args...arg) override
		{
			return Fun(std::forward<Args>(arg)...);
		}
		virtual IDelegateInstance<Ret, Args...>* Clone()
		{
			return new FunDelegateInst(Fun);
		}
	};
	//成员函数
	template<typename Class, typename Ret, typename...Args>
	class MebFunDelegateInst :public IDelegateInstance<Ret, Args...>
	{
		Ret(Class::* Fun)(Args...);
		Class* Obj;
	public:
		MebFunDelegateInst(Class*obj, Ret(Class::*fun)(Args...)) :Obj(obj), Fun(fun) {}
		virtual Ret Execute(Args...arg) override
		{
			if (Obj)
			{
				return (Obj->*Fun)(std::forward<Args>(arg)...);
			}
			else
			{
				assert(false && "Delegate:Bound Object was destroyed");
			}
			return Ret{};
		}
		virtual IDelegateInstance<Ret, Args...>* Clone()
		{
			return new MebFunDelegateInst(Obj, Fun);
		}
	};
	template<typename CallObj, typename Ret, typename...Args>
	class LambdaDelegateInst :public IDelegateInstance<Ret, Args...>
	{
	private:
		CallObj Obj;
	public:
		LambdaDelegateInst(CallObj&& Obj) :Obj(std::forward<CallObj>(Obj)) {}
		virtual Ret Execute(Args...arg) override
		{
			return Obj(std::forward<Args>(arg)...);
		}
		virtual IDelegateInstance<Ret, Args...>* Clone()
		{
			return new LambdaDelegateInst(std::forward<CallObj>(Obj));
		}
	};
}
