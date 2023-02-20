#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/tema2_camera.h"
#include <queue>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderScene(float deltaTimeSeconds);
        void RenderMiniMap(float deltaTimeSeconds);
        void RenderObjects(float deltaTimeSeconds);

        std::queue<VertexFormat> GenerateCarPath(float dist);

        void generateRoadIndices(std::vector<VertexFormat> &vertices, std::vector<VertexFormat> &interiorVertices, std::vector<VertexFormat> &exteriorVertices, std::vector<VertexFormat> &allVertices,
            std::vector<unsigned int> &indices, float distInt, float distExt, bool space);
        void addAllPoints(glm::vec3 A, glm::vec3 B, std::vector<VertexFormat>* vertices);
        float triangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C);
        bool insideTriangle(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C);
        bool checkRoadCollision(glm::vec3 P);
        bool checkCarsCollision(glm::vec3 P);

    protected:
        implemented::CameraT* camera;
        glm::mat4 projectionMatrix;

        std::vector<VertexFormat> vertices;
        std::vector<VertexFormat> interiorVertices;
        std::vector<VertexFormat> exteriorVertices;
        std::vector<VertexFormat> allVertices;
        std::vector<unsigned int> indices;

        std::vector<VertexFormat> interiorVerticesB;
        std::vector<VertexFormat> exteriorVerticesB;
        std::vector<VertexFormat> allVerticesB;
        std::vector<unsigned int> indicesB;

        std::vector<std::queue<VertexFormat>> carsPaths;
        std::vector<float> carsRotation;
        std::vector<float> carsTimer;
        std::vector<glm::vec3> carsColor;
        std::vector<glm::vec3> carsStep;
        std::vector<VertexFormat> carsPosition;

        ViewportArea miniViewportArea;

        float fov, aspect, zNear, zFar;
        float left, right, bottom, top;

        std::string crownShape;
        float rotation;
    };
}   // namespace m1
