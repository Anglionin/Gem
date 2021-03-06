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

#include "GEMglRecti.h"

CPPEXTERN_NEW_WITH_FOUR_ARGS ( GEMglRecti, t_floatarg, A_DEFFLOAT,
                               t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// GEMglViewport
//
/////////////////////////////////////////////////////////
// Constructor
//
GEMglRecti :: GEMglRecti        (t_floatarg arg0, t_floatarg arg1,
                                 t_floatarg arg2, t_floatarg arg3) :
  x1(static_cast<GLint>(arg0)),
  y1(static_cast<GLint>(arg1)),
  x2(static_cast<GLint>(arg2)),
  y2(static_cast<GLint>(arg3))
{
  m_inlet[0] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("x1"));
  m_inlet[1] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("y1"));
  m_inlet[2] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("x2"));
  m_inlet[3] = inlet_new(this->x_obj, &this->x_obj->ob_pd, &s_float,
                         gensym("y2"));
}
/////////////////////////////////////////////////////////
// Destructor
//
GEMglRecti :: ~GEMglRecti ()
{
  inlet_free(m_inlet[0]);
  inlet_free(m_inlet[1]);
  inlet_free(m_inlet[2]);
  inlet_free(m_inlet[3]);
}

/////////////////////////////////////////////////////////
// Render
//
void GEMglRecti :: render(GemState *state)
{
  glRecti (x1, y1, x2, y2);
}

/////////////////////////////////////////////////////////
// Variables
//
void GEMglRecti :: x1Mess (t_float arg1)        // FUN
{
  x1 = static_cast<GLint>(arg1);
  setModified();
}

void GEMglRecti :: y1Mess (t_float arg1)        // FUN
{
  y1 = static_cast<GLint>(arg1);
  setModified();
}

void GEMglRecti :: x2Mess (t_float arg1)        // FUN
{
  x2 = static_cast<GLint>(arg1);
  setModified();
}

void GEMglRecti :: y2Mess (t_float arg1)        // FUN
{
  y2 = static_cast<GLint>(arg1);
  setModified();
}


/////////////////////////////////////////////////////////
// static member functions
//

void GEMglRecti :: obj_setupCallback(t_class *classPtr)
{
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRecti::x1MessCallback),     gensym("x1"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRecti::y1MessCallback),     gensym("y1"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRecti::x2MessCallback),     gensym("x2"),
                  A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr,
                  reinterpret_cast<t_method>(&GEMglRecti::y2MessCallback),     gensym("y2"),
                  A_DEFFLOAT, A_NULL);
};

void GEMglRecti :: x1MessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->x1Mess ( static_cast<t_float>(arg0));
}
void GEMglRecti :: y1MessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->y1Mess ( static_cast<t_float>(arg0));
}
void GEMglRecti :: x2MessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->x2Mess ( static_cast<t_float>(arg0));
}
void GEMglRecti :: y2MessCallback (void* data, t_float arg0)
{
  GetMyClass(data)->y2Mess ( static_cast<t_float>(arg0));
}
