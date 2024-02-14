#include "tests/Test.h"
#include "Vendor/imgui/imgui.h"

namespace test {


	TestMenu::TestMenu(Test*& CurrentTestPointer) : m_CurrentTest(CurrentTestPointer)
	{

	}

	void TestMenu::OnImGuiRender() //Imgui�Ĺ��ܱ�
	{
		for (auto& test : m_Test)
		{
			if (ImGui::Button(test.first.c_str()))     //����һ����ť����ť������test���ơ�firstָ����pair�ĵ�һ������Ҳ����һ��std::string
				m_CurrentTest = test.second(); //m_CurrentTest������Ϊ�����������ʵ�� 
		}
	}

}




