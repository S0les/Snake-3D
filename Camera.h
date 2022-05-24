#pragma once 

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Определяет несколько возможных вариантов движения камеры
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


//Значения камеры по умолчанию 
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVTY = 0.05f;
const GLfloat ZOOM = 45.0f;
// Класс абстрактной камеры, который обрабатывает входные данные и вычисляет соответствующие углы, векторы и матрицы
class Camera
{
    public:
    //Атрибуты камеры 
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;
    //Углы Эулера 
        GLfloat Yaw;
        GLfloat Pitch;
    //Параметры камеры 
        GLfloat MovementSpeed;
        GLfloat MouseSensitivity;
        GLfloat Zoom;

    // Конструктор с векторами
    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f), glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),GLfloat yaw = YAW,GLfloat pitch = PITCH): Front(glm::vec3(0.0f,0.0f,-1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {   
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    // Конструктор со скалярными значениями
    Camera(GLfloat posX , GLfloat posY , GLfloat posZ , GLfloat upX, GLfloat upY, GLfloat upZ , GLfloat yaw, GLfloat pitch ) : Front(glm::vec3(0.0f,0.0f,-1.0f)),MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
    {
        this->Position = glm::vec3(posX,posY,posZ);
        this->WorldUp = glm::vec3(upX,upY,upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

    // Возвращает матрицу вида, рассчитанную с использованием Eular Angles и LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(this->Position, this->Position + this->Front, this->Up); 
    }
    // Обрабатывает ввод, полученный от любой системы ввода, подобной клавиатуре.
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
    {
        GLfloat velocity = this->MovementSpeed * deltaTime;
        if (direction == FORWARD)
            this->Position += this->Front * velocity;
        if (direction == BACKWARD) 
            this->Position -= this->Front * velocity;
        if (direction == LEFT) 
            this->Position -= this->Right * velocity;
        if (direction == RIGHT)
            this->Position += this->Right * velocity;   

    }
    
    // Обрабатывает ввод, полученный от системы ввода мыши. Ожидает значение смещения как в направлении x, так и в направлении y.
    void ProcessMouseMovement(GLfloat xoffset , GLfloat yoffset, GLboolean constrainPitch = true )
{
        xoffset *= this->MouseSensitivity;
        yoffset *= this->MouseSensitivity;

        this->Yaw += xoffset; 
        this->Pitch += yoffset;
    // Убедитесь, что когда шаг выходит за пределы, экран не переворачивается
        if (constrainPitch) {
            if (this ->Pitch > 89.0f)
                this->Pitch = 89.0f;
            if (this->Pitch < -89.0f)
                this->Pitch = -89.0f;
        }
        this->updateCameraVectors();

    }
    // Обрабатывает ввод, полученный от события колеса прокрутки мыши. Требуется только ввод по вертикальной оси колеса
    void ProcessMouseScroll(GLfloat yoffset)
    {
        if (this -> Zoom >= 1.0f && this->Zoom <= 45.0f)
        {
            this->Zoom -= yoffset;
        }
        if (this->Zoom <= 1.0f){
            this->Zoom = 1.0f;
        }
        if (this->Zoom >= 45.0f)
            this->Zoom = 45.0f;
    }

private:
    // Вычисляем(обновляем) фронтальный вектор из углов Eulera  
    void updateCameraVectors()
{
    // Рассчитываем новый вектор Фронта
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw))*cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Также пересчитываем вектор вправо и вверх
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}
};
