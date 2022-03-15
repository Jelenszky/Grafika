#include <array>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <math.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

#define PI 3.14159265359


GLuint		centerLoc;
GLuint		XoffsetLocation;
GLuint		YoffsetLocation;
GLfloat centercoordinates[3];
int steps = 100;
float angle = PI * 2.f / steps;
float xPos = 0.0; float yPos = 0.0; float radius = 0.16666666f;
float incX = 0.01f;
float incY = 0.00f;
float x = 0.0;
float Y = 0.0;
float y = 0.0;
float szog = 0.0f;
float vektor_szog;

/** Hagyományos tömb is használható a csúcspontok tárolásához. */
/** C array can be used to store vertices also. */
static GLfloat s_vertices[306];
static GLfloat s_colors[306];


/** Hagyományos tömb használható a színek tárolásához. */
/** C array can be used to store colors. */
static GLfloat s_line_vertices[6] = { (-1.0 / 3.0), 0.0, 0.0, (1.0 / 3.0), 0.0, 0.0 };
static GLfloat s_vector_vertices[6];





/* Vertex buffer objektumok és vertex array objektum az adattároláshoz. */
/* Vertex buffer object IDs and vertex array object ID for data storing.*/
#define		numVBOs			4
#define		numVAOs			3
GLuint		VBO[numVBOs];
GLuint		VAO[numVAOs];

int			window_width = 600;
int			window_height = 600;
char		window_title[] = "Moving circle";
/** A normál billentyűk a [0..255] tartományban vannak, a nyilak és a speciális billentyűk pedig a [256..511] tartományban helyezkednek el. */
/** Normal keys are fom [0..255], arrow and special keys are from [256..511]. */
GLboolean	keyboard[512] = { GL_FALSE };
GLFWwindow* window = nullptr;
GLuint		renderingProgram;
GLuint		renderingProgram2;
GLuint		renderingProgram3;


void pontgeneral() {

	float prevX = xPos;
	float prevY = yPos - radius;
	s_vertices[0] = 0.0;
	s_vertices[1] = 0.0;
	s_vertices[2] = 0.0;
	s_colors[0] = 1.0;
	s_colors[1] = 0.0;
	s_colors[2] = 0.0;
	for (int i = 0; i <= steps; i++) {
		float newX = radius * cos(angle * i);
		float newY = -radius * sin(angle * i);
		s_vertices[i * 3 + 3] = newX;
		s_vertices[i * 3 + 4] = newY;
		s_vertices[i * 3 + 5] = 0.0;
		s_colors[i * 3 + 3] = 0.0;
		s_colors[i * 3 + 4] = 1.0;
		s_colors[i * 3 + 5] = 0.0;

		prevX = newX;
		prevY = newY;

	}

}

void vektorgeneral() {
	srand(time(NULL));
	rand();
	vektor_szog = (float)(rand() % 90 + 1) / 180 * PI;
	//cout << vektor_szog << endl;
	//cout << (float)(rand() % 90 + 1 ) / 180 << endl;
	//10 pixel hossza = 0.03333
	float hossz = 0.03333;
	s_vector_vertices[3] = hossz * cos(vektor_szog);
	s_vector_vertices[4] = hossz * sin(vektor_szog);

}


