#include "GameObject.h"

GameObject::GameObject() 
	: Position(0.0f,0.0f), Size(1.0f,1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), IsSolid(false), Destroyed(false)
{
	
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
	: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), IsSolid(false), Destroyed(false)
{
	
}

GameObject::GameObject(BrickTexture sprtie, glm::vec2 pos, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
	:Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), IsSolid(false), Destroyed(false), M_texture(sprtie)
{
	
}

//��������ʹ��unique_ptr��draw Ŀǰ�����ò��� 
void GameObject::Draw(const std::unique_ptr<SpriteRender>& renderer,const std::unique_ptr<Shader> &shader ,const std::unique_ptr<Texture> &texture)
{
	renderer->DrawSprite(shader,texture, this->Position, this->Size, this->Rotation, this->Color);
}

//����ש���draw���⽨��һ������ ���ų�����������drawҲ������� 
void GameObject::DrawBricks(const std::unique_ptr<SpriteRender>& renderer, const std::unique_ptr<Shader>& shader)
{
	renderer->DrawSpriteBrick(shader,this->M_texture , this->Position, this->Size, this->Rotation, this->Color);
}






