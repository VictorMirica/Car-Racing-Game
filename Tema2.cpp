#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new implemented::CameraT();
    camera->Set(glm::vec3(4.8, 0.23, 0.3), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    camera->RotateFirstPerson_OY(RADIANS(300));
    camera->RotateFirstPerson_OX(RADIANS(-30));

    // Ground
    {
        Mesh* mesh = new Mesh("plane50");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Cars
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Sphere (for trees)
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Road
    {
        {
            // glm::vec3(4.8, 0, 0.3) // A
            addAllPoints(glm::vec3(4.8, 0, 0.3), glm::vec3(4.2, 0, 1.4), &vertices);
            // glm::vec3(4.2, 0, 1.4) // B
            addAllPoints(glm::vec3(4.2, 0, 1.4), glm::vec3(2.8, 0, 2.14), &vertices);
            // glm::vec3(2.8, 0, 2.14) // C
            addAllPoints(glm::vec3(2.8, 0, 2.14), glm::vec3(1.5, 0, 1.95), &vertices);
            // glm::vec3(1.5, 0, 1.95) // D
            addAllPoints(glm::vec3(1.5, 0, 1.95), glm::vec3(0.6, 0, 1.25), &vertices);
            // glm::vec3(0.6, 0, 1.25) // E
            addAllPoints(glm::vec3(0.6, 0, 1.25), glm::vec3(-0.5, 0, 0.6), &vertices);
            // glm::vec3(-0.5, 0, 0.6) // F
            addAllPoints(glm::vec3(-0.5, 0, 0.6), glm::vec3(-1.8, 0, 1.15), &vertices);
            // glm::vec3(-1.8, 0, 1.15) // G
            addAllPoints(glm::vec3(-1.8, 0, 1.15), glm::vec3(-2.83, 0, 1.43), &vertices);
            // glm::vec3(-2.83, 0, 1.43) // H
            addAllPoints(glm::vec3(-2.83, 0, 1.43), glm::vec3(-3.8, 0, 0.8), &vertices);
            // glm::vec3(-3.8, 0, 0.8) // I
            addAllPoints(glm::vec3(-3.8, 0, 0.8), glm::vec3(-3.95, 0, -0.2), &vertices);
            // glm::vec3(-3.95, 0, -0.2) // J
            addAllPoints(glm::vec3(-3.95, 0, -0.2), glm::vec3(-3.45, 0, -1.05), &vertices);
            // glm::vec3(-3.45, 0, -1.05) // K
            addAllPoints(glm::vec3(-3.45, 0, -1.05), glm::vec3(-2.11, 0, -1.55), &vertices);
            // glm::vec3(-2.11, 0, -1.55) // L
            addAllPoints(glm::vec3(-2.11, 0, -1.55), glm::vec3(-0.68, 0, -1.62), &vertices);
            // glm::vec3(-0.68, 0, -1.62) // M
            addAllPoints(glm::vec3(-0.68, 0, -1.62), glm::vec3(0.5, 0, -0.9), &vertices);
            // glm::vec3(0.5, 0, -0.9) // N
            addAllPoints(glm::vec3(0.5, 0, -0.9), glm::vec3(1.45, 0, -0.24), &vertices);
            // glm::vec3(1.45, 0, -0.24) // O
            addAllPoints(glm::vec3(1.45, 0, -0.24), glm::vec3(2.66, 0, -0.75), &vertices);
            // glm::vec3(2.66, 0, -0.75) // P
            addAllPoints(glm::vec3(2.66, 0, -0.75), glm::vec3(3.45, 0, -1.45), &vertices);
            // glm::vec3(3.45, 0, -1.45) // Q
            addAllPoints(glm::vec3(3.45, 0, -1.45), glm::vec3(4.6, 0, -1.2), &vertices);
            // glm::vec3(4.6, 0, -1.2) // R
            addAllPoints(glm::vec3(4.6, 0, -1.2), glm::vec3(4.8, 0, 0.3), &vertices);
        }

        generateRoadIndices(vertices, interiorVertices, exteriorVertices, allVertices, indices, 40.f, 40.f, false);
  
        meshes["road"] = new Mesh("generated road");
        meshes["road"]->InitFromData(allVertices, indices);
    }

    // Road Line
    {
        generateRoadIndices(vertices, interiorVerticesB, exteriorVerticesB, allVerticesB, indicesB, 1.f, 1.f, true);

        meshes["roadLine"] = new Mesh("generated road line");
        meshes["roadLine"]->InitFromData(allVerticesB, indicesB);
    }

    // Generate obstacle cars paths and starting positions
    // 5 obstacole in total
    // Punctele care formeaza drumul sunt, fata de coordonatele de centru,
    // la o distanta de -40 respectiv 40.
    // Voi avea 5 masini de la -30 pana la 30, cu increment 15
    {
        srand((unsigned)time(NULL)); // Random car color
        for (float i = -30; i <= 30; i += 15) {
            carsPaths.push_back(GenerateCarPath(i));
            carsRotation.push_back(0);
            carsTimer.push_back(0);
            carsColor.push_back(glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX));
            carsStep.push_back(glm::vec3(0, 0, 0));
            carsPosition.push_back(VertexFormat(glm::vec3(0, 0, 0)));
        }
    }

    // Shader
    {
        Shader* shader = new Shader("ShaderTema");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    fov = RADIANS(60);
    aspect = window->props.aspectRatio;
    zNear = 0.01f;
    zFar = 200.0f;

    left = -2;
    right = 2;
    bottom = -2;
    top = 2;

    rotation = RADIANS(55);
    crownShape = "box";

    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);

    // Sets the resolution of the small viewport
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(20, 20, resolution.x / 3.f, resolution.y / 3.f);
}


