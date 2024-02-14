#pragma once
#include <functional>
#include <vector>
#include <string>
#include <GL/glew.h>
#include "GLFW/glfw3.h"


namespace test {
	
	class Test
	{
	public:
		Test() {}
		virtual ~Test(){}

		virtual void OnUpdate(float deltaTime) {} //这里之所以不写成 virtual void OnUpdate(float deltaTime)  =0 是因为我可以选择哪个函数override 而不是全部都得override才能继续
		virtual void OnRender() {}                
		virtual void OnImGuiRender() {}


		
	};

	class TestMenu : public Test
	{
	public:
			TestMenu(Test*& CurrentTestPointer);

			void OnImGuiRender() override;


			template<typename T>
			void RegisterTest(const std::string& name)
			{

						m_Test.push_back(std::make_pair(name, []() {return new T(); }));
	
			}

			template<typename T>
			void RegisterTest(const std::string& name, GLFWwindow* window)
			{
				m_Test.push_back(std::make_pair(name, [window]() { return new T(window); }));
			}


	private:																 //这里需要一个test功能的名称 并且要一个test对象的指针 但是不像他预先创建一个对象 然后激活时指向 而是激活时才创建对象 用到lamda
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Test; //这里的std::function<test*()>不需要接收参数 返回一个new Test()
		Test*& m_CurrentTest;

	};
}
