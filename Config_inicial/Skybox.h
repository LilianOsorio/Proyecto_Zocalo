
#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "stb_image.h"
#include "Shader.h"

class Skybox
{
private:
    GLuint VAO, VBO;
    GLuint cubemapTexture;

    float skyboxVertices[108] = {
        // Back
        -1,  1, -1,  -1, -1, -1,   1, -1, -1,
         1, -1, -1,   1,  1, -1,  -1,  1, -1,

         // Left
         -1, -1,  1,  -1, -1, -1,  -1,  1, -1,
         -1,  1, -1,  -1,  1,  1,  -1, -1,  1,

         // Right
          1, -1, -1,   1, -1,  1,   1,  1,  1,
          1,  1,  1,   1,  1, -1,   1, -1, -1,

          // Front
          -1, -1,  1,  -1,  1,  1,   1,  1,  1,
           1,  1,  1,   1, -1,  1,  -1, -1,  1,

           // Top
           -1,  1, -1,   1,  1, -1,   1,  1,  1,
            1,  1,  1,  -1,  1,  1,  -1,  1, -1,

            // Bottom
            -1, -1, -1,  -1, -1,  1,   1, -1, -1,
             1, -1, -1,  -1, -1,  1,   1, -1,  1
    };

    GLuint loadCubemap(std::vector<std::string> faces)
    {
        GLuint textureID;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_BASE_LEVEL, 0);

        glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAX_LEVEL, 0);
        // No invertir verticalmente las caras del cubemap
        stbi_set_flip_vertically_on_load(false);

        for (GLuint i = 0; i < faces.size(); i++)
        {
            int width, height, channels;

            unsigned char* image = stbi_load(
                faces[i].c_str(),
                &width,
                &height,
                &channels,
                3
            );

            if (image)
            {
                std::cout << "Dimensiones: "
                    << width << " x " << height
                    << " | Canales originales: " << channels
                    << std::endl;

                std::cout << "Primeros pixeles: "
                    << (int)image[0] << ", "
                    << (int)image[1] << ", "
                    << (int)image[2] << std::endl;

                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

                glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB8,
                    width,
                    height,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    image
                );

                GLenum error = glGetError();

                if (error != GL_NO_ERROR)
                {
                    std::cout << "Error OpenGL al cargar cara "
                        << i << ": " << error << std::endl;
                }

                std::cout << "Cara del cielo cargada: "
                    << faces[i] << std::endl;

                stbi_image_free(image);
            }
            else
            {
                std::cout << "ERROR al cargar: "
                    << faces[i] << std::endl;

                std::cout << stbi_failure_reason() << std::endl;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

public:

    Skybox(std::vector<std::string> faces)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(skyboxVertices),
            skyboxVertices,
            GL_STATIC_DRAW
        );

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            (void*)0
        );

        glBindVertexArray(0);

        cubemapTexture = loadCubemap(faces);
    }

    void Draw(Shader& shader, glm::mat4 view, glm::mat4 projection)
    {
        // Permitir dibujar el cielo al fondo
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        shader.Use();

        // Eliminar la traslación de la cámara
        glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "view"),
            1,
            GL_FALSE,
            glm::value_ptr(skyboxView)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(shader.Program, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

        // Activar y vincular el cubemap
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        glUniform1i(
            glGetUniformLocation(shader.Program, "skybox"),
            0
        );

        // Dibujar el cubo
        glBindVertexArray(VAO);

        glDisable(GL_CULL_FACE);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glEnable(GL_CULL_FACE);

        glBindVertexArray(0);

        // Restaurar configuración
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }
};

#endif