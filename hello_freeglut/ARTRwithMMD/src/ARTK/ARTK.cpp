//*******************
// ARToolKitラッパー
//*******************

#include	<math.h>
#include	"ARTK.h"
#include	<string.h>
#include	"../ErrorMes.h"
#include <GL/glut.h>

cARTK		g_clARTK;
static int windowWidth = 640;					// Initial window width, also updated during program execution.
static int windowHeight = 480;                  // Initial window height, also updated during program execution.
ARGL_CONTEXT_SETTINGS_REF gArglSettings = NULL;

//================
// コンストラクタ
//================
cARTK::cARTK( void ) : m_pARTImage( NULL ), m_pArglSettings( NULL ), m_bMarkerFound( false )
{
}

//==============
// デストラクタ
//==============
cARTK::~cARTK( void )
{
	cleanup();
}

//========
// 初期化
//========
bool cARTK::initialize( void )
{
	const char	*szCameraParamFName = "Data/camera_para.dat";
	const char	*szPattFName  = "Data/hiro.patt";
	char		*szVConfFName = "";

	ARParam			sCamParamTemp;

	if (!setupCamera(szCameraParamFName, szVConfFName, &gCparamLT, &gARHandle, &gAR3DHandle))
	{
		exit(-1);
	}

	//
	// Graphics setup.
	//

	// Set up GL context(s) for OpenGL to draw into.
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(640, 480);
	glutCreateWindow("ARTK MMD");

	// Setup ARgsub_lite library for current OpenGL context.
	if ((gArglSettings = arglSetupForCurrentContext(&(gCparamLT->param), arVideoGetPixelFormat())) == NULL) {
		ARLOGe("main(): arglSetupForCurrentContext() returned error.\n");
		cleanup();
		exit(-1);
	}
	arglSetupDebugMode(gArglSettings, gARHandle);
	arUtilTimerReset();


	if ((gARPattHandle = arPattCreateHandle()) == NULL) {
		ARLOGe("setupMarker(): Error: arPattCreateHandle.\n");
		return (FALSE);
	}

	// マーカーパターンファイルの読込み
	if ((m_iPattID = arPattLoad(gARPattHandle, szPattFName)) < 0)
	{
		ErrorMessage("Pattern file load error !!\n");
		return false;
	}

	arPattAttach(gARHandle, gARPattHandle);

	m_dViewScaleFactor = 0.16;
	m_dPattWidth = 40.0;
	m_bFirstTime = true;

	return true;
}

int cARTK::setupCamera(const char *cparam_name, char *vconf, ARParamLT **cparamLT_p, ARHandle **arhandle, AR3DHandle **ar3dhandle)
{
	ARParam			cparam;
	int				xsize, ysize;
	AR_PIXEL_FORMAT pixFormat;

	// Open the video path.
	if (arVideoOpen(vconf) < 0) {
		ARLOGe("setupCamera(): Unable to open connection to camera.\n");
		return (FALSE);
	}

	// Find the size of the window.
	if (arVideoGetSize(&xsize, &ysize) < 0) {
		ARLOGe("setupCamera(): Unable to determine camera frame size.\n");
		arVideoClose();
		return (FALSE);
	}
	ARLOGi("Camera image size (x,y) = (%d,%d)\n", xsize, ysize);

	// Get the format in which the camera is returning pixels.
	pixFormat = arVideoGetPixelFormat();
	if (pixFormat == AR_PIXEL_FORMAT_INVALID) {
		ARLOGe("setupCamera(): Camera is using unsupported pixel format.\n");
		arVideoClose();
		return (FALSE);
	}

	// Load the camera parameters, resize for the window and init.
	if (arParamLoad(cparam_name, 1, &cparam) < 0) {
		ARLOGe("setupCamera(): Error loading parameter file %s for camera.\n", cparam_name);
		arVideoClose();
		return (FALSE);
	}
	if (cparam.xsize != xsize || cparam.ysize != ysize) {
		ARLOGw("*** Camera Parameter resized from %d, %d. ***\n", cparam.xsize, cparam.ysize);
		arParamChangeSize(&cparam, xsize, ysize, &cparam);
	}
#ifdef DEBUG
	ARLOG("*** Camera Parameter ***\n");
	arParamDisp(&cparam);
#endif
	if ((*cparamLT_p = arParamLTCreate(&cparam, AR_PARAM_LT_DEFAULT_OFFSET)) == NULL) {
		ARLOGe("setupCamera(): Error: arParamLTCreate.\n");
		return (FALSE);
	}

	if ((*arhandle = arCreateHandle(*cparamLT_p)) == NULL) {
		ARLOGe("setupCamera(): Error: arCreateHandle.\n");
		return (FALSE);
	}
	if (arSetPixelFormat(*arhandle, pixFormat) < 0) {
		ARLOGe("setupCamera(): Error: arSetPixelFormat.\n");
		return (FALSE);
	}
	if (arSetDebugMode(*arhandle, AR_DEBUG_DISABLE) < 0) {
		ARLOGe("setupCamera(): Error: arSetDebugMode.\n");
		return (FALSE);
	}
	if ((*ar3dhandle = ar3DCreateHandle(&cparam)) == NULL) {
		ARLOGe("setupCamera(): Error: ar3DCreateHandle.\n");
		return (FALSE);
	}

	if (arVideoCapStart() != 0) {
		ARLOGe("setupCamera(): Unable to begin camera data capture.\n");
		return (FALSE);
	}

	return (TRUE);
}

