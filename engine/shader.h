#ifndef __SHADERS_H__
#define __SHADERS_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>

class Shader {
public:
    Shader();
    Shader(std::string name_);
    ~Shader();
    int LoadFromFile(std::string file, GLuint type);
    int LoadFromMemory(const char *var, GLuint type);
    void Begin();
    void End();

    bool setUniform1f(GLcharARB *name, GLfloat v0, GLint index=-1);
    bool setUniform2f(GLcharARB *name, GLfloat v0, GLfloat v1, GLint index=-1);
    bool setUniform3f(GLcharARB *name, GLfloat v0, GLfloat v1, GLfloat v2, GLint index=-1);
    bool setUniform4f(GLcharARB *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index=-1);

    bool setUniform1fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index=-1);
    bool setUniform2fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index=-1);
    bool setUniform3fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index=-1);
    bool setUniform4fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index=-1);

    const std::string GetName() const;

private:
    GLint GetUniformLocation(const GLcharARB *name);

    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    int compile(const char *src, GLuint type);
    char *textFileRead(const char *fn);
    GLuint program;
    bool programset;
    std::string name;
};

#endif
