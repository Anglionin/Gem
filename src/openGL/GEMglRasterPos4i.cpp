////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// Implementation file
//
// Copyright (c) 2002-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//      zmoelnig@iem.kug.ac.at
//  For information on usage and redistribution, and for a DISCLAIMER
//  *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
//
//  this file has been generated...
////////////////////////////////////////////////////////

#include "GEMglRasterPos4i.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglRasterPos4i, t_floatarg, A_DEFFLOAT,
                               t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRasterPos4i :: GEMglRasterPos4i    (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2, t_floatarg arg3) :
  x(static_cast<GLint>(arg0)),
  y(static_cast<GLint>(arg1)),
  z(static_cast<GLint>(arg2)),
  w(static_cast<GLint>(arg3))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("x"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("y"));
  m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("z"));
  m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("w"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRasterPos4i :: ~GEMglRasterPos4i ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
  inlet_free(m_inlet[2]);
  inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRasterPos4i :: render(GemState *state)
{
  glRasterPos4i (x, y, z, w);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglRasterPos4i :: xMess (t_float arg1)   // FUN
{
  x = static_cast<GLint>(arg1);
  setModified();
}

void GEMglRasterPos4i :: yMess (t_float arg1)   // FUN
{
  y = static_cast<GLint>(arg1);
  setModified();
}

void GEMglRasterPos4i :: zMess (t_float arg1)   // FUN
{
  z = static_cast<GLint>(arg1);
  setModified();
}

void GEMglRasterPos4i :: wMess (t_float arg1)   // FUN
{
  w = static_cast<GLint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRasterPos4i :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos4i::xMessCallback),
                  gensym("x"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos4i::yMessCallback),
                  gensym("y"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos4i::zMessCallback),
                  gensym("z"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRasterPos4i::wMessCallback),
                  gensym("w"), A_DEFFLOAT, A_NULL);
};

void GEMglRasterPos4i :: xMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->xMess ( static_cast<t_float>(arg0));
}
void GEMglRasterPos4i :: yMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->yMess ( static_cast<t_float>(arg0));
}
void GEMglRasterPos4i :: zMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->zMess ( static_cast<t_float>(arg0));
}
void GEMglRasterPos4i :: wMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->wMess ( static_cast<t_float>(arg0));
}
