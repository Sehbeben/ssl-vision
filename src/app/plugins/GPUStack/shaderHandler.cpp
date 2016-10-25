#include "shaderHandler.h"

using namespace std;


GLuint ShaderHandler::createShaderProgram(const char* path2Vertex, const char* path2fragment)
{
  string vertex_shader_source = ReadShaderFile(path2Vertex);
  GLuint vertex_shader = compileShader(VERTEX, vertex_shader_source);
  
  string fragment_shader_source = ReadShaderFile(path2fragment);
  GLuint fragment_shader = compileShader(FRAGMENT, fragment_shader_source);
  
  
  GLuint programID = LinkProgram(vertex_shader, fragment_shader);

  return programID;
}


string ShaderHandler::ReadShaderFile(const char* file_path)
{
  string shaderCode;
  ifstream shaderStream(file_path, ios::in);
  if(shaderStream.is_open())
  {
    string line="";
    while(getline(shaderStream, line)) shaderCode+="\n"+line;
      shaderStream.close();
  } else
    {
      cout<<"Fehler beim Einlesen der Datei: "<<file_path<<endl;
      return 0;
    }
  return shaderCode;
}


GLuint ShaderHandler::compileShader(ShaderType type, string shader_source)
{
  GLuint shaderID;
  char const* sourcePointer;
  GLint status;
  
  switch(type)
  {
    case VERTEX: 
      shaderID = glCreateShader(GL_VERTEX_SHADER);
      break;
    case FRAGMENT:
      shaderID = glCreateShader(GL_FRAGMENT_SHADER);
      break;
    default:
      cout<<"Shadertype not supported";
      break;
  }	 
  
  sourcePointer = shader_source.c_str();
  
  glShaderSource(shaderID, 1, &sourcePointer, NULL);
  glCompileShader(shaderID);
	
	
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    char buffer[512];
    glGetShaderInfoLog(shaderID, 512, NULL, buffer);
    cout << buffer << endl;
  }
  else cout << "Vertexshader successfully compiled!\n";
  
  return shaderID;
}


GLuint ShaderHandler::LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
	int infoLogLength;
	GLint result = GL_FALSE;
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShader);
	glAttachShader(programID, fragmentShader);
	glLinkProgram(programID);

	//Check program
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength>0)
	{
		std::vector<char> ProgramErrorMessage(infoLogLength+1);
		glGetProgramInfoLog(programID, infoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
		//std::cout<<"Fehler beim Linken aufgetreten\n"<<"Log-Laenge: "<<infoLogLength;
	}
	glDetachShader(programID, vertexShader);
	glDetachShader(programID, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return programID;
}

