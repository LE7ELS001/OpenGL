#include<iostream>;
#include<fstream>
#include<string>
#include<sstream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>;

#include"Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "mesh.h"
#include "Model.h"
#include "TestClass/ShaderTwo.h"

#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/Test.h"
#include "tests/TestBatchRender.h"

#include "tests/TestCamera.h"
#include "tests/TestLight.h"
#include "tests/LightCaster.h"
#include "tests/TestMultiLight.h"
#include "tests/MeshTest.h"
#include "tests/TestDepth.h"
#include "tests/StencilTest.h"
#include "tests/AlphaTest.h"
#include "tests/FrameBufferTest.h"
#include "tests/SkyboxTest.h"
#include "tests/GeometryTest1.h"
#include "tests/GeometryTestTwo.h"
#include "tests/DrawNormal.h"
#include "tests/InstanceTest.h"
#include "tests/MSAATest.h"
#include "tests/PhongTest.h"
#include"tests/GammaTest.h"
#include "tests/ShadowDepth.h"
#include "tests/ShadowTest2.h"
#include "tests/Point_Shadow_Depth.h"
#include "tests/NormalMapping.h"
#include "tests/steep_parallax_mapping.h"
#include "tests/HDR_Test.h"
#include "tests/BloomTest.h"
#include "tests/Deferred_Shading.h"
#include "tests/SSAO_Test.h"
#include "tests/PBR_Test.h"
#include "tests/PBR_Texture.h"
#include "tests/IBL.h"
#include "tests/Text_test.h"
#include "tests/Game_Test.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <ft2build.h>
#include FT_FREETYPE_H
//旧的OPENGL生成三角形方式
        /*
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();
        */



int main()
{

    GLFWwindow* window;

    //初始化GLFW
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_SAMPLES, 4);

    

    //建立窗口和联系上下文
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    
   

    //创建上下文
    glfwMakeContextCurrent(window);

    //开启垂直同步
    glfwSwapInterval(1);


    //GLEWinit要在上下文创建后才建立
    if (glewInit() != GLEW_OK) 
    {
        std::cout << "Error" << std::endl;
    }

   


    //打印一下OPENGL版本号
    std::cout << glGetString(GL_VERSION) << std::endl;

    {

        //生成渲染器
        Renderer renderer;



        //创建imgui上下文
        ImGui::CreateContext();
        //初始化
        ImGui_ImplGlfwGL3_Init(window, true);
        //设置style
        ImGui::StyleColorsDark();

        test::Test* currentTest = nullptr;
        test::TestMenu* menu = new test::TestMenu(currentTest); //m_currenTest = currentTest
        currentTest = menu;

        
       // menu->RegisterTest<test::TestClearColor>("Clear Color");
       // menu->RegisterTest<test::TestTexture2D>("2d Texture");
       // menu->RegisterTest<test::TestBatchRender>("BatchRender");
       // menu->RegisterTest<test::Camera>("Camera",window);
       // menu->RegisterTest<test::Light>("Light", window);
       // menu->RegisterTest<test::LightCast>("LightCast", window);
       // menu->RegisterTest<test::MultiLight>("MultipleLight", window);
        //menu->RegisterTest<test::MeshTest>("Model", window);
       // menu->RegisterTest<test::TestDepth>("Depth", window);
       // menu->RegisterTest<test::StencilTest>("StencilTest", window);
       // menu->RegisterTest<test::AlphaTest>("AlphaTest", window);
       //menu->RegisterTest<test::FrameBufferTest>("FrameBuffer", window);
       // menu->RegisterTest<test::SkyboxTest>("SkyBoxTest", window);
       //menu->RegisterTest<test::GeometryTestOne>("GeometryTest", window);
       //menu->RegisterTest<test::GeometryTestTwo>("GeometryTest2", window);
       // menu->RegisterTest<test::DrawNormal>("DrawNormal", window);
       //menu->RegisterTest<test::InstanceTest>("Instance", window);
        //menu->RegisterTest<test::MASSTest>("MASS", window);
        //menu->RegisterTest<test::PhongTest>("Phong", window);
        //menu->RegisterTest<test::GammaTest>("gammaTest", window);
        //menu->RegisterTest<test::ShadowDepth>("Shadow", window);
        //menu->RegisterTest < test::ShadowTest2>("Shadow", window);
        //menu->RegisterTest<test::PointShadowDepth>("point_Shadow_depth", window);
        //menu->RegisterTest<test::NormalMapping>("normalMapping", window);
        //menu->RegisterTest<test::SteepParallaxMapping>("SteepParallaxMapping", window);
        //menu->RegisterTest<test::HDR_Test>("HDR", window);
        //menu->RegisterTest<test::BloomTest>("Bloom", window);
        //menu->RegisterTest<test::DeferredShading>("DeferredShading", window);
        //menu->RegisterTest<test::SSAO>("SSAO", window);
        //menu->RegisterTest<test::PBR>("PBR", window);
        //menu->RegisterTest<test::PBR_Texture>("PBR_Texture", window);
        //menu->RegisterTest<test::IBL>("IBL", window);
        //menu->RegisterTest<test::Text_test>("TEXT", window);
        menu->RegisterTest<test::Game_test>("Breakout", window);

        //循环
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            
            ImGui_ImplGlfwGL3_NewFrame();
            if (currentTest != nullptr)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRender();
                ImGui::Begin("Test");
                if (currentTest != menu && ImGui::Button("-<"))
                {
                    delete currentTest;
                    currentTest = menu;
                }
                currentTest->OnImGuiRender();
                ImGui::End();
                
            }
            

           
            
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


            
            glfwSwapBuffers(window);

            
            glfwPollEvents();
        }

    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
	
}