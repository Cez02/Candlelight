#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace candle::core {

    class Transform {
        glm::quat m_Rotation;
        glm::vec3 m_EulerAngles;

    public:
        glm::vec3 m_Position;
        glm::float32 m_Scale;

        void setRotation(glm::vec3 eulerAngles){
            m_EulerAngles = eulerAngles;
            m_Rotation = glm::normalize(glm::quat(eulerAngles));
        }

        void setRotation(glm::quat quat){
            m_Rotation = quat;
        }

        void rotateAroundAxis(float angle, glm::vec3 axis){
            m_Rotation = glm::normalize(glm::rotate(m_Rotation, angle, glm::normalize(axis)));
            m_EulerAngles = glm::eulerAngles(m_Rotation);
        }

        glm::vec3 forward(){
            return m_Rotation * glm::vec4(0, 0, 1.0, 1.0);
        }

        glm::vec3 right(){
            return m_Rotation * glm::vec4(1.0, 0, 0.0, 1.0);
        }

        glm::vec3 up(){
            return m_Rotation * glm::vec4(0.0, 1.0, 0.0, 1.0);
        }

        glm::quat getRotationQuat(){
            return m_Rotation;
        }

        glm::vec3 getRotationEuler(){
            return m_EulerAngles;
        }

        Transform()
            : Transform(glm::vec3(0, 0, 0), 1.0f, glm::identity<glm::quat>()) { }

        Transform(glm::vec3 position, glm::float32 scale, glm::quat rotation)
            : m_Position(position)
            , m_Scale(scale)
            , m_Rotation(rotation) {

            m_EulerAngles = glm::eulerAngles(rotation);
        }

        glm::mat4 toMat4(){
            glm::mat4 model = glm::mat4(1.0);

            return glm::translate(model, m_Position) * glm::mat4_cast(m_Rotation) * glm::scale(model, glm::vec3(m_Scale, m_Scale, m_Scale)) * model;
        }
    };

}
