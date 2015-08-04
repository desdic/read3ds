
#include <string.h>
#include <stdio.h>
#include <GL/glew.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include "shader.h"

Shader::Shader() {
    program = -1;
    programset = false;
    name = "unknown";
}

Shader::Shader(std::string name_) {
    program = -1;
    programset = false;
    name = name_;
}

Shader::~Shader() {
}

char *Shader::textFileRead(const char *fn) {
    FILE *fp;
    long count;
    char *content=NULL;
    
    fp = fopen(fn, "r");

    if (fp==NULL) {
        return NULL;
    }
    
    fseek(fp, 0, SEEK_END);
    count = ftell(fp);

    fclose(fp);
    
    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {
            if (count > 0) {
                content = new char[ count+1 ];
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

int Shader::compile(const char *src, GLuint type) {

    GLint linked;
    if (programset==false) {
        program = glCreateProgram();
        programset=true;
    }

    if (src!=NULL) {
        GLuint v = glCreateShader( type ); // GL_VERTEX_SHADER || GL_FRAGMENT_SHADER
        
        glShaderSource(v, 1, &src,NULL);
        glCompileShader(v);
        printShaderInfoLog(v);

        glAttachShader(program,v);
        glLinkProgram(program);
        printProgramInfoLog(program);
        glGetProgramiv(program, GL_LINK_STATUS, &linked);

        printProgramInfoLog(program);

        if (linked) {
            return 0;
        }
    }    
    return -11;
}

int Shader::LoadFromMemory(const char *var, GLuint type) {

    return compile(var, type);


}

int Shader::LoadFromFile(std::string file, GLuint type) {

    printf("Loading shader %s", file.c_str());

    if (programset==false) {
        program = glCreateProgram();
        programset=true;
    }

    int status = -1;
    if (file.size()!=0) {
        
        const char *var = textFileRead(file.c_str());

        status = compile(var, type);

        delete [] var;

        if (status==0) {
            printf("   [OK]\n");
            return status;
        }
    }
    
    printf("   [ERROR]\n");
    return status;
}

void Shader::Begin() {
    glUseProgram(program);
}

void Shader::End() {
    glUseProgram(0);
}


void Shader::printShaderInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);

    if (infologLength > 1)
    {
        infoLog = new char[infologLength];
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("ShaderInfoLog: %s\n", infoLog);
        delete [] infoLog;
    }
}

void Shader::printProgramInfoLog(GLuint obj) {
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
    
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
    
    if (infologLength > 1)
    {
        infoLog = new char[infologLength];
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("ProgramInfoLog: %s\n",infoLog);
        delete [] infoLog;
    }
}

GLint Shader::GetUniformLocation(const GLcharARB *name) {

    GLint loc;
    loc = glGetUniformLocation(program, name);
    if (loc==-1) {
        printf("Cannot find uniform variable %s\n", name);
    }
    return loc;
}

bool Shader::setUniform1f(GLcharARB *name, GLfloat v0, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform1f(loc, v0);
    return true;
}

bool Shader::setUniform2f(GLcharARB *name, GLfloat v0, GLfloat v1, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform2f(loc, v0,v1);
    return true;
}

bool Shader::setUniform3f(GLcharARB *name, GLfloat v0, GLfloat v1, GLfloat v2, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform3f(loc, v0,v1,v2);
    return true;
}

bool Shader::setUniform4f(GLcharARB *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform4f(loc, v0,v1,v2,v3);
    return true;
}

bool Shader::setUniform1fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform1fv(loc, count, value);
    return true;
}


bool Shader::setUniform2fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform2fv(loc, count, value);
    return true;
}


bool Shader::setUniform3fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform3fv(loc, count, value);
    return true;
}


bool Shader::setUniform4fv(GLcharARB *name, GLsizei count, GLfloat *value, GLint index) {
    GLint loc;
    
    if (name) {
        loc = GetUniformLocation(name);
    } else {
        loc = index;
    }

    if (loc==-1) 
        return false;

    glUniform4fv(loc, count, value);
    return true;
}


const std::string Shader::GetName() const {
    return name;
}
