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

        const Transform2Df& pix2cam = camCmp.PixToCam;

        Transform2Df cam2R0 = camTransform.Transform.QuickInverse();

        auto pix2R0 = pix2cam * cam2R0;

        view.each([&](const Transform2DComponent& R02sprite, const Sprite2DComponent& sprite)
            {
                Transform2Df pix2sprite = pix2R0 * R02sprite.Transform;

                for (const Sprite2D::Triangle& face : sprite.Sprite.Faces)
                {
                    HVector2Df v1 = pix2sprite * sprite.Sprite.Vertices[face.v1];
                    HVector2Df v2 = pix2sprite * sprite.Sprite.Vertices[face.v2];
                    HVector2Df v3 = pix2sprite * sprite.Sprite.Vertices[face.v3];

                    console.DrawLine(
                        (int)v1.x(), (int)v1.y(),
                        (int)v2.x(), (int)v2.y());

                    console.DrawLine(
                        (int)v2.x(), (int)v2.y(),
                        (int)v3.x(), (int)v3.y());

                    console.DrawLine(
                        (int)v1.x(), (int)v1.y(),
                        (int)v3.x(), (int)v3.y());
                }
            });

        // Draw crosshair
        console.DrawLine(
            console.Width() / 2 - 5, console.Height() / 2,
            console.Width() / 2 + 5, console.Height() / 2,
            0, COLOUR::BG_RED
        );

        console.DrawLine(
            console.Width() / 2, console.Height() / 2 - 5,
            console.Width() / 2, console.Height() / 2 + 5,
            0, COLOUR::BG_RED
        );

        console.Render();
    }
}
