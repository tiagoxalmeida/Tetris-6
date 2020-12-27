// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

//definições para o andré
//#define GLEW_STATIC
//#include <glm.hpp>
//#include <gtc/matrix_transform.hpp>
//#include <gtc/type_ptr.hpp>
//#include "shader.cpp"

//definições para o tiago
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <common/shader.cpp>

using namespace glm;

// Vertex array object (VAO)
GLuint VertexArrayID;

// Vertex buffer object (VBO)
GLuint vertexbuffer;

// color buffer object (CBO)
GLuint colorbuffer;

// GLSL program from the shaders
GLuint programID;
//alteraçoes eduardo
int colunas = 10;
int linhas = 20;
int posx = 0;
int posy = 0;
//funçoes usadas no main + algumas extra q podem vir a ser uteis.
int quantos_espaços_linha(int* mapa, int linha) {
    int c = 0;
    for (int x = 0; x < colunas; x++) {
        if (mapa[linha * colunas + x] == 0) {
            c++;
        }
    }
    return c;
}

bool anda_baixo(int* mapa) {
    int p1 = -1, p2 = -1, p3 = -1, p4 = -1, c = 0;
    for (int i = colunas * linhas - 1; -1 < i; i--) {
        if (mapa[i] / 10 != 0) {
            if (c == 0) {
                p1 = i;
            }
            if (c == 1) {
                p2 = i;
            }
            if (c == 2) {
                p3 = i;
            }
            if (c == 3) {
                p4 = i;
            }
            c++;
        }
        if (c == 4) {
            break;
        }
    }
    if (p1< colunas || p2 < colunas || p3 < colunas || p4 < colunas) {
    
        mapa[p1] = mapa[p1] % 10;
        mapa[p2] = mapa[p2] % 10;
        mapa[p3] = mapa[p3] % 10;
        mapa[p4] = mapa[p4] % 10;
        return false;
        }
        if (mapa[p1 - colunas] != 0 && mapa[p1 - colunas] != mapa[p1] ) {
            
            mapa[p1] = mapa[p1] % 10;
            mapa[p2] = mapa[p2] % 10;
            mapa[p3] = mapa[p3] % 10;
            mapa[p4] = mapa[p4] % 10;
            return false;
        }
        if (mapa[p2 - colunas] != 0 && mapa[p2 - colunas] != mapa[p2] ) {
           
            mapa[p1] = mapa[p1] % 10;
            mapa[p2] = mapa[p2] % 10;
            mapa[p3] = mapa[p3] % 10;
            mapa[p4] = mapa[p4] % 10;
            return false;
        }
        if (mapa[p3 - colunas] != 0 && mapa[p3 - colunas] != mapa[p3] ) {
         
            mapa[p1] = mapa[p1] % 10;
            mapa[p2] = mapa[p2] % 10;
            mapa[p3] = mapa[p3] % 10;
            mapa[p4] = mapa[p4] % 10;
            return false;
        }
        if (mapa[p4 - colunas] != 0 && mapa[p4 - colunas] != mapa[p4] ) {
        
            mapa[p1] = mapa[p1] % 10;
            mapa[p2] = mapa[p2] % 10;
            mapa[p3] = mapa[p3] % 10;
            mapa[p4] = mapa[p4] % 10;
            return false;
        }
        int aux1 = mapa[p1], aux2 = mapa[p2], aux3 = mapa[p3], aux4 = mapa[p4];
        mapa[p1] = 0; mapa[p2] = 0; mapa[p3] = 0; mapa[p4] = 0;
        mapa[p1 - colunas] = aux1; mapa[p2 - colunas] = aux2; mapa[p3 - colunas] = aux3; mapa[p4 - colunas] = aux4;
        
        return true;
    
}
bool anda_esquerda(int* mapa) {
    int p1 = -1, p2 = -1, p3 = -1, p4 = -1, c = 0;
    for (int i = colunas * linhas - 1; -1 < i; i--) {
        if (mapa[i] / 10 != 0) {
            if (c == 0) {
                p1 = i;
            }
            if (c == 1) {
                p2 = i;
            }
            if (c == 2) {
                p3 = i;
            }
            if (c == 3) {
                p4 = i;
            }
            c++;
        }
        if (c == 4) {
            break;
        }
    }
    if (p1 % 10 == 0 || p2 % 10 == 0 || p3 % 10 == 0 || p4 % 10 == 0) {
 
        return false;
    }
    if (mapa[p1 - 1] != 0 && mapa[p1 - 1] != mapa[p1]) {
     
        return false;
    }
    if (mapa[p2 - 1] != 0 && mapa[p2 - 1] != mapa[p2]) {
      
        return false;
    }
    if (mapa[p3 - 1] != 0 && mapa[p3 - 1] != mapa[p3]) {
 
        return false;
    }
    if (mapa[p4 - 1] != 0 && mapa[p4 - 1] != mapa[p4]) {
   
        return false;
    }
    int aux1 = mapa[p1], aux2 = mapa[p2], aux3 = mapa[p3], aux4 = mapa[p4];
    mapa[p1] = 0; mapa[p2] = 0; mapa[p3] = 0; mapa[p4] = 0;
    mapa[p1 - 1] = aux1; mapa[p2 - 1] = aux2; mapa[p3 - 1] = aux3; mapa[p4 - 1] = aux4;

    return true;

}
bool anda_direita(int* mapa) {
    //precisa de alterações se o numero das colunas for superior a 10
    int p1 = -1, p2 = -1, p3 = -1, p4 = -1, c = 0;
    for (int i = colunas * linhas - 1; -1 < i; i--) {
        if (mapa[i] / 10 != 0) {
            if (c == 0) {
                p1 = i;
            }
            if (c == 1) {
                p2 = i;
            }
            if (c == 2) {
                p3 = i;
            }
            if (c == 3) {
                p4 = i;
            }
            c++;
        }
        if (c == 4) {
            break;
        }
    }
    if (p1 % 10 == colunas -1 || p2 % 10 == colunas - 1 || p3 % 10 == colunas -1 || p4 % 10 == colunas -1) {
      
        return false;
    }
    if (mapa[p1  +1] != 0 && mapa[p1 + 1] != mapa[p1]) {
       
        return false;
    }
    if (mapa[p2 + 1] != 0 && mapa[p2 + 1] != mapa[p2]) {
       ;
        return false;
    }
    if (mapa[p3 + 1] != 0 && mapa[p3 + 1] != mapa[p3]) {
      
        return false;
    }
    if (mapa[p4 + 1] != 0 && mapa[p4 + 1] != mapa[p4]) {
       
        return false;
    }
    int aux1 = mapa[p1], aux2 = mapa[p2], aux3 = mapa[p3], aux4 = mapa[p4];
    mapa[p1] = 0; mapa[p2] = 0; mapa[p3] = 0; mapa[p4] = 0;
    mapa[p1 + 1] = aux1; mapa[p2 + 1] = aux2; mapa[p3 + 1] = aux3; mapa[p4 + 1] = aux4;

    return true;
}
void display_mapa(int* mapa) {
    for (int i = 0; i < colunas * linhas; i++) {
        printf("%d ", mapa[i]);
        if ((i + 1) % colunas == 0) {
            printf("\n");
        }
    }
}
bool linha_vazia(int* mapa, int linha) {
    for (int x = 0; x < colunas; x++) {
        if (mapa[linha * colunas + x] != 0) {
            return false;
        }
    }
    return true;
}//temos de ter cuidado para nunca chamarmos esta funçao com a linha errada(exemplo haver 20 linhas e chamarmos a 20 da mal pq começa na 0)
bool linha_cheia(int* mapa, int linha) {
    if (linha < 0 || linhas - 1 < linha) {
        return false;
    }
    for (int x = 0; x < colunas; x++) {
        if (mapa[linha * colunas + x] == 0) {
            return false;
        }
    }
    return true;
}//temos de ter cuidado para nunca chamarmos esta funçao com a linha errada(exemplo haver 20 linhas e chamarmos a 20 da mal pq começa na 0)
void substitui_linha(int* mapa, int l1, int l2) {
    for (int x = 0; x < colunas; x++) {
        mapa[l1 * colunas + x] = mapa[l2 * colunas + x];
    }
}
void apaga_linha(int* mapa, int linha) {
    for (int i = linha; i < linhas - 1; i++) {
        substitui_linha(mapa, i, i + 1);
    }
    for (int c = 0; c < colunas; c++) {
        mapa[colunas * (linhas - 1) + c] = 0;
    }
}
int* create_fila(int siz) {
    int * fila = (int*)calloc(siz, sizeof(int));
    for (int i = 0; i < siz; i++) {
        fila[i] = rand() % 7 + 1;
    }
    return fila;
}

