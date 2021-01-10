int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int* mapa = (int*)calloc(10*20, sizeof(int));
	int* auxiliar = (int*)calloc(4 * 2 * 4, sizeof(int));
	int* teste = create_fila();
	inserir_auxiliar(auxiliar, teste[0]);
	anda_auxiliar(auxiliar);
	inserir_auxiliar(auxiliar, teste[1]);
	anda_auxiliar(auxiliar);
	inserir_auxiliar(auxiliar, teste[2]);
	anda_auxiliar(auxiliar);
	inserir_auxiliar(auxiliar, teste[3]);
	for (int i = 0; i < 4 * 4 * 2; i++) {
		printf("%d ", auxiliar[i]);
		if ((i + 1) % 8 == 0) {
			printf("\n");
		}
	}
	int ang = rand() % 4 + 1;
	for (int x = 0; x < 4; x++) {
		printf("%d\n", teste[x]);
	}
	time_t timer1, timer2;
	time(&timer1);

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

				RenderText(LetterShader, "Pontuação", 0.7, 0.25, 0.7, glm::vec3(0.1f, 0.1f, 0.1f));
				RenderText(LetterShader, std::to_string(score), 0.7, 0.35, 0.7, glm::vec3(0.1f, 0.1f, 0.1f));

				LetterShader.setBool("isTexture", false);

				//array de teste para as peças
				//int mapa[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0,0,0,0,0 };
				int* teste = create_fila();
				int ang = rand() % 4 + 1;
				colunas = 10;
				linhas = 20;
				while (inserir_peca(teste[0], (ang), mapa)) {
					posy = linhas - 1;


					bool k = false;
					if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
						k = true;
					}
					time(&timer2);
					double seconds = difftime(timer1, timer2);

					while (((int)(seconds + 0.5) + 1) % 5 != 0) {
						printf("ola1\n");
						time(&timer1);
					}
					while (k) {
						//aqui tera de ter um if com um clock ou algo a simular um para q de tempo em tempo a peça desça mesmo q o player nao faça nada
						printf("ola\n");
						
						
						if (anda_baixo(mapa)) {
							posy = posy - 1;
						}
						if (!anda_baixo(mapa)) {
							if (!(linha_cheia(mapa, posy) || linha_cheia(mapa, posy + 1)) || linha_cheia(mapa, posy + 2) || linha_cheia(mapa, posy + 3) || linha_cheia(mapa, posy + -1) || linha_cheia(mapa, posy - 2) || linha_cheia(mapa, posy - 3)) {
								anda_fila(teste);
								k = false;
								break;
							}
							else {
								int pont = 0;
								for (int i = 0; i < linhas; i++) {
									if (linha_cheia(mapa, i)) {
										pont++;
										apaga_linha(mapa, i);
										pontuaçao(pont);
										i = 0;
									}
									anda_fila(teste);
									ang = rand() % 4 + 1;
									k = false;
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
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.09, static_cast<float>(SCR_HEIGHT * 0.2), SCR_POS_X + SCR_WIDTH * 0.10, static_cast<float>(SCR_HEIGHT * 0.8)), glm::vec3(0.0f, 0.0f, 0.0f));
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.09, static_cast<float>(SCR_HEIGHT * 0.185), SCR_POS_X + SCR_WIDTH * 0.41, static_cast<float>(SCR_HEIGHT * 0.20)), glm::vec3(0.0f, 0.0f, 0.0f));
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.40, static_cast<float>(SCR_HEIGHT * 0.2), SCR_POS_X + SCR_WIDTH * 0.41, static_cast<float>(SCR_HEIGHT * 0.8)), glm::vec3(0.0f, 0.0f, 0.0f));

				//posição dinamica
				float auxx = 0.0;
				float auxy = 0.0;
				int count = 0;
				int aux = 0;
				for (int i = 0;i < (linhas * colunas);i++) {
					if (mapa[i] == 0) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.000, 0.545, 0.545));
					}
					else if (mapa[i] == 1 || mapa[i] == 11) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 0.0f));
					}
					else if (mapa[i] == 2 || mapa[i] == 12) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 0.0f, 1.0f));
					}
					else if (mapa[i] == 3 || mapa[i] == 13) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 1.0f, 0.0f));
					}
					else if (mapa[i] == 4 || mapa[i] == 14) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 1.0f, 0.0f));
					}
					else if (mapa[i] == 5 || mapa[i] == 15) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(0.0f, 1.0f, 1.0f));
					}
					else if (mapa[i] == 6 || mapa[i] == 16) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 1.0f));
					}
					else if (mapa[i] == 7 || mapa[i] == 17) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.10 + auxx), static_cast<float>(SCR_HEIGHT * (0.2 + auxy)), SCR_POS_X + SCR_WIDTH * (0.10 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.2 + auxy + 0.03))), glm::vec3(1.0f, 0.5f, 0.0f));
					}

					if ((i - 9) % 10 == 0 && i != 0) {
						auxy = auxy + 0.03;
						auxx = 0.0;
					}
					else {
						auxx = auxx + 0.03;
					}
				}

				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.84, static_cast<float>(SCR_HEIGHT * 0.2), SCR_POS_X + SCR_WIDTH * 0.85, static_cast<float>(SCR_HEIGHT * 0.46)), glm::vec3(0.0f, 0.0f, 0.0f));
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.59, static_cast<float>(SCR_HEIGHT * 0.185), SCR_POS_X + SCR_WIDTH * 0.85, static_cast<float>(SCR_HEIGHT * 0.20)), glm::vec3(0.0f, 0.0f, 0.0f));
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.59, static_cast<float>(SCR_HEIGHT * 0.2), SCR_POS_X + SCR_WIDTH * 0.60, static_cast<float>(SCR_HEIGHT * 0.46)), glm::vec3(0.0f, 0.0f, 0.0f));
				renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * 0.59, static_cast<float>(SCR_HEIGHT * 0.46), SCR_POS_X + SCR_WIDTH * 0.85, static_cast<float>(SCR_HEIGHT * 0.475)), glm::vec3(0.0f, 0.0f, 0.0f));

				auxx = 0;
				auxy = 0;
				colunas = 4;
				linhas = 2;

				for (int i = 0;i < 8;i++) {
					if (auxiliar[i] == 0) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(0.000, 0.545, 0.545));
					}
					else if (auxiliar[i] == 1 || auxiliar[i] == 11) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 0.0f));
					}
					else if (auxiliar[i] == 2 || auxiliar[i] == 12) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(0.0f, 0.0f, 1.0f));
					}
					else if (auxiliar[i] == 3 || auxiliar[i] == 13) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(0.0f, 1.0f, 0.0f));
					}
					else if (auxiliar[i] == 4 || auxiliar[i] == 14) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(1.0f, 1.0f, 0.0f));
					}
					else if (auxiliar[i] == 5 || auxiliar[i] == 15) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(0.0f, 1.0f, 1.0f));
					}
					else if (auxiliar[i] == 6 || auxiliar[i] == 16) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(1.0f, 0.0f, 1.0f));
					}
					else if (auxiliar[i] == 7 || auxiliar[i] == 17) {
						renderImage(LetterShader, keyboard, glm::vec4(SCR_POS_X + SCR_WIDTH * (0.6 + auxx), static_cast<float>(SCR_HEIGHT * (0.3 + auxy)), SCR_POS_X + SCR_WIDTH * (0.60 + auxx + 0.03), static_cast<float>(SCR_HEIGHT * (0.3 + auxy + 0.03))), glm::vec3(1.0f, 0.5f, 0.0f));
					}
					if ((i - 3) % 10 == 0 && i != 0) {
						auxy = auxy + 0.03;
						auxx = 0.0;
					}
					else {
						auxx = auxx + 0.03;
					}
				}
			}
			
			LetterShader.setBool("isTexture", true);
			float* size;
			size = RenderText(LetterShader, "Menu", defsText5[0], defsText5[1], defsText5[2], colorText5);
			sizeText5[0] = size[0]; sizeText5[1] = size[1]; sizeText5[2] = size[2]; sizeText5[3] = size[3];
			
			
		}
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
