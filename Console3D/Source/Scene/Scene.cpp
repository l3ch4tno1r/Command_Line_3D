#include "Scene.h"
#include "Entity.h"
#include "StdComponent.h"

#include "Console3D/Source/Core/Console.h"

namespace LCN
{
    Entity LCN::Scene::CreateEntity()
    {
        Entity result = { m_Registry.create(), this };

        result.Add<Transform2DComponent>();

        return result;
    }

    void Scene::Render(Entity camera)
    {
        Console& console = Console::Get();

        console.Clear();

        Camera2DComponent&    camCmp       = m_Registry.get<Camera2DComponent>(camera);
        Transform2DComponent& camTransform = m_Registry.get<Transform2DComponent>(camera);

        auto view = m_Registry.view<Transform2DComponent, Sprite2DComponent>();

        const Transform2Df& pix2cam = camCmp.CamTransform;

        Transform2Df cam2R0 = camTransform.Transform.QuickInverse();

        auto pix2R0 = pix2cam * cam2R0;

        view.each([&](Transform2DComponent& R02sprite, Sprite2DComponent& sprite)
            {
                Transform2Df pix2sprite = pix2R0 * R02sprite.Transform;

                for (const Sprite2D::Triangle& face : sprite.Sprite.Faces)
                {
                    HVector2Df v1 = pix2sprite * sprite.Sprite.Vertices[face.v1];
                    HVector2Df v2 = pix2sprite * sprite.Sprite.Vertices[face.v2];
                    HVector2Df v3 = pix2sprite * sprite.Sprite.Vertices[face.v3];

                    console.DrawLine(
                        v1.x(), v1.y(),
                        v2.x(), v2.y());

                    console.DrawLine(
                        v2.x(), v2.y(),
                        v3.x(), v3.y());

                    console.DrawLine(
                        v1.x(), v1.y(),
                        v3.x(), v3.y());
                }
            });

        console.Render();
    }
}
