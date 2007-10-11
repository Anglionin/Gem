////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Created by tigital on 11/13/2005.
// Copyright 2005 James Tittle
//
// Implementation file
//
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "glsl_program.h"

#include <string.h>
#include <stdlib.h>

CPPEXTERN_NEW(glsl_program)

/////////////////////////////////////////////////////////
//
// glsl_program
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
glsl_program :: glsl_program()
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  :
m_program(0), 
m_maxLength(0), m_infoLength(0), m_uniformCount(0),
m_symname(NULL), m_size(NULL), m_type(NULL), m_loc(NULL),
m_param(NULL), m_flag(NULL), m_linked(0), m_wantLink(false),
m_infoLog(NULL), m_num(0)
#endif
{
#if !defined GL_ARB_shader_objects && !defined GL_VERSION_2_0
  post("GEM has been compiled without GLSL support");
  return;
#endif
  // create an outlet to send texture ID
  m_outProgramID = outlet_new(this->x_obj, &s_float);
}

////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
glsl_program :: ~glsl_program()
{
#ifdef GL_VERSION_2_0
  if (m_program)glDeleteProgram( m_program ); m_program=0;
  destroyArrays();
#elif defined GL_ARB_shader_objects
  if (m_program)glDeleteObjectARB( m_program ); m_program=0;
  destroyArrays();
#endif
}

