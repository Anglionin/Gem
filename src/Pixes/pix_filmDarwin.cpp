////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// tigital@mac.com
//
// Implementation file 
//
//    Copyright (c) 1997-1999 Mark Danks.
//    Copyright (c) 2002 James Tittle & Chris Clepper
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////
#ifdef __APPLE__

#include "pix_filmDarwin.h"
#include "OpenGL/glext.h"

static inline int powerOfTwo( int value )
{
    int x = 1;
    while ( x<value ) x<<= 1;
    return( x );
}

CPPEXTERN_NEW_WITH_ONE_ARG(pix_filmDarwin, t_symbol *, A_DEFSYM)

/////////////////////////////////////////////////////////
//
// pix_filmDarwin
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////

pix_filmDarwin :: pix_filmDarwin(t_symbol *filename) :
  pix_film(filename)
{
  // make sure that there are some characters
  if (filename->s_name[0]) openMess(filename);
  m_colorspace = GL_YCBCR_422_GEM;
  m_hiquality = 1;
  m_play = 0;
  m_rate = 1;
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_filmDarwin :: ~pix_filmDarwin()
{
    closeMess();
    deleteBuffer();

    outlet_free(m_outNumFrames);
    outlet_free(m_outEnd);
}

void pix_filmDarwin :: closeMess(void)
{
    switch (m_haveMovie) {
        case GEM_MOVIE_MOV:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_AVI:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_MPG:
            ::DisposeMovie(m_movie);
            ::DisposeGWorld(m_srcGWorld);
            m_srcGWorld = NULL;
            m_haveMovie = GEM_MOVIE_NONE;
            break;
        case GEM_MOVIE_NONE:
        default:
            break;
  }
}
/////////////////////////////////////////////////////////
// really open the file ! (OS dependent)
//
/////////////////////////////////////////////////////////
void pix_filmDarwin :: realOpen(char *filename)
{
    FSSpec		theFSSpec;
    OSErr		err = noErr;
    FSRef		ref;
    
    long		m_rowBytes;

    if (!filename[0]) {
        post("pix_filmDarwin:  no filename passed");
    } else {            
        err = ::FSPathMakeRef((UInt8*)filename, &ref, NULL);
        err = ::FSGetCatalogInfo(&ref, kFSCatInfoNone, NULL, NULL, &theFSSpec, NULL);
            
        if (err) {
            error("GEM: pix_movie: Unable to find file: %#s", theFSSpec.name);
            return;
        }
        m_haveMovie = GEM_MOVIE_MOV;
    }
    
    short	refnum = 0;
    err = ::OpenMovieFile(&theFSSpec, &refnum, fsRdPerm);
    if (err) {
        error("GEM: pix_movie: Couldn't open the movie file: %#s (%d)", theFSSpec.name, err);
        if (refnum) ::CloseMovieFile(refnum);
        return;
    }

	::NewMovieFromFile(&m_movie, refnum, NULL, NULL, newMovieActive, NULL);
	if (refnum) ::CloseMovieFile(refnum);

	m_reqFrame = 0;
	m_curFrame = -1;
        m_numTracks = (int)GetMovieTrackCount(m_movie);

	// Get the length of the movie
        
        movieDur = (long)GetMovieDuration(m_movie);
        movieScale = (long)GetMovieTimeScale(m_movie);
        
                                            
	OSType		whichMediaType = VisualMediaCharacteristic;
	short		flags = nextTimeMediaSample + nextTimeEdgeOK;
        /*
        GetMovieNextInterestingTime( m_movie, flags, (TimeValue)1, &whichMediaType, 0, 
             fixed1, NULL, &duration);
        m_numFrames = movieDur/duration;*/
        
        TimeValue	theTime = 0;
	
	m_numFrames = -1;
	while (theTime >= 0) {
		m_numFrames++;
		::GetMovieNextInterestingTime(m_movie,
                                            flags,
                                            1,
                                            &whichMediaType,
                                            theTime,
                                            0,
                                            &theTime,
                                            &duration);
		// after the first interesting time, don't include the time we
		//  are currently at.
		flags = nextTimeMediaSample;
	}

        durationf = (float)movieDur/(float)m_numFrames;

	// Get the bounds for the movie
	::GetMovieBox(m_movie, &m_srcRect);
        OffsetRect(&m_srcRect,  -m_srcRect.left,  -m_srcRect.top);
	SetMovieBox(m_movie, &m_srcRect);	
	m_xsize = m_srcRect.right - m_srcRect.left;
	m_ysize = m_srcRect.bottom - m_srcRect.top;
       
        if (m_colorspace == GL_BGRA_EXT){
			m_csize = 4;
            m_format = GL_BGRA_EXT;
            m_pixBlock.image.type = GL_UNSIGNED_INT_8_8_8_8_REV;
            
            createBuffer();
            prepareTexture();
            m_rowBytes = m_xsize * 4;
            if (m_hiquality) SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
            err = QTNewGWorldFromPtr(	&m_srcGWorld, 
                                            k32ARGBPixelFormat,	// gives noErr
                                            &m_srcRect, 
                                            NULL, 
                                            NULL, 
                                            0, 
                                            m_pixBlock.image.data, 
                                            m_rowBytes);
                                        
        }else{
            m_csize = 2;
            m_format = GL_YCBCR_422_APPLE;
            m_pixBlock.image.type = GL_UNSIGNED_SHORT_8_8_REV_APPLE;
            
            createBuffer();
           // prepareTexture();
            m_rowBytes = m_xsize * 2;
            if (m_hiquality) SetMoviePlayHints(m_movie, hintsHighQuality, hintsHighQuality);
            err = QTNewGWorldFromPtr(	&m_srcGWorld, 
                                            k422YpCbCr8CodecType,	// gives noErr
                                            &m_srcRect, 
                                            NULL, 
                                            NULL, 
                                            0, 
                                            m_pixBlock.image.data, 
                                            m_rowBytes);
        }
	if (err) {
		error("GEM: pix_filmDarwin: Couldn't make QTNewGWorldFromPtr %d", err);
		m_haveMovie = 0;
		return;
	}
        
        
     
        /* movies task method */
        m_movieTime = GetMovieTime(m_movie,nil);
        playRate = GetMoviePreferredRate(m_movie);
    
	// *** set the graphics world for displaying the movie ***
	::SetMovieGWorld(m_movie, m_srcGWorld, GetGWorldDevice(m_srcGWorld));
        
        if (m_auto) { 
            SetMovieRate(m_movie,playRate);
            m_play = 1;
        }
        else {
            SetMovieRate(m_movie,X2Fix(0.0));
        }
        
	::MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld ***
        
}

/////////////////////////////////////////////////////////
// render
//
/////////////////////////////////////////////////////////
void pix_filmDarwin :: getFrame()
{
    if (!m_haveMovie) return;

    
    if (m_auto){
        //play the startmovie() way
        if (!m_play) SetMovieRate(m_movie,X2Fix(m_rate));
        m_play = 1;
        
        if (m_rate > 0) {
            if (IsMovieDone(m_movie)) GoToBeginningOfMovie(m_movie);
        
            }
            else {
                if (GetMovieTime(m_movie,nil) <= 0) {
                    GoToEndOfMovie(m_movie);
                }
            }
            
        MoviesTask(m_movie, 0);	// *** this does the actual drawing into the GWorld *** 
        
    }
    else
    {
    //play the manual way
        if (m_play) {
            SetMovieRate(m_movie,X2Fix(0.0));
            m_play = 0; //turn off play
            return;
            }
        
        //m_movieTime = m_reqFrame * duration;
        m_movieTime = (long)((float)m_reqFrame * durationf);
        
        SetMovieTimeValue(m_movie, m_movieTime);
        MoviesTask(m_movie, 0);
        
    }

}

void pix_filmDarwin :: LoadRam()
{
    TimeValue	length;
    OSErr err;
    if (m_haveMovie){      
        m_movieTime = 0;
        length = GetMovieDuration(m_movie);
        err =LoadMovieIntoRam(m_movie,m_movieTime,length,keepInRam);
        if (err)
        {
            post("pix_film: LoadMovieIntoRam failed miserably");
        }
    }else{
        post("pix_film: no movie to load into RAM!");
    }
}

void pix_filmDarwin :: MovRate(float rate)
{
    m_rate = (float)rate;
    if (m_auto) {
    SetMovieRate(m_movie,X2Fix((double)m_rate));
    }
}

void pix_filmDarwin :: doDebug()
{
    post("---------- pix_filmDarwin doDebug start----------");
    post("GEM: pix_filmDarwin:  m_numTracks = %d",m_numTracks);
    post("Movie duration = %d timescale = %d timebase = %d", movieDur, movieScale, (long)GetMovieTimeBase(m_movie));
    post("pix_filmDarwin: rect rt:%d lt:%d", m_srcRect.right, m_srcRect.left);
    post("pix_filmDarwin: rect top:%d bottom:%d", m_srcRect.top, m_srcRect.bottom);
    post("pix_filmDarwin: movie size x:%d y:%d", m_xsize, m_ysize);
    if (m_colorspace == GL_BGRA_EXT) post("pix_filmDarwin: color space ARGB");
        else  post("pix_filmDarwin: color space YUV");
    post("pix_filmDarwin: Preferred rate fixed: %d int: %d float %f", playRate, Fix2Long(playRate),(float) Fix2X(playRate));    
        
    post("---------- pix_filmDarwin doDebug end----------");
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_filmDarwin :: obj_setupCallback(t_class *classPtr)
{
  class_addcreator((t_newmethod)_classpix_filmDarwin, gensym("pix_film"), A_DEFSYM, A_NULL);
  pix_film::real_obj_setupCallback(classPtr);

  class_addmethod(classPtr, (t_method)&pix_filmDarwin::openMessCallback,
		  gensym("open"), A_SYMBOL, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::changeImageCallback,
		  gensym("img_num"), A_GIMME, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::autoCallback,
		  gensym("auto"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::ramCallback,
		  gensym("ram"),  A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::hiqualityCallback,
		  gensym("hiquality"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::rateCallback,
		  gensym("rate"), A_DEFFLOAT, A_NULL);
  class_addmethod(classPtr, (t_method)&pix_filmDarwin::debugCallback,
		  gensym("debug"),  A_NULL);              

}

void pix_filmDarwin :: openMessCallback(void *data, t_symbol *filename)
{
  GetMyClass(data)->openMess(filename);
}

void pix_filmDarwin :: changeImageCallback(void *data, t_symbol *, int argc, t_atom *argv)
{
    GetMyClass(data)->changeImage((argc<1)?0:atom_getint(argv), (argc<2)?0:atom_getint(argv+1));
}

void pix_filmDarwin :: autoCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_auto=!(!(int)state);
}

void pix_filmDarwin :: ramCallback(void *data)
{
  GetMyClass(data)->LoadRam();
}

void pix_filmDarwin :: hiqualityCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->m_hiquality=(int)state;
}

void pix_filmDarwin :: rateCallback(void *data, t_floatarg state)
{
  GetMyClass(data)->MovRate((float)state);
}

void pix_filmDarwin :: debugCallback(void *data)
{
  GetMyClass(data)->doDebug();
}
#endif // __APPLE__
