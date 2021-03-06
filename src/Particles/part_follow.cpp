////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////

#include "part_follow.h"

#include "papi/papi.h"

CPPEXTERN_NEW_WITH_ONE_ARG(part_follow, t_floatarg, A_DEFFLOAT);

/////////////////////////////////////////////////////////
//
// part_follow
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
part_follow :: part_follow(t_floatarg num)
  : m_accel(1.f)
{
  if (num != 0) {
    m_accel = num;
  }
  inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"),
            gensym("accel"));
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
part_follow :: ~part_follow(void)
{ }

/////////////////////////////////////////////////////////
// renderParticles
//
/////////////////////////////////////////////////////////
void part_follow :: renderParticles(GemState *state)
{
  if (m_tickTime > 0.f) {
    pFollow(m_accel);
  }
}

/////////////////////////////////////////////////////////
// static member functions
//
/////////////////////////////////////////////////////////
void part_follow :: obj_setupCallback(t_class *classPtr)
{
  CPPEXTERN_MSG1(classPtr, "accel", accelMess, float);
}

void part_follow :: accelMess(float accel)
{
  m_accel=accel;
}
