#include "tests/Test.h"
#include "Vendor/imgui/imgui.h"

namespace test {


	TestMenu::TestMenu(Test*& CurrentTestPointer) : m_CurrentTest(CurrentTestPointer)
	{

	}

	void TestMenu::OnImGuiRender() //Imgui的功能表
	{
		for (auto& test : m_Test)
		{
			if (ImGui::Button(test.first.c_str()))     //生成一个按钮　按钮名称是test名称　first指的是pair的第一个量　也就是一个std::string
				m_CurrentTest = test.second(); //m_CurrentTest被设置为构建测试类的实例 
		}
	}

}




