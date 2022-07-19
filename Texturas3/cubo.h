#pragma once

class Cubo {
public:
    glm::mat4 model;
    //glm::mat4 view;

    //trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    /*
    color estructura
    0. front
    1. behind
    2. left
    3. right
    4. down
    5. up
    */
    Cubo(std::vector<glm::vec4> newColor, glm::vec3 translacion, glm::vec3 escalar, glm::mat4 model_) {

        colores.clear();
        for (int i = 0; i < 6; i++) {
            colores.push_back(newColor[i]);
        }

        model = model_;
;
        transform = glm::scale(transform, escalar);
        transform = glm::translate(transform, translacion);

        float vertices1[] = {
            // positions          // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        float vertices2[] = {
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        };
        float vertices3[] = {
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f
        };
        float vertices4[] = {
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f
        };
        float vertices5[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f
        };
        float vertices6[] = {
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f
        };
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        lados[0].setColor(colores[0]); lados[0].setVertices(vertices1, indices);
        lados[1].setColor(colores[1]); lados[1].setVertices(vertices2, indices);
        lados[2].setColor(colores[2]); lados[2].setVertices(vertices3, indices);
        lados[3].setColor(colores[3]); lados[3].setVertices(vertices4, indices);
        lados[4].setColor(colores[4]); lados[4].setVertices(vertices5, indices);
        lados[5].setColor(colores[5]); lados[5].setVertices(vertices6, indices);

    }

    glm::mat4 get_model() {
        return transform;
    }

    void render(glm::mat4 view, glm::mat4 projection) {
        for (int i = 0; i < 6; i++)
            lados[i].render(transform,model,view, projection);
    }

    void set_rotation(float rot_grades, glm::vec3 rot_vect) {
        glm::mat4 model_2 = glm::rotate(glm::mat4(1.0f), glm::radians(rot_grades), rot_vect);
        transform = model_2 * transform;
    }


    void input(GLFWwindow* window) {
        //vacio
    }

    void setColor(std::vector<glm::vec4> newColor) {
        colores.clear();
        for (int i = 0; i < 6; i++) {
            colores.push_back(newColor[i]);
        }
    }
private:
    glm::mat4               transform = glm::mat4(1.0f);
    Plano* lados = new Plano[6];
    std::vector<glm::vec4>  colores = {
    glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), //azul
    glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), //rojo
    glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), //verde
    glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), //magenta
    glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), //celeste
    glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)  //amarillo
    };
};