//==============
// arglの初期化
//==============
bool cARTK::initArgl( void )
{
	// arglをOpenGLの現在のコンテキスト用にセットアップ
	if( (m_pArglSettings = arglSetupForCurrentContext(&(gCparamLT->param), arVideoGetPixelFormat())) == NULL )
	{
		ErrorMessage( "arglSetupForCurrentContext() returned error.\n" );
		return false;
	}

	double	c = cos( 3.1515926 * 0.5 );
	double	s = sin( 3.1515926 * 0.5 );
	m_dmatRotX[ 0] = 1.0; m_dmatRotX[ 1] = 0.0; m_dmatRotX[ 2] = 0.0; m_dmatRotX[ 3] = 0.0;
	m_dmatRotX[ 4] = 0.0; m_dmatRotX[ 5] =   c; m_dmatRotX[ 6] =   s; m_dmatRotX[ 7] = 0.0;
	m_dmatRotX[ 8] = 0.0; m_dmatRotX[ 9] =  -s; m_dmatRotX[10] =   c; m_dmatRotX[11] = 0.0;
	m_dmatRotX[12] = 0.0; m_dmatRotX[13] = 0.0; m_dmatRotX[14] = 0.0; m_dmatRotX[15] = 1.0;

	MatrixRotationX( m_matRotX, -3.1415926f * 0.5f );

	return true;
}

//==================
// マーカー検出処理
//==================

bool cARTK::update( void )
{
	ARUint8* pImg;

	// カメラ画像の取得
	if( (pImg = arVideoGetImage()) == NULL ) {
        arUtilSleep(2);
        return false;
    }
	m_pARTImage = pImg;

	m_bMarkerFound = false;

	// カメラ画像からマーカーを検出
	if(arDetectMarker(gARHandle, m_pARTImage) < 0 )
	{
		exit( -1 );
	}

	// 検出されたマーカー情報の中から一番信頼性の高いものを探す
	int		k = -1;
	for( int j = 0 ; j < gARHandle->marker_num; j++ )
	{
		if(gARHandle->markerInfo[j].id == m_iPattID )
		{
			if( k == -1 || gARHandle->markerInfo[j].cf > gARHandle->markerInfo[k].cf)	k = j;
		}
	}

	if( k != -1 )
	{
		// カメラのトランスフォーム行列を取得
		if (m_bFirstTime) {
			arGetTransMatSquare(gAR3DHandle, &(gARHandle->markerInfo[k]), m_dPattWidth, m_dPattTransMat);
		}
		else {
			arGetTransMatSquare(gAR3DHandle, &(gARHandle->markerInfo[k]), m_dPattWidth, m_dPattTransMat);
			//arGetTransMatSquareCont(gAR3DHandle, &(pMarkerInfo[k]), m_dPattWidth, m_dPattTransMat);
		}

		m_bFirstTime = false;

		m_bMarkerFound = true;
	}

	//// 次のカメラ画像のキャプチャを開始
	//arVideoCapNext();

	return true;
}

//==============
// 描画時の処理
//==============
#define VIEW_DISTANCE_MIN		40.0        // Objects closer to the camera than this will not be displayed. OpenGL units.
#define VIEW_DISTANCE_MAX		10000.0     // Objects further away from the camera than this will not be displayed. OpenGL units.

