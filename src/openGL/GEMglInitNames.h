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

#ifndef _INCLUDE__GEM_OPENGL_GEMGLINITNAMES_H_
#define _INCLUDE__GEM_OPENGL_GEMGLINITNAMES_H_

#include "Base/GemGLBase.h"

/*
 CLASS
        GEMglInitNames
 KEYWORDS
        openGL  0
 DESCRIPTION
        wrapper for the openGL-function
        "glInitNames( void)"
 */

class GEM_EXTERN GEMglInitNames : public GemGLBase
{
  CPPEXTERN_HEADER(GEMglInitNames, GemGLBase);

public:
  // Constructor
  GEMglInitNames (void);        // CON

protected:
  // Destructor
  virtual ~GEMglInitNames ();
  // check extensions
  virtual bool isRunnable(void);

  // Do the rendering
  virtual void  render (GemState *state);
};
#endif // for header file