void szakaszfrissit() {
	s_line_vertices[1] += y;
	s_line_vertices[4] += y;
	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a vertex buffert (ide kerülnek a csúcspont adatok). */
	/* We attach the vertex buffer to the GL_ARRAY_BUFFER node (vertices are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_line_vertices), s_line_vertices, GL_STATIC_DRAW);
	/* std::array megadása paraméternek. */
	/* std::array as parameter. */
	// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	/* Csatoljuk a vertex array objektumunkat a shader programhoz. */
	/* Attach the vertex array object to the shader program. */
	glBindVertexArray(VAO[1]);
	/* Ezen adatok szolgálják a location = 0 vertex attribútumot (itt: pozíció).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő vertex adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 0 vertex attribute (position).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first vertex data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(0);

	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void szinvalt1() {
	s_colors[0] = 0.0;
	s_colors[1] = 1.0;
	s_colors[2] = 0.0;
	for (int i = 0; i <= steps; i++) {
		s_colors[i * 3 + 3] = 1.0;
		s_colors[i * 3 + 4] = 0.0;
		s_colors[i * 3 + 5] = 0.0;
	}
	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a vertex buffert (ide kerülnek a csúcspont adatok). */
	/* We attach the vertex buffer to the GL_ARRAY_BUFFER node (vertices are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
	/* std::array megadása paraméternek. */
	/* std::array as parameter. */
	// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	/* Csatoljuk a vertex array objektumunkat a shader programhoz. */
	/* Attach the vertex array object to the shader program. */
	glBindVertexArray(VAO[0]);
	/* Ezen adatok szolgálják a location = 0 vertex attribútumot (itt: pozíció).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő vertex adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 0 vertex attribute (position).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first vertex data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(0);
	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a color buffert (ide kerülnek a szín adatok). */
	/* We attach the color buffer to the GL_ARRAY_BUFFER node (color values are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_colors), s_colors, GL_STATIC_DRAW);
	/* Ezen adatok szolgálják a location = 1 vertex attribútumot (itt: szín).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő szín adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 1 vertex attribute (color).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first color data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 1 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 1 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(1);


	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void szinvalt2() {
	s_colors[0] = 1.0;
	s_colors[1] = 0.0;
	s_colors[2] = 0.0;
	for (int i = 0; i <= steps; i++) {
		s_colors[i * 3 + 3] = 0.0;
		s_colors[i * 3 + 4] = 1.0;
		s_colors[i * 3 + 5] = 0.0;
	}
	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a vertex buffert (ide kerülnek a csúcspont adatok). */
	/* We attach the vertex buffer to the GL_ARRAY_BUFFER node (vertices are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
	/* std::array megadása paraméternek. */
	/* std::array as parameter. */
	// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	/* Csatoljuk a vertex array objektumunkat a shader programhoz. */
	/* Attach the vertex array object to the shader program. */
	glBindVertexArray(VAO[0]);
	/* Ezen adatok szolgálják a location = 0 vertex attribútumot (itt: pozíció).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő vertex adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 0 vertex attribute (position).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first vertex data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(0);
	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a color buffert (ide kerülnek a szín adatok). */
	/* We attach the color buffer to the GL_ARRAY_BUFFER node (color values are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_colors), s_colors, GL_STATIC_DRAW);
	/* Ezen adatok szolgálják a location = 1 vertex attribútumot (itt: szín).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő szín adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 1 vertex attribute (color).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first color data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 1 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 1 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(1);


	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool checkOpenGLError() {
	bool	foundError = false;
	int		glErr = glGetError();

	/** Vizsgáljuk meg, hogy van-e aktuálisan OpenGL hiba, és amennyiben igen, írassuk ki azokat a konzolra egyenként. */
	/** Check for OpenGL errors, and send them to the console ony by one. */
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;

		foundError = true;
		glErr = glGetError();
	}

	/** Ha van aktuálisan OpenGL hiba, a visszatérési érték true. */
	/** If there are OpenGL errors, the return value is true. */
	return foundError;
}

