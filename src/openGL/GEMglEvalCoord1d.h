/* ------------------------------------------------------------------
 * GEM - Graphics Environment for Multimedia
 *
 *  Copyright (c) 2002-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
 *     zmoelnig@iem.kug.ac.at
 *  For information on usage and redistribution, and for a DISCLAIMER
 *  OF ALL WARRANTIES, see the file, "GEM.LICENSE.TERMS"
 *
 *  this file has been generated...
 * ------------------------------------------------------------------
 */

#ifndef _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD_D_H_
#define _INCLUDE__GEM_OPENGL_GEMGLEVALCOORD_D_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglEvalCoord1d
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glEvalCoord1d( GLdouble u)"
 */

class GEM_EXTERN GEMglEvalCoord1d : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglEvalCoord1d, GemGLBase);

public:
  // Constructor
  GEMglEvalCoord1d (t_float);   // CON

protected:
  // Destructor
  virtual ~GEMglEvalCoord1d ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);

  // variables
  GLdouble      u;              // VAR
  virtual void  uMess(t_float); // FUN


private:

  // we need some inlets
  t_inlet *m_inlet[1];

  // static member functions
  static void    uMessCallback (void*, t_float);
};
#endif // for header file
