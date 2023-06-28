#pragma once
#include <Shader/Shader.h>
#include <ft2build.h>
#include <map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include FT_FREETYPE_H

//Handles Textbuffer and font initialization and offers renderfunction for easy Textrendering 
class TextRenderer
{
public:
	TextRenderer(int fontSize, int w_width, int w_height);
	void Draw(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color);

private:
	FT_Library ft;
	FT_Face face;
	int fontSize;

	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	std::map<char, Character> Characters;

	glm::mat4 projection;

	//Buffer
	unsigned int VAO;
	unsigned int VBO;

	void initFont();
	void initBuffer();
};