void printShaderLog(GLuint shader) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;

	/** Vizsgáljuk meg, hogy van-e valami a Shader Info Logban, és amennyiben igen, írassuk ki azt a konzolra soronként. */
	/** Check for Shader Info Log, and send it to the console by lines if it is created for the last compile. */
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);

		/** Olvassuk és írassuk ki a következő sort. */
		/** Read out and and send to the console the next line. */
		glGetShaderInfoLog(shader, length, &charsWritten, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void printProgramLog(int prog) {
	int		length = 0;
	int		charsWritten = 0;
	char* log = nullptr;

	/** Vizsgáljuk meg, hogy van-e valami a Program Info Logban, és amennyiben igen, írassuk ki azt a konzolra soronként. */
	/** Check for Program Info Log, and send it to the console by lines if it is created for the last compile. */
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &length);

	if (length > 0) {
		log = (char*)malloc(length);

		/** Olvassuk és írassuk ki a következő sort. */
		/** Read out and and send to the console the next line. */
		glGetProgramInfoLog(prog, length, &charsWritten, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

string readShaderSource(const char* filePath) {
	/** A file stream inicializálása olvasásra. */
	/** Let's initialize the file stream for reading. */
	ifstream	fileStream(filePath, ios::in);
	string		content;
	string		line;

	/** A shader fájl sorainak beolvasása EOF jelzésig. (EOF = End Of File) */
	/** Read in the lines of the shader file until EOF. (EOF = End Of File) */
	while (!fileStream.eof()) {
		getline(fileStream, line);
		/** A shader fájl sorainak összefűzése. */
		/** Append the lines of the shader file. */
		content.append(line + "\n");
	}

	/** A file stream lezárása. */
	/** Let's close the file stream. */
	fileStream.close();

	/** Visszatérés a shader fájl tartalmával. */
	/** Return the content of the shader file. */
	return content;
}

GLuint createShaderProgram() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;
	/** Beolvassuk a shader fájlok tartalmát. */
	/** Read in both shader files. */
	string		vertShaderStr = readShaderSource("vertexShader.glsl");
	string		fragShaderStr = readShaderSource("fragmentShader.glsl");
	/** Létrehozzuk a shader objektumokat és eltároljuk az ID-ket. */
	/** Let's create the shader objects and store the IDs. */
	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);
	/** A shader fájlok tartalmát eltároló string objektum szöveggé konvertálásás is elvégezzük. */
	/** The contents of the shader string objects shall be converted to text of characters. */
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	/** Ekkor a betöltött kódot hozzárendelhetjük a shader objektumainkhoz. */
	/** The loaded source codes are assigned to the shader objects. */
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	/** Fordítsuk le a vertex shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the vertex shader object. */
	glCompileShader(vShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}

	/** Fordítsuk le a fragment shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the fragment shader object. */
	glCompileShader(fShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}

	/** Shader program objektum létrehozása: ez fogja össze a shadereket. Eltároljuk az ID értéket. */
	/** Shader program object initialization: holds together the shaders. vfProgram stores the ID. */
	GLuint		vfProgram = glCreateProgram();
	/** Csatoljuk a shadereket az előző lépésben létrehozott objektumhoz. */
	/** The shaders are attached to the program object. */
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	/** Végül a GLSL szerkesztő ellenőrzi, hogy a csatolt shaderek kompatibilisek-e egymással. */
	/** GLSL linker checks the shaders for compatibility. */
	glLinkProgram(vfProgram);
	/** Hibakeresési lépések. Például sikeres volt-e az összeszerkesztés? Ha nem, mi volt az oka? */
	/** Error checking. Was the link step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}

	/** Ha minden rendben ment a linkelés során, a shader objektumok törölhetőek. */
	/** If everything is OK at linking, the shader objects can be destroyed. */
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	/** Az elkészült program azonosítója a visszatérési értékünk. */
	/** The program ID will be the return value. */
	return vfProgram;
}

GLuint createShaderProgram2() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;
	/** Beolvassuk a shader fájlok tartalmát. */
	/** Read in both shader files. */
	string		vertShaderStr = readShaderSource("vertexShader2.glsl");
	string		fragShaderStr = readShaderSource("fragmentShader2.glsl");
	/** Létrehozzuk a shader objektumokat és eltároljuk az ID-ket. */
	/** Let's create the shader objects and store the IDs. */
	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);
	/** A shader fájlok tartalmát eltároló string objektum szöveggé konvertálásás is elvégezzük. */
	/** The contents of the shader string objects shall be converted to text of characters. */
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	/** Ekkor a betöltött kódot hozzárendelhetjük a shader objektumainkhoz. */
	/** The loaded source codes are assigned to the shader objects. */
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	/** Fordítsuk le a vertex shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the vertex shader object. */
	glCompileShader(vShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}

	/** Fordítsuk le a fragment shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the fragment shader object. */
	glCompileShader(fShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}

	/** Shader program objektum létrehozása: ez fogja össze a shadereket. Eltároljuk az ID értéket. */
	/** Shader program object initialization: holds together the shaders. vfProgram stores the ID. */
	GLuint		vfProgram = glCreateProgram();
	/** Csatoljuk a shadereket az előző lépésben létrehozott objektumhoz. */
	/** The shaders are attached to the program object. */
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	/** Végül a GLSL szerkesztő ellenőrzi, hogy a csatolt shaderek kompatibilisek-e egymással. */
	/** GLSL linker checks the shaders for compatibility. */
	glLinkProgram(vfProgram);
	/** Hibakeresési lépések. Például sikeres volt-e az összeszerkesztés? Ha nem, mi volt az oka? */
	/** Error checking. Was the link step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}

	/** Ha minden rendben ment a linkelés során, a shader objektumok törölhetőek. */
	/** If everything is OK at linking, the shader objects can be destroyed. */
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	/** Az elkészült program azonosítója a visszatérési értékünk. */
	/** The program ID will be the return value. */
	return vfProgram;
}

