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

		virtual void OnUpdate(float deltaTime) {} //����֮���Բ�д�� virtual void OnUpdate(float deltaTime)  =0 ����Ϊ�ҿ���ѡ���ĸ�����override ������ȫ������override���ܼ���
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


	private:																 //������Ҫһ��test���ܵ����� ����Ҫһ��test�����ָ�� ���ǲ�����Ԥ�ȴ���һ������ Ȼ�󼤻�ʱָ�� ���Ǽ���ʱ�Ŵ������� �õ�lamda
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Test; //�����std::function<test*()>����Ҫ���ղ��� ����һ��new Test()
		Test*& m_CurrentTest;

	};
}