// Generate the main, exterior and interior points for the road and road line
// After that, generate the indices
void Tema2::generateRoadIndices(std::vector<VertexFormat> &vertices, std::vector<VertexFormat> &interiorVertices, std::vector<VertexFormat> &exteriorVertices, std::vector<VertexFormat> &allVertices,
    std::vector<unsigned int> &indices, float distInt, float distExt, bool space)
{
    for (int i = 0; i < vertices.size() - 1; i++) {

        glm::vec3 D = vertices[i + 1].position - vertices[i].position;
        glm::vec3 P = cross(D, glm::vec3(0, 1, 0));
        interiorVertices.push_back(VertexFormat(vertices[i].position + distInt * P));
        exteriorVertices.push_back(VertexFormat(vertices[i].position - distExt * P));
    }
    glm::vec3 D = vertices[0].position - vertices[vertices.size() - 1].position;
    glm::vec3 P = cross(D, glm::vec3(0, 1, 0));
    interiorVertices.push_back(VertexFormat(vertices[vertices.size() - 1].position + distInt * P));
    exteriorVertices.push_back(VertexFormat(vertices[vertices.size() - 1].position - distExt * P));

    allVertices = exteriorVertices;
    allVertices.insert(allVertices.end(), interiorVertices.begin(), interiorVertices.end());


    int interiorStart = exteriorVertices.size();
    for (int i = 0; i < (allVertices.size() / 2) - 1; i++) {
        indices.push_back(i);
        indices.push_back(interiorStart + i);
        indices.push_back(interiorStart + i + 1);

        indices.push_back(i);
        indices.push_back(interiorStart + i + 1);
        indices.push_back(i + 1);
        
        // For the road line, add spaces between lines
        if (space && i % 10 == 0) {
            i+=5;
        }
    }

    indices.push_back(interiorStart - 1);
    indices.push_back(allVertices.size() - 1);
    indices.push_back(interiorStart);

    indices.push_back(interiorStart - 1);
    indices.push_back(interiorStart);
    indices.push_back(0);
}

// Function used to calculate and add more points between two points A and B.
// Used for higher point/triangle density (100 new points between A and B)
void Tema2::addAllPoints(glm::vec3 A, glm::vec3 B, vector<VertexFormat>* vertices) 
{
    glm::vec3 step = (B - A) / 100.f;

    for (int i = 0; i < 100; i++) {
        (*vertices).push_back(VertexFormat(A));
        A += step;
    }
}

// Used to generate a car's path points
queue<VertexFormat> Tema2::GenerateCarPath(float dist) {

    queue<VertexFormat> ret;

    for (int i = 0; i < vertices.size() - 1; i++) {
        glm::vec3 D = vertices[i + 1].position - vertices[i].position;
        glm::vec3 P = cross(D, glm::vec3(0, 1, 0));
        ret.push(VertexFormat(vertices[i].position + dist * P));
    }
    glm::vec3 D = vertices[0].position - vertices[vertices.size() - 1].position;
    glm::vec3 P = cross(D, glm::vec3(0, 1, 0));
    ret.push(VertexFormat(vertices[vertices.size() - 1].position + dist * P));

    return ret;
}