GLuint createShaderProgram3() {
	GLint		vertCompiled;
	GLint		fragCompiled;
	GLint		linked;
	/** Beolvassuk a shader fájlok tartalmát. */
	/** Read in both shader files. */
	string		vertShaderStr = readShaderSource("vertexShader3.glsl");
	string		fragShaderStr = readShaderSource("fragmentShader3.glsl");
	/** Létrehozzuk a shader objektumokat és eltároljuk az ID-ket. */
	/** Let's create the shader objects and store the IDs. */
	GLuint		vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fShader = glCreateShader(GL_FRAGMENT_SHADER);
	/** A shader fájlok tartalmát eltároló string objektum szöveggé konvertálásás is elvégezzük. */
	/** The contents of the shader string objects shall be converted to text of characters. */
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	/** Ekkor a betöltött kódot hozzárendelhetjük a shader objektumainkhoz. */
	/** The loaded source codes are assigned to the shader objects. */
	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	/** Fordítsuk le a vertex shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the vertex shader object. */
	glCompileShader(vShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "Vertex compilation failed." << endl;
		printShaderLog(vShader);
	}

	/** Fordítsuk le a fragment shader objektumhoz rendelt kódot. */
	/** Let's compile the code of the fragment shader object. */
	glCompileShader(fShader);
	/** Hibakeresési lépések. Például sikeres volt-e a fordítás? Ha nem, mi volt az oka? */
	/** Error checking. Was the compile step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "Fragment compilation failed." << endl;
		printShaderLog(fShader);
	}

	/** Shader program objektum létrehozása: ez fogja össze a shadereket. Eltároljuk az ID értéket. */
	/** Shader program object initialization: holds together the shaders. vfProgram stores the ID. */
	GLuint		vfProgram = glCreateProgram();
	/** Csatoljuk a shadereket az előző lépésben létrehozott objektumhoz. */
	/** The shaders are attached to the program object. */
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	/** Végül a GLSL szerkesztő ellenőrzi, hogy a csatolt shaderek kompatibilisek-e egymással. */
	/** GLSL linker checks the shaders for compatibility. */
	glLinkProgram(vfProgram);
	/** Hibakeresési lépések. Például sikeres volt-e az összeszerkesztés? Ha nem, mi volt az oka? */
	/** Error checking. Was the link step successful? If not, what was the reason? */
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "Shader linking failed." << endl;
		printProgramLog(vfProgram);
	}

	/** Ha minden rendben ment a linkelés során, a shader objektumok törölhetőek. */
	/** If everything is OK at linking, the shader objects can be destroyed. */
	glDeleteShader(vShader);
	glDeleteShader(fShader);

	/** Az elkészült program azonosítója a visszatérési értékünk. */
	/** The program ID will be the return value. */
	return vfProgram;
}

void init(GLFWwindow* window) {
	/** A rajzoláshoz használt shader programok betöltése. */
	/** Loading the shader programs for rendering. */
	renderingProgram = createShaderProgram();

	/* Létrehozzuk a szükséges vertex buffer és vertex array objektumokat. */
	/* Create the vertex buffer and vertex array objects. */
	glGenBuffers(numVBOs, VBO);
	glGenVertexArrays(numVAOs, VAO);

	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a vertex buffert (ide kerülnek a csúcspont adatok). */
	/* We attach the vertex buffer to the GL_ARRAY_BUFFER node (vertices are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vertices), s_vertices, GL_STATIC_DRAW);
	/* std::array megadása paraméternek. */
	/* std::array as parameter. */
	// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	/* Csatoljuk a vertex array objektumunkat a shader programhoz. */
	/* Attach the vertex array object to the shader program. */
	glBindVertexArray(VAO[0]);
	/* Ezen adatok szolgálják a location = 0 vertex attribútumot (itt: pozíció).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő vertex adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 0 vertex attribute (position).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first vertex data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(0);

	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a color buffert (ide kerülnek a szín adatok). */
	/* We attach the color buffer to the GL_ARRAY_BUFFER node (color values are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_colors), s_colors, GL_STATIC_DRAW);
	/* Ezen adatok szolgálják a location = 1 vertex attribútumot (itt: szín).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő szín adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 1 vertex attribute (color).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first color data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 1 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 1 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(1);


	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	/** Aktiváljuk a shader-program objektumunkat az alapértelmezett fix csővezeték helyett. */
	/** Activate our shader-program object instead of the default fix pipeline. */
	//glUseProgram(renderingProgram);

	/** Állítsuk be a törlési színt az áttetszőségi értékkel együtt! [0.0, 1.0] */
	/** Set the clear color (red, green, blue, alpha), where alpha is transparency! [0.0, 1.0] */
	glClearColor(0.9, 0.9, 0.0, 1.0);
}