void glsl_program :: destroyArrays() {
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  int i;

  if (m_param)
    {
      for (i = 0; i < m_uniformCount; i++){
        if(m_param[i])delete[]m_param[i];
        m_param[i]=NULL;
      }
    }

  if (m_size)   delete[]m_size;   m_size   =NULL;
  if (m_type)   delete[]m_type;   m_type   =NULL;
  if (m_symname)delete[]m_symname;m_symname=NULL;
  if (m_flag)   delete[]m_flag;   m_flag   =NULL;
  if (m_loc)    delete[]m_loc;    m_loc    =NULL;
  if (m_param)  delete[]m_param;  m_param  =NULL;
#endif
}
void glsl_program :: createArrays() {
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  int i;

  m_size   = new GLint     [m_uniformCount];
  m_type   = new GLenum    [m_uniformCount];
  m_symname= new t_symbol* [m_uniformCount];
  m_param  = new float*    [m_uniformCount];
  m_flag   = new int       [m_uniformCount];
  m_loc    = new GLint     [m_uniformCount];

  // allocate maximum size for a param, which is a 4x4 matrix of floats
  // in the future, only allocate for specific type
  // also, technically we should handle arrays of matrices, too...sheesh!
  for (i = 0; i < m_uniformCount; i++) {
    int j=0;
    m_size   [i] = 0;
    m_type   [i] = 0;
    m_symname[i] = 0;
	m_loc    [i] = 0;
    m_param  [i] = new float[16];
    m_flag   [i] = 0;
    for(j=0; j<16; j++)m_param[i][j]=0;
  }
#endif
}
/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void glsl_program :: render(GemState *state)
{
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  if(m_wantLink){
    m_wantLink=0;
    LinkProgram();
  }
#ifdef GL_VERSION_2_0
  if (m_linked) {
    glUseProgram( m_program );
    for(int i=0; i<m_uniformCount; i++)
      {
        if(m_flag[i])
	  {
	    switch (m_type[i])
              {
              case GL_INT:
                glUniform1i( m_loc[i], (GLint)m_param[i][0] );
		break;
              case GL_FLOAT:
                glUniform1f( m_loc[i], (GLfloat)m_param[i][0] );
		break;
              case GL_FLOAT_VEC2:
                glUniform2f( m_loc[i], (GLfloat)m_param[i][0], (GLfloat)m_param[i][1] );
		break;
              case GL_FLOAT_VEC3:
                glUniform3f( m_loc[i], (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
                                (GLfloat)m_param[i][2] );
		break;
              case GL_FLOAT_VEC4:
                glUniform4f( m_loc[i], (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
                                (GLfloat)m_param[i][2], (GLfloat)m_param[i][3] );
		break;
              case GL_INT_VEC2:
                glUniform2i( m_loc[i], (GLint)m_param[i][0], (GLint)m_param[i][1] );
		break;
              case GL_INT_VEC3:
                glUniform3i( m_loc[i], (GLint)m_param[i][0], (GLint)m_param[i][1], 
                                (GLint)m_param[i][2] );
		break;
              case GL_INT_VEC4:
                glUniform4i( m_loc[i], (GLint)m_param[i][0], (GLint)m_param[i][1], 
                                (GLint)m_param[i][2], (GLint)m_param[i][3] );
		break;
              case GL_FLOAT_MAT2:
                // GL_TRUE = row major order, GL_FALSE = column major
                glUniformMatrix2fv( m_loc[i], 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
              case GL_FLOAT_MAT3:
                glUniformMatrix3fv( m_loc[i], 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
              case GL_FLOAT_MAT4:
                glUniformMatrix4fv( m_loc[i], 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
			case GL_SAMPLER_2D_RECT_ARB:
				//post("sampler %s param %d",m_symname[i]->s_name, (GLint)m_param[i][0]);
				//glUniform1i(glGetUniformLocation(m_program, (GLchar*)m_symname[i]->s_name), (GLint)m_param[i][0]);
				glUniform1i(m_loc[i], (GLint)m_param[i][0]);
		break;
		case GL_SAMPLER_2D_ARB:
				glUniform1i(m_loc[i], m_param[i][0]);
		break;
              default:
		;
              }
            // remove flag because the value is in GL's state now...
            m_flag[i]=0;
			
	  }
      }
	 // glUniform1i(glGetUniformLocation(m_program, "MyTex1"), 1);
  } else {
    /* JMZ: this is really annoying... */
    //error("no program linked");
  }
#elif defined GL_ARB_shader_objects
  if (m_linked) {
    glUseProgramObjectARB( m_program );
    for(int i=0; i<m_uniformCount; i++)
      {
        if(m_flag[i])
	  {
	    switch (m_type[i])
              {
              case GL_INT:
                glUniform1iARB( m_loc[i], (GLint)m_param[i][0] );
		break;
              case GL_FLOAT:
                glUniform1fARB( m_loc[i], (GLfloat)m_param[i][0] );
		break;
              case GL_FLOAT_VEC2_ARB:
                glUniform2fARB( m_loc[i], (GLfloat)m_param[i][0], (GLfloat)m_param[i][1] );
		break;
              case GL_FLOAT_VEC3_ARB:
                glUniform3fARB( m_loc[i], (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
                                (GLfloat)m_param[i][2] );
		break;
              case GL_FLOAT_VEC4_ARB:
                glUniform4fARB( m_loc[i], (GLfloat)m_param[i][0], (GLfloat)m_param[i][1], 
                                (GLfloat)m_param[i][2], (GLfloat)m_param[i][3] );
		break;
              case GL_INT_VEC2_ARB:
                glUniform2iARB( m_loc[i], (GLint)m_param[i][0], (GLint)m_param[i][1] );
		break;
              case GL_INT_VEC3_ARB:
                glUniform3iARB( m_loc[i], (GLint)m_param[i][0], (GLint)m_param[i][1], 
                                (GLint)m_param[i][2] );
		break;
              case GL_INT_VEC4_ARB:
                glUniform4iARB( m_loc[i], (GLint)m_param[i][0], (GLint)m_param[i][1], 
                                (GLint)m_param[i][2], (GLint)m_param[i][3] );
		break;
              case GL_FLOAT_MAT2_ARB:
                // GL_TRUE = row major order, GL_FALSE = column major
                glUniformMatrix2fvARB( m_loc[i], 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
              case GL_FLOAT_MAT3_ARB:
                glUniformMatrix3fvARB( m_loc[i], 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
              case GL_FLOAT_MAT4_ARB:
                glUniformMatrix4fvARB( m_loc[i], 1, GL_FALSE, (GLfloat*)&m_param[i] );
		break;
			case GL_SAMPLER_2D_RECT_ARB:
				glUniform1iARB(m_loc[i], m_param[i][0]);
		break;
			case GL_SAMPLER_2D_ARB:
				glUniform1iARB(m_loc[i], m_param[i][0]);
		break;
		
              default:
		;
              }
            // remove flag because the value is in GL's state now...
            m_flag[i]=0;
			
	  }
      }
	//  glUniform1iARB(glGetUniformLocationARB(program_object, "MyTex1"), 1);
  } else {
    /* JMZ: this is really annoying... */
    //error("no program linked");
  }
#endif
  // send program ID to outlet
  /* JMZ: shouldn't this be only done, when we have a linked program? */
  outlet_float(m_outProgramID, (t_float)(int)m_program);
#endif
}

/////////////////////////////////////////////////////////
// postrender
//
/////////////////////////////////////////////////////////
void glsl_program :: postrender(GemState *state)
{
#ifdef GL_VERSION_2_0
  if(m_linked)
    glUseProgram(0);
#elif defined GL_ARB_shader_objects
  if(m_linked)
    glUseProgramObjectARB(0);
#endif
}
/////////////////////////////////////////////////////////
// shaderMess
//
/////////////////////////////////////////////////////////
void glsl_program :: paramMess(t_symbol*s,int argc, t_atom *argv)
{
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  int i=0, j=0;
  if (m_program){
    for(i=0; i<m_uniformCount; i++){
      if(s==m_symname[i]){
        //      post("uniform parameters #%d", i);
        // don't know what to do with that...
        // sketch:
        //   copy the values into memory and add a flag that we have them for this parameter
        //   in the render cycle use it
        for (j=0; j < argc; j++)
	  {
	    m_param[i][j] = atom_getfloat(&argv[j]);
	  }
        // tell the GL state that this variable has changed next render
        m_flag[i] = 1;
        setModified();
        // should we return here?  It only allows one m_param[i] to be changed
        return;
      }
    }
    // if we reach this, then no param-name was matching!
    if(i>m_num)error("glsl_program: no method for '%s' (it's not uniform variable)", s->s_name);
  }
#endif
}

/////////////////////////////////////////////////////////
// shaderMess
//
/////////////////////////////////////////////////////////
void glsl_program :: shaderMess(int argc, t_atom *argv)
{
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  int i;

  if (!argc)
    {
      error("can't link non-existent shaders");
      return;
    }

  if(argc>MAX_NUM_SHADERS)
    {
      argc=MAX_NUM_SHADERS;
      post("only %d shaders supported; skipping the rest", MAX_NUM_SHADERS);
    }
  for (i = 0; i < argc; i++)
    {
#ifdef GL_VERSION_2_0
	  m_shaderObj[i] = (GLuint)(atom_getint(&argv[i]));
#else
      m_shaderObj[i] = (t_GLshaderObj)(atom_getint(&argv[i]));
#endif
    }
  
  //  not sure what to do here:  we don't want to link & re-link every render cycle,
  //  but we do want to link when there are new shaders to link...so I made a seperate
  //  link message
  m_num = argc;
#endif
}

/////////////////////////////////////////////////////////
// LinkProgram
//
/////////////////////////////////////////////////////////
void glsl_program :: LinkProgram()
{
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  int i;
  GLsizei length=0;
  if (!m_num)
    {
      error("can't link zero shaders");
      return;
    }
#ifdef GL_VERSION_2_0
  if(m_program) {
    glDeleteProgram( m_program );
    m_program = 0;
  }
  m_program = glCreateProgram();
  for (i = 0; i < m_num; i++)
    {
      glAttachShader( m_program, m_shaderObj[i] );
    }
  glLinkProgram( m_program );
  glGetProgramiv( m_program, GL_LINK_STATUS, &m_linked );

  glGetProgramiv( m_program, GL_INFO_LOG_LENGTH, &m_infoLength );
  if(m_infoLog)delete[]m_infoLog;m_infoLog=NULL;
  m_infoLog = new GLchar[m_infoLength];

  glGetProgramInfoLog( m_program, m_infoLength, &length, m_infoLog );
#elif defined GL_ARB_shader_objects
  if(m_program) {
    glDeleteObjectARB( m_program );
    m_program = 0;
  }
  m_program = glCreateProgramObjectARB();
  for (i = 0; i < m_num; i++)
    {
      glAttachObjectARB( m_program, m_shaderObj[i] );
    }
  glLinkProgramARB( m_program );
  glGetObjectParameterivARB( m_program, GL_OBJECT_LINK_STATUS_ARB, &m_linked );

  glGetObjectParameterivARB( m_program, GL_OBJECT_INFO_LOG_LENGTH_ARB, &m_infoLength );
  if(m_infoLog)delete[]m_infoLog;m_infoLog=NULL;
  m_infoLog = new GLcharARB[m_infoLength];

  glGetInfoLogARB( m_program, m_infoLength, &length, m_infoLog );
#endif
  if (length)
    {
      post("Info_log:");
      post("%s", m_infoLog);
    }
  //post("freeing log");
  if(m_infoLog)delete[]m_infoLog;m_infoLog=NULL;
  
  //
  // If all went well, make the ProgramObject part of the current state
  //
  //post("did we link?");
#ifdef GL_VERSION_2_0
  if (m_linked) {
    glUseProgram( m_program );
  } else {
    glUseProgram( 0 );
    post("Link failed!");
    return;
  }
#elif defined GL_ARB_shader_objects
  if (m_linked) {
    glUseProgramObjectARB( m_program );
  } else {
    glUseProgramObjectARB( 0 );
    post("Link failed!");
    return;
  }
#endif
  //post("getting variables");
  getVariables();
# ifdef __APPLE__
  // call API to check if linked program is running on hardware or in software emulation
  GLint vertexGPUProcessing, fragmentGPUProcessing;
  CGLGetParameter (CGLGetCurrentContext(), kCGLCPGPUVertexProcessing, &vertexGPUProcessing);
  CGLGetParameter (CGLGetCurrentContext(), kCGLCPGPUFragmentProcessing, &fragmentGPUProcessing);
  
  post("vertex shader running in %sware", vertexGPUProcessing?"hard":"soft");
  post("fragment shader running in %sware", fragmentGPUProcessing?"hard":"soft");
# endif //__APPLE__
#endif
}

/////////////////////////////////////////////////////////
// getVariables
//
/////////////////////////////////////////////////////////
void glsl_program :: getVariables()
{
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  if(!m_linked)return;
  int i;
  //
  // Allocate arrays to store the answers in. For simplicity, the return
  // from malloc is not checked for NULL.
  //
  destroyArrays();
  //
  // Get the number of uniforms, and the length of the longest name.
  //
#ifdef GL_VERSION_2_0
  glGetProgramiv( m_program,
					GL_ACTIVE_UNIFORM_MAX_LENGTH,
					&m_maxLength);
  glGetProgramiv( m_program, GL_ACTIVE_UNIFORMS,
					&m_uniformCount);
#elif defined GL_ARB_shader_objects
  glGetObjectParameterivARB( m_program,
                             GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB,
                             &m_maxLength);
  glGetObjectParameterivARB( m_program, GL_OBJECT_ACTIVE_UNIFORMS_ARB,
                             &m_uniformCount);
#endif
  createArrays();
  
  //
  // Loop over the ActiveUniform's and store the results away.
  //
#ifdef GL_VERSION_2_0
  GLchar *name=new GLchar[m_maxLength];
#else
  GLcharARB *name=new GLcharARB[m_maxLength];
#endif
  GLsizei    length=0;
  for (i = 0; i < m_uniformCount; i++) 
    {
      /*
        glGetActiveUniformARB(m_program, i, m_maxLength, &length,
        &m_size[i], &m_type[i], name);
      */
#ifdef GL_VERSION_2_0
      glGetActiveUniform(m_program, i, m_maxLength, &length, &m_size[i], &m_type[i], name);
	  m_loc[i] = glGetUniformLocation( m_program, name );
#elif defined GL_ARB_shader_objects
      glGetActiveUniformARB(m_program, i, m_maxLength, &length, &m_size[i], &m_type[i], name);
	  m_loc[i] = glGetUniformLocationARB( m_program, name );
#endif
      m_symname[i]=gensym(name);
      //	  post("active uniform variable: %s", name);
    }
  delete[]name;
#endif
}

/////////////////////////////////////////////////////////
// printInfo
//
/////////////////////////////////////////////////////////
void glsl_program :: printInfo()
{
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  int i;

  if(!m_linked) 
    {
      error("no GLSL-program linked");
      return;
    }

  GLint bitnum = 0;
  post("glsl_Program Hardware Info");
  post("============================");
	
  post("");
  for (i=0; i<m_uniformCount; i++)
    { 
      startpost("[%s]: uvar#%d: \"%s\": ",  
                m_objectname->s_name, i, m_symname[i]->s_name);
      switch (m_type[i])
        {
        case 0x1404:
          post("GL_INT");
          break;
        case 0x1406:
          post("GL_FLOAT");
          break;
        case 0x8B50:
          post("GL_FLOAT_VEC2_ARB");
          break;
        case 0x8B51:
          post("GL_FLOAT_VEC3_ARB");
          break;
        case 0x8B52:
          post("GL_FLOAT_VEC4_ARB");
          break;
        case 0x8B53:
          post("GL_INT_VEC2_ARB");
          break;
        case 0x8B54:
          post("GL_INT_VEC3_ARB");
          break;
        case 0x8B55:
          post("GL_INT_VEC4_ARB");
          break;
        case 0x8B56:
          post("GL_BOOL_ARB");
          break;
        case 0x8B57:
          post("GL_BOOL_VEC2_ARB");
          break;
        case 0x8B58:
          post("GL_BOOL_VEC3_ARB");
          break;
        case 0x8B59:
          post("GL_BOOL_VEC4_ARB");
          break;
        case 0x8B5A:
          post("GL_FLOAT_MAT2_ARB");
          break;
        case 0x8B5B:
          post("GL_FLOAT_MAT3_ARB");
          break;
        case 0x8B5C:
          post("GL_FLOAT_MAT4_ARB");
          break;
		case 0x8B63:
          post("GL_SAMPLER_2D_RECT_ARB");
          break;
        default:
          post("unknown (0x%X)", m_type[i]);
          break;
        }
    }
#endif
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void glsl_program :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr, (t_method)&glsl_program::shaderMessCallback,
                  gensym("shader"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&glsl_program::linkCallback,
                  gensym("link"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&glsl_program::printMessCallback,
                  gensym("print"), A_NULL);
  class_addanything(classPtr, (t_method)&glsl_program::paramMessCallback);
}
void glsl_program :: shaderMessCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
  GetMyClass(data)->shaderMess(argc, argv);
}
void glsl_program :: linkCallback(void *data, t_symbol*, int argc, t_atom*argv)
{
  if(argc)
    GetMyClass(data)->shaderMess(argc, argv);
#if defined GL_VERSION_2_0 || defined GL_ARB_shader_objects
  GetMyClass(data)->m_wantLink=1;
#endif
}
void glsl_program :: printMessCallback(void *data)
{
  GetMyClass(data)->printInfo();
}
void glsl_program :: paramMessCallback(void *data, t_symbol *s, int argc, t_atom *argv)
{
  GetMyClass(data)->paramMess(s, argc, argv);
}
