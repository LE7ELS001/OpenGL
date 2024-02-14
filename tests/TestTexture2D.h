#pragma once

#include "Test.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"
#include "Vendor/STB/stb_image.h"
#include "Vendor/imgui/imgui.h"


#include <memory>


namespace test {
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		 void OnUpdate(float deltaTime) override;
		 void OnRender() override;
		 void OnImGuiRender() override;

	private:
		float m_ClearColor[4];

		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexbBuffer;
		std::unique_ptr<VertexArray> m_VertexArray;
		std::unique_ptr<IndexBuffer> m_IndiceBuffer;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_proj, m_view;

		glm::vec3 m_translationA;
		glm::vec3 m_translationB;
		
	};
}