void init2(GLFWwindow* window) {
	/** A rajzoláshoz használt shader programok betöltése. */
	/** Loading the shader programs for rendering. */
	renderingProgram2 = createShaderProgram2();

	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a vertex buffert (ide kerülnek a csúcspont adatok). */
	/* We attach the vertex buffer to the GL_ARRAY_BUFFER node (vertices are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_line_vertices), s_line_vertices, GL_STATIC_DRAW);
	/* std::array megadása paraméternek. */
	/* std::array as parameter. */
	// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	/* Csatoljuk a vertex array objektumunkat a shader programhoz. */
	/* Attach the vertex array object to the shader program. */
	glBindVertexArray(VAO[1]);
	/* Ezen adatok szolgálják a location = 0 vertex attribútumot (itt: pozíció).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő vertex adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 0 vertex attribute (position).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first vertex data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(0);

	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void init3(GLFWwindow* window) {
	/** A rajzoláshoz használt shader programok betöltése. */
	/** Loading the shader programs for rendering. */
	renderingProgram3 = createShaderProgram3();

	/* A GL_ARRAY_BUFFER nevesített csatolóponthoz kapcsoljuk a vertex buffert (ide kerülnek a csúcspont adatok). */
	/* We attach the vertex buffer to the GL_ARRAY_BUFFER node (vertices are stored here). */
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	/* Másoljuk az adatokat a bufferbe! Megadjuk az aktuálisan csatolt buffert, azt hogy hány byte adatot másolunk,
	a másolandó adatot, majd a feldolgozás módját is meghatározzuk: most az adat nem változik a feltöltés után. */
	/* Copy the data to the buffer! First parameter is the currently attached buffer, second is the size of the buffer to be copied,
	third is the array of data, fourth is working mode: now the data can not be modified after this step. */
	glBufferData(GL_ARRAY_BUFFER, sizeof(s_vector_vertices), s_vector_vertices, GL_STATIC_DRAW);
	/* std::array megadása paraméternek. */
	/* std::array as parameter. */
	// glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	/* Csatoljuk a vertex array objektumunkat a shader programhoz. */
	/* Attach the vertex array object to the shader program. */
	glBindVertexArray(VAO[2]);
	/* Ezen adatok szolgálják a location = 0 vertex attribútumot (itt: pozíció).
	   Elsőként megadjuk ezt az azonosítószámot (vertexShader.glsl).
	   Utána az attribútum méretét (vec3, láttuk a shaderben).
	   Harmadik az adat típusa.
	   Negyedik az adat normalizálása, ez maradhat FALSE jelen példában.
	   Az attribútum értékek hogyan következnek egymás után? Milyen lépésköz után találom a következő vertex adatait?
	   Végül megadom azt, hogy honnan kezdődnek az értékek a pufferben. Most rögtön, a legelejétől veszem őket. */
	   /* These values are for location = 0 vertex attribute (position).
		  First is the location (vertexShader.glsl).
		  Second is attribute size (vec3, as in the shader).
		  Third is the data type.
		  Fourth defines whether data shall be normalized or not, this is FALSE for the examples of the course.
		  Fifth is the distance in bytes to the next vertex element of the array.
		  Last is the offset of the first vertex data of the buffer. Now it is the start of the array. */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	/* Engedélyezzük az imént definiált location = 0 attribútumot (vertexShader.glsl). */
	/* Enable the previously defined location = 0 attributum (vertexShader.glsl). */
	glEnableVertexAttribArray(0);

	/* Leválasztjuk a vertex array objektumot és a buffert is. */
	/* Detach the vertex array object and the buffer also. */
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void display(GLFWwindow* window, double currentTime) {
	/* Töröljük le a kiválasztott buffereket! */
	/* Let's clear the selected buffers! */
	glClear(GL_COLOR_BUFFER_BIT);

	if (centercoordinates[0] + radius >= -0.333333333 && centercoordinates[0] - radius <= 0.333333333 && abs(s_line_vertices[1] - centercoordinates[1]) < radius)
		szinvalt2();
	else
		szinvalt1();

	/* Csatoljuk fel a vertex array objektumot a shader programhoz. */
	/* Bind vertex array object to the shader program. */
	glBindVertexArray(VAO[0]);

	/** Ha pontokat rajzolunk, kísérletezzünk a pont méretének növelésével! */
	/** When drawing points, we can make experiments with different point sizes! */
	//glPointSize(3.0);
	/** A megadott adatok segítségével pontokat rajzolunk. */
	/** We draw points with the defined arrays. */
	//glDrawArrays(GL_POINTS, 0, 3);
	/** A megadott adatok segítségével háromszöget rajzolunk. */
	/** We draw triangle with the defined arrays. */
	glUniform3fv(centerLoc, 1, centercoordinates);
	//cout << centercoordinates[0] << endl;
	x += incX; // move the circle along x axis
	Y += incY;
	if (x + radius > 1.0f) incX = -0.01f * cos(szog); // switch to moving the circle to the left
	if (x - radius < -1.0f) incX = 0.01f * cos(szog); // switch to moving the triangle to the right
	if (Y + radius > 1.0f) incY = -0.01f * sin(szog); // switch to moving the circle to the left
	if (Y - radius < -1.0f) incY = 0.01f * sin(szog); // switch to moving the triangle to the right
	GLuint offsetLocX = glGetUniformLocation(renderingProgram, "offsetX"); // get ptr to "offset"
	glProgramUniform1f(renderingProgram, offsetLocX, x); // send value in "x" to "offset"
	GLuint offsetLocY = glGetUniformLocation(renderingProgram, "offsetY"); // get ptr to "offset"
	glProgramUniform1f(renderingProgram, offsetLocY, Y); // send value in "x" to "offset"
	centercoordinates[0] += incX;
	centercoordinates[1] += incY;

	//cout << x << endl;

	glUseProgram(renderingProgram);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 102);
	glBindVertexArray(0);

	glUseProgram(0);
	glBindVertexArray(VAO[1]);
	glUseProgram(renderingProgram2);

	glLineWidth(3.0);
	glDrawArrays(GL_LINES, 0, 2);

	/* Leválasztjuk a VAO-t, nehogy bármilyen érték felülíródjon. */
	/* Detach VAO for safety reasons, not to modify it accidentaly. */
	glBindVertexArray(0);
	glUseProgram(0);
	glBindVertexArray(VAO[2]);
	glUseProgram(renderingProgram3);
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 0, 2);
	//cout << s_vector_vertices[3] << endl;
	//cout << s_vector_vertices[4] << endl;

	glBindVertexArray(0);
}

