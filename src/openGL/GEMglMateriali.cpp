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

#include "GEMglMateriali.h"

CPPEXTERN_NEW_WITH_THREE_ARGS ( GEMglMateriali, t_floatarg, A_DEFFLOAT,
                                t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglMateriali :: GEMglMateriali        (t_floatarg arg0, t_floatarg arg1,
    t_floatarg arg2) :
  face(static_cast<GLenum>(arg0)),
  pname(static_cast<GLenum>(arg1)),
  param(static_cast<GLint>(arg2))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("face"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("pname"));
  m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("param"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglMateriali :: ~GEMglMateriali ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
  inlet_free(m_inlet[2]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglMateriali :: render(GemState *state)
{
  glMateriali (face, pname, param);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglMateriali :: faceMess (t_float arg1)          // FUN
{
  face = static_cast<GLenum>(arg1);
  setModified();
}

void GEMglMateriali :: pnameMess (t_float arg1)         // FUN
{
  pname = static_cast<GLenum>(arg1);
  setModified();
}

void GEMglMateriali :: paramMess (t_float arg1)         // FUN
{
  param = static_cast<GLint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglMateriali :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglMateriali::faceMessCallback),
                  gensym("face"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglMateriali::pnameMessCallback),
                  gensym("pname"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglMateriali::paramMessCallback),
                  gensym("param"), A_DEFFLOAT, A_NULL);
};

void GEMglMateriali :: faceMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->faceMess ( static_cast<t_float>(arg0));
}
void GEMglMateriali :: pnameMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->pnameMess ( static_cast<t_float>(arg0));
}
void GEMglMateriali :: paramMessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->paramMess ( static_cast<t_float>(arg0));
}