// Calculate a triangle's area
float Tema2::triangleArea(glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    glm::vec3 AB = glm::vec3(B.x - A.x, 0, B.z - A.z);
    glm::vec3 AC = glm::vec3(C.x - A.x, 0, C.z - A.z);

    return (glm::length(glm::cross(AB, AC)) / 2.f);
}

// Check if P is inside ABC
bool Tema2::insideTriangle(glm::vec3 P, glm::vec3 A, glm::vec3 B, glm::vec3 C)
{
    float area = triangleArea(A, B, C);
    float sum = 0;
    sum += triangleArea(A, B, P);
    sum += triangleArea(A, C, P);
    sum += triangleArea(B, C, P);

    if (area <= sum + 0.001 && area >= sum - 0.001) 
        return true;
    return false;
}

// Check if P is on the road
bool Tema2::checkRoadCollision(glm::vec3 P)
{
    for (int i = 0; i < indices.size(); i+=3) {
        if (insideTriangle(P, allVertices[indices[i]].position, allVertices[indices[i + 1]].position,
                           allVertices[indices[i + 2]].position))
            return true;
    }
    return false;
}

// check if P is overlapping with a obstacle
bool Tema2::checkCarsCollision(glm::vec3 P)
{
    for (int i = 0; i < carsPosition.size(); i++) {
        if (sqrt(pow(P.x - carsPosition[i].position.x, 2) + pow(P.z - carsPosition[i].position.z, 2)) <= 0.2f)
            return false;
    }
    return true;
}


void Tema2::RenderObjects(float deltaTimeSeconds)
{
    // Render the road
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["road"], shaders["ShaderTema"], modelMatrix, glm::vec3(0.25f, 0.25f, 0.25f));
    }

    // Render the road line
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderSimpleMesh(meshes["roadLine"], shaders["ShaderTema"], modelMatrix, glm::vec3(1.f, 1.f, 1.f));
    }

    // Render the ground
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.01f, 0));
        RenderSimpleMesh(meshes["plane50"], shaders["ShaderTema"], modelMatrix, glm::vec3(0.46f, 0.86f, 0.46f));
    }

    // Render trees
    {
        for (int i = 0; i < allVertices.size(); i++) {
            if (i % 110 == 0) { // Plasez un copac odata la 110 puncte exterioare/interioare
                // Trunk
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(allVertices[i].position.x, allVertices[i].position.y + 0.15f, allVertices[i].position.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.3f, 0.1f));
                RenderSimpleMesh(meshes["box"], shaders["ShaderTema"], modelMatrix, glm::vec3(0.58f, 0.29f, 0.f));

                // Crown
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(allVertices[i].position.x, allVertices[i].position.y + 0.4f, allVertices[i].position.z));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
                if (crownShape == "box") {
                    crownShape = "sphere";
                }
                else {
                    crownShape = "box";
                }
                RenderSimpleMesh(meshes[crownShape], shaders["ShaderTema"], modelMatrix, glm::vec3(0.22f, 0.37f, 0.04f));

            }
        }
    }

    // Render other cars
    {
        for (int i = 0; i < carsPaths.size(); i++) {

            glm::mat4 modelMatrix = glm::mat4(1);

            // Pentru o miscare mai fluenta, impart punctele intre care
            // se misca o masina, in mai multe puncte, prin care va avansa cu 
            // cate un punct la fiecare apel al functiei.
            float carSpeed = 9.f;
            if (carsTimer[i] == 0) {
                carsPosition[i] = carsPaths[i].front();
                carsPaths[i].pop();
                carsPaths[i].push(carsPosition[i]);
                carsStep[i] = (carsPaths[i].front().position - carsPosition[i].position) / (carSpeed / (i + 1));
                // Pentru calcularea carsStep[i], adica urmatoarea valoare pe care o adaug pozitiei masinii,
                // ma folosesc de o variabila de viteza, pentru care am ales valoarea 9. De asemenea impart aceasta
                // "viteza" la indexul masinii, pentru a avea o variatie intre viteza diferitelor masini.
            }

            // Calculez unghiul de rotatie al masinii
            // unghiul dintre vectorul format din punctele intre 
            // care se afla masina si axa OZ 
            // (Masina isi da initial render cu fata inspre OZ)
            int flag = 1;
            if (carsPosition[i].position.z >= 0)
                flag = 1;
            else
                flag = -1;
            carsRotation[i] = (flag * carsStep[i].z / sqrt(pow(carsStep[i].x, 2) + pow(carsStep[i].y, 2) + pow(carsStep[i].z, 2)));

            carsPosition[i].position += carsStep[i];
            carsTimer[i]+= deltaTimeSeconds;
            if (carsTimer[i] >= deltaTimeSeconds * (carSpeed / (i + 1))) {
                carsTimer[i] = 0;
            }

            modelMatrix = glm::translate(modelMatrix, glm::vec3(carsPosition[i].position.x, 0.051f, carsPosition[i].position.z));
            modelMatrix = glm::rotate(modelMatrix, carsRotation[i], glm::vec3(0, 1, 0));
           
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.1f, 0.1f));
            RenderSimpleMesh(meshes["box"], shaders["ShaderTema"], modelMatrix, carsColor[i]);
        }
    }

    // Render the camera target (player car).
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, RADIANS(60), glm::vec3(0, 1, 0));

        modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f, 0.1f, 0.1f));    
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema"], modelMatrix, glm::vec3(1.25f, 0.25f, 0.25f));
    }
}