/** Felesleges objektumok törlése. */
/** Clenup the unnecessary objects. */
void cleanUpScene() {
	/** Töröljük a shader programot. */
	/** Let's delete the shader program. */
	glDeleteProgram(renderingProgram);
	/** Töröljük a GLFW ablakot. */
	/** Destroy the GLFW window. */
	glfwDestroyWindow(window);
	/** Leállítjuk a GLFW-t. */
	/** Stop the GLFW system. */
	glfwTerminate();
	/** Kilépés EXIT_SUCCESS kóddal. */
	/** Stop the software and exit with EXIT_SUCCESS code. */
	exit(EXIT_SUCCESS);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	window_width = width;
	window_height = height;

	/** A kezelt képernyő beállítása a teljes (0, 0, width, height) területre. */
	/** Set the viewport for the full (0, 0, width, height) area. */
	glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	/** ESC billentyűre kilépés. */
	/** Exit on ESC key. */
	if ((action == GLFW_PRESS) && (key == GLFW_KEY_ESCAPE))
		cleanUpScene();

	/** A billentyűk lenyomásának és felengedésének regisztrálása. Lehetővé teszi gombkombinációk használatát. */
	/** Let's register press and release events for keys. Enables the usage of key combinations. */
	if (action == GLFW_PRESS)
		keyboard[key] = GL_TRUE;
	else if (action == GLFW_RELEASE)
		keyboard[key] = GL_FALSE;

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		y = 0.05;
		szakaszfrissit();
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		y = -0.05;
		szakaszfrissit();
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		szog = vektor_szog;
		incX = 0.01f * cos(szog);
		incY = 0.01f * sin(szog);

	}
}

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
	/** Az egér mutató helyét kezelő függvény. */
	/** Callback function for mouse position change. */
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	/** Az egér gombjaihoz köthető események kezelése. */
	/** Callback function for mouse button events. */
}

