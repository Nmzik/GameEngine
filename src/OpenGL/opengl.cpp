// Gives a definition to all the OpenGL functions

#include "opengl.h"

void(GLAPIENTRYP glCullFace)(GLenum mode);
void(GLAPIENTRYP glFrontFace)(GLenum mode);
void(GLAPIENTRYP glHint)(GLenum target, GLenum mode);
void(GLAPIENTRYP glLineWidth)(GLfloat width);
void(GLAPIENTRYP glPointSize)(GLfloat size);
void(GLAPIENTRYP glPolygonMode)(GLenum face, GLenum mode);
void(GLAPIENTRYP glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
void(GLAPIENTRYP glTexParameterfv)(GLenum target, GLenum pname, const GLfloat* params);
void(GLAPIENTRYP glTexParameteri)(GLenum target, GLenum pname, GLint param);
void(GLAPIENTRYP glTexParameteriv)(GLenum target, GLenum pname, const GLint* params);
void(GLAPIENTRYP glTexImage1D)(
    GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
void(GLAPIENTRYP glTexImage2D)(
    GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
void(GLAPIENTRYP glDrawBuffer)(GLenum buf);
void(GLAPIENTRYP glClear)(GLbitfield mask);
void(GLAPIENTRYP glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void(GLAPIENTRYP glClearStencil)(GLint s);
void(GLAPIENTRYP glClearDepth)(GLdouble depth);
void(GLAPIENTRYP glStencilMask)(GLuint mask);
void(GLAPIENTRYP glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
void(GLAPIENTRYP glDepthMask)(GLboolean flag);
void(GLAPIENTRYP glDisable)(GLenum cap);
void(GLAPIENTRYP glEnable)(GLenum cap);
void(GLAPIENTRYP glFinish)(void);
void(GLAPIENTRYP glFlush)(void);
void(GLAPIENTRYP glBlendFunc)(GLenum sfactor, GLenum dfactor);
void(GLAPIENTRYP glLogicOp)(GLenum opcode);
void(GLAPIENTRYP glStencilFunc)(GLenum func, GLint ref, GLuint mask);
void(GLAPIENTRYP glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
void(GLAPIENTRYP glDepthFunc)(GLenum func);
void(GLAPIENTRYP glPixelStoref)(GLenum pname, GLfloat param);
void(GLAPIENTRYP glPixelStorei)(GLenum pname, GLint param);
void(GLAPIENTRYP glReadBuffer)(GLenum src);
void(GLAPIENTRYP glReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
void(GLAPIENTRYP glGetBooleanv)(GLenum pname, GLboolean* data);
void(GLAPIENTRYP glGetDoublev)(GLenum pname, GLdouble* data);
GLenum(GLAPIENTRYP glGetError)(void);
void(GLAPIENTRYP glGetFloatv)(GLenum pname, GLfloat* data);
void(GLAPIENTRYP glGetIntegerv)(GLenum pname, GLint* data);
const GLubyte*(GLAPIENTRYP glGetString)(GLenum name);
void(GLAPIENTRYP glGetTexImage)(GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
void(GLAPIENTRYP glGetTexParameterfv)(GLenum target, GLenum pname, GLfloat* params);
void(GLAPIENTRYP glGetTexParameteriv)(GLenum target, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetTexLevelParameterfv)(GLenum target, GLint level, GLenum pname, GLfloat* params);
void(GLAPIENTRYP glGetTexLevelParameteriv)(GLenum target, GLint level, GLenum pname, GLint* params);
GLboolean(GLAPIENTRYP glIsEnabled)(GLenum cap);
void(GLAPIENTRYP glDepthRange)(GLdouble near, GLdouble far);
void(GLAPIENTRYP glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glDrawArrays)(GLenum mode, GLint first, GLsizei count);
void(GLAPIENTRYP glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
void(GLAPIENTRYP glGetPointerv)(GLenum pname, void** params);
void(GLAPIENTRYP glPolygonOffset)(GLfloat factor, GLfloat units);
void(GLAPIENTRYP glCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
void(GLAPIENTRYP glCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
void(GLAPIENTRYP glCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void(GLAPIENTRYP glCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
void(GLAPIENTRYP glTexSubImage2D)(
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
void(GLAPIENTRYP glBindTexture)(GLenum target, GLuint texture);
void(GLAPIENTRYP glDeleteTextures)(GLsizei n, const GLuint* textures);
void(GLAPIENTRYP glGenTextures)(GLsizei n, GLuint* textures);
GLboolean(GLAPIENTRYP glIsTexture)(GLuint texture);
void(GLAPIENTRYP glDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices);
void(GLAPIENTRYP glTexImage3D)(GLenum target,
                               GLint level,
                               GLint internalformat,
                               GLsizei width,
                               GLsizei height,
                               GLsizei depth,
                               GLint border,
                               GLenum format,
                               GLenum type,
                               const void* pixels);
void(GLAPIENTRYP glTexSubImage3D)(GLenum target,
                                  GLint level,
                                  GLint xoffset,
                                  GLint yoffset,
                                  GLint zoffset,
                                  GLsizei width,
                                  GLsizei height,
                                  GLsizei depth,
                                  GLenum format,
                                  GLenum type,
                                  const void* pixels);
void(GLAPIENTRYP glCopyTexSubImage3D)(
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glActiveTexture)(GLenum texture);
void(GLAPIENTRYP glSampleCoverage)(GLfloat value, GLboolean invert);
void(GLAPIENTRYP glCompressedTexImage3D)(GLenum target,
                                         GLint level,
                                         GLenum internalformat,
                                         GLsizei width,
                                         GLsizei height,
                                         GLsizei depth,
                                         GLint border,
                                         GLsizei imageSize,
                                         const void* data);
void(GLAPIENTRYP glCompressedTexImage2D)(
    GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
void(GLAPIENTRYP glCompressedTexImage1D)(
    GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
void(GLAPIENTRYP glCompressedTexSubImage3D)(GLenum target,
                                            GLint level,
                                            GLint xoffset,
                                            GLint yoffset,
                                            GLint zoffset,
                                            GLsizei width,
                                            GLsizei height,
                                            GLsizei depth,
                                            GLenum format,
                                            GLsizei imageSize,
                                            const void* data);
void(GLAPIENTRYP glCompressedTexSubImage2D)(
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
void(GLAPIENTRYP glCompressedTexSubImage1D)(
    GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
void(GLAPIENTRYP glGetCompressedTexImage)(GLenum target, GLint level, void* img);
void(GLAPIENTRYP glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
void(GLAPIENTRYP glMultiDrawArrays)(GLenum mode, const GLint* first, const GLsizei* count, GLsizei drawcount);
void(GLAPIENTRYP glMultiDrawElements)(GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount);
void(GLAPIENTRYP glPointParameterf)(GLenum pname, GLfloat param);
void(GLAPIENTRYP glPointParameterfv)(GLenum pname, const GLfloat* params);
void(GLAPIENTRYP glPointParameteri)(GLenum pname, GLint param);
void(GLAPIENTRYP glPointParameteriv)(GLenum pname, const GLint* params);
void(GLAPIENTRYP glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
void(GLAPIENTRYP glBlendEquation)(GLenum mode);
void(GLAPIENTRYP glGenQueries)(GLsizei n, GLuint* ids);
void(GLAPIENTRYP glDeleteQueries)(GLsizei n, const GLuint* ids);
GLboolean(GLAPIENTRYP glIsQuery)(GLuint id);
void(GLAPIENTRYP glBeginQuery)(GLenum target, GLuint id);
void(GLAPIENTRYP glEndQuery)(GLenum target);
void(GLAPIENTRYP glGetQueryiv)(GLenum target, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetQueryObjectiv)(GLuint id, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetQueryObjectuiv)(GLuint id, GLenum pname, GLuint* params);
void(GLAPIENTRYP glBindBuffer)(GLenum target, GLuint buffer);
void(GLAPIENTRYP glDeleteBuffers)(GLsizei n, const GLuint* buffers);
void(GLAPIENTRYP glGenBuffers)(GLsizei n, GLuint* buffers);
GLboolean(GLAPIENTRYP glIsBuffer)(GLuint buffer);
void(GLAPIENTRYP glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
void(GLAPIENTRYP glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
void(GLAPIENTRYP glGetBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, void* data);
void*(GLAPIENTRYP glMapBuffer)(GLenum target, GLenum access);
GLboolean(GLAPIENTRYP glUnmapBuffer)(GLenum target);
void(GLAPIENTRYP glGetBufferParameteriv)(GLenum target, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetBufferPointerv)(GLenum target, GLenum pname, void** params);
void(GLAPIENTRYP glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
void(GLAPIENTRYP glDrawBuffers)(GLsizei n, const GLenum* bufs);
void(GLAPIENTRYP glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
void(GLAPIENTRYP glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
void(GLAPIENTRYP glStencilMaskSeparate)(GLenum face, GLuint mask);
void(GLAPIENTRYP glAttachShader)(GLuint program, GLuint shader);
void(GLAPIENTRYP glBindAttribLocation)(GLuint program, GLuint index, const GLchar* name);
void(GLAPIENTRYP glCompileShader)(GLuint shader);
GLuint(GLAPIENTRYP glCreateProgram)(void);
GLuint(GLAPIENTRYP glCreateShader)(GLenum type);
void(GLAPIENTRYP glDeleteProgram)(GLuint program);
void(GLAPIENTRYP glDeleteShader)(GLuint shader);
void(GLAPIENTRYP glDetachShader)(GLuint program, GLuint shader);
void(GLAPIENTRYP glDisableVertexAttribArray)(GLuint index);
void(GLAPIENTRYP glEnableVertexAttribArray)(GLuint index);
void(GLAPIENTRYP glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
void(GLAPIENTRYP glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
void(GLAPIENTRYP glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
GLint(GLAPIENTRYP glGetAttribLocation)(GLuint program, const GLchar* name);
void(GLAPIENTRYP glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void(GLAPIENTRYP glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void(GLAPIENTRYP glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* source);
GLint(GLAPIENTRYP glGetUniformLocation)(GLuint program, const GLchar* name);
void(GLAPIENTRYP glGetUniformfv)(GLuint program, GLint location, GLfloat* params);
void(GLAPIENTRYP glGetUniformiv)(GLuint program, GLint location, GLint* params);
void(GLAPIENTRYP glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble* params);
void(GLAPIENTRYP glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat* params);
void(GLAPIENTRYP glGetVertexAttribiv)(GLuint index, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetVertexAttribPointerv)(GLuint index, GLenum pname, void** pointer);
GLboolean(GLAPIENTRYP glIsProgram)(GLuint program);
GLboolean(GLAPIENTRYP glIsShader)(GLuint shader);
void(GLAPIENTRYP glLinkProgram)(GLuint program);
void(GLAPIENTRYP glShaderSource)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
void(GLAPIENTRYP glUseProgram)(GLuint program);
void(GLAPIENTRYP glUniform1f)(GLint location, GLfloat v0);
void(GLAPIENTRYP glUniform2f)(GLint location, GLfloat v0, GLfloat v1);
void(GLAPIENTRYP glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void(GLAPIENTRYP glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void(GLAPIENTRYP glUniform1i)(GLint location, GLint v0);
void(GLAPIENTRYP glUniform2i)(GLint location, GLint v0, GLint v1);
void(GLAPIENTRYP glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2);
void(GLAPIENTRYP glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void(GLAPIENTRYP glUniform1fv)(GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glUniform2fv)(GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glUniform3fv)(GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glUniform4fv)(GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glUniform1iv)(GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glUniform2iv)(GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glUniform3iv)(GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glUniform4iv)(GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glValidateProgram)(GLuint program);
void(GLAPIENTRYP glVertexAttrib1d)(GLuint index, GLdouble x);
void(GLAPIENTRYP glVertexAttrib1dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttrib1f)(GLuint index, GLfloat x);
void(GLAPIENTRYP glVertexAttrib1fv)(GLuint index, const GLfloat* v);
void(GLAPIENTRYP glVertexAttrib1s)(GLuint index, GLshort x);
void(GLAPIENTRYP glVertexAttrib1sv)(GLuint index, const GLshort* v);
void(GLAPIENTRYP glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y);
void(GLAPIENTRYP glVertexAttrib2dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y);
void(GLAPIENTRYP glVertexAttrib2fv)(GLuint index, const GLfloat* v);
void(GLAPIENTRYP glVertexAttrib2s)(GLuint index, GLshort x, GLshort y);
void(GLAPIENTRYP glVertexAttrib2sv)(GLuint index, const GLshort* v);
void(GLAPIENTRYP glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void(GLAPIENTRYP glVertexAttrib3dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z);
void(GLAPIENTRYP glVertexAttrib3fv)(GLuint index, const GLfloat* v);
void(GLAPIENTRYP glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z);
void(GLAPIENTRYP glVertexAttrib3sv)(GLuint index, const GLshort* v);
void(GLAPIENTRYP glVertexAttrib4Nbv)(GLuint index, const GLbyte* v);
void(GLAPIENTRYP glVertexAttrib4Niv)(GLuint index, const GLint* v);
void(GLAPIENTRYP glVertexAttrib4Nsv)(GLuint index, const GLshort* v);
void(GLAPIENTRYP glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
void(GLAPIENTRYP glVertexAttrib4Nubv)(GLuint index, const GLubyte* v);
void(GLAPIENTRYP glVertexAttrib4Nuiv)(GLuint index, const GLuint* v);
void(GLAPIENTRYP glVertexAttrib4Nusv)(GLuint index, const GLushort* v);
void(GLAPIENTRYP glVertexAttrib4bv)(GLuint index, const GLbyte* v);
void(GLAPIENTRYP glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void(GLAPIENTRYP glVertexAttrib4dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
void(GLAPIENTRYP glVertexAttrib4fv)(GLuint index, const GLfloat* v);
void(GLAPIENTRYP glVertexAttrib4iv)(GLuint index, const GLint* v);
void(GLAPIENTRYP glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
void(GLAPIENTRYP glVertexAttrib4sv)(GLuint index, const GLshort* v);
void(GLAPIENTRYP glVertexAttrib4ubv)(GLuint index, const GLubyte* v);
void(GLAPIENTRYP glVertexAttrib4uiv)(GLuint index, const GLuint* v);
void(GLAPIENTRYP glVertexAttrib4usv)(GLuint index, const GLushort* v);
void(GLAPIENTRYP glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
void(GLAPIENTRYP glUniformMatrix2x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glUniformMatrix3x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glUniformMatrix2x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glUniformMatrix4x2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glUniformMatrix3x4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glUniformMatrix4x3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glColorMaski)(GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
void(GLAPIENTRYP glGetBooleani_v)(GLenum target, GLuint index, GLboolean* data);
void(GLAPIENTRYP glGetIntegeri_v)(GLenum target, GLuint index, GLint* data);
void(GLAPIENTRYP glEnablei)(GLenum target, GLuint index);
void(GLAPIENTRYP glDisablei)(GLenum target, GLuint index);
GLboolean(GLAPIENTRYP glIsEnabledi)(GLenum target, GLuint index);
void(GLAPIENTRYP glBeginTransformFeedback)(GLenum primitiveMode);
void(GLAPIENTRYP glEndTransformFeedback)(void);
void(GLAPIENTRYP glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void(GLAPIENTRYP glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
void(GLAPIENTRYP glTransformFeedbackVaryings)(GLuint program, GLsizei count, const GLchar* const* varyings, GLenum bufferMode);
void(GLAPIENTRYP glGetTransformFeedbackVarying)(
    GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei* size, GLenum* type, GLchar* name);
void(GLAPIENTRYP glClampColor)(GLenum target, GLenum clamp);
void(GLAPIENTRYP glBeginConditionalRender)(GLuint id, GLenum mode);
void(GLAPIENTRYP glEndConditionalRender)(void);
void(GLAPIENTRYP glVertexAttribIPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
void(GLAPIENTRYP glGetVertexAttribIiv)(GLuint index, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetVertexAttribIuiv)(GLuint index, GLenum pname, GLuint* params);
void(GLAPIENTRYP glVertexAttribI1i)(GLuint index, GLint x);
void(GLAPIENTRYP glVertexAttribI2i)(GLuint index, GLint x, GLint y);
void(GLAPIENTRYP glVertexAttribI3i)(GLuint index, GLint x, GLint y, GLint z);
void(GLAPIENTRYP glVertexAttribI4i)(GLuint index, GLint x, GLint y, GLint z, GLint w);
void(GLAPIENTRYP glVertexAttribI1ui)(GLuint index, GLuint x);
void(GLAPIENTRYP glVertexAttribI2ui)(GLuint index, GLuint x, GLuint y);
void(GLAPIENTRYP glVertexAttribI3ui)(GLuint index, GLuint x, GLuint y, GLuint z);
void(GLAPIENTRYP glVertexAttribI4ui)(GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
void(GLAPIENTRYP glVertexAttribI1iv)(GLuint index, const GLint* v);
void(GLAPIENTRYP glVertexAttribI2iv)(GLuint index, const GLint* v);
void(GLAPIENTRYP glVertexAttribI3iv)(GLuint index, const GLint* v);
void(GLAPIENTRYP glVertexAttribI4iv)(GLuint index, const GLint* v);
void(GLAPIENTRYP glVertexAttribI1uiv)(GLuint index, const GLuint* v);
void(GLAPIENTRYP glVertexAttribI2uiv)(GLuint index, const GLuint* v);
void(GLAPIENTRYP glVertexAttribI3uiv)(GLuint index, const GLuint* v);
void(GLAPIENTRYP glVertexAttribI4uiv)(GLuint index, const GLuint* v);
void(GLAPIENTRYP glVertexAttribI4bv)(GLuint index, const GLbyte* v);
void(GLAPIENTRYP glVertexAttribI4sv)(GLuint index, const GLshort* v);
void(GLAPIENTRYP glVertexAttribI4ubv)(GLuint index, const GLubyte* v);
void(GLAPIENTRYP glVertexAttribI4usv)(GLuint index, const GLushort* v);
void(GLAPIENTRYP glGetUniformuiv)(GLuint program, GLint location, GLuint* params);
void(GLAPIENTRYP glBindFragDataLocation)(GLuint program, GLuint color, const GLchar* name);
GLint(GLAPIENTRYP glGetFragDataLocation)(GLuint program, const GLchar* name);
void(GLAPIENTRYP glUniform1ui)(GLint location, GLuint v0);
void(GLAPIENTRYP glUniform2ui)(GLint location, GLuint v0, GLuint v1);
void(GLAPIENTRYP glUniform3ui)(GLint location, GLuint v0, GLuint v1, GLuint v2);
void(GLAPIENTRYP glUniform4ui)(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void(GLAPIENTRYP glUniform1uiv)(GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glUniform2uiv)(GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glUniform3uiv)(GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glUniform4uiv)(GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glTexParameterIiv)(GLenum target, GLenum pname, const GLint* params);
void(GLAPIENTRYP glTexParameterIuiv)(GLenum target, GLenum pname, const GLuint* params);
void(GLAPIENTRYP glGetTexParameterIiv)(GLenum target, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetTexParameterIuiv)(GLenum target, GLenum pname, GLuint* params);
void(GLAPIENTRYP glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint* value);
void(GLAPIENTRYP glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint* value);
void(GLAPIENTRYP glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat* value);
void(GLAPIENTRYP glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
const GLubyte*(GLAPIENTRYP glGetStringi)(GLenum name, GLuint index);
GLboolean(GLAPIENTRYP glIsRenderbuffer)(GLuint renderbuffer);
void(GLAPIENTRYP glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
void(GLAPIENTRYP glDeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);
void(GLAPIENTRYP glGenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
void(GLAPIENTRYP glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
void(GLAPIENTRYP glGetRenderbufferParameteriv)(GLenum target, GLenum pname, GLint* params);
GLboolean(GLAPIENTRYP glIsFramebuffer)(GLuint framebuffer);
void(GLAPIENTRYP glBindFramebuffer)(GLenum target, GLuint framebuffer);
void(GLAPIENTRYP glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
void(GLAPIENTRYP glGenFramebuffers)(GLsizei n, GLuint* framebuffers);
GLenum(GLAPIENTRYP glCheckFramebufferStatus)(GLenum target);
void(GLAPIENTRYP glFramebufferTexture1D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void(GLAPIENTRYP glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
void(GLAPIENTRYP glFramebufferTexture3D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
void(GLAPIENTRYP glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void(GLAPIENTRYP glGetFramebufferAttachmentParameteriv)(GLenum target, GLenum attachment, GLenum pname, GLint* params);
void(GLAPIENTRYP glGenerateMipmap)(GLenum target);
void(GLAPIENTRYP glBlitFramebuffer)(
    GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
void(GLAPIENTRYP glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void(GLAPIENTRYP glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
void*(GLAPIENTRYP glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
void(GLAPIENTRYP glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
void(GLAPIENTRYP glBindVertexArray)(GLuint array);
void(GLAPIENTRYP glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
void(GLAPIENTRYP glGenVertexArrays)(GLsizei n, GLuint* arrays);
GLboolean(GLAPIENTRYP glIsVertexArray)(GLuint array);
void(GLAPIENTRYP glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
void(GLAPIENTRYP glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
void(GLAPIENTRYP glTexBuffer)(GLenum target, GLenum internalformat, GLuint buffer);
void(GLAPIENTRYP glPrimitiveRestartIndex)(GLuint index);
void(GLAPIENTRYP glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void(GLAPIENTRYP glGetUniformIndices)(GLuint program, GLsizei uniformCount, const GLchar* const* uniformNames, GLuint* uniformIndices);
void(GLAPIENTRYP glGetActiveUniformsiv)(GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetActiveUniformName)(GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
GLuint(GLAPIENTRYP glGetUniformBlockIndex)(GLuint program, const GLchar* uniformBlockName);
void(GLAPIENTRYP glGetActiveUniformBlockiv)(GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetActiveUniformBlockName)(GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
void(GLAPIENTRYP glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
void(GLAPIENTRYP glDrawElementsBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLint basevertex);
void(GLAPIENTRYP glDrawRangeElementsBaseVertex)(
    GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void* indices, GLint basevertex);
void(GLAPIENTRYP glDrawElementsInstancedBaseVertex)(
    GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex);
void(GLAPIENTRYP glMultiDrawElementsBaseVertex)(
    GLenum mode, const GLsizei* count, GLenum type, const void* const* indices, GLsizei drawcount, const GLint* basevertex);
void(GLAPIENTRYP glProvokingVertex)(GLenum mode);
GLsync(GLAPIENTRYP glFenceSync)(GLenum condition, GLbitfield flags);
GLboolean(GLAPIENTRYP glIsSync)(GLsync sync);
void(GLAPIENTRYP glDeleteSync)(GLsync sync);
GLenum(GLAPIENTRYP glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
void(GLAPIENTRYP glWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);
void(GLAPIENTRYP glGetInteger64v)(GLenum pname, GLint64* data);
void(GLAPIENTRYP glGetSynciv)(GLsync sync, GLenum pname, GLsizei bufSize, GLsizei* length, GLint* values);
void(GLAPIENTRYP glGetInteger64i_v)(GLenum target, GLuint index, GLint64* data);
void(GLAPIENTRYP glGetBufferParameteri64v)(GLenum target, GLenum pname, GLint64* params);
void(GLAPIENTRYP glFramebufferTexture)(GLenum target, GLenum attachment, GLuint texture, GLint level);
void(GLAPIENTRYP glTexImage2DMultisample)(
    GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void(GLAPIENTRYP glTexImage3DMultisample)(
    GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void(GLAPIENTRYP glGetMultisamplefv)(GLenum pname, GLuint index, GLfloat* val);
void(GLAPIENTRYP glSampleMaski)(GLuint maskNumber, GLbitfield mask);
void(GLAPIENTRYP glBindFragDataLocationIndexed)(GLuint program, GLuint colorNumber, GLuint index, const GLchar* name);
GLint(GLAPIENTRYP glGetFragDataIndex)(GLuint program, const GLchar* name);
void(GLAPIENTRYP glGenSamplers)(GLsizei count, GLuint* samplers);
void(GLAPIENTRYP glDeleteSamplers)(GLsizei count, const GLuint* samplers);
GLboolean(GLAPIENTRYP glIsSampler)(GLuint sampler);
void(GLAPIENTRYP glBindSampler)(GLuint unit, GLuint sampler);
void(GLAPIENTRYP glSamplerParameteri)(GLuint sampler, GLenum pname, GLint param);
void(GLAPIENTRYP glSamplerParameteriv)(GLuint sampler, GLenum pname, const GLint* param);
void(GLAPIENTRYP glSamplerParameterf)(GLuint sampler, GLenum pname, GLfloat param);
void(GLAPIENTRYP glSamplerParameterfv)(GLuint sampler, GLenum pname, const GLfloat* param);
void(GLAPIENTRYP glSamplerParameterIiv)(GLuint sampler, GLenum pname, const GLint* param);
void(GLAPIENTRYP glSamplerParameterIuiv)(GLuint sampler, GLenum pname, const GLuint* param);
void(GLAPIENTRYP glGetSamplerParameteriv)(GLuint sampler, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetSamplerParameterIiv)(GLuint sampler, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetSamplerParameterfv)(GLuint sampler, GLenum pname, GLfloat* params);
void(GLAPIENTRYP glGetSamplerParameterIuiv)(GLuint sampler, GLenum pname, GLuint* params);
void(GLAPIENTRYP glQueryCounter)(GLuint id, GLenum target);
void(GLAPIENTRYP glGetQueryObjecti64v)(GLuint id, GLenum pname, GLint64* params);
void(GLAPIENTRYP glGetQueryObjectui64v)(GLuint id, GLenum pname, GLuint64* params);
void(GLAPIENTRYP glVertexAttribDivisor)(GLuint index, GLuint divisor);
void(GLAPIENTRYP glVertexAttribP1ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void(GLAPIENTRYP glVertexAttribP1uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void(GLAPIENTRYP glVertexAttribP2ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void(GLAPIENTRYP glVertexAttribP2uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void(GLAPIENTRYP glVertexAttribP3ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void(GLAPIENTRYP glVertexAttribP3uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void(GLAPIENTRYP glVertexAttribP4ui)(GLuint index, GLenum type, GLboolean normalized, GLuint value);
void(GLAPIENTRYP glVertexAttribP4uiv)(GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
void(GLAPIENTRYP glMinSampleShading)(GLfloat value);
void(GLAPIENTRYP glBlendEquationi)(GLuint buf, GLenum mode);
void(GLAPIENTRYP glBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
void(GLAPIENTRYP glBlendFunci)(GLuint buf, GLenum src, GLenum dst);
void(GLAPIENTRYP glBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void(GLAPIENTRYP glDrawArraysIndirect)(GLenum mode, const void* indirect);
void(GLAPIENTRYP glDrawElementsIndirect)(GLenum mode, GLenum type, const void* indirect);
void(GLAPIENTRYP glUniform1d)(GLint location, GLdouble x);
void(GLAPIENTRYP glUniform2d)(GLint location, GLdouble x, GLdouble y);
void(GLAPIENTRYP glUniform3d)(GLint location, GLdouble x, GLdouble y, GLdouble z);
void(GLAPIENTRYP glUniform4d)(GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void(GLAPIENTRYP glUniform1dv)(GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glUniform2dv)(GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glUniform3dv)(GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glUniform4dv)(GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix2x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix2x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix3x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix3x4dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix4x2dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glUniformMatrix4x3dv)(GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glGetUniformdv)(GLuint program, GLint location, GLdouble* params);
GLint(GLAPIENTRYP glGetSubroutineUniformLocation)(GLuint program, GLenum shadertype, const GLchar* name);
GLuint(GLAPIENTRYP glGetSubroutineIndex)(GLuint program, GLenum shadertype, const GLchar* name);
void(GLAPIENTRYP glGetActiveSubroutineUniformiv)(GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
void(GLAPIENTRYP glGetActiveSubroutineUniformName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar* name);
void(GLAPIENTRYP glGetActiveSubroutineName)(GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar* name);
void(GLAPIENTRYP glUniformSubroutinesuiv)(GLenum shadertype, GLsizei count, const GLuint* indices);
void(GLAPIENTRYP glGetUniformSubroutineuiv)(GLenum shadertype, GLint location, GLuint* params);
void(GLAPIENTRYP glGetProgramStageiv)(GLuint program, GLenum shadertype, GLenum pname, GLint* values);
void(GLAPIENTRYP glPatchParameteri)(GLenum pname, GLint value);
void(GLAPIENTRYP glPatchParameterfv)(GLenum pname, const GLfloat* values);
void(GLAPIENTRYP glBindTransformFeedback)(GLenum target, GLuint id);
void(GLAPIENTRYP glDeleteTransformFeedbacks)(GLsizei n, const GLuint* ids);
void(GLAPIENTRYP glGenTransformFeedbacks)(GLsizei n, GLuint* ids);
GLboolean(GLAPIENTRYP glIsTransformFeedback)(GLuint id);
void(GLAPIENTRYP glPauseTransformFeedback)(void);
void(GLAPIENTRYP glResumeTransformFeedback)(void);
void(GLAPIENTRYP glDrawTransformFeedback)(GLenum mode, GLuint id);
void(GLAPIENTRYP glDrawTransformFeedbackStream)(GLenum mode, GLuint id, GLuint stream);
void(GLAPIENTRYP glBeginQueryIndexed)(GLenum target, GLuint index, GLuint id);
void(GLAPIENTRYP glEndQueryIndexed)(GLenum target, GLuint index);
void(GLAPIENTRYP glGetQueryIndexediv)(GLenum target, GLuint index, GLenum pname, GLint* params);
void(GLAPIENTRYP glReleaseShaderCompiler)(void);
void(GLAPIENTRYP glShaderBinary)(GLsizei count, const GLuint* shaders, GLenum binaryformat, const void* binary, GLsizei length);
void(GLAPIENTRYP glGetShaderPrecisionFormat)(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
void(GLAPIENTRYP glDepthRangef)(GLfloat n, GLfloat f);
void(GLAPIENTRYP glClearDepthf)(GLfloat d);
void(GLAPIENTRYP glGetProgramBinary)(GLuint program, GLsizei bufSize, GLsizei* length, GLenum* binaryFormat, void* binary);
void(GLAPIENTRYP glProgramBinary)(GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
void(GLAPIENTRYP glProgramParameteri)(GLuint program, GLenum pname, GLint value);
void(GLAPIENTRYP glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
void(GLAPIENTRYP glActiveShaderProgram)(GLuint pipeline, GLuint program);
GLuint(GLAPIENTRYP glCreateShaderProgramv)(GLenum type, GLsizei count, const GLchar* const* strings);
void(GLAPIENTRYP glBindProgramPipeline)(GLuint pipeline);
void(GLAPIENTRYP glDeleteProgramPipelines)(GLsizei n, const GLuint* pipelines);
void(GLAPIENTRYP glGenProgramPipelines)(GLsizei n, GLuint* pipelines);
GLboolean(GLAPIENTRYP glIsProgramPipeline)(GLuint pipeline);
void(GLAPIENTRYP glGetProgramPipelineiv)(GLuint pipeline, GLenum pname, GLint* params);
void(GLAPIENTRYP glProgramUniform1i)(GLuint program, GLint location, GLint v0);
void(GLAPIENTRYP glProgramUniform1iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glProgramUniform1f)(GLuint program, GLint location, GLfloat v0);
void(GLAPIENTRYP glProgramUniform1fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glProgramUniform1d)(GLuint program, GLint location, GLdouble v0);
void(GLAPIENTRYP glProgramUniform1dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glProgramUniform1ui)(GLuint program, GLint location, GLuint v0);
void(GLAPIENTRYP glProgramUniform1uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glProgramUniform2i)(GLuint program, GLint location, GLint v0, GLint v1);
void(GLAPIENTRYP glProgramUniform2iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glProgramUniform2f)(GLuint program, GLint location, GLfloat v0, GLfloat v1);
void(GLAPIENTRYP glProgramUniform2fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glProgramUniform2d)(GLuint program, GLint location, GLdouble v0, GLdouble v1);
void(GLAPIENTRYP glProgramUniform2dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glProgramUniform2ui)(GLuint program, GLint location, GLuint v0, GLuint v1);
void(GLAPIENTRYP glProgramUniform2uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glProgramUniform3i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
void(GLAPIENTRYP glProgramUniform3iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glProgramUniform3f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
void(GLAPIENTRYP glProgramUniform3fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glProgramUniform3d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
void(GLAPIENTRYP glProgramUniform3dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glProgramUniform3ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
void(GLAPIENTRYP glProgramUniform3uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glProgramUniform4i)(GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
void(GLAPIENTRYP glProgramUniform4iv)(GLuint program, GLint location, GLsizei count, const GLint* value);
void(GLAPIENTRYP glProgramUniform4f)(GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
void(GLAPIENTRYP glProgramUniform4fv)(GLuint program, GLint location, GLsizei count, const GLfloat* value);
void(GLAPIENTRYP glProgramUniform4d)(GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
void(GLAPIENTRYP glProgramUniform4dv)(GLuint program, GLint location, GLsizei count, const GLdouble* value);
void(GLAPIENTRYP glProgramUniform4ui)(GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
void(GLAPIENTRYP glProgramUniform4uiv)(GLuint program, GLint location, GLsizei count, const GLuint* value);
void(GLAPIENTRYP glProgramUniformMatrix2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix2x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix3x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix2x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix4x2fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix3x4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix4x3fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
void(GLAPIENTRYP glProgramUniformMatrix2x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix3x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix2x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix4x2dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix3x4dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glProgramUniformMatrix4x3dv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
void(GLAPIENTRYP glValidateProgramPipeline)(GLuint pipeline);
void(GLAPIENTRYP glGetProgramPipelineInfoLog)(GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
void(GLAPIENTRYP glVertexAttribL1d)(GLuint index, GLdouble x);
void(GLAPIENTRYP glVertexAttribL2d)(GLuint index, GLdouble x, GLdouble y);
void(GLAPIENTRYP glVertexAttribL3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z);
void(GLAPIENTRYP glVertexAttribL4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
void(GLAPIENTRYP glVertexAttribL1dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttribL2dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttribL3dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttribL4dv)(GLuint index, const GLdouble* v);
void(GLAPIENTRYP glVertexAttribLPointer)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
void(GLAPIENTRYP glGetVertexAttribLdv)(GLuint index, GLenum pname, GLdouble* params);
void(GLAPIENTRYP glViewportArrayv)(GLuint first, GLsizei count, const GLfloat* v);
void(GLAPIENTRYP glViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
void(GLAPIENTRYP glViewportIndexedfv)(GLuint index, const GLfloat* v);
void(GLAPIENTRYP glScissorArrayv)(GLuint first, GLsizei count, const GLint* v);
void(GLAPIENTRYP glScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
void(GLAPIENTRYP glScissorIndexedv)(GLuint index, const GLint* v);
void(GLAPIENTRYP glDepthRangeArrayv)(GLuint first, GLsizei count, const GLdouble* v);
void(GLAPIENTRYP glDepthRangeIndexed)(GLuint index, GLdouble n, GLdouble f);
void(GLAPIENTRYP glGetFloati_v)(GLenum target, GLuint index, GLfloat* data);
void(GLAPIENTRYP glGetDoublei_v)(GLenum target, GLuint index, GLdouble* data);
void(GLAPIENTRYP glDrawArraysInstancedBaseInstance)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
void(GLAPIENTRYP glDrawElementsInstancedBaseInstance)(
    GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLuint baseinstance);
void(GLAPIENTRYP glDrawElementsInstancedBaseVertexBaseInstance)(
    GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
void(GLAPIENTRYP glGetInternalformativ)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);
void(GLAPIENTRYP glGetActiveAtomicCounterBufferiv)(GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);
void(GLAPIENTRYP glBindImageTexture)(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
void(GLAPIENTRYP glMemoryBarrier)(GLbitfield barriers);
void(GLAPIENTRYP glTexStorage1D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
void(GLAPIENTRYP glTexStorage2D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void(GLAPIENTRYP glTexStorage3D)(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
void(GLAPIENTRYP glDrawTransformFeedbackInstanced)(GLenum mode, GLuint id, GLsizei instancecount);
void(GLAPIENTRYP glDrawTransformFeedbackStreamInstanced)(GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
void(GLAPIENTRYP glClearBufferData)(GLenum target, GLenum internalformat, GLenum format, GLenum type, const void* data);
void(GLAPIENTRYP glClearBufferSubData)(
    GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data);
void(GLAPIENTRYP glDispatchCompute)(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
void(GLAPIENTRYP glDispatchComputeIndirect)(GLintptr indirect);
void(GLAPIENTRYP glCopyImageSubData)(GLuint srcName,
                                     GLenum srcTarget,
                                     GLint srcLevel,
                                     GLint srcX,
                                     GLint srcY,
                                     GLint srcZ,
                                     GLuint dstName,
                                     GLenum dstTarget,
                                     GLint dstLevel,
                                     GLint dstX,
                                     GLint dstY,
                                     GLint dstZ,
                                     GLsizei srcWidth,
                                     GLsizei srcHeight,
                                     GLsizei srcDepth);
void(GLAPIENTRYP glFramebufferParameteri)(GLenum target, GLenum pname, GLint param);
void(GLAPIENTRYP glGetFramebufferParameteriv)(GLenum target, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetInternalformati64v)(GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64* params);
void(GLAPIENTRYP glInvalidateTexSubImage)(
    GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
void(GLAPIENTRYP glInvalidateTexImage)(GLuint texture, GLint level);
void(GLAPIENTRYP glInvalidateBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr length);
void(GLAPIENTRYP glInvalidateBufferData)(GLuint buffer);
void(GLAPIENTRYP glInvalidateFramebuffer)(GLenum target, GLsizei numAttachments, const GLenum* attachments);
void(GLAPIENTRYP glInvalidateSubFramebuffer)(
    GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glMultiDrawArraysIndirect)(GLenum mode, const void* indirect, GLsizei drawcount, GLsizei stride);
void(GLAPIENTRYP glMultiDrawElementsIndirect)(GLenum mode, GLenum type, const void* indirect, GLsizei drawcount, GLsizei stride);
void(GLAPIENTRYP glGetProgramInterfaceiv)(GLuint program, GLenum programInterface, GLenum pname, GLint* params);
GLuint(GLAPIENTRYP glGetProgramResourceIndex)(GLuint program, GLenum programInterface, const GLchar* name);
void(GLAPIENTRYP glGetProgramResourceName)(GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, GLchar* name);
void(GLAPIENTRYP glGetProgramResourceiv)(
    GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei* length, GLint* params);
GLint(GLAPIENTRYP glGetProgramResourceLocation)(GLuint program, GLenum programInterface, const GLchar* name);
GLint(GLAPIENTRYP glGetProgramResourceLocationIndex)(GLuint program, GLenum programInterface, const GLchar* name);
void(GLAPIENTRYP glShaderStorageBlockBinding)(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
void(GLAPIENTRYP glTexBufferRange)(GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void(GLAPIENTRYP glTexStorage2DMultisample)(
    GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void(GLAPIENTRYP glTexStorage3DMultisample)(
    GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void(GLAPIENTRYP glTextureView)(
    GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
void(GLAPIENTRYP glBindVertexBuffer)(GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void(GLAPIENTRYP glVertexAttribFormat)(GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void(GLAPIENTRYP glVertexAttribIFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void(GLAPIENTRYP glVertexAttribLFormat)(GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void(GLAPIENTRYP glVertexAttribBinding)(GLuint attribindex, GLuint bindingindex);
void(GLAPIENTRYP glVertexBindingDivisor)(GLuint bindingindex, GLuint divisor);
void(GLAPIENTRYP glDebugMessageControl)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
void(GLAPIENTRYP glDebugMessageInsert)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
void(GLAPIENTRYP glDebugMessageCallback)(GLDEBUGPROC callback, const void* userParam);
GLuint(GLAPIENTRYP glGetDebugMessageLog)(
    GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
void(GLAPIENTRYP glPushDebugGroup)(GLenum source, GLuint id, GLsizei length, const GLchar* message);
void(GLAPIENTRYP glPopDebugGroup)(void);
void(GLAPIENTRYP glObjectLabel)(GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
void(GLAPIENTRYP glGetObjectLabel)(GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar* label);
void(GLAPIENTRYP glObjectPtrLabel)(const void* ptr, GLsizei length, const GLchar* label);
void(GLAPIENTRYP glGetObjectPtrLabel)(const void* ptr, GLsizei bufSize, GLsizei* length, GLchar* label);
void(GLAPIENTRYP glBufferStorage)(GLenum target, GLsizeiptr size, const void* data, GLbitfield flags);
void(GLAPIENTRYP glClearTexImage)(GLuint texture, GLint level, GLenum format, GLenum type, const void* data);
void(GLAPIENTRYP glClearTexSubImage)(GLuint texture,
                                     GLint level,
                                     GLint xoffset,
                                     GLint yoffset,
                                     GLint zoffset,
                                     GLsizei width,
                                     GLsizei height,
                                     GLsizei depth,
                                     GLenum format,
                                     GLenum type,
                                     const void* data);
void(GLAPIENTRYP glBindBuffersBase)(GLenum target, GLuint first, GLsizei count, const GLuint* buffers);
void(GLAPIENTRYP glBindBuffersRange)(
    GLenum target, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizeiptr* sizes);
void(GLAPIENTRYP glBindTextures)(GLuint first, GLsizei count, const GLuint* textures);
void(GLAPIENTRYP glBindSamplers)(GLuint first, GLsizei count, const GLuint* samplers);
void(GLAPIENTRYP glBindImageTextures)(GLuint first, GLsizei count, const GLuint* textures);
void(GLAPIENTRYP glBindVertexBuffers)(GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides);
void(GLAPIENTRYP glClipControl)(GLenum origin, GLenum depth);
void(GLAPIENTRYP glCreateTransformFeedbacks)(GLsizei n, GLuint* ids);
void(GLAPIENTRYP glTransformFeedbackBufferBase)(GLuint xfb, GLuint index, GLuint buffer);
void(GLAPIENTRYP glTransformFeedbackBufferRange)(GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
void(GLAPIENTRYP glGetTransformFeedbackiv)(GLuint xfb, GLenum pname, GLint* param);
void(GLAPIENTRYP glGetTransformFeedbacki_v)(GLuint xfb, GLenum pname, GLuint index, GLint* param);
void(GLAPIENTRYP glGetTransformFeedbacki64_v)(GLuint xfb, GLenum pname, GLuint index, GLint64* param);
void(GLAPIENTRYP glCreateBuffers)(GLsizei n, GLuint* buffers);
void(GLAPIENTRYP glNamedBufferStorage)(GLuint buffer, GLsizeiptr size, const void* data, GLbitfield flags);
void(GLAPIENTRYP glNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
void(GLAPIENTRYP glNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
void(GLAPIENTRYP glCopyNamedBufferSubData)(GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
void(GLAPIENTRYP glClearNamedBufferData)(GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void* data);
void(GLAPIENTRYP glClearNamedBufferSubData)(
    GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void* data);
void*(GLAPIENTRYP glMapNamedBuffer)(GLuint buffer, GLenum access);
void*(GLAPIENTRYP glMapNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
GLboolean(GLAPIENTRYP glUnmapNamedBuffer)(GLuint buffer);
void(GLAPIENTRYP glFlushMappedNamedBufferRange)(GLuint buffer, GLintptr offset, GLsizeiptr length);
void(GLAPIENTRYP glGetNamedBufferParameteriv)(GLuint buffer, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetNamedBufferParameteri64v)(GLuint buffer, GLenum pname, GLint64* params);
void(GLAPIENTRYP glGetNamedBufferPointerv)(GLuint buffer, GLenum pname, void** params);
void(GLAPIENTRYP glGetNamedBufferSubData)(GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
void(GLAPIENTRYP glCreateFramebuffers)(GLsizei n, GLuint* framebuffers);
void(GLAPIENTRYP glNamedFramebufferRenderbuffer)(GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
void(GLAPIENTRYP glNamedFramebufferParameteri)(GLuint framebuffer, GLenum pname, GLint param);
void(GLAPIENTRYP glNamedFramebufferTexture)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
void(GLAPIENTRYP glNamedFramebufferTextureLayer)(GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
void(GLAPIENTRYP glNamedFramebufferDrawBuffer)(GLuint framebuffer, GLenum buf);
void(GLAPIENTRYP glNamedFramebufferDrawBuffers)(GLuint framebuffer, GLsizei n, const GLenum* bufs);
void(GLAPIENTRYP glNamedFramebufferReadBuffer)(GLuint framebuffer, GLenum src);
void(GLAPIENTRYP glInvalidateNamedFramebufferData)(GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments);
void(GLAPIENTRYP glInvalidateNamedFramebufferSubData)(
    GLuint framebuffer, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glClearNamedFramebufferiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint* value);
void(GLAPIENTRYP glClearNamedFramebufferuiv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint* value);
void(GLAPIENTRYP glClearNamedFramebufferfv)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat* value);
void(GLAPIENTRYP glClearNamedFramebufferfi)(GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
void(GLAPIENTRYP glBlitNamedFramebuffer)(GLuint readFramebuffer,
                                         GLuint drawFramebuffer,
                                         GLint srcX0,
                                         GLint srcY0,
                                         GLint srcX1,
                                         GLint srcY1,
                                         GLint dstX0,
                                         GLint dstY0,
                                         GLint dstX1,
                                         GLint dstY1,
                                         GLbitfield mask,
                                         GLenum filter);
GLenum(GLAPIENTRYP glCheckNamedFramebufferStatus)(GLuint framebuffer, GLenum target);
void(GLAPIENTRYP glGetNamedFramebufferParameteriv)(GLuint framebuffer, GLenum pname, GLint* param);
void(GLAPIENTRYP glGetNamedFramebufferAttachmentParameteriv)(GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
void(GLAPIENTRYP glCreateRenderbuffers)(GLsizei n, GLuint* renderbuffers);
void(GLAPIENTRYP glNamedRenderbufferStorage)(GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
void(GLAPIENTRYP glNamedRenderbufferStorageMultisample)(GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
void(GLAPIENTRYP glGetNamedRenderbufferParameteriv)(GLuint renderbuffer, GLenum pname, GLint* params);
void(GLAPIENTRYP glCreateTextures)(GLenum target, GLsizei n, GLuint* textures);
void(GLAPIENTRYP glTextureBuffer)(GLuint texture, GLenum internalformat, GLuint buffer);
void(GLAPIENTRYP glTextureBufferRange)(GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
void(GLAPIENTRYP glTextureStorage1D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
void(GLAPIENTRYP glTextureStorage2D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
void(GLAPIENTRYP glTextureStorage3D)(GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
void(GLAPIENTRYP glTextureStorage2DMultisample)(
    GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
void(GLAPIENTRYP glTextureStorage3DMultisample)(
    GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
void(GLAPIENTRYP glTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
void(GLAPIENTRYP glTextureSubImage2D)(
    GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
void(GLAPIENTRYP glTextureSubImage3D)(GLuint texture,
                                      GLint level,
                                      GLint xoffset,
                                      GLint yoffset,
                                      GLint zoffset,
                                      GLsizei width,
                                      GLsizei height,
                                      GLsizei depth,
                                      GLenum format,
                                      GLenum type,
                                      const void* pixels);
void(GLAPIENTRYP glCompressedTextureSubImage1D)(
    GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
void(GLAPIENTRYP glCompressedTextureSubImage2D)(
    GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
void(GLAPIENTRYP glCompressedTextureSubImage3D)(GLuint texture,
                                                GLint level,
                                                GLint xoffset,
                                                GLint yoffset,
                                                GLint zoffset,
                                                GLsizei width,
                                                GLsizei height,
                                                GLsizei depth,
                                                GLenum format,
                                                GLsizei imageSize,
                                                const void* data);
void(GLAPIENTRYP glCopyTextureSubImage1D)(GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
void(GLAPIENTRYP glCopyTextureSubImage2D)(GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glCopyTextureSubImage3D)(
    GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
void(GLAPIENTRYP glTextureParameterf)(GLuint texture, GLenum pname, GLfloat param);
void(GLAPIENTRYP glTextureParameterfv)(GLuint texture, GLenum pname, const GLfloat* param);
void(GLAPIENTRYP glTextureParameteri)(GLuint texture, GLenum pname, GLint param);
void(GLAPIENTRYP glTextureParameterIiv)(GLuint texture, GLenum pname, const GLint* params);
void(GLAPIENTRYP glTextureParameterIuiv)(GLuint texture, GLenum pname, const GLuint* params);
void(GLAPIENTRYP glTextureParameteriv)(GLuint texture, GLenum pname, const GLint* param);
void(GLAPIENTRYP glGenerateTextureMipmap)(GLuint texture);
void(GLAPIENTRYP glBindTextureUnit)(GLuint unit, GLuint texture);
void(GLAPIENTRYP glGetTextureImage)(GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
void(GLAPIENTRYP glGetCompressedTextureImage)(GLuint texture, GLint level, GLsizei bufSize, void* pixels);
void(GLAPIENTRYP glGetTextureLevelParameterfv)(GLuint texture, GLint level, GLenum pname, GLfloat* params);
void(GLAPIENTRYP glGetTextureLevelParameteriv)(GLuint texture, GLint level, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetTextureParameterfv)(GLuint texture, GLenum pname, GLfloat* params);
void(GLAPIENTRYP glGetTextureParameterIiv)(GLuint texture, GLenum pname, GLint* params);
void(GLAPIENTRYP glGetTextureParameterIuiv)(GLuint texture, GLenum pname, GLuint* params);
void(GLAPIENTRYP glGetTextureParameteriv)(GLuint texture, GLenum pname, GLint* params);
void(GLAPIENTRYP glCreateVertexArrays)(GLsizei n, GLuint* arrays);
void(GLAPIENTRYP glDisableVertexArrayAttrib)(GLuint vaobj, GLuint index);
void(GLAPIENTRYP glEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
void(GLAPIENTRYP glVertexArrayElementBuffer)(GLuint vaobj, GLuint buffer);
void(GLAPIENTRYP glVertexArrayVertexBuffer)(GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
void(GLAPIENTRYP glVertexArrayVertexBuffers)(
    GLuint vaobj, GLuint first, GLsizei count, const GLuint* buffers, const GLintptr* offsets, const GLsizei* strides);
void(GLAPIENTRYP glVertexArrayAttribBinding)(GLuint vaobj, GLuint attribindex, GLuint bindingindex);
void(GLAPIENTRYP glVertexArrayAttribFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
void(GLAPIENTRYP glVertexArrayAttribIFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void(GLAPIENTRYP glVertexArrayAttribLFormat)(GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
void(GLAPIENTRYP glVertexArrayBindingDivisor)(GLuint vaobj, GLuint bindingindex, GLuint divisor);
void(GLAPIENTRYP glGetVertexArrayiv)(GLuint vaobj, GLenum pname, GLint* param);
void(GLAPIENTRYP glGetVertexArrayIndexediv)(GLuint vaobj, GLuint index, GLenum pname, GLint* param);
void(GLAPIENTRYP glGetVertexArrayIndexed64iv)(GLuint vaobj, GLuint index, GLenum pname, GLint64* param);
void(GLAPIENTRYP glCreateSamplers)(GLsizei n, GLuint* samplers);
void(GLAPIENTRYP glCreateProgramPipelines)(GLsizei n, GLuint* pipelines);
void(GLAPIENTRYP glCreateQueries)(GLenum target, GLsizei n, GLuint* ids);
void(GLAPIENTRYP glGetQueryBufferObjecti64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void(GLAPIENTRYP glGetQueryBufferObjectiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void(GLAPIENTRYP glGetQueryBufferObjectui64v)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void(GLAPIENTRYP glGetQueryBufferObjectuiv)(GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
void(GLAPIENTRYP glMemoryBarrierByRegion)(GLbitfield barriers);
void(GLAPIENTRYP glGetTextureSubImage)(GLuint texture,
                                       GLint level,
                                       GLint xoffset,
                                       GLint yoffset,
                                       GLint zoffset,
                                       GLsizei width,
                                       GLsizei height,
                                       GLsizei depth,
                                       GLenum format,
                                       GLenum type,
                                       GLsizei bufSize,
                                       void* pixels);
void(GLAPIENTRYP glGetCompressedTextureSubImage)(GLuint texture,
                                                 GLint level,
                                                 GLint xoffset,
                                                 GLint yoffset,
                                                 GLint zoffset,
                                                 GLsizei width,
                                                 GLsizei height,
                                                 GLsizei depth,
                                                 GLsizei bufSize,
                                                 void* pixels);
GLenum(GLAPIENTRYP glGetGraphicsResetStatus)(void);
void(GLAPIENTRYP glGetnCompressedTexImage)(GLenum target, GLint lod, GLsizei bufSize, void* pixels);
void(GLAPIENTRYP glGetnTexImage)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* pixels);
void(GLAPIENTRYP glGetnUniformdv)(GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
void(GLAPIENTRYP glGetnUniformfv)(GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
void(GLAPIENTRYP glGetnUniformiv)(GLuint program, GLint location, GLsizei bufSize, GLint* params);
void(GLAPIENTRYP glGetnUniformuiv)(GLuint program, GLint location, GLsizei bufSize, GLuint* params);
void(GLAPIENTRYP glReadnPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);
void(GLAPIENTRYP glTextureBarrier)(void);
GLuint64(GLAPIENTRYP glGetTextureHandleARB)(GLuint texture);
GLuint64(GLAPIENTRYP glGetTextureSamplerHandleARB)(GLuint texture, GLuint sampler);
void(GLAPIENTRYP glMakeTextureHandleResidentARB)(GLuint64 handle);
void(GLAPIENTRYP glMakeTextureHandleNonResidentARB)(GLuint64 handle);
GLuint64(GLAPIENTRYP glGetImageHandleARB)(GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
void(GLAPIENTRYP glMakeImageHandleResidentARB)(GLuint64 handle, GLenum access);
void(GLAPIENTRYP glMakeImageHandleNonResidentARB)(GLuint64 handle);
void(GLAPIENTRYP glUniformHandleui64ARB)(GLint location, GLuint64 value);
void(GLAPIENTRYP glUniformHandleui64vARB)(GLint location, GLsizei count, const GLuint64* value);
void(GLAPIENTRYP glProgramUniformHandleui64ARB)(GLuint program, GLint location, GLuint64 value);
void(GLAPIENTRYP glProgramUniformHandleui64vARB)(GLuint program, GLint location, GLsizei count, const GLuint64* values);
GLboolean(GLAPIENTRYP glIsTextureHandleResidentARB)(GLuint64 handle);
GLboolean(GLAPIENTRYP glIsImageHandleResidentARB)(GLuint64 handle);
void(GLAPIENTRYP glVertexAttribL1ui64ARB)(GLuint index, GLuint64EXT x);
void(GLAPIENTRYP glVertexAttribL1ui64vARB)(GLuint index, const GLuint64EXT* v);
void(GLAPIENTRYP glGetVertexAttribLui64vARB)(GLuint index, GLenum pname, GLuint64EXT* params);
GLsync(GLAPIENTRYP glCreateSyncFromCLeventARB)(struct _cl_context* context, struct _cl_event* event, GLbitfield flags);
void(GLAPIENTRYP glDispatchComputeGroupSizeARB)(
    GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z);
void(GLAPIENTRYP glDebugMessageControlARB)(GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
void(GLAPIENTRYP glDebugMessageInsertARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
void(GLAPIENTRYP glDebugMessageCallbackARB)(GLDEBUGPROCARB callback, const void* userParam);
GLuint(GLAPIENTRYP glGetDebugMessageLogARB)(
    GLuint count, GLsizei bufSize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
void(GLAPIENTRYP glBlendEquationiARB)(GLuint buf, GLenum mode);
void(GLAPIENTRYP glBlendEquationSeparateiARB)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);
void(GLAPIENTRYP glBlendFunciARB)(GLuint buf, GLenum src, GLenum dst);
void(GLAPIENTRYP glBlendFuncSeparateiARB)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
void(GLAPIENTRYP glMultiDrawArraysIndirectCountARB)(GLenum mode, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
void(GLAPIENTRYP glMultiDrawElementsIndirectCountARB)(
    GLenum mode, GLenum type, GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
GLenum(GLAPIENTRYP glGetGraphicsResetStatusARB)(void);
void(GLAPIENTRYP glGetnTexImageARB)(GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* img);
void(GLAPIENTRYP glReadnPixelsARB)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);
void(GLAPIENTRYP glGetnCompressedTexImageARB)(GLenum target, GLint lod, GLsizei bufSize, void* img);
void(GLAPIENTRYP glGetnUniformfvARB)(GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
void(GLAPIENTRYP glGetnUniformivARB)(GLuint program, GLint location, GLsizei bufSize, GLint* params);
void(GLAPIENTRYP glGetnUniformuivARB)(GLuint program, GLint location, GLsizei bufSize, GLuint* params);
void(GLAPIENTRYP glGetnUniformdvARB)(GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
void(GLAPIENTRYP glMinSampleShadingARB)(GLfloat value);
void(GLAPIENTRYP glNamedStringARB)(GLenum type, GLint namelen, const GLchar* name, GLint stringlen, const GLchar* string);
void(GLAPIENTRYP glDeleteNamedStringARB)(GLint namelen, const GLchar* name);
void(GLAPIENTRYP glCompileShaderIncludeARB)(GLuint shader, GLsizei count, const GLchar* const* path, const GLint* length);
GLboolean(GLAPIENTRYP glIsNamedStringARB)(GLint namelen, const GLchar* name);
void(GLAPIENTRYP glGetNamedStringARB)(GLint namelen, const GLchar* name, GLsizei bufSize, GLint* stringlen, GLchar* string);
void(GLAPIENTRYP glGetNamedStringivARB)(GLint namelen, const GLchar* name, GLenum pname, GLint* params);
void(GLAPIENTRYP glBufferPageCommitmentARB)(GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit);
void(GLAPIENTRYP glNamedBufferPageCommitmentEXT)(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
void(GLAPIENTRYP glNamedBufferPageCommitmentARB)(GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit);
void(GLAPIENTRYP glTexPageCommitmentARB)(
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit);

#include <SDL.h>

// C++ doesn't like implicit function pointer casts, so here's a workaround.
#define PROC_CAST *(void**)&

void OpenGL_Init()
{
    PROC_CAST glCullFace = SDL_GL_GetProcAddress("glCullFace");
    PROC_CAST glFrontFace = SDL_GL_GetProcAddress("glFrontFace");
    PROC_CAST glHint = SDL_GL_GetProcAddress("glHint");
    PROC_CAST glLineWidth = SDL_GL_GetProcAddress("glLineWidth");
    PROC_CAST glPointSize = SDL_GL_GetProcAddress("glPointSize");
    PROC_CAST glPolygonMode = SDL_GL_GetProcAddress("glPolygonMode");
    PROC_CAST glScissor = SDL_GL_GetProcAddress("glScissor");
    PROC_CAST glTexParameterf = SDL_GL_GetProcAddress("glTexParameterf");
    PROC_CAST glTexParameterfv = SDL_GL_GetProcAddress("glTexParameterfv");
    PROC_CAST glTexParameteri = SDL_GL_GetProcAddress("glTexParameteri");
    PROC_CAST glTexParameteriv = SDL_GL_GetProcAddress("glTexParameteriv");
    PROC_CAST glTexImage1D = SDL_GL_GetProcAddress("glTexImage1D");
    PROC_CAST glTexImage2D = SDL_GL_GetProcAddress("glTexImage2D");
    PROC_CAST glDrawBuffer = SDL_GL_GetProcAddress("glDrawBuffer");
    PROC_CAST glClear = SDL_GL_GetProcAddress("glClear");
    PROC_CAST glClearColor = SDL_GL_GetProcAddress("glClearColor");
    PROC_CAST glClearStencil = SDL_GL_GetProcAddress("glClearStencil");
    PROC_CAST glClearDepth = SDL_GL_GetProcAddress("glClearDepth");
    PROC_CAST glStencilMask = SDL_GL_GetProcAddress("glStencilMask");
    PROC_CAST glColorMask = SDL_GL_GetProcAddress("glColorMask");
    PROC_CAST glDepthMask = SDL_GL_GetProcAddress("glDepthMask");
    PROC_CAST glDisable = SDL_GL_GetProcAddress("glDisable");
    PROC_CAST glEnable = SDL_GL_GetProcAddress("glEnable");
    PROC_CAST glFinish = SDL_GL_GetProcAddress("glFinish");
    PROC_CAST glFlush = SDL_GL_GetProcAddress("glFlush");
    PROC_CAST glBlendFunc = SDL_GL_GetProcAddress("glBlendFunc");
    PROC_CAST glLogicOp = SDL_GL_GetProcAddress("glLogicOp");
    PROC_CAST glStencilFunc = SDL_GL_GetProcAddress("glStencilFunc");
    PROC_CAST glStencilOp = SDL_GL_GetProcAddress("glStencilOp");
    PROC_CAST glDepthFunc = SDL_GL_GetProcAddress("glDepthFunc");
    PROC_CAST glPixelStoref = SDL_GL_GetProcAddress("glPixelStoref");
    PROC_CAST glPixelStorei = SDL_GL_GetProcAddress("glPixelStorei");
    PROC_CAST glReadBuffer = SDL_GL_GetProcAddress("glReadBuffer");
    PROC_CAST glReadPixels = SDL_GL_GetProcAddress("glReadPixels");
    PROC_CAST glGetBooleanv = SDL_GL_GetProcAddress("glGetBooleanv");
    PROC_CAST glGetDoublev = SDL_GL_GetProcAddress("glGetDoublev");
    PROC_CAST glGetError = SDL_GL_GetProcAddress("glGetError");
    PROC_CAST glGetFloatv = SDL_GL_GetProcAddress("glGetFloatv");
    PROC_CAST glGetIntegerv = SDL_GL_GetProcAddress("glGetIntegerv");
    PROC_CAST glGetString = SDL_GL_GetProcAddress("glGetString");
    PROC_CAST glGetTexImage = SDL_GL_GetProcAddress("glGetTexImage");
    PROC_CAST glGetTexParameterfv = SDL_GL_GetProcAddress("glGetTexParameterfv");
    PROC_CAST glGetTexParameteriv = SDL_GL_GetProcAddress("glGetTexParameteriv");
    PROC_CAST glGetTexLevelParameterfv = SDL_GL_GetProcAddress("glGetTexLevelParameterfv");
    PROC_CAST glGetTexLevelParameteriv = SDL_GL_GetProcAddress("glGetTexLevelParameteriv");
    PROC_CAST glIsEnabled = SDL_GL_GetProcAddress("glIsEnabled");
    PROC_CAST glDepthRange = SDL_GL_GetProcAddress("glDepthRange");
    PROC_CAST glViewport = SDL_GL_GetProcAddress("glViewport");
    PROC_CAST glDrawArrays = SDL_GL_GetProcAddress("glDrawArrays");
    PROC_CAST glDrawElements = SDL_GL_GetProcAddress("glDrawElements");
    PROC_CAST glGetPointerv = SDL_GL_GetProcAddress("glGetPointerv");
    PROC_CAST glPolygonOffset = SDL_GL_GetProcAddress("glPolygonOffset");
    PROC_CAST glCopyTexImage1D = SDL_GL_GetProcAddress("glCopyTexImage1D");
    PROC_CAST glCopyTexImage2D = SDL_GL_GetProcAddress("glCopyTexImage2D");
    PROC_CAST glCopyTexSubImage1D = SDL_GL_GetProcAddress("glCopyTexSubImage1D");
    PROC_CAST glCopyTexSubImage2D = SDL_GL_GetProcAddress("glCopyTexSubImage2D");
    PROC_CAST glTexSubImage1D = SDL_GL_GetProcAddress("glTexSubImage1D");
    PROC_CAST glTexSubImage2D = SDL_GL_GetProcAddress("glTexSubImage2D");
    PROC_CAST glBindTexture = SDL_GL_GetProcAddress("glBindTexture");
    PROC_CAST glDeleteTextures = SDL_GL_GetProcAddress("glDeleteTextures");
    PROC_CAST glGenTextures = SDL_GL_GetProcAddress("glGenTextures");
    PROC_CAST glIsTexture = SDL_GL_GetProcAddress("glIsTexture");
    PROC_CAST glDrawRangeElements = SDL_GL_GetProcAddress("glDrawRangeElements");
    PROC_CAST glTexImage3D = SDL_GL_GetProcAddress("glTexImage3D");
    PROC_CAST glTexSubImage3D = SDL_GL_GetProcAddress("glTexSubImage3D");
    PROC_CAST glCopyTexSubImage3D = SDL_GL_GetProcAddress("glCopyTexSubImage3D");
    PROC_CAST glActiveTexture = SDL_GL_GetProcAddress("glActiveTexture");
    PROC_CAST glSampleCoverage = SDL_GL_GetProcAddress("glSampleCoverage");
    PROC_CAST glCompressedTexImage3D = SDL_GL_GetProcAddress("glCompressedTexImage3D");
    PROC_CAST glCompressedTexImage2D = SDL_GL_GetProcAddress("glCompressedTexImage2D");
    PROC_CAST glCompressedTexImage1D = SDL_GL_GetProcAddress("glCompressedTexImage1D");
    PROC_CAST glCompressedTexSubImage3D = SDL_GL_GetProcAddress("glCompressedTexSubImage3D");
    PROC_CAST glCompressedTexSubImage2D = SDL_GL_GetProcAddress("glCompressedTexSubImage2D");
    PROC_CAST glCompressedTexSubImage1D = SDL_GL_GetProcAddress("glCompressedTexSubImage1D");
    PROC_CAST glGetCompressedTexImage = SDL_GL_GetProcAddress("glGetCompressedTexImage");
    PROC_CAST glBlendFuncSeparate = SDL_GL_GetProcAddress("glBlendFuncSeparate");
    PROC_CAST glMultiDrawArrays = SDL_GL_GetProcAddress("glMultiDrawArrays");
    PROC_CAST glMultiDrawElements = SDL_GL_GetProcAddress("glMultiDrawElements");
    PROC_CAST glPointParameterf = SDL_GL_GetProcAddress("glPointParameterf");
    PROC_CAST glPointParameterfv = SDL_GL_GetProcAddress("glPointParameterfv");
    PROC_CAST glPointParameteri = SDL_GL_GetProcAddress("glPointParameteri");
    PROC_CAST glPointParameteriv = SDL_GL_GetProcAddress("glPointParameteriv");
    PROC_CAST glBlendColor = SDL_GL_GetProcAddress("glBlendColor");
    PROC_CAST glBlendEquation = SDL_GL_GetProcAddress("glBlendEquation");
    PROC_CAST glGenQueries = SDL_GL_GetProcAddress("glGenQueries");
    PROC_CAST glDeleteQueries = SDL_GL_GetProcAddress("glDeleteQueries");
    PROC_CAST glIsQuery = SDL_GL_GetProcAddress("glIsQuery");
    PROC_CAST glBeginQuery = SDL_GL_GetProcAddress("glBeginQuery");
    PROC_CAST glEndQuery = SDL_GL_GetProcAddress("glEndQuery");
    PROC_CAST glGetQueryiv = SDL_GL_GetProcAddress("glGetQueryiv");
    PROC_CAST glGetQueryObjectiv = SDL_GL_GetProcAddress("glGetQueryObjectiv");
    PROC_CAST glGetQueryObjectuiv = SDL_GL_GetProcAddress("glGetQueryObjectuiv");
    PROC_CAST glBindBuffer = SDL_GL_GetProcAddress("glBindBuffer");
    PROC_CAST glDeleteBuffers = SDL_GL_GetProcAddress("glDeleteBuffers");
    PROC_CAST glGenBuffers = SDL_GL_GetProcAddress("glGenBuffers");
    PROC_CAST glIsBuffer = SDL_GL_GetProcAddress("glIsBuffer");
    PROC_CAST glBufferData = SDL_GL_GetProcAddress("glBufferData");
    PROC_CAST glBufferSubData = SDL_GL_GetProcAddress("glBufferSubData");
    PROC_CAST glGetBufferSubData = SDL_GL_GetProcAddress("glGetBufferSubData");
    PROC_CAST glMapBuffer = SDL_GL_GetProcAddress("glMapBuffer");
    PROC_CAST glUnmapBuffer = SDL_GL_GetProcAddress("glUnmapBuffer");
    PROC_CAST glGetBufferParameteriv = SDL_GL_GetProcAddress("glGetBufferParameteriv");
    PROC_CAST glGetBufferPointerv = SDL_GL_GetProcAddress("glGetBufferPointerv");
    PROC_CAST glBlendEquationSeparate = SDL_GL_GetProcAddress("glBlendEquationSeparate");
    PROC_CAST glDrawBuffers = SDL_GL_GetProcAddress("glDrawBuffers");
    PROC_CAST glStencilOpSeparate = SDL_GL_GetProcAddress("glStencilOpSeparate");
    PROC_CAST glStencilFuncSeparate = SDL_GL_GetProcAddress("glStencilFuncSeparate");
    PROC_CAST glStencilMaskSeparate = SDL_GL_GetProcAddress("glStencilMaskSeparate");
    PROC_CAST glAttachShader = SDL_GL_GetProcAddress("glAttachShader");
    PROC_CAST glBindAttribLocation = SDL_GL_GetProcAddress("glBindAttribLocation");
    PROC_CAST glCompileShader = SDL_GL_GetProcAddress("glCompileShader");
    PROC_CAST glCreateProgram = SDL_GL_GetProcAddress("glCreateProgram");
    PROC_CAST glCreateShader = SDL_GL_GetProcAddress("glCreateShader");
    PROC_CAST glDeleteProgram = SDL_GL_GetProcAddress("glDeleteProgram");
    PROC_CAST glDeleteShader = SDL_GL_GetProcAddress("glDeleteShader");
    PROC_CAST glDetachShader = SDL_GL_GetProcAddress("glDetachShader");
    PROC_CAST glDisableVertexAttribArray = SDL_GL_GetProcAddress("glDisableVertexAttribArray");
    PROC_CAST glEnableVertexAttribArray = SDL_GL_GetProcAddress("glEnableVertexAttribArray");
    PROC_CAST glGetActiveAttrib = SDL_GL_GetProcAddress("glGetActiveAttrib");
    PROC_CAST glGetActiveUniform = SDL_GL_GetProcAddress("glGetActiveUniform");
    PROC_CAST glGetAttachedShaders = SDL_GL_GetProcAddress("glGetAttachedShaders");
    PROC_CAST glGetAttribLocation = SDL_GL_GetProcAddress("glGetAttribLocation");
    PROC_CAST glGetProgramiv = SDL_GL_GetProcAddress("glGetProgramiv");
    PROC_CAST glGetProgramInfoLog = SDL_GL_GetProcAddress("glGetProgramInfoLog");
    PROC_CAST glGetShaderiv = SDL_GL_GetProcAddress("glGetShaderiv");
    PROC_CAST glGetShaderInfoLog = SDL_GL_GetProcAddress("glGetShaderInfoLog");
    PROC_CAST glGetShaderSource = SDL_GL_GetProcAddress("glGetShaderSource");
    PROC_CAST glGetUniformLocation = SDL_GL_GetProcAddress("glGetUniformLocation");
    PROC_CAST glGetUniformfv = SDL_GL_GetProcAddress("glGetUniformfv");
    PROC_CAST glGetUniformiv = SDL_GL_GetProcAddress("glGetUniformiv");
    PROC_CAST glGetVertexAttribdv = SDL_GL_GetProcAddress("glGetVertexAttribdv");
    PROC_CAST glGetVertexAttribfv = SDL_GL_GetProcAddress("glGetVertexAttribfv");
    PROC_CAST glGetVertexAttribiv = SDL_GL_GetProcAddress("glGetVertexAttribiv");
    PROC_CAST glGetVertexAttribPointerv = SDL_GL_GetProcAddress("glGetVertexAttribPointerv");
    PROC_CAST glIsProgram = SDL_GL_GetProcAddress("glIsProgram");
    PROC_CAST glIsShader = SDL_GL_GetProcAddress("glIsShader");
    PROC_CAST glLinkProgram = SDL_GL_GetProcAddress("glLinkProgram");
    PROC_CAST glShaderSource = SDL_GL_GetProcAddress("glShaderSource");
    PROC_CAST glUseProgram = SDL_GL_GetProcAddress("glUseProgram");
    PROC_CAST glUniform1f = SDL_GL_GetProcAddress("glUniform1f");
    PROC_CAST glUniform2f = SDL_GL_GetProcAddress("glUniform2f");
    PROC_CAST glUniform3f = SDL_GL_GetProcAddress("glUniform3f");
    PROC_CAST glUniform4f = SDL_GL_GetProcAddress("glUniform4f");
    PROC_CAST glUniform1i = SDL_GL_GetProcAddress("glUniform1i");
    PROC_CAST glUniform2i = SDL_GL_GetProcAddress("glUniform2i");
    PROC_CAST glUniform3i = SDL_GL_GetProcAddress("glUniform3i");
    PROC_CAST glUniform4i = SDL_GL_GetProcAddress("glUniform4i");
    PROC_CAST glUniform1fv = SDL_GL_GetProcAddress("glUniform1fv");
    PROC_CAST glUniform2fv = SDL_GL_GetProcAddress("glUniform2fv");
    PROC_CAST glUniform3fv = SDL_GL_GetProcAddress("glUniform3fv");
    PROC_CAST glUniform4fv = SDL_GL_GetProcAddress("glUniform4fv");
    PROC_CAST glUniform1iv = SDL_GL_GetProcAddress("glUniform1iv");
    PROC_CAST glUniform2iv = SDL_GL_GetProcAddress("glUniform2iv");
    PROC_CAST glUniform3iv = SDL_GL_GetProcAddress("glUniform3iv");
    PROC_CAST glUniform4iv = SDL_GL_GetProcAddress("glUniform4iv");
    PROC_CAST glUniformMatrix2fv = SDL_GL_GetProcAddress("glUniformMatrix2fv");
    PROC_CAST glUniformMatrix3fv = SDL_GL_GetProcAddress("glUniformMatrix3fv");
    PROC_CAST glUniformMatrix4fv = SDL_GL_GetProcAddress("glUniformMatrix4fv");
    PROC_CAST glValidateProgram = SDL_GL_GetProcAddress("glValidateProgram");
    PROC_CAST glVertexAttrib1d = SDL_GL_GetProcAddress("glVertexAttrib1d");
    PROC_CAST glVertexAttrib1dv = SDL_GL_GetProcAddress("glVertexAttrib1dv");
    PROC_CAST glVertexAttrib1f = SDL_GL_GetProcAddress("glVertexAttrib1f");
    PROC_CAST glVertexAttrib1fv = SDL_GL_GetProcAddress("glVertexAttrib1fv");
    PROC_CAST glVertexAttrib1s = SDL_GL_GetProcAddress("glVertexAttrib1s");
    PROC_CAST glVertexAttrib1sv = SDL_GL_GetProcAddress("glVertexAttrib1sv");
    PROC_CAST glVertexAttrib2d = SDL_GL_GetProcAddress("glVertexAttrib2d");
    PROC_CAST glVertexAttrib2dv = SDL_GL_GetProcAddress("glVertexAttrib2dv");
    PROC_CAST glVertexAttrib2f = SDL_GL_GetProcAddress("glVertexAttrib2f");
    PROC_CAST glVertexAttrib2fv = SDL_GL_GetProcAddress("glVertexAttrib2fv");
    PROC_CAST glVertexAttrib2s = SDL_GL_GetProcAddress("glVertexAttrib2s");
    PROC_CAST glVertexAttrib2sv = SDL_GL_GetProcAddress("glVertexAttrib2sv");
    PROC_CAST glVertexAttrib3d = SDL_GL_GetProcAddress("glVertexAttrib3d");
    PROC_CAST glVertexAttrib3dv = SDL_GL_GetProcAddress("glVertexAttrib3dv");
    PROC_CAST glVertexAttrib3f = SDL_GL_GetProcAddress("glVertexAttrib3f");
    PROC_CAST glVertexAttrib3fv = SDL_GL_GetProcAddress("glVertexAttrib3fv");
    PROC_CAST glVertexAttrib3s = SDL_GL_GetProcAddress("glVertexAttrib3s");
    PROC_CAST glVertexAttrib3sv = SDL_GL_GetProcAddress("glVertexAttrib3sv");
    PROC_CAST glVertexAttrib4Nbv = SDL_GL_GetProcAddress("glVertexAttrib4Nbv");
    PROC_CAST glVertexAttrib4Niv = SDL_GL_GetProcAddress("glVertexAttrib4Niv");
    PROC_CAST glVertexAttrib4Nsv = SDL_GL_GetProcAddress("glVertexAttrib4Nsv");
    PROC_CAST glVertexAttrib4Nub = SDL_GL_GetProcAddress("glVertexAttrib4Nub");
    PROC_CAST glVertexAttrib4Nubv = SDL_GL_GetProcAddress("glVertexAttrib4Nubv");
    PROC_CAST glVertexAttrib4Nuiv = SDL_GL_GetProcAddress("glVertexAttrib4Nuiv");
    PROC_CAST glVertexAttrib4Nusv = SDL_GL_GetProcAddress("glVertexAttrib4Nusv");
    PROC_CAST glVertexAttrib4bv = SDL_GL_GetProcAddress("glVertexAttrib4bv");
    PROC_CAST glVertexAttrib4d = SDL_GL_GetProcAddress("glVertexAttrib4d");
    PROC_CAST glVertexAttrib4dv = SDL_GL_GetProcAddress("glVertexAttrib4dv");
    PROC_CAST glVertexAttrib4f = SDL_GL_GetProcAddress("glVertexAttrib4f");
    PROC_CAST glVertexAttrib4fv = SDL_GL_GetProcAddress("glVertexAttrib4fv");
    PROC_CAST glVertexAttrib4iv = SDL_GL_GetProcAddress("glVertexAttrib4iv");
    PROC_CAST glVertexAttrib4s = SDL_GL_GetProcAddress("glVertexAttrib4s");
    PROC_CAST glVertexAttrib4sv = SDL_GL_GetProcAddress("glVertexAttrib4sv");
    PROC_CAST glVertexAttrib4ubv = SDL_GL_GetProcAddress("glVertexAttrib4ubv");
    PROC_CAST glVertexAttrib4uiv = SDL_GL_GetProcAddress("glVertexAttrib4uiv");
    PROC_CAST glVertexAttrib4usv = SDL_GL_GetProcAddress("glVertexAttrib4usv");
    PROC_CAST glVertexAttribPointer = SDL_GL_GetProcAddress("glVertexAttribPointer");
    PROC_CAST glUniformMatrix2x3fv = SDL_GL_GetProcAddress("glUniformMatrix2x3fv");
    PROC_CAST glUniformMatrix3x2fv = SDL_GL_GetProcAddress("glUniformMatrix3x2fv");
    PROC_CAST glUniformMatrix2x4fv = SDL_GL_GetProcAddress("glUniformMatrix2x4fv");
    PROC_CAST glUniformMatrix4x2fv = SDL_GL_GetProcAddress("glUniformMatrix4x2fv");
    PROC_CAST glUniformMatrix3x4fv = SDL_GL_GetProcAddress("glUniformMatrix3x4fv");
    PROC_CAST glUniformMatrix4x3fv = SDL_GL_GetProcAddress("glUniformMatrix4x3fv");
    PROC_CAST glColorMaski = SDL_GL_GetProcAddress("glColorMaski");
    PROC_CAST glGetBooleani_v = SDL_GL_GetProcAddress("glGetBooleani_v");
    PROC_CAST glGetIntegeri_v = SDL_GL_GetProcAddress("glGetIntegeri_v");
    PROC_CAST glEnablei = SDL_GL_GetProcAddress("glEnablei");
    PROC_CAST glDisablei = SDL_GL_GetProcAddress("glDisablei");
    PROC_CAST glIsEnabledi = SDL_GL_GetProcAddress("glIsEnabledi");
    PROC_CAST glBeginTransformFeedback = SDL_GL_GetProcAddress("glBeginTransformFeedback");
    PROC_CAST glEndTransformFeedback = SDL_GL_GetProcAddress("glEndTransformFeedback");
    PROC_CAST glBindBufferRange = SDL_GL_GetProcAddress("glBindBufferRange");
    PROC_CAST glBindBufferBase = SDL_GL_GetProcAddress("glBindBufferBase");
    PROC_CAST glTransformFeedbackVaryings = SDL_GL_GetProcAddress("glTransformFeedbackVaryings");
    PROC_CAST glGetTransformFeedbackVarying = SDL_GL_GetProcAddress("glGetTransformFeedbackVarying");
    PROC_CAST glClampColor = SDL_GL_GetProcAddress("glClampColor");
    PROC_CAST glBeginConditionalRender = SDL_GL_GetProcAddress("glBeginConditionalRender");
    PROC_CAST glEndConditionalRender = SDL_GL_GetProcAddress("glEndConditionalRender");
    PROC_CAST glVertexAttribIPointer = SDL_GL_GetProcAddress("glVertexAttribIPointer");
    PROC_CAST glGetVertexAttribIiv = SDL_GL_GetProcAddress("glGetVertexAttribIiv");
    PROC_CAST glGetVertexAttribIuiv = SDL_GL_GetProcAddress("glGetVertexAttribIuiv");
    PROC_CAST glVertexAttribI1i = SDL_GL_GetProcAddress("glVertexAttribI1i");
    PROC_CAST glVertexAttribI2i = SDL_GL_GetProcAddress("glVertexAttribI2i");
    PROC_CAST glVertexAttribI3i = SDL_GL_GetProcAddress("glVertexAttribI3i");
    PROC_CAST glVertexAttribI4i = SDL_GL_GetProcAddress("glVertexAttribI4i");
    PROC_CAST glVertexAttribI1ui = SDL_GL_GetProcAddress("glVertexAttribI1ui");
    PROC_CAST glVertexAttribI2ui = SDL_GL_GetProcAddress("glVertexAttribI2ui");
    PROC_CAST glVertexAttribI3ui = SDL_GL_GetProcAddress("glVertexAttribI3ui");
    PROC_CAST glVertexAttribI4ui = SDL_GL_GetProcAddress("glVertexAttribI4ui");
    PROC_CAST glVertexAttribI1iv = SDL_GL_GetProcAddress("glVertexAttribI1iv");
    PROC_CAST glVertexAttribI2iv = SDL_GL_GetProcAddress("glVertexAttribI2iv");
    PROC_CAST glVertexAttribI3iv = SDL_GL_GetProcAddress("glVertexAttribI3iv");
    PROC_CAST glVertexAttribI4iv = SDL_GL_GetProcAddress("glVertexAttribI4iv");
    PROC_CAST glVertexAttribI1uiv = SDL_GL_GetProcAddress("glVertexAttribI1uiv");
    PROC_CAST glVertexAttribI2uiv = SDL_GL_GetProcAddress("glVertexAttribI2uiv");
    PROC_CAST glVertexAttribI3uiv = SDL_GL_GetProcAddress("glVertexAttribI3uiv");
    PROC_CAST glVertexAttribI4uiv = SDL_GL_GetProcAddress("glVertexAttribI4uiv");
    PROC_CAST glVertexAttribI4bv = SDL_GL_GetProcAddress("glVertexAttribI4bv");
    PROC_CAST glVertexAttribI4sv = SDL_GL_GetProcAddress("glVertexAttribI4sv");
    PROC_CAST glVertexAttribI4ubv = SDL_GL_GetProcAddress("glVertexAttribI4ubv");
    PROC_CAST glVertexAttribI4usv = SDL_GL_GetProcAddress("glVertexAttribI4usv");
    PROC_CAST glGetUniformuiv = SDL_GL_GetProcAddress("glGetUniformuiv");
    PROC_CAST glBindFragDataLocation = SDL_GL_GetProcAddress("glBindFragDataLocation");
    PROC_CAST glGetFragDataLocation = SDL_GL_GetProcAddress("glGetFragDataLocation");
    PROC_CAST glUniform1ui = SDL_GL_GetProcAddress("glUniform1ui");
    PROC_CAST glUniform2ui = SDL_GL_GetProcAddress("glUniform2ui");
    PROC_CAST glUniform3ui = SDL_GL_GetProcAddress("glUniform3ui");
    PROC_CAST glUniform4ui = SDL_GL_GetProcAddress("glUniform4ui");
    PROC_CAST glUniform1uiv = SDL_GL_GetProcAddress("glUniform1uiv");
    PROC_CAST glUniform2uiv = SDL_GL_GetProcAddress("glUniform2uiv");
    PROC_CAST glUniform3uiv = SDL_GL_GetProcAddress("glUniform3uiv");
    PROC_CAST glUniform4uiv = SDL_GL_GetProcAddress("glUniform4uiv");
    PROC_CAST glTexParameterIiv = SDL_GL_GetProcAddress("glTexParameterIiv");
    PROC_CAST glTexParameterIuiv = SDL_GL_GetProcAddress("glTexParameterIuiv");
    PROC_CAST glGetTexParameterIiv = SDL_GL_GetProcAddress("glGetTexParameterIiv");
    PROC_CAST glGetTexParameterIuiv = SDL_GL_GetProcAddress("glGetTexParameterIuiv");
    PROC_CAST glClearBufferiv = SDL_GL_GetProcAddress("glClearBufferiv");
    PROC_CAST glClearBufferuiv = SDL_GL_GetProcAddress("glClearBufferuiv");
    PROC_CAST glClearBufferfv = SDL_GL_GetProcAddress("glClearBufferfv");
    PROC_CAST glClearBufferfi = SDL_GL_GetProcAddress("glClearBufferfi");
    PROC_CAST glGetStringi = SDL_GL_GetProcAddress("glGetStringi");
    PROC_CAST glIsRenderbuffer = SDL_GL_GetProcAddress("glIsRenderbuffer");
    PROC_CAST glBindRenderbuffer = SDL_GL_GetProcAddress("glBindRenderbuffer");
    PROC_CAST glDeleteRenderbuffers = SDL_GL_GetProcAddress("glDeleteRenderbuffers");
    PROC_CAST glGenRenderbuffers = SDL_GL_GetProcAddress("glGenRenderbuffers");
    PROC_CAST glRenderbufferStorage = SDL_GL_GetProcAddress("glRenderbufferStorage");
    PROC_CAST glGetRenderbufferParameteriv = SDL_GL_GetProcAddress("glGetRenderbufferParameteriv");
    PROC_CAST glIsFramebuffer = SDL_GL_GetProcAddress("glIsFramebuffer");
    PROC_CAST glBindFramebuffer = SDL_GL_GetProcAddress("glBindFramebuffer");
    PROC_CAST glDeleteFramebuffers = SDL_GL_GetProcAddress("glDeleteFramebuffers");
    PROC_CAST glGenFramebuffers = SDL_GL_GetProcAddress("glGenFramebuffers");
    PROC_CAST glCheckFramebufferStatus = SDL_GL_GetProcAddress("glCheckFramebufferStatus");
    PROC_CAST glFramebufferTexture1D = SDL_GL_GetProcAddress("glFramebufferTexture1D");
    PROC_CAST glFramebufferTexture2D = SDL_GL_GetProcAddress("glFramebufferTexture2D");
    PROC_CAST glFramebufferTexture3D = SDL_GL_GetProcAddress("glFramebufferTexture3D");
    PROC_CAST glFramebufferRenderbuffer = SDL_GL_GetProcAddress("glFramebufferRenderbuffer");
    PROC_CAST glGetFramebufferAttachmentParameteriv = SDL_GL_GetProcAddress("glGetFramebufferAttachmentParameteriv");
    PROC_CAST glGenerateMipmap = SDL_GL_GetProcAddress("glGenerateMipmap");
    PROC_CAST glBlitFramebuffer = SDL_GL_GetProcAddress("glBlitFramebuffer");
    PROC_CAST glRenderbufferStorageMultisample = SDL_GL_GetProcAddress("glRenderbufferStorageMultisample");
    PROC_CAST glFramebufferTextureLayer = SDL_GL_GetProcAddress("glFramebufferTextureLayer");
    PROC_CAST glMapBufferRange = SDL_GL_GetProcAddress("glMapBufferRange");
    PROC_CAST glFlushMappedBufferRange = SDL_GL_GetProcAddress("glFlushMappedBufferRange");
    PROC_CAST glBindVertexArray = SDL_GL_GetProcAddress("glBindVertexArray");
    PROC_CAST glDeleteVertexArrays = SDL_GL_GetProcAddress("glDeleteVertexArrays");
    PROC_CAST glGenVertexArrays = SDL_GL_GetProcAddress("glGenVertexArrays");
    PROC_CAST glIsVertexArray = SDL_GL_GetProcAddress("glIsVertexArray");
    PROC_CAST glDrawArraysInstanced = SDL_GL_GetProcAddress("glDrawArraysInstanced");
    PROC_CAST glDrawElementsInstanced = SDL_GL_GetProcAddress("glDrawElementsInstanced");
    PROC_CAST glTexBuffer = SDL_GL_GetProcAddress("glTexBuffer");
    PROC_CAST glPrimitiveRestartIndex = SDL_GL_GetProcAddress("glPrimitiveRestartIndex");
    PROC_CAST glCopyBufferSubData = SDL_GL_GetProcAddress("glCopyBufferSubData");
    PROC_CAST glGetUniformIndices = SDL_GL_GetProcAddress("glGetUniformIndices");
    PROC_CAST glGetActiveUniformsiv = SDL_GL_GetProcAddress("glGetActiveUniformsiv");
    PROC_CAST glGetActiveUniformName = SDL_GL_GetProcAddress("glGetActiveUniformName");
    PROC_CAST glGetUniformBlockIndex = SDL_GL_GetProcAddress("glGetUniformBlockIndex");
    PROC_CAST glGetActiveUniformBlockiv = SDL_GL_GetProcAddress("glGetActiveUniformBlockiv");
    PROC_CAST glGetActiveUniformBlockName = SDL_GL_GetProcAddress("glGetActiveUniformBlockName");
    PROC_CAST glUniformBlockBinding = SDL_GL_GetProcAddress("glUniformBlockBinding");
    PROC_CAST glDrawElementsBaseVertex = SDL_GL_GetProcAddress("glDrawElementsBaseVertex");
    PROC_CAST glDrawRangeElementsBaseVertex = SDL_GL_GetProcAddress("glDrawRangeElementsBaseVertex");
    PROC_CAST glDrawElementsInstancedBaseVertex = SDL_GL_GetProcAddress("glDrawElementsInstancedBaseVertex");
    PROC_CAST glMultiDrawElementsBaseVertex = SDL_GL_GetProcAddress("glMultiDrawElementsBaseVertex");
    PROC_CAST glProvokingVertex = SDL_GL_GetProcAddress("glProvokingVertex");
    PROC_CAST glFenceSync = SDL_GL_GetProcAddress("glFenceSync");
    PROC_CAST glIsSync = SDL_GL_GetProcAddress("glIsSync");
    PROC_CAST glDeleteSync = SDL_GL_GetProcAddress("glDeleteSync");
    PROC_CAST glClientWaitSync = SDL_GL_GetProcAddress("glClientWaitSync");
    PROC_CAST glWaitSync = SDL_GL_GetProcAddress("glWaitSync");
    PROC_CAST glGetInteger64v = SDL_GL_GetProcAddress("glGetInteger64v");
    PROC_CAST glGetSynciv = SDL_GL_GetProcAddress("glGetSynciv");
    PROC_CAST glGetInteger64i_v = SDL_GL_GetProcAddress("glGetInteger64i_v");
    PROC_CAST glGetBufferParameteri64v = SDL_GL_GetProcAddress("glGetBufferParameteri64v");
    PROC_CAST glFramebufferTexture = SDL_GL_GetProcAddress("glFramebufferTexture");
    PROC_CAST glTexImage2DMultisample = SDL_GL_GetProcAddress("glTexImage2DMultisample");
    PROC_CAST glTexImage3DMultisample = SDL_GL_GetProcAddress("glTexImage3DMultisample");
    PROC_CAST glGetMultisamplefv = SDL_GL_GetProcAddress("glGetMultisamplefv");
    PROC_CAST glSampleMaski = SDL_GL_GetProcAddress("glSampleMaski");
    PROC_CAST glBindFragDataLocationIndexed = SDL_GL_GetProcAddress("glBindFragDataLocationIndexed");
    PROC_CAST glGetFragDataIndex = SDL_GL_GetProcAddress("glGetFragDataIndex");
    PROC_CAST glGenSamplers = SDL_GL_GetProcAddress("glGenSamplers");
    PROC_CAST glDeleteSamplers = SDL_GL_GetProcAddress("glDeleteSamplers");
    PROC_CAST glIsSampler = SDL_GL_GetProcAddress("glIsSampler");
    PROC_CAST glBindSampler = SDL_GL_GetProcAddress("glBindSampler");
    PROC_CAST glSamplerParameteri = SDL_GL_GetProcAddress("glSamplerParameteri");
    PROC_CAST glSamplerParameteriv = SDL_GL_GetProcAddress("glSamplerParameteriv");
    PROC_CAST glSamplerParameterf = SDL_GL_GetProcAddress("glSamplerParameterf");
    PROC_CAST glSamplerParameterfv = SDL_GL_GetProcAddress("glSamplerParameterfv");
    PROC_CAST glSamplerParameterIiv = SDL_GL_GetProcAddress("glSamplerParameterIiv");
    PROC_CAST glSamplerParameterIuiv = SDL_GL_GetProcAddress("glSamplerParameterIuiv");
    PROC_CAST glGetSamplerParameteriv = SDL_GL_GetProcAddress("glGetSamplerParameteriv");
    PROC_CAST glGetSamplerParameterIiv = SDL_GL_GetProcAddress("glGetSamplerParameterIiv");
    PROC_CAST glGetSamplerParameterfv = SDL_GL_GetProcAddress("glGetSamplerParameterfv");
    PROC_CAST glGetSamplerParameterIuiv = SDL_GL_GetProcAddress("glGetSamplerParameterIuiv");
    PROC_CAST glQueryCounter = SDL_GL_GetProcAddress("glQueryCounter");
    PROC_CAST glGetQueryObjecti64v = SDL_GL_GetProcAddress("glGetQueryObjecti64v");
    PROC_CAST glGetQueryObjectui64v = SDL_GL_GetProcAddress("glGetQueryObjectui64v");
    PROC_CAST glVertexAttribDivisor = SDL_GL_GetProcAddress("glVertexAttribDivisor");
    PROC_CAST glVertexAttribP1ui = SDL_GL_GetProcAddress("glVertexAttribP1ui");
    PROC_CAST glVertexAttribP1uiv = SDL_GL_GetProcAddress("glVertexAttribP1uiv");
    PROC_CAST glVertexAttribP2ui = SDL_GL_GetProcAddress("glVertexAttribP2ui");
    PROC_CAST glVertexAttribP2uiv = SDL_GL_GetProcAddress("glVertexAttribP2uiv");
    PROC_CAST glVertexAttribP3ui = SDL_GL_GetProcAddress("glVertexAttribP3ui");
    PROC_CAST glVertexAttribP3uiv = SDL_GL_GetProcAddress("glVertexAttribP3uiv");
    PROC_CAST glVertexAttribP4ui = SDL_GL_GetProcAddress("glVertexAttribP4ui");
    PROC_CAST glVertexAttribP4uiv = SDL_GL_GetProcAddress("glVertexAttribP4uiv");
    PROC_CAST glMinSampleShading = SDL_GL_GetProcAddress("glMinSampleShading");
    PROC_CAST glBlendEquationi = SDL_GL_GetProcAddress("glBlendEquationi");
    PROC_CAST glBlendEquationSeparatei = SDL_GL_GetProcAddress("glBlendEquationSeparatei");
    PROC_CAST glBlendFunci = SDL_GL_GetProcAddress("glBlendFunci");
    PROC_CAST glBlendFuncSeparatei = SDL_GL_GetProcAddress("glBlendFuncSeparatei");
    PROC_CAST glDrawArraysIndirect = SDL_GL_GetProcAddress("glDrawArraysIndirect");
    PROC_CAST glDrawElementsIndirect = SDL_GL_GetProcAddress("glDrawElementsIndirect");
    PROC_CAST glUniform1d = SDL_GL_GetProcAddress("glUniform1d");
    PROC_CAST glUniform2d = SDL_GL_GetProcAddress("glUniform2d");
    PROC_CAST glUniform3d = SDL_GL_GetProcAddress("glUniform3d");
    PROC_CAST glUniform4d = SDL_GL_GetProcAddress("glUniform4d");
    PROC_CAST glUniform1dv = SDL_GL_GetProcAddress("glUniform1dv");
    PROC_CAST glUniform2dv = SDL_GL_GetProcAddress("glUniform2dv");
    PROC_CAST glUniform3dv = SDL_GL_GetProcAddress("glUniform3dv");
    PROC_CAST glUniform4dv = SDL_GL_GetProcAddress("glUniform4dv");
    PROC_CAST glUniformMatrix2dv = SDL_GL_GetProcAddress("glUniformMatrix2dv");
    PROC_CAST glUniformMatrix3dv = SDL_GL_GetProcAddress("glUniformMatrix3dv");
    PROC_CAST glUniformMatrix4dv = SDL_GL_GetProcAddress("glUniformMatrix4dv");
    PROC_CAST glUniformMatrix2x3dv = SDL_GL_GetProcAddress("glUniformMatrix2x3dv");
    PROC_CAST glUniformMatrix2x4dv = SDL_GL_GetProcAddress("glUniformMatrix2x4dv");
    PROC_CAST glUniformMatrix3x2dv = SDL_GL_GetProcAddress("glUniformMatrix3x2dv");
    PROC_CAST glUniformMatrix3x4dv = SDL_GL_GetProcAddress("glUniformMatrix3x4dv");
    PROC_CAST glUniformMatrix4x2dv = SDL_GL_GetProcAddress("glUniformMatrix4x2dv");
    PROC_CAST glUniformMatrix4x3dv = SDL_GL_GetProcAddress("glUniformMatrix4x3dv");
    PROC_CAST glGetUniformdv = SDL_GL_GetProcAddress("glGetUniformdv");
    PROC_CAST glGetSubroutineUniformLocation = SDL_GL_GetProcAddress("glGetSubroutineUniformLocation");
    PROC_CAST glGetSubroutineIndex = SDL_GL_GetProcAddress("glGetSubroutineIndex");
    PROC_CAST glGetActiveSubroutineUniformiv = SDL_GL_GetProcAddress("glGetActiveSubroutineUniformiv");
    PROC_CAST glGetActiveSubroutineUniformName = SDL_GL_GetProcAddress("glGetActiveSubroutineUniformName");
    PROC_CAST glGetActiveSubroutineName = SDL_GL_GetProcAddress("glGetActiveSubroutineName");
    PROC_CAST glUniformSubroutinesuiv = SDL_GL_GetProcAddress("glUniformSubroutinesuiv");
    PROC_CAST glGetUniformSubroutineuiv = SDL_GL_GetProcAddress("glGetUniformSubroutineuiv");
    PROC_CAST glGetProgramStageiv = SDL_GL_GetProcAddress("glGetProgramStageiv");
    PROC_CAST glPatchParameteri = SDL_GL_GetProcAddress("glPatchParameteri");
    PROC_CAST glPatchParameterfv = SDL_GL_GetProcAddress("glPatchParameterfv");
    PROC_CAST glBindTransformFeedback = SDL_GL_GetProcAddress("glBindTransformFeedback");
    PROC_CAST glDeleteTransformFeedbacks = SDL_GL_GetProcAddress("glDeleteTransformFeedbacks");
    PROC_CAST glGenTransformFeedbacks = SDL_GL_GetProcAddress("glGenTransformFeedbacks");
    PROC_CAST glIsTransformFeedback = SDL_GL_GetProcAddress("glIsTransformFeedback");
    PROC_CAST glPauseTransformFeedback = SDL_GL_GetProcAddress("glPauseTransformFeedback");
    PROC_CAST glResumeTransformFeedback = SDL_GL_GetProcAddress("glResumeTransformFeedback");
    PROC_CAST glDrawTransformFeedback = SDL_GL_GetProcAddress("glDrawTransformFeedback");
    PROC_CAST glDrawTransformFeedbackStream = SDL_GL_GetProcAddress("glDrawTransformFeedbackStream");
    PROC_CAST glBeginQueryIndexed = SDL_GL_GetProcAddress("glBeginQueryIndexed");
    PROC_CAST glEndQueryIndexed = SDL_GL_GetProcAddress("glEndQueryIndexed");
    PROC_CAST glGetQueryIndexediv = SDL_GL_GetProcAddress("glGetQueryIndexediv");
    PROC_CAST glReleaseShaderCompiler = SDL_GL_GetProcAddress("glReleaseShaderCompiler");
    PROC_CAST glShaderBinary = SDL_GL_GetProcAddress("glShaderBinary");
    PROC_CAST glGetShaderPrecisionFormat = SDL_GL_GetProcAddress("glGetShaderPrecisionFormat");
    PROC_CAST glDepthRangef = SDL_GL_GetProcAddress("glDepthRangef");
    PROC_CAST glClearDepthf = SDL_GL_GetProcAddress("glClearDepthf");
    PROC_CAST glGetProgramBinary = SDL_GL_GetProcAddress("glGetProgramBinary");
    PROC_CAST glProgramBinary = SDL_GL_GetProcAddress("glProgramBinary");
    PROC_CAST glProgramParameteri = SDL_GL_GetProcAddress("glProgramParameteri");
    PROC_CAST glUseProgramStages = SDL_GL_GetProcAddress("glUseProgramStages");
    PROC_CAST glActiveShaderProgram = SDL_GL_GetProcAddress("glActiveShaderProgram");
    PROC_CAST glCreateShaderProgramv = SDL_GL_GetProcAddress("glCreateShaderProgramv");
    PROC_CAST glBindProgramPipeline = SDL_GL_GetProcAddress("glBindProgramPipeline");
    PROC_CAST glDeleteProgramPipelines = SDL_GL_GetProcAddress("glDeleteProgramPipelines");
    PROC_CAST glGenProgramPipelines = SDL_GL_GetProcAddress("glGenProgramPipelines");
    PROC_CAST glIsProgramPipeline = SDL_GL_GetProcAddress("glIsProgramPipeline");
    PROC_CAST glGetProgramPipelineiv = SDL_GL_GetProcAddress("glGetProgramPipelineiv");
    PROC_CAST glProgramUniform1i = SDL_GL_GetProcAddress("glProgramUniform1i");
    PROC_CAST glProgramUniform1iv = SDL_GL_GetProcAddress("glProgramUniform1iv");
    PROC_CAST glProgramUniform1f = SDL_GL_GetProcAddress("glProgramUniform1f");
    PROC_CAST glProgramUniform1fv = SDL_GL_GetProcAddress("glProgramUniform1fv");
    PROC_CAST glProgramUniform1d = SDL_GL_GetProcAddress("glProgramUniform1d");
    PROC_CAST glProgramUniform1dv = SDL_GL_GetProcAddress("glProgramUniform1dv");
    PROC_CAST glProgramUniform1ui = SDL_GL_GetProcAddress("glProgramUniform1ui");
    PROC_CAST glProgramUniform1uiv = SDL_GL_GetProcAddress("glProgramUniform1uiv");
    PROC_CAST glProgramUniform2i = SDL_GL_GetProcAddress("glProgramUniform2i");
    PROC_CAST glProgramUniform2iv = SDL_GL_GetProcAddress("glProgramUniform2iv");
    PROC_CAST glProgramUniform2f = SDL_GL_GetProcAddress("glProgramUniform2f");
    PROC_CAST glProgramUniform2fv = SDL_GL_GetProcAddress("glProgramUniform2fv");
    PROC_CAST glProgramUniform2d = SDL_GL_GetProcAddress("glProgramUniform2d");
    PROC_CAST glProgramUniform2dv = SDL_GL_GetProcAddress("glProgramUniform2dv");
    PROC_CAST glProgramUniform2ui = SDL_GL_GetProcAddress("glProgramUniform2ui");
    PROC_CAST glProgramUniform2uiv = SDL_GL_GetProcAddress("glProgramUniform2uiv");
    PROC_CAST glProgramUniform3i = SDL_GL_GetProcAddress("glProgramUniform3i");
    PROC_CAST glProgramUniform3iv = SDL_GL_GetProcAddress("glProgramUniform3iv");
    PROC_CAST glProgramUniform3f = SDL_GL_GetProcAddress("glProgramUniform3f");
    PROC_CAST glProgramUniform3fv = SDL_GL_GetProcAddress("glProgramUniform3fv");
    PROC_CAST glProgramUniform3d = SDL_GL_GetProcAddress("glProgramUniform3d");
    PROC_CAST glProgramUniform3dv = SDL_GL_GetProcAddress("glProgramUniform3dv");
    PROC_CAST glProgramUniform3ui = SDL_GL_GetProcAddress("glProgramUniform3ui");
    PROC_CAST glProgramUniform3uiv = SDL_GL_GetProcAddress("glProgramUniform3uiv");
    PROC_CAST glProgramUniform4i = SDL_GL_GetProcAddress("glProgramUniform4i");
    PROC_CAST glProgramUniform4iv = SDL_GL_GetProcAddress("glProgramUniform4iv");
    PROC_CAST glProgramUniform4f = SDL_GL_GetProcAddress("glProgramUniform4f");
    PROC_CAST glProgramUniform4fv = SDL_GL_GetProcAddress("glProgramUniform4fv");
    PROC_CAST glProgramUniform4d = SDL_GL_GetProcAddress("glProgramUniform4d");
    PROC_CAST glProgramUniform4dv = SDL_GL_GetProcAddress("glProgramUniform4dv");
    PROC_CAST glProgramUniform4ui = SDL_GL_GetProcAddress("glProgramUniform4ui");
    PROC_CAST glProgramUniform4uiv = SDL_GL_GetProcAddress("glProgramUniform4uiv");
    PROC_CAST glProgramUniformMatrix2fv = SDL_GL_GetProcAddress("glProgramUniformMatrix2fv");
    PROC_CAST glProgramUniformMatrix3fv = SDL_GL_GetProcAddress("glProgramUniformMatrix3fv");
    PROC_CAST glProgramUniformMatrix4fv = SDL_GL_GetProcAddress("glProgramUniformMatrix4fv");
    PROC_CAST glProgramUniformMatrix2dv = SDL_GL_GetProcAddress("glProgramUniformMatrix2dv");
    PROC_CAST glProgramUniformMatrix3dv = SDL_GL_GetProcAddress("glProgramUniformMatrix3dv");
    PROC_CAST glProgramUniformMatrix4dv = SDL_GL_GetProcAddress("glProgramUniformMatrix4dv");
    PROC_CAST glProgramUniformMatrix2x3fv = SDL_GL_GetProcAddress("glProgramUniformMatrix2x3fv");
    PROC_CAST glProgramUniformMatrix3x2fv = SDL_GL_GetProcAddress("glProgramUniformMatrix3x2fv");
    PROC_CAST glProgramUniformMatrix2x4fv = SDL_GL_GetProcAddress("glProgramUniformMatrix2x4fv");
    PROC_CAST glProgramUniformMatrix4x2fv = SDL_GL_GetProcAddress("glProgramUniformMatrix4x2fv");
    PROC_CAST glProgramUniformMatrix3x4fv = SDL_GL_GetProcAddress("glProgramUniformMatrix3x4fv");
    PROC_CAST glProgramUniformMatrix4x3fv = SDL_GL_GetProcAddress("glProgramUniformMatrix4x3fv");
    PROC_CAST glProgramUniformMatrix2x3dv = SDL_GL_GetProcAddress("glProgramUniformMatrix2x3dv");
    PROC_CAST glProgramUniformMatrix3x2dv = SDL_GL_GetProcAddress("glProgramUniformMatrix3x2dv");
    PROC_CAST glProgramUniformMatrix2x4dv = SDL_GL_GetProcAddress("glProgramUniformMatrix2x4dv");
    PROC_CAST glProgramUniformMatrix4x2dv = SDL_GL_GetProcAddress("glProgramUniformMatrix4x2dv");
    PROC_CAST glProgramUniformMatrix3x4dv = SDL_GL_GetProcAddress("glProgramUniformMatrix3x4dv");
    PROC_CAST glProgramUniformMatrix4x3dv = SDL_GL_GetProcAddress("glProgramUniformMatrix4x3dv");
    PROC_CAST glValidateProgramPipeline = SDL_GL_GetProcAddress("glValidateProgramPipeline");
    PROC_CAST glGetProgramPipelineInfoLog = SDL_GL_GetProcAddress("glGetProgramPipelineInfoLog");
    PROC_CAST glVertexAttribL1d = SDL_GL_GetProcAddress("glVertexAttribL1d");
    PROC_CAST glVertexAttribL2d = SDL_GL_GetProcAddress("glVertexAttribL2d");
    PROC_CAST glVertexAttribL3d = SDL_GL_GetProcAddress("glVertexAttribL3d");
    PROC_CAST glVertexAttribL4d = SDL_GL_GetProcAddress("glVertexAttribL4d");
    PROC_CAST glVertexAttribL1dv = SDL_GL_GetProcAddress("glVertexAttribL1dv");
    PROC_CAST glVertexAttribL2dv = SDL_GL_GetProcAddress("glVertexAttribL2dv");
    PROC_CAST glVertexAttribL3dv = SDL_GL_GetProcAddress("glVertexAttribL3dv");
    PROC_CAST glVertexAttribL4dv = SDL_GL_GetProcAddress("glVertexAttribL4dv");
    PROC_CAST glVertexAttribLPointer = SDL_GL_GetProcAddress("glVertexAttribLPointer");
    PROC_CAST glGetVertexAttribLdv = SDL_GL_GetProcAddress("glGetVertexAttribLdv");
    PROC_CAST glViewportArrayv = SDL_GL_GetProcAddress("glViewportArrayv");
    PROC_CAST glViewportIndexedf = SDL_GL_GetProcAddress("glViewportIndexedf");
    PROC_CAST glViewportIndexedfv = SDL_GL_GetProcAddress("glViewportIndexedfv");
    PROC_CAST glScissorArrayv = SDL_GL_GetProcAddress("glScissorArrayv");
    PROC_CAST glScissorIndexed = SDL_GL_GetProcAddress("glScissorIndexed");
    PROC_CAST glScissorIndexedv = SDL_GL_GetProcAddress("glScissorIndexedv");
    PROC_CAST glDepthRangeArrayv = SDL_GL_GetProcAddress("glDepthRangeArrayv");
    PROC_CAST glDepthRangeIndexed = SDL_GL_GetProcAddress("glDepthRangeIndexed");
    PROC_CAST glGetFloati_v = SDL_GL_GetProcAddress("glGetFloati_v");
    PROC_CAST glGetDoublei_v = SDL_GL_GetProcAddress("glGetDoublei_v");
    PROC_CAST glDrawArraysInstancedBaseInstance = SDL_GL_GetProcAddress("glDrawArraysInstancedBaseInstance");
    PROC_CAST glDrawElementsInstancedBaseInstance = SDL_GL_GetProcAddress("glDrawElementsInstancedBaseInstance");
    PROC_CAST glDrawElementsInstancedBaseVertexBaseInstance = SDL_GL_GetProcAddress("glDrawElementsInstancedBaseVertexBaseInstance");
    PROC_CAST glGetInternalformativ = SDL_GL_GetProcAddress("glGetInternalformativ");
    PROC_CAST glGetActiveAtomicCounterBufferiv = SDL_GL_GetProcAddress("glGetActiveAtomicCounterBufferiv");
    PROC_CAST glBindImageTexture = SDL_GL_GetProcAddress("glBindImageTexture");
    PROC_CAST glMemoryBarrier = SDL_GL_GetProcAddress("glMemoryBarrier");
    PROC_CAST glTexStorage1D = SDL_GL_GetProcAddress("glTexStorage1D");
    PROC_CAST glTexStorage2D = SDL_GL_GetProcAddress("glTexStorage2D");
    PROC_CAST glTexStorage3D = SDL_GL_GetProcAddress("glTexStorage3D");
    PROC_CAST glDrawTransformFeedbackInstanced = SDL_GL_GetProcAddress("glDrawTransformFeedbackInstanced");
    PROC_CAST glDrawTransformFeedbackStreamInstanced = SDL_GL_GetProcAddress("glDrawTransformFeedbackStreamInstanced");
    PROC_CAST glClearBufferData = SDL_GL_GetProcAddress("glClearBufferData");
    PROC_CAST glClearBufferSubData = SDL_GL_GetProcAddress("glClearBufferSubData");
    PROC_CAST glDispatchCompute = SDL_GL_GetProcAddress("glDispatchCompute");
    PROC_CAST glDispatchComputeIndirect = SDL_GL_GetProcAddress("glDispatchComputeIndirect");
    PROC_CAST glCopyImageSubData = SDL_GL_GetProcAddress("glCopyImageSubData");
    PROC_CAST glFramebufferParameteri = SDL_GL_GetProcAddress("glFramebufferParameteri");
    PROC_CAST glGetFramebufferParameteriv = SDL_GL_GetProcAddress("glGetFramebufferParameteriv");
    PROC_CAST glGetInternalformati64v = SDL_GL_GetProcAddress("glGetInternalformati64v");
    PROC_CAST glInvalidateTexSubImage = SDL_GL_GetProcAddress("glInvalidateTexSubImage");
    PROC_CAST glInvalidateTexImage = SDL_GL_GetProcAddress("glInvalidateTexImage");
    PROC_CAST glInvalidateBufferSubData = SDL_GL_GetProcAddress("glInvalidateBufferSubData");
    PROC_CAST glInvalidateBufferData = SDL_GL_GetProcAddress("glInvalidateBufferData");
    PROC_CAST glInvalidateFramebuffer = SDL_GL_GetProcAddress("glInvalidateFramebuffer");
    PROC_CAST glInvalidateSubFramebuffer = SDL_GL_GetProcAddress("glInvalidateSubFramebuffer");
    PROC_CAST glMultiDrawArraysIndirect = SDL_GL_GetProcAddress("glMultiDrawArraysIndirect");
    PROC_CAST glMultiDrawElementsIndirect = SDL_GL_GetProcAddress("glMultiDrawElementsIndirect");
    PROC_CAST glGetProgramInterfaceiv = SDL_GL_GetProcAddress("glGetProgramInterfaceiv");
    PROC_CAST glGetProgramResourceIndex = SDL_GL_GetProcAddress("glGetProgramResourceIndex");
    PROC_CAST glGetProgramResourceName = SDL_GL_GetProcAddress("glGetProgramResourceName");
    PROC_CAST glGetProgramResourceiv = SDL_GL_GetProcAddress("glGetProgramResourceiv");
    PROC_CAST glGetProgramResourceLocation = SDL_GL_GetProcAddress("glGetProgramResourceLocation");
    PROC_CAST glGetProgramResourceLocationIndex = SDL_GL_GetProcAddress("glGetProgramResourceLocationIndex");
    PROC_CAST glShaderStorageBlockBinding = SDL_GL_GetProcAddress("glShaderStorageBlockBinding");
    PROC_CAST glTexBufferRange = SDL_GL_GetProcAddress("glTexBufferRange");
    PROC_CAST glTexStorage2DMultisample = SDL_GL_GetProcAddress("glTexStorage2DMultisample");
    PROC_CAST glTexStorage3DMultisample = SDL_GL_GetProcAddress("glTexStorage3DMultisample");
    PROC_CAST glTextureView = SDL_GL_GetProcAddress("glTextureView");
    PROC_CAST glBindVertexBuffer = SDL_GL_GetProcAddress("glBindVertexBuffer");
    PROC_CAST glVertexAttribFormat = SDL_GL_GetProcAddress("glVertexAttribFormat");
    PROC_CAST glVertexAttribIFormat = SDL_GL_GetProcAddress("glVertexAttribIFormat");
    PROC_CAST glVertexAttribLFormat = SDL_GL_GetProcAddress("glVertexAttribLFormat");
    PROC_CAST glVertexAttribBinding = SDL_GL_GetProcAddress("glVertexAttribBinding");
    PROC_CAST glVertexBindingDivisor = SDL_GL_GetProcAddress("glVertexBindingDivisor");
    PROC_CAST glDebugMessageControl = SDL_GL_GetProcAddress("glDebugMessageControl");
    PROC_CAST glDebugMessageInsert = SDL_GL_GetProcAddress("glDebugMessageInsert");
    PROC_CAST glDebugMessageCallback = SDL_GL_GetProcAddress("glDebugMessageCallback");
    PROC_CAST glGetDebugMessageLog = SDL_GL_GetProcAddress("glGetDebugMessageLog");
    PROC_CAST glPushDebugGroup = SDL_GL_GetProcAddress("glPushDebugGroup");
    PROC_CAST glPopDebugGroup = SDL_GL_GetProcAddress("glPopDebugGroup");
    PROC_CAST glObjectLabel = SDL_GL_GetProcAddress("glObjectLabel");
    PROC_CAST glGetObjectLabel = SDL_GL_GetProcAddress("glGetObjectLabel");
    PROC_CAST glObjectPtrLabel = SDL_GL_GetProcAddress("glObjectPtrLabel");
    PROC_CAST glGetObjectPtrLabel = SDL_GL_GetProcAddress("glGetObjectPtrLabel");
    PROC_CAST glBufferStorage = SDL_GL_GetProcAddress("glBufferStorage");
    PROC_CAST glClearTexImage = SDL_GL_GetProcAddress("glClearTexImage");
    PROC_CAST glClearTexSubImage = SDL_GL_GetProcAddress("glClearTexSubImage");
    PROC_CAST glBindBuffersBase = SDL_GL_GetProcAddress("glBindBuffersBase");
    PROC_CAST glBindBuffersRange = SDL_GL_GetProcAddress("glBindBuffersRange");
    PROC_CAST glBindTextures = SDL_GL_GetProcAddress("glBindTextures");
    PROC_CAST glBindSamplers = SDL_GL_GetProcAddress("glBindSamplers");
    PROC_CAST glBindImageTextures = SDL_GL_GetProcAddress("glBindImageTextures");
    PROC_CAST glBindVertexBuffers = SDL_GL_GetProcAddress("glBindVertexBuffers");
    PROC_CAST glClipControl = SDL_GL_GetProcAddress("glClipControl");
    PROC_CAST glCreateTransformFeedbacks = SDL_GL_GetProcAddress("glCreateTransformFeedbacks");
    PROC_CAST glTransformFeedbackBufferBase = SDL_GL_GetProcAddress("glTransformFeedbackBufferBase");
    PROC_CAST glTransformFeedbackBufferRange = SDL_GL_GetProcAddress("glTransformFeedbackBufferRange");
    PROC_CAST glGetTransformFeedbackiv = SDL_GL_GetProcAddress("glGetTransformFeedbackiv");
    PROC_CAST glGetTransformFeedbacki_v = SDL_GL_GetProcAddress("glGetTransformFeedbacki_v");
    PROC_CAST glGetTransformFeedbacki64_v = SDL_GL_GetProcAddress("glGetTransformFeedbacki64_v");
    PROC_CAST glCreateBuffers = SDL_GL_GetProcAddress("glCreateBuffers");
    PROC_CAST glNamedBufferStorage = SDL_GL_GetProcAddress("glNamedBufferStorage");
    PROC_CAST glNamedBufferData = SDL_GL_GetProcAddress("glNamedBufferData");
    PROC_CAST glNamedBufferSubData = SDL_GL_GetProcAddress("glNamedBufferSubData");
    PROC_CAST glCopyNamedBufferSubData = SDL_GL_GetProcAddress("glCopyNamedBufferSubData");
    PROC_CAST glClearNamedBufferData = SDL_GL_GetProcAddress("glClearNamedBufferData");
    PROC_CAST glClearNamedBufferSubData = SDL_GL_GetProcAddress("glClearNamedBufferSubData");
    PROC_CAST glMapNamedBuffer = SDL_GL_GetProcAddress("glMapNamedBuffer");
    PROC_CAST glMapNamedBufferRange = SDL_GL_GetProcAddress("glMapNamedBufferRange");
    PROC_CAST glUnmapNamedBuffer = SDL_GL_GetProcAddress("glUnmapNamedBuffer");
    PROC_CAST glFlushMappedNamedBufferRange = SDL_GL_GetProcAddress("glFlushMappedNamedBufferRange");
    PROC_CAST glGetNamedBufferParameteriv = SDL_GL_GetProcAddress("glGetNamedBufferParameteriv");
    PROC_CAST glGetNamedBufferParameteri64v = SDL_GL_GetProcAddress("glGetNamedBufferParameteri64v");
    PROC_CAST glGetNamedBufferPointerv = SDL_GL_GetProcAddress("glGetNamedBufferPointerv");
    PROC_CAST glGetNamedBufferSubData = SDL_GL_GetProcAddress("glGetNamedBufferSubData");
    PROC_CAST glCreateFramebuffers = SDL_GL_GetProcAddress("glCreateFramebuffers");
    PROC_CAST glNamedFramebufferRenderbuffer = SDL_GL_GetProcAddress("glNamedFramebufferRenderbuffer");
    PROC_CAST glNamedFramebufferParameteri = SDL_GL_GetProcAddress("glNamedFramebufferParameteri");
    PROC_CAST glNamedFramebufferTexture = SDL_GL_GetProcAddress("glNamedFramebufferTexture");
    PROC_CAST glNamedFramebufferTextureLayer = SDL_GL_GetProcAddress("glNamedFramebufferTextureLayer");
    PROC_CAST glNamedFramebufferDrawBuffer = SDL_GL_GetProcAddress("glNamedFramebufferDrawBuffer");
    PROC_CAST glNamedFramebufferDrawBuffers = SDL_GL_GetProcAddress("glNamedFramebufferDrawBuffers");
    PROC_CAST glNamedFramebufferReadBuffer = SDL_GL_GetProcAddress("glNamedFramebufferReadBuffer");
    PROC_CAST glInvalidateNamedFramebufferData = SDL_GL_GetProcAddress("glInvalidateNamedFramebufferData");
    PROC_CAST glInvalidateNamedFramebufferSubData = SDL_GL_GetProcAddress("glInvalidateNamedFramebufferSubData");
    PROC_CAST glClearNamedFramebufferiv = SDL_GL_GetProcAddress("glClearNamedFramebufferiv");
    PROC_CAST glClearNamedFramebufferuiv = SDL_GL_GetProcAddress("glClearNamedFramebufferuiv");
    PROC_CAST glClearNamedFramebufferfv = SDL_GL_GetProcAddress("glClearNamedFramebufferfv");
    PROC_CAST glClearNamedFramebufferfi = SDL_GL_GetProcAddress("glClearNamedFramebufferfi");
    PROC_CAST glBlitNamedFramebuffer = SDL_GL_GetProcAddress("glBlitNamedFramebuffer");
    PROC_CAST glCheckNamedFramebufferStatus = SDL_GL_GetProcAddress("glCheckNamedFramebufferStatus");
    PROC_CAST glGetNamedFramebufferParameteriv = SDL_GL_GetProcAddress("glGetNamedFramebufferParameteriv");
    PROC_CAST glGetNamedFramebufferAttachmentParameteriv = SDL_GL_GetProcAddress("glGetNamedFramebufferAttachmentParameteriv");
    PROC_CAST glCreateRenderbuffers = SDL_GL_GetProcAddress("glCreateRenderbuffers");
    PROC_CAST glNamedRenderbufferStorage = SDL_GL_GetProcAddress("glNamedRenderbufferStorage");
    PROC_CAST glNamedRenderbufferStorageMultisample = SDL_GL_GetProcAddress("glNamedRenderbufferStorageMultisample");
    PROC_CAST glGetNamedRenderbufferParameteriv = SDL_GL_GetProcAddress("glGetNamedRenderbufferParameteriv");
    PROC_CAST glCreateTextures = SDL_GL_GetProcAddress("glCreateTextures");
    PROC_CAST glTextureBuffer = SDL_GL_GetProcAddress("glTextureBuffer");
    PROC_CAST glTextureBufferRange = SDL_GL_GetProcAddress("glTextureBufferRange");
    PROC_CAST glTextureStorage1D = SDL_GL_GetProcAddress("glTextureStorage1D");
    PROC_CAST glTextureStorage2D = SDL_GL_GetProcAddress("glTextureStorage2D");
    PROC_CAST glTextureStorage3D = SDL_GL_GetProcAddress("glTextureStorage3D");
    PROC_CAST glTextureStorage2DMultisample = SDL_GL_GetProcAddress("glTextureStorage2DMultisample");
    PROC_CAST glTextureStorage3DMultisample = SDL_GL_GetProcAddress("glTextureStorage3DMultisample");
    PROC_CAST glTextureSubImage1D = SDL_GL_GetProcAddress("glTextureSubImage1D");
    PROC_CAST glTextureSubImage2D = SDL_GL_GetProcAddress("glTextureSubImage2D");
    PROC_CAST glTextureSubImage3D = SDL_GL_GetProcAddress("glTextureSubImage3D");
    PROC_CAST glCompressedTextureSubImage1D = SDL_GL_GetProcAddress("glCompressedTextureSubImage1D");
    PROC_CAST glCompressedTextureSubImage2D = SDL_GL_GetProcAddress("glCompressedTextureSubImage2D");
    PROC_CAST glCompressedTextureSubImage3D = SDL_GL_GetProcAddress("glCompressedTextureSubImage3D");
    PROC_CAST glCopyTextureSubImage1D = SDL_GL_GetProcAddress("glCopyTextureSubImage1D");
    PROC_CAST glCopyTextureSubImage2D = SDL_GL_GetProcAddress("glCopyTextureSubImage2D");
    PROC_CAST glCopyTextureSubImage3D = SDL_GL_GetProcAddress("glCopyTextureSubImage3D");
    PROC_CAST glTextureParameterf = SDL_GL_GetProcAddress("glTextureParameterf");
    PROC_CAST glTextureParameterfv = SDL_GL_GetProcAddress("glTextureParameterfv");
    PROC_CAST glTextureParameteri = SDL_GL_GetProcAddress("glTextureParameteri");
    PROC_CAST glTextureParameterIiv = SDL_GL_GetProcAddress("glTextureParameterIiv");
    PROC_CAST glTextureParameterIuiv = SDL_GL_GetProcAddress("glTextureParameterIuiv");
    PROC_CAST glTextureParameteriv = SDL_GL_GetProcAddress("glTextureParameteriv");
    PROC_CAST glGenerateTextureMipmap = SDL_GL_GetProcAddress("glGenerateTextureMipmap");
    PROC_CAST glBindTextureUnit = SDL_GL_GetProcAddress("glBindTextureUnit");
    PROC_CAST glGetTextureImage = SDL_GL_GetProcAddress("glGetTextureImage");
    PROC_CAST glGetCompressedTextureImage = SDL_GL_GetProcAddress("glGetCompressedTextureImage");
    PROC_CAST glGetTextureLevelParameterfv = SDL_GL_GetProcAddress("glGetTextureLevelParameterfv");
    PROC_CAST glGetTextureLevelParameteriv = SDL_GL_GetProcAddress("glGetTextureLevelParameteriv");
    PROC_CAST glGetTextureParameterfv = SDL_GL_GetProcAddress("glGetTextureParameterfv");
    PROC_CAST glGetTextureParameterIiv = SDL_GL_GetProcAddress("glGetTextureParameterIiv");
    PROC_CAST glGetTextureParameterIuiv = SDL_GL_GetProcAddress("glGetTextureParameterIuiv");
    PROC_CAST glGetTextureParameteriv = SDL_GL_GetProcAddress("glGetTextureParameteriv");
    PROC_CAST glCreateVertexArrays = SDL_GL_GetProcAddress("glCreateVertexArrays");
    PROC_CAST glDisableVertexArrayAttrib = SDL_GL_GetProcAddress("glDisableVertexArrayAttrib");
    PROC_CAST glEnableVertexArrayAttrib = SDL_GL_GetProcAddress("glEnableVertexArrayAttrib");
    PROC_CAST glVertexArrayElementBuffer = SDL_GL_GetProcAddress("glVertexArrayElementBuffer");
    PROC_CAST glVertexArrayVertexBuffer = SDL_GL_GetProcAddress("glVertexArrayVertexBuffer");
    PROC_CAST glVertexArrayVertexBuffers = SDL_GL_GetProcAddress("glVertexArrayVertexBuffers");
    PROC_CAST glVertexArrayAttribBinding = SDL_GL_GetProcAddress("glVertexArrayAttribBinding");
    PROC_CAST glVertexArrayAttribFormat = SDL_GL_GetProcAddress("glVertexArrayAttribFormat");
    PROC_CAST glVertexArrayAttribIFormat = SDL_GL_GetProcAddress("glVertexArrayAttribIFormat");
    PROC_CAST glVertexArrayAttribLFormat = SDL_GL_GetProcAddress("glVertexArrayAttribLFormat");
    PROC_CAST glVertexArrayBindingDivisor = SDL_GL_GetProcAddress("glVertexArrayBindingDivisor");
    PROC_CAST glGetVertexArrayiv = SDL_GL_GetProcAddress("glGetVertexArrayiv");
    PROC_CAST glGetVertexArrayIndexediv = SDL_GL_GetProcAddress("glGetVertexArrayIndexediv");
    PROC_CAST glGetVertexArrayIndexed64iv = SDL_GL_GetProcAddress("glGetVertexArrayIndexed64iv");
    PROC_CAST glCreateSamplers = SDL_GL_GetProcAddress("glCreateSamplers");
    PROC_CAST glCreateProgramPipelines = SDL_GL_GetProcAddress("glCreateProgramPipelines");
    PROC_CAST glCreateQueries = SDL_GL_GetProcAddress("glCreateQueries");
    PROC_CAST glGetQueryBufferObjecti64v = SDL_GL_GetProcAddress("glGetQueryBufferObjecti64v");
    PROC_CAST glGetQueryBufferObjectiv = SDL_GL_GetProcAddress("glGetQueryBufferObjectiv");
    PROC_CAST glGetQueryBufferObjectui64v = SDL_GL_GetProcAddress("glGetQueryBufferObjectui64v");
    PROC_CAST glGetQueryBufferObjectuiv = SDL_GL_GetProcAddress("glGetQueryBufferObjectuiv");
    PROC_CAST glMemoryBarrierByRegion = SDL_GL_GetProcAddress("glMemoryBarrierByRegion");
    PROC_CAST glGetTextureSubImage = SDL_GL_GetProcAddress("glGetTextureSubImage");
    PROC_CAST glGetCompressedTextureSubImage = SDL_GL_GetProcAddress("glGetCompressedTextureSubImage");
    PROC_CAST glGetGraphicsResetStatus = SDL_GL_GetProcAddress("glGetGraphicsResetStatus");
    PROC_CAST glGetnCompressedTexImage = SDL_GL_GetProcAddress("glGetnCompressedTexImage");
    PROC_CAST glGetnTexImage = SDL_GL_GetProcAddress("glGetnTexImage");
    PROC_CAST glGetnUniformdv = SDL_GL_GetProcAddress("glGetnUniformdv");
    PROC_CAST glGetnUniformfv = SDL_GL_GetProcAddress("glGetnUniformfv");
    PROC_CAST glGetnUniformiv = SDL_GL_GetProcAddress("glGetnUniformiv");
    PROC_CAST glGetnUniformuiv = SDL_GL_GetProcAddress("glGetnUniformuiv");
    PROC_CAST glReadnPixels = SDL_GL_GetProcAddress("glReadnPixels");
    PROC_CAST glTextureBarrier = SDL_GL_GetProcAddress("glTextureBarrier");
    PROC_CAST glGetTextureHandleARB = SDL_GL_GetProcAddress("glGetTextureHandleARB");
    PROC_CAST glGetTextureSamplerHandleARB = SDL_GL_GetProcAddress("glGetTextureSamplerHandleARB");
    PROC_CAST glMakeTextureHandleResidentARB = SDL_GL_GetProcAddress("glMakeTextureHandleResidentARB");
    PROC_CAST glMakeTextureHandleNonResidentARB = SDL_GL_GetProcAddress("glMakeTextureHandleNonResidentARB");
    PROC_CAST glGetImageHandleARB = SDL_GL_GetProcAddress("glGetImageHandleARB");
    PROC_CAST glMakeImageHandleResidentARB = SDL_GL_GetProcAddress("glMakeImageHandleResidentARB");
    PROC_CAST glMakeImageHandleNonResidentARB = SDL_GL_GetProcAddress("glMakeImageHandleNonResidentARB");
    PROC_CAST glUniformHandleui64ARB = SDL_GL_GetProcAddress("glUniformHandleui64ARB");
    PROC_CAST glUniformHandleui64vARB = SDL_GL_GetProcAddress("glUniformHandleui64vARB");
    PROC_CAST glProgramUniformHandleui64ARB = SDL_GL_GetProcAddress("glProgramUniformHandleui64ARB");
    PROC_CAST glProgramUniformHandleui64vARB = SDL_GL_GetProcAddress("glProgramUniformHandleui64vARB");
    PROC_CAST glIsTextureHandleResidentARB = SDL_GL_GetProcAddress("glIsTextureHandleResidentARB");
    PROC_CAST glIsImageHandleResidentARB = SDL_GL_GetProcAddress("glIsImageHandleResidentARB");
    PROC_CAST glVertexAttribL1ui64ARB = SDL_GL_GetProcAddress("glVertexAttribL1ui64ARB");
    PROC_CAST glVertexAttribL1ui64vARB = SDL_GL_GetProcAddress("glVertexAttribL1ui64vARB");
    PROC_CAST glGetVertexAttribLui64vARB = SDL_GL_GetProcAddress("glGetVertexAttribLui64vARB");
    PROC_CAST glCreateSyncFromCLeventARB = SDL_GL_GetProcAddress("glCreateSyncFromCLeventARB");
    PROC_CAST glDispatchComputeGroupSizeARB = SDL_GL_GetProcAddress("glDispatchComputeGroupSizeARB");
    PROC_CAST glDebugMessageControlARB = SDL_GL_GetProcAddress("glDebugMessageControlARB");
    PROC_CAST glDebugMessageInsertARB = SDL_GL_GetProcAddress("glDebugMessageInsertARB");
    PROC_CAST glDebugMessageCallbackARB = SDL_GL_GetProcAddress("glDebugMessageCallbackARB");
    PROC_CAST glGetDebugMessageLogARB = SDL_GL_GetProcAddress("glGetDebugMessageLogARB");
    PROC_CAST glBlendEquationiARB = SDL_GL_GetProcAddress("glBlendEquationiARB");
    PROC_CAST glBlendEquationSeparateiARB = SDL_GL_GetProcAddress("glBlendEquationSeparateiARB");
    PROC_CAST glBlendFunciARB = SDL_GL_GetProcAddress("glBlendFunciARB");
    PROC_CAST glBlendFuncSeparateiARB = SDL_GL_GetProcAddress("glBlendFuncSeparateiARB");
    PROC_CAST glMultiDrawArraysIndirectCountARB = SDL_GL_GetProcAddress("glMultiDrawArraysIndirectCountARB");
    PROC_CAST glMultiDrawElementsIndirectCountARB = SDL_GL_GetProcAddress("glMultiDrawElementsIndirectCountARB");
    PROC_CAST glGetGraphicsResetStatusARB = SDL_GL_GetProcAddress("glGetGraphicsResetStatusARB");
    PROC_CAST glGetnTexImageARB = SDL_GL_GetProcAddress("glGetnTexImageARB");
    PROC_CAST glReadnPixelsARB = SDL_GL_GetProcAddress("glReadnPixelsARB");
    PROC_CAST glGetnCompressedTexImageARB = SDL_GL_GetProcAddress("glGetnCompressedTexImageARB");
    PROC_CAST glGetnUniformfvARB = SDL_GL_GetProcAddress("glGetnUniformfvARB");
    PROC_CAST glGetnUniformivARB = SDL_GL_GetProcAddress("glGetnUniformivARB");
    PROC_CAST glGetnUniformuivARB = SDL_GL_GetProcAddress("glGetnUniformuivARB");
    PROC_CAST glGetnUniformdvARB = SDL_GL_GetProcAddress("glGetnUniformdvARB");
    PROC_CAST glMinSampleShadingARB = SDL_GL_GetProcAddress("glMinSampleShadingARB");
    PROC_CAST glNamedStringARB = SDL_GL_GetProcAddress("glNamedStringARB");
    PROC_CAST glDeleteNamedStringARB = SDL_GL_GetProcAddress("glDeleteNamedStringARB");
    PROC_CAST glCompileShaderIncludeARB = SDL_GL_GetProcAddress("glCompileShaderIncludeARB");
    PROC_CAST glIsNamedStringARB = SDL_GL_GetProcAddress("glIsNamedStringARB");
    PROC_CAST glGetNamedStringARB = SDL_GL_GetProcAddress("glGetNamedStringARB");
    PROC_CAST glGetNamedStringivARB = SDL_GL_GetProcAddress("glGetNamedStringivARB");
    PROC_CAST glBufferPageCommitmentARB = SDL_GL_GetProcAddress("glBufferPageCommitmentARB");
    PROC_CAST glNamedBufferPageCommitmentEXT = SDL_GL_GetProcAddress("glNamedBufferPageCommitmentEXT");
    PROC_CAST glNamedBufferPageCommitmentARB = SDL_GL_GetProcAddress("glNamedBufferPageCommitmentARB");
    PROC_CAST glTexPageCommitmentARB = SDL_GL_GetProcAddress("glTexPageCommitmentARB");
}