void cARTK::display( void )
{
	ARdouble p[16];
	ARdouble m[16];

	// Select correct buffer for this context.
	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers for new frame.

	arglPixelBufferDataUpload(gArglSettings, m_pARTImage);
	arglDispImage(gArglSettings);
	m_pARTImage = NULL; // Invalidate image data.

					  // Projection transformation.
	arglCameraFrustumRH(&(gCparamLT->param), VIEW_DISTANCE_MIN, VIEW_DISTANCE_MAX, p);
	glMatrixMode(GL_PROJECTION);
#ifdef ARDOUBLE_IS_FLOAT
	glLoadMatrixf(p);
#else
	glLoadMatrixd(p);
#endif
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);

	// Viewing transformation.
	glLoadIdentity();
	// Lighting and geometry that moves with the camera should go here.
	// (I.e. must be specified before viewing transformations.)
	//none


	// Any 2D overlays go here.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, (GLdouble)windowWidth, 0, (GLdouble)windowHeight, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	//glutSwapBuffers();
}

//====================
// カメラの位置の取得
//====================
void cARTK::getCameraPos( Vector3 *pvec3CamPos )
{
	double		dInvPattTransMat[3][4];

	arUtilMatInv( m_dPattTransMat, dInvPattTransMat );

	pvec3CamPos->x = (float)dInvPattTransMat[0][3];
	pvec3CamPos->y = (float)dInvPattTransMat[1][3];
	pvec3CamPos->z = (float)dInvPattTransMat[2][3];
	Vector3Transform( pvec3CamPos, pvec3CamPos, m_matRotX );

	pvec3CamPos->x *= (float)m_dViewScaleFactor;
	pvec3CamPos->y *= (float)m_dViewScaleFactor;
	pvec3CamPos->z *= (float)m_dViewScaleFactor;
}

//============================
// プロジェクション行列の取得
//============================
void cARTK::getProjectionMat( double dMat[16] )
{
								//        near  far
	arglCameraFrustumRH( &m_sCameraParam, 0.1, 400.0, dMat );
}

//========================
// モデルビュー行列の取得
//========================
void cARTK::getModelViewMat( double dMat[16] )
{
	double	dmatTemp[16];

	arglCameraViewRH( m_dPattTransMat, dmatTemp, m_dViewScaleFactor );

	for( int i = 0 ; i < 4 ; i++ )
	{
		int		idx = i << 2;
		dMat[idx + 0] =	m_dmatRotX[idx] * dmatTemp[0] + m_dmatRotX[idx + 1] * dmatTemp[4] + m_dmatRotX[idx + 2] * dmatTemp[ 8] + m_dmatRotX[idx + 3] * dmatTemp[12];
		dMat[idx + 1] =	m_dmatRotX[idx] * dmatTemp[1] + m_dmatRotX[idx + 1] * dmatTemp[5] + m_dmatRotX[idx + 2] * dmatTemp[ 9] + m_dmatRotX[idx + 3] * dmatTemp[13];
		dMat[idx + 2] =	m_dmatRotX[idx] * dmatTemp[2] + m_dmatRotX[idx + 1] * dmatTemp[6] + m_dmatRotX[idx + 2] * dmatTemp[10] + m_dmatRotX[idx + 3] * dmatTemp[14];
		dMat[idx + 3] =	m_dmatRotX[idx] * dmatTemp[3] + m_dmatRotX[idx + 1] * dmatTemp[7] + m_dmatRotX[idx + 2] * dmatTemp[11] + m_dmatRotX[idx + 3] * dmatTemp[15];
	}
}

//==============
// 拡大率の増減
//==============
void cARTK::addViewScaleFactor( double dAdd )
{
	m_dViewScaleFactor += dAdd;

	// 数字が小さいほどモデルは大きく表示される
	if( m_dViewScaleFactor < 0.01 )	m_dViewScaleFactor = 0.01;
	if( 0.80 < m_dViewScaleFactor )	m_dViewScaleFactor = 0.80;
}

//======================
// ライブラリの終了処理
//======================
void cARTK::cleanup( void )
{
	if( m_pArglSettings )
	{
		arglCleanup( m_pArglSettings );
		m_pArglSettings = NULL;
	}
	arVideoCapStop();
	arVideoClose();
}
