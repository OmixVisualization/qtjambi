/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

import QtJambiGenerator 1.0

TypeSystem{
    packageName: "io.qt.opengl"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtOpenGL"
    module: "qtjambi.opengl"
    description: "Classes that make it easy to use OpenGL in Qt applications."

    PrimitiveType{
        name: "GLuint"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLshort"
        javaName: "short"
        jniName: "jshort"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLushort"
        javaName: "short"
        jniName: "jshort"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLint64"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLuint64"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLintptr"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLsizeiptr"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "qopengl_GLsizeiptr"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "qopengl_GLintptr"
        javaName: "long"
        jniName: "jlong"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLint"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLchar"
        javaName: "char"
        jniName: "jchar"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLenum"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLbitfield"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLsizei"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLboolean"
        javaName: "boolean"
        jniName: "jboolean"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLdouble"
        javaName: "double"
        jniName: "jdouble"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLfloat"
        javaName: "float"
        jniName: "jfloat"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLubyte"
        javaName: "byte"
        jniName: "jbyte"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLbyte"
        javaName: "byte"
        jniName: "jbyte"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLvoid"
        javaName: "void"
        jniName: "void"
        preferredConversion: false
    }
    
    PrimitiveType{
        name: "GLclampf"
        javaName: "float"
        jniName: "jfloat"
        preferredConversion: false
    }
    
    TemplateType{
        name: "QOpenGLESFunctions"
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "#ifndef GL_MAX_LABEL_LENGTH\n"+
                          "#define GL_MAX_LABEL_LENGTH 0x82E8\n"+
                          "#endif"}
        }
        ModifyFunction{
            signature: "glDeleteBuffers(GLsizei,const GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteFramebuffers(GLsizei,const GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteRenderbuffers(GLsizei,const GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteTextures(GLsizei,const GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenBuffers(GLsizei,GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenFramebuffers(GLsizei,GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenRenderbuffers(GLsizei,GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenTextures(GLsizei,GLuint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetBooleanv(GLenum,GLboolean*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetBufferParameteriv(GLenum,GLenum,GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetFloatv(GLenum,GLfloat*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetFramebufferAttachmentParameteriv(GLenum,GLenum,GLenum,GLint*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetIntegerv(GLenum,GLint*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetUniformfv(GLuint, GLint, GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetUniformiv(GLuint, GLint, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetRenderbufferParameteriv(GLenum, GLenum, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexParameterfv(GLenum, GLenum, GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexParameteriv(GLenum, GLenum, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glReadPixels(GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,GLvoid*)"
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexImage2D(GLenum,GLint,GLint,GLsizei,GLsizei,GLint,GLenum,GLenum,const GLvoid*)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexParameterfv(GLenum,GLenum,const GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexParameteriv(GLenum,GLenum,const GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexSubImage2D(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLenum,const GLvoid*)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawElements(GLenum, GLsizei, GLenum, const GLvoid*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetAttachedShaders(GLuint, GLsizei, GLsizei*, GLuint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetProgramiv(GLuint, GLenum, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetShaderPrecisionFormat(GLenum, GLenum, GLint*, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetShaderiv(GLuint, GLenum, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetUniformfv(GLuint, GLint, GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetUniformiv(GLuint, GLint, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetVertexAttribfv(GLuint, GLenum, GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetVertexAttribiv(GLuint, GLenum, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform1fv(GLint, GLsizei, const GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform1iv(GLint, GLsizei, const GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform2fv(GLint, GLsizei, const GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform2iv(GLint, GLsizei, const GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform3fv(GLint, GLsizei, const GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform3iv(GLint, GLsizei, const GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform4fv(GLint, GLsizei, const GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform4iv(GLint, GLsizei, const GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix2fv(GLint, GLsizei, GLboolean, const GLfloat*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix3fv(GLint, GLsizei, GLboolean, const GLfloat*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix4fv(GLint, GLsizei, GLboolean, const GLfloat*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexAttrib1fv(GLuint, const GLfloat*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexAttrib2fv(GLuint, const GLfloat*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexAttrib3fv(GLuint, const GLfloat*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexAttrib4fv(GLuint, const GLfloat*)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_until_4_5"
        ModifyFunction{
            signature: "glGetBooleanv(GLenum, GLboolean *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetDoublev(GLenum, GLdouble *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetFloatv(GLenum, GLfloat *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetIntegerv(GLenum, GLint *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetTexLevelParameterfv(GLenum, GLint, GLenum, GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexLevelParameteriv(GLenum, GLint, GLenum, GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexParameterfv(GLenum, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexParameteriv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetString(GLenum)"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "java.lang.@NonNull String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jstring>(%env, QLatin1String(reinterpret_cast<const char *>(%in)));"}
                }
            }
        }
        ModifyFunction{
            signature: "glTexParameterfv(GLenum,GLenum,const GLfloat*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexParameteriv(GLenum,GLenum,const GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_until_4_4"
        ModifyFunction{
            signature: "glReadPixels(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *)"
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexImage(GLenum, GLint, GLenum, GLenum, GLvoid *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexImage1D(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *)"
            ModifyArgument{
                index: 8
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexImage2D(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_since_4_4"
        ModifyFunction{
            signature: "glReadPixels(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, void *)"
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexImage(GLenum, GLint, GLenum, GLenum, void *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexImage1D(GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 8
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexImage2D(GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_until_3_1A"
        ModifyFunction{
            signature: "glPixelMapfv(GLenum, GLint, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glPixelMapuiv(GLenum, GLint, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glPixelMapusv(GLenum, GLint, const GLushort *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCallLists(GLsizei, GLenum, const GLvoid *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawPixels(GLsizei, GLsizei, GLenum, GLenum, const GLvoid *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_until_3_1B"
        ModifyFunction{
            signature: "glPixelMapfv(GLenum, GLsizei, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glPixelMapuiv(GLenum, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glPixelMapusv(GLenum, GLsizei, const GLushort *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCallLists(GLsizei, GLenum, const void *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawPixels(GLsizei, GLsizei, GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_until_3_1"
        ModifyFunction{
            signature: "glColor4ub(GLubyte,GLubyte,GLubyte,GLubyte)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor3ub(GLubyte,GLubyte,GLubyte)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor4ui(GLuint,GLuint,GLuint,GLuint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor3ui(GLuint,GLuint,GLuint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor4us(GLushort,GLushort,GLushort,GLushort)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor3us(GLushort,GLushort,GLushort)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor4ubv(const GLubyte *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor3ubv(const GLubyte *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor4uiv(const GLuint *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor3uiv(const GLuint *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor4usv(const GLushort *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glColor3usv(const GLushort *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glEdgeFlagv(const GLboolean *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glGetTexGendv(GLenum, GLenum, GLdouble *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexGenfv(GLenum, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexGeniv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetClipPlane(GLenum, GLdouble *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glGetMaterialfv(GLenum, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetMaterialiv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glClipPlane(GLenum, const GLdouble *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glGetPixelMapfv(GLenum, GLfloat *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glPolygonStipple(const GLubyte *)"
            ModifyArgument{
                index: 1
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glRectdv(const GLdouble *,const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glRectfv(const GLfloat *,const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glRectiv(const GLint *,const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glRectsv(const GLshort *,const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glBitmap(GLsizei, GLsizei, GLfloat, GLfloat, GLfloat, GLfloat, const GLubyte *)"
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glFogiv(GLenum, const GLint *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glFogfv(GLenum, const GLfloat *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glMultMatrixd(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 16
                }
            }
        }
        ModifyFunction{
            signature: "glMultMatrixf(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 16
                }
            }
        }
        ModifyFunction{
            signature: "glNormal3bv(const GLbyte *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glNormal3sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glNormal3iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glNormal3fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glNormal3dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glColor4bv(const GLbyte *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glColor3bv(const GLbyte *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glColor4sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glColor3sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glColor4iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glColor3iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glColor4fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glColor3fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glColor4dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glColor3dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos4sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos3sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos2sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos4iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos3iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos2iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos4fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos3fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos2fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos4dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos3dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glRasterPos2dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glLightModeliv(GLenum, const GLint *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glLightModelfv(GLenum, const GLfloat *)"
            ModifyArgument{
                index: 2
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetPolygonStipple(GLubyte *)"
            ModifyArgument{
                index: 1
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetLightfv(GLenum, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetLightiv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glLightiv(GLenum, GLenum, const GLint *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glLightfv(GLenum, GLenum, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glEvalCoord1fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glEvalCoord1dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glEvalCoord2fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glEvalCoord2dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glGetMapdv(GLenum, GLenum, GLdouble *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetMapfv(GLenum, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetMapiv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetPixelMapuiv(GLenum, GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetPixelMapusv(GLenum, GLushort *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glSelectBuffer(GLsizei,GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{
                    lengthParameter: 1
                }
            }
        }
        ModifyFunction{
            signature: "glMaterialiv(GLenum, GLenum, const GLint *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glMaterialfv(GLenum, GLenum, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glVertex4sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glVertex3sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glVertex2sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glVertex4iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glVertex3iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glVertex2iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glVertex4fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glVertex3fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glVertex2fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glVertex4dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glVertex3dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glVertex2dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord4sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord3sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord2sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord1sv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord4iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord3iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord2iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord1iv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord4fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord3fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord2fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord1fv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord4dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 4
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord3dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 3
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord2dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 2
                }
            }
        }
        ModifyFunction{
            signature: "glTexCoord1dv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glTexEnviv(GLenum, GLenum, const GLint *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glTexEnvfv(GLenum, GLenum, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glGetTexEnvfv(GLenum, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexEnviv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexGendv(GLenum, GLenum, const GLdouble *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glTexGenfv(GLenum, GLenum, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glTexGeniv(GLenum, GLenum, const GLint *)"
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glFeedbackBuffer(GLsizei, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{
                    lengthParameter: 1
                }
            }
        }
        ModifyFunction{
            signature: "glLoadMatrixd(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 16
                }
            }
        }
        ModifyFunction{
            signature: "glLoadMatrixf(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 16
                }
            }
        }
        ModifyFunction{
            signature: "glMap1d(GLenum, GLdouble, GLdouble, GLint, GLint, const GLdouble *)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glMap1f(GLenum, GLfloat, GLfloat, GLint, GLint, const GLfloat *)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glMap2d(GLenum, GLdouble, GLdouble, GLint, GLint, GLdouble, GLdouble, GLint, GLint, const GLdouble *)"
            ModifyArgument{
                index: 10
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glMap2f(GLenum, GLfloat, GLfloat, GLint, GLint, GLfloat, GLfloat, GLint, GLint, const GLfloat *)"
            ModifyArgument{
                index: 10
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glIndexdv(const GLdouble *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glIndexfv(const GLfloat *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glIndexiv(const GLint *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
        ModifyFunction{
            signature: "glIndexsv(const GLshort *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_deprecated"
        ModifyFunction{
            signature: "glAreTexturesResident(GLsizei, const GLuint *, GLboolean *)"
            ModifyArgument{
                index: 2
                AsBuffer{
                    lengthParameter: 1
                }
            }
            ModifyArgument{
                index: 3
                AsArray{
                    lengthParameter: 1
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "glPrioritizeTextures(GLsizei, const GLuint *, const GLfloat *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_deprecatedA"
        ModifyFunction{
            signature: "glColorPointer(GLint, GLenum, GLsizei, const GLvoid *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexPointer(GLint, GLenum, GLsizei, const GLvoid *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexCoordPointer(GLint, GLenum, GLsizei, const GLvoid *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glInterleavedArrays(GLenum, GLsizei, const GLvoid *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glNormalPointer(GLenum, GLsizei, const GLvoid *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glEdgeFlagPointer(GLsizei, const GLvoid *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glIndexPointer(GLenum, GLsizei, const GLvoid *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_deprecatedB"
        ModifyFunction{
            signature: "glColorPointer(GLint, GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexPointer(GLint, GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexCoordPointer(GLint, GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glInterleavedArrays(GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glNormalPointer(GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glEdgeFlagPointer(GLsizei, const void *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glIndexPointer(GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1"
        ModifyFunction{
            signature: "glGenTextures(GLsizei, GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteTextures(GLsizei, const GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1A"
        ModifyFunction{
            signature: "glGetPointerv(GLenum, GLvoid**)"
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  "                                );\n"+
                                  "if(%out.size()==0){\n"+
                                  "    JavaException::raiseIllegalArgumentException(%env, \"Array length is 0.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "glDrawElements(GLenum, GLsizei, GLenum, const GLvoid *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexSubImage2D(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexSubImage1D(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *)"
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1B"
        ModifyFunction{
            signature: "glDrawElements(GLenum, GLsizei, GLenum, const void *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexSubImage2D(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexSubImage1D(GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_until_4_4"
        ModifyFunction{
            signature: "glIndexubv(const GLubyte *)"
            ModifyArgument{
                index: 1
                AsArray{
                    minLength: 1
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1B_until_4_4"
        ModifyFunction{
            signature: "glGetPointerv(GLenum, void**)"
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  "                                );\n"+
                                  "if(%out.size()==0){\n"+
                                  "    JavaException::raiseIllegalArgumentException(%env, \"Array length is 0.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_4A"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "glMultiDrawElements(GLenum, const GLsizei *, GLenum, const GLvoid *const*, GLsizei)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<const void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                        JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                        %scope.addDeletion(bufferData);\n"+
                                  "                                        pointer = bufferData->data<void>();\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  "                                );\n"+
                                  "if(%out.size()==0){\n"+
                                  "    JavaException::raiseIllegalArgumentException(%env, \"Array length is 0.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_4B"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "glMultiDrawElements(GLenum, const GLsizei *, GLenum, const void *const*, GLsizei)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<const void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                        JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                        %scope.addDeletion(bufferData);\n"+
                                  "                                        pointer = bufferData->data<void>();\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  ");"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_5"
        ModifyFunction{
            signature: "glMapBuffer(GLenum, GLenum)"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "INT_MAX"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_2_0"
        ModifyFunction{
            signature: "glShaderSource(GLuint, GLsizei, const GLchar *const*, const GLint*)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector<QSharedPointer<J2CStringBuffer>> %inVec;\n"+
                                  "const char** %out = nullptr;\n"+
                                  "QScopedArrayPointer<const char*> %outPtr(%out = new const char*[quint32(__qt_%2)]);\n"+
                                  "for(jsize i=0; i<__qt_%2; ++i){\n"+
                                  "    J2CStringBuffer* b = new J2CStringBuffer(%env, jstring(%env->GetObjectArrayElement(jobjectArray(%in), i)));\n"+
                                  "    %out[i] = *b;\n"+
                                  "    %inVec << QSharedPointer<J2CStringBuffer>(b);\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out = %env->GetArrayLength(jarray(%3));"}
                }
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_5A"
        ModifyFunction{
            signature: "glGetBufferPointerv(GLenum, GLenum, GLvoid **)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  "                                );\n"+
                                  "if(%out.size()==0){\n"+
                                  "    JavaException::raiseIllegalArgumentException(%env, \"Array length is 0.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_5B"
        ModifyFunction{
            signature: "glGetBufferPointerv(GLenum, GLenum, void **)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  "                                );\n"+
                                  "if(%out.size()==0){\n"+
                                  "    JavaException::raiseIllegalArgumentException(%env, \"Array length is 0.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_2_0A"
        Import{
            template: "QOpenGLFunctions_2_0"
        }
        Import{
            template: "QOpenGLFunctions_1_4A"
        }
        Import{
            template: "QOpenGLFunctions_1_5"
        }
        Import{
            template: "QOpenGLFunctions_1_5A"
        }
        ModifyFunction{
            signature: "glGetVertexAttribPointerv(GLuint, GLenum, GLvoid **)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  ");"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_2_0B"
        ModifyFunction{
            signature: "glGetBufferPointerv(GLenum, GLenum, void **)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  "                                );\n"+
                                  "if(%out.size()==0){\n"+
                                  "    JavaException::raiseIllegalArgumentException(%env, \"Array length is 0.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_2_0B"
        Import{
            template: "QOpenGLFunctions_2_0"
        }
        Import{
            template: "QOpenGLFunctions_1_4B"
        }
        Import{
            template: "QOpenGLFunctions_1_5"
        }
        Import{
            template: "QOpenGLFunctions_1_5B"
        }
        ModifyFunction{
            signature: "glGetVertexAttribPointerv(GLuint, GLenum, void **)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  ");"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_3_0"
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "#ifndef GL_MAX_LABEL_LENGTH\n"+
                          "#define GL_MAX_LABEL_LENGTH 0x82E8\n"+
                          "#endif"}
        }
        ModifyFunction{
            signature: "glTransformFeedbackVaryings(GLuint, GLsizei, const GLchar *const *, GLenum)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out = %env->GetArrayLength(jarray(%3));"}
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector<QSharedPointer<J2CStringBuffer>> %inVec;\n"+
                                  "const GLchar** %out = nullptr;\n"+
                                  "QScopedArrayPointer<const GLchar*> %outPtr(%out = new const GLchar*[quint32(__qt_%2)]);\n"+
                                  "for(jsize i=0; i<__qt_%2; ++i){\n"+
                                  "    J2CStringBuffer* b = new J2CStringBuffer(%env, jstring(%env->GetObjectArrayElement(jobjectArray(%in), i)));\n"+
                                  "    %out[i] = *b;\n"+
                                  "    %inVec << QSharedPointer<J2CStringBuffer>(b);\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetTransformFeedbackVarying(GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %out = GL_MAX_LABEL_LENGTH;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %in = 0;\n"+
                                  "GLsizei* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 5
                AsArray{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 6
                AsArray{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 7
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out_buffer(GL_MAX_LABEL_LENGTH, '\\0');\n"+
                                  "char* %out = %out_buffer.data();\n"+
                                  "%scope.addFinalAction([&](){\n"+
                                  "        jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %4));\n"+
                                  "        %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "    });"}
                }
            }
            InjectCode{
                Text{content: "if (name.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'name': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
        }
        ModifyFunction{
            signature: "glGetStringi(GLenum, GLuint)"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "java.lang.@NonNull String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jstring>(%env, QLatin1String(reinterpret_cast<const char *>(%in)));"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetBooleani_v(GLenum, GLuint, GLboolean *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.IntBuffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBufferData %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "glMapBufferRange(GLenum, GLintptr, GLsizeiptr, GLbitfield)"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthParameter: 3
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_3_1"
        Import{
            template: "QOpenGLFunctions_3_0"
        }
        ModifyFunction{
            signature: "glGetUniformIndices(GLuint, GLsizei, const GLchar *const *, GLuint *)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out = qMin(%env->GetArrayLength(jarray(%3)), %env->GetArrayLength(jarray(%4)));"}
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector<QSharedPointer<J2CStringBuffer>> %inVec;\n"+
                                  "const char** %out = nullptr;\n"+
                                  "QScopedArrayPointer<const char*> %outPtr(%out = new const char*[quint32(__qt_%2)]);\n"+
                                  "for(jsize i=0; i<__qt_%2; ++i){\n"+
                                  "    J2CStringBuffer* b = new J2CStringBuffer(%env, jstring(%env->GetObjectArrayElement(jobjectArray(%in), i)));\n"+
                                  "    %out[i] = *b;\n"+
                                  "    %inVec << QSharedPointer<J2CStringBuffer>(b);\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JIntArrayPointer %out(%env, %in, true);"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_3_2"
        ModifyFunction{
            signature: "glGetSynciv(__GLsync *, GLenum, GLsizei, GLsizei *, GLint *)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            ModifyArgument{
                index: 4
                AsArray{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "java.nio.IntBuffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBufferData %out(%env, %in);\n"+
                                  "GLsizei %3 = GLsizei(%out.size());"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_3_2A"
        Import{
            template: "QOpenGLFunctions_3_2"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "glMultiDrawElementsBaseVertex(GLenum, const GLsizei *, GLenum, const GLvoid *const*, GLsizei, const GLint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<const void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                        JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                        %scope.addDeletion(bufferData);\n"+
                                  "                                        pointer = bufferData->data<void>();\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  ");"}
                }
            }
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_3_2B"
        Import{
            template: "QOpenGLFunctions_3_2"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "glMultiDrawElementsBaseVertex(GLenum, const GLsizei *, GLenum, const void *const*, GLsizei, const GLint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<const void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                        JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                        %scope.addDeletion(bufferData);\n"+
                                  "                                        pointer = bufferData->data<void>();\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  ");"}
                }
            }
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_4_1"
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        ModifyFunction{
            signature: "glGetProgramPipelineInfoLog(GLuint, GLsizei, GLsizei *, GLchar *)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %out = GL_MAX_LABEL_LENGTH;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %in = 0;\n"+
                                  "GLsizei* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out_buffer(GL_MAX_LABEL_LENGTH, '\\0');\n"+
                                  "char* %out = %out_buffer.data();\n"+
                                  "%scope.addFinalAction([&](){\n"+
                                  "        jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %3));\n"+
                                  "        %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "    });"}
                }
            }
            InjectCode{
                Text{content: "if (infoLog.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'infoLog': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
        }
        ModifyFunction{
            signature: "glCreateShaderProgramv(GLenum, GLsizei, const GLchar * const *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector<QSharedPointer<J2CStringBuffer>> %inVec;\n"+
                                  "const char** %out = nullptr;\n"+
                                  "QScopedArrayPointer<const char*> %outPtr(%out = new const char*[quint32(__qt_%2)]);\n"+
                                  "for(jsize i=0; i<__qt_%2; ++i){\n"+
                                  "    J2CStringBuffer* b = new J2CStringBuffer(%env, jstring(%env->GetObjectArrayElement(jobjectArray(%in), i)));\n"+
                                  "    %out[i] = *b;\n"+
                                  "    %inVec << QSharedPointer<J2CStringBuffer>(b);\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out = %env->GetArrayLength(jarray(%3));"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_4_3"
        Import{
            template: "QOpenGLFunctions_4_1"
        }
        ModifyFunction{
            signature: "glGetProgramResourceName(GLuint, GLenum, GLuint, GLsizei, GLsizei *, GLchar *)"
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %out = GL_MAX_LABEL_LENGTH;"}
                }
            }
            ModifyArgument{
                index: 5
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %in = 0;\n"+
                                  "GLsizei* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 6
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out_buffer(GL_MAX_LABEL_LENGTH, '\\0');\n"+
                                  "char* %out = %out_buffer.data();\n"+
                                  "%scope.addFinalAction([&](){\n"+
                                  "        jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %5));\n"+
                                  "        %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "    });"}
                }
            }
            InjectCode{
                Text{content: "if (name.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'name': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_4_4"
        Import{
            template: "QOpenGLFunctions_4_3"
        }
        ModifyFunction{
            signature: "glDebugMessageCallback(GLDEBUGPROC, const void *)"
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const void * %out = nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetDebugMessageLog(GLuint, GLsizei, GLenum *, GLenum *, GLuint *, GLenum *, GLsizei *, GLchar *)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLuint %out = GLuint(%env->GetArrayLength(jarray(%3)));\n"+
                                  "%out = qMin(%out, GLuint(%env->GetArrayLength(jarray(%4))));\n"+
                                  "%out = qMin(%out, GLuint(%env->GetArrayLength(jarray(%5))));\n"+
                                  "%out = qMin(%out, GLuint(%env->GetArrayLength(jarray(%8))));"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %out = GL_MAX_LABEL_LENGTH;"}
                }
            }
            ModifyArgument{
                index: 3
                AsArray{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 4
                AsArray{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 5
                AsArray{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 6
                AsArray{
                    minLength: 1
                }
            }
            ModifyArgument{
                index: 7
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QScopedArrayPointer<GLsizei> %in(new GLsizei[__qt_%1]);\n"+
                                  "GLsizei* %out = %in.get();"}
                }
            }
            ModifyArgument{
                index: 8
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out_buffer(GL_MAX_LABEL_LENGTH, '\\0');\n"+
                                  "char* %out = %out_buffer.data();"}
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = jint(%in);\n"+
                                  "int offset = 0;\n"+
                                  "for(jsize i=0; i<jsize(__qt_return_value); ++i){\n"+
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(__qt_%8+offset, __qt_%7[i]));\n"+
                                  "    offset += __qt_%7[i];\n"+
                                  "    %env->SetObjectArrayElement(%8, i, result);\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetObjectPtrLabel(const void *, GLsizei, GLsizei *, GLchar *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.gui.gl.GLsync"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const void* %out = QtJambiAPI::convertJavaObjectToNative(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %out = GL_MAX_LABEL_LENGTH;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %in = 0;\n"+
                                  "GLsizei* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out_buffer(GL_MAX_LABEL_LENGTH, '\\0');\n"+
                                  "char* %out = %out_buffer.data();\n"+
                                  "%scope.addFinalAction([&](){\n"+
                                  "        jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %3));\n"+
                                  "        %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "    });"}
                }
            }
            InjectCode{
                Text{content: "if (label.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'label': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
        }
        ModifyFunction{
            signature: "glGetObjectLabel(GLenum, GLuint, GLsizei, GLsizei *, GLchar *)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %out = GL_MAX_LABEL_LENGTH;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "GLsizei %in = 0;\n"+
                                  "GLsizei* %out = &%in;"}
                }
            }
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray %out_buffer(GL_MAX_LABEL_LENGTH, '\\0');\n"+
                                  "char* %out = %out_buffer.data();\n"+
                                  "%scope.addFinalAction([&](){\n"+
                                  "        jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %4));\n"+
                                  "        %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "    });"}
                }
            }
            InjectCode{
                Text{content: "if (label.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'label': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
        }
        ModifyFunction{
            signature: "glObjectPtrLabel(const void *, GLsizei, const GLchar *)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.gui.gl.GLsync"
                }
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const void* %out = QtJambiAPI::convertJavaObjectToNative(%env, %in);"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, %in);\n"+
                                  "GLsizei %2(%out.length());"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_4_5"
        Import{
            template: "QOpenGLFunctions_4_4"
        }
        ModifyFunction{
            signature: "glMapNamedBuffer(GLuint, GLenum)"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "INT_MAX"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "glMapNamedBufferRange(GLuint,GLintptr,GLsizei,GLbitfield)"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "INT_MAX"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "glGetNamedBufferPointerv(GLuint, GLenum, void **)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  ");"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_full"
        Import{
            template: "QOpenGLFunctions_1_0_template_until_3_1"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_3_1A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_4"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_0_template_full2"
        Import{
            template: "QOpenGLFunctions_1_0_template_until_3_1"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_3_1B"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_since_4_4"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_template_full"
        Import{
            template: "QOpenGLFunctions_1_1"
        }
        Import{
            template: "QOpenGLFunctions_1_1_until_4_4"
        }
        Import{
            template: "QOpenGLFunctions_1_1A"
        }
        Import{
            template: "QOpenGLFunctions_1_1_deprecated"
        }
        Import{
            template: "QOpenGLFunctions_1_1_deprecatedA"
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_template_full2"
        Import{
            template: "QOpenGLFunctions_1_1"
        }
        Import{
            template: "QOpenGLFunctions_1_1B"
        }
        Import{
            template: "QOpenGLFunctions_1_1_until_4_4"
        }
        Import{
            template: "QOpenGLFunctions_1_1B_until_4_4"
        }
        Import{
            template: "QOpenGLFunctions_1_1_deprecated"
        }
        Import{
            template: "QOpenGLFunctions_1_1_deprecatedB"
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_template_until_4_5"
        Import{
            template: "QOpenGLFunctions_1_1"
        }
        Import{
            template: "QOpenGLFunctions_1_1A"
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_1_1_template_until_4_5B"
        Import{
            template: "QOpenGLFunctions_1_1"
        }
        Import{
            template: "QOpenGLFunctions_1_1B"
        }
    }
    
    RequiredLibrary{
        name: "libGLESv2"
        mode: RequiredLibrary.Optional
        platforms: ["windows"]
    }
    
    RequiredLibrary{
        name: "libEGL"
        mode: RequiredLibrary.Optional
        platforms: ["windows"]
    }
    
    RequiredLibrary{
        name: "opengl32sw"
        mode: RequiredLibrary.Optional
        platforms: ["windows"]
    }
    
    RequiredLibrary{
        name: "d3dcompiler_47"
        mode: RequiredLibrary.Optional
        platforms: ["windows"]
    }
    
    Rejection{
        className: "QOpenGLFunctions_*Backend"
    }
    
    Rejection{
        className: "PFNGL*"
    }
    
    Rejection{
        className: "GLDEBUGPROCAMD"
    }
    
    Rejection{
        className: "GLDEBUGPROCARB"
    }
    
    Rejection{
        className: "GLVULKANPROCNV"
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsStorage"
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsBackend"
        enumName: "Version"
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsBackend"
        fieldName: "refs"
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsBackend"
    }
    
    Rejection{
        className: "QOpenGLExtraFunctionsPrivate"
    }
    
    Rejection{
        className: "QOpenGLFunctionsPrivate"
    }
    
    Rejection{
        className: "QAbstractOpenGLFunctionsPrivate"
    }
    
    Rejection{
        className: "QOpenGLTextureHelper"
    }
    
    ValueType{
        name: "QOpenGLBuffer"

        EnumType{
            name: "Access"
        }

        EnumType{
            name: "Type"
        }

        EnumType{
            name: "RangeAccessFlag"
        }

        EnumType{
            name: "UsagePattern"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "allocate(const void *, int)"
            ModifyArgument{
                index: 1
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "read(int, void *, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "write(int, const void *, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "map(QOpenGLBuffer::Access)"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "__qt_this->size()"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "mapRange(int, int, QOpenGLBuffer::RangeAccessFlags)"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "qMin<qsizetype>(count1, __qt_this->size()-offset0)"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
    }
    
    ObjectType{
        name: "QOpenGLDebugLogger"

        EnumType{
            name: "LoggingMode"
        }
    }
    
    ValueType{
        name: "QOpenGLDebugMessage"

        EnumType{
            name: "Severity"
            RejectEnumValue{
                name: "LastSeverity"
            }
        }

        EnumType{
            name: "Source"
            RejectEnumValue{
                name: "LastSource"
            }
        }

        EnumType{
            name: "Type"
            RejectEnumValue{
                name: "LastType"
            }
        }
    }
    
    ValueType{
        name: "QOpenGLFramebufferObjectFormat"
    }
    
    ObjectType{
        name: "QOpenGLFramebufferObject"

        EnumType{
            name: "Attachment"
        }

        EnumType{
            name: "FramebufferRestorePolicy"
        }
    }
    
    ObjectType{
        name: "QOpenGLVersionFunctionsFactory"
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "QAbstractOpenGLFunctions* qtjambi_QOpenGLVersionFunctionsFactory_get(JNIEnv *__jni_env, jclass type, QOpenGLContext* context);"}
        }
        ModifyFunction{
            signature: "get<TYPE>(QOpenGLContext *)"
            Instantiation{
                proxyCall: "qtjambi_QOpenGLVersionFunctionsFactory_get"
                Argument{
                    type: "QAbstractOpenGLFunctions"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "T"
                        modifiedJniType: "jobject"
                    }
                }
                AddTypeParameter{
                    name: "T"
                    extending: "io.qt.opengl.QAbstractOpenGLFunctions"
                }
                AddArgument{
                    index: 1
                    name: "type"
                    type: "java.lang.Class<T>"
                }
            }
        }
        ModifyFunction{
            signature: "get(const QOpenGLVersionProfile &, QOpenGLContext *)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "QAbstractOpenGLFunctions"
        ModifyFunction{
            signature: "setOwningContext(const QOpenGLContext*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "QAbstractOpenGLFunctions()"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_0"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_1_0>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_1"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_1_1>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_2"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_1_2>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_3"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_1_3>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_4"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_1_4>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_4A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_5"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_1_5>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_4A"
        }
        Import{
            template: "QOpenGLFunctions_1_5A"
        }
        Import{
            template: "QOpenGLFunctions_1_5"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_2_0"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_2_0>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_2_1"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_2_1>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_0"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_3_0>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_0"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_1"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_3_1>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_2_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_3_2_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_2_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_3_2_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_3_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_3_3_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_3_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_3_3_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_0_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_0_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_0_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_0_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_1"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_1_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_1_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        Import{
            template: "QOpenGLFunctions_4_1"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_1_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_1_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_4_1"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_2_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_2_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        Import{
            template: "QOpenGLFunctions_4_1"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_2_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_2_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_4_1"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_3_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_3_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        Import{
            template: "QOpenGLFunctions_4_3"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_3_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_3_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_3_2A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_4_3"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_4_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_4_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0B"
        }
        Import{
            template: "QOpenGLFunctions_3_2B"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full2"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full2"
        }
        Import{
            template: "QOpenGLFunctions_4_4"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_4_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_0_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0B"
        }
        Import{
            template: "QOpenGLFunctions_3_2B"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5B"
        }
        Import{
            template: "QOpenGLFunctions_4_4"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_5_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_5_Compatibility>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0B"
        }
        Import{
            template: "QOpenGLFunctions_3_2B"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full2"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full2"
        }
        Import{
            template: "QOpenGLFunctions_4_5"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_5_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_4_5_Core>)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_2_0B"
        }
        Import{
            template: "QOpenGLFunctions_3_2B"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_until_4_5"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_until_4_5B"
        }
        Import{
            template: "QOpenGLFunctions_4_5"
        }
    }
    
    ObjectType{
        name: "QOpenGLFunctions_ES2"
        ppCondition: "QT_CONFIG(opengles2) && __has_include(<QtOpenGL/QOpenGLFunctions_ES2>)"
        targetType: "final class"
        ModifyFunction{
            signature: "glBindAttribLocation(GLuint,GLuint,const GLchar*)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
        }
        ModifyFunction{
            signature: "glGetVertexAttribPointerv(GLuint, GLenum, GLvoid **)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                    [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                        pointer = nullptr;\n"+
                                  "                                    },\n"+
                                  "                                    [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                        return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                    }\n"+
                                  ");"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetString(GLenum)"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "java.lang.@NonNull String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = qtjambi_cast<jstring>(%env, QLatin1String(reinterpret_cast<const char *>(%in)));"}
                }
            }
        }
        ModifyFunction{
            signature: "glBufferData(GLenum,GLsizeiptr,const GLvoid*,GLenum)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glBufferSubData(GLenum,GLintptr,GLsizeiptr,const GLvoid*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCompressedTexImage2D(GLenum,GLint,GLenum,GLsizei,GLsizei,GLint,GLsizei,const GLvoid*)"
            ModifyArgument{
                index: 8
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCompressedTexSubImage2D(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLsizei,const GLvoid*)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetActiveAttrib(GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetActiveUniform(GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, GLchar*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetAttribLocation(GLuint, const GLchar*)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
        }
        ModifyFunction{
            signature: "glGetProgramInfoLog(GLuint, GLsizei, GLsizei*, GLchar*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetShaderInfoLog(GLuint, GLsizei, GLsizei*, GLchar*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glShaderBinary(GLsizei,const GLuint*,GLenum,const GLvoid*,GLsizei)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glShaderSource(GLuint, GLsizei, const GLchar**, const GLint*)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector<QSharedPointer<J2CStringBuffer>> %inVec;\n"+
                                  "const char** %out = nullptr;\n"+
                                  "QScopedArrayPointer<const char*> %outPtr(%out = new const char*[quint32(__qt_%2)]);\n"+
                                  "for(jsize i=0; i<__qt_%2; ++i){\n"+
                                  "    J2CStringBuffer* b = new J2CStringBuffer(%env, jstring(%env->GetObjectArrayElement(jobjectArray(%in), i)));\n"+
                                  "    %out[i] = *b;\n"+
                                  "    %inVec << QSharedPointer<J2CStringBuffer>(b);\n"+
                                  "}"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out = %env->GetArrayLength(jarray(%3));"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const GLint * %out = nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "glVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetUniformLocation(GLuint, const GLchar*)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
            }
        }
        ModifyFunction{
            signature: "glGetShaderSource(GLuint, GLsizei, GLsizei*, GLchar*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        Import{
            template: "QOpenGLESFunctions"
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QOpenGLFunctions_ES2___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QOpenGLPaintDevice"
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QOpenGLShader"

        EnumType{
            name: "ShaderTypeBit"
        }
        ModifyFunction{
            signature: "compileSourceCode ( const char *)"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QOpenGLShaderProgram"
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtJambiGui/hashes.h"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "addCacheableShaderFromSourceCode(QOpenGLShader::ShaderType, const char *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "uniformLocation(const char *) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValue(int, GLuint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValue(const char*, GLuint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValue(int, Array)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValue(const char*, Array)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const GLuint *, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const GLuint *, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addShaderFromSourceCode(QOpenGLShader::ShaderType, const char *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "attributeLocation(const char *)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "bindAttributeLocation(const char *, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "addShader(QOpenGLShader*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSharers"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeShader(QOpenGLShader*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcSharers"
                    action: ReferenceCount.Take
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QOpenGLShaderProgram___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(int, const GLfloat *, int, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(const char *, const GLfloat *, int, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(int, const QVector2D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(int, const QVector3D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(int, const QVector4D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(const char *, const QVector2D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(const char *, const QVector3D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(const char *, const QVector4D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(int, GLenum, const void *, int, int)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(const char *, GLenum, const void *, int, int)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setAttributeValue(int, const GLfloat *, int, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setAttributeValue ( const char * , const GLfloat * , int , int )"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const GLuint *, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const GLint *, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const GLfloat *, int, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QVector2D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QVector3D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QVector4D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix4x4 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix2x2 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix3x2 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix4x2 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix2x3 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix3x3 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix4x3 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix2x4 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(int, const QMatrix3x4 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const GLuint *, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const GLint *, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const GLfloat *, int, int)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QVector2D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QVector3D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QVector4D *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix4x4 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix2x2 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix3x2 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix4x2 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix2x3 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix3x3 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix4x3 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix2x4 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "setUniformValueArray(const char*, const QMatrix3x4 *, int)"
            ModifyArgument{
                index: 2
                AsArray{
                    lengthParameter: 3
                    noOffset: true
                }
            }
        }
    }
    
    ObjectType{
        name: "QOpenGLTimeMonitor"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2)"
    }
    
    ValueType{
        name: "QOpenGLVersionProfile"
    }
    
    ObjectType{
        name: "QOpenGLVertexArrayObject"
        ObjectType{
            name: "Binder"
            implementing: "AutoCloseable"
            InjectCode{
                target: CodeClass.Java
                Text{content: "@Override\n"+
                              "@QtUninvokable\n"+
                              "public final void close(){\n"+
                              "    dispose();\n"+
                              "}"}
            }
        }
    }
    
    ObjectType{
        name: "QOpenGLTextureBlitter"
        EnumType{
            name: "Origin"
        }
        ExtraIncludes{
            Include{
                fileName: "hashes.h"
                location: Include.Global
                suppressed: true
            }
            Include{
                fileName: "QtJambiCore/hashes.h"
                location: Include.Global
            }
        }
    }
    
    ValueType{
        name: "QOpenGLVersionStatus"

        EnumType{
            name: "OpenGLStatus"
        }
        CustomConstructor{
            Text{content: "return new(placement) QOpenGLVersionStatus(copy->version.first, copy->version.second, copy->status);"}
        }
    }
    
    ObjectType{
        name: "QOpenGLTexture"
        EnumType{
            name: "BindingTarget"
        }

        EnumType{
            name: "DepthStencilMode"
        }

        EnumType{
            name: "CoordinateDirection"
        }

        EnumType{
            name: "Feature"
        }

        EnumType{
            name: "CubeMapFace"
        }

        EnumType{
            name: "Filter"
        }

        EnumType{
            name: "MipMapGeneration"
        }

        EnumType{
            name: "PixelFormat"
        }

        EnumType{
            name: "PixelType"
        }

        EnumType{
            name: "SwizzleComponent"
        }

        EnumType{
            name: "SwizzleValue"
        }

        EnumType{
            name: "Target"
        }

        EnumType{
            name: "TextureFormat"
        }

        EnumType{
            name: "TextureUnitReset"
        }

        EnumType{
            name: "WrapMode"
        }

        EnumType{
            name: "TextureFormatClass"
        }

        EnumType{
            name: "ComparisonFunction"
            RejectEnumValue{
                name: "CommpareNotEqual"
                since: [6, 1]
            }
        }

        EnumType{
            name: "ComparisonMode"
        }
        ModifyFunction{
            signature: "createTextureView(QOpenGLTexture::Target, QOpenGLTexture::TextureFormat, int, int, int, int) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "borderColor(int *) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "borderColor(float *) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "borderColor(unsigned int *) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setBorderColor(uint, uint, uint, uint)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setData(int, int, int, int, int, int, int, int, QOpenGLTexture::CubeMapFace, int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions *const)"
            ModifyArgument{
                index: 13
                AsBuffer{}
            }
            since: [5, 14]
        }
        ModifyFunction{
            signature: "setData(int, int, int, int, int, int, int, int, QOpenGLTexture::CubeMapFace, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions *const)"
            ModifyArgument{
                index: 12
                AsBuffer{}
            }
            since: [5, 14]
        }
        ModifyFunction{
            signature: "setData(int, int, int, int, int, int, int, int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions *const)"
            ModifyArgument{
                index: 11
                AsBuffer{}
            }
            since: [5, 14]
        }
        ModifyFunction{
            signature: "setData(int, int, int, int, int, int, int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions *const)"
            ModifyArgument{
                index: 10
                AsBuffer{}
            }
            since: [5, 14]
        }
        ModifyFunction{
            signature: "setData(int, int, int, int, int, int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions *const)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
            since: [5, 14]
        }
        ModifyFunction{
            signature: "setData(int, int, QOpenGLTexture::CubeMapFace, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setData(int, int, int, QOpenGLTexture::CubeMapFace, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setData(int, int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setData(int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setData(QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, const void *, const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setCompressedData(int, int, int, QOpenGLTexture::CubeMapFace, int, const void *,const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setCompressedData(int, int, QOpenGLTexture::CubeMapFace, int, const void *,const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setCompressedData(int, int, int, const void *,const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setCompressedData(int, int, const void *,const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "setCompressedData(int, const void *,const QOpenGLPixelTransferOptions * const)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
    }
    
    ValueType{
        name: "QOpenGLPixelTransferOptions"
    }
    
    ObjectType{
        name: "QOpenGLTimerQuery"
        ppCondition: "!defined(QT_NO_OPENGL) && !QT_CONFIG(opengles2)"
    }
    
    ObjectType{
        name: "QOpenGLWindow"

        EnumType{
            name: "UpdateBehavior"
        }
        ModifyFunction{
            signature: "paintGL()"
            isPaintMethod: true
        }
    }
    
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QOpenGLBuffer."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QOpenGLPixelTransferOptions."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'glGetPointerv(GLenum,GLvoid**)' for function modification in '*' not found.*"}
}