int main(void) {

	pontgeneral();
	vektorgeneral();
	/** Próbáljuk meg inicializálni a GLFW-t! */
	/** Try to initialize GLFW! */
	if (!glfwInit())
		exit(EXIT_FAILURE);

	/** A használni kívánt OpenGL verzió: 4.3. */
	/** The needed OpenGL version: 4.3. */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make macOS happy; should not be needed.
	/** Próbáljuk meg létrehozni az ablakunkat. */
	/** Let's try to create a window for drawing. */
	/** GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor * monitor, GLFWwindow * share) */
	window = glfwCreateWindow(window_width, window_height, window_title, nullptr, nullptr);

	/** Válasszuk ki az ablakunk OpenGL kontextusát, hogy használhassuk. */
	/** Select the OpenGL context (window) for drawing. */
	glfwMakeContextCurrent(window);

	/** A képernyő átméretezés kezelése. */
	/** Callback function for window size change. */
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	/** Billentyűzethez köthető események kezelése. */
	/** Callback function for keyboard events. */
	glfwSetKeyCallback(window, keyCallback);
	/** Az egér mutató helyét kezelő függvény megadása. */
	/** Callback function for mouse position change. */
	glfwSetCursorPosCallback(window, cursorPosCallback);
	/** Az egér gombjaihoz köthető események kezelése. */
	/** Callback function for mouse button events. */
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	/** Incializáljuk a GLEW-t, hogy elérhetővé váljanak az OpenGL függvények, probléma esetén kilépés EXIT_FAILURE értékkel. */
	/** Initalize GLEW, so the OpenGL functions will be available, on problem exit with EXIT_FAILURE code. */
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);

	/** 0 = v-sync kikapcsolva, 1 = v-sync bekapcsolva, n = n db képkockányi időt várakozunk */
	/** 0 = v-sync off, 1 = v-sync on, n = n pieces frame time waiting */
	glfwSwapInterval(1);

	/** A window ablak minimum és maximum szélességének és magasságának beállítása. */
	/** The minimum and maximum width and height values of the window object. */
	glfwSetWindowSizeLimits(window, 400, 400, 800, 800);
	/** A window oldalarányának megadása a számláló és az osztó segítségével. */
	/** Setting the aspect ratio using the numerator and the denominator values. */
	glfwSetWindowAspectRatio(window, 1, 1);

	/** Az alkalmazáshoz kapcsolódó előkészítő lépések, pl. a shader objektumok létrehozása. */
	/** The first initialization steps of the program, e.g.: creating the shader objects. */
	init(window);
	init2(window);
	init3(window);
	centercoordinates[0] = s_vertices[0];
	centercoordinates[1] = s_vertices[1];
	centercoordinates[2] = s_vertices[2];

	/** A megadott window struktúra "close flag" vizsgálata. */
	/** Checks the "close flag" of the specified window. */
	while (!glfwWindowShouldClose(window)) {
		/** A kód, amellyel rajzolni tudunk a GLFWwindow objektumunkba. */
		/** Call display function which will draw into the GLFWwindow object. */
		/*centercoordinates[0] = s_vertices[0];
		centercoordinates[1] = s_vertices[1];
		centercoordinates[2] = s_vertices[2];*/
		/*cout << s_vertices[309] << endl;
		cout << s_vertices[310] << endl;
		cout << centercoordinates[0] << endl;
		cout << centercoordinates[1] << endl;*/
		centerLoc = glGetUniformLocation(renderingProgram, "center");
		display(window, glfwGetTime());
		/** Double buffered működés. */
		/** Double buffered working = swap the front and back buffer here. */
		glfwSwapBuffers(window);
		/** Események kezelése az ablakunkkal kapcsolatban, pl. gombnyomás. */
		/** Handle events related to our window, e.g.: pressing a key or moving the mouse. */
		glfwPollEvents();
	}

	/** Felesleges objektumok törlése. */
	/** Clenup the unnecessary objects. */
	cleanUpScene();

	/** Kilépés EXIT_SUCCESS kóddal. */
	/** Stop the software and exit with EXIT_SUCCESS code. */
	return EXIT_SUCCESS;
}