void anda_fila(int* fila, int siz) {
    
    for (int i = 0; i < siz-1; i++) {
        fila[i] = fila[i + 1];
    }
    fila[siz - 1] = rand() % 7 + 1;
}
bool inserir_peça(int p, int ang, int* mapa) {//p representa qual das 7 peças será desenhada e o ang representa qual o angulo inicial da peça
    if (p == 1) {
        if (ang == 1 || ang ==3) { //assumi que o angulo 1 é o horizontal
            if (colunas < posx + 4) {
                posx = colunas - 4;//isto pq assumi q a posição do player e o quadrado mais a esquerda
            }
            for (int l = 0; l < 4; l++) {
                if (mapa[(linhas - 1) * colunas + posx + l] != 0) {
                    return false;
                }
                mapa[(linhas - 1) * colunas + posx + l] = 11;
            }
            return true;
        }
        if (ang == 2 || ang == 4) {
            for (int x = 0; x < 4; x++) {
                if (mapa[(linhas - 1) * colunas + posx - x * colunas] != 0) {//verifica se ha quatro espaços para baixo na posicaox do player
                    return false;
                }
                mapa[(linhas - 1) * colunas + posx - x * colunas] = 11;
            }
            return true;
        }
        return false;
    }
    if (p == 2) {
        if (ang == 1) {
            if (colunas < posx+3) {
                posx = colunas - 3;
            }
            if (mapa[(linhas - 1) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 1) * colunas + posx] = 12;
            for (int l = 0; l < 3; l++) {
                if (mapa[(linhas - 2) * colunas + posx + l] != 0) {
                    return false;
                }
                mapa[(linhas - 2) * colunas + posx + l] = 12;   
            }
        }
        if (ang == 2) {
            if (colunas < posx + 2) {
                posx = colunas - 2;
            }
            if (mapa[(linhas-1) * colunas + posx] != 0 || mapa[(linhas-1) * colunas + posx + 1] != 0) {
                return false;
            }
            mapa[(linhas - 1) * colunas + posx] = 12;
            mapa[(linhas - 1) * colunas + posx + 1] = 12;
            if (mapa[(linhas - 2) * colunas + posx] != 0 || mapa[(linhas - 3) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 2) * colunas + posx] = 12;
            mapa[(linhas - 3) * colunas + posx] = 12;
            return true;
        }
        if (ang == 3) {
            if (posx < 2) {
                posx = 2;
            }
            for (int l = 0; l < 3; l++) {
                if (mapa[(linhas - 1) * colunas + posx - l] != 0) {
                    return false;
                }
                mapa[(linhas - 1) * colunas + posx - l] = 12;
            }
            if (mapa[(linhas - 2) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 2) * colunas + posx] = 12;
            return true;
        }
        if (ang == 4) {
            if (colunas < posx+2) {
                posx = colunas-2;
            }
            for (int l = 1; l < 4; l++) {
                if (mapa[(linhas - l) * colunas + posx + 1] != 0) {
                    printf("aqui %d \n", (linhas - l) * colunas + posx + 1);
                    return false;
                }
                mapa[(linhas - l) * colunas + posx + 1] = 12;
            }
            if (mapa[(linhas - 2) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 3) * colunas + posx] = 12;
            return true;
        }

    }
    if (p == 3) {
        if (ang == 1) {
            if (posx - 3 < -1) {
                posx = 2;
            }
            if (mapa[(linhas - 1) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 1) * colunas + posx] = 13;
            for (int l = 0; l < 3; l++) {
                if (mapa[(linhas - 2) * colunas + posx - l] != 0) {
                    return false;
                }
                mapa[(linhas - 2) * colunas + posx - l] = 13;
            }
            return true;
        }
        if (ang == 2) {
            if (posx == 0) {
                posx = 1;
            }
            if (mapa[(linhas - 3) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 3) * colunas + posx] = 13;
            for (int l = 1; l < 4; l++) {
                if (mapa[(linhas - l) * colunas + posx - 1] != 0) {
                    return false;
                }
                mapa[(linhas - l) * colunas + posx - 1] = 13;
            }
            return true;
        }
        if (ang == 3) {
            if (colunas < posx + 3) {
                posx = colunas - 3;
            }
            if (mapa[(linhas - 2) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 2) * colunas + posx] = 13;
            for (int l = 0; l < 3; l++) {
                if (mapa[(linhas - 1) * colunas + posx + l] != 0) {
                    return false;
                }
                mapa[(linhas - 1) * colunas + posx + l] = 13;
            }
            return true;
        }
        if (ang == 4) {
            if (posx == colunas - 1) {
                posx = colunas - 2;
            }
            if (mapa[(linhas - 1) * colunas + posx] != 0) {
                return false;
            }
            mapa[(linhas - 1) * colunas + posx] = 13;
            for (int l = 0; l < 3; l++) {
                if (mapa[(linhas - 1 - l) * colunas + posx + 1] != 0) {
                    return false;
                }
                mapa[(linhas - 1 - l) * colunas + posx + 1] = 13;
            }
            return true;
        }
    }
    if (p == 4){
        if (colunas < posx + 2) {
            posx = colunas - 2;
        }
        if (mapa[(linhas - 1) * colunas + posx + 1] != 0 || mapa[(linhas - 1) * colunas + posx ] != 0) {
            return false;
        }
        mapa[(linhas - 1) * colunas + posx + 1] = 14;
        mapa[(linhas - 1) * colunas + posx] = 14;
        if (mapa[(linhas - 2) * colunas + posx + 1] != 0 || mapa[(linhas - 2) * colunas + posx] != 0) {
            return false;
        }
        mapa[(linhas - 2) * colunas + posx + 1] = 14;
        mapa[(linhas - 2) * colunas + posx] = 14;
        return true;
    }
    if (p == 5) {
        if (ang == 1 || ang == 3) {
            if (colunas < posx + 3) {
                posx = colunas - 3;
            }
            if (mapa[colunas * (linhas - 1) + posx] != 0 || mapa[colunas * (linhas - 1) + posx + 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx] = 15;
            mapa[colunas * (linhas - 1) + posx + 1] = 15;
            if (mapa[colunas * (linhas - 2) + posx + 2] != 0 || mapa[colunas * (linhas - 2) + posx + 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 2) + posx + 2] = 15;
            mapa[colunas * (linhas - 2) + posx + 1] = 15;
            return true;
        }
        if (ang == 2 || ang == 4) {
            if (posx == 0) {
                posx = 1;
            }
            if (mapa[colunas * (linhas - 1) + posx] != 0 || mapa[colunas * (linhas - 2) + posx] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx] = 15;
            mapa[colunas * (linhas - 2) + posx] = 15;
            if (mapa[colunas * (linhas - 2) + posx-1] != 0 || mapa[colunas * (linhas - 3) + posx-1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 2) + posx - 1] = 15;
            mapa[colunas * (linhas - 3) + posx - 1] = 15;
            return true;
        }
    }
    if (p == 6) {
        if (ang == 1 || ang == 3) {
            if (colunas < posx + 3) {
                posx = colunas - 3;
            }
            if (mapa[colunas * (linhas - 2) + posx] != 0 || mapa[colunas * (linhas - 2) + posx + 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 2) + posx] = 16;
            mapa[colunas * (linhas - 2) + posx + 1] = 16;
            if (mapa[colunas * (linhas - 1) + posx + 1] != 0 || mapa[colunas * (linhas - 1) + posx + 2] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx + 1] = 16;
            mapa[colunas * (linhas - 1) + posx + 2] = 16;
            return true;
        }
        if (ang == 2 || ang == 4) {
            if (posx == colunas - 1) {
                posx = colunas - 2;
            }
            if (mapa[colunas * (linhas - 1) + posx] != 0 || mapa[colunas * (linhas - 2) + posx ] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx] = 16;
            mapa[colunas * (linhas - 2) + posx] = 16;
            if (mapa[colunas * (linhas - 2) + posx+1] != 0 || mapa[colunas * (linhas - 3) + posx+1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 2) + posx + 1] = 16;
            mapa[colunas * (linhas - 3) + posx + 1] = 16;
            return true;
        }
    }
    if (p == 7) {
        if (ang == 1) {
            if (posx == 0) {
                posx = 1;
            }
            if (posx == colunas - 1) {
                posx = colunas - 2;
            }
            if (mapa[colunas * (linhas - 1) + posx] != 0 || mapa[colunas * (linhas - 2) + posx] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx] = 17;
            mapa[colunas * (linhas - 2) + posx] = 17;
            if (mapa[colunas * (linhas - 2) + posx +1] != 0 || mapa[colunas * (linhas - 2) + posx - 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 2) + posx + 1] = 17;
            mapa[colunas * (linhas - 2) + posx - 1] = 17;
            return true;
        }
        if (ang == 2) {
            if (posx == 0) {
                posx = 1;
            }
            if (mapa[colunas * (linhas - 2) + posx] != 0 || mapa[colunas * (linhas - 2) + posx - 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 2) + posx] = 17;
            mapa[colunas * (linhas - 2) + posx - 1] = 17;
            if (mapa[colunas * (linhas - 1) + posx-1] != 0 || mapa[colunas * (linhas - 3) + posx - 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx - 1] = 17; 
            mapa[colunas * (linhas - 3) + posx - 1] = 17;
            return true;
        }
        if (ang == 3) {
            if (posx == 0) {
                posx = 1;
            }
            if (posx == 9) {
                posx = 8;
            }
            if (mapa[colunas * (linhas - 1) + posx] != 0 || mapa[colunas * (linhas - 2) + posx] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx] = 17;
            mapa[colunas * (linhas - 2) + posx] = 17;
            if (mapa[colunas * (linhas - 1) + posx + 1] != 0 || mapa[colunas * (linhas - 1) + posx - 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx + 1] = 17;
            mapa[colunas * (linhas - 1) + posx - 1] = 17;
            return true;
        }
        if (ang == 4) {
            if (posx == colunas-1) {
                posx = colunas -2 ;
            }
            if (mapa[colunas * (linhas - 2) + posx] != 0 || mapa[colunas * (linhas - 2) + posx + 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 2) + posx] = 17;
            mapa[colunas * (linhas - 2) + posx + 1] = 17;
            if (mapa[colunas * (linhas - 1) + posx - 1] != 0 || mapa[colunas * (linhas - 3) + posx - 1] != 0) {
                return false;
            }
            mapa[colunas * (linhas - 1) + posx + 1] = 17;
            mapa[colunas * (linhas - 3) + posx + 1] = 17;
            return true;
        }
        }
    else { return false; }
    }
int posicao[200];
GLint WindowWidth = 600;
GLint WindowHeight = 600;
GLint LastWindowWidth = 600;
GLint LastWindowHeight = 600;
float t = 0;
float l = 0;
float w = WindowWidth;
float h = WindowHeight;
float mouse_x = 0;
float mouse_y = 0;

int x = 0;
int orient = 0;
int p1 = 0;
int p2 = 0;
int p3 = 0;
int p4 = 0;
int timer = 0;
//vamos ter flags para identificar o que estamos a ver no momento
bool is_menu_view = true;
bool is_game_view = false;
bool is_options_view = false; // talvez numa fase mais avançada poderemos implementar opções para definir a velocidade das peças a cairem, sons e etc
bool is_scores_view = false;
bool is_name_view = false;


void transferDataToGPUMemory(void) //functio to transfer data for GPU
{
	// VAO
	glGenVertexArrays(1, &VertexArrayID); //definition of vertex array object
	glBindVertexArray(VertexArrayID); //bind the array object

	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("shader.vertexshader", "shader.fragmentshader");//load shaders

	//Definition of the vertices
	float var1 = 20.0f;
	float var2 = -5.0f;
	GLfloat g_vertex_buffer_data[200 * 6*3];
	GLfloat g_color_buffer_data[200 * 6*3];
	for (int z = 0;z < 200 * 6*3;z = z + 6 * 3) {
		g_vertex_buffer_data[z] = var2;
		g_vertex_buffer_data[z + 3] = var2 + 1.0f;
		g_vertex_buffer_data[z + 6] = var2;
		g_vertex_buffer_data[z + 9] = var2;
		g_vertex_buffer_data[z + 12] = var2 + 1.0f;
		g_vertex_buffer_data[z + 15] = var2 + 1.0f;
		if (var2 == 4.0f) {
			var2 = -5.0f;
		}
		else {
			var2 = var2 + 1.0f;
		}
	}
	var2 = -5.0f;
	for (int i = 1; i < 200 * 6*3; i = i + 6*3) {
		g_vertex_buffer_data[i] = var1;
		g_vertex_buffer_data[i + 3] = var1;
		g_vertex_buffer_data[i + 6] = var1 - 1.0f;
		g_vertex_buffer_data[i + 9] = var1 - 1.0f;
		g_vertex_buffer_data[i + 12] = var1;
		g_vertex_buffer_data[i + 15] = var1 - 1.0f;
		if (var2 == 4.0f) {
			var1 = var1-1.0f;
		}
		if (var2 == 4.0f) {
			var2 = -5.0f;
		}
		else {
			var2 = var2 + 1.0f;
		}
	}
	for (int w = 2; w < 200 * 6*3; w = w + 6 * 3) {
		g_vertex_buffer_data[w] = 0.0f;
		g_vertex_buffer_data[w + 3] = 0.0f;
		g_vertex_buffer_data[w + 6] = 0.0f;
		g_vertex_buffer_data[w + 9] = 0.0f;
		g_vertex_buffer_data[w + 12] = 0.0f;
		g_vertex_buffer_data[w + 15] = 0.0f;
	}
	


	for (int i = 0;i < 200 * 6*3;i = i + 3) {
		g_color_buffer_data[i] = 1.0f;
		g_color_buffer_data[i+1] = 0.0f;
		g_color_buffer_data[i+2] = 0.0f;
	}

	// Move vertex data to video memory; specifically to VBO called vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Move color data to video memory; specifically to CBO called colorbuffer
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

}

void cleanupDataFromGPU()
{
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
}
void draw(void) {
	// Clear the screen
	//glClear(GL_COLOR_BUFFER_BIT);

	// Use our shader
	glUseProgram(programID);

	// create transformations
	//glm::mat4 model = glm::mat4(1.0f);
	//glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 mvp = glm::ortho(-10.0f, 10.0f, 0.0f,20.0f);

	// Our ModelViewProjection : multiplication of our 3 matrices
	//glm::mat4 mvp = projection * view * model;
	// Remember, matrix multiplication is the other way around

	// retrieve the matrix uniform locations
	unsigned int matrix = glGetUniformLocation(programID, "mvp");
	glUniformMatrix4fv(matrix, 1, GL_FALSE, &mvp[0][0]);


	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);


	//glEnable(GL_PROGRAM_POINT_SIZE);
	//glPointSize(10);
	// Draw the triangle !
	for (int i = 0;i < 200;i++) {
		if (posicao[i] == 1) {
			glDrawArrays(GL_TRIANGLES, i * 6, 6); // 3 indices starting at 0 -> 1 triangle
			//glDrawArrays(GL_POINTS, 0, 9); // 3 indices starting at 0 -> 1 triangle
		}
	}
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}
void removerlinha(void) {
	int count = 0;
	for (int i = 0;i < 200;i = i + 10) {
		for (int j = 0;j < 10;j++) {
			if (posicao[i + j] == 1)
				count++;
		}
		if (count == 10) {
			for (int j = 0;j < 10;j++) {
				posicao[i + j] = 0;
			}
			for (int k = i - 1;k >= 0;k--) {
				if (posicao[k] == 1) {
					posicao[k] = 0;
					posicao[k + 10] = 1;
				}
			}

		}
		count = 0;
	}
}
int criarpeca(void) {
	int random = 1 + rand() % 7;
	if (random == 1) {
		p1 = 4;
		p2 = 5;
		p3 = 14;
		p4 = 15;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
	if (random == 2) {
		p1 = 3;
		p2 = 4;
		p3 = 5;
		p4 = 6;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
	if (random == 3) {
		p1 = 3;
		p2 = 13;
		p3 = 14;
		p4 = 15;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
	if (random == 4) {
		p1 = 3;
		p2 = 4;
		p3 = 5;
		p4 = 13;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
	if (random == 5) {
		p1 = 4;
		p2 = 5;
		p3 = 13;
		p4 = 14;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
	if (random == 6) {
		p1 = 3;
		p2 = 4;
		p3 = 14;
		p4 = 15;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
	if (random == 7) {
		p1 = 4;
		p2 = 14;
		p3 = 13;
		p4 = 15;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
	return random;
}
void andaresquerda(void) {
	if ((p1 % 10) == 0 || (p2 & 10) == 0 || (p3 % 10) == 0 || (p4 % 10) == 0 || (posicao[p1 - 1] == 1 && (p1 - 1) != p2 && (p1 - 1) != p3 && (p1 - 1) != p4) || (posicao[p2 - 1] == 1 && (p2 - 1) != p1 && (p2 - 1) != p3 && (p2 - 1) != p4) || (posicao[p3 - 1] == 1 && (p3 - 1) != p1 && (p3 - 1) != p2 && (p3 - 1) != p4) || (posicao[p4 - 1] == 1 && (p4 - 1) != p1 && (p4 - 1) != p2 && (p4 - 1) != p3)) {

	}
	else {
		posicao[p1] = 0;
		posicao[p2] = 0;
		posicao[p3] = 0;
		posicao[p4] = 0;
		p1 -= 1;
		p2 -= 1;
		p3 -= 1;
		p4 -= 1;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
}
void andardireita(void) {
	if ((p1 % 10) == 9 || (p2 & 10) == 9 || (p3 % 10) == 9 || (p4 % 10) == 9 || (posicao[p1 + 1] == 1 && (p1 + 1) != p2 && (p1 + 1) != p3 && (p1 + 1) != p4) || (posicao[p2 + 1] == 1 && (p2 + 1) != p1 && (p2 + 1) != p3 && (p2 + 1) != p4)|| (posicao[p3 + 1] == 1 && (p3 + 1) != p1 && (p3 + 1) != p2 && (p3 + 1) != p4) || (posicao[p4 + 1] == 1 && (p4 + 1) != p1 && (p4 + 1) != p2 && (p4 + 1) != p3) ) {

	}
	else {
		posicao[p1] = 0;
		posicao[p2] = 0;
		posicao[p3] = 0;
		posicao[p4] = 0;
		p1 += 1;
		p2 += 1;
		p3 += 1;
		p4 += 1;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
}
void andarbaixo(void) {
	if (p1 >= 190 || p2 >= 190 || p3 >= 190 || p4 >= 190 || (posicao[p1 + 10] == 1 && (p1 + 10) != p2 && (p1 + 10) != p3 && (p1 + 10) != p4) || (posicao[p2 + 10] == 1 && (p2 + 10) != p1 && (p2 + 10) != p3 && (p2 + 10) != p4) || (posicao[p3 + 10] == 1 && (p3 + 10) != p1 && (p3 + 10) != p2 && (p3 + 10) != p4) || (posicao[p4 + 10] == 1 && (p4 + 10) != p1 && (p4 + 10) != p2 && (p4 + 10) != p3)) {
		orient = 0;
		removerlinha();
		x = criarpeca();
	}

	else {
		posicao[p1] = 0;
		posicao[p2] = 0;
		posicao[p3] = 0;
		posicao[p4] = 0;
		p1 += 10;
		p2 += 10;
		p3 += 10;
		p4 += 10;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
}
void andarcima(void){
	if (p1 < 10 || p2 < 10 || p3 < 10 || p4 < 10 || (posicao[p1 - 10] == 1 && (p1 - 10) != p2 && (p1 - 10) != p3 && (p1 - 10) != p4) || (posicao[p2 - 10] == 1 && (p2 - 10) != p1 && (p2 - 10) != p3 && (p2 - 10) != p4) || (posicao[p3 - 10] == 1 && (p3 - 10) != p1 && (p3 - 10) != p2 && (p3 - 10) != p4) || (posicao[p4 - 10] == 1 && (p4 - 10) != p1 && (p4 - 10) != p2 && (p4 - 10) != p3)) {

	}
	
	else {
		posicao[p1] = 0;
		posicao[p2] = 0;
		posicao[p3] = 0;
		posicao[p4] = 0;
		p1 -= 10;
		p2 -= 10;
		p3 -= 10;
		p4 -= 10;
		posicao[p1] = 1;
		posicao[p2] = 1;
		posicao[p3] = 1;
		posicao[p4] = 1;
	}
}
void rodarclockwise(int x) {
	if (x == 1) {

	}
	if (x == 2) {
		if(orient == 0) {
			if (p2 - 9 < 0) {
				andarbaixo();
				andarbaixo();
			}
			if (posicao[p2 - 9] == 1) {
				andarbaixo();
				andarbaixo();
			}
			if (p1 - 18 < 0) {
				andarbaixo();
			}
			if (posicao[p1 - 18] == 1) {
				andarbaixo();
			}
			if (p4 + 9 > 200) {
				andarcima();
			}
			posicao[p1] = 0;
			posicao[p2] = 0;
			posicao[p4] = 0;
			p1 -= 18;
			p2 -= 9;
			p4 += 9;
			posicao[p1] = 1;
			posicao[p2] = 1;
			posicao[p4] = 1;
			orient = 1;
		}
		else {
			if (orient == 1) {
				if (p4 % 10 == 9) {
					andaresquerda();
				}
				if (p1 % 10 == 0) {
					andardireita();
					andardireita();
				}
				posicao[p1] = 0;
				posicao[p2] = 0;
				posicao[p4] = 0;
				p1 += 18;
				p2 += 9;
				p4 -= 9;
				posicao[p1] = 1;
				posicao[p2] = 1;
				posicao[p4] = 1;
				orient = 0;
			}

		}
	}
	if (x == 3) {
		if (orient == 0) {
			if (posicao[p4 + 9] == 1) {
				andarcima();
			}
			posicao[p1] = 0;
			posicao[p2] = 0;
			posicao[p4] = 0;
			p1 += 2;
			p2 -= 9;
			p4 += 9;
			posicao[p1] = 1;
			posicao[p2] = 1;
			posicao[p4] = 1;
			orient = 1;
		}
		else {
			if (orient == 1) {
				if (p3 % 10 == 9) {
					andaresquerda();
				}
				posicao[p1] = 0;
				posicao[p2] = 0;
				posicao[p4] = 0;
				p1 += 20;
				p2 += 11;
				p4 -= 11;
				posicao[p1] = 1;
				posicao[p2] = 1;
				posicao[p4] = 1;
				orient = 2;
			}
			else {
				if (orient == 2) {
					posicao[p1] = 0;
					posicao[p2] = 0;
					posicao[p4] = 0;
					p1 -= 2;
					p2 += 9;
					p4 -= 9;
					posicao[p1] = 1;
					posicao[p2] = 1;
					posicao[p4] = 1;
					orient = 3;
				}
				else {
					if (orient == 3) {
						posicao[p1] = 0;
						posicao[p2] = 0;
						posicao[p4] = 0;
						p1 -= 20;
						p2 -= 11;
						p4 += 11;
						posicao[p1] = 1;
						posicao[p2] = 1;
						posicao[p4] = 1;
						orient = 0;
					}
				}
			}
		}

	}
	if (x == 1) {

	}
	if (x == 1) {

	}
	if (x == 1) {

	}
	if (x == 1) {

	}
}
bool is_CTRL_down;
bool is_F_down;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT)) { // clicar na tecla UP e/ou continuar a clicar
		andarcima();
	}

	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT)) {// clicar na tecla DOWN e/ou continuar a clicar
		andarbaixo();
		
	}

	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {// clicar na tecla LEFT e/ou continuar a clicar
		andaresquerda();
	}

	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT)) {// clicar na tecla RIGHT e/ou continuar a clicar
		andardireita();
	}
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	}
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		rodarclockwise(x);
	}
	if (key == GLFW_KEY_ESCAPE && (action == GLFW_PRESS)) {//clicar na tecla ESCAPE para 
		int width_mm, height_mm;
		glfwGetMonitorPhysicalSize(glfwGetPrimaryMonitor(), &width_mm, &height_mm);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowMonitor(window, NULL, (mode->width - LastWindowWidth) / 2, (mode->height - LastWindowHeight) / 2, LastWindowWidth, LastWindowHeight, 0);
		glViewport(l, t, w, h);
	}
	if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL)
		is_CTRL_down = action == GLFW_PRESS;
	if (key == GLFW_KEY_F) 
		is_F_down = action == GLFW_PRESS;
	if (is_CTRL_down && is_F_down)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwSetWindowPos(window, (mode->width - WindowWidth) / 2, (mode->height - WindowHeight) / 2);

		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		glViewport(l, t, w, h);
	}


}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	if (is_name_view) { // se estiver na view de escrever o nome então escrevemos o nome

	}
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_x = xpos;
	mouse_y = ypos;
	printf("%f,%f\n", mouse_x, mouse_y);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//use yoffset to move up and down in menus
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		//if mouse right button clicked
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//if mouse left button clicked
	
	}
}

