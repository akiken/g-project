//*******************
// ARToolKitÉâÉbÉpÅ[
//*******************

#ifndef		_ARTK_H_
#define		_ARTK_H_

#include	<AR/config.h>
#include	<AR/video.h>
#include	<AR/param.h>
#include	<AR/ar.h>
#include	<AR/gsub_lite.h>

#include	"../MMD/VecMatQuat.h"

#define ARLOGe printf
#define ARLOGi printf
#define ARLOGw printf
#define m_sCameraParam (gCparamLT->param)

class cARTK
{
	private :
		ARParamLT*		gCparamLT;
		ARHandle *gARHandle;
		ARPattHandle *gARPattHandle;
		AR3DHandle *gAR3DHandle;
		int				m_iPattID;

		ARGL_CONTEXT_SETTINGS_REF
						m_pArglSettings;

		double			m_dmatRotX[16];
		Matrix			m_matRotX;

		ARUint8			*m_pARTImage;
		bool			m_bMarkerFound;

		ARdouble			m_dPattWidth;
		ARdouble			m_dPattCenter[2];
		ARdouble			m_dPattTransMat[3][4];

		double			m_dViewScaleFactor;

		bool			m_bFirstTime;

	public :
		cARTK( void );
		~cARTK( void );

		bool initialize( void );
		bool initArgl( void );
		int setupCamera(const char *cparam_name, char *vconf, ARParamLT **cparamLT_p, ARHandle **arhandle, AR3DHandle **ar3dhandle);

		bool update( void );
		void display( void );

		void getCameraPos( Vector3 *pvec3CamPos );

		void getProjectionMat( double dMat[16] );
		void getModelViewMat( double dMat[16] );
		void addViewScaleFactor( double dAdd );

		void cleanup( void );


		inline bool isMarkerFound( void ){return m_bMarkerFound; }
};

extern cARTK	g_clARTK;

#endif	// _ARTK_H_
