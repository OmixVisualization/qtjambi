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
    packageName: "io.qt.gui"
    defaultSuperClass: "QtObject"
    qtLibrary: "QtGui"
    module: "qtjambi"    
    LoadTypeSystem{name: "QtCore"}

    InjectCode{
        target: CodeClass.MetaInfo
        position: Position.Position1
        Text{content: "void initialize_meta_info_gui();"}
    }

    InjectCode{
        target: CodeClass.MetaInfo
        Text{content: "initialize_meta_info_gui();"}
    }

    Rejection{
        className: "QNativeInterface::Private::QInterfaceProxyImp"
        since: [6,5]
    }
    
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );\n"+
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );\n"+
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
                                  "                                          [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                              JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                              %scope.addDeletion(bufferData);\n"+
                                  "                                              pointer = bufferData->data<void>();\n"+
                                  "                                          },\n"+
                                  "                                          [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                              return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                          }\n"+
                                  "                                      );\n"+
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
                                  "                                          [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                              JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                              %scope.addDeletion(bufferData);\n"+
                                  "                                              pointer = bufferData->data<void>();\n"+
                                  "                                          },\n"+
                                  "                                          [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                              return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                          }\n"+
                                  "                                      );"}
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );\n"+
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );\n"+
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );\n"+
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %4));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (name.length < 1)\n"+
                              "throw new IllegalArgumentException(\"Argument 'name': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
        }
        ModifyFunction{
            signature: "glGetStringi(GLenum, GLuint)"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "java.lang.String"
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
                NoNullPointer{
                }
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
                    Text{content: "JIntArrayPointer %out(%env, %in);"}
                }
            }
        }
    }
    
    TemplateType{
        name: "QOpenGLFunctions_3_2"
        ModifyFunction{
            signature: "glFenceSync(GLenum,GLbitfield)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
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
                                  "                                [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                    JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                    %scope.addDeletion(bufferData);\n"+
                                  "                                    pointer = bufferData->data<void>();\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
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
                                  "                                [&%scope](const void* & pointer,JNIEnv *env, jobject o){\n"+
                                  "                                    JBufferData* bufferData = new JBufferData(env, o);\n"+
                                  "                                    %scope.addDeletion(bufferData);\n"+
                                  "                                    pointer = bufferData->data<void>();\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, const void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %3));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (infoLog.length < 1)\n"+
                              "throw new IllegalArgumentException(\"Argument 'infoLog': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %5));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (name.length < 1)\n"+
                              "throw new IllegalArgumentException(\"Argument 'name': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %3));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (label.length < 1)\n"+
                              "throw new IllegalArgumentException(\"Argument 'label': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %4));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (label.length < 1)\n"+
                              "throw new IllegalArgumentException(\"Argument 'label': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
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
    
    Template{
        name: "gui.matrix_constructor"
        Text{content: "private static java.nio.FloatBuffer wrap(float[] values){\n"+
                      "    if(values.length < %COUNT)\n"+
                      "        throw new ArrayIndexOutOfBoundsException(%COUNT);\n"+
                      "    return java.nio.FloatBuffer.wrap(values);\n"+
                      "}\n"+
                      "\n"+
                      "public %TYPE(float[] values){\n"+
                      "    this(wrap(values));\n"+
                      "}\n"+
                      "\n"+
                      "@QtUninvokable\n"+
                      "public final void copyDataTo(float[] values)    {\n"+
                      "    copyDataTo(wrap(values));\n"+
                      "}"}
    }
    
    Template{
        name: "gui.getter_returning_nativepointer"
        Text{content: "public final %RETURN_TYPE %FUNCTION_NAME() {\n"+
                      "    QNativePointer np = %FUNCTION_NAME_private();\n"+
                      "    %RETURN_TYPE tmp = np == null || np.isNull() ? null : np.object(%RETURN_TYPE.class);\n"+
                      "    return tmp == null ? null : new %RETURN_TYPE(tmp);\n"+
                      "}"}
    }
    
    Template{
        name: "gui.convert_validationdata_to_java"
        Text{content: "jstring __qt_converted_input = qtjambi_cast<jstring>(%env, %STRING);\n"+
                      "jobject %out = Java::QtGui::QValidator$QValidationData::newInstance(%env, __qt_converted_input, %POS);\n"+
                      "jobject __java_validation_data = %out;"}
    }
    
    Template{
        name: "gui.cleanup_validationdata_to_java"
        Text{content: "jstring __java_string = jstring(Java::QtGui::QValidator$QValidationData::string(%env, __java_validation_data));\n"+
                      "%STRING = qtjambi_cast<QString>(%env, __java_string);\n"+
                      "%POS = Java::QtGui::QValidator$QValidationData::position(%env, __java_validation_data);"}
    }
    
    Template{
        name: "gui.convert_validationdata_to_string"
        Text{content: "jstring __java_string = Java::QtGui::QValidator$QValidationData::string(%env, %in);\n"+
                      "QString %out = qtjambi_cast<QString>(%env, __java_string);\n"+
                      "QString *__string_ptr = &%out;"}
    }
    
    Template{
        name: "gui.convert_validationdata_to_pos"
        Text{content: "int %out = Java::QtGui::QValidator$QValidationData::position(%env, %1);\n"+
                      "int *__position_ptr = &%out;"}
    }
    
    Template{
        name: "gui.cleanup_validationdata_from_java"
        Text{content: "Java::QtGui::QValidator$QValidationData::set_position(%env, %1, *__position_ptr);\n"+
                      "Java::QtGui::QValidator$QValidationData::set_string(%env, %1, qtjambi_cast<jstring>(%env, *__string_ptr));"}
    }
    
    Template{
        name: "gui.tabarray.check"
        Text{content: "if(tabarray!=null && tabarray.length>0) {\n"+
                      "    int i = tabarray.length-1;\n"+
                      "    for (; i >= 0; --i) {\n"+
                      "        if(tabarray[i]==0)\n"+
                      "            break;\n"+
                      "    }\n"+
                      "    if(i==0) {\n"+
                      "        tabarray = java.util.Arrays.copyOf(tabarray, tabarray.length+1);\n"+
                      "    }\n"+
                      "}"}
    }
    
    TemplateType{
        name: "QMatrixXxX"
        ModifyFunction{
            signature: "operator()(int, int)"
            rename: "setValue"
            AddArgument{
                name: "value"
                type: "float"
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "__qt_return_value = value;"}
            }
        }
        ModifyFunction{
            signature: "operator()(int, int)const"
            rename: "value"
        }
        ModifyFunction{
            signature: "operator*=(float)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator/=(float)"
            rename: "divide"
        }
    }
    
    PrimitiveType{
        name: "QAccessible::Id"
        javaName: "int"
        jniName: "jint"
        preferredConversion: false
    }
    
    RequiredLibrary{
        name: "libGLESv2"
        mode: RequiredLibrary.Optional
        platforms: ["windows"]
        until: 5
    }
    
    RequiredLibrary{
        name: "libEGL"
        mode: RequiredLibrary.Optional
        platforms: ["windows"]
        until: 5
    }
    
    RequiredLibrary{
        name: "QtANGLE"
        mode: RequiredLibrary.Optional
        platforms: ["windows"]
        until: [5, 14]
    }
    
    RequiredLibrary{
        name: "QtPrintSupport"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["macos"]
        until: 5
    }
    
    RequiredLibrary{
        name: "QtSvg"
        mode: RequiredLibrary.ProvideOnly
    }
    
    RequiredLibrary{
        name: "QtDBus"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["macos", "linux"]
    }
    
    RequiredLibrary{
        name: "QtNetwork"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
        until: 5
    }

    RequiredLibrary{
        name: "QtNetwork"
        mode: RequiredLibrary.ProvideOnly
        since: 6
    }

    RequiredLibrary{
        name: "QtOpenGL"
        mode: RequiredLibrary.ProvideOnly
        since: 6
    }

    RequiredLibrary{
        name: "QtPdf"
        mode: RequiredLibrary.ProvideOnly
        since: 6
    }
    
    RequiredLibrary{
        name: "QtWaylandClient"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
    }
    
    RequiredLibrary{
        name: "QtWaylandEglClientHwIntegration"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
        since: 6
    }
    
    RequiredLibrary{
        name: "QtWlShellIntegration"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
    }
    
    RequiredLibrary{
        name: "QtOpenGL"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
    }
    
    RequiredLibrary{
        name: "QtWidgets"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["macos", "linux"]
        until: 5
    }

    RequiredLibrary{
        name: "QtEglFSDeviceIntegration"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
    }

    RequiredLibrary{
        name: "QtXcbQpa"
        mode: RequiredLibrary.ProvideOnly
        platforms: ["linux"]
    }
    
    Rejection{
        className: "*"
        functionName: "d_func"
    }
    
    Rejection{
        className: "*"
        functionName: "d_ptr"
    }
    
    Rejection{
        className: "*"
        fieldName: "d_ptr"
    }
    
    Rejection{
        className: "*"
        fieldName: "d"
    }
    
    Rejection{
        className: "QPlatformBackingStore"
    }
    
    Rejection{
        className: "QPlatformNativeInterface"
    }
    
    Rejection{
        className: "QPlatformMenu"
    }
    
    Rejection{
        className: "QPlatformMenuBar"
    }
    
    Rejection{
        className: "QPlatformOffscreenSurface"
    }
    
    Rejection{
        className: "QPlatformOpenGLContext"
    }
    
    Rejection{
        className: "QPagedPaintDevicePrivate"
    }
    
    Rejection{
        className: "QPlatformPixmap"
    }
    
    Rejection{
        className: "QPlatformScreen"
    }
    
    Rejection{
        className: "QPlatformWindow"
    }
    
    Rejection{
        className: "QTextDocumentPrivate"
    }
    
    Rejection{
        className: "QPostEventList"
    }
    
    Rejection{
        className: "QRgbaF"
    }
    
    Rejection{
        className: "QRgbaFloat"
    }
    
    Rejection{
        className: "QRgbaFloat16"
    }
    
    Rejection{
        className: "QRgbaFloat32"
    }
    
    Rejection{
        className: "QTextEngine"
    }
    
    Rejection{
        className: "QGuiApplication"
        functionName: "nativeInterface"
        since: [6, 2]
    }
    
    Rejection{
        className: "QBrushDataPointerDeleter"
        since: [6, 2]
    }
    
    Rejection{
        className: "QEglFSFunctions::SwitchLangType"
    }
    
    Rejection{
        className: "QLinuxFbFunctions::LoadKeymapType"
    }
    
    Rejection{
        className: "QLinuxFbFunctions::SwitchLangType"
    }
    
    Rejection{
        className: "QCocoaNativeContext"
    }
    
    Rejection{
        className: "QEGLNativeContext"
    }
    
    Rejection{
        className: "QGLXNativeContext"
    }
    
    Rejection{
        className: "QWGLNativeContext"
    }
    
    Rejection{
        className: "QInputMethodQueryEvent::QueryPair"
    }
    
    Rejection{
        className: "QImageCleanupFunction"
    }
    
    Rejection{
        className: "QAccessible2"
    }
    
    Rejection{
        className: "QAccessibleActionInterface"
        functionName: "tr"
    }
    
    Rejection{
        className: "QAccessibleActionInterface"
        functionName: "trUtf8"
    }
    
    Rejection{
        className: "QAccessibleInterfaceEx"
        functionName: "interface_cast"
    }
    
    Rejection{
        className: "QBrushData"
    }
    
    Rejection{
        className: "QGradient::QGradientData"
    }
    
    Rejection{
        className: "QDesktopServices::StandardLocation"
    }
    
    Rejection{
        className: "QDesktopServices"
        functionName: "storageLocation"
    }
    
    Rejection{
        className: "QDesktopServices"
        functionName: "displayName"
    }
    
    Rejection{
        className: "QGenericMatrix"
    }
    
    Rejection{
        className: "QIconEngine::AvailableSizesArgument"
    }
    
    Rejection{
        className: "QIconEngine::IconEngineHook"
    }
    
    Rejection{
        className: "QImageTextKeyLang"
    }
    
    Rejection{
        className: "QPaintEngine"
        functionName: "fix_neg_rect"
    }
    
    Rejection{
        className: "QPainterPathPrivate"
    }
    
    Rejection{
        className: "QPenPrivate"
    }
    
    Rejection{
        className: "QPixmapFilterPrivate"
    }

    Rejection{
        className: "QRegionData"
    }
    
    Rejection{
        className: "QSupportedWritingSystems"
    }
    
    Rejection{
        className: "QTabletEvent"
        fieldName: "mExtra"
    }
    
    Rejection{
        className: "QTextFrameLayoutData"
    }
    
    Rejection{
        className: "QWheelEvent"
        enumName: "enum_1"
    }
    
    
    Rejection{
        className: "QTouchEvent"
        fieldName: "_window"
    }
    
    Rejection{
        className: "QTouchEvent"
        fieldName: "_target"
    }
    
    Rejection{
        className: "QTouchEvent"
        fieldName: "_device"
    }
    
    Rejection{
        className: "QTouchEvent"
        fieldName: "_touchPointStates"
    }
    
    Rejection{
        className: "QTouchEvent"
        fieldName: "_touchPoints"
    }
    
    Rejection{
        className: "QEglFSFunctions"
        functionName: "vsp2AddBlendListener"
    }
    
    Rejection{
        className: "QImageReader"
        functionName: "tr"
    }
    
    Rejection{
        className: "QImageReader"
        functionName: "trUtf8"
    }
    
    Rejection{
        className: "QImageWriter"
        functionName: "tr"
    }
    
    Rejection{
        className: "QImageWriter"
        functionName: "trUtf8"
    }
    
    Rejection{
        className: "QSurface"
        functionName: "surfaceHandle"
    }
    
    Rejection{
        className: "QOffscreenSurface"
        functionName: "surfaceHandle"
    }
    
    Rejection{
        className: "QVulkanInstance"
        since: [5, 10]
    }
    
    Rejection{
        className: "QVulkanWindow"
        since: [5, 10]
    }
    
    Rejection{
        className: "QVulkanWindowRenderer"
        since: [5, 10]
    }
    
    Rejection{
        className: "QActionGroup"
        functionName: "selected"
        since: 6
    }
    
    EnumType{
        name: "QAction::ActionEvent"
        since: 6
    }
    
    EnumType{
        name: "QAction::MenuRole"
        since: 6
    }
    
    EnumType{
        name: "QAction::Priority"
        since: 6
    }
    
    EnumType{
        name: "QActionGroup::ExclusionPolicy"
        since: 6
    }
    
    EnumType{
        name: "QContextMenuEvent::Reason"
    }
    
    EnumType{
        name: "QInputMethodEvent::AttributeType"
    }
    
    EnumType{
        name: "QPagedPaintDevice::PdfVersion"
        since: [5, 10]
    }
    
    
    EnumType{
        name: "QDoubleValidator::Notation"
    }
    
    EnumType{
        name: "QFont::Capitalization"
    }
    
    EnumType{
        name: "QFont::HintingPreference"
    }
    
    
    EnumType{
        name: "QFont::SpacingType"
    }
    
    EnumType{
        name: "QFont::Stretch"
    }
    
    EnumType{
        name: "QFont::Style"
    }
    
    EnumType{
        name: "QFont::StyleStrategy"
    }
    
    EnumType{
        name: "QFont::Weight"
    }
    
    EnumType{
        name: "QFont::StyleHint"
        RejectEnumValue{
            name: "SansSerif"
        }
        RejectEnumValue{
            name: "Serif"
        }
        RejectEnumValue{
            name: "TypeWriter"
        }
        RejectEnumValue{
            name: "Decorative"
        }
    }
    
    EnumType{
        name: "QFont::ResolveProperties"
    }
    
    EnumType{
        name: "QFontDatabase::WritingSystem"
        RejectEnumValue{
            name: "Other"
        }
    }
    
    EnumType{
        name: "QFontDatabase::SystemFont"
    }
    
    EnumType{
        name: "QIcon::Mode"
    }
    
    EnumType{
        name: "QIcon::State"
    }
    
    EnumType{
        name: "QImage::Format"
    }
    
    EnumType{
        name: "QImage::InvertMode"
    }
    
    EnumType{
        name: "QImageIOHandler::ImageOption"
    }
    
    EnumType{
        name: "QImageIOHandler::Transformation"
    }
    
    EnumType{
        name: "QImageReader::ImageReaderError"
    }
    
    EnumType{
        name: "QImageWriter::ImageWriterError"
    }
    
    EnumType{
        name: "QImageIOPlugin::Capability"
    }
    
    EnumType{
        name: "QInputMethod::Action"
    }
    
    EnumType{
        name: "QKeySequence::SequenceFormat"
    }
    
    EnumType{
        name: "QKeySequence::SequenceMatch"
    }
    
    EnumType{
        name: "QKeySequence::StandardKey"
    }
    
    EnumType{
        name: "QMovie::CacheMode"
    }
    
    EnumType{
        name: "QMovie::MovieState"
    }
    
    EnumType{
        name: "QSurface::SurfaceType"
    }
    
    EnumType{
        name: "QPaintDevice::PaintDeviceMetric"
    }
    
    EnumType{
        name: "QPaintEngine::DirtyFlag"
    }
    
    EnumType{
        name: "QPaintEngine::PaintEngineFeature"
    }
    
    EnumType{
        name: "QPaintEngine::PolygonDrawMode"
    }
    
    EnumType{
        name: "QPaintEngine::Type"
        upperBound: "QPaintEngine.MaxUser"
        lowerBound: "QPaintEngine.User"
        extensible: true
    }
    
    EnumType{
        name: "QPagedPaintDevice::PageSize"
        RejectEnumValue{
            name: "AnsiA"
        }
        RejectEnumValue{
            name: "AnsiB"
        }
        RejectEnumValue{
            name: "EnvelopeC5"
        }
        RejectEnumValue{
            name: "EnvelopeDL"
        }
        RejectEnumValue{
            name: "Envelope10"
        }
        RejectEnumValue{
            name: "LastPageSize"
        }
        RejectEnumValue{
            name: "NPageSize"
        }
        RejectEnumValue{
            name: "NPaperSize"
        }
    }
    
    EnumType{
        name: "QStandardItem::ItemType"
    }
    
    EnumType{
        name: "QStaticText::PerformanceHint"
    }
    
    EnumType{
        name: "QSurfaceFormat::FormatOption"
    }
    
    EnumType{
        name: "QSurfaceFormat::OpenGLContextProfile"
    }
    
    EnumType{
        name: "QSurfaceFormat::RenderableType"
    }
    
    EnumType{
        name: "QSurfaceFormat::SwapBehavior"
    }
    
    EnumType{
        name: "QTabletEvent::PointerType"
    }
    
    EnumType{
        name: "QTabletEvent::TabletDevice"
    }
    
    EnumType{
        name: "QTextBlockFormat::LineHeightTypes"
    }
    
    EnumType{
        name: "QTextBlockFormat::MarkerType"
        since: [5, 14]
    }
    
    EnumType{
        name: "QTextCharFormat::UnderlineStyle"
    }
    
    EnumType{
        name: "QTextCharFormat::VerticalAlignment"
    }
    
    EnumType{
        name: "QTextCursor::MoveMode"
    }
    
    EnumType{
        name: "QTextCursor::MoveOperation"
    }
    
    EnumType{
        name: "QTextCursor::SelectionType"
    }
    
    EnumType{
        name: "QTextDocument::FindFlag"
    }
    
    EnumType{
        name: "QTextDocument::MetaInformation"
    }
    
    EnumType{
        name: "QTextDocument::ResourceType"
    }
    
    EnumType{
        name: "QTextDocument::Stacks"
    }
    
    EnumType{
        name: "QTextDocument::MarkdownFeature"
        since: [5, 14]
    }
    
    EnumType{
        name: "QTextFormat::ObjectTypes"
    }
    
    EnumType{
        name: "QTextFormat::PageBreakFlag"
    }
    
    EnumType{
        name: "QTextFrameFormat::Position"
    }
    
    EnumType{
        name: "QTextFrameFormat::BorderStyle"
    }
    
    EnumType{
        name: "QTextItem::RenderFlag"
    }
    
    EnumType{
        name: "QTextLayout::CursorMode"
    }
    
    EnumType{
        name: "QTextLength::Type"
    }
    
    EnumType{
        name: "QTextLine::CursorPosition"
    }
    
    EnumType{
        name: "QTextLine::Edge"
    }
    
    EnumType{
        name: "QTextListFormat::Style"
    }
    
    EnumType{
        name: "QTextOption::Flag"
    }
    
    EnumType{
        name: "QTextOption::WrapMode"
    }
    
    EnumType{
        name: "QTextOption::TabType"
    }
    
    EnumType{
        name: "QTextFormat::FormatType"
    }
    
    EnumType{
        name: "QTextFormat::Property"
        RejectEnumValue{
            name: "FontSizeIncrement"
        }
        RejectEnumValue{
            name: "FirstFontProperty"
        }
        RejectEnumValue{
            name: "LastFontProperty"
        }
        RejectEnumValue{
            name: "OldFontFamily"
            since: [6, 4]
        }
    }
    
    EnumType{
        name: "QTouchDevice::CapabilityFlag"
        until: 5
    }
    
    EnumType{
        name: "QTouchDevice::DeviceType"
        until: 5
    }
    
    EnumType{
        name: "QValidator::State"
    }
    
    EnumType{
        name: "QWindow::AncestorMode"
    }
    
    EnumType{
        name: "QWindow::Visibility"
    }
    
    EnumType{
        name: "QTouchEvent::TouchPoint::InfoFlag"
        until: 5
    }
    
    EnumType{
        name: "QSessionManager::RestartHint"
    }
    
    EnumType{
        name: "QScrollEvent::ScrollState"
    }
    
    EnumType{
        name: "QIconEngine::IconEngineHook"
    }
    
    EnumType{
        name: "QPalette::ColorGroup"
        RejectEnumValue{
            name: "Normal"
        }
    }
    
    EnumType{
        name: "QPalette::ColorRole"
        RejectEnumValue{
            name: "Foreground"
        }
        RejectEnumValue{
            name: "Background"
        }
    }
    
    EnumType{
        name: "QColor::NameFormat"
    }
    
    EnumType{
        name: "QTextCharFormat::FontPropertiesInheritanceBehavior"
    }
    
    EnumType{
        name: "QPageSize::PageSizeId"
        RejectEnumValue{
            name: "Letter"
        }
        RejectEnumValue{
            name: "C5E"
        }
        RejectEnumValue{
            name: "Ledger"
        }
        RejectEnumValue{
            name: "DLE"
        }
        RejectEnumValue{
            name: "Comm10E"
        }
        RejectEnumValue{
            name: "LastPageSize"
        }
        RejectEnumValue{
            name: "NPageSize"
        }
        RejectEnumValue{
            name: "NPaperSize"
        }
    }
    
    EnumType{
        name: "QPageSize::SizeMatchPolicy"
    }
    
    EnumType{
        name: "QPageSize::Unit"
    }
    
    ValueType{
        name: "QPageLayout"
        EnumType{
            name: "Mode"
        }
        EnumType{
            name: "Orientation"
        }
        EnumType{
            name: "Unit"
        }
    }
    
    NamespaceType{
        name: "QColorConstants"

        NamespaceType{
            name: "Svg"
        }
    }
    
    EnumType{
        name: "QSurfaceFormat::ColorSpace"
    }
    
    
    ObjectType{
        name: "QAccessible"

        EnumType{
            name: "Event"
        }

        EnumType{
            name: "RelationFlag"
        }

        EnumType{
            name: "Role"
            RejectEnumValue{
                name: "PushButton"
            }
        }

        InterfaceType{
            name: "ActivationObserver"
        }

        EnumType{
            name: "InterfaceType"
        }

        EnumType{
            name: "TextBoundaryType"
        }

        ValueType{
            name: "State"
        }

        EnumType{
            name: "Text"
        }

        FunctionalType{
            name: "InterfaceFactory"
            ExtraIncludes{
                Include{
                    fileName: "QAccessible"
                    location: Include.Global
                }
                since: [6, 4]
            }
        }

        FunctionalType{
            name: "RootObjectHandler"
            ExtraIncludes{
                Include{
                    fileName: "QAccessible"
                    location: Include.Global
                }
                since: [6, 4]
            }
        }

        FunctionalType{
            name: "UpdateHandler"
            ExtraIncludes{
                Include{
                    fileName: "QAccessible"
                    location: Include.Global
                }
                since: [6, 4]
            }
        }

        ModifyFunction{
            signature: "cleanup()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "registerAccessibleInterface(QAccessibleInterface *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "installActivationObserver(QAccessible::ActivationObserver*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcActivationObserver"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeActivationObserver(QAccessible::ActivationObserver*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcActivationObserver"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setRootObject(QObject *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "queryAccessibleInterface(QObject *)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "accessibleInterface(QAccessible::Id)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QAccessibleBridgePlugin"
        ModifyFunction{
            signature: "create(QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            target: CodeClass.MetaInfo
            position: Position.Position1
            Text{content: "registerInterfaceID(typeId, QAccessibleBridgeFactoryInterface_iid);"}
        }
    }
    
    ObjectType{
        name: "QIconEnginePlugin"
        ModifyFunction{
            signature: "create(QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            target: CodeClass.MetaInfo
            position: Position.Position1
            Text{content: "registerInterfaceID(typeId, QIconEngineFactoryInterface_iid);"}
        }
    }
    
    InterfaceType{
        name: "QIconEngineFactoryInterface"
    }
    
    InterfaceType{
        name: "QUrlResourceProvider"
        ModifyFunction{
            signature: "setDefaultProvider(QUrlResourceProvider *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDefaultProvider"
                    action: ReferenceCount.Set
                }
            }
        }
        since: [6, 1]
    }
    
    ObjectType{
        name: "QAccessibleBridge"
        ModifyFunction{
            signature: "setRootObject(QAccessibleInterface *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "notifyAccessibilityUpdate(QAccessibleEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    
    
    InterfaceType{
        name: "QAccessibleInterface"
        Rejection{
            functionName: "cast_helper"
        }
        Rejection{
            functionName: "virtual_hook"
        }
        Rejection{
            functionName: "interface_cast"
        }
        ModifyFunction{
            signature: "indexOfChild(const QAccessibleInterface*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "actionInterface()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "editableTextInterface()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "imageInterface()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "tableCellInterface()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "tableInterface()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "textInterface()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "valueInterface()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "focusChild()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "parent()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "childAt(int,int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "child(int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QAccessibleSelectionInterface"
        ModifyFunction{
            signature: "isSelected(QAccessibleInterface*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "select(QAccessibleInterface*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "unselect(QAccessibleInterface*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        since: [6,5]
    }
    
    ObjectType{
        name: "QAccessibleObject"
    }
    
    ObjectType{
        name: "QAccessibleApplication"
    }
    
    InterfaceType{
        name: "QAccessibleTableCellInterface"
        ModifyFunction{
            signature: "table()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QAccessibleTableInterface"
        ModifyFunction{
            signature: "modelChange(QAccessibleTableModelChangeEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "caption()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "cellAt(int,int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "summary()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QAccessibleEditableTextInterface"
    }
    
    InterfaceType{
        name: "QAccessibleSimpleEditableTextInterface"
    }
    
    InterfaceType{
        name: "QAccessibleTextInterface"
        ModifyFunction{
            signature: "attributes(int, int *, int *) const"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "selection(int, int *, int *) const"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "textAfterOffset(int, QAccessible::TextBoundaryType, int *, int *) const"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "textAtOffset(int, QAccessible::TextBoundaryType, int *, int *) const"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
        ModifyFunction{
            signature: "textBeforeOffset(int, QAccessible::TextBoundaryType, int *, int *) const"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize %out_size = 1;\n"+
                                  "int* %out = qtjambi_array_cast<int*>(%env, %scope, jintArray(%in), %out_size);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "jintArray %out = qtjambi_array_cast<jintArray>(%env, %scope, %in, %in ? 1 : 0);"}
                }
            }
        }
    }
    
    InterfaceType{
        name: "QAccessibleValueInterface"
    }
    
    InterfaceType{
        name: "QAccessibleActionInterface"
    }
    
    InterfaceType{
        name: "QAccessibleImageInterface"
    }
    
    InterfaceType{
        name: "QAccessibleHyperlinkInterface"
        since: [6, 2]
    }
    
    ObjectType{
        name: "QAccessiblePlugin"
        ModifyFunction{
            signature: "create(QString, QObject*)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QInputMethodEvent"

        ValueType{
            name: "Attribute"
            Include{
                fileName: "QInputMethodEvent"
                location: Include.Global
            }
            CustomConstructor{
                Text{content: "if(copy){\n"+
                              "    return new(placement) QInputMethodEvent::Attribute(copy->type, copy->start, copy->length, copy->value);\n"+
                              "}else{\n"+
                              "    return new(placement) QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 0, 0);\n"+
                              "}"}
            }
            CustomConstructor{
                type: CustomConstructor.Copy
                Text{content: "new(placement) QInputMethodEvent::Attribute(copy->type, copy->start, copy->length, copy->value);"}
            }
            CustomConstructor{
                type: CustomConstructor.Default
                Text{content: "new(placement) QInputMethodEvent::Attribute(QInputMethodEvent::TextFormat, 0, 0);"}
            }
        }
        ModifyFunction{
            signature: "operator=(const QInputMethodEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QActionEvent"
        ModifyFunction{
            signature: "operator=(const QActionEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
        InjectCode{
            target: CodeClass.Native
            Text{content: "class QAction;"}
        }
        ModifyFunction{
            signature: "QActionEvent(int, QAction *, QAction *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "io.qt.widgets.QAction"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QAction* %out = static_cast<QAction*>(QtJambiAPI::convertJavaObjectToQObject(%env, %in));"}
                }
            }
            ModifyArgument{
                index: 3
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QAction* %out = static_cast<QAction*>(QtJambiAPI::convertJavaObjectToQObject(%env, %in));"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "action() const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, \"io/qt/widgets/QAction\");"}
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "before() const"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = QtJambiAPI::convertQObjectToJavaObject(%env, %in, \"io/qt/widgets/QAction\");"}
                }
            }
            until: 5
        }
    }
    
    ObjectType{
        name: "QNativeGestureEvent"
        ModifyFunction{
            signature: "operator=(const QNativeGestureEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyField{
            name: "mGestureType"
            read: false
            write: true
            rename: "gestureType"
        }
        ModifyField{
            name: "mLocalPos"
            read: false
            write: true
            rename: "localPos"
        }
        ModifyField{
            name: "mWindowPos"
            read: false
            write: true
            rename: "windowPos"
        }
        ModifyField{
            name: "mScreenPos"
            read: false
            write: true
            rename: "screenPos"
        }
        ModifyField{
            name: "mRealValue"
            read: false
            write: true
            rename: "value"
        }
        ModifyField{
            name: "mSequenceId"
            read: false
            write: true
            rename: "sequenceId"
        }
        ModifyField{
            name: "mIntValue"
            read: false
            write: true
            rename: "intValue"
        }
        ModifyFunction{
            signature: "device()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "QNativeGestureEvent(Qt::NativeGestureType, const QPointingDevice *, int, QPointF, QPointF, QPointF, qreal, QPointF, quint64)"
            ModifyArgument{
                index: 9
                ReplaceDefaultExpression{
                    expression: "Long.MAX_VALUE"
                }
            }
            since: [6, 2]
        }
    }
    
    ObjectType{
        name: "QCloseEvent"
        ModifyFunction{
            signature: "operator=(const QCloseEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: [6, 4]
        }
    }
    
    ObjectType{
        name: "QContextMenuEvent"
        ModifyField{
            name: "p"
            read: false
            write: true
            rename: "pos"
        }
        ModifyField{
            name: "gp"
            read: false
            write: true
            rename: "globalPos"
        }
        ModifyField{
            name: "reas"
            read: false
            write: true
            rename: "reason"
        }
        ModifyFunction{
            signature: "operator=(const QContextMenuEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QInputEvent"
        ModifyFunction{
            signature: "modifiers()const"
            access: Modification.NonFinal
        }
        ModifyField{
            name: "modState"
            read: false
            write: false
        }
        ModifyField{
            name: "ts"
            read: false
            write: false
        }
        ModifyField{
            name: "m_timeStamp"
            read: false
            write: false
        }
        ModifyField{
            name: "m_reserved"
            read: false
            write: false
        }
        ModifyField{
            name: "m_modState"
            read: false
            write: false
        }
        ModifyField{
            name: "m_dev"
            read: false
            write: true
            rename: "device"
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcDevice"
            }
        }
        ModifyFunction{
            signature: "operator=(const QInputEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QInputMethodQueryEvent"
        ModifyFunction{
            signature: "operator=(const QInputMethodQueryEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QMouseEvent"
        ModifyField{
            name: "l"
            read: false
            write: false
        }
        ModifyField{
            name: "w"
            read: false
            write: true
            rename: "windowPos"
        }
        ModifyField{
            name: "s"
            read: false
            write: true
            rename: "screenPos"
        }
        ModifyField{
            name: "b"
            read: false
            write: true
            rename: "button"
        }
        ModifyField{
            name: "mouseState"
            read: false
            write: true
            rename: "buttons"
        }
        ModifyField{
            name: "caps"
            read: true
            write: true
        }
        ModifyField{
            name: "velocity"
            read: true
            write: true
        }
        ModifyFunction{
            signature: "operator=(const QMouseEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QMoveEvent"
        ModifyField{
            name: "p"
            read: false
            write: true
            rename: "pos"
        }
        ModifyField{
            name: "oldp"
            read: false
            write: true
            rename: "oldPos"
        }
        ModifyFunction{
            signature: "operator=(const QMoveEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QResizeEvent"
        ModifyField{
            name: "s"
            read: false
            write: true
            rename: "size"
        }
        ModifyField{
            name: "olds"
            read: false
            write: true
            rename: "oldSize"
        }
        ModifyFunction{
            signature: "operator=(const QResizeEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QShortcutEvent"
        ModifyField{
            name: "sequence"
            read: false
            write: true
            rename: "key"
        }
        ModifyField{
            name: "ambig"
            read: false
            write: true
            rename: "ambiguous"
        }
        ModifyField{
            name: "sid"
            read: false
            write: true
            rename: "shortcutId"
        }
        ModifyField{
            name: "m_sequence"
            read: false
            write: true
            rename: "key"
        }
        ModifyField{
            name: "m_ambiguous"
            read: false
            write: true
            rename: "ambiguous"
        }
        ModifyField{
            name: "m_shortcutId"
            read: false
            write: true
            rename: "shortcutId"
        }
        ModifyFunction{
            signature: "operator=(const QShortcutEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QShowEvent"
        ModifyFunction{
            signature: "operator=(const QShowEvent &)"
            rename: "set"
            ModifyArgument{
                index: "return"
                replaceType: "void"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: [6, 4]
        }
    }
    
    ObjectType{
        name: "QStatusTipEvent"
        ModifyFunction{
            signature: "operator=(const QStatusTipEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QTabletEvent"
        ModifyField{
            name: "mPos"
            read: false
            write: true
            rename: "posF"
        }
        ModifyField{
            name: "mGPos"
            read: false
            write: true
            rename: "globalPosF"
        }
        ModifyField{
            name: "mDev"
            read: false
            write: true
            rename: "device"
        }
        ModifyField{
            name: "mPointerType"
            read: false
            write: true
            rename: "pointerType"
        }
        ModifyField{
            name: "mXT"
            read: false
            write: true
            rename: "xTilt"
        }
        ModifyField{
            name: "mYT"
            read: false
            write: true
            rename: "yTilt"
        }
        ModifyField{
            name: "mZ"
            read: false
            write: true
            rename: "z"
        }
        ModifyField{
            name: "mPress"
            read: false
            write: true
            rename: "pressure"
        }
        ModifyField{
            name: "mTangential"
            read: false
            write: true
            rename: "tangentialPressure"
        }
        ModifyField{
            name: "mRot"
            read: false
            write: true
            rename: "rotation"
        }
        ModifyField{
            name: "mUnique"
            read: false
            write: true
            rename: "uniqueId"
        }
        ModifyField{
            name: "mExtra"
            read: false
            write: false
        }
        ModifyField{
            name: "m_tangential"
            read: false
            write: true
            rename: "tangentialPressure"
        }
        ModifyFunction{
            signature: "operator=(const QTabletEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QWheelEvent"
        ModifyField{
            name: "p"
            read: false
            write: true
            rename: "posF"
        }
        ModifyField{
            name: "g"
            read: false
            write: true
            rename: "globalPosF"
        }
        ModifyField{
            name: "pixelD"
            read: false
            write: true
            rename: "pixelDelta"
        }
        ModifyField{
            name: "angleD"
            read: false
            write: true
            rename: "angleDelta"
        }
        ModifyField{
            name: "qt4D"
            read: false
            write: true
            rename: "delta"
        }
        ModifyField{
            name: "qt4O"
            read: false
            write: true
            rename: "orientation"
        }
        ModifyField{
            name: "mouseState"
            read: false
            write: true
            rename: "buttons"
        }
        ModifyField{
            name: "ph"
            read: false
            write: true
            rename: "phase"
        }
        ModifyField{
            name: "src"
            read: false
            write: true
            rename: "source"
        }
        ModifyField{
            name: "invertedScrolling"
            read: false
            write: true
            rename: "inverted"
        }
        ModifyField{
            name: "reserved"
            read: false
            write: false
        }
        ModifyFunction{
            signature: "operator=(const QWheelEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QWindowStateChangeEvent"
        ModifyFunction{
            signature: "operator=(const QWindowStateChangeEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QWhatsThisClickedEvent"
        ModifyFunction{
            signature: "operator=(const QWhatsThisClickedEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QPaintEvent"
        ModifyField{
            name: "m_rect"
            read: false
            write: true
            rename: "rect"
        }
        ModifyField{
            name: "m_region"
            read: false
            write: true
            rename: "region"
        }
        ModifyField{
            name: "m_erased"
            read: true
            write: true
            rename: "erased"
        }
        ModifyFunction{
            signature: "operator=(const QPaintEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QScrollEvent"
        ModifyFunction{
            signature: "operator=(const QScrollEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QScrollPrepareEvent"
        ModifyFunction{
            signature: "operator=(const QScrollPrepareEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QAbstractTextDocumentLayout"

        ValueType{
            name: "PaintContext"
            Include{
                fileName: "QAbstractTextDocumentLayout"
                location: Include.Global
            }
        }

        ValueType{
            name: "Selection"
        }
        forceAbstract: true
        ModifyFunction{
            signature: "setPaintDevice(QPaintDevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcPaintDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "paintDevice()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "draw(QPainter*,QAbstractTextDocumentLayout::PaintContext)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawInlineObject(QPainter*,QRectF,QTextInlineObject,int,QTextFormat)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QAbstractTextDocumentLayout___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "registerHandler(int,QObject*)"
            rename: "_registerHandler"
            access: Modification.Private
        }
        ModifyFunction{
            signature: "unregisterHandler(int,QObject*)"
            rename: "_unregisterHandler"
            access: Modification.Private
        }
        ModifyFunction{
            signature: "handlerForObject(int)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
    }
    
    ValueType{
        name: "QBitmap"
        noImplicitConstructors: true
        threadAffinity: "pixmap"
        ModifyFunction{
            signature: "operator=(QPixmap)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QBitmap(QPixmap)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "fromData(QSize,const uchar*,QImage::Format)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "byte[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JByteArrayPointer %out(%env, jbyteArray(%in));"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QBackingStore"
        ModifyFunction{
            signature: "paintDevice()"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ValueType{
        name: "QBrush"
        Rejection{
            functionName: "cleanUp"
        }
        ExtraIncludes{
            Include{
                fileName: "QPixmap"
                location: Include.Global
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QBrush___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "QBrush(QColor,Qt::BrushStyle)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type:"QString"}
                InhibitImplicitCall{type:"Qt::GlobalColor"}
            }
        }
        ModifyFunction{
            signature: "QBrush(QColor,QPixmap)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type:"QString"}
                InhibitImplicitCall{type:"Qt::GlobalColor"}
            }
        }
    }
    
    ObjectType{
        name: "QClipboard"
        ExtraIncludes{
            Include{
                fileName: "QImage"
                location: Include.Global
            }
            Include{
                fileName: "QPixmap"
                location: Include.Global
            }
        }
        EnumType{
            name: "Mode"
            RejectEnumValue{
                name: "LastMode"
            }
        }
        ModifyFunction{
            signature: "setMimeData(QMimeData *, QClipboard::Mode)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QClipboard___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QRgba64"
        ModifyFunction{
            signature: "operator quint64()const"
            rename: "toRgba64"
        }
    }
    
    ValueType{
        name: "QColor"
        Rejection{className: "CT"}
        EnumType{
            name: "Spec"
        }
        ModifyFunction{
            signature: "QColor(QColor::Spec)"
            isForcedExplicit: true
        }
        ModifyFunction{
            signature: "QColor(uint)"
            isForcedExplicit: true
        }
        ModifyFunction{
            signature: "QColor(QLatin1String)"
            remove: RemoveFlag.All
            since: [5, 8]
            until: [6, 3]
        }
        ModifyFunction{
            signature: "QColor(QLatin1StringView)"
            remove: RemoveFlag.All
            since: [6, 4]
        }
        ModifyFunction{
            signature: "QColor(QStringView)"
            remove: RemoveFlag.All
            since: [5, 8]
        }
        ModifyFunction{
            signature: "setNamedColor(QLatin1String)"
            remove: RemoveFlag.All
            since: [5, 8]
            until: [6, 3]
        }
        ModifyFunction{
            signature: "setNamedColor(QLatin1StringView)"
            remove: RemoveFlag.All
            since: [6, 4]
        }
        ModifyFunction{
            signature: "setNamedColor(QStringView)"
            remove: RemoveFlag.All
            since: [5, 8]
        }
        ModifyFunction{
            signature: "isValidColor(QLatin1String)"
            remove: RemoveFlag.All
            since: [5, 8]
            until: [6, 3]
        }
        ModifyFunction{
            signature: "isValidColor(QLatin1StringView)"
            remove: RemoveFlag.All
            since: [6, 4]
        }
        ModifyFunction{
            signature: "isValidColor(QStringView)"
            remove: RemoveFlag.All
            since: [5, 10]
        }
        ModifyFunction{
            signature: "QColor(const char*)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "getCmyk(int*,int*,int*,int*,int*)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "getCmyk(int*,int*,int*,int*,int*) const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "getCmykF(qreal*,qreal*,qreal*,qreal*,qreal*)"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "getCmykF(qreal*,qreal*,qreal*,qreal*,qreal*) const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "getCmykF(float*,float*,float*,float*,float*) const"
            Remove{
            }
            since: 6
        }
        ModifyFunction{
            signature: "getHsv(int*,int*,int*,int*)const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "getHsvF(qreal*,qreal*,qreal*,qreal*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "getHsvF(float*,float*,float*,float*) const"
            Remove{
            }
            since: 6
        }
        ModifyFunction{
            signature: "getHsl(int*,int*,int*,int*)const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "getHslF(qreal*,qreal*,qreal*,qreal*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "getHslF(float*,float*,float*,float*) const"
            Remove{
            }
            since: 6
        }
        ModifyFunction{
            signature: "getRgb(int*,int*,int*,int*)const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "getRgbF(qreal*,qreal*,qreal*,qreal*)const"
            Remove{
            }
            until: 5
        }
        ModifyFunction{
            signature: "getRgbF(float*,float*,float*,float*) const"
            Remove{
            }
            since: 6
        }
        ModifyFunction{
            signature: "dark(int)const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "light(int)const"
            remove: RemoveFlag.All
            until: 5
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QColor___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "fromRgba64(ushort,ushort,ushort,ushort)"
            ModifyArgument{
                index: 4
                ReplaceDefaultExpression{
                    expression: "0xffff"
                }
            }
        }
    }
    
    ValueType{
        name: "QConicalGradient"
        polymorphicIdExpression: "%1->type() == QGradient::ConicalGradient"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QConicalGradient(copy->center(), copy->angle());\n"+
                          "}else{\n"+
                          "    return new(placement) QConicalGradient();\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) QConicalGradient(copy->center(), copy->angle());"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QConicalGradient();"}
        }
    }
    
    ValueType{
        name: "QCursor"
        threadAffinity: "pixmap"
        ExtraIncludes{
            Include{
                fileName: "QPixmap"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QCursor()"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "QCursor(QCursor)"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "QCursor(QPixmap, int, int)"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "QCursor(Qt::CursorShape)"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "QCursor(QBitmap, QBitmap, int, int)"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "operator=(QCursor)"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "setPos(QScreen*,QPoint)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setPos(QScreen*,int,int)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "bitmap()const"
            remove: RemoveFlag.All
            since: [5, 15]
        }
        ModifyFunction{
            signature: "mask()const"
            remove: RemoveFlag.All
            since: [5, 15]
        }
        ModifyFunction{
            signature: "bitmap(Qt::ReturnByValueConstant)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Qt::ReturnByValueConstant %out = Qt::ReturnByValue;"}
                }
            }
            since: [5, 15]
        }
        ModifyFunction{
            signature: "mask(Qt::ReturnByValueConstant)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "Qt::ReturnByValueConstant %out = Qt::ReturnByValue;"}
                }
            }
            since: [5, 15]
        }
    }
    
    ObjectType{
        name: "QDesktopServices"
        ModifyFunction{
            signature: "setUrlHandler(const QString &, QObject *, const char *)"
            access: Modification.Private
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = \"handleUrl\";"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QDesktopServices___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "unsetUrlHandler(const QString &)"
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "__rcUrlHandlers.remove(%1);"}
            }
        }
    }
    
    ObjectType{
        name: "QDoubleValidator"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QDrag"
        ExtraIncludes{
            Include{
                fileName: "QPoint"
                location: Include.Global
            }
            Include{
                fileName: "QPixmap"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setMimeData(QMimeData*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "start(Qt::DropActions)"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ObjectType{
        name: "QDragEnterEvent"
        ModifyFunction{
            signature: "operator=(const QDragEnterEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: [6, 4]
        }
    }
    
    ObjectType{
        name: "QDragLeaveEvent"
        ModifyFunction{
            signature: "operator=(const QDragLeaveEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: [6, 4]
        }
    }
    
    ObjectType{
        name: "QDragMoveEvent"
        ModifyFunction{
            signature: "operator=(const QDragMoveEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "accept()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "ignore()"
            remove: RemoveFlag.All
        }
        ModifyField{
            name: "rect"
            read: false
            write: true
            rename: "answerRect"
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QDropEvent"
        ModifyFunction{
            signature: "operator=(const QDropEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyField{
            name: "p"
            read: false
            write: true
            rename: "posF"
        }
        ModifyField{
            name: "m_pos"
            read: false
            write: true
            rename: "position"
        }
        ModifyField{
            name: "mouseState"
            read: false
            write: true
            rename: "mouseButtons"
        }
        ModifyField{
            name: "m_mouseState"
            read: false
            write: true
            rename: "mouseButtons"
        }
        ModifyField{
            name: "modState"
            read: false
            write: true
            rename: "keyboardModifiers"
        }
        ModifyField{
            name: "m_modState"
            read: false
            write: true
            rename: "keyboardModifiers"
        }
        ModifyField{
            name: "act"
            read: false
            write: true
            rename: "possibleActions"
        }
        ModifyField{
            name: "m_actions"
            read: false
            write: true
            rename: "possibleActions"
        }
        ModifyField{
            name: "m_dropAction"
            read: false
            write: false
        }
        ModifyField{
            name: "drop_action"
            read: false
            write: false
        }
        ModifyField{
            name: "default_action"
            read: false
            write: true
            rename: "proposedAction"
        }
        ModifyField{
            name: "m_defaultAction"
            read: false
            write: true
            rename: "proposedAction"
        }
        ModifyField{
            name: "mdata"
            read: false
            write: true
            rename: "mimeData"
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcMimeData"
            }
        }
        ModifyField{
            name: "m_data"
            read: false
            write: true
            rename: "mimeData"
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcMimeData"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QEnterEvent"
        ModifyFunction{
            signature: "operator=(const QEnterEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyField{
            name: "l"
            read: false
            write: true
            rename: "localPos"
        }
        ModifyField{
            name: "w"
            read: false
            write: true
            rename: "windowPos"
        }
        ModifyField{
            name: "s"
            read: false
            write: true
            rename: "screenPos"
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QExposeEvent"
        ModifyFunction{
            signature: "operator=(const QExposeEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyField{
            name: "rgn"
            read: false
            write: true
            rename: "region"
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QPlatformSurfaceEvent"

        EnumType{
            name: "SurfaceEventType"
        }
        ModifyField{
            name: "m_surfaceEventType"
            read: false
            write: true
            rename: "surfaceEventType"
        }
        ModifyFunction{
            signature: "operator=(const QPlatformSurfaceEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QFileOpenEvent"
        ModifyFunction{
            signature: "operator=(const QFileOpenEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QFocusEvent"
        ModifyFunction{
            signature: "operator=(const QFocusEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QHelpEvent"
        ModifyFunction{
            signature: "operator=(const QHelpEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QHideEvent"
        ModifyFunction{
            signature: "operator=(const QHideEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: [6, 4]
        }
    }
    
    ObjectType{
        name: "QHoverEvent"
        ModifyField{
            name: "p"
            read: false
            write: true
            rename: "posF"
            until: 5
        }
        ModifyField{
            name: "op"
            read: false
            write: true
            rename: "oldPosF"
            until: 5
        }
        ModifyField{
            name: "m_oldPos"
            read: false
            write: false
            since: 6
        }
        ModifyField{
            name: "m_reserved"
            read: false
            write: false
            since: 6
        }
        ModifyFunction{
            signature: "operator=(const QHoverEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QIconDragEvent"
        ModifyFunction{
            signature: "operator=(const QIconDragEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: [6, 4]
        }
    }
    
    ObjectType{
        name: "QKeyEvent"
        ModifyField{
            name: "txt"
            read: false
            write: true
            rename: "text"
        }
        ModifyField{
            name: "k"
            read: false
            write: true
            rename: "key"
        }
        ModifyField{
            name: "nScanCode"
            read: false
            write: true
            rename: "nativeScanCode"
        }
        ModifyField{
            name: "nVirtualKey"
            read: false
            write: true
            rename: "nativeVirtualKey"
        }
        ModifyField{
            name: "nModifiers"
            read: false
            write: true
            rename: "nativeModifiers"
        }
        ModifyField{
            name: "c"
            read: false
            write: true
            rename: "count"
        }
        ModifyField{
            name: "m_count"
            read: false
            write: true
            rename: "count"
        }
        ModifyField{
            name: "autor"
            read: false
            write: true
            rename: "autoRepeat"
            ReplaceType{
                modifiedType: "boolean"
            }
        }
        ModifyField{
            name: "m_autoRepeat"
            read: false
            write: true
            rename: "autoRepeat"
            ReplaceType{
                modifiedType: "boolean"
            }
        }
        ModifyFunction{
            signature: "operator=(const QKeyEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }

        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }

        ModifyFunction{
            signature: "operator==(QKeySequence::StandardKey)"
            remove: RemoveFlag.All
            since: 6
        }
    }
    GlobalFunction{
        signature: "operator==(QKeyEvent*,QKeySequence::StandardKey)"
        remove: RemoveFlag.All
        until: 5
    }
    
    ObjectType{
        name: "QTouchEvent"
        ModifyFunction{
            signature: "operator=(const QTouchEvent &)"
            Delegate{
                deprecated: true
                name: "set"
            }
        }
        ModifyField{
            name: "m_reserved"
            read: false
            write: false
        }
        ModifyField{
            name: "m_touchPointStates"
            read: false
            write: true
            rename: "touchPointStates"
        }
        ModifyField{
            name: "m_target"
            read: false
            write: true
            rename: "target"
            ReferenceCount{
                action: ReferenceCount.Set
                variableName: "__rcTarget"
            }
        }
        ModifyFunction{
            signature: "setDevice(QTouchDevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setTarget(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcTarget"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setWindow(QWindow*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcWidget"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QAccessibleEvent"

        EnumType{
            name: "Event"
        }
        ModifyField{
            name: "m_type"
            read: false
            write: false
        }
        ModifyField{
            name: "m_object"
            read: false
            write: false
        }
        ModifyField{
            name: "m_child"
            read: false
            write: false
        }
        ModifyField{
            name: "m_uniqueId"
            read: false
            write: false
        }
        ModifyFunction{
            signature: "accessibleInterface()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QAccessibleValueChangeEvent"
    }
    
    ObjectType{
        name: "QAccessibleStateChangeEvent"
        ModifyField{
            name: "m_changedStates"
            read: false
            write: true
            rename: "changedStates"
        }
    }
    
    ObjectType{
        name: "QAccessibleTextSelectionEvent"
    }
    
    ObjectType{
        name: "QAccessibleTextUpdateEvent"
    }
    
    ObjectType{
        name: "QAccessibleTextRemoveEvent"
    }
    
    ObjectType{
        name: "QAccessibleTextInsertEvent"
    }
    
    ObjectType{
        name: "QAccessibleTextCursorEvent"
    }
    
    ObjectType{
        name: "QAccessibleTableModelChangeEvent"

        EnumType{
            name: "ModelChangeType"
        }
    }
    
    ValueType{
        name: "QFontInfo"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QFontInfo(*copy);\n"+
                          "}else{\n"+
                          "    return new(placement) QFontInfo(QFont());\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) QFontInfo(*copy);"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QFontInfo(QFont());"}
        }
    }
    
    ValueType{
        name: "QFont"
        ExtraIncludes{
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QFont(const QFont &, QPaintDevice*)"
            remove: RemoveFlag.All
            since: [5, 12]
            until: 5
        }
    }
    
    ValueType{
        name: "QFontDatabase"
        ExtraIncludes{
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QFontDatabase()"
            InjectCode{
                position: Position.Beginning
                Text{content: "if(io.qt.gui.QGuiApplication.instance()==null)\n"+
                              "    throw new IllegalStateException(\"Cannot create QFontDatabase before initializing QGuiApplication.\");"}
            }
            until: 5
        }
    }
    
    ValueType{
        name: "QFontMetricsF"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QFontMetricsF(*copy);\n"+
                          "}else{\n"+
                          "    return new(placement) QFontMetricsF(QFont());\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) QFontMetricsF(*copy);"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QFontMetricsF(QFont());"}
        }
        ModifyFunction{
            signature: "boundingRect(QRectF,int,QString,int,int*)const"
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JIntArrayPointer %out(%env, %in);"}
                }
            }
            InjectCode{
                InsertTemplate{
                    name: "gui.tabarray.check"
                }
            }
        }
        ModifyFunction{
            signature: "size(int,QString,int,int*)const"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JIntArrayPointer %out(%env, %in);"}
                }
            }
            InjectCode{
                InsertTemplate{
                    name: "gui.tabarray.check"
                }
            }
        }
    }
    
    ValueType{
        name: "QFontMetrics"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    return new(placement) QFontMetrics(*copy);\n"+
                          "}else{\n"+
                          "    return new(placement) QFontMetrics(QFont());\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) QFontMetrics(*copy);"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QFontMetrics(QFont());"}
        }
        ModifyFunction{
            signature: "boundingRect(int,int,int,int,int,QString,int,int*)const"
            ModifyArgument{
                index: 8
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JIntArrayPointer %out(%env, %in);"}
                }
            }
            InjectCode{
                InsertTemplate{
                    name: "gui.tabarray.check"
                }
            }
        }
        ModifyFunction{
            signature: "boundingRect(QRect,int,QString,int,int*)const"
            ModifyArgument{
                index: 5
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JIntArrayPointer %out(%env, %in);"}
                }
            }
            InjectCode{
                InsertTemplate{
                    name: "gui.tabarray.check"
                }
            }
        }
        ModifyFunction{
            signature: "size(int,QString,int,int*)const"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JIntArrayPointer %out(%env, %in);"}
                }
            }
            InjectCode{
                InsertTemplate{
                    name: "gui.tabarray.check"
                }
            }
        }
    }
    
    ValueType{
        name: "QGlyphRun"

        EnumType{
            name: "GlyphRunFlag"
        }
        ModifyFunction{
            signature: "setRawData(const quint32 *, const QPointF *, int)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QRawFont"

        EnumType{
            name: "AntialiasingType"
        }

        EnumType{
            name: "LayoutFlag"
        }
        ModifyFunction{
            signature: "advancesForGlyphIndexes(const quint32 *, QPointF *, int, QRawFont::LayoutFlags) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "advancesForGlyphIndexes(const quint32 *, QPointF *, int) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "glyphIndexesForChars(const QChar *, int, quint32 *, int *) const"
            remove: RemoveFlag.All
        }
    }
    
    
    ValueType{
        name: "QGradient"
        Rejection{enumName: "Type"}
        Rejection{functionName: "type"}

        EnumType{
            name: "Spread"
            upperBound: "QGradient.RepeatSpread"
            lowerBound: "QGradient.PadSpread"
        }

        EnumType{
            name: "CoordinateMode"
        }

        EnumType{
            name: "Preset"
        }

        EnumType{
            name: "InterpolationMode"
        }

        isPolymorphicBase: true
        polymorphicIdExpression: "%1->type() == QGradient::NoGradient"
        CustomConstructor{
            Text{content: "Q_UNUSED(copy)\n"+
                          "qWarning(\"Copying empty QGradient object\");\n"+
                          "return new(placement) QGradient();"}
        }
        ModifyFunction{
            signature: "operator==(const QGradient &)const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "QGradient()"
            Remove{
            }
        }
        ModifyFunction{
            signature: "QGradient(QGradient::Preset)"
            Remove{
            }
            since: [5, 12]
        }
        InjectCode{
            since: [5, 12]
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QGradient_java__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QGuiApplication"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
            Include{
                fileName: "QBasicTimer"
                location: Include.Global
            }
            Include{
                fileName: "QFont"
                location: Include.Global
            }
            Include{
                fileName: "QFontMetrics"
                location: Include.Global
            }
            Include{
                fileName: "QPalette"
                location: Include.Global
            }
            Include{
                fileName: "QIcon"
                location: Include.Global
            }
            Include{
                fileName: "QLocale"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "topLevelAt(const QPoint &)"
            Rename{
                to: "topLevelWindowAt"
            }
        }
        ModifyFunction{
            signature: "setPalette(QPalette)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setLayoutDirection(Qt::LayoutDirection)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowIcon(QIcon)"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "clipboard()"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QGuiApplication___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtCore/private/qthread_p.h"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "QGuiApplication(int &, char **, int)"
            Access{
                modifiers: Modification.Protected
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "if(!Java::QtCore::QCoreApplication::__qt_isInitializing(%env)){\n"+
                              "    Java::Runtime::IllegalAccessError::throwNew(%env, \"Not allowed to instantiate QGuiApplication. Please use QGuiApplication.initialize() instead.\" QTJAMBI_STACKTRACEINFO );\n"+
                              "    return;\n"+
                              "}"}
            }
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "// nothing to do"}
                }
            }
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "std::unique_ptr<ApplicationData> applicationData(new ApplicationData(%env, jobjectArray(%in)));\n"+
                                  "char** %out = applicationData->chars();\n"+
                                  "int& __qt_%1 = applicationData->size();"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "applicationData->update(%env);\n"+
                              "QTJAMBI_SET_OBJECTUSERDATA(ApplicationData, __qt_this, applicationData.release());"}
            }
        }
        ModifyFunction{
            signature: "exec()"
            InjectCode{
                target: CodeClass.Native
                position: Position.Beginning
                Text{content: "bool useQApplicationExec = false;\n"+
                              "QCoreApplication* instance = QCoreApplication::instance();\n"+
                              "if (!instance)\n"+
                              "    JavaException::raiseRuntimeException(%env, \"QGuiApplication has not been initialized with QGuiApplication.initialize()\" QTJAMBI_STACKTRACEINFO );\n"+
                              "else if(instance->inherits(\"QApplication\")){\n"+
                              "    useQApplicationExec = true;\n"+
                              "    QTJAMBI_TRY_ANY{\n"+
                              "        Java::QtWidgets::QApplication::getClass(%env);\n"+
                              "    }QTJAMBI_CATCH_ANY{\n"+
                              "        useQApplicationExec = false;\n"+
                              "    }QTJAMBI_TRY_END\n"+
                              "    if(useQApplicationExec)\n"+
                              "        __java_return_value = Java::QtWidgets::QApplication::exec(%env);\n"+
                              "}else if(instance->thread()!=QThread::currentThread())\n"+
                              "    JavaException::raiseRuntimeException(%env, \"exec() must be called from the main thread.\" QTJAMBI_STACKTRACEINFO );\n"+
                              "else if(QThreadData::get2(instance->thread())->eventLoops.size()>0)\n"+
                              "    JavaException::raiseRuntimeException(%env, \"The event loop is already running.\" QTJAMBI_STACKTRACEINFO );\n"+
                              "if(!useQApplicationExec){"}
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                Text{content: "}"}
            }
        }
        ModifyFunction{
            signature: "overrideCursor()"
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = %in ? qtjambi_cast<jobject>(%env, *static_cast<const QCursor*>(%in)) : nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            access: Modification.Private
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends io.qt.@Nullable QtObjectInterface> QNativeInterface"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, %in, %1);"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Class<QNativeInterface>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "if(inherits(\"QApplication\")){\n"+
                                  "    bool classNotFound = false;\n"+
                                  "    QTJAMBI_TRY_ANY{\n"+
                                  "        Java::QtWidgets::QApplication::getClass(%env);\n"+
                                  "    }QTJAMBI_CATCH_ANY{\n"+
                                  "        classNotFound = true;\n"+
                                  "    }QTJAMBI_TRY_END\n"+
                                  "    if(!classNotFound)\n"+
                                  "        return Java::QtWidgets::QApplication::resolveInterface(%env, CoreAPI::javaObject(__this_nativeId, %env), %in);\n"+
                                  "}\n"+
                                  "CoreAPI::NITypeInfo info = CoreAPI::getNativeInterfaceInfo(%env, %in);\n"+
                                  "const char* %out = info.name;"}
                }
            }
            ModifyArgument{
                index: 2
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out = info.revision;"}
                }
                RemoveArgument{
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "notify(QObject *, QEvent *)"
            ModifyArgument{
                index: 1
                threadAffinity: true
            }
        }
    }
    
    ValueType{
        name: "QIcon"
        threadAffinity: "pixmap"
        ModifyFunction{
            signature: "QIcon()"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "QIcon(QIcon)"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "QIcon(QString)"
            threadAffinity: Affinity.Pixmap
        }
        ModifyFunction{
            signature: "QIcon(QIconEngine *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QIconEngine"
        ModifyFunction{
            signature: "paint(QPainter*,QRect,QIcon::Mode,QIcon::State)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "read(QDataStream&)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "write(QDataStream&)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "virtual_hook(int,void*)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
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
    
    ValueType{
        name: "QIconEngine::ScaledPixmapArgument"
    }
    
    ValueType{
        name: "QImageCleanupFunction"
        generate: false
    }
    
    ValueType{
        name: "QImage"
        Rejection{
            functionName: "convertToFormat_helper"
            since: 6
        }
        Rejection{
            functionName: "convertToFormat_inplace"
            since: 6
        }
        Rejection{
            functionName: "rgbSwapped_inplace"
            since: 6
        }
        Rejection{
            functionName: "rgbSwapped_helper"
            since: 6
        }
        Rejection{
            functionName: "mirrored_inplace"
            since: 6
        }
        Rejection{
            functionName: "mirrored_helper"
            since: 6
        }
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "bits()"
            Remove{
            }
        }
        ModifyFunction{
            signature: "scanLine(int)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "constScanLine(int)const"
            Remove{
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QImage(uchar *, int, int, QImage::Format, QImageCleanupFunction, void *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QImage(const uchar *, int, int, QImage::Format, QImageCleanupFunction, void *)"
            remove: RemoveFlag.JavaAndNative
        }
        ModifyFunction{
            signature: "QImage(uchar *, int, int, int, QImage::Format, QImageCleanupFunction, void *)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "QImage(uchar *, int, int, qsizetype, QImage::Format, QImageCleanupFunction, void *)"
            remove: RemoveFlag.All
            since: 6
        }
        ModifyFunction{
            signature: "QImage(const uchar *, int, int, int, QImage::Format, QImageCleanupFunction, void *)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "QImage(const uchar *, int, int, qsizetype, QImage::Format, QImageCleanupFunction, void *)"
            remove: RemoveFlag.All
            since: 6
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QImage___"
                quoteBeforeLine: "}// class"
            }
        }
        InjectCode{
            target: CodeClass.Native
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QImage::JNI"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QImage(const char * const*)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize length = %env->GetArrayLength(jarray(%in));\n"+
                                  "QVector<QSharedPointer<J2CStringBuffer>> %inVec;\n"+
                                  "const char** %out = nullptr;\n"+
                                  "QScopedArrayPointer<const char*> %outPtr(%out = new const char*[quint32(length)]);\n"+
                                  "for(jsize i=0; i<length; ++i){\n"+
                                  "    J2CStringBuffer* b = new J2CStringBuffer(%env, jstring(%env->GetObjectArrayElement(jobjectArray(%in), i)));\n"+
                                  "    %out[i] = *b;\n"+
                                  "    %inVec << QSharedPointer<J2CStringBuffer>(b);\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "fromData(const uchar*,int,const char*)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter: 2
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
            }
        }
        ModifyFunction{
            signature: "fromData(QByteArrayView,const char*)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type: "java.nio.@NonNull ByteBuffer"}
                InhibitImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "fromData(QByteArray,const char*)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type: "byte @NonNull[]"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "loadFromData(const uchar*,int,const char*)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter: 2
                }
            }
        }
        ModifyFunction{
            signature: "loadFromData(QByteArrayView,const char*)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type: "java.nio.@NonNull ByteBuffer"}
                InhibitImplicitCall{type: "io.qt.core.@NonNull QByteArray"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "loadFromData(QByteArray,const char*)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type: "byte @NonNull[]"}
            }
            since: 6
        }
        ModifyFunction{
            signature: "bits()const"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "%this->sizeInBytes()"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "constBits()const"
            rename: "bytes"
            ModifyArgument{
                index: 0
                AsArray{
                    lengthExpression: "%this->sizeInBytes()"
                }
                NoNullPointer{}
            }
        }
        ModifyFunction{
            signature: "scanLine(int)const"
            ModifyArgument{
                index: 0
                AsBuffer{
                    lengthExpression: "__qt_this->sizeInBytes()"
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
    }
    
    ObjectType{
        name: "QImageWriter"
        ModifyFunction{
            signature: "setDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "description()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setDescription(QString)"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ObjectType{
        name: "QImageIOHandler"
        ExtraIncludes{
            Include{
                fileName: "QRect"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setFormat(const QByteArray &)const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "name()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "read(QImage*)"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QImage __image;\n"+
                                  "QImage* %out = &__image;"}
                }
            }
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.gui.@Nullable QImage"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "bool %out = __java_return_value!=nullptr;\n"+
                                  "if(%out && %1)\n"+
                                  "    *%1 = qtjambi_cast<const QImage&>(%env, %in);"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = %in ? qtjambi_cast<jobject>(%env, __image) : nullptr;"}
                }
            }
        }
        InjectCode{
            since: 6
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QImageIOHandler_6__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QImageReader"
        ExtraIncludes{
            Include{
                fileName: "QColor"
                location: Include.Global
            }
            Include{
                fileName: "QRect"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QImage"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "read(QImage*)"
            Remove{
            }
        }
        ModifyFunction{
            signature: "setDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    
    ObjectType{
        name: "QImageIOPlugin"
        ModifyFunction{
            signature: "create(QIODevice*,QByteArray)const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            target: CodeClass.MetaInfo
            position: Position.Position1
            Text{content: "registerInterfaceID(typeId, QImageIOHandlerFactoryInterface_iid);"}
        }
    }
    
    ObjectType{
        name: "QPlatformInputContextPlugin"
    }
    
    ObjectType{
        name: "QPlatformIntegrationPlugin"
    }
    
    ObjectType{
        name: "QPlatformThemePlugin"
    }
    
    ObjectType{
        name: "QInputMethod"
    }
    
    ObjectType{
        name: "QIntValidator"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
    }
    
    ValueType{
        name: "QKeySequence"
        InjectCode{
            until: 5
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QKeySequence_5__"
                quoteBeforeLine: "}// class"
            }
        }
        InjectCode{
            since: 6
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QKeySequence_6__"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QKeyCombination"
        ModifyFunction{
            signature: "operator int() const"
            remove: RemoveFlag.All
        }
        since: 6
    }
    
    ValueType{
        name: "QLinearGradient"
        polymorphicIdExpression: "%1->type() == QGradient::LinearGradient"
        CustomConstructor{
            Text{content: "if(copy){\n"+
                          "    QLinearGradient *lg = new(placement) QLinearGradient(copy->start(), copy->finalStop());\n"+
                          "    lg->setSpread(copy->spread());\n"+
                          "    lg->setStops(copy->stops());\n"+
                          "    return lg;\n"+
                          "}else{\n"+
                          "    return new(placement) QLinearGradient();\n"+
                          "}"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "QLinearGradient *lg = new(placement) QLinearGradient(copy->start(), copy->finalStop());\n"+
                          "lg->setSpread(copy->spread());\n"+
                          "lg->setStops(copy->stops());"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QLinearGradient();"}
        }
    }
    
    ValueType{
        name: "QMatrix4x3"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix4x3(const float *)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 12
                }
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix4x3 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix4x3 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 12
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsArray{
                    minLength: 12
                }
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsBuffer{
                    minLength: 12
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix4x3"
                }
                Replace{
                    from: "%COUNT"
                    to: "12"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix4x2"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix4x2(const float *)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 8
                }
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix4x2 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix4x2 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 8
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsArray{
                    minLength: 8
                }
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsBuffer{
                    minLength: 8
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix4x2"
                }
                Replace{
                    from: "%COUNT"
                    to: "8"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix3x4"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix3x4(const float *)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 12
                }
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix3x4 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix3x4 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 12
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsArray{
                    minLength: 12
                }
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsBuffer{
                    minLength: 12
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix3x4"
                }
                Replace{
                    from: "%COUNT"
                    to: "12"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix3x3"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix3x3(const float *)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 9
                }
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix3x3 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix3x3 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 9
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsArray{
                    minLength: 9
                }
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsBuffer{
                    minLength: 9
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix3x3"
                }
                Replace{
                    from: "%COUNT"
                    to: "9"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix3x2"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix3x2(const float *)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 6
                }
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix3x2 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix3x2 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 6
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsArray{
                    minLength: 6
                }
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsBuffer{
                    minLength: 6
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix3x2"
                }
                Replace{
                    from: "%COUNT"
                    to: "6"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix2x4"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix2x4(const float *)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 8
                }
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix2x4 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix2x4 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 8
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsArray{
                    minLength: 8
                }
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsBuffer{
                    minLength: 8
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix2x4"
                }
                Replace{
                    from: "%COUNT"
                    to: "8"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix2x3"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix2x3(const float *)"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 6
                }
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix2x3 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix2x3 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 6
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                NoNullPointer{}
                AsArray{
                    minLength: 6
                }
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                AsBuffer{
                    minLength: 6
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
                NoNullPointer{}
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix2x3"
                }
                Replace{
                    from: "%COUNT"
                    to: "6"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix2x2"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QMatrix2x2(const float *)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    minLength: 4
                }
                NoNullPointer{}
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix2x2 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix2x2 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                AsBuffer{
                    minLength: 4
                }
                NoNullPointer{}
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                AsArray{
                    minLength: 4
                }
                NoNullPointer{}
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                AsBuffer{
                    minLength: 4
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
                NoNullPointer{}
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix2x2"
                }
                Replace{
                    from: "%COUNT"
                    to: "4"
                }
            }
        }
    }
    
    ValueType{
        name: "QMatrix4x4"
        Rejection{
            functionName: "toGenericMatrix"
        }

        EnumType{
            name: "Flag"
            since: 6
        }
        ModifyFunction{
            signature: "data()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        Import{
            template: "QMatrixXxX"
        }
        ModifyFunction{
            signature: "QMatrix4x4(const float *)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    minLength: 16
                }
                NoNullPointer{}
            }
        }
        ModifyFunction{
            signature: "QMatrix4x4(const float *, int, int)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.nio.FloatBuffer"
                }
                NoNullPointer{}
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBufferConstData %out(%env, %in);\n"+
                                  "if(%out.size()<16){\n"+
                                  "    JavaException::raiseIndexOutOfBoundsException(%env, \"Buffer size is less 16.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "    return;\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "operator*=(const QMatrix4x4 &)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator+=(const QMatrix4x4 &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QMatrix4x4 &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "inverted(bool *)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
        }
        ModifyFunction{
            signature: "constData()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "copyDataTo(float*)const"
            ModifyArgument{
                index: 1
                NoNullPointer{}
                AsBuffer{
                    minLength: 16
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            rename: "array"
            ModifyArgument{
                index: 0
                AsArray{
                    minLength: 16
                }
                NoNullPointer{}
            }
        }
        ModifyFunction{
            signature: "data()"
            ModifyArgument{
                index: 0
                AsBuffer{
                    minLength: 16
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
                NoNullPointer{}
            }
        }
        InjectCode{
            InsertTemplate{
                name: "gui.matrix_constructor"
                Replace{
                    from: "%TYPE"
                    to: "QMatrix4x4"
                }
                Replace{
                    from: "%COUNT"
                    to: "16"
                }
            }
        }
    }

    /*GlobalFunction{
        signature: "operator*(const QMatrix4x4&, const QPoint&)"
        targetType: "QMatrix4x4"
    }

    GlobalFunction{
        signature: "operator*(const QMatrix4x4&, const QPointf&)"
        targetType: "QMatrix4x4"
    }

    GlobalFunction{
        signature: "operator*(const QMatrix4x4&, const QVector3D&)"
        targetType: "QMatrix4x4"
    }

    GlobalFunction{
        signature: "operator*(const QMatrix4x4&, const QVector4D&)"
        targetType: "QMatrix4x4"
    }*/
    
    ObjectType{
        name: "QMovie"
        ExtraIncludes{
            Include{
                fileName: "QColor"
                location: Include.Global
            }
            Include{
                fileName: "QImage"
                location: Include.Global
            }
            Include{
                fileName: "QPixmap"
                location: Include.Global
            }
            Include{
                fileName: "QRect"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "cacheMode()const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "QMovie(QIODevice*,const QByteArray&,QObject *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    EnumType{
        name: "QSurface::SurfaceClass"
        generate: false
    }
    
    InterfaceType{
        name: "QSurface"
        ModifyField{
            name: "m_type"
            read: false
            write: false
        }
        ModifyFunction{
            signature: "QSurface(QSurface::SurfaceClass)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "surfaceClass()const"
            remove: RemoveFlag.All
        }
    }
    
    ObjectType{
        name: "QOffscreenSurface"
        ModifyFunction{
            signature: "QOffscreenSurface(QScreen *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScreen"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "QOffscreenSurface(QScreen *,QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScreen"
                    action: ReferenceCount.Set
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setScreen(QScreen *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcScreen"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "nativeHandle() const"
            remove: RemoveFlag.JavaAndNative
            since: [5, 9]
            until: 5
        }
        ModifyFunction{
            signature: "setNativeHandle(void *)"
            remove: RemoveFlag.JavaAndNative
            since: [5, 9]
            until: 5
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "create()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            rename: "nativeInterface"
            access: Modification.Public
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends io.qt.@Nullable QtObjectInterface> QNativeInterface"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, %in, %1);"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Class<QNativeInterface>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "CoreAPI::NITypeInfo info = CoreAPI::getNativeInterfaceInfo(%env, %in);\n"+
                                  "const char* %out = info.name;"}
                }
            }
            ModifyArgument{
                index: 2
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out = info.revision;"}
                }
                RemoveArgument{
                }
            }
            since: [6, 2]
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            rename: "nativeInterface"
            access: Modification.Public
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends io.qt.@Nullable QtObjectInterface> QNativeInterface"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, %in, %1);"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Class<QNativeInterface>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "CoreAPI::NITypeInfo info = CoreAPI::getNativeInterfaceInfo(%env, %in);\n"+
                                  "const char* %out = info.name;"}
                }
            }
            ModifyArgument{
                index: 2
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out = info.revision;"}
                }
                RemoveArgument{
                }
            }
            since: [6, 2]
        }
    }
    
    InterfaceType{
        name: "QPaintDevice"
        ModifyField{
            name: "painters"
            read: false
            write: false
        }
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "initPainter(QPainter*)const"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                inherited: true
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPaintDevice___"
                quoteBeforeLine: "}// class"
            }
        }
        InjectCode{
            target: CodeClass.DestructorFunction
            Text{content: "if(QPainter* painter = %shelltype::sharedPainter()){\n" +
                          "    if(painter->isActive()){\n" +
                          "        painter->end();\n" +
                          "    }\n" +
                          "}"}
        }
        InjectCode{
            target: CodeClass.DeleterFunction
            Text{content: "struct PaintDeviceAccess : QPaintDevice{\n" +
                          "    inline QPainter* getSharedPainter() const { return sharedPainter(); }\n" +
                          "};\n" +
                          "if(QPainter* painter = reinterpret_cast<PaintDeviceAccess*>(dynamic_cast<QPaintDevice*>(%this))->getSharedPainter()){\n" +
                          "    if(painter->isActive()){\n" +
                          "        painter->end();\n" +
                          "    }\n" +
                          "}"}
        }
        ModifyFunction{
            signature: "devicePixelRatio()const"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "double"
                }
            }
        }
        ModifyFunction{
            signature: "paintEngine()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "redirected(QPoint*)const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "sharedPainter()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    InterfaceType{
        name: "QPagedPaintDevice"
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "devicePageLayout()"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ValueType{
        name: "QPagedPaintDevice::Margins"
    }
    
    ObjectType{
        name: "QPaintEngine"
        ModifyFunction{
            signature: "begin(QPaintDevice*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "drawTextItem(QPointF,QTextItem)"
            ModifyArgument{
                index: 2
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QVarLengthArray"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setPaintDevice(QPaintDevice*)"
            Remove{
            }
        }
        ModifyField{
            name: "active"
            read: false
            write: false
        }
        ModifyField{
            name: "selfDestruct"
            read: false
            write: false
        }
        ModifyField{
            name: "gccaps"
            read: true
            write: true
            rename: "features"
        }
        ModifyField{
            name: "state"
            read: false
            write: false
        }
        ModifyField{
            name: "extended"
            read: false
            write: true
            ReplaceType{
                modifiedType: "boolean"
            }
        }
        ModifyFunction{
            signature: "drawLines(const QLine *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawLines(const QLineF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawRects(const QRectF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawRects(const QRect *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPoints(const QPoint *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPoints(const QPointF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPolygon(const QPoint *, int, QPaintEngine::PolygonDrawMode)"
            ModifyArgument{
                index: 1
                AsArray{
                    noOffset: true
                    lengthParameter: 2
                }
            }
        }
        ModifyFunction{
            signature: "drawPolygon(const QPointF *, int, QPaintEngine::PolygonDrawMode)"
            ModifyArgument{
                index: 1
                AsArray{
                    noOffset: true
                    lengthParameter: 2
                }
            }
        }
        ModifyFunction{
            signature: "paintDevice()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "painter()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ObjectType{
        name: "QPaintEngineState"
        targetType: "final class"
        generate: "no-shell"
        noMetaType: true
        ExtraIncludes{
            Include{
                fileName: "QPainterPath"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QPaintEngineState()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QPaintEngineState(const QPaintEngineState &)"
            remove: RemoveFlag.All
        }
        ModifyField{
            name: "dirtyFlags"
            write: false
            read: false
        }
        ModifyFunction{
            signature: "painter()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
    }
    
    ValueType{
        name: "QPainterPath"

        EnumType{
            name: "ElementType"
        }
        ModifyFunction{
            signature: "operator|(const QPainterPath &)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator|=(const QPainterPath &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator&(const QPainterPath &)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator-=(const QPainterPath &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "operator+=(const QPainterPath &)"
            rename: "unite"
        }
        ModifyFunction{
            signature: "operator&=(const QPainterPath &)"
            rename: "intersect"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
    }
    
    ValueType{
        name: "QPainter::PixmapFragment"
    }
    
    ValueType{
        name: "QPainterPath::Element"
        ModifyField{
            name: "x"
            write: false
        }
        ModifyField{
            name: "y"
            write: false
        }
        ModifyField{
            name: "type"
            write: false
        }
        Include{
            fileName: "QPainterPath"
            location: Include.Global
        }
        ModifyFunction{
            signature: "operator QPointF()const"
            rename: "toPoint"
        }
    }
    
    ObjectType{
        name: "QPainterPathStroker"
    }
    
    ObjectType{
        name: "QPainter"
        ExtraIncludes{
            Include{
                fileName: "QPainterPath"
                location: Include.Global
            }
            Include{
                fileName: "QPixmap"
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }

        EnumType{
            name: "CompositionMode"
        }

        EnumType{
            name: "RenderHint"
        }

        EnumType{
            name: "PixmapFragmentHint"
        }
        ModifyFunction{
            signature: "device()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "paintEngine()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "drawText(const QPointF &, const QString &, int, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "drawLines(const QVector<QPoint> &)"
            Rename{
                to: "drawLinesFromPoints"
            }
            until: 5
        }
        ModifyFunction{
            signature: "drawLines(const QVector<QPointF> &)"
            Rename{
                to: "drawLinesFromPointsF"
            }
            until: 5
        }
        ModifyFunction{
            signature: "drawLines(const QVector<QLineF> &)"
            Rename{
                to: "drawLinesF"
            }
            until: 5
        }
        ModifyFunction{
            signature: "drawRects(const QVector<QRectF> &)"
            Rename{
                to: "drawRectsF"
            }
            until: 5
        }
        ModifyFunction{
            signature: "drawLines(const QList<QPoint> &)"
            Rename{
                to: "drawLinesFromPoints"
            }
            since: 6
        }
        ModifyFunction{
            signature: "drawLines(const QList<QPointF> &)"
            Rename{
                to: "drawLinesFromPointsF"
            }
            since: 6
        }
        ModifyFunction{
            signature: "drawLines(const QList<QLineF> &)"
            Rename{
                to: "drawLinesF"
            }
            since: 6
        }
        ModifyFunction{
            signature: "drawRects(const QList<QRectF> &)"
            Rename{
                to: "drawRectsF"
            }
            since: 6
        }
        ModifyFunction{
            signature: "initFrom(const QPaintDevice *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setRedirected(const QPaintDevice *, QPaintDevice *, const QPoint &)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "restoreRedirected(const QPaintDevice *)"
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
            until: 5
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPainter_redir__"
                quoteBeforeLine: "}// class"
                until: 5
            }
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPainter___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "QPainter(QPaintDevice *)"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "initialize(%1, true);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.registerDependentObject(this, %1);"}
            }
        }
        ModifyFunction{
            signature: "begin(QPaintDevice *)"
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "initialize(%1, false);"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%0"
                }
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if(%0)\n"+
                              "    QtJambi_LibraryUtilities.internal.registerDependentObject(this, %1);"}
            }
        }
        ModifyFunction{
            signature: "end()"
            InjectCode{
                position: Position.Beginning
                Text{content: "QPaintDevice device = device();"}
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 0
                    metaName: "%0"
                }
                Text{content: "if(%0 && device!=null){\n"+
                              "    QtJambi_LibraryUtilities.internal.unregisterDependentObject(this, device);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "setRedirected(const QPaintDevice*, QPaintDevice*, const QPoint &)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            ModifyArgument{
                index: 2
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%2"
                }
                Text{content: "__rcRedirections.put(%1, %2);"}
            }
            until: 5
        }
        ModifyFunction{
            signature: "restoreRedirected(const QPaintDevice*)"
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "__rcRedirections.remove(%1);"}
            }
            until: 5
        }
        ModifyFunction{
            signature: "drawPixmapFragments(const QPainter::PixmapFragment *, int, const QPixmap &, QPainter::PixmapFragmentHints)"
            ModifyArgument{
                index: 1
                AsArray{
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawConvexPolygon(const QPoint *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawConvexPolygon(const QPointF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawLines(const QLine *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawLines(const QLineF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawLines(const QPoint *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawLines(const QPointF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPoints(const QPoint *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPoints(const QPointF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPolygon(const QPoint *, int, Qt::FillRule)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPolygon(const QPointF *, int, Qt::FillRule)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPolyline(const QPoint *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawPolyline(const QPointF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawRects(const QRect *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "drawRects(const QRectF *, int)"
            ModifyArgument{
                index: 1
                AsArray{
                    varargs: true
                    lengthParameter: 2
                    noOffset: true
                }
            }
        }
        ModifyFunction{
            signature: "redirected(const QPaintDevice*,QPoint*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "drawImage(QRectF,QImage,QRectF,Qt::ImageConversionFlags)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "drawImage(QPointF,QImage,QRectF,Qt::ImageConversionFlags)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "drawRoundRect(QRectF,int,int)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            until: 5
        }
        ModifyFunction{
            signature: "drawRoundedRect(QRectF,qreal,qreal,Qt::SizeMode)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "drawText(QRectF,int,QString,QRectF*)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "drawTiledPixmap(QRectF,QPixmap,QPointF)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "drawText(QRectF,int,QString,QRectF*)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "drawText(QRectF,int,QString,QRectF*)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "fillRect(QRectF,QBrush)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "fillRect(QRectF,QColor)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 2
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "setClipRect(QRectF,Qt::ClipOperation)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
    }
    
    ValueType{
        name: "QPalette"
        noImplicitConstructors: true
        ModifyFunction{
            signature: "QPalette(QColor)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "QPalette(QColor,QColor)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "QPalette(QBrush,QBrush,QBrush,QBrush,QBrush,QBrush,QBrush,QBrush,QBrush)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "QPalette(QColor,QColor,QColor,QColor,QColor,QColor,QColor)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "setColorGroup(QPalette::ColorGroup,QBrush,QBrush,QBrush,QBrush,QBrush,QBrush,QBrush,QBrush,QBrush)"
            noImplicitArguments: true
        }
    }
    
    ValueType{
        name: "QPicture"
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setData(const char*,uint)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter: 2
                    AsArray{}
                }
            }
        }
        ModifyFunction{
            signature: "data()const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "byte[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jint size = jint(__qt_this->size());\n"+
                                  "if(size>0){\n"+
                                  "    %out = %env->NewByteArray(size);\n"+
                                  "    %env->SetByteArrayRegion(%out, 0, size, reinterpret_cast<const jbyte *>(%in));\n"+
                                  "}"}
                }
            }
        }
    }
    
    ObjectType{
        name: "QPictureIO"
        ModifyFunction{
            signature: "QPictureIO(QIODevice*,const char*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setIODevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
        InjectCode{
            Text{content: "private io.qt.core.QByteArray __rc_format;\n"+
                          "\n"+
                          "public QPictureIO(io.qt.core.QIODevice ioDevice, String format){\n"+
                          "    this(ioDevice, new io.qt.core.QByteArray(format));\n"+
                          "}\n"+
                          "\n"+
                          "public QPictureIO(java.lang.String fileName, String format){\n"+
                          "    this(fileName, new io.qt.core.QByteArray(format));\n"+
                          "}\n"+
                          "\n"+
                          "public final void setFormat(String format) {\n"+
                          "    setFormat(new io.qt.core.QByteArray(format));\n"+
                          "}"}
        }
        ModifyFunction{
            signature: "QPictureIO(QIODevice*,const char*)"
            access: Modification.Private
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "io.qt.core.QByteArray"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray& %out_ba = qtjambi_cast<QByteArray&>(%env, %in);\n"+
                                  "const char* %out = %out_ba.data();"}
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "__rc_format = format;"}
            }
        }
        ModifyFunction{
            signature: "QPictureIO(QString,const char*)"
            access: Modification.Private
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "io.qt.core.QByteArray"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray& %out_ba = qtjambi_cast<QByteArray&>(%env, %in);\n"+
                                  "const char* %out = %out_ba.data();"}
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "__rc_format = format;"}
            }
        }
        ModifyFunction{
            signature: "setFormat(const char*)"
            access: Modification.Private
            ModifyArgument{
                index: 1
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "io.qt.core.QByteArray"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QByteArray& %out_ba = qtjambi_cast<QByteArray&>(%env, %in);\n"+
                                  "const char* %out = %out_ba.data();"}
                }
            }
            InjectCode{
                target: CodeClass.Java
                position: Position.End
                Text{content: "__rc_format = arg__1;"}
            }
        }
        until: 5
    }
    
    FunctionalType{
        name: "picture_io_handler"
        javaName: "QPictureIOHandler"
        ExtraIncludes{
            Include{
                fileName: "QtGui/QPictureIO"
                location: Include.Global
            }
        }
        ModifyArgument{
            index: 1
            invalidateAfterUse: true
        }
        until: 5
    }
    
    ValueType{
        name: "QRegion"

        Rejection{functionName: "cleanUp"}
        Rejection{functionName: "cbegin"}
        Rejection{functionName: "rbegin"}
        Rejection{functionName: "crbegin"}
        Rejection{functionName: "cend"}
        Rejection{functionName: "rend"}
        Rejection{functionName: "crend"}
        Rejection{className: "QRegionData"}

        IteratorType{
            name: "const_iterator"
        }

        IteratorType{
            name: "iterator"
            isConst: true
        }

        EnumType{
            name: "RegionType"
        }
        ModifyFunction{
            signature: "QRegion(QBitmap)"
            noImplicitArguments: true
        }
        ModifyFunction{
            signature: "operator&=(QRegion)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator^=(QRegion)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator|=(QRegion)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator&(QRegion)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator&(QRect)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator^(QRegion)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator|(QRegion)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator+=(QRect)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator+=(QRegion)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator-=(QRegion)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator&=(QRect)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setRects(const QRect*,int)"
            ModifyArgument{
                index: 1
                AsArray{
                    lengthParameter: 2
                    noOffset: true
                    varargs: true
                }
            }
        }
    }
    
    ObjectType{
        name: "QPdfWriter"
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QPen"
        ExtraIncludes{
            Include{
                fileName: "QBrush"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "QPen(QColor)"
            ModifyArgument{
                index: 1
                InhibitImplicitCall{type: "QString"}
                InhibitImplicitCall{type: "int"}
            }
        }
    }
    
    ValueType{
        name: "QPixmap"
        Rejection{
            functionName: "grabWidget"
        }
        Rejection{
            functionName: "grabWindow"
        }
        threadAffinity: "pixmap"
        noImplicitConstructors: true
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "fill(const QPaintDevice *, const QPoint &)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "fill(const QPaintDevice *, int, int)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "fromImageInPlace(QImage &, Qt::ImageConversionFlags)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QPixmap(const char*const*)"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.String[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jsize length = %env->GetArrayLength(jarray(%in));\n"+
                                  "QVector<QSharedPointer<J2CStringBuffer>> %inVec;\n"+
                                  "const char** %out = nullptr;\n"+
                                  "QScopedArrayPointer<const char*> %outPtr(%out = new const char*[quint32(length)]);\n"+
                                  "for(jsize i=0; i<length; ++i){\n"+
                                  "    J2CStringBuffer* b = new J2CStringBuffer(%env, jstring(%env->GetObjectArrayElement(jobjectArray(%in), i)));\n"+
                                  "    %out[i] = *b;\n"+
                                  "    %inVec << QSharedPointer<J2CStringBuffer>(b);\n"+
                                  "}"}
                }
            }
        }
        ModifyFunction{
            signature: "fill(const QColor &)"
            ModifyArgument{
                index: 1
                ReplaceDefaultExpression{
                    expression: "new io.qt.gui.QColor(io.qt.core.Qt.GlobalColor.white)"
                }
            }
        }
        ModifyFunction{
            signature: "loadFromData(const uchar*,uint,const char*,Qt::ImageConversionFlags)"
            ModifyArgument{
                index: 1
                AsBuffer{
                    lengthParameter: 2
                    AsArray{}
                }
            }
        }
        ModifyFunction{
            signature: "loadFromData(QByteArray,const char*,Qt::ImageConversionFlags)"
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
        }
    }
    
    ObjectType{
        name: "QPixmapCache"
        ValueType{
            name: "Key"
        }
        ModifyFunction{
            signature: "find(QString)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "find(QString,QPixmap&)"
            remove: RemoveFlag.All
            until: 5
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPixmapCache___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QPolygon"
        noImplicitConstructors: true
        ExtraIncludes{
            Include{
                fileName: "io.qt.core.QPoint"
                location: Include.Java
            }
        }
        Rejection{
            functionName: "points"
        }
        ModifyFunction{
            signature: "QPolygon(int, const int *)"
            ModifyArgument{
                index: 2
                NoNullPointer{}
                AsBuffer{
                    lengthParameter: 1
                    AsArray{
                        noOffset: true
                        varargs: true
                    }
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Position1
                ArgumentMap{
                    metaName: "%1"
                    index: 1
                }
                Text{content: "%1 = qFloor(%1/2.);"}
            }
        }
        ModifyFunction{
            signature: "setPoints(int, const int *)"
            ModifyArgument{
                index: 2
                NoNullPointer{}
                AsBuffer{
                    lengthParameter: 1
                    AsArray{
                        noOffset: true
                        varargs: true
                    }
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Position1
                ArgumentMap{
                    metaName: "%1"
                    index: 1
                }
                Text{content: "%1 = qFloor(%1/2.);"}
            }
        }
        ModifyFunction{
            signature: "putPoints(int, int, const int *)"
            ModifyArgument{
                index: 3
                NoNullPointer{}
                AsBuffer{
                    lengthParameter: 2
                    AsArray{
                        noOffset: true
                        varargs: true
                    }
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.Position1
                ArgumentMap{
                    metaName: "%2"
                    index: 2
                }
                Text{content: "%2 = qFloor(%2/2.);"}
            }
        }
        ModifyFunction{
            signature: "point(int, int *, int *) const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setPoints(int, int, int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "putPoints(int, int, int, int)"
            remove: RemoveFlag.All
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPolygon___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QPolygonF"
        noImplicitConstructors: true
        ExtraIncludes{
            Include{
                fileName: "io.qt.core.QPointF"
                location: Include.Java
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPolygonF___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QQuaternion"
        ModifyFunction{
            signature: "getAxisAndAngle(float *, float *, float *, float *) const"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        ModifyFunction{
            signature: "operator*=(const QQuaternion &)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator*=(float)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator+=(const QQuaternion &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QQuaternion &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "operator/=(float)"
            rename: "divide"
        }
        ModifyFunction{
            signature: "getAxes(QVector3D *, QVector3D *, QVector3D *) const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector3D xAxis;\n"+
                                  "QVector3D* %out = &xAxis;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector3D yAxis;\n"+
                                  "QVector3D* %out = &yAxis;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector3D zAxis;\n"+
                                  "QVector3D* %out = &zAxis;"}
                }
            }
            ModifyArgument{
                index: 0
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "io.qt.gui.QQuaternion$Axes"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtGui::QQuaternion$Axes::newInstance(%env, qtjambi_cast<jobject>(%env, xAxis), qtjambi_cast<jobject>(%env, yAxis), qtjambi_cast<jobject>(%env, zAxis));"}
                }
            }
        }
        ModifyFunction{
            signature: "getAxisAndAngle(QVector3D *, float *) const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QVector3D axis;\n"+
                                  "QVector3D* %out = &axis;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "float fValue = 0;\n"+
                                  "float* %out = &fValue;"}
                }
            }
            ModifyArgument{
                index: 0
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "io.qt.gui.QQuaternion$AxisAndAngle"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtGui::QQuaternion$AxisAndAngle::newInstance(%env, qtjambi_cast<jobject>(%env, axis), fValue);"}
                }
            }
        }
        ModifyFunction{
            signature: "getEulerAngles(float *, float *, float *) const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "float aValue = 0;\n"+
                                  "float* %out = &aValue;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "float bValue = 0;\n"+
                                  "float* %out = &bValue;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "float cValue = 0;\n"+
                                  "float* %out = &cValue;"}
                }
            }
            ModifyArgument{
                index: 0
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "io.qt.gui.QQuaternion$EulerAngles"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtGui::QQuaternion$EulerAngles::newInstance(%env, aValue, bValue, cValue);"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QQuaternion___"
                quoteBeforeLine: "}// class"
            }
        }
    }

    GlobalFunction{
        signature: "operator*(const QQuaternion&, const QVector3D&)"
        targetType: "QQuaternion"
    }
    
    ValueType{
        name: "QRadialGradient"
        polymorphicIdExpression: "%1->type() == QGradient::RadialGradient"
        CustomConstructor{
            Text{content: "if(copy)\n"+
                          "    return new(placement) QRadialGradient(copy->center(), copy->radius(), copy->focalPoint());\n"+
                          "else\n"+
                          "    return new(placement) QRadialGradient();"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) QRadialGradient(copy->center(), copy->radius(), copy->focalPoint());"}
        }
        CustomConstructor{
            type: CustomConstructor.Default
            Text{content: "new(placement) QRadialGradient();"}
        }
    }
    
    ObjectType{
        name: "QRegExpValidator"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QScreen"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.Native
            since: [6, 2]
            Text{content: "// QScreen::resolveInterface(const char * name, int revision) const\n"+
                          "extern \"C\" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QScreen_nativeInterface)\n"+
                          "(JNIEnv *__jni_env,\n"+
                          " jobject __this,\n"+
                          " QtJambiNativeID __this_nativeId,\n"+
                          " jclass name0)\n"+
                          "{\n"+
                          "    Q_UNUSED(__this)\n"+
                          "    jobject __java_return_value{0};\n"+
                          "    QTJAMBI_TRY{\n"+
                          "        struct Screen : QScreen{\n"+
                          "            jobject nativeInterface(JNIEnv *__jni_env, QtJambiNativeID __this_nativeId, jclass name0)const{\n"+
                          "                CoreAPI::NITypeInfo info = CoreAPI::getNativeInterfaceInfo(__jni_env, name0);\n"+
                          "                const char* __qt_name0 = info.name;\n"+
                          "                int __qt_revision1 = info.revision;\n"+
                          "                void* __qt_return_value = resolveInterface(__qt_name0, static_cast<int>(__qt_revision1));\n"+
                          "                jobject __java_return_value = QtJambiAPI::convertNativeToJavaObjectAsWrapper(__jni_env, __qt_return_value, name0);\n"+
                          "                QtJambiAPI::registerDependency(__jni_env, __java_return_value, __this_nativeId);\n"+
                          "                return __java_return_value;\n"+
                          "            }\n"+
                          "        };\n"+
                          "\n"+
                          "        const Screen *__qt_this = QtJambiAPI::objectFromNativeId<Screen>(__this_nativeId);\n"+
                          "        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);\n"+
                          "        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL(\"QScreen::resolveInterface(const char * name, int revision) const\", __qt_this)\n"+
                          "        __java_return_value = __qt_this->nativeInterface(__jni_env, __this_nativeId, name0);\n"+
                          "    }QTJAMBI_CATCH(const JavaException& exn){\n"+
                          "        exn.raiseInJava(__jni_env);\n"+
                          "    }QTJAMBI_TRY_END\n"+
                          "    return __java_return_value;\n"+
                          "\n"+
                          "}"}
        }
        InjectCode{
            target: CodeClass.Java
            since: [6, 2]
            Text{content: "@QtUninvokable\n"+
                          "public final <QNativeInterface extends QtObjectInterface> QNativeInterface nativeInterface(java.lang.Class<QNativeInterface> name){\n"+
                          "    return nativeInterface(QtJambi_LibraryUtilities.internal.nativeId(this), name);\n"+
                          "}\n"+
                          "\n"+
                          "@QtUninvokable\n"+
                          "private native <QNativeInterface extends QtObjectInterface> QNativeInterface nativeInterface(long __this__nativeId, java.lang.Class<QNativeInterface> name);"}
        }
    }
    
    ObjectType{
        name: "QStandardItem"
        ModifyFunction{
            signature: "appendColumn(const QList<QStandardItem *> &)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "appendRow(const QList<QStandardItem *> &)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "appendRow(QStandardItem *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertColumn(int, const QList<QStandardItem *> &)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int, const QList<QStandardItem *> &)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int, QStandardItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setChild(int,int,QStandardItem *)"
            ModifyArgument{
                index: 3
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setChild(int,QStandardItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "takeChild(int,int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeColumn(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeRow(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
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
        name: "QStandardItemModel"
        ModifyFunction{
            signature: "insertColumn(int,const QModelIndex &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "insertRow(int,const QModelIndex &)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "parent()const"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QStringList"
                location: Include.Global
            }
            Include{
                fileName: "QSize"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "appendColumn(const QList<QStandardItem *>&)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "takeColumn(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeRow(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeHorizontalHeaderItem(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeVerticalHeaderItem(int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "takeItem(int,int)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Default
                }
            }
        }
        ModifyFunction{
            signature: "appendRow(const QList<QStandardItem *>&)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "appendRow(QStandardItem *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertColumn(int, const QList<QStandardItem *>&)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int, const QList<QStandardItem *>&)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "insertRow(int, QStandardItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setHorizontalHeaderItem(int, QStandardItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setItem(int, int, QStandardItem *)"
            ModifyArgument{
                index: 3
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setItem(int, QStandardItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setItemPrototype(const QStandardItem *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setVerticalHeaderItem(int, QStandardItem *)"
            ModifyArgument{
                index: 2
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ValueType{
        name: "QStaticText"
    }
    
    ObjectType{
        name: "QStyleHints"
    }
    
    ValueType{
        name: "QSurfaceFormat"
    }
    
    ObjectType{
        name: "QSyntaxHighlighter"
        ModifyFunction{
            signature: "setCurrentBlockUserData(QTextBlockUserData*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "setDocument(QTextDocument*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDocument"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "setFormat(int,int,QFont)"
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
        ModifyFunction{
            signature: "setFormat(int,int,QColor)"
            ModifyArgument{
                index: 3
                noImplicitCalls: true
            }
        }
    }
    
    ValueType{
        name: "QTextBlock"
        implementing: "java.lang.Iterable<QTextFragment>"
        ModifyFunction{
            signature: "setUserData(QTextBlockUserData *)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Java
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QTextBlock___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ValueType{
        name: "QTextBlockFormat"
    }
    
    ValueType{
        name: "QTextTableCellFormat"
    }
    
    ValueType{
        name: "QTextCharFormat"
        ModifyFunction{
            signature: "isValid()const"
            access: Modification.NonFinal
        }
        ModifyFunction{
            signature: "anchorName()const"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "setAnchorName(QString)"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ValueType{
        name: "QTextFormat"
        ModifyFunction{
            signature: "isValid()const"
            access: Modification.NonFinal
        }
    }
    
    ValueType{
        name: "QTextListFormat"
    }
    
    ValueType{
        name: "QTextFrame::iterator"
        Include{
            fileName: "QTextFrame"
            location: Include.Global
        }
        ModifyFunction{
            signature: "operator++(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator--(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QTextFrame::iterator)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "operator++()"
            rename: "next"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        ModifyFunction{
            signature: "operator--()"
            rename: "previous"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        ModifyFunction{
            signature: "QTextFrame::iterator()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QTextFrame::iterator(const QTextFrame::iterator &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QTextBlock::iterator"
        Include{
            fileName: "QTextBlock"
            location: Include.Global
        }
        ModifyFunction{
            signature: "operator++()"
            rename: "next"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        ModifyFunction{
            signature: "operator--()"
            rename: "previous"
            ModifyArgument{
                index: "return"
                ReplaceType{
                    modifiedType: "void"
                }
            }
        }
        ModifyFunction{
            signature: "operator++(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator--(int)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator=(QTextBlock::iterator)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "QTextBlock::iterator()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QTextBlock::iterator(const QTextBlock::iterator &)"
            remove: RemoveFlag.All
        }
    }
    
    ValueType{
        name: "QTextCursor"
        threadAffinity: "%1->isNull() ? nullptr : %1->document()"
        ExtraIncludes{
            Include{
                fileName: "QTextBlock"
                location: Include.Global
            }
            Include{
                fileName: "QTextDocumentFragment"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "createList(QTextListFormat::Style)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ModifyFunction{
            signature: "createList(QTextListFormat)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
        ExtraIncludes{
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QTextCursor___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "selectedTableCells(int*,int*,int*,int*)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int a = 0;\n"+
                                  "int* %out = &a;"}
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int b = 0;\n"+
                                  "int* %out = &b;"}
                }
            }
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int c = 0;\n"+
                                  "int* %out = &c;"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int d = 0;\n"+
                                  "int* %out = &d;"}
                }
            }
            ModifyArgument{
                index: 0
                NoNullPointer{}
                ReplaceType{
                    modifiedType: "io.qt.gui.QTextCursor$SelectedTableCells"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = Java::QtGui::QTextCursor$SelectedTableCells::newInstance(%env,  a, b, c, d);"}
                }
            }
        }
    }
    
    ValueType{
        name: "QTextLayout::FormatRange"
        Include{
            fileName: "QTextLayout"
            location: Include.Global
        }
    }
    
    ValueType{
        name: "QTextTableCell"
        ExtraIncludes{
            Include{
                fileName: "QTextCursor"
                location: Include.Global
            }
        }
    }
    
    ValueType{
        name: "QTextOption::Tab"
    }
    
    ObjectType{
        name: "QTextBlockGroup"
    }
    
    ObjectType{
        name: "QTextBlockUserData"
    }
    
    ObjectType{
        name: "QTextItem"
    }
    
    ObjectType{
        name: "QTextList"
        ModifyFunction{
            signature: "format()const"
            rename: "textListFormat"
        }
        ModifyFunction{
            signature: "isEmpty()const"
            remove: RemoveFlag.All
            until: 5
        }
    }
    
    ObjectType{
        name: "QTextObject"
    }
    
    InterfaceType{
        name: "QTextObjectInterface"
        ModifyFunction{
            signature: "drawObject(QPainter*,QRectF,QTextDocument*,int,QTextFormat)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
    }
    
    ObjectType{
        name: "QTextFrame"
        ExtraIncludes{
            Include{
                fileName: "QTextCursor"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QTextLayout"
        ExtraIncludes{
            Include{
                fileName: "QTextOption"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "draw(QPainter *, const QPointF &, const QVector<QTextLayout::FormatRange> &, const QRectF &) const"
            Access{
                modifiers: Modification.Private
            }
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "draw(QPainter *, const QPointF &, const QList<QTextLayout::FormatRange> &, const QRectF &) const"
            Access{
                modifiers: Modification.Private
            }
            ModifyArgument{
                index: 3
                ReplaceDefaultExpression{
                    expression: "java.util.Collections.emptyList()"
                }
            }
            since: 6
        }
    }

    EnumType{
        name: "QTextLayout::GlyphRunRetrievalFlag"
        since: [6,5]
    }
    
    InterfaceType{
        name: "QAbstractUndoItem"
    }
    
    FunctionalType{
        name: "QTextDocument::ResourceProvider"
        since: [6, 1]
    }
    
    ObjectType{
        name: "QTextDocument"
        isValueOwner: true
        ExtraIncludes{
            Include{
                fileName: "QTextBlock"
                location: Include.Global
            }
            Include{
                fileName: "QTextFormat"
                location: Include.Global
            }
            Include{
                fileName: "QTextCursor"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "setDocumentLayout(QAbstractTextDocumentLayout*)"
            ModifyArgument{
                index: 1
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "createObject(QTextFormat)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "clone(QObject*) const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
            }
        }
    }
    
    ObjectType{
        name: "QTextDocumentWriter"
        ModifyFunction{
            signature: "setCodec(QTextCodec*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcCodec"
                    action: ReferenceCount.Set
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "setDevice(QIODevice*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcDevice"
                    action: ReferenceCount.Set
                }
            }
        }
    }
    
    ObjectType{
        name: "QTextTable"
        ModifyFunction{
            signature: "format() const"
            Rename{
                to: "tableFormat"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QTextCursor"
                location: Include.Global
            }
        }
    }
    
    ValueType{
        name: "QTextFragment"
    }
    
    ValueType{
        name: "QTextInlineObject"
        ModifyFunction{
            signature: "QTextInlineObject()"
            Access{
                modifiers: Modification.Private
            }
        }
    }
    
    ValueType{
        name: "QTextDocumentFragment"
    }
    
    ValueType{
        name: "QTextOption"
    }
    
    ValueType{
        name: "QTextLine"
        ModifyFunction{
            signature: "cursorToX(int*,QTextLine::Edge)const"
            Remove{
            }
        }
        ModifyFunction{
            signature: "draw(QPainter*,QPointF,const QTextLayout::FormatRange*)const"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "QTextLayout::FormatRange *%out = nullptr;"}
                }
            }
            until: 5
        }
    }
    
    ValueType{
        name: "QTextTableFormat"
    }
    
    ValueType{
        name: "QTextImageFormat"
    }
    
    ValueType{
        name: "QTextFrameFormat"
        ModifyFunction{
            signature: "isValid()const"
            access: Modification.NonFinal
        }
    }
    
    ValueType{
        name: "QTextLength"
    }
    
    ObjectType{
        name: "QTouchDevice"
    }
    
    ValueType{
        name: "QTransform"
        Rejection{functionName: "asAffineMatrix"}
        EnumType{
            name: "TransformationType"
        }
        ModifyFunction{
            signature: "map(int,int,int*,int*)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "map(qreal,qreal,qreal*,qreal*)const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "operator*=(qreal)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator+=(qreal)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(qreal)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "operator/=(qreal)"
            rename: "divide"
        }
        ModifyFunction{
            signature: "operator*(QTransform)const"
            rename: "multiplied"
        }
        ModifyFunction{
            signature: "operator*=(QTransform)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "inverted(bool*)const"
            ModifyArgument{
                index: 1
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "bool ok(false);\n"+
                                  "bool* %out = &ok;"}
                }
            }
            ModifyArgument{
                index: 0
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "%out = ok ? qtjambi_cast<jobject>(%env, %in) : nullptr;"}
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QTransform___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    ObjectType{
        name: "QValidator"
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QValidator___"
                quoteBeforeLine: "}// class"
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "validate(QString&,int&)const"
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "io.qt.gui.QValidator$QValidationData"
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    InsertTemplate{
                        name: "gui.convert_validationdata_to_java"
                        Replace{
                            from: "%STRING"
                            to: "%1"
                        }
                        Replace{
                            from: "%POS"
                            to: "%2"
                        }
                    }
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "gui.convert_validationdata_to_string"
                    }
                }
            }
            ModifyArgument{
                index: 2
                RemoveArgument{
                }
                ConversionRule{
                    codeClass: CodeClass.Shell
                    Text{content: "// nothing needed"}
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    InsertTemplate{
                        name: "gui.convert_validationdata_to_pos"
                        Replace{
                            from: "%VALIDATIONDATA"
                            to: "%1"
                        }
                    }
                }
            }
            InjectCode{
                target: CodeClass.Native
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                InsertTemplate{
                    name: "gui.cleanup_validationdata_from_java"
                }
            }
            InjectCode{
                target: CodeClass.Shell
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%STRING"
                }
                ArgumentMap{
                    index: 2
                    metaName: "%POS"
                }
                InsertTemplate{
                    name: "gui.cleanup_validationdata_to_java"
                }
            }
        }
    }
    
    ValueType{
        name: "QVector2D"
        ModifyFunction{
            signature: "operator[](int)"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "i"
                }
                Text{content: "if(i<0 || i>2)\n"+
                              "    throw new IndexOutOfBoundsException(i);"}
            }
        }
        ModifyFunction{
            signature: "operator[](int)const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "i"
                }
                Text{content: "if(i<0 || i>2)\n"+
                              "    throw new IndexOutOfBoundsException(i);"}
            }
        }
        ModifyFunction{
            signature: "operator*=(const QVector2D &)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator*=(float)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator+=(const QVector2D &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QVector2D &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "operator/=(float)"
            rename: "divide"
        }
        ModifyFunction{
            signature: "operator/=(QVector2D)"
            rename: "divide"
        }
    }
    
    ValueType{
        name: "QVector3D"
        ModifyFunction{
            signature: "QVector3D(QVector2D)"
            isForcedExplicit: true
        }
        ModifyFunction{
            signature: "operator[](int)"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "i"
                }
                Text{content: "if(i<0 || i>3)\n"+
                              "    throw new IndexOutOfBoundsException(i);"}
            }
        }
        ModifyFunction{
            signature: "operator[](int)const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "i"
                }
                Text{content: "if(i<0 || i>3)\n"+
                              "    throw new IndexOutOfBoundsException(i);"}
            }
        }
        ModifyFunction{
            signature: "operator*=(const QVector3D &)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator*=(float)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator+=(const QVector3D &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QVector3D &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "operator/=(float)"
            rename: "divide"
        }
        ModifyFunction{
            signature: "operator/=(QVector3D)"
            rename: "divide"
        }
    }
    
    ValueType{
        name: "QVector4D"
        ModifyFunction{
            signature: "QVector4D(QVector3D)"
            isForcedExplicit: true
        }
        ModifyFunction{
            signature: "QVector4D(QVector2D)"
            isForcedExplicit: true
        }
        ModifyFunction{
            signature: "operator[](int)"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "i"
                }
                Text{content: "if(i<0 || i>4)\n"+
                              "    throw new IndexOutOfBoundsException(i);"}
            }
        }
        ModifyFunction{
            signature: "operator[](int)const"
            InjectCode{
                target: CodeClass.Java
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "i"
                }
                Text{content: "if(i<0 || i>4)\n"+
                              "    throw new IndexOutOfBoundsException(i);"}
            }
        }
        ModifyFunction{
            signature: "operator*=(const QVector4D &)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator*=(float)"
            rename: "multiply"
        }
        ModifyFunction{
            signature: "operator+=(const QVector4D &)"
            rename: "add"
        }
        ModifyFunction{
            signature: "operator-=(const QVector4D &)"
            rename: "subtract"
        }
        ModifyFunction{
            signature: "operator/=(float)"
            rename: "divide"
        }
        ModifyFunction{
            signature: "operator/=(QVector4D)"
            rename: "divide"
        }
    }
    
    ObjectType{
        name: "QWindow"
        Rejection{
            functionName: "vulkanInstance"
        }
        Rejection{
            functionName: "surfaceHandle"
        }
        ModifyFunction{
            signature: "setVisible(bool)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setCursor(QCursor)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setIcon(QIcon)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "show()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "hide()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "showFullScreen()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "showMaximized()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "showMinimized()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "showNormal()"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "requestUpdate()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "setWindowState(Qt::WindowState)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "setWindowStates(Qt::WindowStates)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "nativeEvent(const QByteArray &, void *, long *)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "nativeEvent(const QByteArray &, void *, qintptr *)"
            remove: RemoveFlag.All
            since: 6
        }
        ModifyFunction{
            signature: "setVulkanInstance(QVulkanInstance *)"
            remove: RemoveFlag.All
            since: [5, 10]
        }
        ModifyFunction{
            signature: "vulkanInstance() const"
            remove: RemoveFlag.All
            since: [5, 10]
        }
        ModifyFunction{
            signature: "parent()const"
            rename: "parentWindow"
            until: 5
        }
        ModifyFunction{
            signature: "parent(QWindow::AncestorMode)const"
            ModifyArgument{
                index: 1
                RemoveDefaultExpression{
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setParent(QWindow*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setScreen(QScreen*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setTransientParent(QWindow*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "closeEvent(QCloseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: [6, 1]
        }
        ModifyFunction{
            signature: "paintEvent(QPaintEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            since: [6, 1]
        }
        ModifyFunction{
            signature: "exposeEvent(QExposeEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "focusInEvent(QFocusEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "focusOutEvent(QFocusEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "hideEvent(QHideEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyPressEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "keyReleaseEvent(QKeyEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseDoubleClickEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseMoveEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mousePressEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "mouseReleaseEvent(QMouseEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "moveEvent(QMoveEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "resizeEvent(QResizeEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "showEvent(QShowEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "tabletEvent(QTabletEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "touchEvent(QTouchEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ModifyFunction{
            signature: "wheelEvent(QWheelEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QWindow___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "fromWinId(WId)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "requestUpdate()"
            threadAffinity: Affinity.UI
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            rename: "nativeInterface"
            access: Modification.Public
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends io.qt.@Nullable QtObjectInterface> QNativeInterface"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, %in, %1);"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Class<QNativeInterface>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "CoreAPI::NITypeInfo info = CoreAPI::getNativeInterfaceInfo(%env, %in);\n"+
                                  "const char* %out = info.name;"}
                }
            }
            ModifyArgument{
                index: 2
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out = info.revision;"}
                }
                RemoveArgument{
                }
            }
            since: [6, 2]
        }
    }
    
    ValueType{
        name: "QTouchEvent::TouchPoint"
        until: 5
    }
    
    ObjectType{
        name: "QRangeCollection"
        since: 6
    }
    
    ObjectType{
        name: "QRegularExpressionValidator"
        ExtraIncludes{
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
    }
    
    ObjectType{
        name: "QSessionManager"
    }
    
    
    ObjectType{
        name: "QRasterPaintEngine"
    }
    
    ValueType{
        name: "QPageSize"
    }
    
    ValueType{
        name: "QPageRanges"
        since: 6
    }
    
    ValueType{
        name: "QPageRanges::Range"
        since: 6
    }
    
    ValueType{
        name: "QPixelFormat"

        Rejection{functionName: "get"}
        Rejection{functionName: "set"}

        EnumType{
            name: "AlphaPosition"
        }

        EnumType{
            name: "AlphaPremultiplied"
        }

        EnumType{
            name: "AlphaUsage"
        }

        EnumType{
            name: "ByteOrder"
        }

        EnumType{
            name: "ColorModel"
        }

        EnumType{
            name: "TypeInterpretation"
        }

        EnumType{
            name: "YUVLayout"
        }
        CustomConstructor{
            Text{content: "if(copy) return new(placement) QPixelFormat(*copy); else return new(placement) QPixelFormat();"}
        }
        CustomConstructor{
            type: CustomConstructor.Copy
            Text{content: "new(placement) quint64(*reinterpret_cast<const quint64*>(copy));"}
        }
        CustomConstructor{
            type: CustomConstructor.Move
            Text{content: "new(placement) quint64(std::move(*reinterpret_cast<quint64*>(copy)));"}
        }
    }
    
    ObjectType{
        name: "QRasterWindow"
    }
    
    ObjectType{
        name: "QPaintDeviceWindow"
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "event(QEvent *)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "width()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "height()const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "devicePixelRatio()const"
            remove: RemoveFlag.All
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPaintDeviceWindow___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "paintEvent(QPaintEvent*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
            isPaintMethod: true
        }
    }
    
    Rejection{
        className: "QPlatformGraphicsBuffer"
    }
    
    Rejection{
        className: "QPlatformSystemTrayIcon"
    }
    
    ValueType{
        name: "QPointingDeviceUniqueId"
        since: [5, 8]
    }
    
    ObjectType{
        name: "QApplicationStateChangeEvent"
        ModifyFunction{
            signature: "operator=(const QApplicationStateChangeEvent &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    ObjectType{
        name: "QScreenOrientationChangeEvent"
        ModifyFunction{
            signature: "operator=(const QScreenOrientationChangeEvent &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
    }
    
    NamespaceType{
        name: "QXcbWindowFunctions"

        EnumType{
            name: "WmWindowType"
        }
        ModifyFunction{
            signature: "setWmWindowTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setWmWindowRoleIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setWmWindowIconTextIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "visualIdIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setWmWindowIconText(QWindow*,QString)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWmWindowRole(QWindow*,QByteArray)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setWmWindowType(QWindow*,QXcbWindowFunctions::WmWindowType)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 11]
        until: 5
    }
    
    NamespaceType{
        name: "QXcbScreenFunctions"
        ModifyFunction{
            signature: "virtualDesktopNumberIdentifier()"
            remove: RemoveFlag.All
        }
        since: [5, 11]
        until: 5
    }
    
    NamespaceType{
        name: "QWindowsWindowFunctions"

        Rejection{className: "SetTouchWindowTouchType"}
        Rejection{className: "SetHasBorderInFullScreen"}
        Rejection{className: "SetHasBorderInFullScreenDefault"}
        Rejection{className: "SetWindowActivationBehaviorType"}
        Rejection{className: "IsTabletModeType"}

        EnumType{
            name: "TouchWindowTouchType"
        }

        EnumType{
            name: "WindowActivationBehavior"
        }
        ModifyFunction{
            signature: "setTouchWindowTouchTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setHasBorderInFullScreenIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setWindowActivationBehaviorIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isTabletModeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setHasBorderInFullScreen(QWindow*,bool)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setTouchWindowTouchType(QWindow*,QWindowsWindowFunctions::TouchWindowTouchTypes)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 11]
        until: 5
    }
    
    NamespaceType{
        name: "QXcbIntegrationFunctions"
        ModifyFunction{
            signature: "xEmbedSystemTrayVisualHasAlphaChannelIdentifier()"
            remove: RemoveFlag.All
        }
        since: [5, 11]
        until: 5
    }
    
    NamespaceType{
        name: "QCocoaWindowFunctions"
        ModifyFunction{
            signature: "bottomLeftClippedByNSWindowOffsetIdentifier()"
            remove: RemoveFlag.All
        }
        since: [5, 11]
        until: 5
    }
    
    NamespaceType{
        name: "QWaylandWindowFunctions"
        ModifyFunction{
            signature: "setSyncIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setDeSyncIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "isSyncIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setDeSync(QWindow*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setSync(QWindow*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: [5, 11]
        until: 5
    }
    
    NamespaceType{
        name: "QEglFSFunctions"
        ModifyFunction{
            signature: "loadKeymapTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vsp2AddLayerTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vsp2RemoveLayerTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vsp2SetLayerBufferTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vsp2SetLayerPositionTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vsp2SetLayerAlphaTypeIdentifier()"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "vsp2AddBlendListenerTypeIdentifier()"
            remove: RemoveFlag.All
        }
        since: [5, 11]
        until: 5
    }
    
    ValueType{
        name: "QColorSpace"
        CustomConstructor{
            Text{content: "return new(placement) QColorSpace(copy->primaries(), copy->transferFunction(), copy->gamma());"}
        }
        since: [5, 14]
    }
    
    ValueType{
        name: "QColorTransform"
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "hash_type qHash(const QColorTransform &value, hash_type seed = 0);"}
        }
        since: [5, 14]
    }
    
    EnumType{
        name: "QColorSpace::NamedColorSpace"
        since: [5, 14]
    }
    
    EnumType{
        name: "QColorSpace::Primaries"
        since: [5, 14]
    }
    
    EnumType{
        name: "QColorSpace::TransferFunction"
        since: [5, 14]
    }
    
    ObjectType{
        name: "QGenericPlugin"
        ModifyFunction{
            signature: "create(QString,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        InjectCode{
            target: CodeClass.MetaInfo
            position: Position.Position1
            Text{content: "registerInterfaceID(typeId, QGenericPluginFactoryInterface_iid);"}
        }
    }
    
    ObjectType{
        name: "QGenericPluginFactory"
        ModifyFunction{
            signature: "create(QString,QString)"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
    }
    
    ObjectType{
        name: "QActionGroup"
        ModifyFunction{
            signature: "addAction(QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Add
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.setReferenceCount(%1, QAction.class, \"__rcActionGroup\", false, false, this);"}
            }
        }
        ModifyFunction{
            signature: "removeAction(QAction*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcActions"
                    action: ReferenceCount.Take
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "QtJambi_LibraryUtilities.internal.setReferenceCount(%1, QAction.class, \"__rcActionGroup\", false, false, null);"}
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QAction"
        ExtraIncludes{
            Include{
                fileName: "QtGui/QGuiApplication"
                location: Include.Global
            }
            Include{
                fileName: "utils_p.h"
                location: Include.Local
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QAction___"
                quoteBeforeLine: "}// class"
            }
        }
        ModifyFunction{
            signature: "setActionGroup(QActionGroup*)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
            InjectCode{
                position: Position.End
                ArgumentMap{
                    index: 1
                    metaName: "%1"
                }
                Text{content: "if (__rcActionGroup != null) {\n"+
                              "    QtJambi_LibraryUtilities.internal.removeFromCollectionReferenceCount(__rcActionGroup, QActionGroup.class, \"__rcActions\", false, this);\n"+
                              "}\n"+
                              "__rcActionGroup = %1;\n"+
                              "if (__rcActionGroup != null) {\n"+
                              "    QtJambi_LibraryUtilities.internal.addReferenceCount(__rcActionGroup, QActionGroup.class, \"__rcActions\", false, false, this);\n"+
                              "}"}
            }
        }
        ModifyFunction{
            signature: "setMenu<T>(T)"
            Instantiation{
                threadAffinity: true
                Argument{
                    type: "QObject*"
                }
                ModifyArgument{
                    index: 1
                    ReplaceType{
                        modifiedType: "io.qt.widgets.@Nullable QMenu"
                    }
                    ReferenceCount{
                        variableName: "__rcMenu"
                        action: ReferenceCount.Set
                    }
                    ConversionRule{
                        codeClass: CodeClass.Native
                        Text{content: "static bool iswidgetApplication = QGuiApplication::instance()->inherits(\"QApplication\");\n"+
                                      "if(iswidgetApplication && %in && !Java::QtWidgets::QMenu::isInstanceOf(%env, %in)){\n"+
                                      "    JavaException::raiseIllegalArgumentException(%env, \"QAction.setMenu(menu) expects a QMenu in widget applications.\" QTJAMBI_STACKTRACEINFO );\n"+
                                      "}\n"+
                                      "QObject* %out = QtJambiAPI::convertJavaObjectToQObject(%env, %in);"}
                    }
                }
            }
        }
        ModifyFunction{
            signature: "menu<T>()const"
            Instantiation{
                threadAffinity: true
                Argument{
                    type: "QObject*"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "io.qt.widgets.@Nullable QMenu"
                    }
                }
            }
        }
        ModifyFunction{
            signature: "parentWidget<T>()const"
            Instantiation{
                threadAffinity: true
                Argument{
                    type: "QObject*"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "io.qt.widgets.@Nullable QWidget"
                    }
                }
            }
        }
        ModifyFunction{
            signature: "associatedWidgets<T>()const"
            Instantiation{
                threadAffinity: true
                Argument{
                    type: "QObject*"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "io.qt.core.@NonNull QList<io.qt.widgets.@Nullable QWidget>"
                    }
                }
            }
        }
        ModifyFunction{
            signature: "associatedGraphicsWidgets<T>()const"
            Instantiation{
                threadAffinity: true
                Argument{
                    type: "QObject*"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "io.qt.core.@NonNull QList<io.qt.widgets.@Nullable QGraphicsWidget>"
                    }
                }
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QUndoCommand"
        ModifyFunction{
            signature: "mergeWith(const QUndoCommand*)"
            ModifyArgument{
                index: 1
                invalidateAfterUse: true
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QUndoStack"
        ModifyFunction{
            signature: "createUndoAction(QObject *, const QString &) const"
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
        ModifyFunction{
            signature: "createRedoAction(QObject *, const QString &) const"
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
        ModifyFunction{
            signature: "push(QUndoCommand *)"
            threadAffinity: true
            ModifyArgument{
                index: 1
                threadAffinity: true
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Cpp
                }
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QUndoGroup"
        ModifyFunction{
            signature: "createUndoAction(QObject *, const QString &) const"
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
        ModifyFunction{
            signature: "createRedoAction(QObject *, const QString &) const"
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
        ModifyFunction{
            signature: "addStack(QUndoStack*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStacks"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removeStack(QUndoStack*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcStacks"
                    action: ReferenceCount.Take
                }
            }
        }
        ModifyFunction{
            signature: "setActiveStack(QUndoStack*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QShortcut"
        ModifyFunction{
            signature: "QShortcut<Func1>(QKeySequence::StandardKey,QWidget*,Func1,Qt::ShortcutContext)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "QShortcut<Func1>(QKeySequence,QWidget*,Func1,Qt::ShortcutContext)"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "QShortcut<Func1>(QKeySequence::StandardKey,QObject*,Func1,Qt::ShortcutContext)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QShortcut<Func1>(QKeySequence,QObject*,Func1,Qt::ShortcutContext)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "QShortcut(QKeySequence::StandardKey,QObject*,const char*,const char*,Qt::ShortcutContext)"
            access: Modification.Private
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
            ModifyArgument{
                index: 3
                RemoveDefaultExpression{
                }
                ReplaceType{
                    modifiedType: "java.lang.Object"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = nullptr;\n"+
                                  "Q_UNUSED(%in)"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveDefaultExpression{
                }
                ReplaceType{
                    modifiedType: "java.lang.Object"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = nullptr;\n"+
                                  "Q_UNUSED(%in)"}
                }
            }
        }
        ModifyFunction{
            signature: "QShortcut(QKeySequence,QObject*,const char*,const char*,Qt::ShortcutContext)"
            access: Modification.Private
            ModifyArgument{
                index: 1
                noImplicitCalls: true
            }
            ModifyArgument{
                index: 2
                NoNullPointer{
                }
            }
            ModifyArgument{
                index: 3
                RemoveDefaultExpression{
                }
                ReplaceType{
                    modifiedType: "java.lang.Object"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = nullptr;\n"+
                                  "Q_UNUSED(%in)"}
                }
            }
            ModifyArgument{
                index: 4
                RemoveDefaultExpression{
                }
                ReplaceType{
                    modifiedType: "java.lang.Object"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "const char* %out = nullptr;\n"+
                                  "Q_UNUSED(%in)"}
                }
            }
        }
        ModifyFunction{
            signature: "parentWidget<T>()const"
            Instantiation{
                threadAffinity: true
                Argument{
                    type: "QObject*"
                }
                ModifyArgument{
                    index: 0
                    ReplaceType{
                        modifiedType: "io.qt.widgets.@Nullable QWidget"
                    }
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QShortcut__"
                quoteBeforeLine: "}// class"
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QFileSystemModel"
        ModifyFunction{
            signature: "setIconProvider(QAbstractFileIconProvider *)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcIconProvider"
                    action: ReferenceCount.Set
                }
            }
            since: 6
        }
        since: 6
    }
    
    EnumType{
        name: "QFileSystemModel::Roles"
        since: 6
    }
    
    EnumType{
        name: "QFileSystemModel::Option"
        since: 6
    }
    
    EnumType{
        name: "QInputDevice::DeviceType"
        since: 6
    }
    
    EnumType{
        name: "QInputDevice::Capability"
        since: 6
    }
    
    ObjectType{
        name: "QInputDevice"
        since: 6
    }
    
    EnumType{
        name: "QPointingDevice::PointerType"
        since: 6
    }
    
    EnumType{
        name: "QPointingDevice::GrabTransition"
        since: 6
    }
    
    ObjectType{
        name: "QPointingDevice"
        ModifyFunction{
            signature: "setType(QInputDevice::DeviceType)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setCapabilities(QInputDevice::Capabilities)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setMaximumTouchPoints(int)"
            remove: RemoveFlag.All
        }
        since: 6
    }
    
    ValueType{
        name: "QEventPoint"
        ModifyFunction{
            signature: "operator=(QEventPoint)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "pos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "startPos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "scenePos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "startScenePos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "screenPos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "startScreenPos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "startNormalizedPos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "normalizedPos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "lastPos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "lastScenePos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "lastScreenPos() const"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "lastNormalizedPos() const"
            remove: RemoveFlag.All
        }
        ModifyField{
            name: "m_parent"
            read: true
            write: true
            rename: "parent"
        }
        ModifyField{
            name: "m_pos"
            read: false
            write: true
            rename: "position"
        }
        ModifyField{
            name: "m_scenePos"
            read: false
            write: true
            rename: "scenePosition"
        }
        ModifyField{
            name: "m_globalPos"
            read: false
            write: true
            rename: "globalPosition"
        }
        ModifyField{
            name: "m_globalPressPos"
            read: false
            write: true
            rename: "globalPressPosition"
        }
        ModifyField{
            name: "m_globalGrabPos"
            read: false
            write: true
            rename: "globalGrabPosition"
        }
        ModifyField{
            name: "m_globalLastPos"
            read: false
            write: true
            rename: "globalLastPosition"
        }
        ModifyField{
            name: "m_rotation"
            read: false
            write: true
            rename: "rotation"
        }
        ModifyField{
            name: "m_ellipseDiameters"
            read: false
            write: true
            rename: "ellipseDiameters"
        }
        ModifyField{
            name: "m_velocity"
            read: false
            write: true
            rename: "velocity"
        }
        ModifyField{
            name: "m_timestamp"
            read: false
            write: true
            rename: "timestamp"
        }
        ModifyField{
            name: "m_pressTimestamp"
            read: false
            write: true
            rename: "pressTimestamp"
        }
        ModifyField{
            name: "m_uniqueId"
            read: false
            write: true
            rename: "uniqueId"
        }
        ModifyField{
            name: "m_pointId"
            read: false
            write: true
            rename: "id"
        }
        ModifyField{
            name: "m_state"
            read: false
            write: true
            rename: "state"
        }
        ModifyField{
            name: "m_accept"
            read: false
            write: true
            rename: "accept"
        }
        ModifyField{
            name: "m_pressure"
            read: false
            write: true
            rename: "pressure"
        }
        ModifyField{
            name: "m_stationaryWithModifiedProperty"
            read: true
            write: true
            rename: "stationaryWithModifiedProperty"
        }
        since: 6
    }
    
    ObjectType{
        name: "QPointerEvent"
        ModifyFunction{
            signature: "operator=(const QPointerEvent &)"
            Delegate{
                name: "set"
                deprecated: true
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "point(qsizetype)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            InjectCode{
                position: Position.Beginning
                ArgumentMap{
                    index: 1
                    metaName: "%COUNT"
                }
                Text{content: "if(pointCount() <= %COUNT)\n"+
                              "    throw new IndexOutOfBoundsException(\"Index out of range: \"+%COUNT);"}
            }
        }
        ModifyFunction{
            signature: "pointById(int)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
        }
        ModifyFunction{
            signature: "setExclusiveGrabber(QEventPoint,QObject*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcExclusiveGrabber"
                    action: ReferenceCount.Set
                }
            }
        }
        ModifyFunction{
            signature: "addPassiveGrabber(QEventPoint,QObject*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcPassiveGrabber"
                    action: ReferenceCount.Add
                }
            }
        }
        ModifyFunction{
            signature: "removePassiveGrabber(QEventPoint,QObject*)"
            ModifyArgument{
                index: 2
                ReferenceCount{
                    variableName: "__rcPassiveGrabber"
                    action: ReferenceCount.Take
                }
            }
        }
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QPointerEvent___"
                quoteBeforeLine: "}// class"
            }
        }
        since: 6
    }
    
    ObjectType{
        name: "QSinglePointEvent"
        ModifyField{
            name: "m_reserved"
            read: false
            write: false
        }
        ModifyField{
            name: "m_reserved2"
            read: false
            write: false
        }
        ModifyField{
            name: "m_point"
            read: false
            write: true
            rename: "point"
        }
        ModifyField{
            name: "m_button"
            read: false
            write: true
            rename: "button"
        }
        ModifyField{
            name: "m_mouseState"
            read: false
            write: true
            rename: "buttons"
        }
        ModifyField{
            name: "m_phase"
            read: true
            write: true
            rename: "phaseFlag"
        }
        ModifyField{
            name: "m_source"
            read: true
            write: true
            rename: "sourceFlag"
        }
        ModifyField{
            name: "m_doubleClick"
            read: true
            write: true
            rename: "doubleClick"
            ReplaceType{
                modifiedType: "boolean"
            }
        }
        ModifyField{
            name: "m_invertedScrolling"
            read: true
            write: true
            rename: "invertedScrolling"
            ReplaceType{
                modifiedType: "boolean"
            }
        }
        ModifyFunction{
            signature: "clone()const"
            ModifyArgument{
                index: "return"
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
            since: 6
        }
        ModifyFunction{
            signature: "setExclusivePointGrabber(QObject*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    variableName: "__rcExclusiveGrabber"
                    action: ReferenceCount.Set
                }
            }
        }
        since: 6
    }
    
    EnumType{
        name: "QEventPoint::State"
        since: 6
    }
    
    Rejection{
        className: "QOpenGLFunctions_*Backend"
        until: 5
    }
    
    Rejection{
        className: "PFNGL*"
        until: 5
    }
    
    Rejection{
        className: "GLDEBUGPROCAMD"
        until: 5
    }
    
    Rejection{
        className: "GLDEBUGPROCARB"
        until: 5
    }
    
    Rejection{
        className: "GLVULKANPROCNV"
        until: 5
    }
    
    Rejection{
        className: "GLDEBUGPROCKHR"
        since: [6, 1]
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsStorage"
        until: 5
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsBackend"
        enumName: "Version"
        until: 5
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsBackend"
        fieldName: "refs"
        until: 5
    }
    
    Rejection{
        className: "QOpenGLVersionFunctionsBackend"
        until: 5
    }
    
    Rejection{
        className: "QOpenGLExtraFunctionsPrivate"
    }
    
    Rejection{
        className: "QOpenGLFunctionsPrivate"
    }
    
    Rejection{
        className: "QAbstractOpenGLFunctionsPrivate"
        until: 5
    }
    
    Rejection{
        className: "QOpenGLTextureHelper"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLWindow::UpdateBehavior"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLBuffer::Access"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLBuffer::Type"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLBuffer::RangeAccessFlag"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLBuffer::UsagePattern"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLDebugLogger::LoggingMode"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLDebugMessage::Severity"
        RejectEnumValue{
            name: "LastSeverity"
        }
        until: 5
    }
    
    EnumType{
        name: "QOpenGLDebugMessage::Source"
        RejectEnumValue{
            name: "LastSource"
        }
        until: 5
    }
    
    EnumType{
        name: "QOpenGLDebugMessage::Type"
        RejectEnumValue{
            name: "LastType"
        }
        until: 5
    }
    
    EnumType{
        name: "QOpenGLFramebufferObject::Attachment"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLFramebufferObject::FramebufferRestorePolicy"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLFunctions::OpenGLFeature"
    }
    
    EnumType{
        name: "QOpenGLShader::ShaderTypeBit"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::BindingTarget"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::DepthStencilMode"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::CoordinateDirection"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::Feature"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::CubeMapFace"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::Filter"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::MipMapGeneration"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::PixelFormat"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::PixelType"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::SwizzleComponent"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::SwizzleValue"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::Target"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::TextureFormat"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::TextureUnitReset"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::WrapMode"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::TextureFormatClass"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::ComparisonFunction"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTexture::ComparisonMode"
        until: 5
    }
    
    EnumType{
        name: "QOpenGLVersionStatus::OpenGLStatus"
        until: 5
    }
    
    ValueType{
        name: "QOpenGLBuffer"
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLContext"
        EnumType{
            name: "OpenGLModuleType"
        }
        ExtraIncludes{
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/CoreAPI"
                location: Include.Global
            }
            Include{
                fileName: "QSet"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "makeCurrent(QSurface *)"
            threadAffinity: true
        }
        ModifyFunction{
            signature: "openGLModuleHandle()"
            remove: RemoveFlag.All
            until: 5
        }
        ModifyFunction{
            signature: "extraFunctions()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "functions()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "surface()const"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setScreen(QScreen*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "setShareContext(QOpenGLContext*)"
            ModifyArgument{
                index: 1
                ReferenceCount{
                    action: ReferenceCount.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "makeCurrent(QSurface *)"
            threadAffinity: true
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            until: 5
            Text{content: "QAbstractOpenGLFunctions* qtjambi_QOpenGLContext_versionFunctions(JNIEnv *__jni_env, const QOpenGLContext *__qt_this, jclass type);"}
        }
        ModifyFunction{
            signature: "versionFunctions(const QOpenGLVersionProfile &)const"
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
            until: 5
        }
        ModifyFunction{
            signature: "versionFunctions<TYPE>()const"
            Instantiation{
                proxyCall: "qtjambi_QOpenGLContext_versionFunctions"
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
                    extending: "io.qt.gui.@Nullable QAbstractOpenGLFunctions"
                }
                AddArgument{
                    index: 1
                    name: "type"
                    type: "java.lang.Class<T>"
                }
            }
            until: 5
        }
        ModifyFunction{
            signature: "resolveInterface(const char *, int)const"
            rename: "nativeInterface"
            access: Modification.Public
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "<QNativeInterface extends io.qt.@Nullable QtObjectInterface> QNativeInterface"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "jobject %out = QtJambiAPI::convertNativeToJavaObjectAsWrapper(%env, %in, %1);"}
                }
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Dependent
                }
            }
            ModifyArgument{
                index: 1
                ReplaceType{
                    modifiedType: "java.lang.Class<QNativeInterface>"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "CoreAPI::NITypeInfo info = CoreAPI::getNativeInterfaceInfo(%env, %in);\n"+
                                  "const char* %out = info.name;"}
                }
            }
            ModifyArgument{
                index: 2
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "int %out = info.revision;"}
                }
                RemoveArgument{
                }
            }
            since: [6, 2]
        }
    }
    
    ObjectType{
        name: "QOpenGLContextGroup"
    }
    
    ObjectType{
        name: "QAbstractFileIconProvider"
        since: 6
    }
    
    EnumType{
        name: "QAbstractFileIconProvider::IconType"
        since: 6
    }
    
    EnumType{
        name: "QAbstractFileIconProvider::Option"
        since: 6
    }
    
    ObjectType{
        name: "QOpenGLDebugLogger"
        until: 5
    }
    
    ValueType{
        name: "QOpenGLDebugMessage"
        until: 5
    }
    
    ValueType{
        name: "QOpenGLFramebufferObjectFormat"
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFramebufferObject"
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
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
            signature: "glBufferData(GLenum,qopengl_GLsizeiptr,const void*,GLenum)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glBufferSubData(GLenum,qopengl_GLintptr,qopengl_GLsizeiptr,const void*)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCompressedTexImage2D(GLenum,GLint,GLenum,GLsizei,GLsizei,GLint,GLsizei,const void*)"
            ModifyArgument{
                index: 8
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCompressedTexSubImage2D(GLenum,GLint,GLint,GLint,GLsizei,GLsizei,GLenum,GLsizei,const void*)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetActiveAttrib(GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, char*)"
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
            signature: "glGetActiveUniform(GLuint, GLuint, GLsizei, GLsizei*, GLint*, GLenum*, char*)"
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
            signature: "glGetProgramInfoLog(GLuint, GLsizei, GLsizei*, char*)"
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
            signature: "glGetShaderInfoLog(GLuint, GLsizei, GLsizei*, char*)"
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
            signature: "glShaderBinary(GLint, const GLuint*, GLenum, const void*, GLint)"
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
            signature: "glShaderSource(GLuint, GLsizei, const char**, const GLint*)"
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
            signature: "glVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetShaderSource(GLuint, GLsizei, GLsizei*, char*)"
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
    }
    
    ObjectType{
        name: "QLinuxFbFunctions"
    }
    
    ObjectType{
        name: "QOpenGLExtraFunctions"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
            Include{
                fileName: "QtJambi/JavaAPI"
                location: Include.Global
            }
        }
        InjectCode{
            target: CodeClass.Native
            position: Position.Beginning
            Text{content: "#ifndef GL_MAX_LABEL_LENGTH\n"+
                          "#define GL_MAX_LABEL_LENGTH 0x82E8\n"+
                          "#endif"}
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );\n"+
                                  "if(%out.size()==0){\n"+
                                  "    JavaException::raiseIllegalArgumentException(%env, \"Array length is 0.\" QTJAMBI_STACKTRACEINFO );\n"+
                                  "}"}
                }
            }
        }
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
                }
            }
        }
        ModifyFunction{
            signature: "glFenceSync(GLenum,GLbitfield)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    ownership: Ownership.Ignore
                }
            }
        }
        ModifyFunction{
            signature: "glClearBufferfv(GLenum, GLint, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glClearBufferiv(GLenum, GLint, const GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glClearBufferuiv(GLenum, GLint, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCompressedTexImage3D(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const void *)"
            ModifyArgument{
                index: 9
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glCompressedTexSubImage3D(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 11
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glInvalidateFramebuffer(GLenum, GLsizei, const GLenum *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glInvalidateSubFramebuffer(GLenum, GLsizei, const GLenum *, GLint, GLint, GLsizei, GLsizei)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteProgramPipelines(GLsizei, const GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteQueries(GLsizei, const GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteSamplers(GLsizei, const GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteTransformFeedbacks(GLsizei, const GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDeleteVertexArrays(GLsizei, const GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawArraysIndirect(GLenum, const void *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawBuffers(GLsizei, const GLenum *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawElementsIndirect(GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawElementsInstanced(GLenum, GLsizei, GLenum, const void *, GLsizei)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawRangeElements(GLenum, GLuint, GLuint, GLsizei, GLenum, const void *)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenProgramPipelines(GLsizei, GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenQueries(GLsizei, GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenSamplers(GLsizei, GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenTransformFeedbacks(GLsizei, GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGenVertexArrays(GLsizei, GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetActiveUniformBlockName(GLuint, GLuint, GLsizei, GLsizei *, GLchar *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetActiveUniformBlockiv(GLuint, GLuint, GLenum, GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetActiveUniformsiv(GLuint, GLsizei, const GLuint *, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
            ModifyArgument{
                index: 5
                AsBuffer{}
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
        ModifyFunction{
            signature: "glGetUniformBlockIndex(GLuint, const GLchar *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, jstring(%in));"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetFragDataLocation(GLuint, const GLchar *)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, jstring(%in));"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetProgramResourceIndex(GLuint, GLenum, const GLchar *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, jstring(%in));"}
                }
            }
        }
        ModifyFunction{
            signature: "glObjectLabel(GLenum, GLuint, GLsizei, const GLchar *)"
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, jstring(%in));"}
                }
            }
        }
        ModifyFunction{
            signature: "glPushDebugGroup(GLenum, GLuint, GLsizei, const GLchar *)"
            ModifyArgument{
                index: 3
                RemoveArgument{
                }
            }
            ModifyArgument{
                index: 4
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, jstring(%in));\n"+
                                  "GLsizei %3 = GLsizei(%out.length());"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetProgramResourceLocation(GLuint, GLenum, const GLchar *)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, jstring(%in));"}
                }
            }
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %3));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (infoLog.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'infoLog': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %5));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (name.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'name': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %4));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (label.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'label': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
        }
        ModifyFunction{
            signature: "glGetPointerv(GLenum, void **)"
            ModifyArgument{
                index: 2
                ReplaceType{
                    modifiedType: "java.nio.Buffer[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JObjectArrayPointer<void*> %out(%env, jobjectArray(%in),\n"+
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
                }
            }
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %3));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (infoLog.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'infoLog': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %4));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (label.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'label': Wrong number of elements in array. Found: 0, expected: 1\");"}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %3));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
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
                NoNullPointer{
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, %in);\n"+
                                  "GLsizei %2(-1);"}
                }
            }
        }
        ModifyFunction{
            signature: "glDebugMessageControl(GLenum, GLenum, GLenum, GLsizei, const GLuint *, GLboolean)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawElementsBaseVertex(GLenum, GLsizei, GLenum, const void *, GLint)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawRangeElementsBaseVertex(GLenum, GLuint, GLuint, GLsizei, GLenum, const void *, GLint)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDrawElementsInstancedBaseVertex(GLenum, GLsizei, GLenum, const void *, GLsizei, GLint)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glDebugMessageInsert(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar *)"
            ModifyArgument{
                index: 6
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetBooleani_v(GLenum, GLuint, GLboolean *)"
            ModifyArgument{
                index: 3
                NoNullPointer{
                }
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
            signature: "glGetInteger64i_v(GLenum, GLuint, GLint64 *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetInternalformativ(GLenum, GLenum, GLenum, GLsizei, GLint*)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetIntegeri_v(GLenum, GLuint, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetInteger64v(GLenum, GLint64 *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetBufferParameteri64v(GLenum, GLenum, GLint64*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetFramebufferParameteriv(GLenum, GLenum, GLint*)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexAttribI4iv(GLuint, const GLint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexAttribI4uiv(GLuint, const GLuint *)"
            ModifyArgument{
                index: 2
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glVertexAttribIPointer(GLuint, GLint, GLenum, GLsizei, const void *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix4x3fv(GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix4x2fv(GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix3x4fv(GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix3x2fv(GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix2x4fv(GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniformMatrix2x3fv(GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix4x3fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix4x2fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix4fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix3x4fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix3x2fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix3fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix2x4fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix2x3fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniformMatrix2fv(GLuint, GLint, GLsizei, GLboolean, const GLfloat *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform1uiv(GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform2uiv(GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform3uiv(GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glUniform4uiv(GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexParameterIiv(GLenum, GLenum, const GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexParameterIuiv(GLenum, GLenum, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glSamplerParameterIuiv(GLuint, GLenum, const GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glSamplerParameterIiv(GLuint, GLenum, const GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glSamplerParameteriv(GLuint, GLenum, const GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glSamplerParameterfv(GLuint, GLenum, const GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform4uiv(GLuint, GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform4iv(GLuint, GLint, GLsizei, const GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform4fv(GLuint, GLint, GLsizei, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform3uiv(GLuint, GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform3iv(GLuint, GLint, GLsizei, const GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform3fv(GLuint, GLint, GLsizei, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform2uiv(GLuint, GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform2iv(GLuint, GLint, GLsizei, const GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform2fv(GLuint, GLint, GLsizei, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform1uiv(GLuint, GLint, GLsizei, const GLuint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform1iv(GLuint, GLint, GLsizei, const GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glProgramUniform1fv(GLuint, GLint, GLsizei, const GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetnUniformfv(GLuint, GLint, GLsizei, GLfloat *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetnUniformuiv(GLuint, GLint, GLsizei, GLuint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetnUniformiv(GLuint, GLint, GLsizei, GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetMultisamplefv(GLenum, GLuint, GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetProgramBinary(GLuint, GLsizei, GLsizei *, GLenum *, void *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetProgramInterfaceiv(GLuint, GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 4
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetProgramPipelineiv(GLuint, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetProgramResourceiv(GLuint, GLenum, GLuint, GLsizei, const GLenum *, GLsizei, GLsizei *, GLint *)"
            ModifyArgument{
                index: 5
                AsBuffer{}
            }
            ModifyArgument{
                index: 7
                AsBuffer{}
            }
            ModifyArgument{
                index: 8
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetQueryObjectuiv(GLuint, GLenum, GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetQueryiv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetSamplerParameterIiv(GLuint, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetSamplerParameterIuiv(GLuint, GLenum, GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetSamplerParameterfv(GLuint, GLenum, GLfloat *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetSamplerParameteriv(GLuint, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
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
            signature: "glGetTexParameterIiv(GLenum, GLenum, GLint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetTexParameterIuiv(GLenum, GLenum, GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
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
                                  "    jstring result = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%out, %4));\n"+
                                  "    %env->SetObjectArrayElement(%in, 0, result);\n"+
                                  "});"}
                }
            }
            InjectCode{
                Text{content: "if (name.length < 1)\n"+
                              "    throw new IllegalArgumentException(\"Argument 'name': Wrong number of elements in array. Found: 0, expected: 1\");"}
            }
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
                NoNullPointer{
                }
                ReplaceType{
                    modifiedType: "int[]"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JIntArrayPointer %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "glGetUniformuiv(GLuint, GLint, GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glGetVertexAttribIiv(GLuint, GLenum, GLint *)"
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
            signature: "glGetVertexAttribIuiv(GLuint, GLenum, GLuint *)"
            ModifyArgument{
                index: 3
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glReadnPixels(GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLsizei, void *)"
            ModifyArgument{
                index: 8
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexImage3D(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 10
                AsBuffer{}
            }
        }
        ModifyFunction{
            signature: "glTexSubImage3D(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const void *)"
            ModifyArgument{
                index: 11
                AsBuffer{}
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
        InjectCode{
            ImportFile{
                name: ":/io/qtjambi/generator/typesystem/QtJambiGui.java"
                quoteAfterLine: "class QOpenGLExtraFunctions___"
                quoteBeforeLine: "}// class"
            }
        }
    }
    
    FunctionalType{
        name: "GLDEBUGPROC"
        packageName: "io.qt.gui.gl"
        ExtraIncludes{
            Include{
                fileName: "QtGui/QOpenGLExtraFunctions"
                location: Include.Global
            }
        }
        ModifyArgument{
            index: 5
            RemoveArgument{
            }
        }
        ModifyArgument{
            index: 7
            RemoveArgument{
            }
        }
        ModifyArgument{
            index: 6
            ReplaceType{
                modifiedType: "java.lang.String"
            }
            ConversionRule{
                codeClass: CodeClass.Native
                Text{content: "J2CStringBuffer %out(%env, %in);\n"+
                              "GLsizei %5 = GLsizei(%out.length());\n"+
                              "const void * __qt_%7 = nullptr;"}
            }
            ConversionRule{
                codeClass: CodeClass.Shell
                Text{content: "Q_UNUSED(%7)\n"+
                              "jstring %out = qtjambi_cast<jstring>(%env, QString::fromLocal8Bit(%in, %5));"}
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_0"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_1"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_2"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_3"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_4"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_1_5"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_2_0"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_2_1"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
        Import{
            template: "QOpenGLFunctions_2_0A"
        }
        Import{
            template: "QOpenGLFunctions_1_0_template_full"
        }
        Import{
            template: "QOpenGLFunctions_1_1_template_full"
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_0"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_1"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_2_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_2_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_3_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_3_3_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_0_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_0_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_1_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_1_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_2_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_2_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_3_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_3_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_4_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_4_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_5_Compatibility"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_4_5_Core"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLFunctions_ES2"
        ppCondition: "defined(QT_OPENGL_ES_2)"
        targetType: "final class"
        ExtraIncludes{
            Include{
                fileName: "QtCore/QSharedPointer"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "glBindAttribLocation(GLuint,GLuint,const GLchar*)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.lang.String"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, %in);"}
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
                                  "                                [](void* & pointer,JNIEnv *, jobject){\n"+
                                  "                                    pointer = nullptr;\n"+
                                  "                                },\n"+
                                  "                                [](JNIEnv * env, void* const& ptr) -> jobject {\n"+
                                  "                                    return DataJBuffer(env, ptr, INT_MAX).take();\n"+
                                  "                                }\n"+
                                  "                            );"}
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
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, %in);"}
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
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "J2CStringBuffer %out(%env, %in);"}
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLPaintDevice"
        ModifyFunction{
            signature: "devType() const"
            remove: RemoveFlag.All
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLShader"
        ModifyFunction{
            signature: "compileSourceCode ( const char *)"
            remove: RemoveFlag.All
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLShaderProgram"
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
                ReplaceType{
                    modifiedType: "java.nio.Buffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBufferConstData %out(%env, %in);"}
                }
            }
        }
        ModifyFunction{
            signature: "setAttributeArray(const char *, GLenum, const void *, int, int)"
            ModifyArgument{
                index: 3
                ReplaceType{
                    modifiedType: "java.nio.Buffer"
                }
                ConversionRule{
                    codeClass: CodeClass.Native
                    Text{content: "JBufferConstData %out(%env, %in);"}
                }
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
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLTimeMonitor"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        until: 5
    }
    
    ValueType{
        name: "QOpenGLVersionProfile"
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLVertexArrayObject"
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLVertexArrayObject::Binder"
        implementing: "AutoCloseable"
        InjectCode{
            target: CodeClass.Java
            Text{content: "@Override\n"+
                          "@QtUninvokable\n"+
                          "public final void close(){\n"+
                          "    dispose();\n"+
                          "}"}
        }
        until: 5
    }
    
    EnumType{
        name: "QOpenGLTextureBlitter::Origin"
        since: [5, 8]
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLTextureBlitter"
        since: [5, 8]
        until: 5
    }
    
    ValueType{
        name: "QOpenGLVersionStatus"
        CustomConstructor{
            Text{content: "return new(placement) QOpenGLVersionStatus(copy->version.first, copy->version.second, copy->status);"}
        }
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLTexture"
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
            signature: "setCompressedData(int, void *, const QOpenGLPixelTransferOptions *const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setCompressedData(int, int, void *, const QOpenGLPixelTransferOptions *const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setCompressedData(int, int, QOpenGLTexture::CubeMapFace, int, void *, const QOpenGLPixelTransferOptions *const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setCompressedData(int, int, int, void *, const QOpenGLPixelTransferOptions *const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setData(QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, void *, const QOpenGLPixelTransferOptions *const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setData(int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, void *, const QOpenGLPixelTransferOptions *const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setData(int, int, QOpenGLTexture::CubeMapFace, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, void *, const QOpenGLPixelTransferOptions *const)"
            remove: RemoveFlag.All
        }
        ModifyFunction{
            signature: "setData(int, int, QOpenGLTexture::PixelFormat, QOpenGLTexture::PixelType, void *, const QOpenGLPixelTransferOptions *const)"
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
        until: 5
    }
    
    ValueType{
        name: "QOpenGLPixelTransferOptions"
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLTimerQuery"
        ppCondition: "!defined(QT_NO_OPENGL) && !defined(QT_OPENGL_ES_2)"
        until: 5
    }
    
    ObjectType{
        name: "QOpenGLWindow"
        ModifyFunction{
            signature: "paintGL()"
            isPaintMethod: true
        }
        until: 5
    }
    ObjectType{
        name: "QUtiMimeConverter"
        ppCondition: "defined(Q_OS_MACOS)"
        since: 6.5
    }

    ObjectType{
        name: "QWindowsMimeConverter"
        ppCondition: "defined(Q_OS_WIN)"
        since: 6.5
    }
    SuppressedWarning{text: "skipping function 'QWindowsMimeConverter::*', unmatched *type '*FORMATETC*'"; since: 6.5}
    SuppressedWarning{text: "skipping function 'QWindowsMimeConverter::*', unmatched *type '*STGMEDIUM*'"; since: 6.5}
    SuppressedWarning{text: "skipping function 'QWindowsMimeConverter::*', unmatched *type '*IDataObject*'"; since: 6.5}
    SuppressedWarning{text: "Unimplementable pure virtual function: QWindowsMimeConverter::*"; since: 6.5}
    
    ValueType{
        name: "HWND__"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "HGLRC__"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "HMODULE__"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "__GLXcontextRec"
        generate: false
        since: [6, 2]
    }
    
    ValueType{
        name: "ANativeWindow"
        generate: false
        since: [6, 2]
    }
    
    TypeAliasType{
        name: "GLXContext"
    }
    
    TypeAliasType{
        name: "EGLContext"
    }
    
    TypeAliasType{
        name: "EGLDisplay"
    }
    
    TypeAliasType{
        name: "NSOpenGLContext"
    }
    
    TypeAliasType{
        name: "HMODULE"
    }
    
    TypeAliasType{
        name: "HGLRC"
    }
    
    TypeAliasType{
        name: "HWND"
    }
    
    TypeAliasType{
        name: "HINSTANCE"
    }
    
    InterfaceType{
        name: "QNativeInterface::Private::QCocoaWindow"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QCocoaWindow"
        ppCondition: "defined(Q_OS_MACOS)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QCocoaWindow()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QWindow"
                location: Include.Global
            }
            Include{
                fileName: "qpa/qplatformwindow_p.h"
                location: Include.Global
            }
            Include{
                fileName: "QtGui/qpa/qplatformwindow_p.h"
                suppressed: true
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::QCocoaGLContext"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QCocoaGLContext"
        ppCondition: "defined(Q_OS_MACOS)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QCocoaGLContext()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QOpenGLContext"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "fromNative(NSOpenGLContext*,QOpenGLContext*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::Private::QWindowsWindow"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QWindowsWindow"
        ppCondition: "defined(Q_OS_WIN)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QWindowsWindow()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QWindow"
                location: Include.Global
            }
            Include{
                fileName: "qpa/qplatformwindow_p.h"
                location: Include.Global
            }
            Include{
                fileName: "QtGui/qpa/qplatformwindow_p.h"
                suppressed: true
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::QWGLContext"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QWGLContext"
        ppCondition: "defined(Q_OS_WIN)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QWGLContext()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QOpenGLContext"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "fromNative(HGLRC,HWND,QOpenGLContext*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::QGLXContext"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QGLXContext"
        ppCondition: "QT_CONFIG(xcb_glx_plugin)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QGLXContext()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QOpenGLContext"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "fromNative(GLXContext,QOpenGLContext*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        ModifyFunction{
            signature: "fromNative(GLXContext,void*,QOpenGLContext*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::QEGLContext"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QEGLContext"
        ppCondition: "QT_CONFIG(egl)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }
        Rejection{
            functionName: "config"
            since: [6, 3]
        }
        ModifyFunction{
            signature: "QEGLContext()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QOpenGLContext"
                location: Include.Global
            }
        }
        ModifyFunction{
            signature: "fromNative(EGLContext,EGLDisplay,QOpenGLContext*)"
            ModifyArgument{
                index: 0
                DefineOwnership{
                    codeClass: CodeClass.Native
                    ownership: Ownership.Java
                }
                DefineOwnership{
                    codeClass: CodeClass.Shell
                    ownership: Ownership.Cpp
                }
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::Private::QWindowsApplication"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QWindowsApplication"
        ppCondition: "defined(Q_OS_WIN)"
        isNativeInterface: true
        generate: "no-shell"

        Rejection{
            className: "TypeInfo"
        }

        Rejection{
            functionName: "registerMime"
        }

        Rejection{
            functionName: "unregisterMime"
        }

        EnumType{
            name: "WindowActivationBehavior"
        }

        EnumType{
            name: "DarkModeHandlingFlag"
        }

        EnumType{
            name: "TouchWindowTouchType"
        }
        ModifyFunction{
            signature: "QWindowsApplication()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QGuiApplication"
                location: Include.Global
            }
        }
        since: [6, 2]
    }

    PrimitiveType{
        name: "xcb_connection_t"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }

    PrimitiveType{
        name: "Display"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::QX11Application"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QX11Application"
        ppCondition: "QT_CONFIG(xcb)"
        isNativeInterface: true
        generate: "no-shell"

        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QX11Application()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QGuiApplication"
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::Private::QXcbWindow"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QXcbWindow"
        ppCondition: "QT_CONFIG(xcb)"
        isNativeInterface: true
        generate: "no-shell"
        Rejection{
            className: "TypeInfo"
        }

        EnumType{
            name: "WindowType"
        }
        ModifyFunction{
            signature: "QXcbWindow()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QWindow"
                location: Include.Global
            }
            Include{
                fileName: "qpa/qplatformwindow_p.h"
                location: Include.Global
            }
            Include{
                fileName: "QtGui/qpa/qplatformwindow_p.h"
                suppressed: true
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::QAndroidOffscreenSurface"
        ppCondition: "defined(Q_OS_ANDROID)"
        generate: "no-shell"

        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QAndroidOffscreenSurface()"
            remove: RemoveFlag.All
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::Private::QXcbScreen"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QXcbScreen"
        ppCondition: "QT_CONFIG(xcb)"
        isNativeInterface: true
        generate: "no-shell"

        Rejection{
            className: "TypeInfo"
        }
        ModifyFunction{
            signature: "QXcbScreen()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QScreen"
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::Private::QVsp2Screen"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QVsp2Screen"
        ppCondition: "QT_CONFIG(vsp2)"
        isNativeInterface: true
        generate: false
        Rejection{
            className: "TypeInfo"
        }

        Rejection{
            functionName: "addBlendListener"
        }
        ModifyFunction{
            signature: "QVsp2Screen()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QScreen"
                location: Include.Global
            }
        }
        since: [6, 2]
    }
    
    InterfaceType{
        name: "QNativeInterface::Private::QWebOSScreen"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QWebOSScreen"
        ppCondition: "defined(Q_OS_WEBOS)"
        isNativeInterface: true
        generate: false
        Rejection{
            className: "TypeInfo"
        }

        Rejection{
            functionName: "addFlipListener"
        }
        ModifyFunction{
            signature: "QWebOSScreen()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QScreen"
                location: Include.Global
            }
        }
        since: [6, 2]
    }

    PrimitiveType{
        name: "wl_seat"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }

    PrimitiveType{
        name: "wl_keyboard"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }

    PrimitiveType{
        name: "wl_display"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }

    PrimitiveType{
        name: "wl_pointer"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }

    PrimitiveType{
        name: "wl_compositor"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }

    PrimitiveType{
        name: "wl_touch"
        javaName: "io.qt.QNativePointer"
        jniName: "jobject"
        preferredConversion: false
        since: [6, 2]
    }

    InterfaceType{
        name: "QNativeInterface::QWaylandApplication"
        packageName: "io.qt.gui.nativeinterface"
        javaName: "QWaylandApplication"
        ppCondition: "defined(Q_OS_UNIX)"
        isNativeInterface: true
        generate: false
        Rejection{
            className: "TypeInfo"
        }

        ModifyFunction{
            signature: "QWaylandApplication()"
            remove: RemoveFlag.All
        }
        ExtraIncludes{
            Include{
                fileName: "QtGui/QScreen"
                location: Include.Global
            }
        }
        since: [6, 2]
    }

    ObjectType{
        name: "QPlatformIntegration"
        until: 6
        packageName: "io.qt.gui.qpa"
        generate: "no-shell"
        ModifyFunction{
            signature: "QPlatformIntegration()"
            remove: RemoveFlag.All
        }
        EnumType{
            name: "Capability"
        }
        EnumType{
            name: "StyleHint"
        }

        Rejection{functionName: "call"}
        Rejection{functionName: "createPlatformPixmap"}
        Rejection{functionName: "createPlatformWindow"}
        Rejection{functionName: "createForeignWindow"}
        Rejection{functionName: "createPlatformBackingStore"}
        Rejection{functionName: "createPlatformOpenGLContext"}
        Rejection{functionName: "createPlatformSharedGraphicsCache"}
        Rejection{functionName: "createImagePaintEngine"}
        Rejection{functionName: "createEventDispatcher"}
        Rejection{functionName: "initialize"}
        Rejection{functionName: "destroy"}
        Rejection{functionName: "fontDatabase"}
        Rejection{functionName: "clipboard"}
        Rejection{functionName: "drag"}
        Rejection{functionName: "inputContext"}
        Rejection{functionName: "accessibility"}
        Rejection{functionName: "nativeInterface"}
        Rejection{functionName: "services"}
        Rejection{functionName: "createPlatformTheme"}
        Rejection{functionName: "createPlatformOffscreenSurface"}
        Rejection{functionName: "createPlatformSessionManager"}
        Rejection{functionName: "sync"}
        Rejection{functionName: "createPlatformVulkanInstance"}
        InjectCode{
            target: CodeClass.Java
            Text{content: "@QtUninvokable\n"+
                          "public native static QPlatformIntegration instance();"}
        }

        ModifyFunction{
            signature: "themeNames() const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QStringList"
                }
            }
        }
        ModifyFunction{
            signature: "possibleKeys(const QKeyEvent *) const"
            ModifyArgument{
                index: 0
                ReplaceType{
                    modifiedType: "io.qt.core.@NonNull QList<java.lang.@QtPrimitiveType()@NonNull() Integer>"
                }
            }
        }
    }
    
    Rejection{
        className: ""
        functionName: "qAccessibleEventString"
    }
    
    Rejection{
        className: ""
        functionName: "qAccessibleLocalizedActionDescription"
    }
    
    Rejection{
        className: ""
        functionName: "qAccessibleRoleString"
    }
    
    Rejection{
        className: ""
        functionName: "qAlpha"
    }
    
    Rejection{
        className: ""
        functionName: "qBlue"
    }
    
    Rejection{
        className: ""
        functionName: "qGray"
    }
    
    Rejection{
        className: ""
        functionName: "qGreen"
    }
    
    Rejection{
        className: ""
        functionName: "qRgb"
    }
    
    Rejection{
        className: ""
        functionName: "qRgba"
    }
    
    Rejection{
        className: ""
        functionName: "qRgba64"
    }
    
    Rejection{
        className: ""
        functionName: "qRed"
    }
    
    Rejection{
        className: ""
        functionName: "qUnpremultiply"
    }
    
    Rejection{
        className: ""
        functionName: "qt_findAtNxFile"
    }
    
    Rejection{
        className: ""
        functionName: "qt_handleTouchEvent"
    }
    
    GlobalFunction{
        signature: "qFuzzyCompare(QQuaternion, QQuaternion)"
        targetType: "QQuaternion"
    }
    
    GlobalFunction{
        signature: "qFuzzyCompare(QTransform, QTransform)"
        targetType: "QTransform"
    }
    
    GlobalFunction{
        signature: "qIsGray(QRgb)"
        targetType: "QColor"
    }
    
    GlobalFunction{
        signature: "qPremultiply(QRgb)"
        targetType: "QColor"
    }
    
    GlobalFunction{
        signature: "qPremultiply(QRgba64)"
        targetType: "QRgba64"
    }
    
    GlobalFunction{
        signature: "qPixelFormatAlpha(uchar, QPixelFormat::TypeInterpretation)"
        targetType: "QPixelFormat"
    }
    
    GlobalFunction{
        signature: "qPixelFormatCmyk(uchar, uchar, QPixelFormat::AlphaUsage, QPixelFormat::AlphaPosition, QPixelFormat::TypeInterpretation)"
        targetType: "QPixelFormat"
    }
    
    GlobalFunction{
        signature: "qPixelFormatGrayscale(uchar, QPixelFormat::TypeInterpretation)"
        targetType: "QPixelFormat"
    }
    
    GlobalFunction{
        signature: "qPixelFormatHsl(uchar, uchar, QPixelFormat::AlphaUsage, QPixelFormat::AlphaPosition, QPixelFormat::TypeInterpretation)"
        targetType: "QPixelFormat"
    }
    
    GlobalFunction{
        signature: "qPixelFormatHsv(uchar, uchar, QPixelFormat::AlphaUsage, QPixelFormat::AlphaPosition, QPixelFormat::TypeInterpretation)"
        targetType: "QPixelFormat"
    }
    
    GlobalFunction{
        signature: "qPixelFormatRgba(uchar, uchar, uchar, uchar, QPixelFormat::AlphaUsage, QPixelFormat::AlphaPosition, QPixelFormat::AlphaPremultiplied, QPixelFormat::TypeInterpretation)"
        targetType: "QPixelFormat"
    }
    
    GlobalFunction{
        signature: "qPixelFormatYuv(QPixelFormat::YUVLayout, uchar, QPixelFormat::AlphaUsage, QPixelFormat::AlphaPosition, QPixelFormat::AlphaPremultiplied, QPixelFormat::TypeInterpretation, QPixelFormat::ByteOrder)"
        targetType: "QPixelFormat"
    }
    
    GlobalFunction{
        signature: "qFuzzyCompare(const QMatrix&, const QMatrix&)"
        remove: RemoveFlag.All
        until: [5, 15]
    }
    
    GlobalFunction{
        signature: "qFuzzyCompare(const QMatrix4x4&, const QMatrix4x4&)"
        targetType: "QMatrix4x4"
        until: [5, 15]
    }
    
    GlobalFunction{
        signature: "qFuzzyCompare(const QVector2D&, const QVector2D&)"
        targetType: "QVector2D"
        until: [5, 15]
    }
    
    GlobalFunction{
        signature: "qFuzzyCompare(const QVector3D&, const QVector3D&)"
        targetType: "QVector3D"
        until: [5, 15]
    }
    
    GlobalFunction{
        signature: "qFuzzyCompare(const QVector4D&, const QVector4D&)"
        targetType: "QVector4D"
        until: [5, 15]
    }
    
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unsupported default value '0., 0., 1000000000., 1000000000.' of argument in function 'update', class 'QAbstractTextDocumentLayout'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type '*QImageTextKeyLang*'"}
    SuppressedWarning{text: "WARNING(MetaInfoGenerator) :: class 'QInputEvent' inherits from polymorphic class 'QEvent', but has no polymorphic id set"}
    SuppressedWarning{text: "WARNING(MetaInfoGenerator) :: class 'QPixmapFilter' inherits from polymorphic class 'QPixmapFilter', but has no polymorphic id set"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QPixmap::QPixmap', unmatched parameter type 'QPixmapData*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPixmap::Type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QPixmap::pixmapData', unmatched return type 'QPixmapData*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: class 'QTextObjectInterface' inherits from unknown base class 'QTextObjectInterface'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unknown interface for 'QTextObjectInterface': 'QTextObjectInterfaceInterface'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QTextEngine\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QTextFrameLayoutData\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QTextEngine\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformSurface\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformWindow\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QAccessibleInterface\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'const QMatrix&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix3x3'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'const QGenericMatrix<N,M,float>&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QOpenGLFunctionsPrivate const\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QFontPrivate const\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QTextDocumentPrivate const\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformBackingStore\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformNativeInterface\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPostEventList\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformPixmap\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QAbstractNativeEventFilter\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformOpenGLContext\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'const QOpenGLVersionStatus&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformOffscreenSurface\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix4x3*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix4x2*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix3x2*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix3x3*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix3x4*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix2x2*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix2x3*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QMatrix2x4*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QFontEngine\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QSurfacePrivate'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QImageCleanupFunction'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QPlatformScreen\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QOpenGLVersionFunctionsBackend\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QAccessibleTableCellInterface\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QAbstractOpenGLFunctions\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QAbstractFileIconProviderPrivate\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'TYPE\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QOpenGLFunctions_*' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: QPainter.draw*(const QVector*) mostly equal to QPainter.draw*(const QVector*)"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QOpenGLContext::setQGLContextHandle', unmatched parameter type 'void'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QOpenGLContext::versionFunctions', unmatched parameter type 'QAbstractOpenGLFunctions\\*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * 'QOpenGLContext::functionsBackendStorage', unmatched *type 'QOpenGLVersionFunctionsStorage*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: non-public function 'QSurface' in interface 'QSurface'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: non-public function '*' in interface 'QPaintDevice'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: template baseclass 'QGenericMatrix<float>' of '*' is not known"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'surfaceHandle() const' in '*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QRasterPaintEngine' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: type 'QPlatformSurface' is specified in typesystem, but not defined. This could potentially lead to compilation errors."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QPair::QPair', unmatched parameter type 'const QPair<TT1,TT2>&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QPair::operator=', unmatched parameter type 'const QPair<TT1,TT2>&'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping *, unmatched * type 'QOpenGLTextureHelper*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: private virtual function 'paintEngine() const' in 'QPaintDeviceWindow'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: Final class 'QPaintDeviceWindow' set to non-final, as it is extended by other classes"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QMenu::addAction', unmatched return type 'QtPrivate::QEnableIf<EXPRESSION,QAction*>::Type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QToolBar::addAction', unmatched return type 'QtPrivate::QEnableIf<EXPRESSION,QAction*>::Type'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'setVulkanInstance(QVulkanInstance*)' for function modification in 'QWindow' not found.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'vulkanInstance()const' for function modification in 'QWindow' not found.*"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function 'QColorTransform::map', unmatched return type 'QRgbaFloat*'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * '*', unmatched *type 'QVulkan*'"}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QColorTransform."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QFontDatabase."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QFontInfo."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QFormLayout::TakeRowResult."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QIcon."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QIconEngine::ScaledPixmapArgument."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QInputMethodEvent::Attribute."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QPainter::PixmapFragment."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QPicture."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTextLine."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTextInlineObject."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTextEdit::ExtraSelection."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTextDocumentFragment."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTextBoundaryFinder."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QOpenGLBuffer."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QOpenGLPixelTransferOptions."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QPagedPaintDevice::Margins."}
    SuppressedWarning{text: "WARNING(JavaGenerator) :: No ==/!= operator found for value type QTouchEvent::TouchPoint."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: class 'VkImage' inherits from unknown base class 'quint64'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: unsupported default value of argument in function 'doAction', class 'QAccessibleInterface'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: object type 'QAccessible' extended by interface type 'QAbstractAccessibleFactory'. The resulting API will be less expressive than the original."}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: class 'QAccessibleEvent' has polymorphic id but does not inherit a polymorphic class"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping * unmatched *type 'QAccessibleTableCellInterface'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: class 'Display' inherits from unknown base class '_XDisplay'"}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: signature 'QPaintEngineState(*)' for function modification in 'QPaintEngineState' not found. Possible candidates: "}
    SuppressedWarning{text: "WARNING(MetaJavaBuilder) :: skipping function '*', unmatched *type 'QRhi*'"; until: 6.6}
}
