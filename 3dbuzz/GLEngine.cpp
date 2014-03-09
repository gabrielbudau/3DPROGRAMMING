#include "GLEngine.h"

const int fontSize = 13;
const int fontSpace = 10;
GLEngine::GLEngine()
{

}
GLEngine::~GLEngine()
{

}
GLvoid GLEngine::Uninitialize(GLvoid)
{
	delete GLEngine::getEngine();
}
GLEngine *GLEngine:: getEngine(GLvoid)
{
	static GLEngine *engine = new GLEngine();

	return engine;
}
GLvoid GLEngine::initialize(GLint width, GLint height)
{
	Light::Initialise();

	fontTexture = new Texture("..\\Resources\\Textures\\font_3.tga", "Font Texture");
	buildTextureFont();
}
GLvoid GLEngine::buildTextureFont(GLvoid)
{
	fontBase = glGenLists(256);

	glBindTexture(GL_TEXTURE_2D, fontTexture->texID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glColor4f(.0, .0, .0, 1.0);
	for (int i = 0; i < 256; i++)
	{
		float cx = (float)(i % 16) / 16.0f;
		float cy = (float)(i / 16) / 16.0f;

		glNewList(fontBase + i, GL_COMPILE);

		glBegin(GL_QUADS);
		glTexCoord2f(cx, 1 - cy - (1.0f / 16.0f));glVertex2i(0, fontSize);
		glTexCoord2f(cx + (1.0f / 16.0f), 1 - cy - (1.0f / 16.0f));glVertex2i(fontSize, fontSize);
		glTexCoord2f(cx + (1.0f / 16.0f), 1 - cy);glVertex2i(fontSize, 0);
		glTexCoord2f(cx, 1 - cy); glVertex2i(0, 0);
		glEnd();

		glTranslated(fontSpace, 0, 0);

		glEndList();
	}
}
GLvoid GLEngine::drawText(GLint x, GLint y, const char *_text, ...)
{
	char text[256];
	va_list ap;
	va_start(ap, _text);
	vsprintf_s(text, _text, ap);
	va_end(ap);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTexture->texID);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	glTranslated(x, y, 0);

	glListBase(fontBase - 32);
	glCallLists((GLsizei)strlen(text), GL_BYTE, text);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}
GLuint GLEngine::getTextWidth(const char *text)
{
	return (GLuint)(strlen(text) + 1) * fontSpace;
}
GLuint GLEngine::getTextHeight(const char *text)
{
	return (GLuint)(fontSize);
}