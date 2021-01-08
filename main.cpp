#include <iostream>
#include <map>
#include <string>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader_m.h"

//colors
glm::vec3 llGray = glm::vec3(0.863f, 0.863f, 0.863f);
glm::vec3 dGray = glm::vec3(0.1f, 0.1f, 0.1f);


//input
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
float * RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

int colunas = 10;
int linhas = 20;
int posx = 0;
int posy = 0;
int score = 0;

void pontuaçao(int x) {
	if (x == 1) {
		score = score + 80;
	}
	if (x == 2) {
		score = score + 20;
	}
	if (x == 3) {
		score = score + 200;
	}
	if (x == 4) {
		score = score + 900;
	}
	if (4 < x) {
		score = score + 500;
	}
}
void remove_andamento(int* mapa) {
	int q = 0;
	for (int i = 0; i < linhas * colunas; i++) {
		if (mapa[i] / 10 == 1) {
			q++;
			mapa[i] = 0;
		}
		if (q == 4) {
			break;
		}
	}
}
int quantos_espacos_linha(int* mapa, int linha) {
	int c = 0;
	for (int x = 0; x < colunas; x++) {
		if (mapa[linha * colunas + x] == 0) {
			c++;
		}
	}
	return c;
}

bool inserir_peca(int p, int ang, int* mapa) {//p representa qual das 7 peças será desenhada e o ang representa qual o angulo inicial da peça
	if (p == 1) {
		if (ang == 1 || ang == 3) { //assumi que o angulo 1 é o horizontal
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
			if (colunas < posx + 3) {
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
			if (mapa[(linhas - 1) * colunas + posx] != 0 || mapa[(linhas - 1) * colunas + posx + 1] != 0) {
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
			if (colunas < posx + 2) {
				posx = colunas - 2;
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
	if (p == 4) {
		if (colunas < posx + 2) {
			posx = colunas - 2;
		}
		if (mapa[(linhas - 1) * colunas + posx + 1] != 0 || mapa[(linhas - 1) * colunas + posx] != 0) {
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
			if (mapa[colunas * (linhas - 2) + posx - 1] != 0 || mapa[colunas * (linhas - 3) + posx - 1] != 0) {
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
			if (mapa[colunas * (linhas - 1) + posx] != 0 || mapa[colunas * (linhas - 2) + posx] != 0) {
				return false;
			}
			mapa[colunas * (linhas - 1) + posx] = 16;
			mapa[colunas * (linhas - 2) + posx] = 16;
			if (mapa[colunas * (linhas - 2) + posx + 1] != 0 || mapa[colunas * (linhas - 3) + posx + 1] != 0) {
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
			if (mapa[colunas * (linhas - 2) + posx + 1] != 0 || mapa[colunas * (linhas - 2) + posx - 1] != 0) {
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
			if (mapa[colunas * (linhas - 1) + posx - 1] != 0 || mapa[colunas * (linhas - 3) + posx - 1] != 0) {
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
			if (posx == colunas - 1) {
				posx = colunas - 2;
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
	if (p1 < colunas || p2 < colunas || p3 < colunas || p4 < colunas) {

		mapa[p1] = mapa[p1] % 10;
		mapa[p2] = mapa[p2] % 10;
		mapa[p3] = mapa[p3] % 10;
		mapa[p4] = mapa[p4] % 10;
		return false;
	}
	if (mapa[p1 - colunas] != 0 && mapa[p1 - colunas] != mapa[p1]) {

		mapa[p1] = mapa[p1] % 10;
		mapa[p2] = mapa[p2] % 10;
		mapa[p3] = mapa[p3] % 10;
		mapa[p4] = mapa[p4] % 10;
		return false;
	}
	if (mapa[p2 - colunas] != 0 && mapa[p2 - colunas] != mapa[p2]) {

		mapa[p1] = mapa[p1] % 10;
		mapa[p2] = mapa[p2] % 10;
		mapa[p3] = mapa[p3] % 10;
		mapa[p4] = mapa[p4] % 10;
		return false;
	}
	if (mapa[p3 - colunas] != 0 && mapa[p3 - colunas] != mapa[p3]) {

		mapa[p1] = mapa[p1] % 10;
		mapa[p2] = mapa[p2] % 10;
		mapa[p3] = mapa[p3] % 10;
		mapa[p4] = mapa[p4] % 10;
		return false;
	}
	if (mapa[p4 - colunas] != 0 && mapa[p4 - colunas] != mapa[p4]) {

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
	if (p1 % 10 == colunas - 1 || p2 % 10 == colunas - 1 || p3 % 10 == colunas - 1 || p4 % 10 == colunas - 1) {

		return false;
	}
	if (mapa[p1 + 1] != 0 && mapa[p1 + 1] != mapa[p1]) {

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
	int* fila = (int*)calloc(siz, sizeof(int));
	for (int i = 0; i < siz; i++) {
		fila[i] = rand() % 7 + 1;
	}
	return fila;
}

void anda_fila(int* fila, int siz) {

	for (int i = 0; i < siz - 1; i++) {
		fila[i] = fila[i + 1];
	}
	fila[siz - 1] = rand() % 7 + 1;
}

void rotacoes(int p, int* mapa, int ang, int lado) {
	int cima = 0;
	int baixo = 0;
	remove_andamento(mapa);
	if (ang + lado == 5) {
		cima = 1;
		ang = 1;
		lado = 0;
	}
	if (ang + lado == 0) {
		baixo = 1;
		ang = 4;
		lado = 0;
	}
	if (!inserir_peca(p, ang + lado, mapa)) {
		if (cima == 1) {
			remove_andamento(mapa);
			ang = 4;
			inserir_peca(p, ang, mapa);
		}
		if (baixo == 1) {
			remove_andamento(mapa);
			ang = 1;
			inserir_peca(p, ang, mapa);
		}
		if (baixo == 0 && cima == 0) {
			remove_andamento(mapa);
			inserir_peca(p, ang, mapa);
		}
	}
}
// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
float SCR_POS_X = 0.0f;
float SCR_POS_Y = 0.0f;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// menu Text
glm::vec3 colorText1 = glm::vec3(0.1f, 0.1f, 0.1f);
GLfloat sizeText1[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText1[3] = { 0.5f, 0.4, 0.6f };
glm::vec3 colorText2 = glm::vec3(0.1f, 0.1f, 0.1f);
GLfloat sizeText2[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText2[3] = { 0.5f, 0.5f, 0.6f };
glm::vec3 colorText3 = glm::vec3(0.1f, 0.1f, 0.1f);
GLfloat sizeText3[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText3[3] = { 0.5f, 0.6, 0.6f };
glm::vec3 colorText4 = glm::vec3(0.1f, 0.1f, 0.1f);
GLfloat sizeText4[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText4[3] = { 0.5f, 0.7, 0.6f };
glm::vec3 colorText5 = glm::vec3(0.1f, 0.1f, 0.1f);
GLfloat sizeText5[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText5[3] = { 0.5f, 0.95, 0.6f };
glm::vec3 colorText6 = glm::vec3(0.5f, 0.5f, 0.5f);
GLfloat sizeText6[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText6[3] = { 0.495f, 0.6, 0.6f };
glm::vec3 colorText7 = glm::vec3(0.1f, 0.1f, 0.1f);
GLfloat sizeText7[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText7[3] = { 0.5f, 0.95, 0.6f };
glm::vec3 colorText8 = glm::vec3(0.1f, 0.1f, 0.1f);
GLfloat sizeText8[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
GLfloat defsText8[3] = { 0.5f, 0.95, 0.6f };

//menu buttons
int btnSelected = -1;

//pages
const int numpages = 4; //number of pages
int lastpage = 1; //last page viewed
int page = 1; //current page
int nextpage = 1; //page to view next
bool transition = false; //flag to see if in transition or not
float tspeed = 0.025; //speed of the transition
bool init = false;
bool isNamed = false;

//for scores
std::string name = " ";

//debug
bool flag = true;

/// Holds all state information relevant to a character as loaded using FreeType
struct Character {
	unsigned int TextureID; // ID handle of the glyph texture
	glm::ivec2   Size;      // Size of glyph
	glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
	unsigned int Advance;   // Horizontal offset to advance to next glyph
};

std::map<GLchar, Character> Characters;
unsigned int VAO, VBO;


void loadFont(std::string font_name) {
	// FreeType
	// --------
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	// find path to font
	if (font_name.empty())
	{
		std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
		return;
	}

	// load font as face
	FT_Face face;
	if (FT_New_Face(ft, font_name.c_str(), 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	else {
		// set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 72);

		// disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 255; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0, //para o layout
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// now store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)*1.2
			};
			Characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

unsigned int loadPic(std::string pic_name) {
	// load and create a texture 
	// -------------------------
	unsigned int texture1;
	// texture 1
	// ---------
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	const char *name = pic_name.c_str();
	unsigned char *data = stbi_load(name, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load picture" << std::endl;
	}
	stbi_image_free(data);
	return texture1;
}

void renderImage(Shader shader,unsigned int texture, glm::vec4 position, glm::vec3 color) {

	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	float vertices[6][4] = {
		{ position.x, position.y, 0.0f, 0.0f },
		{ position.z, position.y, 1.0f, 0.0f },
		{ position.z, position.w, 1.0f, 1.0f },
			
		{ position.z, position.w, 1.0f, 1.0f },
		{ position.x, position.w, 0.0f, 1.0f },
		{ position.x, position.y, 0.0f, 0.0f }
	};
	// render glyph texture over quad
	glBindTexture(GL_TEXTURE_2D, texture);
	// update content of VBO memory
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// render quad
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int* mapa = (int*)calloc(10*20, sizeof(int));

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwSetWindowSizeLimits(window, 600, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// OpenGL state
	// ------------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// compile and setup the shader
	// ----------------------------
	Shader LetterShader("lettershader.vs", "lettershader.fs");
	
	//loadFont("fonts/AmaticSC-Regular.ttf");
	loadFont("fonts/Staatliches-Regular.ttf");
	unsigned int keyboard = loadPic("media/keyboard.jpg");
	unsigned int btnTex = loadPic("media/btnTex.png");

	// configure VAO/VBO for texture quads
	// -----------------------------------
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	// render loop
	// -----------
	// variables for transition betwen pages
	//
	float posxi = 0.0f;
	float posyi = 0.0f;
	float posxf = 0.0f;
	float posyf = 0.0f;
	float posxa = 0.0f;
	float posya = 0.0f;
	
	glm::mat4 model;
	bool arrived[2] = { false,false };
	while (!glfwWindowShouldClose(window))
	{
		//printf("%f %f\n%d %d\n\n", SCR_POS_X, SCR_POS_Y, SCR_WIDTH, SCR_HEIGHT);
		float pagesPos[2 * numpages] = {
			0.0f, 0.0f,
			static_cast<float>(SCR_WIDTH), 0.0f,
			-static_cast<float>(SCR_WIDTH), 0.0f,
			0.0f, static_cast<float>(SCR_HEIGHT)

		};
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.000, 0.545, 0.545, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (transition) {
			if (!init) {
				posxi = pagesPos[0 + 2*(page - 1)];
				posyi = pagesPos[1 + 2*(page - 1)];

				posxf = pagesPos[0 + 2*(nextpage - 1)];
				posyf = pagesPos[1 + 2*(nextpage - 1)];

				posxa = posxi;
				posya = posyi;
				//printf("%f %f", posxf, posyf);
				init = true;
				arrived[0] = false;
				arrived[1] = false;
			}

			posxa += (posxf - posxi)*tspeed;
			posya += (posyf - posyi)*tspeed;

			model = glm::ortho(posxa, SCR_WIDTH + posxa, posya, SCR_HEIGHT + posya);
			//printf("current=%f %f\n target=%f %f", posxa, posya, posxf, posyf);
			if ((int)(posxf * 100 + .5) == (int)(posxa * 100 + .5))
				arrived[0] = true;
			if ((int)(posyf * 100 + .5) == (int)(posya * 100 + .5))
				arrived[1] = true;
			if (arrived[0] && arrived[1]) {
				//printf("%f %f\n", posxf, posyf);
				transition = false;
				lastpage = page;
				page = nextpage;
				init = false;
				SCR_POS_X = posxf;
				SCR_POS_Y = posyf;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				glfwSetCursor(window, NULL);
				btnSelected = -1;
				isNamed = false;
				name = " ";
			}
		}
		else {
			SCR_POS_X = pagesPos[0 + 2 * (page - 1)];
			SCR_POS_Y = pagesPos[1 + 2 * (page - 1)];
			model = glm::ortho(pagesPos[0 + 2*(page - 1)], static_cast<float>(SCR_WIDTH)+SCR_POS_X, pagesPos[1 + 2*(page - 1)], static_cast<float>(SCR_HEIGHT)+SCR_POS_Y);
		}

		if (page == 1) {
			// world transformation
			
			LetterShader.use();
			LetterShader.setMat4("model", model);
			LetterShader.setBool("isTransparency", true);
			LetterShader.setBool("isTexture", true);
			RenderText(LetterShader, "Tetris 6", 0.5f, SCR_HEIGHT - 80.0f, 0.9f, glm::vec3(1.0f, 1.0f, 1.0f));
			float * size;
			size = RenderText(LetterShader, "Iniciar", defsText1[0], defsText1[1], defsText1[2], colorText1);
			sizeText1[0] = size[0];sizeText1[1] = size[1];sizeText1[2] = size[2];sizeText1[3] = size[3];
		
			size = RenderText(LetterShader, "Classificações",defsText2[0], defsText2[1], defsText2[2], colorText2);
			sizeText2[0] = size[0]; sizeText2[1] = size[1]; sizeText2[2] = size[2]; sizeText2[3] = size[3];

			size = RenderText(LetterShader, "Controlos", defsText3[0], defsText3[1], defsText3[2], colorText3);
			sizeText3[0] = size[0]; sizeText3[1] = size[1]; sizeText3[2] = size[2]; sizeText3[3] = size[3];

			size = RenderText(LetterShader, "Sair", defsText4[0], defsText4[1], defsText4[2], colorText4);
			sizeText4[0] = size[0]; sizeText4[1] = size[1]; sizeText4[2] = size[2]; sizeText4[3] = size[3];

			RenderText(LetterShader, "Pressiona a tecla f para entrares em ecrã cheio", 0.5f, 40.0f, 0.35f, glm::vec3(0.863f, 0.863f, 0.863f));
			RenderText(LetterShader, "Made by Turn", 15.0f, 15.0f, 0.3f, glm::vec3(0.3, 0.7f, 0.9f));
			LetterShader.setBool("isTexture", true);
			LetterShader.setBool("isTransparency", false);
			
		}
		else if (page == 2) {
			LetterShader.use();
			LetterShader.setMat4("model", model);
			LetterShader.setBool("isTransparency", true);
			if (!isNamed) {
				float * size;
				LetterShader.setBool("isTexture", true);
				RenderText(LetterShader, "O seu nome:", 0.495, 0.2, 0.7, glm::vec3(0.1f, 0.1f, 0.1f));
				size = RenderText(LetterShader, name, 0.495, ((SCR_HEIGHT*0.6) + 12.0f), 0.5, glm::vec3(0.1f, 0.1f, 0.1f));
				float sizet[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				sizet[0] = size[0]; sizet[1] = size[1]; sizet[2] = size[2]; sizet[3] = size[3];
				time_t current_time;
				current_time = time(NULL);
				if ((current_time + 1) % 2 == 0) {
					renderImage(LetterShader, keyboard, glm::vec4(sizet[2] + 5.0f, ((SCR_HEIGHT*0.6) + 12.0f), sizet[2] + 7.0f, ((SCR_HEIGHT*0.6) + 42.0f)), glm::vec3(0.1f, 0.1f, 0.1f));
				}
				size = RenderText(LetterShader, "Ok", defsText6[0], defsText6[0], defsText6[0], colorText6);
				sizeText6[0] = size[0]; sizeText6[1] = size[1]; sizeText6[2] = size[2]; sizeText6[3] = size[3];
				LetterShader.setBool("isTexture", false);
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH*0.2, static_cast<float>(SCR_HEIGHT*0.6), SCR_POS_X + SCR_WIDTH*0.8, static_cast<float>(SCR_HEIGHT*0.6)+3.0f), glm::vec3(0.1f, 0.1f, 0.1f));
				
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH*0.1, static_cast<float>(SCR_HEIGHT*0.3), SCR_POS_X + SCR_WIDTH*0.9, static_cast<float>(SCR_HEIGHT*0.9)), glm::vec3(0.529, 0.808, 0.922));
			}
			else {

			}
			LetterShader.setBool("isTexture", true);
			float * size;
			size = RenderText(LetterShader, "Menu", defsText5[0], defsText5[1], defsText5[2], colorText5);
			sizeText5[0] = size[0]; sizeText5[1] = size[1]; sizeText5[2] = size[2]; sizeText5[3] = size[3];

			LetterShader.setBool("isTexture", false);

			//array de teste para as peças
			//int mapa[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 };
			int* teste = create_fila(4);
			int clock = 0;
			int ang = rand() % 4 + 1;
			while (inserir_peca(teste[0], (ang), mapa)) {
				posy = linhas - 1;
				printf("\n fila: ");
				for (int i = 0; i < 4; i++) {
					printf("%d ", teste[i]);
				}


				while (true) {
					//aqui tera de ter um if com um clock ou algo a simular um para q de tempo em tempo a peça desça mesmo q o player nao faça nada
						
					if (anda_baixo(mapa)) {
							posy = posy - 1;
						}
					if (!anda_baixo(mapa)) {
						if (!(linha_cheia(mapa, posy) || linha_cheia(mapa, posy + 1)) || linha_cheia(mapa, posy + 2) || linha_cheia(mapa, posy + 3) || linha_cheia(mapa, posy + -1) || linha_cheia(mapa, posy - 2) || linha_cheia(mapa, posy - 3)) {
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
								ang = rand() % 4 + 1;
								break;
							}
						}
					}


					//if glwpress_right anda_direita and if true posx = posx+1; corri o programa so em cpp entao nao dava para usar os glwpress q o tiago tinha feito
					//if glwpress_left anda_esquerda and if true posx = posx-1;
				//if glwpress_a  rotacoes(teste[0], mapa, ang, -1)}
				// if glwpress_d rotacoes(teste[0], mapa, ang, +1)
				}
			}
			//definação dos limites do campo
			renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.09, static_cast<float>(SCR_HEIGHT * 0.2), SCR_POS_X + SCR_WIDTH * 0.10, static_cast<float>(SCR_HEIGHT * 0.8)), glm::vec3(0.0f,0.0f,0.0f));	
			renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.09, static_cast<float>(SCR_HEIGHT * 0.185), SCR_POS_X + SCR_WIDTH * 0.41, static_cast<float>(SCR_HEIGHT * 0.20)), glm::vec3(0.0f, 0.0f, 0.0f));
			renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.40, static_cast<float>(SCR_HEIGHT * 0.2), SCR_POS_X + SCR_WIDTH * 0.41, static_cast<float>(SCR_HEIGHT * 0.8)), glm::vec3(0.0f, 0.0f, 0.0f));
				
			//posição dinamica
			float auxx = 0.0;
			float auxy = 0.0;
			int count = 0;
			int aux = 0;
			for (int i = 0;i < (linhas*colunas);i++) {
				if (mapa[i] == 0) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.000, 0.545, 0.545));
				}
				if (mapa[i] == 1) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 0.0f));
				}
				if (mapa[i] == 2) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 0.0f));
				}
				if (mapa[i] == 3) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 1.0f, 0.0f));
				}
				if (mapa[i] == 4) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 0.0f, 1.0f));
				}
				if (mapa[i] == 5) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.5f, 0.5f, 0.0f));
				}
				if (mapa[i] == 6) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.5f, 0.0f, 0.5f));
				}
				if (mapa[i] == 7) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 1.5f, 0.5f));
				}
				if (mapa[i] == 11) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 1.0f));
				}
				if (mapa[i] == 12) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(2.0f, 1.0f, 0.0f));
				}
				if (mapa[i] == 13) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 1.0f));
				}
				if (mapa[i] == 14) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 2.0f));
				}
				if (mapa[i] == 15) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 3.0f, 0.0f));
				}
				if (mapa[i] == 16) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.9f, 0.8f));
				}
				if (mapa[i] == 16) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 0.3f, 1.0f));
				}
				if (mapa[i] == 17) {
					renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 0.8f, 0.0f));
				}
				if ((i - 9) % 10 == 0 && i != 0) {
					auxy = auxy + 0.03;
					auxx = 0.0;
				}
				else {
					auxx = auxx + 0.03;
				}
			}
		}
		//teste
		else if (page == 3) {
			LetterShader.use();
			LetterShader.setBool("isTransparency", true);
			LetterShader.setBool("isTexture", true);
			LetterShader.setMat4("model", model);
			float * size;
			size = RenderText(LetterShader, "Melhores Classificações", 0.5f, 0.1f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
			//primeiro valor
			size = RenderText(LetterShader, "12000", 0.5f, 0.2f, 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
			//Fazer o loop para mostrar os 5 primeiros resultados
			//float x = size[1];
			//size = RenderText(LetterShader, "11000", 0.5f, SCR_HEIGHT - (x + 50.0f), 0.4f, glm::vec3(1.0f, 1.0f, 1.0f));
			
			size = RenderText(LetterShader, "Menu", 0.5f, 0.9f, 0.6f, colorText7);
			sizeText7[0] = size[0]; sizeText7[1] = size[1]; sizeText7[2] = size[2]; sizeText7[3] = size[3];
			
		}
		else if (page == 4) {
			LetterShader.use();
			LetterShader.setMat4("model", model);
			LetterShader.setBool("isTransparency", true);
			LetterShader.setBool("isTexture", true);
			RenderText(LetterShader, "Controlos", 0.5, 0.15, 0.7, glm::vec3(1.0f, 1.0f, 1.0f));
			float *size;
			size = RenderText(LetterShader, "W, UP - RODAR A PEÇA", 0.5, 0.7, 0.4, glm::vec3(1.0f, 1.0f, 1.0f));
			float x = size[1];
			size = RenderText(LetterShader, "A, LEFT, D, RIGHT, S, DOWN - MOVIMENTAR A PEÇA", 0.5, SCR_HEIGHT - (x + 55.0f), 0.4, glm::vec3(1.0f, 1.0f, 1.0f));
			x = size[1];
			size = RenderText(LetterShader, "ENTER - CONFIRMAR", 0.5, SCR_HEIGHT - (x + 55.0f), 0.4, glm::vec3(1.0f, 1.0f, 1.0f));
			x = size[1];
			size = RenderText(LetterShader, "F - FULLSCREEN", 0.5, SCR_HEIGHT - (x + 55.0f), 0.4, glm::vec3(1.0f, 1.0f, 1.0f));

			
			size = RenderText(LetterShader, "MENU", 0.5, 0.95, 0.6, colorText8);
			sizeText8[0] = size[0]; sizeText8[1] = size[1]; sizeText8[2] = size[2]; sizeText8[3] = size[3];
			LetterShader.setBool("isTransparency", false);
			renderImage(LetterShader, keyboard, glm::vec4(SCR_WIDTH*0.1, static_cast<float>(SCR_HEIGHT*0.4 + SCR_HEIGHT), SCR_WIDTH*0.9, static_cast<float>(SCR_HEIGHT*0.8+SCR_HEIGHT)), glm::vec3(1.0f, 1.0f, 1.0f));

			
		}
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		//-------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if (page != 2) {
		if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && action == GLFW_PRESS)
			btnSelected -= 1;
		if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && action == GLFW_PRESS)
			btnSelected += 1;
		if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
			mouse_button_callback(window, GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0);
	}
	if (page == 2) {
		if (!isNamed) {
			if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
				isNamed = true;
			}
			else if (key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS) {
				if (!name.empty()) {
					name.erase(std::prev(name.end()));
				}
				if (name.empty()) {
					name += " ";
				}
			}
			else {
				if (action == GLFW_PRESS && mods == 0)
					name += key;
			}
		}

	}
	
}