//TODO fazer com que as proporções fiquem iguais caso façamos o resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) //para fazer o jogo responsivo
{
	t = t*height / (float)WindowHeight;
	l = l*width / (float)WindowWidth;
	w = w*width / (float)WindowWidth;
	h = h*height / (float)WindowHeight;
	glViewport(l, t, w, h);
	LastWindowHeight = WindowHeight;
	WindowHeight = h;
	LastWindowWidth = WindowWidth;
	WindowWidth = w;
}

void mainProgram() {
	// render scene for each frame
	for (int i = 0;i < 200;i++) {
		posicao[i] = 0;
	}
	x=criarpeca();
	do {
		glClear(GL_COLOR_BUFFER_BIT);
		//left top
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//draw(); //draw function
			// drawing callback

		draw();
		// Swap buffers
		glfwSwapBuffers(window);
		// looking for events
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwWindowShouldClose(window) == 0);
}
/* main q eu tenho q contem um exemplo de 1 jogo de tetris + os displays do mapa(o display do mapa é invertido verticalmente pq calhou e n me apeteceu mudar e nao e usado no jogo, só para debug)
int main()
{
    int * mapa = (int*)calloc(colunas * linhas, sizeof(int));
    int* teste = create_fila(4);
    int clock = 0;
    while (inserir_peça(teste[0], (rand() % 4 + 1), mapa)) {
        posy = linhas -1;
        printf("\n fila: ");
        for (int i = 0; i < 4; i++) {
            printf("%d ", teste[i]);
        }


        printf("mapa:  \n");
        display_mapa(mapa);
        while (true) {
            	aqui tera de ter um if com um clock ou algo a simular um para q de tempo em tempo a peça desça mesmo q o player nao faça nada
                if (anda_baixo(mapa)) {
                    posy = posy - 1;
                }
                if (!anda_baixo(mapa)) {
                    if (!(linha_cheia(mapa, posy)|| linha_cheia(mapa, posy +1)) || linha_cheia(mapa, posy +2) || linha_cheia(mapa, posy + 3) || linha_cheia(mapa, posy + -1) || linha_cheia(mapa, posy - 2) || linha_cheia(mapa, posy -3)){
                        anda_fila(teste, 4);
                        break;
                    }
                    else {
                        for (int i = 0; i < linhas; i++) {
                            if (linha_cheia(mapa, i)) {
                                apaga_linha(mapa, i);
                                i = 0;
                            }
                            anda_fila(teste, 4);
                            break;
                        }
                    }
                }

            
            //if glwpress_right anda_direita and if true posx = posx+1; corri o programa so em cpp entao nao dava para usar os glwpress q o tiago tinha feito
            //if glwpress_left anda_esquerda and if true posx = posx-1;
        }
    }
    display_mapa(mapa);

    
    
    
    return 0;
}

*/
int main(void)
{
	// Initialise GLFW
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window
	window = glfwCreateWindow(WindowWidth, WindowHeight, "6TURN Tetris -The Game", NULL, NULL);

	// Create window context
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	glewInit();

	//key input callback
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);

	//mouse input callback
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// White background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// transfer my data (vertices, colors, and shaders) to GPU side
	transferDataToGPUMemory();

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	//glBindFramebuffer(GL_FRAMEBUFFER, FramebufvisualerName);


	mainProgram();

	// delete framebuffer
	glDeleteFramebuffers(1, &FramebufferName);

	// Cleanup VAO, VBOs, and shaders from GPU
	cleanupDataFromGPU();

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
