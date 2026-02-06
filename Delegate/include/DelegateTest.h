#pragma once
#include"BaseDelegate.h"
#include"MultiDelegate.h"
#include"DelegateHash.h"
using namespace DM;
void TestFun()
{
	std::cout << "TestFun()" << std::endl;
}
static int TestStaticFun(std::string a)
{
	std::cout << "TestStaticFun():"<<a << std::endl;
	return 2;
}
void BindFun_Test()
{
	BaseDelegate<void()>Test;
	Test.Bind(TestFun);
	Test.Execute();
}
void BindStaticFun_Test()
{
	BaseDelegate<int(std::string)>Test;
	Test.Bind(TestStaticFun);
	Test.Execute("2222222222");
}
class MebFunTest
{
public:
	MebFunTest() = default;
	int a = 0;
	float TestMebFun(float val)
	{
		std::cout << "Member fun Test,Param:"<< this->a << std::endl;
		this->a;
		return val;
	}
	void MultiDelegateTest()
	{
		std::cout << "MultiDelegate MemeberFun Test" << std::endl;
	}
	static void StaticFunction()
	{
		std::cout << "StaticFunction" << std::endl;
	}
};
void BindMebFun_Test()
{
	BaseDelegate<float(float)>test;
	{
		MebFunTest*a=new MebFunTest();
		a->a = 4;
		test.Bind(a, &MebFunTest::TestMebFun);
		test.Execute(1.f);
	}
}
void BindStaticMebFun_Test()
{
	BaseDelegate<void(void)>Test;
	MebFunTest* a = new MebFunTest();
	Test.Bind(&MebFunTest::StaticFunction);
	Test.Execute();
}

void BindLamFun_Test()
{
	BaseDelegate<float(char ch)>test;
	char ch = 'a';
	{
		const auto lam = [&](char c)->float {
			ch = 'b';
			std::cout << c << std::endl;
			return 1;
		};
		test.Bind(lam);
	}
	std::cout << test.Execute('c') << std::endl;
	std::cout << ch << std::endl;
}

void MultiDelegateFun_Test1()
{
	std::cout << "MultiDelegateFun_Test1" << std::endl;
}
void MultiDelegateFun_Test2()
{
	std::cout << "MultiDelegateFun_Test2" << std::endl;
}
static void MultiDelegateFun_Test3()
{
	std::cout << "Static Fun MultiDelegateFun_Test3" << std::endl;
}
void MultiDelegate_Test()
{
	MultiDelegate<void(void)>md;
	md.Add(MultiDelegateFun_Test1);
	md.Add(MultiDelegateFun_Test1);
	md.Add(MultiDelegateFun_Test2);
	md.Add(MultiDelegateFun_Test3);
	auto a = []()
	{
		std::cout << "Lambda 表达式" << std::endl;
	};
	md.Add(a);
	md.Add(a);
	MebFunTest m;
	md.Add(&m, &MebFunTest::MultiDelegateTest);
	md.BroadCast();
}
void DelegateTest()
{
	std::cout << "-----------------Function---------------------" << std::endl;
	BindFun_Test();
	std::cout << "-----------------StaticFunction---------------------" << std::endl;
	BindStaticFun_Test();
	std::cout << "-----------------MemberFunction---------------------" << std::endl;
	BindMebFun_Test();
	std::cout << "-----------------StaticMemberFunction---------------------" << std::endl;
	BindStaticMebFun_Test();
	std::cout << "-----------------Lambda---------------------" << std::endl;
	BindLamFun_Test();
	std::cout << "-----------------MultiDelegate---------------------" << std::endl;
	MultiDelegate_Test();

}