void Tema2::RenderScene(float deltaTimeSeconds)
{
    projectionMatrix = glm::perspective(fov, aspect, zNear, zFar);

    RenderObjects(deltaTimeSeconds);
}

void Tema2::RenderMiniMap(float deltaTimeSeconds)
{   
    // Retin fostele valori pentru forward si up
    glm::vec3 forward = camera->forward;
    glm::vec3 up = camera->up;

    // Mut camera in masina, ii schimb orientarea in jos
    // dupa care o mut in sus, pentru a captura si masina
    camera->TranslateForward(camera->distanceToTarget);
    camera->forward = glm::vec3(0, -1, 0);
    camera->up = glm::vec3(0, 0, -1);
    camera->TranslateForward(-1);

    projectionMatrix = glm::ortho(left, right, bottom, top, zNear, zFar);

    RenderObjects(deltaTimeSeconds);

    // Readuc camera in pozitia si orientarea initiala
    camera->TranslateForward(1);
    camera->forward = forward;
    camera->up = up;
    camera->TranslateForward(-(camera->distanceToTarget));

}

void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.53f, 0.81f, 0.94f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Tema2::Update(float deltaTimeSeconds)
{
    // Sets the screen area where to draw
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    RenderScene(deltaTimeSeconds);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    // render the scene again, in the new viewport
    RenderMiniMap(deltaTimeSeconds);
}


void Tema2::FrameEnd()
{

}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind color vector
    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(object_color, 1, glm::value_ptr(color));

    // Bind car position vector
    glm::vec3 carPos = camera->GetTargetPosition();
    int car_position = glGetUniformLocation(shader->program, "car_position");
    glUniform3fv(car_position, 1, glm::value_ptr(carPos));

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 1.5f;

    
    if (window->KeyHold(GLFW_KEY_W)) {
        glm::vec3 nextForwardLocation = camera->GetNextTargetPosition(deltaTime * cameraSpeed);
        if (checkRoadCollision(nextForwardLocation) && checkCarsCollision(nextForwardLocation)) {
            camera->MoveForward(deltaTime * cameraSpeed);
        }
        
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        glm::vec3 nextBackLocation = camera->GetNextTargetPosition(-deltaTime * cameraSpeed);
        if (checkRoadCollision(nextBackLocation) && checkCarsCollision(nextBackLocation))
            camera->MoveForward(-deltaTime * cameraSpeed);

    }

    if (window->KeyHold(GLFW_KEY_A)) {
        glm::vec3 nextForwardLocation = camera->GetNextTargetPosition(deltaTime * cameraSpeed);
        glm::vec3 nextBackLocation = camera->GetNextTargetPosition(-deltaTime * cameraSpeed);

        if (checkCarsCollision(nextForwardLocation) || checkCarsCollision(nextBackLocation)) {
            camera->RotateThirdPerson_OY(deltaTime * cameraSpeed);
            rotation = rotation + (deltaTime * cameraSpeed);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        glm::vec3 nextForwardLocation = camera->GetNextTargetPosition(deltaTime * cameraSpeed);
        glm::vec3 nextBackLocation = camera->GetNextTargetPosition(-deltaTime * cameraSpeed);

        if (checkCarsCollision(nextForwardLocation) || checkCarsCollision(nextBackLocation)) {
            camera->RotateThirdPerson_OY(-deltaTime * cameraSpeed);
            rotation = rotation + (-deltaTime * cameraSpeed);
        }
    } 
}

void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