void processInput(GLFWwindow *window)
{
	if (page == 1 && btnSelected != -1) {
		if (btnSelected < 1)
			btnSelected = 4;
		if (btnSelected > 4 || btnSelected == 0)
			btnSelected = 1;
	}


	if (btnSelected == 1)
		colorText1 = glm::vec3(1.0f, 1.0f, 1.0f);
	else
		colorText1 = glm::vec3(0.1f, 0.1f, 0.1f);
	if (btnSelected == 2)
		colorText2 = glm::vec3(1.0f, 1.0f, 1.0f);
	else
		colorText2 = glm::vec3(0.1f, 0.1f, 0.1f);
	if (btnSelected == 3)
		colorText3 = glm::vec3(1.0f, 1.0f, 1.0f);
	else
		colorText3 = glm::vec3(0.1f, 0.1f, 0.1f);
	if (btnSelected == 4)
		colorText4 = glm::vec3(1.0f, 1.0f, 1.0f);
	else
		colorText4 = glm::vec3(0.1f, 0.1f, 0.1f);
	if (btnSelected == 5)
		colorText5 = glm::vec3(1.0f, 1.0f, 1.0f);
	else
		colorText5 = glm::vec3(0.1f, 0.1f, 0.1f);
	if (btnSelected == 6)
		colorText6 = glm::vec3(0.000, 0.545, 0.545);
	else
		colorText6 = glm::vec3(0.1f, 0.1f, 0.1f);
	if (btnSelected == 7)
		colorText7 = glm::vec3(1.0f, 1.0f, 1.0f);
	else
		colorText7 = glm::vec3(0.1f, 0.1f, 0.1f);
	if (btnSelected == 8)
		colorText8 = glm::vec3(1.0f, 1.0f, 1.0f);
	else
		colorText8 = glm::vec3(0.1f, 0.1f, 0.1f);

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	SCR_HEIGHT = height;
	SCR_WIDTH = width;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//printf("mousepos: %f %f\n", xpos, ypos);
	//printf("textpos:%f %f %f %f\n", sizeText1[0], sizeText1[2],sizeText1[1],sizeText1[3]);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursor(window, NULL);
	btnSelected = -1;
	if (page == 1) {
		if (xpos >= sizeText1[0]  && xpos <= sizeText1[2] && ypos >= sizeText1[1] && ypos <= sizeText1[3]) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 1;
		}
		if (xpos >= sizeText2[0] && xpos <= sizeText2[2] && ypos >= sizeText2[1] && ypos <= sizeText2[3]) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 2;
		}
		if (xpos >= sizeText3[0] && xpos <= sizeText3[2] && ypos >= sizeText3[1] && ypos <= sizeText3[3]) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 3;
		}
		if (xpos >= sizeText4[0] && xpos <= sizeText4[2] && ypos >= sizeText4[1] && ypos <= sizeText4[3]) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 4;
		}
	}
	else if (page == 2) {
		
		if (xpos >= sizeText5[0] - SCR_POS_X && xpos <= sizeText5[2] - SCR_POS_X && ypos >= sizeText5[1] && ypos <= sizeText5[3]) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 5;
		}
		if (xpos >= sizeText6[0] - SCR_POS_X && xpos <= sizeText6[2] - SCR_POS_X && ypos >= sizeText6[1] && ypos <= sizeText6[3]) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 6;
		}
	}
	else if (page == 3) {
		if (xpos >= sizeText7[0] - SCR_POS_X && xpos <= sizeText7[2] - SCR_POS_X && ypos >= sizeText7[1] - SCR_POS_Y && ypos <= sizeText7[3] - SCR_POS_Y) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 7;
		}
	}
	else if (page == 4) {
		printf("%f %f\n%f %f %f %f\n\n", xpos, ypos, sizeText8[0], sizeText8[1], sizeText8[2], sizeText8[3]);
		if (xpos >= sizeText8[0] - SCR_POS_X && xpos <= sizeText8[2] - SCR_POS_X && ypos >= sizeText8[1] + SCR_POS_Y && ypos <= sizeText8[3] + SCR_POS_Y) {
			glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
			btnSelected = 8;
		}
	}
	
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		switch (btnSelected) {
			case 1:
				lastpage = page;
				nextpage = 2;
				transition = true;
				break;
			case 2:
				lastpage = page;
				nextpage = 3;
				transition = true;
				break;
			case 3:
				lastpage = page;
				nextpage = 4;
				transition = true;
				break;
			case 4:
				glfwDestroyWindow(window);
				break;
			case 5:
				lastpage = page;
				nextpage = 1;
				transition = true;
				break;
			case 6:
				isNamed = true;
				break;
			case 7:
				lastpage = page;
				nextpage = 1;
				transition = true;
				break;
			case 8:
				lastpage = page;
				nextpage = 1;
				transition = true;
				break;
				
			default:
				break;
		}
			
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	if (yoffset > 0) {
		btnSelected -= 1;
	}
	if (yoffset < 0) {
		btnSelected += 1;
	}

}


// render line of text
// -------------------
float * RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
	// activate corresponding render state	
	shader.use();
	glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// iterate through all characters
	std::string::const_iterator c;
	float xpos=0;
	float ypos=0;
	float w;
	float h;
	float finalpos[4];
	if (y > 0.0f && y < 1.0f) {
		y = SCR_POS_Y + ( SCR_HEIGHT ) * ( 1.0f - y );
	}

	if (x < 1.0f && x > 0.0f) {
		float aux = x;
		x = 0.0f;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];
			xpos = x + ch.Bearing.x * scale;
			w = ch.Size.x * scale;
			x += (ch.Advance >> 6) * scale;
		}
		if (xpos != 0) {
			x = (SCR_POS_X + (SCR_WIDTH ) * aux - xpos/2);
		}
		else {
			x = (SCR_POS_X + (SCR_WIDTH)* aux);
		}
		
	}
	finalpos[0] = x;
	finalpos[3] = SCR_HEIGHT - y;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		xpos = x + ch.Bearing.x * scale;
		ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		w = ch.Size.x * scale;
		h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	finalpos[2] = xpos + w;
	finalpos[1] = SCR_HEIGHT - (ypos + h);
	return finalpos;
}
