#include "ShaderProgram.h"

GLint ShaderProgram::checkCompileErrors(GLuint shader, std::string type) {
  GLint success;
#ifdef CERR_OUTPUT
  GLchar infoLog[1024];
#endif
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
#ifdef CERR_OUTPUT
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      std::cerr
          << "ERROR. Shader compilation error of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
#endif
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
#ifdef CERR_OUTPUT
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      std::cerr
          << "ERROR. Shader program linking error of type: " << type << "\n"
          << infoLog
          << "\n -- --------------------------------------------------- -- "
          << std::endl;
#endif
    }
  }
  return success;
}

ShaderProgram::ShaderProgram(std::string vertex_shader_code,
                             std::string fragment_shader_code,
                             std::string geometry_shader_code) {
  unsigned int vertex, fragment;
  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  const char* vShaderCode = vertex_shader_code.c_str();
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  int i = checkCompileErrors(vertex, "VERTEX");
  if (not i) {
    throw exceptions::VERTEX_COMPILATION_ERROR;
  }
  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fShaderCode = fragment_shader_code.c_str();
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  i = checkCompileErrors(fragment, "FRAGMENT");
  if (not i) {
    throw exceptions::FRAGMENT_COMPILATION_ERROR;
  }
  // if geometry shader is given, compile geometry shader
  unsigned int geometry = 0;
  if (geometry_shader_code != "") {
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    const char* gShaderCode = geometry_shader_code.c_str();
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCompileShader(geometry);
    i = checkCompileErrors(geometry, "GEOMETRY");
    if (not i) {
      throw exceptions::GEOMETRY_COMPILATION_ERROR;
    }
  }
  // shader Program
  sp_id_ = glCreateProgram();
  glAttachShader(sp_id_, vertex);
  glAttachShader(sp_id_, fragment);
  if (geometry_shader_code != "") glAttachShader(sp_id_, geometry);
  glLinkProgram(sp_id_);
  i = checkCompileErrors(sp_id_, "PROGRAM");
  if (not i) {
    throw exceptions::PROGRAM_COMPILATION_ERROR;
  }
  // delete the shaders as they're linked into our program now and no longer
  // necessery
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (geometry_shader_code != "") glDeleteShader(geometry);
}

ShaderProgram::~ShaderProgram() { glDeleteProgram(sp_id_); }

void ShaderProgram::SetBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(sp_id_, name.c_str()), (int)value);
}

void ShaderProgram::SetInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(sp_id_, name.c_str()), value);
}

void ShaderProgram::SetUInt(const std::string& name, unsigned int value) const {
  glUniform1ui(glGetUniformLocation(sp_id_, name.c_str()), value);
}

void ShaderProgram::SetFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(sp_id_, name.c_str()), value);
}

void ShaderProgram::SetVec1(const std::string& name,
                            const glm::vec1& value) const {
  GLint location = glGetUniformLocation(sp_id_, name.c_str());
  glUniform1fv(location, 1, &value[0]);
}

void ShaderProgram::SetVec2(const std::string& name,
                            const glm::vec2& value) const {
  GLint location = glGetUniformLocation(sp_id_, name.c_str());
  glUniform2fv(location, 1, &value[0]);
}

void ShaderProgram::SetVec3(const std::string& name,
                            const glm::vec3& value) const {
  GLint location = glGetUniformLocation(sp_id_, name.c_str());
  glUniform3fv(location, 1, &value[0]);
}

void ShaderProgram::SetVec4(const std::string& name,
                            const glm::vec4& value) const {
  GLint location = glGetUniformLocation(sp_id_, name.c_str());
  glUniform4fv(location, 1, &value[0]);
}

void ShaderProgram::SetMat2(const std::string& name,
                            const glm::mat2& value) const {
  glUniformMatrix2fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}

void ShaderProgram::SetMat3(const std::string& name,
                            const glm::mat3& value) const {
  glUniformMatrix3fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}

void ShaderProgram::SetMat4(const std::string& name,
                            const glm::mat4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}

void ShaderProgram::SetMat2x2(const std::string& name,
                              const glm::mat2x2& value) const {
  SetMat2(name, value);
}

void ShaderProgram::SetMat2x3(const std::string& name,
                              const glm::mat2x3& value) const {
  glUniformMatrix2x3fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void ShaderProgram::SetMat2x4(const std::string& name,
                              const glm::mat2x4& value) const {
  glUniformMatrix2x4fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void ShaderProgram::SetMat3x2(const std::string& name,
                              const glm::mat3x2& value) const {
  glUniformMatrix3x2fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void ShaderProgram::SetMat3x3(const std::string& name,
                              const glm::mat3x3& value) const {
  SetMat3(name, value);
}

void ShaderProgram::SetMat3x4(const std::string& name,
                              const glm::mat3x4& value) const {
  glUniformMatrix2x4fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void ShaderProgram::SetMat4x2(const std::string& name,
                              const glm::mat4x2& value) const {
  glUniformMatrix4x2fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void ShaderProgram::SetMat4x3(const std::string& name,
                              const glm::mat4x3& value) const {
  glUniformMatrix4x2fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

void ShaderProgram::SetMat4x4(const std::string& name,
                              const glm::mat4x4& value) const {
  glUniformMatrix4fv(glGetUniformLocation(sp_id_, name.c_str()), 1, GL_FALSE,
                     &value[0][0]);
}