#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cassert>
#include <bitset>
#include <cstdio>
#include <unistd.h>	// for usleep
#include <dirent.h>
using namespace std;

#include <filesystem>
using std::__fs::filesystem::exists;
using std::__fs::filesystem::is_directory;
using std::__fs::filesystem::create_directory;

#include <boost/format.hpp>
#include <boost/numeric/interval.hpp>
#include <boost/numeric/interval/io.hpp>
using namespace boost;
using namespace boost::numeric;

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
// #include <boost/numeric/ublas/io.hpp>
// using namespace boost::numeric::ublas;

#include <strstream>
#include <string>
#include <GLUT/glut.h>
#include <GL/glui.h>

#include "Vars.h"

#include "Common.h"
#include "Filter.h"
#include "Delaunay.h"
#include "Height.h"
#include "LandPlot.h"
#include "Mesh.h"
#include "Level.h"
#include "JCN.h"
#include "Fiber.h"

#include "Map.h"

#include "LandFlag.h"

#include "LandMap.h"

#include "Bridson.h"

#include "Config.h"

// GL2PS
#include "gl2ps.h"

//------------------------------------------------------------------------------
//	Macro switches
//------------------------------------------------------------------------------
// [[[Select one of them]]]
//#define ORDINARY_DOMAIN
//#define RESTRICTED_DOMAIN
//#define FUKUSHIMA_DOMAIN
//#define NIIDA_DOMAIN

//#define DEBUG_FOR_TRACKING_FEATURE_VALUES

// Color for longitude and latitude lines
#define LL_LINE_COLOR	0.4, 0.4, 0.4

// To render the R-value as the texture on the map
//------------------------------------------------------------------------------
// #define ENABLE_R_VALUE_COLORING // <= This must be validated for rendering the R map.
//------------------------------------------------------------------------------

#ifdef ENABLE_R_VALUE_COLORING
#define LIMIT_R_INTERVAL_MIN	(-0.60)
#define LIMIT_R_INTERVAL_MAX	(1.00)

#define SAMPLE_TEXTURE_Y_VALUE	(0.8)
#endif	// ENABLE_R_VALUE_COLORING

#define AUTOMATIC_NORMALIZATION

// #define USING_DELAUNAY_SAMPLING
#define USING_POISSON_SAMPLING

#define RIGHTMOST_PANEL

// #define HIDE_RANGE_WINDOW

#define DISABLE_AREA_RECTANGLES

// #define USE_ROLLOUT

// #define SOLID_TEXTURING
#define DISABLE_THIRD_TEXTURING

#define USE_FTGL

#ifdef USE_FTGL
// FTGL
#include <FTGL/ftgl.h>
#endif	// USE_FTGL

//------------------------------------------------------------------------------
//	Macro values
//------------------------------------------------------------------------------
#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))
#define TITLEBAR_GAP		(48)
#define MAX_UPPER_LIMIT		(5)
#define NUM_SKIP_FACTORS	(18)

#define DEFAULT_WIDTH		(384)
#define DEFAULT_HEIGHT		(384)
//#define DEFAULT_WIDTH		(512)
//#define DEFAULT_HEIGHT		(512)
#define LARGE_WIDTH		(1024)
#define LARGE_HEIGHT		(1024)

#define WINDOW_SIDE		(1.0)
#define WINDOW_MARGIN		(0.2)
// #define WINDOW_MARGIN		(1.0)
#define MAP_SIDE		(WINDOW_SIDE+WINDOW_MARGIN)

#define TEXTURE_SIZE		(RASTER_RESOLUTION)

// #define POISSON_SAMPLING_SCALE	(0.5)	// sparse sampling
#define POISSON_SAMPLING_SCALE	(1.0)	// normal sampling
// #define POISSON_SAMPLING_SCALE	(2.0)	// dense sampling
// #define POISSON_SAMPLING_SCALE	(5.0)

#define KEY_ESC		(27)

// #define DRAW_SEPARATING_LINES

//#define NUM_MESHES		(2)
#define NUM_VARIABLES		(2)

#define FLAT_MODE		(100)
#define TEXTURE_MODE		(110)
#define WIREFRAME_MODE		(120)
#define BASEMENT_MODE		(130)

#define RENDER_MAP		(140)

#define VALUE_TYPE_0		(200)
#define VALUE_TYPE_1		(201)
#define VALUE_TYPE_2		(202)

#define VALUE_TYPE_UPDATE	(209)

#define	CS137_HEIGHT_DISTANCE	(400)
#define	CS137_HEIGHT_GRAD	(401)
#define	CS137_HEIGHT_ACCUM	(402)

#define RESOLUTION_BASE_ALL	(500)
#define RESOLUTION_BASE_P	(520)
#define RESOLUTION_BASE_Q	(540)
#define RESOLUTION_BASE_R	(560)
#define PROJECT_TRIANGLES	(599)

#define	OUTLINE_ON		(301)
#define	OUTLINE_OFF		(302)
#define	FILL_ON			(303)
#define	FILL_OFF		(304)
#define	COMMON_ON		(305)
#define	COMMON_OFF		(306)

#define ALL_ON			(391)
#define ALL_OFF			(392)

#define RICE_ON			(401)
#define RICE_OFF		(402)
#define FARM_ON			(403)
#define FARM_OFF		(404)
#define FOREST_ON		(405)
#define FOREST_OFF		(406)
#define WASTE_ON		(407)
#define WASTE_OFF		(408)

#define URBAN_ON		(411)
#define URBAN_OFF		(412)
#define ROAD_ON			(413)
#define ROAD_OFF		(414)
#define RAILWAY_ON		(415)
#define RAILWAY_OFF		(416)
#define MARSH_ON		(417)
#define MARSH_OFF		(418)

#define BEACH_ON		(421)
#define BEACH_OFF		(422)
#define OCEAN_ON		(423)
#define OCEAN_OFF		(424)
#define GOLF_ON			(425)
#define GOLF_OFF		(426)
#define OTHER_ON		(427)
#define OTHER_OFF		(428)

#define LAND_FILTER		(431)

#define DIST_LIMIT_000_020	(451)
#define DIST_LIMIT_000_040	(452)
#define DIST_LIMIT_000_060	(453)
#define DIST_LIMIT_000_080	(454)
#define DIST_LIMIT_000_100	(455)
#define DIST_LIMIT_020_040	(456)
#define DIST_LIMIT_040_060	(457)
#define DIST_LIMIT_060_080	(458)
#define DIST_LIMIT_080_100	(459)
#define DIST_LIMIT_OFF		(499)

#define UPDATE_LOWER		(496)
#define UPDATE_UPPER		(497)
#define DIST_SEGMENT		(498)

#define BLANK_ITEM		(999)


#define CHANGE_FACTOR		(651)
#define DOWNSAMPLE_MAP		(652)
#define RESAMPLE_MAP		(653)

#define CSV_OUTPUT		(680)

#define TRANSFORM_MAP		(711)


#define PROCESS_ALL		(600)
#define INIT_VIEWPOINT		(700)
#define CHANGE_VIEWPOINT	(701)
#define DUMP_SELECTED		(751)
#define WIREFRAME_EPS		(799)
#define CAPTURE_SHAPE		(800)
#define CAPTURE_PQ		(805)
#define CAPTURE_PR		(806)
#define CAPTURE_QR		(807)
#define CAPTURE_RANGE		(810)
#define CAPTURE_ALL		(820)
#define QUIT			(900)



// macro for Similarity Matrix
#define JOINT_CONTOUR_NET	(400)
#define CONTOUR_TREE		(410)

//#define FRONT_DEPTH		(0.001)
#define FRONT_DEPTH		(0.01)
//#define FRONT_DEPTH		(0.1)


using namespace std;

// shader objects
// static unsigned int textureProgram, simpleProgram, solidtexProgram, imagetexProgram;
// buffer objects
// static GLuint vertexbuffer, uvbuffer, elementbuffer;

std::vector < string >		filelist;
// original triangular mesh
Height				* fundField = NULL;
Height				* adaptField = NULL;
Polyhedron			meshMap;	// Poisson disk sampling
std::vector< P_Facet_handle >	ptrFacet;	// facets of meshDisk

// Land map
LandMap				landMap;


#ifdef USE_DISTANCES
int	valueID = 0;
//int	valueID = 3;
#endif	// USE_DISTANCES
#ifdef USE_EXPONENTS
int	valueID = 8;
#endif	// USE_EXPONENTS
int	skip_factor[ NUM_SKIP_FACTORS ] = {
    1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 20, 25, 50, 100, 200, 250, 500, 1000
};
#ifdef USE_DISTANCES
int	current_factor = skip_factor[ 0 ];	// 1
//int	current_factor = skip_factor[ 6 ];	// 8
//int	current_factor = skip_factor[ NUM_SKIP_FACTORS - 1 ]; // 25
#endif	// USE_DISTANCES
#ifdef USE_EXPONENTS
//int	current_factor = skip_factor[ 6 ];	// 8
int	current_factor = skip_factor[ 3 ];	// 4
#endif	// USE_EXPONENTS
/* display mode */
int	display_mode	= 1;
//bool	flag_outline	= true;
bool	flag_outline	= false;
bool	flag_fill	= true;
bool	flag_all	= false;
bool	flag_data	= false;	

bool	is_processed		= false;
bool	is_focused		= false;
// bool	is_computed		= true;
bool	is_displayed_shape	= false;
bool	is_displayed_PQ		= false;
bool	is_displayed_PR		= false;
bool	is_displayed_QR		= false;
bool	is_displayed_range	= false;

LandFlag	landflag;

// descriptor of display list.
GLuint			objects;
unsigned int		num_displaylist = 0;
// size of selection buffer
#define SELECTIONS (256)

/* debug ID */
// static int debugID = 0;

/* window ID */
static int		win_shape;
static int		win_PQ, win_PR, win_QR;
static int		win_range;
// win_sim, win_legend;

// static double		alpha = 0.8;

/* window sizes */
static int		width_shape	= LARGE_WIDTH;
static int		height_shape	= LARGE_HEIGHT;
static int		width_range	= DEFAULT_WIDTH;
static int		height_range	= DEFAULT_HEIGHT;
static int		width_PQ	= DEFAULT_WIDTH;
static int		height_PQ	= DEFAULT_HEIGHT;
static int		width_PR	= DEFAULT_WIDTH;
static int		height_PR	= DEFAULT_HEIGHT;
static int		width_QR	= DEFAULT_WIDTH;
static int		height_QR	= DEFAULT_HEIGHT;

/* projection parameters */
static double fovy	= 24.0;
static double aspect	= 1.0;
static double near	= 0.0001;
static double far	= 100.0;

//double view_distance	= 5.0;
double view_distance	= 5.5;
double view_azimuth	= 0.0;	/*179.429688;*///45.0;   /*angle of object*/
//double view_incidence	= 45.0; /*90.484375*/ //60.0;
double view_incidence	= 0.0; /*90.484375*/ //60.0;
double view_twist	= 0.0;

/* mouse positions and its button status */
int		mx_shape, my_shape;
bool		left_shape = false, middle_shape = false, right_shape = false;
int		mx_range, my_range;
bool		left_range = false, middle_range = false, right_range = false;
int		mx_PQ, my_PQ;
bool		left_PQ = false, middle_PQ = false, right_PQ = false;
int		mx_PR, my_PR;
bool		left_PR = false, middle_PR = false, right_PR = false;
int		mx_QR, my_QR;
bool		left_QR = false, middle_QR = false, right_QR = false;

// multivariate functions
double (*mapP)( P_Vertex_handle v ) = NULL;
double (*mapQ)( P_Vertex_handle v ) = NULL;
double (*mapR)( P_Vertex_handle v ) = NULL;

LandPlot * land	= new LandPlot;

void preprocess( void );
void refocus( void );
void sparseSampling( ublas::matrix< double > & from,
		     ublas::matrix< double > & to,
		     unsigned int factor );
void timer( int value );
void matchSamples( Height * H,
		   std::vector< P_Facet_handle > & ptrFacet );

// Overall bounding box
P_Bbox2						domain;
P_Bbox2						boundPQ, boundPR, boundQR;
P_Bbox3						boundPQR;
std::vector< string >				title;
ublas::matrix< double >				entire, region, poisson;
ublas::matrix< double >				radius;
std::vector< double >				coordX, coordY;
std::vector< double >				attrP, attrQ, attrR;

//------------------------------------------------------------------------------
//	configuration variables
//------------------------------------------------------------------------------
string	place;
string  tessellation;
string  density;
double	x_min, x_max, y_min, y_max;
unsigned int idP = 0, idQ = 0, idR = 0;
//interval< double >				rangeX( 139.000, 141.000 );
//interval< double >				rangeY(  36.333,  37,666 );
interval< double >				rangeX( 130.000, 150.000 );
interval< double >				rangeY(  20.333,  50,666 );
string	file_name;
string	bin_file_name;
string	csv_file_name;

#ifdef SKIP
// Variables for Poisson disk sampling
std::vector< P_Point2 >				disk;
std::vector< double >				diskX, diskY, diskH, diskD;
std::vector< double >				diskPriAve, diskPosAve;
std::vector< double >				diskPriSdv, diskPosSdv;
#endif	// SKIP

// X in [140.807, 141.041] 
// Y in [37.2432, 37.6037]



#ifdef ORDINARY_DOMAIN
interval< double >				rangeX( 139.85, 141.05 );
interval< double >				rangeY(  36.95,  37.75 );
#endif	// ORDINARY_DOMAIN

// ------------------------------------------------------------
#ifdef RESTRICTED_DOMAIN
// 37.65762939264862, 141.03593786123952
// 37.38086642587012, 140.5552561834309
// translate = ( 140.805 , 37.5195 )
// scale = ( 0.207753 , 0.138502 )
// main.cpp:4034  translate = ( 140.755 , 37.5195 )
// main.cpp:4036  scale = ( 0.292677 , 0.195118 )

// for WIREFRAME EPS
// Translate X 140.755 => 140.720
interval< double >				rangeX( 140.462, 141.047 );
// OTHERWISE
//interval< double >				rangeX( 140.462, 141.047 );
interval< double >				rangeY(  37.324,  37.715 );
//#define DATA_FILE_NAME				"JAEA_10YR_integrated_map_less_020.csv"
//#define BIN_FILE_NAME				"JAEA_10YR_integrated_map_less_020.bin"
#if defined( WITH_STANDARD_DEVIATION ) || defined( WITH_LAND_TYPE ) || defined( WITH_REDUCTION_RATIO )
#define DATA_FILE_NAME			"JAEA_2017_ONLY_integrated_map_less_120.csv"
#define BIN_FILE_NAME			"JAEA_2017_ONLY_integrated_map_less_120.bin"
//#define DATA_FILE_NAME			"JAEA_2017_ONLY_integrated_map_less_040.csv"
#endif	// defined
#ifdef WITH_TWO_RATIOS
#define DATA_FILE_NAME			"JAEA_Every_3YRS_integrated_map_less_120.csv"
#define BIN_FILE_NAME			"JAEA_Every_3YRS_integrated_map_less_120.bin"
//#define DATA_FILE_NAME			"JAEA_Every_3YRS_integrated_map_less_020.csv"
//#define BIN_FILE_NAME			"JAEA_Every_3YRS_integrated_map_less_020.bin"
#endif	// WITH_TWO_RATIOS
#endif	// RESTRICTED_DOMAIN
// ------------------------------------------------------------

// ------------------------------------------------------------
#ifdef FUKUSHIMA_DOMAIN
//37.84579533190114, 140.59712877347485
//37.70889474057918, 140.35936065404852    
// translate = ( 140.458 , 37.7675 )
// scale = ( 0.184608 , 0.123072 )
// main.cpp:4036  translate = ( 140.525 , 37.7195 )
// main.cpp:4038  scale = ( 0.323308 , 0.215539 )
// main.cpp:4059  translate = ( 140.604 , 37.6695 )
// main.cpp:4061  scale = ( 0.293424 , 0.195616 )
//------------------------------------------------------------------------------
//	Previous setup
//------------------------------------------------------------------------------
//interval< double >				rangeX( 140.311, 140.897 );
//interval< double >				rangeY(  37.474,  37.865 );
//------------------------------------------------------------------------------
//	New setup	Center ( 37.7842454, 140.48895126 )
//	SN interval	-> 0.391
//	EW interval	-> 0.586	1.5 bai
//------------------------------------------------------------------------------
//interval< double >				rangeX( 140.1959513, 140.8919513 );
//interval< double >				rangeY(  37.5887454,  37.9797454 );
// ************************************************************
interval< double >				rangeX( 140.3747065, 140.6028522 );
						// center: 140.48877935
						// interval: 0.2281457 (/1.5 = 0.152097133)
interval< double >				rangeY(  37.7176532,  37.8697504 );
						// 0.076048566
// ************************************************************
//interval< double >				rangeX( 140.3807863, 140.5971163 );
						// 0.108165
//interval< double >				rangeY(  37.7215918,  37.8658118 );
						// center: 37.79370180
						// interval: 0.14422	(x1.5 = 0.21633)
#if defined( WITH_STANDARD_DEVIATION ) || defined( WITH_LAND_TYPE ) || defined( WITH_REDUCTION_RATIO )
#define DATA_FILE_NAME			"JAEA_2017_ONLY_integrated_map_less_160.csv"
#define BIN_FILE_NAME			"JAEA_2017_ONLY_integrated_map_less_160.bin"
#endif	// defined
#ifdef WITH_TWO_RATIOS
//#define DATA_FILE_NAME			"JAEA_2017_ONLY_integrated_map_less_040.csv"
//#define BIN_FILE_NAME			"JAEA_2017_ONLY_integrated_map_less_040.bin"
#define DATA_FILE_NAME			"JAEA_Every_3YRS_integrated_map_all.csv"
#define BIN_FILE_NAME			"JAEA_Every_3YRS_integrated_map_all.bin"
#endif	// WITH_TWO_RATIOS
#endif	// FUKUSHIMA_DOMAIN
// ------------------------------------------------------------

// ------------------------------------------------------------
#ifdef NIIDA_DOMAIN
// for WIREFRAME EPS
interval< double >				rangeX( 140.666, 141.833 );
// OTHERWISE
interval< double >				rangeY(  37.426,  37.916 );
#ifdef WITH_TWO_RATIOS
#define DATA_FILE_NAME			"JAEA_Every_1YR_integrated_map_less_100.csv"
#define BIN_FILE_NAME			"JAEA_Every_1YR_integrated_map_less_100.bin"
#endif	// WITH_TWO_RATIOS
#endif	// NIIDA_DOMAIN
// ------------------------------------------------------------
    
    
//interval< double >				rangeX( 140.8070, 141.0410 );
//interval< double >				rangeY(  37.2432,  37.6037 );

//interval< double >				rangeX( 140.340, 141.060 );
//interval< double >				rangeY(  37.260, 37.740 );

//interval< double >				rangeX( 138.80, 141.20 );
//interval< double >				rangeY(  36.70,  38.30 );

interval< double > 				rangeH = INTERVAL_HEIGHT;
interval< double > 				rangeD = INTERVAL_DISTANCE;
interval< double > 				rangeG = INTERVAL_GRADIENT;

interval< double > 				rangePriAve = INTERVAL_PRI_AVE;
interval< double > 				rangePosAve = INTERVAL_POS_AVE;
interval< double > 				rangePriSdv = INTERVAL_PRI_SDV;
interval< double > 				rangePosSdv = INTERVAL_POS_SDV;

interval< double >				rangeRatioF = INTERVAL_RATIO;
interval< double >				rangeRatioM = INTERVAL_RATIO;
interval< double >				rangeRatioL = INTERVAL_RATIO;

#ifdef SKIP
#ifdef WITH_STANDARD_DEVIATION
interval< double >				rangeP = rangePosAve;
interval< double >				rangeQ = rangeH;
interval< double >				rangeR = rangeD;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
interval< double >				rangeP = rangePosAve;
interval< double >				rangeQ = rangeH;
interval< double >				rangeR = rangeD;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
interval< double >				rangeP = rangePosAve;
interval< double >				rangeQ = rangeRatioM;
interval< double >				rangeR = rangeD;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
interval< double >				rangeP = rangeRatioF;
interval< double >				rangeQ = rangeRatioL;
interval< double >				rangeR = rangePosAve;
#endif	// WITH_TWO_RATIOS
#endif	// SKIP
interval< double >				rangeP;
interval< double >				rangeQ;
interval< double >				rangeR;

const int					nScales = 2;
const double					pitchH = 0.42;
const double					pitchV = 0.38;

float translate_x	= median( rangeX );
float translate_y	= median( rangeY );
float map_scale		= 0.5 * std::max( width( rangeX ), width( rangeY )*3.0/2.0 );
float scale_x		= map_scale;
float scale_y		= 2.0 * map_scale / 3.0;

// flag for rasterization
bool	visible_shape = false;
bool    visible_range = false;
bool	visible_PQ = false, visible_PR = false, visible_QR = false;
bool	switchFlag				= true;
bool	distFlag				= false;
// lower and upper limit of the distance from FDNNP
double	lowerDistLimit				= 0.0;			
double	upperDistLimit				= 100.0;
// bool	rasflag					= false;
// ID list for the rasteraized faces
std::vector< VecFace >				pixelFacePQ[ NUM_VALUES ];
std::vector< VecFace >				pixelFacePR[ NUM_VALUES ];
std::vector< VecFace >				pixelFaceQR[ NUM_VALUES ];
// ID list for the rasterized edges
std::vector< VecHalf >				pixelEdgePQ[ NUM_VALUES ];
std::vector< VecHalf >				pixelEdgePR[ NUM_VALUES ];
std::vector< VecHalf >				pixelEdgeQR[ NUM_VALUES ];

// currently selected
std::vector< unsigned int >			pixelPQ, pixelPR, pixelQR;
bool						touchPQ = true,
						touchPR = true,
					        touchQR = true,
					        touchAll = true; 
map< unsigned int, P_Facet_handle >		secPQ, secPR, secQR, secAll;


// Rasterization reference
bool	isProjected[ NUM_VALUES ];
bool	(*cf)(const Tuple3 & ,const Tuple3 &)	= CGAL::lexicographically_xyz_smaller;
typedef map< Tuple3, Level*, bool(*)(const Tuple3 &, const Tuple3 & ) > Sampling;
// Sampling resolution( cf );
Sampling ** resolution;
Level * curLevel				= NULL;

// vector < VecFace >				selectedFace;
// VecVertex					vertexBuf;

// parameters

unsigned int		numVer = 0;
unsigned int		PNFcount = 0;

typedef unsigned char	Zone[TEXTURE_SIZE][TEXTURE_SIZE][4];
static GLubyte	teximage[ NUM_RESOLUTIONS ][ TEXTURE_SIZE ][ TEXTURE_SIZE ][ 4 ];
//static GLuint	texid[ NUM_RESOLUTIONS ];


#ifdef USING_ANNOTATION
// for annotation
const unsigned int nCities = 6;
double cityPosition[ nCities ][ 2 ] = { 
    { 139.932432,	37.496652 },	// Aizu Wakamatsu
    { 140.363646,	37.405074 },	// Koriyama
    { 140.476255,	37.759859 },	// Fukushima
    { 140.893736,	37.050793 },	// Iwaki
    { 140.923948,	37.792422 },	// Soma
    { 140.217648,	37.125286 }	// Shirakawa
};
char cityName[ nCities ][ 32 ] = {
    "Aizu Wakamatsu",
    "Koriyama",
    "Fukushima",
    "Iwaki",
    "Soma",
    "Shirakawa"
};
double cityCoord[ nCities ][ 2 ];
Vertex_handle cityVh[ nCities ];
#else	// USING_ANNOTATION
const unsigned int nMarks = 1;
const unsigned int nCities = nMarks + 8 + 9 + 11 + 9 + 10;
double cityPosition[ nCities ][ 2 ] = { 
    //    { 141.032615,	37.421456 },	// FDNNP
    // Major points 6
    { 141.002615,	37.421456 },	// FDNNP
    //{ 141.26,		39.02 },	// Area1
    //{ 139.51,		36.71 },	// Area2
    //{ 140.26,		37.41 },	// Area3
    //{ 140.57,		37.16 },	// Area4
    //{ 140.75,		37.65 },	// Area5

    // Miyagi 8
    { 140.866667,	38.266667 },	// Sendai
    { 141.3,		38.416667 },	// Ishinomaki
    { 141.016667,	38.316667 },	// Shiogama
    { 140.95,		38.566667 },	// Furukawa	
    { 141.566667,	38.9	  },	// Kesennuma
    { 140.616667,	38	  },	// Shiroishi
    { 140.783333,	37.966667 },	// Kakuda
    { 140.866667,	38.1	  },	// Iwanuma

    // Fukushima 9
    { 140.466667,	37.75	  },	// Fukushima
    { 139.916667,	37.483333 },	// Aizuwakamatsu
    { 140.35,		37.4	  },	// Koriyama
    { 140.883333,	37.05	  },	// Iwaki
    { 140.2,		37.116667 },	// Shirakawa
    { 140.366667,	37.283333 },	// Sukagawa
    { 139.866667,	37.65	  },	// Kitakata
    { 140.916667,	37.783333 },	// Souma
    { 140.433333,	37.583333 },	// Nihonnmatsu

    // Ibaraki 11
    { 140.466667,	36.366667 },	// Mito
    { 140.65,		36.6	  },	// Hitachi
    { 140.183333,	36.066667 },	// Tsuchiura
    { 139.7,		36.183333 },	// Koga
    { 140.283333,	36.183333 },	// Ishioka
    { 140.7,		36.716667 },	// Takahagi
    { 140.75,		36.8	  },	// Kitaibaraki
    { 140.05,		35.9	  },	// Toride
    // { 140.066667,	36.033333 },	// Tsukuba
    { 140.066667,	36.05 },	// Tsukuba
    { 140.633333,	35.966667 },	// Kashima
    { 139.966667,	35.95	  },	// Moriya

    // Tochigi 9
    { 139.883333,	36.55	  },	// Utsunomiya
    { 139.45,		36.333333 },	// Ashikaga
    { 139.733333,	36.383333 },	// Tochigi
    { 139.566667,	36.316667 },	// Sano
    { 139.6,		36.75	  },	// Nikkou
    { 139.8,		36.316667 },	// Oyama
    { 140,		36.433333 },	// Moka
    { 140.016667,	36.866667 },	// Otawara
    { 140.033333,	36.95	  },	// Kuroiso

    // Gunma 10
    { 139.05,		36.383333 },	// Maebashi
    { 139,		36.316667 },	// Takasaki
    { 139.316667,	36.4	  },	// Kiryu
    { 139.183333,	36.3	  },	// Isezaki
    { 139.366667,	36.283333 },	// Ota
    { 139.033333,	36.633333 },	// Numata 
    { 139.533333,	36.233333 },	// Tatebayashi
    { 139,		36.483333 },	// Shibukawa
    { 139.066667,	36.25	  },	// Fujioka
    { 138.883333,	36.25	  },	// Tomioka
    
};
char cityName[ nCities ][ 32 ] = {
    "FDNPP",
    //"Area 1",
    //"Area 2",
    //"Area 3",
    //"Area 4",
    //"Area 5",

    "Sendai",
    "Ishinomaki",
    "Shiogama",
    "Furukawa",
    "Kesennuma",
    "Shiroishi",
    "Kakuda",
    "Iwanuma",

    "Fukushima",
    "Aizuwakamatsu",
    "Koriyama",
    "Iwaki",
    "Shirakawa",
    "Sukagawa",
    "Kitakata",
    "Souma",
    "Nihonnmatsu",

    "Mito",
    "Hitachi",
    "Tsuchiura",
    "Koga",
    "Ishioka",
    "Takahagi",
    "Kitaibaraki",
    "Toride",
    "Tsukuba",
    "Kashima",
    "Moriya",

    "Utsunomiya",
    "Ashikaga",
    "Tochigi",
    "Sano",
    "Nikkou",
    "Oyama",
    "Moka",
    "Otawara",
    "Kuroiso",

    "Maebashi",
    "Takasaki",
    "Kiryu",
    "Isezaki",
    "Ota",
    "Numata",
    "Tatebayashi",
    "Shibukawa",
    "Fujioka",
    "Tomioka",

};
int cityLayout[ nCities ] = {
    1,	// "FDNPP",

    1,	// "Sendai",
    1,	// "Ishinomaki",
    1,	// "Shiogama",
    1,	// "Furukawa",
    1,	// "Kesennuma",
    1,	// "Shiroishi",
    1,	// "Kakuda",
    1,	// "Iwanuma",

    1,	// "Fukushima",
    1,	// "Aizuwakamatsu",
    1,	// "Koriyama",
    1,	// "Iwaki",
    1,	// "Shirakawa",
    1,	// "Sukagawa",
    1,	// "Kitakata",
    1,	// "Souma",
    1,	// "Nihonnmatsu",

    1,	// "Mito",
    1,	// "Hitachi",
    1,	// "Tsuchiura",
    1,	// "Koga",
    1,	// "Ishioka",
    1,	// "Takahagi",
    1,	// "Kitaibaraki",
    1,	// "Toride",
    4,	// "Tsukuba",
    1,	// "Kashima",
    1,	// "Moriya",

    1,	// "Utsunomiya",
    1,	// "Ashikaga",
    1,	// "Tochigi",
    4,	// "Sano",
    1,	// "Nikkou",
    1,	// "Oyama",
    1,	// "Moka",
    1,	// "Otawara",
    1,	// "Kuroiso",

    1,	// "Maebashi",
    1,	// "Takasaki",
    1,	// "Kiryu",
    4,	// "Isezaki",
    1,	// "Ota",
    1,	// "Numata",
    1,	// "Tatebayashi",
    1,	// "Shibukawa",
    4,	// "Fujioka",
    1	// "Tomioka",

};
double cityCoord[ nCities ][ 2 ];
P_Vertex_handle cityVh[ nCities ];

#ifndef DISABLE_AREA_RECTANGLES
const double dryPosition[ 4 ][ 2 ] = {
    // { 140.738,	36.838 },
    // { 140.738,	37.248 },
    // { 140.512,	37.248 },
    // { 140.512,	36.838 }
    { 140.0 + 3.10/4.2,	37.0 - 0.85/5.25 },
    { 140.0 + 3.10/4.2,	37.0 + 1.30/5.25 },
    { 140.0 + 2.15/4.2,	37.0 + 1.30/5.25 },
    { 140.0 + 2.15/4.2,	37.0 - 0.85/5.25 }
};
const double wetPosition[ 4 ][ 2 ] = {
    // { 140.333,	37.008 },	
    // { 140.619,	37.571 },
    // { 140.357,	37.666 },
    // { 140.071,	37.171 }
    // { 140.0 + 1.4/4.2,	37.0 + 0.4/5.25 },	
    // { 140.0 + 2.6/4.2,	37.0 + 3.0/5.25 },
    // { 140.0 + 1.5/4.2,	37.0 + 3.5/5.25 },
    // { 140.0 + 0.3/4.2,	37.0 + 0.9/5.25 }
    { 140.0 + 1.45/4.2,	37.0 + 0.25/5.25 },	
    { 140.0 + 2.65/4.2,	37.0 + 2.75/5.25 },
    { 140.0 + 1.45/4.2,	37.0 + 3.65/5.25 },
    { 140.0 + 0.25/4.2,	37.0 + 1.15/5.25 }
};
double dryCoord[ 4 ][ 2 ];
double wetCoord[ 4 ][ 2 ];
int	dryBit = 0;
int	wetBit = 0;
#endif	// DISABLE_AREA_RECTANGLES

#endif	// USING_ANNOTATION


// Map * iwate		=	new Map(); //  3
Map * miyagi		=	new Map(); //  4
// Map * yamagata		=	new Map(); //  6
Map * fukushima		=	new Map(); //  7
Map * ibaraki		=	new Map(); //  8
Map * tochigi		=	new Map(); //  9
Map * gunma		=	new Map(); // 10

/* normal light model */
float normalLM_ambient[] =	{  0.2,  0.2,  0.2,  1.0 };

/* normal material and light */
float materialN_ambient[] =	{  0.2,  0.2,  0.2,  1.0 };
float materialN_diffuse[] =	{  0.5,  0.5,  0.5,  1.0 };
float materialN_emission[] =	{  0.1,  0.1,  0.1,  1.0 };
float materialN_specular[] =	{  0.8,  0.8,  0.8,  1.0 };
float materialN_shininess[] =	{  5.0 };

float lightL_ambient[] =	{  0.2,  0.2,  0.2,  1.0 };
float lightL_diffuse[] =	{  0.6,  0.6,  0.6,  1.0 };
float lightL_specular[] =	{  0.6,  0.6,  0.6,  1.0 };
float lightL_position[] =	{ -3.0, -2.0, -1.0,  0.0 };

float lightR_ambient[] =	{  0.2,  0.2,  0.2,  1.0 };
float lightR_diffuse[] =	{  0.6,  0.6,  0.6,  1.0 };
float lightR_specular[] =	{  0.6,  0.6,  0.6,  1.0 };
float lightR_position[] =	{  3.0,  2.0,  1.0,  0.0 };

// Variables for interaction
GLUI *		glui = NULL;
const char *	GLUI_TITLE	= { "Panel" };
const int	GLUI_TRUE	= { true  };
const int	GLUI_FALSE	= { false };
//------------------------------------------------------------------------------
#ifdef ENABLE_R_VALUE_COLORING
int		gridSwitch	= 4;
#else	// ENABLE_R_VALUE_COLORING
//int		gridSwitch	= 2;
int		gridSwitch	= 3;
#endif	// ENABLE_R_VALUE_COLORING
int		stepSwitch	= valueID;
int		outlineSwitch	= 0;
int		fillSwitch	= 1;
int		lowerSwitch	= 0;
//int		upperSwitch	= 0;
int		upperSwitch	= MAX_UPPER_LIMIT;
//int		factorSwitch	= 12;	
int		factorSwitch	= current_factor;	
//------------------------------------------------------------------------------
int		forestBit	= 1;
int		farmBit		= 1;
int		urbanBit	= 1;
int		oceanBit	= 1;


//------------------------------------------------------------------------------
//	Fundamentals
//------------------------------------------------------------------------------
inline unsigned int power( unsigned int base, unsigned int n )
{
    unsigned int p;
    p = 1;
    for ( unsigned int i = 1; i <= n; ++i )
	p = p * base;
    return p;
}

//------------------------------------------------------------------------------
//	OpenGL Functions
//------------------------------------------------------------------------------
void visibility_shape	( int status );
void visibility_PQ	( int status );
void visibility_PR	( int status );
void visibility_QR	( int status );
void visibility_range	( int status );
void display_shape	( void );
void display_PQ		( void );
void display_PR		( void );
void display_QR		( void );
void display_range	( void );
void initGLUI		( void );


void redisplay_all( void )
{
    glutSetWindow( win_shape );
    glutPostRedisplay();
    glutSetWindow( win_PQ );
    glutPostRedisplay();
    glutSetWindow( win_PR );
    glutPostRedisplay();
    glutSetWindow( win_QR );
    glutPostRedisplay();
    glutSetWindow( win_range );
    glutPostRedisplay();
}


//------------------------------------------------------------------------------
//	Annotation
//------------------------------------------------------------------------------
//#ifdef FUKUSHIMA
void initAnnotation( Polyhedron & mesh, P_Bbox2 & domain )
{
    double sideX = domain.xmax() - domain.xmin();
    double sideY = domain.ymax() - domain.ymin();

    cerr << " Domain = " << domain << endl;
    // for each city
    for ( unsigned int k = 0; k < nCities; ++k ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	cityCoord[ k ][ 0 ] = 2.0 * ( cityPosition[ k ][ 0 ] - domain.xmin() )/sideX - 1.0;
	cityCoord[ k ][ 1 ] = 2.0 * ( cityPosition[ k ][ 1 ] - domain.ymin() )/sideY - 1.0;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	cityCoord[ k ][ 0 ] = cityPosition[ k ][ 0 ];
	cityCoord[ k ][ 1 ] = cityPosition[ k ][ 1 ];
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	double minDist = 1.0e+8;
#ifdef DEBUG
	cerr << " k = " << k << " cityCoord = " 
	     << cityCoord[ k ][ 0 ] << " , " << cityCoord[ k ][ 1 ]
	     << " city name = " << cityName[ k ] << endl;
	cerr << " Distance by Hubeny = "
	     << calcDistHubeny( cityPosition[ 0 ][ 1 ], cityPosition[ 0 ][ 0 ],
				cityPosition[ k ][ 1 ], cityPosition[ k ][ 0 ] )
	     << endl;
#endif	// DEBUG
	// search for a vertex the is the nearest to the city
	for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
	    P_Point2 cityPoint( cityCoord[ k ][ 0 ], cityCoord[ k ][ 1 ] );
	    P_Point2 vertexPoint( vi->point().x(), vi->point().y() );
	    double curDist = sqrt( ( vertexPoint - cityPoint ).squared_length() );
	    // double vx = vi->point().x();
	    // double vy = vi->point().y();
	    // double curDist = sqrt( SQUARE( vx - cityCoord[ k ][ 0 ] ) + 
	    // SQUARE( vy - cityCoord[ k ][ 1 ] ) );
	    // cerr << " Vertex ID = " << vi->id() << " curDist = " << curDist << " minDist = " << minDist << endl;
	    if ( curDist < minDist ) {
		cityVh[ k ] = ( P_Vertex_handle )vi;
		minDist = curDist;
	    }
	}
#ifdef DEBUG
	cerr << cityName[ k ] << " :\t ID of the nearest vertex = " << cityVh[ k ]->id()
	     << " Coord = " << cityVh[ k ]->point().x() << " , " << cityVh[ k ]->point().y() 
	     << " dist = " << minDist << endl;
#endif	// DEBUG
    }

#ifndef DISABLE_AREA_RECTANGLES
    // wet & dry rectangular areas
    cerr << HERE << " Updating wet & dry rectangle coordinates ===" << endl;
    for ( unsigned int k = 0; k < 4; ++k ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	wetCoord[ k ][ 0 ] = 2.0 * ( wetPosition[ k ][ 0 ] - domain.xmin() )/sideX - 1.0;
	wetCoord[ k ][ 1 ] = 2.0 * ( wetPosition[ k ][ 1 ] - domain.ymin() )/sideY - 1.0;
	dryCoord[ k ][ 0 ] = 2.0 * ( dryPosition[ k ][ 0 ] - domain.xmin() )/sideX - 1.0;
	dryCoord[ k ][ 1 ] = 2.0 * ( dryPosition[ k ][ 1 ] - domain.ymin() )/sideY - 1.0;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	wetCoord[ k ][ 0 ] = wetPosition[ k ][ 0 ];
	wetCoord[ k ][ 1 ] = wetPosition[ k ][ 1 ];
	dryCoord[ k ][ 0 ] = dryPosition[ k ][ 0 ];
	dryCoord[ k ][ 1 ] = dryPosition[ k ][ 1 ];
#endif	// ENABLE_MAP_COORD_NORMALIZATION
    }	

    for ( unsigned int k = 0; k < 4; ++k ) {
	cerr << " Dry " << k << " " << dryCoord[ k ][ 0 ] << " , " << dryCoord[ k ][ 1 ] << endl;
    }
    for ( unsigned int k = 0; k < 4; ++k ) {
	cerr << " Wet " << k << " " << wetCoord[ k ][ 0 ] << " , " << wetCoord[ k ][ 1 ] << endl;
    }
#endif	// DISABLE_AREA_RECTANGLES

}
//#endif	// FUKUSHIMA

//------------------------------------------------------------------------------
//	Open GL initialization
//------------------------------------------------------------------------------
void initLights( void )
{
    glMaterialfv( GL_FRONT, GL_AMBIENT,		materialN_ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,		materialN_diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR,	materialN_specular );
    glMaterialfv( GL_FRONT, GL_SHININESS,	materialN_shininess );
    glMaterialfv( GL_FRONT, GL_EMISSION,	materialN_emission );
    
    glLightfv( GL_LIGHT0, GL_AMBIENT,		lightL_ambient );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,		lightL_diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR,		lightL_specular );
    glLightfv( GL_LIGHT0, GL_POSITION,		lightL_position );
    
    glLightfv( GL_LIGHT1, GL_AMBIENT,		lightR_ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE,		lightR_diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR,		lightR_specular );
    glLightfv( GL_LIGHT1, GL_POSITION,		lightR_position );
    
    //glEnable( GL_LIGHTING );
    //glEnable ( GL_LIGHT0 );
    //glEnable ( GL_LIGHT1 );
    // glDisable( GL_LIGHT2 );
    // glDisable( GL_LIGHT3 );
    
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT,	normalLM_ambient );
}



//------------------------------------------------------------------------------
//	Texture generation
//------------------------------------------------------------------------------
void RGYBColor( const double & x, const double & y, const double & z,
		GLubyte & R,
		GLubyte & G,
		GLubyte & B,
		GLubyte & A )
{
    // double xi           = 0.4*(vec[ 0 ]+1.0)+0.2;
    // double eta          = 0.4*(vec[ 1 ]+1.0)+0.2;
    // double lambda       = 0.4*(vec[ 2 ]+1.0)+0.2;
    double eta = y;
    double xi = x;
    // double eta = 0.5 + 0.5 * y;
    // double xi = 0.5 + 0.5 * x;
    double lambda = 0.5 + 0.5 * z;
#ifdef DARK_TEXTURE
    R = ( GLubyte )( 255.0 * lambda*xi );
    G = ( GLubyte )( 255.0 * lambda*eta );
    B = ( GLubyte )( 255.0 * ( 1.0 - max( xi, eta ) ) );
    A = ( GLubyte )255;
#else	// DARK_TEXTURE
    const double base = 63.0;
    const double span = 255.0 - base;
    R = ( GLubyte )( span * lambda*xi + base );
    G = ( GLubyte )( span * lambda*eta + base );
    B = ( GLubyte )( span * ( 1.0 - max( xi, eta ) ) + base );
    A = ( GLubyte )255;
#endif	// DARK_TEXTURE
#ifdef SKIP
    cerr << " x = " << x << " y = " << y
	 << " R = " << R
	 << " G = " << G
	 << " B = " << B
	 << " A = " << A << endl;
#endif	// SKIP
}


void RGYBTexture( const unsigned int & l,
		  const unsigned int & size, 
		  const unsigned int & nW, const unsigned int & nH )
{
    unsigned int itvW = size / nW;
    unsigned int itvH = size / nH;

    // texture for JCN
    for ( unsigned int i = 0; i < nH; ++i ) {
	double y = ( ( double )i+0.5 )/( double )nH;
	for ( unsigned int j = 0; j < nW; ++j ) {
	    double x = ( ( double )j+0.5 )/( double )nW;
	    for ( unsigned p = 0; p < itvH; ++p ) {
		unsigned int indexH = i*itvH + p;
		for ( unsigned q = 0; q < itvW; ++q ) {
		    unsigned int indexW = j*itvW + q;
		    // unsigned int id = indexH * size + indexW;
		    RGYBColor( x, y, 1.0,
			       teximage[ l ][indexH][indexW][0], 
			       teximage[ l ][indexH][indexW][1],
			       teximage[ l ][indexH][indexW][2],
			       teximage[ l ][indexH][indexW][3] );
		}
	    }
	}
    }
}


void initTextures( void )
{
    // texture
    unsigned int k = 0;
    for ( unsigned int p = 0; p < NUM_LEVELS; ++p ) {
	unsigned int sizeP = power( 2, p );
	for ( unsigned int q = 0; q < NUM_LEVELS; ++q ) {
	    unsigned int sizeQ = power( 2, q );
	    RGYBTexture( k, TEXTURE_SIZE, sizeP, sizeQ );
	    k++;
	}
    }
}


//------------------------------------------------------------------------------
//	Sampling functions
//------------------------------------------------------------------------------
double funcP( P_Vertex_handle v )
{
    return v->P();
}

double funcQ( P_Vertex_handle v )
{
    return v->Q();
}

double funcR( P_Vertex_handle v )
{
    return v->R();
}



//------------------------------------------------------------------------------
//	Fundamental functions for JCN construction
//------------------------------------------------------------------------------
void fixRange( Polyhedron & mesh, 
               double & minP, double & maxP, 
	       double & minQ, double & maxQ,
	       double & minR, double & maxR );

void initVars( void )
{
    for ( unsigned int i = 0; i < NUM_VALUES; ++i ) {
	Sampling * ptrS = resolution[ i ];
	for ( Sampling::iterator it = ptrS->begin(); it != ptrS->end(); ++it ) {
	    delete it->second;
	}
	ptrS->clear();
	pixelFacePQ[ i ].clear();
	pixelFacePR[ i ].clear();
	pixelFaceQR[ i ].clear();
	pixelEdgePQ[ i ].clear();
	pixelEdgePR[ i ].clear();
	pixelEdgeQR[ i ].clear();
    }

}


void smallString( float x, float y, float z, const char* string )
{
    // glColor3f( 0.0, 0.0, 0.0 );
    glRasterPos3f( x, y, z );
    char* p = (char*) string;
    while ( *p != '\0' )
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_12, *p++ );
}

void largeString( float x, float y, float z, const char* string )
{
    // glColor3f( 0.0, 0.0, 0.0 );
    glRasterPos3f( x, y, z );
    char* p = (char*) string;
    while ( *p != '\0' )
        glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, *p++ );
}

#ifdef USE_FTGL
void vectorString( float x, float y, float z, int fontsize, const char* string )
{
    // No rotation
    static FTGLPixmapFont * font = NULL;

    glDisable( GL_TEXTURE_2D );

    if ( font == NULL )
	// font = new FTGLPixmapFont( "fonts/cour.ttf" );
	// font = new FTGLPixmapFont( "fonts/comicbd.ttf" );
	font = new FTGLPixmapFont( "fonts/arial.ttf" );

    if ( font->Error() ) {
        cerr << "Cannot open the font file" << endl;
        exit( 1 );          // can't open font file
    }
    if ( ! font->FaceSize( fontsize ) ) {
        cerr << "Cannot set the font size" << endl;
        exit( 1 );              // can't set font size
    }
    if ( ! font->CharMap( ft_encoding_unicode ) ) {
        cerr << "Cannot set the charmap" << endl;
        exit( 1 );              // can't set charmap
    }

    glRasterPos3f( x, y, z );
    font->Render( string );
    glRasterPos3f( x, y, z );
    gl2psText( string, "Helvetica", fontsize );
}
#endif	// USE_FTGL


//------------------------------------------------------------------------------
//	Rasterization
//------------------------------------------------------------------------------
template< class T > void retrievePixels( unsigned int w, unsigned int h, 
                                         T & hash, std::vector< T > & buffer )
{
    //#define DUMP_IMAGES
#ifdef DUMP_IMAGES
    static IplImage *		ptrImage = NULL;	// Mesh image
#endif	// DUMP_IMAGES
    static GLubyte *		pixel	= NULL;

#ifdef DEBUG
    cerr << " Limit of the hash = " << hash.size() << endl;
    for ( unsigned int i = 0; i < hash.size(); ++i ) {
        cerr << HERE << " Hash[ " << setw( 2 ) << i << " ] = " << ends;
        cerr << hash[ i ]->id() << endl;
    }
#endif	// DEBUG

    assert( buffer.size() == w*h );
    // *3 will incur memory problems
    pixel	= new GLubyte [ w*h * 4 ];
    if ( pixel == NULL ) {
        cerr << " cannot allocate the memory" << endl;
        assert( pixel != NULL );
    }
    glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixel );

#ifdef DEBUG
    for ( unsigned int i = 0; i < h; i++ )
        for ( unsigned int j = 0; j < w; j++ ) {
            cerr << HERE << "[ " << setw( 3 ) << i << " , " << setw( 3 ) << j << " ] : ";
            unsigned int index = i*w + j;
            cerr << bitset< 8 >( ( int )pixel[ index*4 ] ) << " ";
            cerr << bitset< 8 >( ( int )pixel[ index*4+1 ] ) << " ";
            cerr << bitset< 8 >( ( int )pixel[ index*4+2 ] ) << " | ";

            for ( unsigned int k = 0; k < buffer[ index ].size(); ++k ) {
                cerr << setw( 5 ) << buffer[ index ][ k ]->id() << " ";
            }
            cerr << endl;
        }
#endif

#ifdef DEBUG
    for ( unsigned int i = 0; i < h; i++ )
        for ( unsigned int j = 0; j < w; j++ ) {
            cerr << HERE << "[ " << setw( 3 ) << i << " , " << setw( 3 ) << j << " ] : ";
            unsigned int index = i*w + j;
            cerr << bitset< 8 >( ( int )pixel[ index*4 ] ) << " ";
            cerr << bitset< 8 >( ( int )pixel[ index*4+1 ] ) << " ";
            cerr << bitset< 8 >( ( int )pixel[ index*4+2 ] ) << " | ";
            for ( unsigned int k = 0; k < buffer[ index ].size(); ++k ) {
                cerr << setw( 5 ) << buffer[ index ][ k ]->id() << " ";
            }
            cerr << endl;
        }
#endif

    for ( unsigned int i = 0; i < h; i++ ) {
        for ( unsigned int j = 0; j < w; j++ ) {
            unsigned int index = i * w + j;
            const GLubyte & R = pixel[ index * 4 ];
            const GLubyte & G = pixel[ index * 4 + 1 ];
            const GLubyte & B = pixel[ index * 4 + 2 ];
            GLubyte maskR = 1, maskG = 1, maskB = 1;
            unsigned int idR = 0, idG = 8, idB = 16;

#ifdef DEBUG
            cerr << " i = " << i << " j = " << j
                 << " pixel = " << ( int )*R << " , "
                 << ( int )*G << " , "
                 << ( int )*B << endl;
#endif	// DEBUG
            // for R
            for ( unsigned int k = 0; k < 8; ++k ) {
#ifdef DEBUG
                cerr << " k = " << k
                     << " R => " << ( int )( R & maskR ) << endl;
#endif	// DEBUG
                if ( ( (int)( R & maskR ) != 0 ) && ( idR < hash.size() ) )
                    buffer[ index ].push_back( hash[ idR ] );
                idR++;
                maskR = maskR << 1;
            }
            // for G
            for ( unsigned int k = 0; k < 8; ++k ) {
#ifdef DEBUG
                cerr << " k = " << k
                     << " G => " << ( int )( G & maskG ) << endl;
#endif	// DEBUG
                if ( ( (int)( G & maskG ) != 0 ) && ( idG < hash.size() ) )
                    buffer[ index ].push_back( hash[ idG ] );
                idG++;
                maskG = maskG << 1;
            }
            // for B
            for ( unsigned int k = 0; k < 8; ++k ) {
#ifdef DEBUG
                cerr << " k = " << k
                     << " B => " << ( int )( B & maskB ) << endl;
#endif	// DEBUG
                if ( ( (int)( B & maskB ) != 0 ) && ( idB < hash.size() ) )
                    buffer[ index ].push_back( hash[ idB ] );
                idB++;
                maskB = maskB << 1;
            }
        }
    }

#ifdef DEBUG
    for ( unsigned int i = 0; i < h; i++ )
        for ( unsigned int j = 0; j < w; j++ ) {
            cerr << "[ " << setw( 3 ) << i << " , " << setw( 3 ) << j << " ] : ";
            unsigned int index = i*w + j;
            cerr << bitset< 8 >( ( int )pixel[ index*4 ] ) << " ";
            cerr << bitset< 8 >( ( int )pixel[ index*4+1 ] ) << " ";
            cerr << bitset< 8 >( ( int )pixel[ index*4+2 ] ) << " | ";
            for ( unsigned int k = 0; k < buffer[ index ].size(); ++k ) {
                cerr << setw( 5 ) << buffer[ index ][ k ]->id() << " ";
            }
            cerr << endl;
        }
#endif	// DEBUG

#ifdef DUMP_IMAGES
    cvReleaseImage( &ptrImage );
    ptrImage = NULL;
#endif

    assert( pixel != NULL );
    delete [] pixel;
    pixel = NULL;
}

void projectFaces( Polyhedron & poly, 
                   unsigned int sz, const P_Bbox2 & box,
		   double (*funcX)( P_Vertex_handle ), double (*funcY)( P_Vertex_handle ),
                   std::vector< VecFace > & buffer )
{
    // const double largestep = 0.012;
    // const double smallstep = 0.002;
    const double largestep = 0.000;
    const double smallstep = 0.000;
    unsigned int w = sz, h = sz;
    // 1.0e-6 does not work..
    // const double eps = 1.0e-5;
    // const double eps = 1.0e-4;
    
    // debugID = 100;
    cerr << HERE << " projectFaces starting ...: " << w << " x " << h << endl;
    cerr << HERE << " bounding box: [ " << box.xmin() << " , "
	 << box.xmax() << " ] x [ " << box.ymin() << " , " << box.ymax() << " ] "
	 << endl;
    const unsigned int number_of_colors = 24;
    buffer.clear();
    buffer.resize( w*h );

    glutSetWindow( win_range );
    glViewport ( 0, 0, w, h );

    glClearColor ( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );

    glDisable( GL_LINE_SMOOTH );

    glLineWidth( 1.0 );
    // glShadeModel( GL_FLAT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( box.xmin(), box.xmax(), box.ymin(), box.ymax() );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    VecFace hash;

    unsigned int counter = 0;	// counter for triangle face IDs
    // unsigned int base = counter;
    double curdepth = 0.01;
    hash.clear();
#ifdef SKIP
    double maxV = -100000.0;
    double minV =  100000.0;
#endif	// SKIP

    for ( P_Facet_iterator fi = poly.facets_begin(); fi != poly.facets_end(); ++fi ) {
	// if ( counter % 100000 == 0 )
	// cerr << HERE << " Projecting Faces : " << counter << endl;

#ifdef SKIP_LAND_CLASSIFICATION
	//------------------------------------------------------------------------------
	// Reject unwanted faces by label
	if ( ! landflag.passByCode( fi->label() ) ) continue;
	if ( ! ( ( ! distFlag ) ||
		 ( distFlag &&
		   ( lowerDistLimit <= fi->dist() ) &&
		   ( fi->dist() <= upperDistLimit ) ) ) ) continue;
	//------------------------------------------------------------------------------
#endif	// SKIP_LAND_CLASSIFICATION

	P_Halfedge_facet_circulator hfc = fi->facet_begin();
        // draw every 8 triangles and set color that can be matched with the
        // correponding face ID
        GLubyte R = 0, G = 0, B = 0;
        // assert( counter == fi->id() );

	unsigned int index = counter % number_of_colors;
        hash.push_back( ( P_Facet_handle )fi );

        // if ( ( 0 <= index ) && ( index <= 7 ) )
        if ( index <= 7 ) {
            R = 1;
            R = R << index;
        }
        else if ( ( 8 <= index ) && ( index <= 15 ) ) {
            G = 1;
            G = G << ( index - 8 );
        }
        else if ( ( 16 <= index ) && ( index <= 23 ) ) {
            B = 1;
            B = B << ( index - 16 );
        }
        glColor4ub( R, G, B, 255 );
#ifdef DEBUG
        cerr << " counter = " << setw( 3 ) << counter << " index = " << setw( 3 ) << index
             << " : R = " << bitset< 8 >( (int)R )
             << " G = " << bitset< 8 >( (int)G )
             << " B = " << bitset< 8 >( (int)B ) << endl;
#endif	// DEBUG

        // #ifdef SKIP
        // Draw each triangle on 2D Plane by just referring to the x and y coordinates
        curdepth += largestep;
        hfc = fi->facet_begin();
        glLineWidth( 5.0 );
        glBegin( GL_POLYGON );
        do {
            double valueX = (*funcX)( hfc->vertex() );
            double valueY = (*funcY)( hfc->vertex() );
#ifdef DEBUG
	    if ( ( valueX < box.xmin() ) || ( valueX > box.xmax() ) ||
		 ( valueY < box.ymin() ) || ( valueY > box.ymax() ) ) {
		cerr << "VID = " << hfc->vertex()->id() <<
		    " (X,Y) = ( " << valueX << " , " << valueY << " ) " << endl;
	    }
#endif	// DEBUG
#ifdef SKIP
	    valueX = min( box.xmax()-eps, max( box.xmin()+eps, valueX ) );
	    valueY = min( box.ymax()-eps, max( box.ymin()+eps, valueY ) );
#endif	// SKIP
#ifdef SKIP
	    if ( funcX == funcQ ) {
		cerr << "VID = " << hfc->vertex()->id() <<
		    " (X,Y) = ( " << valueX << " , " << valueY << " ) " << endl;
	    }
#endif	// SKIP
            glVertex3d( valueX, valueY, curdepth );
        } while ( ++hfc != fi->facet_begin() );
        glEnd();

        // Outline each triangle on 2D Plane by just referring to the x and y coordinates
        hfc = fi->facet_begin();
        curdepth -= smallstep;
        glLineWidth( 1.0 );
        glBegin( GL_LINE_LOOP );
        do {
            double valueX = (*funcX)( hfc->vertex() );
            double valueY = (*funcY)( hfc->vertex() );
#ifdef SKIP
	    valueX = min( box.xmax()-eps, max( box.xmin()+eps, valueX ) );
	    valueY = min( box.ymax()-eps, max( box.ymin()+eps, valueY ) );
#endif	// SKIP
            glVertex3d( valueX, valueY, curdepth );
#ifdef SKIP
            if ( maxV < valueY )  maxV = valueY;
            if ( minV > valueY )  minV = valueY;
#endif	// SKIP
        } while ( ++hfc != fi->facet_begin() );
        glEnd();
        //#endif // SKIP

        hfc = fi->facet_begin();
        glPointSize( 1.0 );
        glBegin( GL_POINTS );
        do {
            double valueX = (*funcX)( hfc->vertex() );
            double valueY = (*funcY)( hfc->vertex() );
            glVertex3d( valueX, valueY, curdepth );
        } while ( ++hfc != fi->facet_begin() );
        glEnd();

        counter++;

        if ( counter % number_of_colors == 0 ) {
#ifdef DEBUG
            cerr << HERE << " base = " << base << endl;
#endif	// DEBUG
            retrievePixels< VecFace >( w, h, hash, buffer );
            // base = counter;
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            curdepth = 0.0;
            hash.clear();
        }
    }
    cerr << "Projecting Faces : " << counter << endl;
    cerr << "Face projection ... Done" << endl;

    retrievePixels< VecFace >( w, h, hash, buffer );
    glViewport ( 0, 0, width_range, height_range );
    glClearColor ( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//    std::cerr<< "min = " << min << " " <<__FILE__<<" " <<__LINE__<<std::endl;
//    std::cerr<< "max = " << max << " " <<__FILE__<<" " <<__LINE__<<std::endl;
}


void projectEdges( Polyhedron & poly, 
                   unsigned int s, const P_Bbox2 & box,
		   double (*funcX)( P_Vertex_handle ), double (*funcY)( P_Vertex_handle ),
                   std::vector< VecHalf > & buffer )
{
    // const double largestep = 0.010;
    const double largestep = 0.000;
    unsigned int w = s, h = s;
    // 1.0e-6 does not work..
    // const double eps = 1.0e-5;

    // debugID = 500;
    cerr << HERE << " projectEdges starting ... " << endl;
    const unsigned int number_of_colors = 24;
    buffer.clear();
    buffer.resize( w*h );

    glutSetWindow( win_range );
    glViewport ( 0, 0, w, h );

    glClearColor ( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE,GL_ONE );

    glDisable( GL_LINE_SMOOTH );

    //glLineWidth( 2.0 );
    glLineWidth( 5.0 );
    // glShadeModel( GL_FLAT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( box.xmin(), box.xmax(), box.ymin(), box.ymax() );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    VecHalf hash;

    unsigned int counter = 0;	// counter for edge IDs
    // unsigned int base = counter;
    // double curdepth = 0.0;
    double curdepth = 0.001;
    hash.clear();
    for ( P_Halfedge_iterator hi = poly.halfedges_begin(); hi != poly.halfedges_end(); ++hi ) {
	// if ( counter % 100000 == 0 )
	// cerr << HERE << " Projecting Edges : " << counter << endl;

        GLubyte R = 0, G = 0, B = 0;
#ifdef DEBUG
        cerr << HERE << "############################## halfedge ID = " << hi->id() << endl;
#endif	// DEBUG
        // Project half of the halfedges to avoid eventually projecting the same edge twice
	if ( hi->vertex()->id() > hi->opposite()->vertex()->id() ) {
	    // if ( hi->id() != counter )
#ifdef DEBUG
            cerr << HERE << " Skipped: ID = " << hi->id() << endl;
#endif	// DEBUG
            continue;
        }
        // assert( counter == hi->id() );

        unsigned int index = counter % number_of_colors;
        hash.push_back( ( P_Halfedge_handle )hi );

        // if ( ( 0 <= index ) && ( index <= 7 ) )
        if ( index <= 7 ) {
            R = 1;
            R = R << index;
        }
        else if ( ( 8 <= index ) && ( index <= 15 ) ) {
            G = 1;
            G = G << ( index - 8 );
        }
        else if ( ( 16 <= index ) && ( index <= 23 ) ) {
            B = 1;
            B = B << ( index - 16 );
        }
        glColor4ub( R, G, B, 255 );

        P_Vertex_handle orig = hi->vertex();
        P_Vertex_handle dest = hi->opposite()->vertex();
        double valuePO = (*funcX)( orig );
        double valueQO = (*funcY)( orig );
        double valuePD = (*funcX)( dest );
        double valueQD = (*funcY)( dest );
#ifdef SKIP
	valuePO = min( box.xmax()-eps, max( box.xmin()+eps, valuePO ) );
	valuePD = min( box.xmax()-eps, max( box.xmin()+eps, valuePD ) );
	valueQO = min( box.ymax()-eps, max( box.ymin()+eps, valueQO ) );
	valueQD = min( box.ymax()-eps, max( box.ymin()+eps, valueQD ) );
#endif	// SKIP

        glLineWidth( 2.0 );
        curdepth += largestep;
        glBegin( GL_LINES );
        glVertex3d( valuePO, valueQO, curdepth );
        glVertex3d( valuePD, valueQD, curdepth );
        glEnd();

        glPointSize( 1.0 );
        glBegin( GL_POINTS );
        glVertex3d( valuePO, valueQO, curdepth );
        glVertex3d( valuePD, valueQD, curdepth );
        glEnd();


        counter++;

        if ( counter % number_of_colors == 0 ) {
#ifdef DEBUG
            cerr << HERE << " base = " << base << endl;
#endif	// DEBUG
            retrievePixels< VecHalf >( w, h, hash, buffer );
            // base = counter;
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
            curdepth = 0.0;
            hash.clear();
        }
    }
    cerr << "Projecting Edges : " << counter << endl;
    cerr << HERE << " Edge projection ... Done" << endl;

    retrievePixels< VecHalf >( w, h, hash, buffer );
    glViewport ( 0, 0, width_range, height_range );
    glClearColor ( 0.0, 0.0, 0.0, 0.0 );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


void adaptiveSampling( const Tuple3 & key, Polyhedron & mesh )
{
#ifdef SKIP
    unsigned int sum = 0;
    for ( unsigned k = 0; k < pixelFacePQ[ valueID ].size(); ++k )
	sum += pixelFacePQ[ valueID ][ k ].size();
    cerr << HERE << " number of projected face fragments onto PQ = " << sum << endl;
#endif	// SKIP
    
    //------------------------------------------------------------------------------
    //	Downsampling
    //------------------------------------------------------------------------------
    Sampling * ptrS = resolution[ valueID ];
    Sampling::iterator it = ptrS->find( key );
    if ( ( it == ptrS->end() ) || switchFlag ) {
	if ( ( it != ptrS->end() ) && switchFlag ) {
	    ptrS->erase( it );
	}
	switchFlag = false;
	Level * ptrLevel = new Level;
	std::vector< VecFace > & vvfPQ = pixelFacePQ[ valueID ];
	std::vector< VecFace > & vvfPR = pixelFacePR[ valueID ];
	std::vector< VecFace > & vvfQR = pixelFaceQR[ valueID ];
	std::vector< VecHalf > & vvhPQ = pixelEdgePQ[ valueID ];
	std::vector< VecHalf > & vvhPR = pixelEdgePR[ valueID ];
	std::vector< VecHalf > & vvhQR = pixelEdgeQR[ valueID ];
	cerr << endl << HERE << " resolution => "
	     << setw( 3 ) << key.x() << "x"
	     << setw( 3 ) << key.y() << "x"
	     << setw( 3 ) << key.z() << " : " << ends;
	cerr << "*" << ends;
	unsigned int resP = key.x();
	unsigned int resQ = key.y();
	unsigned int resR = key.z();

	unsigned int nFaces = mesh.size_of_facets();
	// unsigned int sZ = it->first.z();
	//------------------------------------------------------------------------------
	//	PQ	
	cerr << "1" << ends;
	aggregate	( RASTER_RESOLUTION, RASTER_RESOLUTION,
			  resP, resQ, vvfPQ, vvhPQ,
			  ptrLevel->facePQ(), ptrLevel->edgePQ(),
			  nFaces, ptrLevel->projPQ() );
	cerr << "1" << ends;
	groupFaces	( resP, resQ,
			  ptrLevel->facePQ(), ptrLevel->edgePQ(),
			  ptrLevel->compPQ() );
	cerr << "1" << ends;
	constructJCN	( resP, resQ, boundPQ,
			  ptrLevel->compPQ(), ptrLevel->vertexPQ(),
			  ptrLevel->netPQ() );
	cerr << "1" << ends;
#ifdef TENTATIVE_SKIP
	fiberDetector	( resP, resQ,
			  ptrLevel->vertexPQ(),
			  ptrLevel->netPQ(),
			  ptrLevel->fiberPQ(),
			  ptrLevel->singularPQ() );
#endif	// TENTATIVE_SKIP
	//------------------------------------------------------------------------------
	//	PR	
	cerr << "2" << ends;
	aggregate	( RASTER_RESOLUTION, RASTER_RESOLUTION,
			  resP, resR, vvfPR, vvhPR,
			  ptrLevel->facePR(), ptrLevel->edgePR(),
			  nFaces, ptrLevel->projPR() );
	cerr << "2" << ends;
	groupFaces	( resP, resR,
			  ptrLevel->facePR(), ptrLevel->edgePR(),
			  ptrLevel->compPR() );
	cerr << "2" << ends;
	constructJCN	( resP, resR, boundPR,
			  ptrLevel->compPR(), ptrLevel->vertexPR(),
			  ptrLevel->netPR() );
	cerr << "2" << ends;
#ifdef TENTATIVE_SKIP
	fiberDetector	( resP, resR,
			  ptrLevel->vertexPR(),
			  ptrLevel->netPR(),
			  ptrLevel->fiberPR(),
			  ptrLevel->singularPR() );
#endif	// TENTATIVE_SKIP
	//------------------------------------------------------------------------------
	//	QR	
	cerr << "3" << ends;
	aggregate	( RASTER_RESOLUTION, RASTER_RESOLUTION,
			  resQ, resR, vvfQR, vvhQR,
			  ptrLevel->faceQR(), ptrLevel->edgeQR(),
			  nFaces, ptrLevel->projQR() );
	cerr << "3" << ends;
	groupFaces	( resQ, resR,
			  ptrLevel->faceQR(), ptrLevel->edgeQR(),
			  ptrLevel->compQR() );
	cerr << "3" << ends;
	constructJCN	( resQ, resR, boundQR,
			  ptrLevel->compQR(), ptrLevel->vertexQR(),
			  ptrLevel->netQR() );
	cerr << "3" << ends;
#ifdef TENTATIVE_SKIP
	fiberDetector	( resQ, resR,
			  ptrLevel->vertexQR(),
			  ptrLevel->netQR(),
			  ptrLevel->fiberQR(),
			  ptrLevel->singularQR() );
#endif	// TENTATIVE_SKIP

	//------------------------------------------------------------------------------
	//	post-process
	ptrLevel->res() = key;
	pair< Tuple3, Level* > newlevel( key, ptrLevel );
	ptrS->insert( newlevel );
	curLevel = ptrLevel;
    }
    else {
	curLevel = it->second;
    }

    cerr << HERE
	 << " P = " << curLevel->res().x()
	 << " Q = " << curLevel->res().y()
	 << " R = " << curLevel->res().z()
	 << endl;
    cerr << "\a\a\a" << ends;

#ifdef DEBUG
    cerr << " Enter the key ... ";
    getchar();
#endif	// DEBUG
}


void rasterization( void )
{
#ifdef SKIP
    unsigned int sum = 0;
    for ( unsigned k = 0; k < pixelFacePQ[ valueID ].size(); ++k )
	sum += pixelFacePQ[ valueID ][ k ].size();
    cerr << HERE << " number of projected face fragments onto PQ = " << sum << endl;
#endif	// SKIP

    Polyhedron & curMesh = meshMap;
    //------------------------------------------------------------------------------
    //	Rasterization
    //------------------------------------------------------------------------------
    if ( ! isProjected[ valueID ] ) {
	cerr << HERE << "******** current time step = " << valueID << endl;
	cerr << "Projecting onto PQ-range space" << endl;
	projectFaces( curMesh, RASTER_RESOLUTION, boundPQ, funcP, funcQ, pixelFacePQ[ valueID ] );
	projectEdges( curMesh, RASTER_RESOLUTION, boundPQ, funcP, funcQ, pixelEdgePQ[ valueID ] );
	cerr << "Projecting onto PR-range space" << endl;
	projectFaces( curMesh, RASTER_RESOLUTION, boundPR, funcP, funcR, pixelFacePR[ valueID ] );
	projectEdges( curMesh, RASTER_RESOLUTION, boundPR, funcP, funcR, pixelEdgePR[ valueID ] );
	cerr << "Projecting onto QR-range space" << endl;
	projectFaces( curMesh, RASTER_RESOLUTION, boundQR, funcQ, funcR, pixelFaceQR[ valueID ] );
	projectEdges( curMesh, RASTER_RESOLUTION, boundQR, funcQ, funcR, pixelEdgeQR[ valueID ] );
	isProjected[ valueID ] = true;
    }
}

//------------------------------------------------------------------------------
//	Open GL initialization
//------------------------------------------------------------------------------
void initShape( void )
{
    // glClearColor ( 0.8, 0.8, 0.8, 0.0 );
    glClearColor ( 1.0, 1.0, 1.0, 0.0 );
    /* glShadeModel ( GL_FLAT ); */
    /* glEnable( GL_LIGHTING ); */
    /* glEnable( GL_AUTO_NORMAL ); */
    /* glEnable( GL_NORMALIZE ); */

    /* Enable Z-buffer */
    glEnable( GL_AUTO_NORMAL );
    glEnable( GL_NORMALIZE );
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_NORMALIZE );
    glDepthFunc( GL_LESS );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fovy, aspect, near, far );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}


void initRange( void )
{
    glClearColor ( 0.0, 0.0, 0.0, 0.0 );
    /* glShadeModel ( GL_FLAT ); */
    /* glEnable( GL_LIGHTING ); */
    /* glEnable( GL_AUTO_NORMAL ); */
    /* glEnable( GL_NORMALIZE ); */

    /* Enable Z-buffer */
    // glEnable( GL_AUTO_NORMAL );
    // glEnable( GL_NORMALIZE );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );

    // glClearStencil(0x0);
}

//------------------------------------------------------------------------------
//	Drawing functions
//------------------------------------------------------------------------------
void HSVtoRGB( double normalizedCur, std::vector<double> & rgb )
{
    double H,S,V,R,G,B,p,q,t,Hi;
    double F;
    int Red, Green, Blue;
    H = normalizedCur;
    S = 200.0;
    V = 200.0;
    
    if ( S == 0 ) {
        Red   = V;
        Green = V;
        Blue  = V;
    }
    
    if ( S != 0 ) {
        Hi = floor(H / 60);
        F = H/60 - Hi;
        p = V*(1-S/255);
        q = V*(1-S*F/255);
        t = V*(1-S*(1-F)/255);

        if(Hi==0){
            R = V;
            G = t;
            B = p;
        }

        if(Hi == 1){
            R = q;
            G = V;
            B = p;
        }

        if(Hi == 2){
            R = p;
            G = V;
            B = t;
        }

        if(Hi == 3){
            R = p;
            G = q;
            B = V;
        }

        if(Hi == 4){
            R = t;
            G = p;
            B = V;
        }

        if(Hi == 5){
            R = V;
            G = p;
            B = q;
        }

        Red   = floor(R+0.5);
        Green = floor(G+0.5);
        Blue  = floor(B+0.5);
    }

    rgb.push_back( Red   );
    rgb.push_back( Green );
    rgb.push_back( Blue  );
}


void colorMap( double cur, double minV, double maxV, std::vector< double > & rgb )
{
    double normalizedCur = 0.0;
    normalizedCur = ( cur - minV ) / ( maxV - minV );
    normalizedCur = fabs( ( normalizedCur * 240 ) - 240 );
    HSVtoRGB( normalizedCur, rgb );
}


void curColor( int size, P_Bbox2 box, double value, double & HSVcolor )
{
    double cColor = box.ymin();//-2*M_PI;
    unsigned int count = 0;
    double range = box.ymax() - box.ymin();
    while( 1 ){
        cColor += ( range / size );
        if( cColor >= value )    break;
        ++count;
    }
    HSVcolor = 240.0 - ( ( 240.0 / ( MAX2( ( double )(size-1), 1.0 ) ) ) * (double)count );
}


//------------------------------------------------------------------------------
//	Individual functions for main window
//------------------------------------------------------------------------------
void coordinate( void )
{
    const int spacing = 5;	// 5 minutes
    const double depth = FRONT_DEPTH + 0.01;

    // for enabling antialiasing
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    // glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBlendFunc(GL_ONE, GL_ZERO); // <= overwrite
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glDisable( GL_LIGHTING );

    glLineWidth( 1.0 );
    gl2psLineWidth( 1.0 );
    
    double cenx = 0.5 * ( domain.xmin() + domain.xmax() );
    double ceny = 0.5 * ( domain.ymin() + domain.ymax() );
    double sidex = domain.xmax() - cenx;
    double sidey = domain.ymax() - ceny;

    // double limitL = std::max( -4.0, -translate_x - 1.0/map_scale );
    // double limitR = std::min(  4.0, -translate_x + 1.0/map_scale );
#ifdef ENABLE_MAP_COORD_NORMALIZATION
    double limitL = std::max( -4.0, ( -1.0 - translate_x ) / map_scale );
    double limitR = std::min(  4.0, (  1.0 - translate_x ) / map_scale );
#else	// ENABLE_MAP_COORD_NORMALIZATION
    const double shrink_x = 0.86;
    const double annotate_x = 0.98;
    const double offset_y = 0.02;
    double limitL = translate_x - scale_x * WINDOW_SIDE;
    // double limitR = translate_x + shrink_x * scale_x * WINDOW_SIDE;
    double limitR = translate_x + shrink_x * scale_x * WINDOW_SIDE;
    double limitP = translate_x + annotate_x * scale_x * WINDOW_SIDE;
    double offsetP = offset_y * scale_y * WINDOW_SIDE;
#endif	// ENABLE_MAP_COORD_NORMALIZATION
    // cerr << HERE << " limitL = " << limitL << " limitR = " << limitR << endl;
   
    for ( int deg = 36; deg <= 39; ++deg ) {
	for ( int min = 0; min < 60; min += spacing ) {
	    double lat = ( double )deg + ( double )min/60.0;
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double y = ( lat - ceny ) / sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double y = lat;
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    if ( display_mode == 2 )
		glColor3d( 1.0, 0.5, 0.0 );
	    else 
		glColor3d( LL_LINE_COLOR );
	    
	    glEnable( GL_LINE_STIPPLE );
	    glLineStipple( 1, 0xC3C3 );
	    glBegin( GL_LINES );
	    glVertex3d( limitL, y, depth );
	    glVertex3d( limitR, y, depth );
	    glEnd();
	    glDisable( GL_LINE_STIPPLE );
	    
	    glColor3d( 0.0, 0.0, 1.0 );
	    ostringstream ostr;
	    ostr << boost::format( "%2d°%02d'" ) % deg % min << ends;
#ifdef USE_FTGL
	    // vectorString( limitP, y-offsetP, depth, 24, ostr.str().c_str() );
	    vectorString( limitR, y-offsetP, depth, 24, ostr.str().c_str() );
#else	// USE_FTGL
	// largeString( 0.9*sidex + cenx, (double)lat, depth, ostr.str().c_str() );
	    largeString( limitP, y-offsetP, depth, ostr.str().c_str() );
#endif	// USE_FTGL
	}
    }

#ifdef ENABLE_MAP_COORD_NORMALIZATION
    double limitT = std::min(  4.0, (  1.0 - translate_y ) / map_scale );
    double limitB = std::max( -4.0, ( -1.0 - translate_y ) / map_scale );
    // cerr << HERE << " limitB = " << limitB << " limitT = " << limitT << endl;
#else	// ENABLE_MAP_COORD_NORMALIZATION
    const double shrink_y = 0.95;
    const double annotate_y = 0.98;
    const double offset_x = 0.07;
    double limitT = translate_y - shrink_y * scale_y * WINDOW_SIDE;
    // double limitB = translate_y + shrink_y * scale_y * WINDOW_SIDE;
    double limitB = translate_y + scale_y * WINDOW_SIDE;
    double limitQ = translate_y - annotate_y * scale_y * WINDOW_SIDE;
    double offsetQ = offset_x * scale_x * WINDOW_SIDE;
    // cerr << HERE << " limitB = " << limitB << " limitT = " << limitT << endl;
#endif	// ENABLE_MAP_COORD_NORMALIZATION
    
    for ( int deg = 139; deg <= 141; ++deg ) {
	for ( int min = 0; min < 60; min += spacing ) {
	    double lon = ( double )deg + ( double )min/60.0;
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( lon - cenx ) / sidex;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = lon;
#endif	// ENABLE_MAP_COORD_NORMALIZATION

	    if ( display_mode == 2 )
		glColor3d( 1.0, 0.5, 0.0 );
	    else 
		glColor3d( LL_LINE_COLOR );

	    glEnable( GL_LINE_STIPPLE );
	    glLineStipple( 1, 0xC3C3 );
	    glBegin( GL_LINES );
	    glVertex3d( x, limitB, depth );
	    glVertex3d( x, limitT, depth );
	    glEnd();
	    glDisable( GL_LINE_STIPPLE );

	    glColor3d( 0.0, 0.0, 1.0 );
	    ostringstream ostr;
	    ostr << boost::format( "%3d°%02d'" ) % deg % min << ends;
#ifdef USE_FTGL
	    if ( display_mode == 2 )
		vectorString( x, limitQ, depth, 24, ostr.str().c_str() );
	    else
		vectorString( x-offsetQ, limitQ, depth, 24, ostr.str().c_str() );
#else	// USE_FTGL
	    largeString( x-0.05, limitT, depth, ostr.str().c_str() );
#endif	// USE_FTGL
	}
    }

    // for disabling antialiasing
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );

    return;
}


void boundary( void )
{
    const double depth = FRONT_DEPTH;

    // glColor3d( 0.0, 0.0, 0.0 );

#ifdef SKIP
    // for enabling antialiasing
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
#endif	// SKIP

#ifdef SKIP
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( domain.xmin(), domain.xmax(), domain.ymin(), domain.ymax() );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
#endif	// SKIP

    double cenx = 0.5 * ( domain.xmin() + domain.xmax() );
    double ceny = 0.5 * ( domain.ymin() + domain.ymax() );
    double sidex = domain.xmax() - cenx;
    double sidey = domain.ymax() - ceny;

    // glColor3d( 1.0, 0.0, 0.0 );
    glLineWidth( 1.0 );
    //glColor3d( 0.3, 0.5, 0.7 );
    glColor3d( 0.2, 0.2, 0.2 );
    std::vector< Polyline > & border07 = fukushima->border();
    for ( unsigned int i = 0; i < border07.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < border07[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( border07[ i ][ j ].x() - cenx )/sidex;
	    double y = ( border07[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = border07[ i ][ j ].x();
	    double y = border07[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }

#ifdef SKIP
    glLineWidth( 1.0 );
    glColor3d( 0.6, 0.6, 0.6 );
    std::vector< Polyline > & border03 = iwate->border();
    for ( unsigned int i = 0; i < border03.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < border03[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( border03[ i ][ j ].x() - cenx )/sidex;
	    double y = ( border03[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = border03[ i ][ j ].x();
	    double y = border03[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }
#endif	// SKIP

    glLineWidth( 1.0 );
    glColor3d( 0.6, 0.6, 0.6 );
    std::vector< Polyline > & border04 = miyagi->border();
    for ( unsigned int i = 0; i < border04.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < border04[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( border04[ i ][ j ].x() - cenx )/sidex;
	    double y = ( border04[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = border04[ i ][ j ].x();
	    double y = border04[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }

#ifdef SKIP
    glLineWidth( 1.0 );
    glColor3d( 0.6, 0.6, 0.6 );
    std::vector< Polyline > & border06 = yamagata->border();
    for ( unsigned int i = 0; i < border06.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < border06[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( border06[ i ][ j ].x() - cenx )/sidex;
	    double y = ( border06[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = border06[ i ][ j ].x();
	    double y = border06[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }
#endif	// SKIP

    glLineWidth( 1.0 );
    glColor3d( 0.6, 0.6, 0.6 );
    std::vector< Polyline > & border08 = ibaraki->border();
    for ( unsigned int i = 0; i < border08.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < border08[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( border08[ i ][ j ].x() - cenx )/sidex;
	    double y = ( border08[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = border08[ i ][ j ].x();
	    double y = border08[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }

    glLineWidth( 1.0 );
    glColor3d( 0.6, 0.6, 0.6 );
    std::vector< Polyline > & border09 = tochigi->border();
    for ( unsigned int i = 0; i < border09.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < border09[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( border09[ i ][ j ].x() - cenx )/sidex;
	    double y = ( border09[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = border09[ i ][ j ].x();
	    double y = border09[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }

    glLineWidth( 1.0 );
    glColor3d( 0.8, 0.8, 0.8 );
    std::vector< Polyline > & border10 = gunma->border();
    for ( unsigned int i = 0; i < border10.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < border10[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( border10[ i ][ j ].x() - cenx )/sidex;
	    double y = ( border10[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = border10[ i ][ j ].x();
	    double y = border10[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }

#ifdef SKIP
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();
#endif	// SKIP

#ifdef SKIP
    // for disabling antialiasing
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
#endif	// SKIP
}


void network( void )
{
    const double depth = FRONT_DEPTH;
    double cenx = 0.5 * ( domain.xmin() + domain.xmax() );
    double ceny = 0.5 * ( domain.ymin() + domain.ymax() );
    double sidex = domain.xmax() - cenx;
    double sidey = domain.ymax() - ceny;

    glDisable( GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );

    // glLineWidth( 2.0 );
    glLineWidth( 3.0 );
    // glColor3d( 0.4, 0.4, 0.4 );
    // glColor3d( 0.7, 0.7, 0.7 );
    glColor3d( 0.6, 0.6, 0.6 );
    std::vector< Polyline > & road07 = fukushima->road();
    for ( unsigned int i = 0; i < road07.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < road07[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( road07[ i ][ j ].x() - cenx )/sidex;
	    double y = ( road07[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = road07[ i ][ j ].x();
	    double y = road07[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	    // cerr << HERE << " i = " << i << " j = " << j << " x = " << x << " y = " << y << endl;
	}
	glEnd();
    }
}


void brook( void )
{
    const double depth = FRONT_DEPTH;
    double cenx = 0.5 * ( domain.xmin() + domain.xmax() );
    double ceny = 0.5 * ( domain.ymin() + domain.ymax() );
    double sidex = domain.xmax() - cenx;
    double sidey = domain.ymax() - ceny;

    glDisable( GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    
    // glLineWidth( 5.0 );
    // glLineWidth( 3.0 );
    glLineWidth( 2.0 );
    glColor3d( 0.0, 0.6, 0.6 );
    // glColor3d( 0.0, 0.8, 0.8 );
    // glColor3d( 1.0, 0.0, 0.0 );
    // glColor4d( 1.0, 0.0, 0.0, 1.0 );
    std::vector< Polyline > & brook07 = fukushima->river();
    for ( unsigned int i = 0; i < brook07.size(); ++i ) {
	glBegin( GL_LINE_STRIP );
	for ( unsigned int j = 0; j < brook07[ i ].size(); ++j ) {
#ifdef ENABLE_MAP_COORD_NORMALIZATION
	    double x = ( brook07[ i ][ j ].x() - cenx )/sidex;
	    double y = ( brook07[ i ][ j ].y() - ceny )/sidey;
#else	// ENABLE_MAP_COORD_NORMALIZATION
	    double x = brook07[ i ][ j ].x();
	    double y = brook07[ i ][ j ].y();
#endif	// ENABLE_MAP_COORD_NORMALIZATION
	    glVertex3d( x, y, depth );
	}
	glEnd();
    }
}


#ifndef DISABLE_AREA_RECTANGLES
void rectangle( void )
{
    glColor3d( 0.2, 0.2, 0.2 );
    glLineWidth( 3.0 );

    // cerr << HERE << " Drawing wet & dry rectangles >>>" << endl;

#ifdef SKIP
    // reference area
    glBegin( GL_LINE_LOOP );
    for ( unsigned int k = 0; k < 4; ++k )
	glVertex2d( -0.5, -0.5 );
	glVertex2d(  0.5, -0.5 );
	glVertex2d(  0.5,  0.5 );
	glVertex2d( -0.5,  0.5 );
    glEnd();
#endif	// SKIP
    
    // dry area
    if ( dryBit ) {
	glBegin( GL_LINE_LOOP );
	for ( unsigned int k = 0; k < 4; ++k )
	    glVertex2dv( dryCoord[ k ] );
	glEnd();
    }

    // wet area
    if ( wetBit ) {
	glBegin( GL_LINE_LOOP );
	for ( unsigned int k = 0; k < 4; ++k )
	    glVertex2dv( wetCoord[ k ] );
	glEnd();
    }
}
#endif	// DISABLE_AREA_RECTANGLES

    
void plot( Polyhedron & mesh )
{
    // for enabling antialiasing
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glDisable( GL_LIGHTING );

    glEnable( GL_POINT_SMOOTH );

    glColor3d( 0.0, 0.0, 0.0 );
    glPointSize( 5.0 );

    glBegin( GL_POINTS );
    for ( P_Vertex_iterator vi = mesh.vertices_begin(); vi != mesh.vertices_end(); ++vi ) {
        glVertex3d( vi->point().x(),
                    vi->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
                    0.0
#else	// ZERO_HEIGHT_PROJECTION
                    vi->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
		    );
    }
    glEnd();

    // for disabling antialiasing
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );

    return;
}    

void wireframe( Polyhedron & mesh )
{
    // for enabling antialiasing
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glDisable( GL_LIGHTING );
    // glColor3d( 0.0, 0.0, 0.0 );
    glColor3d( 0.5, 0.5, 0.5 );
    glLineWidth( 1.0 );
    gl2psLineWidth( 1.0 );
    for ( P_Halfedge_iterator hi = mesh.halfedges_begin(); hi != mesh.halfedges_end(); ++hi ) {
	P_Vertex_handle vhO = hi->vertex();
	P_Vertex_handle vhD = hi->opposite()->vertex();
	if ( vhO->id() < vhD->id() ) {
	    glBegin( GL_LINES );
	    glVertex3d( vhO->point().x(),
			vhO->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
			0.0
#else	// ZERO_HEIGHT_PROJECTION
			vhO->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
			);
	    glVertex3d( vhD->point().x(),
			vhD->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
			0.0
#else	// ZERO_HEIGHT_PROJECTION
			vhD->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
			);
	    glEnd();
	}
    }

    // for disabling antialiasing
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );

    // post-process
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();
        hfc = fi->facet_begin();
	unsigned int nEdges = 0;
        do {
            nEdges++;
        } while ( ++hfc != fi->facet_begin() );
	if ( nEdges > 3 ) {
	    cerr << " Face ID : " << fi->id() << " Number of edges = " << nEdges << endl;
	}
    }

    glColor3d( 0.8, 0.8, 0.8 );
    glEnable( GL_LIGHTING );
    
    return;
}    


void hidden( Polyhedron & mesh )
{
    // for enabling antialiasing
    glEnable( GL_LINE_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

    glDisable( GL_LIGHTING );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();

        // Facets in polyhedral surfaces are at least triangles.
        CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );

        // draw boundary lines of faces in black
        glColor3d( 0.0, 0.0, 0.0 );
        glLineWidth( 1.0 );
        glPointSize( 1.0 );
        glBegin( GL_POLYGON );
        do {
            glVertex3d( hfc->vertex()->point().x(),
                        hfc->vertex()->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
                        0.0
#else	// ZERO_HEIGHT_PROJECTION
                        hfc->vertex()->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
			);
        } while ( ++hfc != fi->facet_begin() );
        glEnd();
    }
    // for disabling antialiasing
    glDisable( GL_LINE_SMOOTH );
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable( GL_POLYGON_OFFSET_FILL );
    // glPolygonOffset( 1.0, 1.0 );
    glPolygonOffset( 0.0, 0.1 );

    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();
        // Facets in polyhedral surfaces are at least triangles.
        CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );
        glColor3d( 0.8, 0.8, 0.8 );
        glBegin( GL_POLYGON );
        do {
            glVertex3d( hfc->vertex()->point().x(),
                        hfc->vertex()->point().y(),
                        hfc->vertex()->point().z() );
        } while ( ++hfc != fi->facet_begin() );
        glEnd();
    }
    glDisable( GL_POLYGON_OFFSET_FILL );
    return;
}    


void flat( Polyhedron & mesh )
{
    glEnable( GL_LIGHTING );
    initLights();
    
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();
	P_Facet_handle fh = fi;

	// Facets in polyhedral surfaces are at least triangles.
        CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );

        // draw boundary lines of faces in black
	P_Vector3 normal = fh->faceEq().orthogonal_vector();
        glBegin( GL_POLYGON );
        do {
	    glNormal3d( normal.x(), normal.y(), normal.z() );
            glVertex3d( hfc->vertex()->point().x(),
                        hfc->vertex()->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
                        0.0
#else	// ZERO_HEIGHT_PROJECTION
                        hfc->vertex()->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
			);
        } while ( ++hfc != fi->facet_begin() );
        glEnd();
    }

    return;
}    


void texture2D( Polyhedron & mesh, P_Bbox2 & box,
		double (*funcX)( P_Vertex_handle ), 
		double (*funcY)( P_Vertex_handle ) )
{
    const double eps = 1.0e-4;
    glDisable( GL_LIGHTING );
    glShadeModel( GL_FLAT );
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_DEPTH_TEST );

    unsigned int powerX = ( unsigned int )round( log2( curLevel->res().x() ) );
    unsigned int powerY = ( unsigned int )round( log2( curLevel->res().y() ) );
    unsigned int resID = powerX * NUM_LEVELS + powerY;

    cerr << HERE << " powerX = " << powerX << " powerY = " << powerY << " resID = " << resID << endl;

    unsigned texsize = TEXTURE_SIZE;
    Zone & mytex = teximage[ resID ];

    /* テクスチャ画像はバイト単位に詰め込まれている */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
    /* テクスチャの割り当て */
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texsize, texsize, 0,
		  GL_RGBA, GL_UNSIGNED_BYTE, mytex);
    
    /* テクスチャを拡大・縮小する方法の指定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    // Note: We need to call glTexEnvi with GL_REPLACE
    // GL_MODULATE is the default for glTexEnvi, but this cannot produce
    // good results
    // noted by ST on 2023/01/28
    //glTexEnvi	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
    //glTexEnvi	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    glTexEnvf	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

#ifdef SKIP_TENTATIVE_CODE
    glBindTexture	( GL_TEXTURE_2D, texid[ resID ] );
#endif	// SKIP_TENTATIVE_CODE

#ifdef ENABLE_R_VALUE_COLORING
    interval< double > range_R_value( LIMIT_R_INTERVAL_MIN, LIMIT_R_INTERVAL_MAX );
    string histname( "histogram.csv" );
    cerr << HERE << " Saving the samples of the target value " << endl;
    ofstream histofs( histname.c_str() );
    if ( !histofs ) {
	cerr << HERE << " Cannot open the file : " << histname.c_str() << endl;
	exit( -1 );
    }
#endif	// ENABLE_R_VALUE_COLORING
    
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();
	P_Facet_handle fh = fi;
        // Facets in polyhedral surfaces are at least triangles.
        CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );

	// draw boundary lines of faces in black
#ifdef ENABLE_R_VALUE_COLORING
	double R_value = 0.0;
	bool R_inside = true;
#endif	// ENABLE_R_VALUE_COLORING
	P_Vector3 normal = fh->faceEq().orthogonal_vector();
	glBegin( GL_POLYGON );
	do {
	    double paramP = ( (*funcX)( hfc->vertex() ) - box.xmin() )/( box.xmax() - box.xmin() ); 
	    double paramQ = ( (*funcY)( hfc->vertex() ) - box.ymin() )/( box.ymax() - box.ymin() ); 

//------------------------------------------------------------------------------
//	
	    double tP = funcP( hfc->vertex() );
	    double tQ = funcQ( hfc->vertex() );
	    double tR = funcR( hfc->vertex() );
	    double adjustP = rangeP.lower() * ( 1.0 - tP ) + rangeP.upper() * tP;
	    double adjustQ = rangeQ.lower() * ( 1.0 - tQ ) + rangeQ.upper() * tQ;
	    double adjustR = rangeR.lower() * ( 1.0 - tR ) + rangeR.upper() * tR;
	    
//------------------------------------------------------------------------------
#ifdef DEBUG_FOR_TRACKING_FEATURE_VALUES
//------------------------------------------------------------------------------
	    const double targetX = 140.547040;
	    const double targetY = 37.851180;
	    const double eps = 0.0001;
	    // const double eps = 0.001;

	    if ( ( abs( hfc->vertex()->point().x() - targetX ) < eps ) && 
		 ( abs( hfc->vertex()->point().y() - targetY ) < eps ) ) {

		cerr << HERE << "##############################" << endl;

		cerr << HERE << "TARGET ( "
		     << fixed << setprecision( 8 ) << targetX
		     << " , "
		     << fixed << setprecision( 8 ) << targetY
		     << " ) " << endl;
		cerr << HERE << "REFERENCE ( "
		     << fixed << setprecision( 8 ) << hfc->vertex()->point().x()
		     << " , "
		     << fixed << setprecision( 8 ) << hfc->vertex()->point().y()
		     << " ) " << endl;
		cerr << HERE << " Found the plot!!" << endl;

		cerr << HERE
		     << " funcP = " << tP << " funcQ = " << tQ << " funcR = " << tR
		     << " funcY = " << (*funcY)( hfc->vertex() )
		     << " box.ymin = " << box.ymin() << " box.ymax = " << box.ymax() << endl;
		cerr << HERE
		     << " rangeP : " << rangeP
		     << " rangeQ : " << rangeQ
		     << " rangeR : " << rangeR << endl;

		cerr << HERE
		     << " adjustP : " << adjustP
		     << " adjustQ : " << adjustQ
		     << " adjustR : " << adjustR
		     << endl;
//------------------------------------------------------------------------------
		//getchar();
	    }
#endif	// DEBUG_FOR_TRACKING_FEATURE_VALUES


	    // This adjustment is necessary
#ifdef DISABLE_THIRD_TEXTURING
	    paramP = max( eps, min( 1.0-eps, paramP ) );
	    paramQ = max( eps, min( 1.0-eps, paramQ ) );
	    // paramQ = 0.5;
#else	// DISABLE_THIRD_TEXTURING
	    paramP = max( eps, min( 1.0-eps, paramP ) );
	    paramQ = max( eps, min( 1.0-eps, paramQ ) );
#endif	// DISABLE_THIRD_TEXTURING
	    
#ifdef ENABLE_R_VALUE_COLORING
	    // paramP = max( tQ, min( 1.0, tQ ) );
	    paramP = ( adjustQ - range_R_value.lower() )/( range_R_value.upper() - range_R_value.lower() );
	    paramQ = SAMPLE_TEXTURE_Y_VALUE;
	    R_value += adjustQ;
	    if ( ( translate_x - scale_x * WINDOW_SIDE > hfc->vertex()->point().x() ) ||
		 ( translate_x + scale_x * WINDOW_SIDE < hfc->vertex()->point().x() ) )
		R_inside = false;
	    if ( ( translate_y - scale_y * WINDOW_SIDE > hfc->vertex()->point().y() ) ||
		 ( translate_y + scale_y * WINDOW_SIDE < hfc->vertex()->point().y() ) )
		R_inside = false;
#endif	// ENABLE_R_VALUE_COLORING
	    glTexCoord2d( paramP, paramQ );
	    glVertex3d( hfc->vertex()->point().x(),
			hfc->vertex()->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
			0.0
#else	// ZERO_HEIGHT_PROJECTION
			hfc->vertex()->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
			);
	} while ( ++hfc != fi->facet_begin() );
	glEnd();
#ifdef ENABLE_R_VALUE_COLORING
	R_value /= 3.0;
	if ( R_inside ) {
	    if ( R_value < LIMIT_R_INTERVAL_MIN ) {
		cerr << HERE << " Illegal R value = " << R_value << endl;
	    }
	    else {
		histofs << R_value << endl;
	    }
	}
#endif	// ENABLE_R_VALUE_COLORING
    }
#ifdef ENABLE_R_VALUE_COLORING
    histofs.close();
#endif	// ENABLE_R_VALUE_COLORING
    
    glDisable( GL_TEXTURE_2D );
    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();
	P_Facet_handle fh = fi;
        // Facets in polyhedral surfaces are at least triangles.
        CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );

	if ( fh->flag() ) {
	    glColor3d( 1.0, 1.0, 1.0 );
	    glBegin( GL_POLYGON );
	    do {
		glVertex3d( hfc->vertex()->point().x(),
			    hfc->vertex()->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
			    0.05
#else	// ZERO_HEIGHT_PROJECTION
			    hfc->vertex()->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
			    );
	    } while ( ++hfc != fi->facet_begin() );
	    glEnd();
	}
    }

    glEnable( GL_LIGHTING );
    glShadeModel( GL_SMOOTH );

    return;
}    


void basement( Polyhedron & mesh )
{
    // for Picking
    glInitNames         ();
    glPushName          ( -1 );

    // cerr << HERE << " In basement!!" << endl;
    
    // const double eps = 1.0e-4;
    glDisable( GL_LIGHTING );
    // glShadeModel( GL_FLAT );
    glDisable( GL_TEXTURE_2D );
    // glEnable( GL_DEPTH_TEST );

    for ( P_Facet_iterator fi = mesh.facets_begin(); fi != mesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();
	P_Facet_handle fh = fi;

	if ( fh->flag() )
	    // glColor3d( 0.5, 0.5, 0.5 );
	    // glColor3d( 0.3, 0.3, 0.3 );
	    glColor3d( 1.0, 0.8, 0.6 );
	else if ( landflag.passByCode( fi->label() ) ) {
	    if ( ( ! distFlag ) ||
		 ( distFlag &&
		   ( lowerDistLimit <= fi->dist() ) &&
		   ( fi->dist() <= upperDistLimit ) ) )
		glColor3d( 1.0, 1.0, 1.0 );
	    else
		glColor3d( 0.7, 0.7, 0.7 );
	}
	else glColor3d( 0.7, 0.7, 0.7 );
        // Facets in polyhedral surfaces are at least triangles.
        CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );

	glLoadName( fh->id() );

//------------------------------------------------------------------------------
//	Use this if you want to color specific area in red (as urban or peak)
//	You must call fh->setUse() in writePlotCSV to activate specific areas
//------------------------------------------------------------------------------
#ifdef ENABLE_USE_BIT
	// Land Use 
	if ( fh->use() ) {
	    glColor3d( 1.0, 0.0, 0.0 );
	}
#endif	// ENABLE_USE_BIT

	// draw boundary lines of faces in black
	// Vector3 normal = fh->faceEq().orthogonal_vector();
        glBegin( GL_POLYGON );
        do {
            glVertex3d( hfc->vertex()->point().x(),
			hfc->vertex()->point().y(),
#ifdef ZERO_HEIGHT_PROJECTION
			0.0
#else	// ZERO_HEIGHT_PROJECTION
			hfc->vertex()->point().z()
#endif	// ZERO_HEIGHT_PROJECTION
			);
        } while ( ++hfc != fi->facet_begin() );
        glEnd();
    }
	
    glEnable( GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    glShadeModel( GL_SMOOTH );

    glInitNames         ();
    glPushName          ( -1 );

    return;
}    


void sectionPQ( void )
{
    if ( curLevel == NULL ) return;
    if ( curLevel->facePQ().size() == 0 ) return;
    if ( pixelPQ.size() == 0 ) return;
    
#ifdef DEBUG
    cerr << HERE << " gx = " << gx << " gy = " << gy
	 << " sx = " << sx << " sy = " << sy << endl;
    cerr << HERE << " id = " << id << " / " << curLevel->bufferFace().size() << endl;
#endif	// DEBUG
    
    if ( touchPQ ) {
	touchPQ = false;
	touchAll = true;
	secPQ.clear();

	for ( unsigned int i = 0; i < pixelPQ.size(); ++i ) {
	    unsigned int id = pixelPQ[ i ];
	    for ( unsigned int k = 0; k < curLevel->facePQ()[ id ].size(); ++k ) {
		P_Facet_handle & fh = curLevel->facePQ()[ id ][ k ];
		map< unsigned int, P_Facet_handle >::iterator it = secPQ.find( fh->id() );
		if ( it == secPQ.end() ) {
		    secPQ.insert( pair< unsigned int, P_Facet_handle >( fh->id(), fh ) );
		}
	    }
	}
    }

    if ( flag_all ) return;
//------------------------------------------------------------------------------
//	Drawing sectional triangles
#ifdef SKIP
    // glUseProgram( maskProgram );
    glUseProgram( simpleProgram );
#endif	// SKIP

    glDisable( GL_LIGHTING );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_ALWAYS );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glLineWidth( 1.0 );
    if ( flag_outline ) 
	// ***** Red *****
	glColor4d( 1.0, 0.0, 0.0, 1.0 );
    else
	glColor4d( 1.0, 0.0, 0.0, 0.5 );

    for ( map< unsigned int, P_Facet_handle >::iterator it = secPQ.begin();
	  it != secPQ.end(); it++ ) {
	P_Facet_handle & fh = it->second;
	// outline the face when necessary (previous -> current -> next)
	P_Halfedge_handle hh[ 3 ];
	hh[ 0 ] = fh->halfedge()->prev();
	hh[ 1 ] = fh->halfedge();
	hh[ 2 ] = fh->halfedge()->next();
	// outline the face when necessary
	for ( unsigned int k = 0; k < 3; ++k ) {
	    if ( ! hh[ k ]->opposite()->is_border() ) {
		unsigned int idF = hh[ k ]->opposite()->facet()->id();
		map< unsigned int, P_Facet_handle >::iterator it = secPQ.find( idF );
		if ( it == secPQ.end() ) {
		    P_Point3 & pS = hh[ k ]->vertex()->point();
		    P_Point3 & pT = hh[ k ]->opposite()->vertex()->point();
		    glBegin( GL_LINES );
#ifdef ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), 0.0 );
		    glVertex3d( pT.x(), pT.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), pS.z() );
		    glVertex3d( pT.x(), pT.y(), pT.z() );
#endif	// ZERO_HEIGHT_PROJECTION
		    glEnd();
		}
	    }
	}
    }

    if ( flag_fill ) {
	// ***** Red *****
	if ( flag_outline ) glColor4d( 1.0, 0.0, 0.0, 0.2 );
	else glColor4d( 1.0, 0.0, 0.0, 0.5 );

	for ( map< unsigned int, P_Facet_handle >::iterator it = secPQ.begin();
	      it != secPQ.end(); it++ ) {
	    P_Facet_handle & fh = it->second;
	    // fill the face
	    glBegin( GL_POLYGON );
	    P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    P_Point3 & a = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), a.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & b = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), b.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & c = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), c.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    glEnd();
	}
    }
	
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glDisable( GL_BLEND );
    
    glColor4d( 0.8, 0.8, 0.8, 1.0 );
}


void sectionPR( void )
{
    if ( curLevel == NULL ) return;
    if ( curLevel->facePR().size() == 0 ) return;
    if ( pixelPR.size() == 0 ) return;
    
#ifdef DEBUG
    cerr << HERE << " gx = " << gx << " gy = " << gy
	 << " sx = " << sx << " sy = " << sy << endl;
    cerr << HERE << " id = " << id << " / " << curLevel->bufferFace().size() << endl;
#endif	// DEBUG
    
    if ( touchPR ) {
	touchPR = false;
	touchAll = true;
	secPR.clear();
	
	for ( unsigned int i = 0; i < pixelPR.size(); ++i ) {
	    unsigned int id = pixelPR[ i ];
	    for ( unsigned int k = 0; k < curLevel->facePR()[ id ].size(); ++k ) {
		P_Facet_handle & fh = curLevel->facePR()[ id ][ k ];
		map< unsigned int, P_Facet_handle >::iterator it = secPR.find( fh->id() );
		if ( it == secPR.end() ) {
		    secPR.insert( pair< unsigned int, P_Facet_handle >( fh->id(), fh ) );
		}
	    }
	}
    }

    if ( flag_all ) return;
//------------------------------------------------------------------------------
//	Drawing sectional triangles
#ifdef SKIP
    // glUseProgram( maskProgram );
    glUseProgram( simpleProgram );
#endif	// SKIP

    glDisable( GL_LIGHTING );
    // ***** Green *****
    glColor4d( 0.0, 1.0, 0.0, 0.4 );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_ALWAYS );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glLineWidth( 1.0 );
    if ( flag_outline )
	// ***** Green *****
	glColor4d( 0.0, 1.0, 0.0, 1.0 );
    else
	glColor4d( 0.0, 1.0, 0.0, 0.5 );

    for ( map< unsigned int, P_Facet_handle >::iterator it = secPR.begin();
	  it != secPR.end(); it++ ) {
	P_Facet_handle & fh = it->second;
	// outline the face when necessary (previous -> current -> next)
	P_Halfedge_handle hh[ 3 ];
	hh[ 0 ] = fh->halfedge()->prev();
	hh[ 1 ] = fh->halfedge();
	hh[ 2 ] = fh->halfedge()->next();
	// outline the face when necessary
	for ( unsigned int k = 0; k < 3; ++k ) {
	    if ( ! hh[ k ]->opposite()->is_border() ) {
		unsigned int idF = hh[ k ]->opposite()->facet()->id();
		map< unsigned int, P_Facet_handle >::iterator it = secPR.find( idF );
		if ( it == secPR.end() ) {
		    P_Point3 & pS = hh[ k ]->vertex()->point();
		    P_Point3 & pT = hh[ k ]->opposite()->vertex()->point();
		    glBegin( GL_LINES );
#ifdef ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), 0.0 );
		    glVertex3d( pT.x(), pT.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), pS.z() );
		    glVertex3d( pT.x(), pT.y(), pT.z() );
#endif	// ZERO_HEIGHT_PROJECTION
		    glEnd();
		}
	    }
	}
    }

    if ( flag_fill ) {
	// ***** Green *****
	if ( flag_outline ) glColor4d( 0.0, 1.0, 0.0, 0.2 );
	else glColor4d( 0.0, 1.0, 0.0, 0.5 );

	for ( map< unsigned int, P_Facet_handle >::iterator it = secPR.begin();
	      it != secPR.end(); it++ ) {
	    P_Facet_handle & fh = it->second;
	    // fill the face
	    glBegin( GL_POLYGON );
	    P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    P_Point3 & a = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), a.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & b = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), b.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & c = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), c.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    glEnd();
	}
    }
    
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glDisable( GL_BLEND );
    
    glColor4d( 0.8, 0.8, 0.8, 1.0 );
}


void sectionQR( void )
{
    if ( curLevel == NULL ) return;
    if ( curLevel->faceQR().size() == 0 ) return;
    if ( pixelQR.size() == 0 ) return;
    
#ifdef DEBUG
    cerr << HERE << " gx = " << gx << " gy = " << gy
	 << " sx = " << sx << " sy = " << sy << endl;
    cerr << HERE << " id = " << id << " / " << curLevel->bufferFace().size() << endl;
#endif	// DEBUG
    
    if ( touchQR ) {
	touchQR = false;
	touchAll = true;
	secQR.clear();
	
	for ( unsigned int i = 0; i < pixelQR.size(); ++i ) {
	    unsigned int id = pixelQR[ i ];
	    for ( unsigned int k = 0; k < curLevel->faceQR()[ id ].size(); ++k ) {
		P_Facet_handle & fh = curLevel->faceQR()[ id ][ k ];
		map< unsigned int, P_Facet_handle >::iterator it = secQR.find( fh->id() );
		if ( it == secQR.end() ) {
		    secQR.insert( pair< unsigned int, P_Facet_handle >( fh->id(), fh ) );
		}
	    }
	}
    }

    if ( flag_all ) return;
//------------------------------------------------------------------------------
//	Drawing sectional triangles
#ifdef SKIP
    glUseProgram( simpleProgram );
#endif	// SKIP

    glDisable( GL_LIGHTING );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_ALWAYS );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glLineWidth( 1.0 );
    if ( flag_outline )
	// ***** Blue *****
	glColor4d( 0.0, 0.5, 1.0, 1.0 );
    else
	glColor4d( 0.0, 0.5, 1.0, 0.5 );

    for ( map< unsigned int, P_Facet_handle >::iterator it = secQR.begin();
	  it != secQR.end(); it++ ) {
	P_Facet_handle & fh = it->second;
	// outline the face when necessary (previous -> current -> next)
	P_Halfedge_handle hh[ 3 ];
	hh[ 0 ] = fh->halfedge()->prev();
	hh[ 1 ] = fh->halfedge();
	hh[ 2 ] = fh->halfedge()->next();
	// outline the face when necessary
	for ( unsigned int k = 0; k < 3; ++k ) {
	    if ( ! hh[ k ]->opposite()->is_border() ) {
		unsigned int idF = hh[ k ]->opposite()->facet()->id();
		map< unsigned int, P_Facet_handle >::iterator it = secQR.find( idF );
		if ( it == secQR.end() ) {
		    P_Point3 & pS = hh[ k ]->vertex()->point();
		    P_Point3 & pT = hh[ k ]->opposite()->vertex()->point();
		    glBegin( GL_LINES );
#ifdef ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), 0.0 );
		    glVertex3d( pT.x(), pT.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), pS.z() );
		    glVertex3d( pT.x(), pT.y(), pT.z() );
#endif	// ZERO_HEIGHT_PROJECTION
		    glEnd();
		}
	    }
	}
    }


    if ( flag_fill ) {
	// ***** Blue *****
	if ( flag_outline ) 
	    glColor4d( 0.0, 0.5, 1.0, 0.2 );
	else glColor4d( 0.0, 0.5, 1.0, 0.5 );

	for ( map< unsigned int, P_Facet_handle >::iterator it = secQR.begin();
	      it != secQR.end(); it++ ) {
	    P_Facet_handle & fh = it->second;
	    glBegin( GL_POLYGON );
	    P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    P_Point3 & a = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), a.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & b = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), b.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & c = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), c.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    glEnd();
	}
    }

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glDisable( GL_BLEND );

    glColor4d( 0.8, 0.8, 0.8, 1.0 );
}


void sectionAll( void )
{
    if ( curLevel == NULL ) return;
    
    if ( touchAll ) {
	touchAll = false;
	secAll.clear();
	
	for ( map< unsigned int, P_Facet_handle >::iterator itPQ = secPQ.begin();
	      itPQ != secPQ.end(); itPQ++ ) {
	    unsigned int id = itPQ->first;
	    P_Facet_handle fh = itPQ->second;
	    map< unsigned int, P_Facet_handle >::iterator itPR = secPR.find( id );
	    if ( itPR != secPR.end() ) {
		map< unsigned int, P_Facet_handle >::iterator itQR = secQR.find( id );
		if ( itQR != secQR.end() ) {
		    secAll.insert( pair< unsigned int, P_Facet_handle >( id, fh ) );
		}
	    }
	}
    }

    if ( !flag_all ) return;
//------------------------------------------------------------------------------
//	Drawing sectional triangles
#ifdef SKIP
    glUseProgram( simpleProgram );
#endif	// SKIP

    glDisable( GL_LIGHTING );
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_ALWAYS );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glLineWidth( 1.0 );
    if ( flag_outline ) {
	// ***** Gray *****
	if ( display_mode == 3 ) glColor4d( 0.8, 0.8, 0.8, 1.0 );
	else glColor4d( 0.2, 0.2, 0.2, 1.0 );
    }
    else {
	if ( display_mode == 3 ) glColor4d( 1.0, 1.0, 1.0, 0.8 );
	else glColor4d( 0.4, 0.4, 0.4, 0.5 );
    }
    for ( map< unsigned int, P_Facet_handle >::iterator it = secAll.begin();
	  it != secAll.end(); it++ ) {
	P_Facet_handle & fh = it->second;
	// outline the face when necessary (previous -> current -> next)
	P_Halfedge_handle hh[ 3 ];
	hh[ 0 ] = fh->halfedge()->prev();
	hh[ 1 ] = fh->halfedge();
	hh[ 2 ] = fh->halfedge()->next();
	// outline the face when necessary
	for ( unsigned int k = 0; k < 3; ++k ) {
	    if ( ! hh[ k ]->opposite()->is_border() ) {
		unsigned int idF = hh[ k ]->opposite()->facet()->id();
		map< unsigned int, P_Facet_handle >::iterator it = secAll.find( idF );
		if ( it == secAll.end() ) {
		    P_Point3 & pS = hh[ k ]->vertex()->point();
		    P_Point3 & pT = hh[ k ]->opposite()->vertex()->point();
		    glBegin( GL_LINES );
#ifdef ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), 0.0 );
		    glVertex3d( pT.x(), pT.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
		    glVertex3d( pS.x(), pS.y(), pS.z() );
		    glVertex3d( pT.x(), pT.y(), pT.z() );
#endif	// ZERO_HEIGHT_PROJECTION
		    glEnd();
		}
	    }
	}
    }

    if ( flag_fill ) {
	// ***** Blue *****
	if ( flag_outline ) 
	    glColor4d( 0.5, 0.5, 0.5, 0.2 );
	else if ( display_mode == 3 )
	    glColor4d( 1.0, 1.0, 1.0, 0.8 );
	else
	    glColor4d( 0.4, 0.4, 0.4, 0.5 );

	for ( map< unsigned int, P_Facet_handle >::iterator it = secAll.begin();
	      it != secAll.end(); it++ ) {


	    P_Facet_handle & fh = it->second;
	    glBegin( GL_POLYGON );
	    P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    P_Point3 & a = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( a.x(), a.y(), a.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & b = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( b.x(), b.y(), b.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    ++hfc;
	    P_Point3 & c = hfc->vertex()->point();
#ifdef ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), 0.0 );
#else	// ZERO_HEIGHT_PROJECTION
	    glVertex3d( c.x(), c.y(), c.z() );
#endif	// ZERO_HEIGHT_PROJECTION
	    glEnd();
	}
    }

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );
    glDisable( GL_BLEND );

    glColor4d( 0.8, 0.8, 0.8, 1.0 );
}


//------------------------------------------------------------------------------
//	Handling fiber-surface-like objects
//------------------------------------------------------------------------------

void addToInversePQ( unsigned int mx, unsigned int my )
{
    if ( curLevel == NULL ) return;

    unsigned int sx = curLevel->res().x();
    unsigned int sy = curLevel->res().y();
    unsigned int ix = width_PQ  / sx;
    unsigned int iy = height_PQ / sy;
    unsigned int gx = mx / ix;
    unsigned int gy = ( height_PQ - my ) / iy;
    unsigned int id = gy * sx + gx;

    if ( curLevel->facePQ().size() == 0 ) return;

    std::vector< SetFace > & comp = curLevel->compPQ();
    if ( comp[ id ].size() == 0 ) return;

#ifdef SKIP
    cerr << "########## Newly inserting id = " << id
	 << " No. of faces = " << curLevel->facePQ()[ id ].size() << endl;
#endif	// SKIP
    if ( ! binary_search( pixelPQ.begin(), pixelPQ.end(), id ) ) {
	std::vector< unsigned int >::iterator it = upper_bound( pixelPQ.begin(), pixelPQ.end(), id );
	pixelPQ.insert( it, id );
	touchPQ = true;
#ifdef DEBUG
	cerr << " pixelPQ : ";
	for ( unsigned int k = 0; k < pixelPQ.size(); k++ ) {
	    cerr << setw( 3 ) << pixelPQ[ k ];
	}
	cerr << endl;
#endif	// DEBUG
    }
}

void clearInversePQ( void )
{
#ifdef OLD
    selectedPQ.clear();
#endif	// OLD
    secPQ.clear();
    pixelPQ.clear();
    touchPQ = true;
}


void addToInversePR( unsigned int mx, unsigned int my )
{
    if ( curLevel == NULL ) return;

    unsigned int sP = curLevel->res().x();
    // unsigned int sQ = curLevel->res().y();
    unsigned int sR = curLevel->res().z();
    unsigned int iP = width_PR  / sP;
    unsigned int iR = height_PR / sR;
    unsigned int gP = mx / iP;
    unsigned int gR = ( height_PR - my ) / iR;
    unsigned int id = gR * sP + gP;

    if ( curLevel->facePR().size() == 0 ) return;

    std::vector< SetFace > & comp = curLevel->compPR();
    if ( comp[ id ].size() == 0 ) return;
    
#ifdef SKIP
    cerr << "########## Newly inserting id = " << id
	 << " No. of faces = " << curLevel->facePR()[ id ].size() << endl;
#endif	// SKIP

#ifdef OLD
    map< unsigned int, VecFace* >::iterator it = selectedPR.find( id );
    if ( it == selectedPR.end() ) {
	selectedPR.insert( pair< unsigned int, VecFace* >( id,
							   &( curLevel->facePR()[ id ] ) ) );
    }
#else	// OLD
    if ( ! binary_search( pixelPR.begin(), pixelPR.end(), id ) ) {
	std::vector< unsigned int >::iterator it = upper_bound( pixelPR.begin(), pixelPR.end(), id );
	pixelPR.insert( it, id );
	touchPR = true;
#ifdef DEBUG
	cerr << " pixelPR : ";
	for ( unsigned int k = 0; k < pixelPR.size(); k++ ) {
	    cerr << setw( 3 ) << pixelPR[ k ];
	}
	cerr << endl;
#endif	// DEBUG
    }
#endif	// OLD
}

void clearInversePR( void )
{
#ifdef OLD
    selectedPR.clear();
#endif	// OLD
    secPR.clear();
    pixelPR.clear();
    touchPR = true;
}


void addToInverseQR( unsigned int mx, unsigned int my )
{
    if ( curLevel == NULL ) return;

    // unsigned int sx = curLevel->res().x();
    unsigned int sQ = curLevel->res().y();
    unsigned int sR = curLevel->res().z();
    unsigned int iQ = width_QR  / sQ;
    unsigned int iR = height_QR / sR;
    unsigned int gQ = mx / iQ;
    unsigned int gR = ( height_QR - my ) / iR;
    unsigned int id = gR * sQ + gQ;

    if ( curLevel->faceQR().size() == 0 ) return;

    std::vector< SetFace > & comp = curLevel->compQR();
    if ( comp[ id ].size() == 0 ) return;

#ifdef SKIP
    cerr << "########## Newly inserting id = " << id
	 << " No. of faces = " << curLevel->faceQR()[ id ].size() << endl;
#endif	// SKIP

#ifdef OLD
    map< unsigned int, VecFace* >::iterator it = selectedQR.find( id );
    if ( it == selectedQR.end() ) {
	selectedQR.insert( pair< unsigned int, VecFace* >( id,
							   &( curLevel->faceQR()[ id ] ) ) );
    }
#else	// OLD
    if ( ! binary_search( pixelQR.begin(), pixelQR.end(), id ) ) {
	std::vector< unsigned int >::iterator it = upper_bound( pixelQR.begin(), pixelQR.end(), id );
	pixelQR.insert( it, id );
	touchQR = true;
#ifdef DEBUG
	cerr << " pixelQR : ";
	for ( unsigned int k = 0; k < pixelQR.size(); k++ ) {
	    cerr << setw( 3 ) << pixelQR[ k ];
	}
	cerr << endl;
#endif	// DEBUG
    }
#endif	// OLD
}

void clearInverseQR( void )
{
#ifdef OLD
    selectedQR.clear();
#endif	// OLD
    secQR.clear();
    pixelQR.clear();
    touchQR = true;
}

void clearInverseAll( void )
{
#ifdef OLD
    selectedQR.clear();
#endif	// OLD
    secAll.clear();
    touchAll = true;
}

//------------------------------------------------------------------------------
//	Preparation for triangle picking
//------------------------------------------------------------------------------
bool handleTriangle( int nHits, unsigned int * buf, std::vector< unsigned int > & faceSet )
{
    unsigned int *	ptr		= NULL; //, names;
    // float		minDepth	= 1000.0;
    // int			hitID		= NO_INDEX;
    
    faceSet.clear();

    ptr = buf;
    
    for ( int i = 0; i < nHits; ++i ) { // for each bit
	if ( ptr[ 0 ] != 1 ) {
	    cerr << " Number of names for hit = " << ( int )ptr[ 0 ] << endl;
	    assert( ptr[ 0 ] == 1 );
	}
	// float curDepth = (float)ptr[ 1 ]/0xffffffff;
#ifdef DEBUG
	cerr << " i = " << i 
	     << " [0]: " << ptr[ 0 ]
	     << " [1]: " << ptr[ 1 ]
	     << " [2]: " << ptr[ 2 ]
	     << " [3]: " << ptr[ 3 ] << endl;
#endif	// DEBUG
	// if ( ( curDepth < minDepth ) && ( curID != NO_INDEX ) ) {
	// minDepth = curDepth;
	// hitID = ptr[ 3 ]
	int curID = ( int )ptr[ 3 ];
	if ( curID != NO_INDEX ) {
	    faceSet.push_back( ( unsigned int )curID );
	}
	ptr += 4;
    }
    
    // cerr << " hitID = " << hitID << " depth = " << minDepth << endl;
    
    if ( faceSet.size() > 0 ) return true;
    else return false;
}


bool pickTriangle( int x, int y, std::vector< unsigned int > & faceSet )
{
    unsigned int	selectBuf[ SELECTION_SIZE ];
    int			nHits;
    int			viewport[ 4 ];
    std::vector< unsigned int >	fset;

    // cerr << " pickTriangle " << endl;

    glGetIntegerv	( GL_VIEWPORT, viewport );
    
    // Picking begins here
    glSelectBuffer	( SELECTION_SIZE, selectBuf );
    glRenderMode	( GL_SELECT );
    
    glInitNames();
    
    glMatrixMode	( GL_PROJECTION );
    glPushMatrix(); // <====
    glLoadIdentity();
    // create small picking region near cursor location
    //double const size tolerance = 1.0;
    // const double tolerance = 2.0;
    // const double tolerance = 5.0;
    // const double tolerance = 8.0; // original tolerance <<<<<
    // const double tolerance = 10.0;
    const double tolerance = 16.0;
    // const double tolerance = 20.0;
#ifdef DEBUG
    cerr << "viewport = " << viewport[0] << "," << viewport[1] << ","
         << viewport[2] << "," << viewport[3] << endl;
    cerr << " x = " << x << " y = " << y << endl;
#endif	// DEBUG
    gluPickMatrix( (double)x, (double)(viewport[3]-y), tolerance, tolerance, viewport );

#ifdef SKIP
    gluPerspective( fovy, aspect, near, far );
#endif	// SKIP
    gluOrtho2D( translate_x - scale_x * WINDOW_SIDE,
		translate_x + scale_x * WINDOW_SIDE,
		translate_y - scale_y * WINDOW_SIDE,
		translate_y + scale_y * WINDOW_SIDE );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();	// <====
    glLoadIdentity();
    
    Polyhedron & curMesh = meshMap;
    basement( curMesh );
    
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();	// <====
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();	// <====
    
    glFlush();

    nHits = glRenderMode( GL_RENDER );

    glutPostRedisplay();

    if ( handleTriangle( nHits, selectBuf, fset ) ) {
	faceSet = fset;
	// cerr << " Face ID = " << faceID << " found." << endl;
	return true;
    }

    return false;
}



//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
void annotate( void )
{
    double defaultColor[ 3 ], specialColor[ 3 ];

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
    glPointSize( 5.0 );
    gl2psPointSize( 5.0 );
    glEnable( GL_POINT_SMOOTH );

    switch ( display_mode ) {
      case 0:
	  defaultColor[ 0 ] = 0.0; defaultColor[ 1 ] = 0.0; defaultColor[ 2 ] = 0.0;
	  specialColor[ 0 ] = 1.0; specialColor[ 1 ] = 0.3; specialColor[ 2 ] = 0.0;
	  break;
      case 1:
	  // defaultColor[ 0 ] = 0.0; defaultColor[ 1 ] = 0.0; defaultColor[ 2 ] = 0.0;
	  defaultColor[ 0 ] = 0.0; defaultColor[ 1 ] = 0.0; defaultColor[ 2 ] = 0.6;
	  specialColor[ 0 ] = 1.0; specialColor[ 1 ] = 0.3; specialColor[ 2 ] = 0.0;
	  break;
      case 2:
	  defaultColor[ 0 ] = 1.0; defaultColor[ 1 ] = 0.0; defaultColor[ 2 ] = 0.0;
	  specialColor[ 0 ] = 1.0; specialColor[ 1 ] = 0.0; specialColor[ 2 ] = 0.0;
	  break;
      case 3:
	  defaultColor[ 0 ] = 1.0; defaultColor[ 1 ] = 1.0; defaultColor[ 2 ] = 1.0;
	  specialColor[ 0 ] = 0.0; specialColor[ 1 ] = 0.0; specialColor[ 2 ] = 0.0;
	  break;
    }

    //#ifdef FUKUSHIMA
    glColor3dv( defaultColor );
    glBegin( GL_POINTS );
    for ( unsigned int k = 0; k < nCities; ++k ) {
	// Point3 & p = cityVh[ k ]->point();
	// glVertex3d( p.x(), p.y(), p.z() );
	double *p = cityCoord[ k ];
	if ( ( display_mode != 2 ) || ( k < nMarks ) )
	    glVertex3d( p[ 0 ], p[ 1 ], 0.1 );
    }
    glEnd();


    for ( unsigned int k = 0; k < nCities; ++k ) {
	double *p = cityCoord[ k ];
	if ( k < nMarks ) glColor3dv( specialColor );
	else glColor3dv( defaultColor );
#ifdef USE_FTGL
	const double shift_x = 0.02;
	double shiftS = shift_x * scale_x * WINDOW_SIDE;
	if ( display_mode == 2 ) {
	    if ( k < nMarks )
		vectorString( p[ 0 ]-0.032, p[ 1 ], 0.1, 16, cityName[ k ] );
	}
	else {
	    switch( cityLayout[ k ] ) {
	      case 1:
		  //vectorString( p[ 0 ]+0.010, p[ 1 ], 0.1, 16, cityName[ k ] );
		  vectorString( p[ 0 ]+shiftS, p[ 1 ], 0.1, 16, cityName[ k ] );
		  break;
	      case 4:
		  // vectorString( p[ 0 ]+0.010, p[ 1 ]-0.014, 0.1, 16, cityName[ k ] );
		  vectorString( p[ 0 ]+shiftS, p[ 1 ]-0.018, 0.1, 16, cityName[ k ] );
		  break;
	      default:
		  // vectorString( p[ 0 ]+0.010, p[ 1 ], 0.1, 16, cityName[ k ] );
		  vectorString( p[ 0 ]+shiftS, p[ 1 ], 0.1, 16, cityName[ k ] );
		  break;
	    }
	}
#else	// USE_FTGL
	smallString( p[ 0 ]+0.01, p[ 1 ], 0.1, cityName[ k ] );
#endif	// USE_FTGL
    }
    //#endif	// FUKUSHIMA
    // This is necessary!!
    glColor3d( 1.0, 1.0, 1.0 );
    glEnable( GL_LIGHTING );
    glEnable( GL_DEPTH_TEST );
}


void display_shape( void )
{
    // cerr << HERE << " display_shape" << endl;

    Polyhedron & curMesh = meshMap;
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    if ( ! is_displayed_shape ) is_displayed_shape = true;
    if ( ! is_processed ) return;
    
    // if ( curLevel == NULL ) return;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluOrtho2D( translate_x - scale_x * WINDOW_SIDE,
		translate_x + scale_x * WINDOW_SIDE,
		translate_y - scale_y * WINDOW_SIDE,
		translate_y + scale_y * WINDOW_SIDE );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glPushMatrix();

    // cerr << " display_mode = " << display_mode << endl;

#ifndef DISABLE_AREA_RECTANGLES
    rectangle();
#endif	// DISABLE_AREA_RECTANGLES

    switch ( display_mode ) {
      case 0:
	  flat( curMesh );
	  break;
      case 1:
	  boundary();
	  network();
	  brook();
	  basement( curMesh );
	  coordinate();
	  // plot( curMesh );
	  break;
      case 2:
	  // hidden( curMesh );
	  wireframe( curMesh );
	  coordinate();
	  break;
      case 3:
	  boundary();
	  // prepareJCNTextures();
	  texture2D( curMesh, boundPQ, funcP, funcQ );
	  network();
	  brook();
	  coordinate();
	  break;
      default:
	  break;
    }

    glEnable( GL_BLEND );
    glBlendFunc( GL_ONE, GL_ONE );
    // glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glDisable( GL_DEPTH_TEST );
    sectionPQ();
    sectionPR();
    sectionQR();
    sectionAll();
    glDisable( GL_BLEND );
    glEnable( GL_DEPTH_TEST );

    annotate();

    glPopMatrix();

    glutSwapBuffers();

    // for debugging
    unsigned int numD = 0, numPQ = 0, numPR = 0, numQR = 0;
//------------------------------------------------------------------------------
//	Domain
//------------------------------------------------------------------------------
    for ( unsigned int k = 0; k < ptrFacet.size(); ++k )
	if ( ptrFacet[ k ]->flag() ) numD++;
    cerr << HERE << " Domain : " << setw( 8 ) << numD << " / " << ptrFacet.size()
	 << " ( " << 100.0 * ( double )numD/( double )ptrFacet.size()
	 << " % ) " << endl;

//------------------------------------------------------------------------------
//	PQ
//------------------------------------------------------------------------------
    for ( unsigned int i = 0; i < pixelPQ.size(); ++i ) {
	unsigned int id = pixelPQ[ i ];
	for ( unsigned int k = 0; k < curLevel->facePQ()[ id ].size(); ++k ) {
	    P_Facet_handle & fh = curLevel->facePQ()[ id ][ k ];
	    // P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    // do {
	    // hfc->vertex()->setFlag();
	    // } while ( ++hfc != fh->facet_begin() );
	    numPQ++;
	}
    }
    cerr << HERE << " RangePQ: " << setw( 8 ) << numPQ << " / " << ptrFacet.size()
	 << " ( " << 100.0 * ( double )numPQ/( double )ptrFacet.size()
	 << " % ) " << endl;
//------------------------------------------------------------------------------
//	PR
//------------------------------------------------------------------------------
    for ( unsigned int i = 0; i < pixelPR.size(); ++i ) {
	unsigned int id = pixelPR[ i ];
	for ( unsigned int k = 0; k < curLevel->facePR()[ id ].size(); ++k ) {
	    P_Facet_handle & fh = curLevel->facePR()[ id ][ k ];
	    // P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    // do {
	    // hfc->vertex()->setFlag();
	    // } while ( ++hfc != fh->facet_begin() );
	    numPR++;
	}
    }
    cerr << HERE << " RangePR: " << setw( 8 ) << numPR << " / " << ptrFacet.size()
	 << " ( " << 100.0 * ( double )numPR/( double )ptrFacet.size()
	 << " % ) " << endl;
//------------------------------------------------------------------------------
//	QR
//------------------------------------------------------------------------------
    for ( unsigned int i = 0; i < pixelQR.size(); ++i ) {
	unsigned int id = pixelQR[ i ];
	for ( unsigned int k = 0; k < curLevel->faceQR()[ id ].size(); ++k ) {
	    P_Facet_handle & fh = curLevel->faceQR()[ id ][ k ];
	    // P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    // do {
	    // hfc->vertex()->setFlag();
	    // } while ( ++hfc != fh->facet_begin() );
	    numQR++;
	}
    }
    cerr << HERE << " RangeQR: " << setw( 8 ) << numQR << " / " << ptrFacet.size()
	 << " ( " << 100.0 * ( double )numQR/( double )ptrFacet.size()
	 << " % ) " << endl;
//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
}


/* reshape the display size */
void reshape_shape( int w, int h )
{
    width_shape = w;
    height_shape = h;
    glViewport ( 0, 0, width_shape, height_shape );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( fovy, (double)width_shape/(double)height_shape, near, far );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

}

void visibility_shape( int state )
{
    cerr << HERE << " visibility_shape " << endl;

    if ( state == GLUT_VISIBLE ) visible_shape = true;
    if ( state == GLUT_NOT_VISIBLE ) visible_shape = false;
}

void dumpSelected( const char * name, 
		   const std::vector< double > & coordX,
		   const std::vector< double > & coordY,
		   const std::vector< double > & attrP,
		   const std::vector< double > & attrQ,
		   const std::vector< double > & attrR,
		   const interval< double > & rngX,
		   const interval< double > & rngY,
		   const interval< double > & rngP,
		   const interval< double > & rngQ,
		   const interval< double > & rngR )
{
    ofstream ofs( name );
    if ( ! ofs ) {
	cerr << " cannot open the file : " << name << endl;
	return; 
    }

    cerr << "Dump the coordinates of selected samples " << ends;

    // check selected faces
    Polyhedron & curMesh = meshMap;
    for ( P_Facet_iterator fi = curMesh.facets_begin(); fi != curMesh.facets_end(); ++fi ) {
        P_Halfedge_facet_circulator hfc = fi->facet_begin();
        // Facets in polyhedral surfaces are at least triangles.
        // CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );
        do {
	    hfc->vertex()->setFlag();
        } while ( ++hfc != fi->facet_begin() );
    }
    cerr << "." << ends;

    // check selected pixels in range PQ 
    for ( unsigned int i = 0; i < pixelPQ.size(); ++i ) {
	unsigned int id = pixelPQ[ i ];
	for ( unsigned int k = 0; k < curLevel->facePQ()[ id ].size(); ++k ) {
	    P_Facet_handle & fh = curLevel->facePQ()[ id ][ k ];
	    // CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );
	    P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    do {
		hfc->vertex()->setFlag();
	    } while ( ++hfc != fh->facet_begin() );
	}
    }
    cerr << "." << ends;

    // check selected pixels in range PR 
    for ( unsigned int i = 0; i < pixelPR.size(); ++i ) {
	unsigned int id = pixelPR[ i ];
	for ( unsigned int k = 0; k < curLevel->facePR()[ id ].size(); ++k ) {
	    P_Facet_handle & fh = curLevel->facePR()[ id ][ k ];
	    // CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );
	    P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    do {
		hfc->vertex()->setFlag();
	    } while ( ++hfc != fh->facet_begin() );
	}
    }
    cerr << "." << ends;

    // check selected pixels in range QR 
    for ( unsigned int i = 0; i < pixelQR.size(); ++i ) {
	unsigned int id = pixelQR[ i ];
	for ( unsigned int k = 0; k < curLevel->faceQR()[ id ].size(); ++k ) {
	    P_Facet_handle & fh = curLevel->faceQR()[ id ][ k ];
	    // CGAL_assertion( CGAL::circulator_size( hfc ) >= 3 );
	    P_Halfedge_facet_circulator hfc = fh->facet_begin();
	    do {
		hfc->vertex()->setFlag();
	    } while ( ++hfc != fh->facet_begin() );
	}
    }
    cerr << "." << ends;

    for ( P_Vertex_iterator vi = curMesh.vertices_begin();
	  vi != curMesh.vertices_end(); ++vi ) {
	if ( vi->flag() ) {
	    unsigned int id = vi->id();
	    double rX = 0.5 * ( coordX[ id ] + 1.0 );
	    double rY = 0.5 * ( coordY[ id ] + 1.0 );
	    double rP = 0.5 * ( attrP[ id ] + 1.0 );
	    double rQ = 0.5 * ( attrQ[ id ] + 1.0 );
	    double rR = 0.5 * ( attrR[ id ] + 1.0 );
	    double X = ( 1.0 - rX )*rngX.lower() + rX*rngX.upper();
	    double Y = ( 1.0 - rY )*rngY.lower() + rY*rngY.upper();
	    double P = ( 1.0 - rP )*rngP.lower() + rP*rngP.upper();
	    double Q = ( 1.0 - rQ )*rngQ.lower() + rQ*rngQ.upper();
	    double R = ( 1.0 - rR )*rngR.lower() + rR*rngR.upper();
	    ofs << Y << "\t" 
		<< X << "\t" 
		<< pow( 10.0, P ) << "\t" 
		<< Q << "\t" 
		<< R << endl;
	    vi->clearFlag();
	}
    }
    cerr << "." << ends;

    ofs.close();
    cerr << " Done." << endl;
    return;
}



// draw wireframe as eps file */
void wireframe_eps( const char * name )
{
    FILE *fp = NULL;
    int state = GL2PS_OVERFLOW, buffsize = 0;
    fp = fopen( name, "wb" );    
    int previous_mode = display_mode;
    display_mode = 2;

    while ( state == GL2PS_OVERFLOW ) {
        buffsize += 1024*1024;
	gl2psBeginPage( "wireframe", "gl2ps", 
			NULL, GL2PS_EPS, GL2PS_SIMPLE_SORT,
			GL2PS_DRAW_BACKGROUND | GL2PS_USE_CURRENT_VIEWPORT,
			// GL2PS_USE_CURRENT_VIEWPORT,
			GL_RGBA, 0, NULL, 0, 0, 0, buffsize, fp, name );
	display_shape();
        state = gl2psEndPage();
    }
    fclose( fp );
    display_mode = previous_mode;

    cerr << "Writing the wireframe as eps .. done" << endl;
}    


/* dump the window image */
void capture_shape( const char * name )
{
    static cv::Mat              image;
    static GLubyte		* data = NULL;
    /* round off height */
    int				h = ( int )height_shape;
    /* round off width */
    int				w = ( int )width_shape;
    const unsigned int          nChannels = 4;

    redisplay_all();

    glutSetWindow( win_shape );
    if ( data == NULL ) data = new GLubyte [ w * h * nChannels ];
    glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );

    // image = cv::Mat( cv::Size( w, h ), CV_8UC3 );
    image = cv::Mat( cv::Size( w, h ), CV_8UC4 );
    memcpy( image.data, data, w * h * nChannels );
    cv::cvtColor( image, image, cv::COLOR_BGR2RGB );
    cv::flip( image, image, 0 );
    cv::imwrite( name, image );
	
    delete [] data;
    data = NULL;
    image.release();
    
    cerr << "Capturing the shape window .. done" << endl;
}    

/* handle the mouse motion */
void motion_shape( int x, int y )
{
    const double scale = 0.1;
    if ( left_shape ) {
        /* The left button is used for the menu selection. */
        //  glReadPixels(x,width-y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);
    }
    else if ( middle_shape ) {
        view_distance -= scale * ( double )( y - my_shape );
        mx_shape = x;
        my_shape = y;
        // glutPostRedisplay();
    }
    else if ( right_shape ) {
#ifdef SKIP
        const int ratio = 2;
        view_azimuth -=
                ( ( double )( x - mx_shape ) / ( double )( ratio * width_shape ) ) * 180.0;
        view_incidence -=
                ( ( double )( y - my_shape ) / ( double )( ratio * height_shape ) ) * 90.0;
#endif	// SKIP
        mx_shape = x;
        my_shape = y;
	std::vector< unsigned int > unionSet;
	for ( int p = x-MAP_RADIUS; p <= x+MAP_RADIUS; ++p ) {
	    if ( ( 0 <= p ) && ( p < LARGE_WIDTH ) ) {
		for ( int q = y-MAP_RADIUS; q <= y+MAP_RADIUS; ++q ) {
		    if ( ( 0 <= q ) && ( q < LARGE_HEIGHT ) ) {
			std::vector< unsigned int > currSet, eachSet;
			currSet = unionSet;
			pickTriangle( p, q, eachSet );
			sort( eachSet.begin(), eachSet.end() );
			unionSet.resize( currSet.size() + eachSet.size() );
			std::vector< unsigned int >::iterator it =
			    set_union( currSet.begin(), currSet.end(),
				       eachSet.begin(), eachSet.end(), unionSet.begin() );
			unionSet.resize( it - unionSet.begin() );
		    }
		}
	    }
	}
	for ( unsigned int k = 0; k < unionSet.size(); ++k ) {
#ifdef DEBUG
	    assert( unionSet[ k ] < ptrFacet.size() );
	    assert( ptrFacet[ unionSet[ k ] ] != NULL );
	    cerr << "[ " << unionSet[ k ] << " ]-";
#endif	// DEBUG
	    ptrFacet[ unionSet[ k ] ]->setFlag();
	}
	// ***** Extra handling
	glutSetWindow( win_shape );
	display_shape();
	// *****
	redisplay_all();
    }
    glutPostRedisplay();
}


/* handle the mouse clicking */
void mouse_shape( int press, int state, int x, int y )
{
    if ( press == GLUT_LEFT_BUTTON ) {
        if ( state == GLUT_DOWN ) {
            mx_shape = x;
            my_shape = y;
            left_shape = true;
        }
        else {
            left_shape = false;
        }
    }
    else if ( press == GLUT_MIDDLE_BUTTON ) {
        if ( state == GLUT_DOWN ) {
            mx_shape = x;
            my_shape = y;
            middle_shape = true;
        }
        else {
            middle_shape = false;
	    // cerr << "clear the set of selected triangles" << endl; 
	    for ( unsigned int k = 0; k < ptrFacet.size(); ++k )
		ptrFacet[ k ]->clearFlag();
	    redisplay_all();
        }
    }
    else if ( press == GLUT_RIGHT_BUTTON ) {
        if ( state == GLUT_DOWN ) {
            // cerr << " Left button pressed" << endl;
            mx_shape = x;
            my_shape = y;
            right_shape = true;
        }
        else {
            // cerr << " Left button released" << endl;
            right_shape = false;
        }
    }
    glutPostRedisplay();
}

void keyboard_shape( unsigned char key, int x, int y )
{
    switch ( key ) {
#ifndef DISABLE_AREA_RECTANGLES
      case 'd':
      case 'D':
	  dryBit = std::max( 0, 1 - dryBit );
	  break;
      case 'w':
      case 'W':
	  wetBit = std::max( 0, 1 - wetBit );
	  break;
#endif	// DISABLE_AREA_RECTANGLES
      case 'p':
	  if ( valueID > 0 ) { 
	      valueID--;
	      cerr << " current time step = " << valueID << endl;
	      isProjected[ valueID ] = false;
	      Tuple3 key = curLevel->res();
	      // rasflag = false;
	      rasterization();
	      // rasflag = true;
	      adaptiveSampling( key, meshMap );
	  }
	  break;
      case 'n':
	  if ( valueID < NUM_VALUES - 1 ) {
	      valueID++;
	      cerr << " current time step = " << valueID << endl;
	      Tuple3 key = curLevel->res();
	      // rasflag = false;
	      rasterization();
	      // rasflag = true;
	      adaptiveSampling( key, meshMap );
	  }
	  break;
      case 'i':			// zoom In by increasing the map scale
	  map_scale	/= 1.05;
	  scale_x	= map_scale;
	  scale_y	= 2.0 * map_scale / 3.0;
	  break;
      case 'o':			// zoom Out by decreasing the map scale
	  map_scale	*= 1.05;
	  scale_x	= map_scale;
	  scale_y	= 2.0 * map_scale / 3.0;
	  break;
      case 'r':			// Poisson disk resampling
	  is_focused = false;
	  glutTimerFunc( 50, timer, 0 );
	  break;
      case 'c':
	  matchSamples( fundField, ptrFacet );
	  break;
      case 'f':
	  cerr << HERE << "=========> Flat mode" << endl;
	  display_mode = 0;
	  break;
      case 'b':
	  cerr << HERE << "=========> Basement mode" << endl;
	  display_mode = 1;
	  break;
      case 'w':
	  cerr << HERE << "=========> Wireframe mode" << endl;
	  display_mode = 2;
	  break;
      case 't':
	  cerr << HERE << "=========> Texture mode" << endl;
	  display_mode = 3;
	  break;
      case 'q':
      case 27:
	  exit( 0 );
	  break;
    }

    cerr << HERE << " translate = ( " << translate_x << " , "
	 << translate_y << " ) " << endl;
    cerr << HERE << " scale = ( " << scale_x << " , "
	 << scale_y << " ) " << endl;
    redisplay_all();
}

void special_shape( int key, int x, int y )
{
    const double step = 0.01;
    // int m = glutGetModifiers();

    switch ( key ) {
      case GLUT_KEY_LEFT:
	  translate_x -= step;
	  break;
      case GLUT_KEY_RIGHT:
	  translate_x += step;
	  break;
      case GLUT_KEY_UP:
	  translate_y += step;
	  break;
      case GLUT_KEY_DOWN:
	  translate_y -= step;
	  break;
      default:
	  break;
    }

    cerr << HERE << " translate = ( " << translate_x << " , "
	 << translate_y << " ) " << endl;
    cerr << HERE << " scale = ( " << scale_x << " , "
	 << scale_y << " ) " << endl;
    glutPostRedisplay();
}


//------------------------------------------------------------------------------
//	Range functions
//------------------------------------------------------------------------------

void reshape_range( int w, int h )
{
    // glClearColor(0.3, 0.3, 1.0, 0.0);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport ( 0, 0, w, h );
}

//#define LOAD_TEXTURE_IMAGE
void display_range( void )
{
    cerr << HERE << " display_range" << endl;

    // Is this necessary?
    // glClearColor( 0.0, 0.0, 0.0, 0.0 );
    // glViewport( 0, 0, width_range, height_range );

    if ( ! is_displayed_range ) is_displayed_range = true;
    // #ifndef LOAD_TEXTURE_IMAGE
    if ( ! is_processed ) {
	cerr << HERE << endl;
	cerr << HERE << "++++++++++++++++++++ display_range: not processed" << endl;
	cerr << HERE << endl;
	return;
    }
    if ( curLevel == NULL ) {
	cerr << HERE << endl;
	cerr << HERE << "++++++++++++++++++++ display_range: null curlevel pointer" << endl;
	cerr << HERE << endl;
	return;
    }
    // #endif	// LOAD_TEXTURE_IMAGE

#ifndef LOAD_TEXTURE_IMAGE

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    // #ifdef CURRENT_SKIP
    // Graph & curNet			= curLevel->netPQ();
    unsigned int sx			= curLevel->res().x();
    unsigned int sy			= curLevel->res().y();
    // unsigned int sz			= curLevel->res().z();
    // VertexIDMap		vertexID	= get( vertex_myid, curNet );
    // VertexCoordMap	vertexCoord	= get( vertex_mycoord, curNet );
    // VertexFacetsMap	vertexFacets	= get( vertex_myfacets, curNet );
    // num_displaylist = 0;
    // <<<<<debug>>>>>
#else	// LOAD_TEXTURE_IMAGE
    unsigned int sx = 4;
    unsigned int sy = 4;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
#endif	// LOAD_TEXTURE_IMAGE
    
    // prepareJCNTextures();

#ifdef CURRENT_SKIP
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    // gluOrtho2D( -0.5, (double)sx-0.5, -0.5, (double)sy-0.5 );
    gluOrtho2D( 0.0, (double)sx, 0.0, (double)sy );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
#endif	// CURRENT_SKIP
    glLoadIdentity();
    gluOrtho2D( 0.0, (double)sx, 0.0, (double)sy );

#ifdef CURRENT_SKIP
//------------------------------------------------------------------------------
//	Closed line placement for debug
//------------------------------------------------------------------------------
    // cerr << "JCN texture ID = " << jcn_texid[ resID ] << endl;
    glColor3d( 0.8, 0.7, 0.6 );
    glBegin( GL_LINE_LOOP );
    // glBegin( GL_POLYGON );
    // glTexCoord2d( 0.0, 0.0 );
    // glTexCoord2d( 0.1, 0.1 );
    glVertex2d( 0.0, 0.0 );
    // glTexCoord2d( 1.0, 0.0 );
    // glTexCoord2d( 0.9, 0.1 );
    glVertex2d(  0.5*(double)sx, 0.0 );
    // glTexCoord2d( 1.0, 1.0 );
    // glTexCoord2d( 0.9, 0.9 );
    glVertex2d(  (double)sx,  (double)sy );
    // glTexCoord2d( 0.0, 1.0 );
     // glTexCoord2d( 0.1, 0.9 );
    glVertex2d(  0.0,  0.5*(double)sy );
    glEnd();
#endif	// CURRENT_SKIP

#ifdef LOAD_TEXTURE_IMAGE
//------------------------------------------------------------------------------
//	Texture placement
//------------------------------------------------------------------------------
    const unsigned int texsize = 64;
    GLubyte mytex[texsize][texsize][3];
    FILE *fp;
  
    /* テクスチャ画像の読み込み */
    if ((fp = fopen("another.rgb", "rb")) != NULL) {
	fread( mytex, sizeof mytex, 1, fp);
	fclose(fp);
    }

    /* テクスチャ画像はバイト単位に詰め込まれている */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
    /* テクスチャの割り当て */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texsize, texsize, 0,
		 GL_RGBA, GL_UNSIGNED_BYTE, mytex);
    
    /* テクスチャを拡大・縮小する方法の指定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    // glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    // Note: We need to call glTexEnvi with GL_REPLACE
    // GL_MODULATE is the default for glTexEnvi, but this cannot produce
    // good results
    // noted by ST on 2023/01/28
    //glTexEnvi	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
    //glTexEnvi	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    glTexEnvf	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

#ifdef DEBUG_FOR_DUMP
    const unsigned int		nChannels = 4;
    unsigned int		w = texsize;
    unsigned int		h = texsize;
    Mat image =		cv::Mat( cv::Size( w, h ), CV_8UC4 );
    memcpy( image.data, mytex, w * h * nChannels );
    cv::cvtColor( image, image, cv::COLOR_BGR2RGB );
    cv::flip( image, image, 0 );
    ostringstream ostr;
    ostr << "dump/mytex" << ".jpg" << ends;
    cerr << " Saving " << ostr.str() << endl;
    cv::imwrite( ostr.str().c_str(), image );
    image.release();
#endif	// DEBUG_FOR_DUMP

#else	// LOAD_TEXTURE_IMAGE
    unsigned int powerX = ( unsigned int )round( log2( curLevel->res().x() ) );
    unsigned int powerY = ( unsigned int )round( log2( curLevel->res().y() ) );
    unsigned int resID = powerX * NUM_LEVELS + powerY;
    cerr << HERE << " powerX = " << powerX << " powerY = " << powerY
	 << " resID = " << resID << endl;

    unsigned texsize = TEXTURE_SIZE;
    Zone & mytex = teximage[ resID ];

    /* テクスチャ画像はバイト単位に詰め込まれている */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  
    /* テクスチャの割り当て */
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texsize, texsize, 0,
		  GL_RGBA, GL_UNSIGNED_BYTE, mytex);
    
    /* テクスチャを拡大・縮小する方法の指定 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    //glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
    glTexParameteri	( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

    // Note: We need to call glTexEnvi with GL_REPLACE
    // GL_MODULATE is the default for glTexEnvi, but this cannot produce
    // good results
    // noted by ST on 2023/01/28
    //glTexEnvi	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND );
    //glTexEnvi	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );
    glTexEnvf	( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );

#ifdef DEBUG_FOR_DUMP
    const unsigned int		nChannels = 4;
    unsigned int		w = texsize;
    unsigned int		h = texsize;
    Mat image =		cv::Mat( cv::Size( w, h ), CV_8UC4 );
    memcpy( image.data, mytex, w * h * nChannels );
    cv::cvtColor( image, image, cv::COLOR_BGR2RGB );
    cv::flip( image, image, 0 );
    ostringstream ostr;
    ostr << "dump/mypat" << setw( 2 ) << setfill( '0' ) << resID << ".jpg" << ends;
    cerr << " Saving " << ostr.str() << endl;
    cv::imwrite( ostr.str().c_str(), image );
    image.release();
#endif	// DEBUG_FOR_DUMP
#endif	// LOAD_TEXTURE_IMAGE

    glEnable( GL_TEXTURE_2D );
    glNormal3d(0.0, 0.0, 1.0);
    // glColor3d( 1.0, 0.0, 0.0 );

    // cerr << "JCN texture ID = " << jcn_texid[ resID ] << endl;
    glBegin( GL_QUADS );
    // glBegin( GL_POLYGON );
#ifdef ENABLE_R_VALUE_COLORING
    glTexCoord2d( 0.0, SAMPLE_TEXTURE_Y_VALUE );
#else	// ENABLE_R_VALUE_COLORING
    glTexCoord2d( 0.0, 0.0 );
#endif	// ENABLE_R_VALUE_COLORING
    glVertex2d( 0.0, 0.0 );

#ifdef ENABLE_R_VALUE_COLORING
    glTexCoord2d( 0.0, SAMPLE_TEXTURE_Y_VALUE );
#else	// ENABLE_R_VALUE_COLORING
    glTexCoord2d( 1.0, 0.0 );
#endif	// ENABLE_R_VALUE_COLORING
    glVertex2d(  (double)sx, 0.0 );

#ifdef ENABLE_R_VALUE_COLORING
    glTexCoord2d( 1.0, SAMPLE_TEXTURE_Y_VALUE );
#else	// ENABLE_R_VALUE_COLORING
    glTexCoord2d( 1.0, 1.0 );
#endif	// ENABLE_R_VALUE_COLORING
    glVertex2d(  (double)sx,  (double)sy );

#ifdef ENABLE_R_VALUE_COLORING
    glTexCoord2d( 1.0, SAMPLE_TEXTURE_Y_VALUE );
#else	// ENABLE_R_VALUE_COLORING
    glTexCoord2d( 0.0, 1.0 );
#endif	// ENABLE_R_VALUE_COLORING
    glVertex2d(  0.0,  (double)sy );

    glEnd();

    glDisable( GL_TEXTURE_2D );
    // glShadeModel( GL_SMOOTH );

    glutSwapBuffers();
}


void mouse_range( int press, int state, int x, int y )
{
    switch (press) {
      case GLUT_LEFT_BUTTON:
	  break;
      case GLUT_MIDDLE_BUTTON:
	  break;
      case GLUT_RIGHT_BUTTON:
	  break;
      default:
	  break;
    }
    glutPostRedisplay();
}

void motion_range( int x, int y )
{
    glutPostRedisplay();
}

void visibility_range( int state )
{
    if ( state == GLUT_VISIBLE ) visible_range = true;
    if ( state == GLUT_NOT_VISIBLE ) visible_range = false;
}

// dump the window image
void capture_range( const char * name )
{
    static cv::Mat		image;
    static GLubyte		*data = NULL;
    /* round off height */
    int				h = ( int )height_range;
    /* round off width */
    int				w = ( int )width_range;
    const unsigned int		nChannels = 4;

    redisplay_all();

    glutSetWindow( win_range );
    if ( data == NULL ) data = new GLubyte [ w * h * nChannels ];
    glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );

    // image = cv::Mat( cv::Size( w, h ), CV_8UC3 );
    image = cv::Mat( cv::Size( w, h ), CV_8UC4 );
    memcpy( image.data, data, w * h * nChannels );
    cv::cvtColor( image, image, cv::COLOR_BGR2RGB );
    cv::flip( image, image, 0 );
    cv::imwrite( name, image );
    
    delete [] data;
    data = NULL;
    image.release();

    cerr << "Capturing the range window .. done" << endl;
}    


//------------------------------------------------------------------------------
//	scatterplot window
//------------------------------------------------------------------------------
void reshape_scatterplot( int w, int h )
{
    glViewport ( 0, 0, w, h );
}


//------------------------------------------------------------------------------
//	PQ window
//------------------------------------------------------------------------------
void display_PQ( void )
{
    // cerr << HERE << " display_PQ" << endl;

    // Is this necessary
    // glClearColor( 0.0, 0.0, 0.0, 0.0 );

    glViewport( 0, 0, width_PQ, height_PQ );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if ( ! is_displayed_PQ ) is_displayed_PQ = true;
    if ( ! is_processed ) return;
    
    if ( curLevel != NULL ) {

	unsigned int sX = curLevel->res().x();
	unsigned int sY = curLevel->res().y();
	// unsigned int sZ = curLevel->res().z();

	// array for display
	float * buf = new GLfloat[ sX * sY * 3 ];
	// initialization
	for ( unsigned int i = 0; i < sX*sY*3; ++i) {
	    buf[ i ] = 0.0f;
	}
#ifdef TENTATIVE_SKIP
	FiberMap & fiber = curLevel->fiberPQ();
	setFiberColor( fiber, buf );
#endif	// TENTATIVE_SKIP

	// Singularity & sing = curLevel->singularPQ();

	float * spa = new float[ sX * sY * 3 ];
	for ( unsigned int i = 0; i < sX * sY * 3; ++i ) {
	    spa[ i ] = 0.0f;
	}
    
	std::vector< SetFace > & comp = curLevel->compPQ();
	// cerr << HERE << " comp.size = " << comp.size() << " dyadic = " << dyadic_size[ resID ] << endl;
	assert( comp.size() == sX * sY );

	for ( unsigned int i = 0; i < sX*sY; i++ ) {
	    unsigned int nComp = comp[ i ].size();
	    double ratio = 0.0;
	    ratio = ( sqrt(sX*sY)/32.0 ) * ( ( double )nComp )/LIMIT_NUM_COMPS;
	    ratio = min( 1.0, max( 0.0, ratio ) );
	    if ( ratio == 0.0 ) {
		spa[ i * 3 + 0 ] = spa[ i * 3 + 1 ] = spa[ i * 3 + 2 ] = 0.0;
	    }
	    else {
		spa[ i * 3 + 0 ] = spa[ i * 3 + 1 ] = spa[ i * 3 + 2 ] = 
		    CONTRAST_INTENSITY * ratio + BASE_INTENSITY;
	    }
	}
	
//------------------------------------------------------------------------------
//	Triangle projections
//------------------------------------------------------------------------------
	// cerr << " Number of facets = " << ptrFacet.size() << endl;
	SetUInt sel;
	for ( unsigned int k = 0; k < ptrFacet.size(); ++k ) {
	    if ( ptrFacet[ k ]->flag() ) {
		// cerr << " Triangle " << k << " is ON. No. = " << curLevel->projPQ()[ k ].size() << endl;
		for ( unsigned int m = 0; m < curLevel->projPQ()[ k ].size(); m++ ) {
		    unsigned int id = curLevel->projPQ()[ k ][ m ];
		    if ( ! binary_search( sel.begin(), sel.end(), id ) ) { 
			SetUInt::iterator it = upper_bound( sel.begin(), sel.end(), id );
			sel.insert( it, id );
		    }
		}
	    }
	}
#ifdef SKIP
	cerr << HERE << " Number of selected triangles in the map domain = "
	     << sel.size() << " / " << ptrFacet.size() << endl;
#endif	// SKIP
	
//------------------------------------------------------------------------------
//	Pixel color adjustment
//------------------------------------------------------------------------------
	// assign red to the selected pixels
	const double t = BLENDING_RATIO;
	const double s = 1.0 - t;
	for ( unsigned int id = 0; id < sX*sY; ++id ) {
	    // Copy original color to the target color
	    double tarColor[ 3 ];
	    for ( unsigned int k = 0; k < 3; ++k ) 
		tarColor[ k ] = spa[ id * 3 + k ];

	    // if the pixel is selected in range space, then assign red
	    if ( binary_search( pixelPQ.begin(), pixelPQ.end(), id ) )
		tarColor[ 0 ] = FULL_INTENSITY;
	    // if the pixel is selected in domain space, then assign cyan
	    if ( binary_search( sel.begin(), sel.end(), id ) )
		tarColor[ 1 ] = tarColor[ 2 ] = MODERATE_INTENSITY;

	    // Adjust pixel color
	    for ( unsigned int k = 0; k < 3; ++k )
		spa[ id * 3 + k ] = t * tarColor[ k ] + s * spa[ id * 3 + k ];
	}

//------------------------------------------------------------------------------
//	Draw pixels
//------------------------------------------------------------------------------

	glRasterPos2i( -1, -1 );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelZoom( width_PQ/( int )sX, height_PQ/( int )sY );
	glDrawPixels( sX, sY, GL_RGB, GL_FLOAT, spa );
	// Caution: This is important!!
	// Reset the zoom factor of pixel write operations
	glPixelZoom( 1.0, 1.0 );

	delete[] buf;
	delete[] spa;
    }

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_ALWAYS );
    glDisable( GL_BLEND );
    
//------------------------------------------------------------------------------
//	Axis and scale rendering
//------------------------------------------------------------------------------
#ifdef NIIDA_COMBINED_RANGES_FOR_P_AND_Q
    glColor3d( 1.0, 1.0, 1.0 );
    glLineWidth( 3.0 );
    glBegin( GL_LINES );
    glVertex2d( -1.0, -1.0 );
    glVertex2d( 1.0, 1.0 );
    glEnd();
#endif	// NIIDA_COMBINED_RANGES_FOR_P_AND_Q

    // axis name
    glColor3d( 0.0, 0.7, 0.7 );
    const double a = 0.90;
    const double b = 0.95;
#ifdef USE_FTGL
    vectorString(  a-0.05, -b+0.1, -0.1, 32, "P" );
    vectorString( -b,  a-0.05, -0.1, 32, "Q" );
#else	// USE_FTGL
    largeString(  a, -b+0.1, -0.1, "P" );
    largeString( -b,  a, -0.1, "Q" );
#endif	// USE_FTGL

    // glColor3d( 0.0, 0.0, 0.0 );
    // scale in P
#ifdef SKIP
    cerr << HERE << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cerr << HERE << " rangeP = " << rangeP << endl;
    cerr << HERE << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cerr << HERE << " rangeQ = " << rangeQ << endl;
#endif	// SKIP
    for ( int k = -nScales; k <= nScales; ++k ) {
	double ratio = ( double )( k + nScales )/( double )( 2*nScales );
//------------------------------------------------------------------------------
#ifdef NORMAL_DOSE_MAP
	double expP = ( 1.0 - ratio ) * rangeP.lower() + ratio * rangeP.upper();
	double valP = pow( 10.0, expP );
#endif	// NORMAL_DOSE_MAP
#ifdef RATIO_COMPARISON
	double valP = ( 1.0 - ratio ) * rangeP.lower() + ratio * rangeP.upper();
#endif	// RATIO_COMPARISON
//------------------------------------------------------------------------------

        //#ifdef SKIP
	// cerr << HERE << " k = " << k << " ratio = " << ratio << " expP = " << expP << " valP = " << valP << endl;
	cerr << HERE << " k = " << k << " ratio = " << ratio << " valP = " << valP << endl;
	//#endif	// SKIP
	double posP = ( double )k*pitchH - 0.1;
	ostringstream ostrP;

//------------------------------------------------------------------------------
#ifdef WITH_STANDARD_DEVIATION
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_TWO_RATIOS
//------------------------------------------------------------------------------

#ifdef USE_FTGL
	vectorString( posP, -b, -0.1, 16, ostrP.str().c_str() );
#else	// USE_FTGL
	smallString( posP, -b, -0.1, ostrP.str().c_str() );
#endif	// USE_FTGL
    }
    // scale in Q
    for ( int k = -nScales; k <= nScales; ++k ) {
	double ratio = ( double )( k + nScales )/( double )( 2*nScales );
	double valQ = ( 1.0 - ratio ) * rangeQ.lower() + ratio * rangeQ.upper();

        //#ifdef SKIP
	cerr << HERE << " k = " << k << " ratio = " << ratio << " valQ = " << valQ << endl;
	//#endif	// SKIP


	double posQ = ( double )k*pitchV - 0.05;
	ostringstream ostrQ;
//------------------------------------------------------------------------------
#ifdef WITH_STANDARD_DEVIATION
	ostrQ << boost::format( "%4.0f" ) % valQ << ends;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	ostrQ << boost::format( "%4.0f" ) % valQ << ends;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	ostrQ << boost::format( "%3.1f" ) % valQ << ends;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	ostrQ << boost::format( "%3.1f" ) % valQ << ends;
#endif	// WITH_TWO_RATIOS
//------------------------------------------------------------------------------

#ifdef USE_FTGL
	vectorString( -b, posQ, -0.1, 16, ostrQ.str().c_str() );
#else	// USE_FTGL
	smallString( -b, posQ, -0.1, ostrQ.str().c_str() );
#endif	// USE_FTGL
    }

    glDepthFunc( GL_LESS );
    glEnable( GL_BLEND );

    glutSwapBuffers();
}


void mouse_PQ( int press, int state, int x, int y )
{
    switch (press) {
      case GLUT_LEFT_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      left_PQ = true;
	  }
	  else {
	      // cerr << HERE << "Left mouse button released" << endl;
	      left_PQ = false;
	      // clearInverse();
	  }
	  break;
      case GLUT_MIDDLE_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      middle_PQ = true;
	  }
	  else {
	      middle_PQ = false;
	      clearInversePQ();
	  }
	  break;
      case GLUT_RIGHT_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      right_PQ = true;
	      mx_PQ = max( 0, min( x, width_PQ - 1 ) );
	      my_PQ = max( 0, min( y, height_PQ - 1 ) );
	      // cerr << HERE << " x = " << mx_fiber << " y = " << my_fiber << endl;
	      addToInversePQ( mx_PQ, my_PQ );
	  }
	  else {
	      // cerr << HERE << "Right mouse button released" << endl;
	      right_PQ = false;
	  }
	  break;
      default:
	  break;
    }
    redisplay_all();
    glutPostRedisplay();
}

void motion_PQ( int x, int y )
{
    int unitP = DEFAULT_WIDTH / curLevel->res().x();
    int unitQ = DEFAULT_HEIGHT / curLevel->res().y();
    int radiusP = unitP * PAINT_RADIUS;
    int radiusQ = unitQ * PAINT_RADIUS;
#ifdef DEBUG
    cerr << HERE << " unitP = " << unitP << " unitQ = " << unitQ
	 << " radiusP = " << radiusP << " radiusQ = " << radiusQ << endl;
#endif	// DEBUG
    if ( right_PQ ) {
	for ( int p = x-radiusP; p <= x+radiusP; p++ ) {
	    if ( ( 0 <= p ) && ( p < DEFAULT_WIDTH ) ) {
		// mx_PQ = max( 0, min( p, width_PQ - 1 ) );
		for ( int q = y-radiusQ; q <= y+radiusQ; q++ ) {
		    // my_PQ = max( 0, min( q, height_PQ - 1 ) );
		    // cerr << HERE << " x = " << mx_PQ << " y = " << my_PQ << endl;
		    if ( ( 0 <= q ) && ( q < DEFAULT_HEIGHT ) ) {
			// addToInversePQ( mx_PQ, my_PQ );
			addToInversePQ( p, q );
		    }
		}
	    }
	}
	redisplay_all();
    }
    glutPostRedisplay();
}


void visibility_PQ( int state )
{
    // cerr << HERE << " visibility_PQ " << endl;

    if ( state == GLUT_VISIBLE ) visible_PQ = true;
    if ( state == GLUT_NOT_VISIBLE ) visible_PQ = false;
}


// dump the window image
void capture_PQ( const char * name )
{
    static cv::Mat              image;
    static GLubyte		*data = NULL;
    /* round off height */
    int				h = ( int )height_PQ;
    /* round off width */
    int				w = ( int )width_PQ;
    const unsigned int		nChannels = 4;

    redisplay_all();

    glutSetWindow( win_PQ );
    if ( data == NULL ) data = new GLubyte [ w * h * nChannels ];
    glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );

    // image = cv::Mat( cv::Size( w, h ), CV_8UC3 );
    image = cv::Mat( cv::Size( w, h ), CV_8UC4 );
    memcpy( image.data, data, w * h * nChannels );
    cv::cvtColor( image, image, cv::COLOR_BGR2RGB );
    cv::flip( image, image, 0 );
    cv::imwrite( name, image );

    delete [] data;
    data = NULL;
    image.release();
    
    cerr << "Capturing the fiber window .. done" << endl;
}    


//------------------------------------------------------------------------------
//	PR window
//------------------------------------------------------------------------------
void display_PR( void )
{
    // cerr << HERE << " display_PR" << endl;

    // Is this necessary
    // glClearColor( 0.0, 0.0, 0.0, 0.0 );

    glViewport( 0, 0, width_PR, height_PR );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if ( ! is_displayed_PR ) is_displayed_PR = true;
    if ( ! is_processed ) return;

    if ( curLevel != NULL ) {

	unsigned int sX = curLevel->res().x();
	// unsigned int sY = curLevel->res().y();
	unsigned int sZ = curLevel->res().z();

	//array for display
	float * buf = new GLfloat[ sX * sZ * 3 ];
	// initialization
	for ( unsigned int i = 0; i < sX*sZ*3; ++i) {
	    buf[ i ] = 0.0f;
	}
#ifdef TENTATIVE_SKIP
	FiberMap & fiber = curLevel->fiberPR();
	setFiberColor( fiber, buf );
#endif	// TENTATIVE_SKIP

	// Singularity & sing = curLevel->singularPR();

	float * spa = new float[ sX * sZ * 3 ];
	for ( unsigned int i = 0; i < sX * sZ * 3; ++i ) {
	    spa[ i ] = 0.0f;
	}
    
	std::vector< SetFace > & comp = curLevel->compPR();
	// cerr << HERE << " comp.size = " << comp.size() << " dyadic = " << dyadic_size[ resID ] << endl;
	assert( comp.size() == sX * sZ );

	for ( unsigned int i = 0; i < sX*sZ; i++ ) {
	    unsigned int nComp = comp[ i ].size();
	    double ratio = 0.0;
	    ratio = ( sqrt(sX*sZ)/32.0 ) * ( ( double )nComp )/LIMIT_NUM_COMPS;
	    ratio = min( 1.0, max( 0.0, ratio ) );
	    if ( ratio == 0.0 ) {
		spa[ i * 3 + 0 ] = spa[ i * 3 + 1 ] = spa[ i * 3 + 2 ] = 0.0;
	    }
	    else {
		spa[ i * 3 + 0 ] = spa[ i * 3 + 1 ] = spa[ i * 3 + 2 ] = 
		    CONTRAST_INTENSITY * ratio + BASE_INTENSITY;
	    }
	}
	
//------------------------------------------------------------------------------
//	Triangle projections
//------------------------------------------------------------------------------
	// cerr << " Number of facets = " << ptrFacet.size() << endl;
	SetUInt sel;
	for ( unsigned int k = 0; k < ptrFacet.size(); ++k ) {
	    if ( ptrFacet[ k ]->flag() ) {
		for ( unsigned int m = 0; m < curLevel->projPR()[ k ].size(); m++ ) {
		    unsigned int id = curLevel->projPR()[ k ][ m ];
		    if ( ! binary_search( sel.begin(), sel.end(), id ) ) {
			SetUInt::iterator it = upper_bound( sel.begin(), sel.end(), id  );
			sel.insert( it, id );
		    }
		}
	    }
	}
#ifdef SKIP
	cerr << HERE << " Number of selected triangles in the map domain = "
	     << sel.size() << " / " << ptrFacet.size() << endl;
#endif	// SKIP
	    
//------------------------------------------------------------------------------
//	Pixel color adjustment
//------------------------------------------------------------------------------
	// assign green to the selected pixels
	const double t = BLENDING_RATIO;
	const double s = 1.0 - t;
	for ( unsigned int id =  0; id < sX*sZ; ++id ) {
	    // Copy original color to the target color
	    double tarColor[ 3 ];
	    for ( unsigned int k = 0; k < 3; ++k ) 
		tarColor[ k ] = spa[ id * 3 + k ];

	    // if the pixel is selected in range space, then assign green
	    if ( binary_search( pixelPR.begin(), pixelPR.end(), id ) )
		tarColor[ 1 ] = FULL_INTENSITY;
	    // if the pixel is selected in domain space, then assign purple
	    if ( binary_search( sel.begin(), sel.end(), id ) )
		tarColor[ 0 ] = tarColor[ 2 ] = MODERATE_INTENSITY;

	    // Adjust pixel color
	    for ( unsigned int k = 0; k < 3; ++k )
		spa[ id * 3 + k ] = t * tarColor[ k ] + s * spa[ id * 3 + k ];
	}

//------------------------------------------------------------------------------
//	Draw pixels
//------------------------------------------------------------------------------

	glRasterPos2i( -1, -1 );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelZoom( width_PR/( int )sX, height_PR/( int )sZ );
	glDrawPixels( sX, sZ, GL_RGB, GL_FLOAT, spa );
	// Caution: This is important!!
	// Reset the zoom factor of pixel write operations
	glPixelZoom( 1.0, 1.0 );

	delete[] buf;
	delete[] spa;
    }

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_ALWAYS );
    glDisable( GL_BLEND );

//------------------------------------------------------------------------------
//	Axis and scale rendering
//------------------------------------------------------------------------------
    // axis name
    // glColor3d( 1.0, 0.5, 0.0 );
    // glColor3d( 1.0, 0.0, 1.0 );
    glColor3d( 0.7, 0.0, 0.7 );
    const double a = 0.90;
    const double b = 0.95;
#ifdef USE_FTGL
    vectorString(  a-0.05, -b+0.1, -0.1, 32, "P" );
    vectorString( -b,  a-0.05, -0.1, 32, "R" );
#else	// USE_FTGL
    largeString(  a, -b+0.1, -0.1, "P" );
    largeString( -b,  a, -0.1, "R" );
#endif	// USE_FTGL
    glDepthFunc( GL_LESS );
    glEnable( GL_BLEND );

    // glColor3d( 0.0, 0.0, 0.0 );
    // scale in P
    for ( int k = -nScales; k <= nScales; ++k ) {
	double ratio = ( double )( k + nScales )/( double )( 2*nScales );
//------------------------------------------------------------------------------
#ifdef NORMAL_DOSE_MAP
	double expP = ( 1.0 - ratio ) * rangeP.lower() + ratio * rangeP.upper();
	double valP = pow( 10.0, expP );
#endif	// NORMAL_DOSE_MAP
#ifdef RATIO_COMPARISON
	double valP = ( 1.0 - ratio ) * rangeP.lower() + ratio * rangeP.upper();
#endif	// RATIO_COMPARISON
//------------------------------------------------------------------------------
	double posP = ( double )k*pitchH - 0.1;
	ostringstream ostrP;

//------------------------------------------------------------------------------
#ifdef WITH_STANDARD_DEVIATION
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	ostrP << boost::format( "%3.1f" ) % valP << ends;
#endif	// WITH_TWO_RATIOS
//------------------------------------------------------------------------------

#ifdef USE_FTGL
	vectorString( posP, -b, -0.1, 16, ostrP.str().c_str() );
#else	// USE_FTGL
	smallString( posP, -b, -0.1, ostrP.str().c_str() );
#endif	// USE_FTGL
    }
    // scale in R
    for ( int k = -nScales; k <= nScales; ++k ) {
	double ratio = ( double )( k + nScales )/( double )( 2*nScales );
	double valR = ( 1.0 - ratio ) * rangeR.lower() + ratio * rangeR.upper();
	double posR = ( double )k*pitchV - 0.05;
	ostringstream ostrR;

//------------------------------------------------------------------------------
#ifdef WITH_STANDARD_DEVIATION
	ostrR << boost::format( "%2.4f" ) % valR << ends;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	ostrR << boost::format( "%2.4f" ) % valR << ends;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	ostrR << boost::format( "%2.4f" ) % valR << ends;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	ostrR << boost::format( "%3.1f" ) % valR << ends;
#endif	// WITH_TWO_RATIOS
//------------------------------------------------------------------------------
	
#ifdef USE_FTGL
	vectorString( -b, posR, -0.1, 16, ostrR.str().c_str() );
#else	// USE_FTGL
	smallString( -b, posR, -0.1, ostrR.str().c_str() );
#endif	// USE_FTGL
    }

    glutSwapBuffers();
}


void mouse_PR( int press, int state, int x, int y )
{
    switch (press) {
      case GLUT_LEFT_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      left_PR = true;
	  }
	  else {
	      // cerr << HERE << "Left mouse button released" << endl;
	      left_PR = false;
	      // clearInverse();
	  }
	  break;
      case GLUT_MIDDLE_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      middle_PR = true;
	  }
	  else {
	      middle_PR = false;
	      clearInversePR();
	  }
	  break;
      case GLUT_RIGHT_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      right_PR = true;
	      mx_PR = max( 0, min( x, width_PR - 1 ) );
	      my_PR = max( 0, min( y, height_PR - 1 ) );
	      // cerr << HERE << " x = " << mx_fiber << " y = " << my_fiber << endl;
	      addToInversePR( mx_PR, my_PR );
	  }
	  else {
	      // cerr << HERE << "Right mouse button released" << endl;
	      right_PR = false;
	  }
	  break;
      default:
	  break;
    }
    redisplay_all();
    glutPostRedisplay();
}

void motion_PR( int x, int y )
{
    int unitP = DEFAULT_WIDTH / curLevel->res().x();
    int unitR = DEFAULT_HEIGHT / curLevel->res().z();
    int radiusP = unitP * PAINT_RADIUS;
    int radiusR = unitR * PAINT_RADIUS;
    if ( right_PR ) {
	for ( int p = x-radiusP; p <= x+radiusP; p++ ) {
	    if ( ( 0 <= p ) && ( p < DEFAULT_WIDTH ) ) {
		// mx_PR = max( 0, min( p, width_PR - 1 ) );
		for ( int r = y-radiusR; r <= y+radiusR; r++ ) {
		    // my_PR = max( 0, min( r, height_PR - 1 ) );
		    // cerr << HERE << " x = " << mx_PR << " y = " << my_PR << endl;
		    if ( ( 0 <= r ) && ( r < DEFAULT_HEIGHT ) ) {
			// addToInversePR( mx_PR, my_PR );
			addToInversePR( p, r );
		    }
		}
	    }
	}
	redisplay_all();
    }
    glutPostRedisplay();
}


void visibility_PR( int state )
{
    // cerr << HERE << " visibility_PR " << endl;

    if ( state == GLUT_VISIBLE ) visible_PR = true;
    if ( state == GLUT_NOT_VISIBLE ) visible_PR = false;
}


// dump the window image
void capture_PR( const char * name )
{
    static cv::Mat              image;
    static GLubyte		*data = NULL;
    /* round off height */
    int				h = ( int )height_PR;
    /* round off width */
    int				w = ( int )width_PR;
    const unsigned int		nChannels = 4;

    redisplay_all();

    glutSetWindow( win_PR );
    if ( data == NULL ) data = new GLubyte [ w * h * nChannels ];
    glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );

    // image = cv::Mat( cv::Size( w, h ), CV_8UC3 );
    image = cv::Mat( cv::Size( w, h ), CV_8UC4 );
    memcpy( image.data, data, w * h * nChannels );
    cv::cvtColor( image, image, cv::COLOR_BGR2RGB );
    cv::flip( image, image, 0 );
    cv::imwrite( name, image );
    
    delete [] data;
    data = NULL;
    image.release();

    cerr << "Capturing the fiber window .. done" << endl;
}    



//------------------------------------------------------------------------------
//	QR window
//------------------------------------------------------------------------------
void display_QR( void )
{
    // cerr << HERE << " display_QR" << endl;

    // Is this necessary
    // glClearColor( 0.0, 0.0, 0.0, 0.0 );

    glViewport( 0, 0, width_QR, height_QR );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    if ( ! is_displayed_QR ) is_displayed_QR = true;
    if ( ! is_processed ) return;
    
    if ( curLevel != NULL ) {

	// unsigned int sX = curLevel->res().x();
	unsigned int sY = curLevel->res().y();
	unsigned int sZ = curLevel->res().z();

	// array for display
	float * buf = new GLfloat[ sY * sZ * 3 ];
	// initialization
	for ( unsigned int i = 0; i < sY*sZ*3; ++i) {
	    buf[ i ] = 0.0f;
	}
#ifdef TENTATIVE_SKIP
	FiberMap & fiber = curLevel->fiberQR();
	setFiberColor( fiber, buf );
#endif	// TENTATIVE_SKIP

	// Singularity & sing = curLevel->singularQR();

	float * spa = new float[ sY * sZ * 3 ];
	for ( unsigned int i = 0; i < sY * sZ * 3; ++i ) {
	    spa[ i ] = 0.0f;
	}
    
	std::vector< SetFace > & comp = curLevel->compQR();
	// cerr << HERE << " comp.size = " << comp.size() << " dyadic = " << dyadic_size[ resID ] << endl;
	assert( comp.size() == sY * sZ );

	for ( unsigned int i = 0; i < sY*sZ; i++ ) {
	    unsigned int nComp = comp[ i ].size();
	    double ratio = 0.0;
	    ratio = ( sqrt(sY*sZ)/32.0 ) * ( ( double )nComp )/LIMIT_NUM_COMPS;
	    ratio = min( 1.0, max( 0.0, ratio ) );
	    if ( ratio == 0.0 ) {
		spa[ i * 3 + 0 ] = spa[ i * 3 + 1 ] = spa[ i * 3 + 2 ] = 0.0;
	    }
	    else {
		spa[ i * 3 + 0 ] = spa[ i * 3 + 1 ] = spa[ i * 3 + 2 ] = 
		    CONTRAST_INTENSITY * ratio + BASE_INTENSITY;
	    }
	}
	
//------------------------------------------------------------------------------
//	Triangle projections
//------------------------------------------------------------------------------
	// cerr << " Number of facets = " << ptrFacet.size() << endl;
	SetUInt sel;
	for ( unsigned int k = 0; k < ptrFacet.size(); ++k ) {
	    if ( ptrFacet[ k ]->flag() ) {
		// cerr << " Triangle " << k << " is ON. No. = " << curLevel->projQR()[ k ].size() << endl;
		for ( unsigned int m = 0; m < curLevel->projQR()[ k ].size(); m++ ) {
		    unsigned int id = curLevel->projQR()[ k ][ m ];
		    if ( ! binary_search( sel.begin(), sel.end(), id ) ) { 
			SetUInt::iterator it = upper_bound( sel.begin(), sel.end(), id );
			sel.insert( it, id );
		    }
		}
	    }
	}
#ifdef SKIP
	cerr << HERE << " Number of selected triangles in the map domain = "
	     << sel.size() << " / " << ptrFacet.size() << endl;
#endif	// SKIP

//------------------------------------------------------------------------------
//	Pixel color adjustment
//------------------------------------------------------------------------------
	// assign red to the selected pixels
	const double t = BLENDING_RATIO;
	const double s = 1.0 - t;
	for ( unsigned int id = 0; id < sY*sZ; ++id ) {
	    // Copy original color to the target color
	    double tarColor[ 3 ];
	    for ( unsigned int k = 0; k < 3; ++k ) 
		tarColor[ k ] = spa[ id * 3 + k ];

	    // if the pixel is selected in range space, then assign blue
	    if ( binary_search( pixelQR.begin(), pixelQR.end(), id ) ) {
		tarColor[ 2 ] = FULL_INTENSITY;
		tarColor[ 0 ] = tarColor[ 1 ] = AUXILIARY_INTENSITY;
	    }
	    // if the pixel is selected in domain space, then assign yellow
	    if ( binary_search( sel.begin(), sel.end(), id ) )
		tarColor[ 0 ] = tarColor[ 1 ] = MODERATE_INTENSITY;

	    // Adjust pixel color
	    for ( unsigned int k = 0; k < 3; ++k )
		spa[ id * 3 + k ] = t * tarColor[ k ] + s * spa[ id * 3 + k ];
	}

//------------------------------------------------------------------------------
//	Draw pixels
//------------------------------------------------------------------------------

	glRasterPos2i( -1, -1 );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelZoom( width_QR/( int )sY, height_QR/( int )sZ );
	glDrawPixels( sY, sZ, GL_RGB, GL_FLOAT, spa );
	// Caution: This is important!!
	// Reset the zoom factor of pixel write operations
	glPixelZoom( 1.0, 1.0 );

	delete[] buf;
	delete[] spa;
    }

    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_ALWAYS );
    glDisable( GL_BLEND );

//------------------------------------------------------------------------------
//	Axis and scale rendering
//------------------------------------------------------------------------------
    // axis name
    glColor3d( 0.7, 0.7, 0.0 );
    const double a = 0.90;
    const double b = 0.95;
#ifdef USE_FTGL
    vectorString(  a-0.05, -b+0.1, -0.1, 32, "Q" );
    vectorString( -b,  a-0.05, -0.1, 32, "R" );
#else	// USE_FTGL
    largeString(  a, -b+0.1, -0.1, "Q" );
    largeString( -b,  a, -0.1, "R" );
#endif	// USE_FTGL

    // glColor3d( 0.0, 0.0, 0.0 );
    // scale in Q
    for ( int k = -nScales; k <= nScales; ++k ) {
	double ratio = ( double )( k + nScales )/( double )( 2*nScales );
	double valQ = ( 1.0 - ratio ) * rangeQ.lower() + ratio * rangeQ.upper();
	double posQ = ( double )k*pitchH - 0.1;
	ostringstream ostrQ;
//------------------------------------------------------------------------------
#ifdef WITH_STANDARD_DEVIATION
	ostrQ << boost::format( "%4.0f" ) % valQ << ends;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	ostrQ << boost::format( "%4.0f" ) % valQ << ends;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	ostrQ << boost::format( "%3.1f" ) % valQ << ends;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	ostrQ << boost::format( "%3.1f" ) % valQ << ends;
#endif	// WITH_TWO_RATIOS
//------------------------------------------------------------------------------

#ifdef USE_FTGL
	vectorString( posQ, -b, -0.1, 16, ostrQ.str().c_str() );
#else	// USE_FTGL
	smallString( posQ, -b, -0.1, ostrQ.str().c_str() );
#endif	// USE_FTGL
    }
    // scale in R
    for ( int k = -nScales; k <= nScales; ++k ) {
	double ratio = ( double )( k + nScales )/( double )( 2*nScales );
	double valR = ( 1.0 - ratio ) * rangeR.lower() + ratio * rangeR.upper();
	double posR = ( double )k*pitchV - 0.05;
	ostringstream ostrR;

	//------------------------------------------------------------------------------
#ifdef WITH_STANDARD_DEVIATION
	ostrR << boost::format( "%2.4f" ) % valR << ends;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	ostrR << boost::format( "%2.4f" ) % valR << ends;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	ostrR << boost::format( "%2.4f" ) % valR << ends;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	ostrR << boost::format( "%3.1f" ) % valR << ends;
#endif	// WITH_TWO_RATIOS
//------------------------------------------------------------------------------

#ifdef USE_FTGL
	vectorString( -b, posR, -0.1, 16, ostrR.str().c_str() );
#else	// USE_FTGL
	smallString( -b, posR, -0.1, ostrR.str().c_str() );
#endif	// USE_FTGL
    }

    glDepthFunc( GL_LESS );
    glEnable( GL_BLEND );

    glutSwapBuffers();
}


void mouse_QR( int press, int state, int x, int y )
{
    switch (press) {
      case GLUT_LEFT_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      left_QR = true;
	  }
	  else {
	      // cerr << HERE << "Left mouse button released" << endl;
	      left_QR = false;
	      // clearInverse();
	  }
	  break;
      case GLUT_MIDDLE_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      middle_QR = true;
	  }
	  else {
	      middle_QR = false;
	      clearInverseQR();
	  }
	  break;
      case GLUT_RIGHT_BUTTON:
	  if ( state == GLUT_DOWN ) {
	      right_QR = true;
	      mx_QR = max( 0, min( x, width_QR - 1 ) );
	      my_QR = max( 0, min( y, height_QR - 1 ) );
	      // cerr << HERE << " x = " << mx_fiber << " y = " << my_fiber << endl;
	      addToInverseQR( mx_QR, my_QR );
	  }
	  else {
	      // cerr << HERE << "Right mouse button released" << endl;
	      right_QR = false;
	  }
	  break;
      default:
	  break;
    }
    redisplay_all();
    glutPostRedisplay();
}

void motion_QR( int x, int y )
{
    int unitQ = DEFAULT_WIDTH / curLevel->res().x();
    int unitR = DEFAULT_HEIGHT / curLevel->res().y();
    int radiusQ = unitQ * PAINT_RADIUS;
    int radiusR = unitR * PAINT_RADIUS;
    if ( right_QR ) {
	for ( int q = x-radiusQ; q <= x+radiusQ; q++ ) {
	    if ( ( 0 <= q ) && ( q < DEFAULT_WIDTH ) ) {
		// mx_QR = max( 0, min( q, width_QR - 1 ) );
		for ( int r = y-radiusR; r <= y+radiusR; r++ ) {
		    // my_QR = max( 0, min( r, height_QR - 1 ) );
		    // cerr << HERE << " x = " << mx_QR << " y = " << my_QR << endl;
		    if ( ( 0 <= r ) && ( r < DEFAULT_HEIGHT ) ) {
			// addToInverseQR( mx_QR, my_QR );
			addToInverseQR( q, r );
		    }
		}
	    }
	}
	redisplay_all();
    }
    glutPostRedisplay();
}


void visibility_QR( int state )
{
    // cerr << HERE << " visibility_QR " << endl;

    if ( state == GLUT_VISIBLE ) visible_QR = true;
    if ( state == GLUT_NOT_VISIBLE ) visible_QR = false;
}


// dump the window image
void capture_QR( const char * name )
{
    static cv::Mat              image;
    static GLubyte		*data = NULL;
    /* round off height */
    int				h = ( int )height_QR;
    /* round off width */
    int				w = ( int )width_QR;
    const unsigned int		nChannels = 4;

    redisplay_all();

    glutSetWindow( win_QR );
    if ( data == NULL ) data = new GLubyte [ w * h * nChannels ];
    glReadPixels( 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, data );

    // image = cv::Mat( cv::Size( w, h ), CV_8UC3 );
    image = cv::Mat( cv::Size( w, h ), CV_8UC4 );
    memcpy( image.data, data, w * h * nChannels );
    cv::cvtColor( image, image, cv::COLOR_BGR2RGB );
    cv::flip( image, image, 0 );
    cv::imwrite( name, image );

    delete [] data;
    data = NULL;
    image.release();

    cerr << "Capturing the fiber window .. done" << endl;
}    


//------------------------------------------------------------------------------
//	Handling the main window
//------------------------------------------------------------------------------
void perform_task( int mode )
{
    // #ifdef SKIP
    cerr << HERE << " check item = " << mode << endl;
    cerr << HERE << " grid switch = " << gridSwitch << endl;
    cerr << HERE << " outline switch = " << outlineSwitch << endl;
    cerr << HERE << " fill switch = " << fillSwitch << endl;
    cerr << HERE << " forest bit = " << forestBit << endl;
    cerr << HERE << " farm bit = " << farmBit << endl;
    cerr << HERE << " urban bit = " << urbanBit << endl;
    cerr << HERE << " ocean bit = " << oceanBit << endl;
    cerr << HERE << " curLevel = " << curLevel << endl;
    cerr << HERE << " ##############################" << endl;
    // #endif	// SKIP

    switch ( mode ) {
    /* int id,i; */
      case PROJECT_TRIANGLES:
	  {
	      isProjected[ valueID ] = false;
	      rasterization();
      //------------------------------------------------------------------------------
      //	Downsampling
      //------------------------------------------------------------------------------
#ifdef NO_NEED
	      // unsigned int resP = curLevel->res().x();
	      // unsigned int resQ = curLevel->res().y();
	      // unsigned int resR = curLevel->res().z();
#endif	// NO_NEED
	      unsigned int resP, resQ, resR;
	      int n = gridSwitch + 2;
	      resP = resQ = resR = power( 2, n );
	      cerr << HERE << " resP = " << resP << " resQ = " << resQ
		   << " resR = " << resR << endl;

	      Tuple3 key = Tuple3( resP, resQ, resR );
	      adaptiveSampling( key, meshMap );
	  
	      left_PQ = left_PR = left_QR = false;
	      clearInversePQ();
	      clearInversePR();
	      clearInverseQR();
	      clearInverseAll();
	  }
	  break;
      case VALUE_TYPE_UPDATE:
	  {
	      Tuple3 key = curLevel->res();
	      valueID = stepSwitch;
	      cerr << HERE << " valueID = " << valueID << endl;
	      isProjected[ valueID ] = false;
	      rasterization();
	      adaptiveSampling( key, meshMap );
	      
	      left_PQ = left_PR = left_QR = false;
	      clearInversePQ();
	      clearInversePR();
	      clearInverseQR();
	      clearInverseAll();
	  }
	  break;
      case LAND_FILTER:
	  //------------------------------------------------------------------------------
	  if ( forestBit ) {
	      landflag.setByType( FOREST );
	  }
	  else {
	      landflag.clearByType( FOREST );
	  }
	  //------------------------------------------------------------------------------
	  if ( farmBit ) {
	      landflag.setByType( RICE );
	      landflag.setByType( FARM );
	  }
	  else {
	      landflag.clearByType( RICE );
	      landflag.clearByType( FARM );
	  }
	  //------------------------------------------------------------------------------
	  if ( urbanBit ) {
	      landflag.setByType( URBAN );
	      landflag.setByType( ROAD );
	      landflag.setByType( RAILWAY );
	  }
	  else {
	      landflag.clearByType( URBAN );
	      landflag.clearByType( ROAD );
	      landflag.clearByType( RAILWAY );
	  }
	  //------------------------------------------------------------------------------
	  if ( oceanBit ) {
	      landflag.setByType( MARSH );
	      landflag.setByType( BEACH );
	      landflag.setByType( OCEAN );
	  }
	  else {
	      landflag.clearByType( MARSH );
	      landflag.clearByType( BEACH );
	      landflag.clearByType( OCEAN );
	  }
	  valueID = stepSwitch;
	  isProjected[ valueID ] = false;
	  switchFlag = true;
	  break;
      case UPDATE_LOWER:
	  cerr << HERE << " UPDATE_LOWER : "
	       << " lowerSwitch = " << lowerSwitch
	       << " upperSwitch = " << upperSwitch << endl;
	  if ( lowerSwitch - 1 > upperSwitch )
	      upperSwitch = lowerSwitch - 1;
	  cerr << HERE << " UPDATE_LOWER : "
	       << " lowerSwitch = " << lowerSwitch
	       << " upperSwitch = " << upperSwitch << endl;
	  initGLUI();
	  break;
      case UPDATE_UPPER:
	  cerr << HERE << " UPDATE_UPPER : "
	       << " lowerSwitch = " << lowerSwitch
	       << " upperSwitch = " << upperSwitch << endl;
	  if ( lowerSwitch > upperSwitch + 1 )
	      lowerSwitch = upperSwitch + 1;
	  cerr << HERE << " UPDATE_UPPER : "
	       << " lowerSwitch = " << lowerSwitch
	       << " upperSwitch = " << upperSwitch << endl;
	  initGLUI();
	  break;
      case DIST_SEGMENT:
	  cerr << HERE << " DIST_SEGMENT : "
	       << " lowerSwitch = " << lowerSwitch
	       << " upperSwitch = " << upperSwitch << endl;
	  cerr << HERE << " DIST_SEGMENT : "
	       << " lowerLimit = " << lowerDistLimit
	       << " upperLimit = " << upperDistLimit << endl;
	  lowerDistLimit =   20.0 * ( double )lowerSwitch;
	  upperDistLimit =   20.0 * ( double )( upperSwitch + 1 );
	  if ( upperSwitch == MAX_UPPER_LIMIT )
	      upperDistLimit = 10000.0; // infity
	  distFlag = true;
	  switchFlag = true;
	  cerr << HERE << " DIST_SEGMENT : "
	       << " lowerLimit = " << lowerDistLimit
	       << " upperLimit = " << upperDistLimit << endl;
	  break;
      case CHANGE_FACTOR:
	  current_factor = factorSwitch;
	  cerr << HERE << " current sampling factor = " << current_factor << endl;
	  break;
      case DOWNSAMPLE_MAP:
	  sparseSampling( entire, region, current_factor );
	  distFlag = true;
	  switchFlag = true;
	  break;
      case RESAMPLE_MAP:
	  keyboard_shape( 'r', 0, 0 );
	  distFlag = true;
	  switchFlag = true;
	  break;
      case CSV_OUTPUT:
	  keyboard_shape( 'c', 0, 0 );
	  break;
      case RENDER_MAP:
	  flag_outline = ( outlineSwitch == 1 );
	  flag_fill = ( fillSwitch == 1 );
	  if ( flag_outline ) cerr << HERE << " Outline ON " << endl;
	  else cerr << HERE << " Outline OFF" << endl;
	  if ( flag_fill ) cerr << HERE << " Fill ON " << endl;
	  else cerr << HERE << " Fill OFF" << endl;
	  break;
      case TRANSFORM_MAP:
	  initGLUI();
	  break;
      case QUIT:
	  exit( 0 );
	  break;
      default:
	  break;
    }

    redisplay_all();
}


/* handling the menu selection */
void select_item( int mode )
{
    char buf[ 256 ];
    char filename[ 256 ];

    /* int id,i; */
    if (
	( ( RESOLUTION_BASE_ALL <= mode ) && ( mode < RESOLUTION_BASE_ALL+NUM_LEVELS ) ) ||
	( ( RESOLUTION_BASE_P   <= mode ) && ( mode < RESOLUTION_BASE_P+NUM_LEVELS ) ) ||
	( ( RESOLUTION_BASE_Q   <= mode ) && ( mode < RESOLUTION_BASE_Q+NUM_LEVELS ) ) ||
	( ( RESOLUTION_BASE_R   <= mode ) && ( mode < RESOLUTION_BASE_R+NUM_LEVELS ) )
	) {
	rasterization();
	//------------------------------------------------------------------------------
	//	Downsampling
	//------------------------------------------------------------------------------
	cerr << HERE << " curLevel = " << curLevel << endl;
	unsigned int resP = curLevel->res().x();
	unsigned int resQ = curLevel->res().y();
	unsigned int resR = curLevel->res().z();
	cerr << HERE << " resP = " << resP << " resQ = " << resQ << " resR = " << resR << endl;
	Tuple3 key;
	if ( ( RESOLUTION_BASE_ALL <= mode ) && ( mode < RESOLUTION_BASE_ALL+NUM_LEVELS ) ) {
	    unsigned int n = mode - RESOLUTION_BASE_ALL;
	    resP = resQ = resR = power( 2, n );
	    key = Tuple3( resP, resQ, resR );
	}
	else if ( ( RESOLUTION_BASE_P <= mode ) && ( mode < RESOLUTION_BASE_P+NUM_LEVELS ) ) {
	    unsigned int n = mode - RESOLUTION_BASE_P;
	    resP = power( 2, n );
	    key = Tuple3( resP, resQ, resR );
	}
	else if ( ( RESOLUTION_BASE_Q <= mode ) && ( mode < RESOLUTION_BASE_Q+NUM_LEVELS ) ) {
	    unsigned int n = mode - RESOLUTION_BASE_Q;
	    resQ = power( 2, n );
	    key = Tuple3( resP, resQ, resR );
	}
	else if ( ( RESOLUTION_BASE_R <= mode ) && ( mode < RESOLUTION_BASE_R+NUM_LEVELS ) ) {
	    unsigned int n = mode - RESOLUTION_BASE_R;
	    resR = power( 2, n );
	    key = Tuple3( resP, resQ, resR );
	}
	adaptiveSampling( key, meshMap );

	left_PQ = left_PR = left_QR = false;
	clearInversePQ();
	clearInversePR();
	clearInverseQR();
	clearInverseAll();

	// display_mode = 1;
	// view_distance	= 5.5;
	// view_azimuth	= 0.0;	/*179.429688;*///45.0;   /*angle of object*/
	// view_incidence	= 0.0; /*90.484375*/ //60.0;
	// view_twist	= 0.0;
	redisplay_all();
    }
    /* int id,i; */
    else if ( ( VALUE_TYPE_0 <= mode ) && ( mode < VALUE_TYPE_0 + NUM_VALUES ) ) {
	Tuple3 key = curLevel->res();
	valueID = ( unsigned int )( mode - VALUE_TYPE_0 );
	// rasflag = false;
	rasterization();
	// rasflag = true;
	adaptiveSampling( key, meshMap );
	redisplay_all();
    }
    else {
	switch( mode ) {
	  case FLAT_MODE:
	      display_mode = 0;
	      break;
	  case BASEMENT_MODE:
	      display_mode = 1;
	      break;
	  case WIREFRAME_MODE:
	      display_mode = 2;
	      break;
	  case TEXTURE_MODE:
	      display_mode = 3;
	      break;
	      // drawing mode
	  case OUTLINE_ON:
	      flag_outline = true;
	      break;
	  case OUTLINE_OFF:
	      flag_outline = false;
	      break;
	  case FILL_ON:
	      flag_fill = true;
	      break;
	  case FILL_OFF:
	      flag_fill = false;
	      break;
	  case COMMON_ON:
	      flag_all = true;
	      break;
	  case COMMON_OFF:
	      flag_all = false;
	      break;
//------------------------------------------------------------------------------
//	LAND USE TYPE
//------------------------------------------------------------------------------
	  case ALL_ON:
	      landflag.setByType( RICE );
	      landflag.setByType( FARM );
	      landflag.setByType( FOREST );
	      landflag.setByType( WASTE );
	      landflag.setByType( URBAN );
	      landflag.setByType( ROAD );
	      landflag.setByType( RAILWAY );
	      landflag.setByType( OTHER );
	      landflag.setByType( MARSH );
	      landflag.setByType( BEACH );
	      landflag.setByType( OCEAN );
	      landflag.setByType( GOLF );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case ALL_OFF:
	      landflag.clearByType( RICE );
	      landflag.clearByType( FARM );
	      landflag.clearByType( FOREST );
	      landflag.clearByType( WASTE );
	      landflag.clearByType( URBAN );
	      landflag.clearByType( ROAD );
	      landflag.clearByType( RAILWAY );
	      landflag.clearByType( OTHER );
	      landflag.clearByType( MARSH );
	      landflag.clearByType( BEACH );
	      landflag.clearByType( OCEAN );
	      landflag.clearByType( GOLF );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
//------------------------------------------------------------------------------
	  case RICE_ON:
	      landflag.setByType( RICE );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case RICE_OFF:
	      landflag.clearByType( RICE );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case FARM_ON:
	      landflag.setByType( FARM );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case FARM_OFF:
	      landflag.clearByType( FARM );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case FOREST_ON:
	      landflag.setByType( FOREST );
	      cerr << HERE << endl << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case FOREST_OFF:
	      landflag.clearByType( FOREST );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case WASTE_ON:
	      landflag.setByType( WASTE );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case WASTE_OFF:
	      landflag.clearByType( WASTE );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
//------------------------------------------------------------------------------
	  case URBAN_ON:
	      landflag.setByType( URBAN );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case URBAN_OFF:
	      landflag.clearByType( URBAN );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case ROAD_ON:
	      landflag.setByType( ROAD );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case ROAD_OFF:
	      landflag.clearByType( ROAD );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case RAILWAY_ON:
	      landflag.setByType( RAILWAY );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case RAILWAY_OFF:
	      landflag.clearByType( RAILWAY );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case MARSH_ON:
	      landflag.setByType( MARSH );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case MARSH_OFF:
	      landflag.clearByType( MARSH );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
//------------------------------------------------------------------------------
	  case BEACH_ON:
	      landflag.setByType( BEACH );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case BEACH_OFF:
	      landflag.clearByType( BEACH );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case OCEAN_ON:
	      landflag.setByType( OCEAN );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case OCEAN_OFF:
	      landflag.clearByType( OCEAN );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case GOLF_ON:
	      landflag.setByType( GOLF );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case GOLF_OFF:
	      landflag.clearByType( GOLF );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case OTHER_ON:
	      landflag.setByType( OTHER );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
	  case OTHER_OFF:
	      landflag.clearByType( OTHER );
	      cerr << HERE << landflag << endl;
	      isProjected[ valueID ] = false;
	      switchFlag = true;
	      break;
//------------------------------------------------------------------------------
	  case DIST_LIMIT_000_020:
	      lowerDistLimit =   0.0;
	      upperDistLimit =  20.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_000_040:
	      lowerDistLimit =   0.0;
	      upperDistLimit =  40.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_000_060:
	      lowerDistLimit =   0.0;
	      upperDistLimit =  60.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_000_080:
	      lowerDistLimit =   0.0;
	      upperDistLimit =  80.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_000_100:
	      lowerDistLimit =   0.0;
	      upperDistLimit = 100.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_020_040:
	      lowerDistLimit =  20.0;
	      upperDistLimit =  40.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_040_060:
	      lowerDistLimit =  40.0;
	      upperDistLimit =  60.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_060_080:
	      lowerDistLimit =  60.0;
	      upperDistLimit =  80.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_080_100:
	      lowerDistLimit =  80.0;
	      upperDistLimit = 100.0;
	      distFlag = true;
	      switchFlag = true;
	      break;
	  case DIST_LIMIT_OFF:
	      lowerDistLimit =    0.0;
	      upperDistLimit = 10000.0;
	      distFlag = false;
	      switchFlag = true;
	      break;
//------------------------------------------------------------------------------
	      //    case PLOT_MODE:
	      //        display_mode = 3;
	      //        break;
#ifdef SKIP
	  case SWITCH_RASTER_MODE:
	      raster_mode = 3 - raster_mode;
	      cerr << " Raster mode = " << raster_mode << endl;
	      break;
	      //    case PLOT_MODE:
	      //        display_mode = 3;
	      //        break;
#endif	// SKIP
	  case INIT_VIEWPOINT:
	      view_distance	= 5.5;
	      view_azimuth	= 0.0;	/*179.429688;*///45.0;   /*angle of object*/
	      view_incidence	= 0.0; /*90.484375*/ //60.0;
	      view_twist	= 0.0;
	      break;
	  case CHANGE_VIEWPOINT:
	      fprintf( stderr,"Input view parameters: (distance = %6.3f):", view_distance );
	      fgets( buf, sizeof(buf), stdin );
	      sscanf( buf, "%lf", &view_distance );
	      fprintf( stderr,"Input view parameters: (incidence = %6.3f):", view_incidence );
	      fgets( buf, sizeof(buf), stdin );
	      sscanf( buf, "%lf", &view_incidence );
	      fprintf( stderr,"Input view parameters: (azimuth = %6.3f):", view_azimuth );
	      fgets( buf, sizeof(buf), stdin );
	      sscanf( buf, "%lf", &view_azimuth );
	      break;
	  case DUMP_SELECTED:
	      fprintf( stderr, " Input file name : " );
	      fgets( buf, sizeof( buf ), stdin );
	      sscanf( buf, "%s", filename );
	      dumpSelected( filename,
			    coordX, coordY,
			    attrP, attrQ, attrR,
			    rangeX, rangeY,
			    rangeP, rangeQ, rangeR );
	      return;
	  case WIREFRAME_EPS:
	      fprintf( stderr, " Input file name : " );
	      fgets( buf, sizeof( buf ), stdin );
	      sscanf( buf, "%s", filename );
	      wireframe_eps( filename );
	      return;
	  case CAPTURE_SHAPE:
	      fprintf( stderr, " Input file name : " );
	      fgets( buf, sizeof( buf ), stdin );
	      sscanf( buf, "%s", filename );
	      capture_shape( filename );
	      return;
	  case CAPTURE_RANGE:
	      fprintf( stderr, " Input file name : " );
	      fgets( buf, sizeof( buf ), stdin );
	      sscanf( buf, "%s", filename );
	      capture_range( filename );
	      return;
	  case CAPTURE_PQ:
	      fprintf( stderr, " Input file name : " );
	      fgets( buf, sizeof( buf ), stdin );
	      sscanf( buf, "%s", filename );
	      capture_PQ( filename );
	      return;
	  case CAPTURE_PR:
	      fprintf( stderr, " Input file name : " );
	      fgets( buf, sizeof( buf ), stdin );
	      sscanf( buf, "%s", filename );
	      capture_PR( filename );
	      return;
	  case CAPTURE_QR:
	      fprintf( stderr, " Input file name : " );
	      fgets( buf, sizeof( buf ), stdin );
	      sscanf( buf, "%s", filename );
	      capture_QR( filename );
	      return;
	  case CAPTURE_ALL:
	      {
		  ostringstream ostr;
		  string strbuf;
		  cerr << " Input file name: ";
		  getline( cin, strbuf );

		  ostr << strbuf << "_shape.png" << ends;
		  cerr << " Saving file: " << ostr.str().c_str() << " : ";
		  capture_shape( ostr.str().c_str() );
		  ostr.str("");

		  ostr << strbuf << "_range.png" << ends;
		  cerr << " Saving file: " << ostr.str().c_str() << " : ";
		  capture_range( ostr.str().c_str() );
		  ostr.str("");

		  ostr << strbuf << "_PQ.png" << ends;
		  cerr << " Saving file: " << ostr.str().c_str() << " : ";
		  capture_PQ( ostr.str().c_str() );
		  ostr.str("");

		  ostr << strbuf << "_PR.png" << ends;
		  cerr << " Saving file: " << ostr.str().c_str() << " : ";
		  capture_PR( ostr.str().c_str() );
		  ostr.str("");

		  ostr << strbuf << "_QR.png" << ends;
		  cerr << " Saving file: " << ostr.str().c_str() << " : ";
		  capture_QR( ostr.str().c_str() );
		  ostr.str("");
	      }
	      break;
	  case QUIT:
	      exit( 0 );
	      break;
	  default:
	      break;
	}
    }
    
    redisplay_all();
    // glutPostRedisplay();
}


void sparseSampling( ublas::matrix< double > & from,
		     ublas::matrix< double > & to, unsigned int factor )
{
    cerr << HERE << "########## Down sampling factor = " << factor << endl;
    // getchar();
    
    cerr << HERE << " Downsampling ..." << ends;
    cerr << " Original samples = " << from.size1() << ends;
    downSample( from, to, ( unsigned int )factor );
    cerr << " Reduced samples = " << to.size1() << ends;
    cerr << " ... finished" << endl;
    // from.clear();

#ifdef SKIP
    cerr << HERE << " to[ 0 ] = " << ends;
    for ( unsigned int k = 0; k < to.size2(); ++k ) {
	cerr << to( 0, k ) << " , ";
    }
    cerr << endl;
#endif	// SKIP
}


void autoTriangulateField( const ublas::matrix< double > & plot,
			   interval< double > & rngH,
			   interval< double > & rngD,
			   interval< double > & rngG,
			   interval< double > & rngPriAve,
			   interval< double > & rngPosAve,
			   interval< double > & rngPriSdv,
			   interval< double > & rngPosSdv,
			   interval< double > & rngRatioF,
			   interval< double > & rngRatioM,
			   interval< double > & rngRatioL,
			   interval< double > & rngP,
			   interval< double > & rngQ,
			   interval< double > & rngR,
			   Height * H )
{
//------------------------------------------------------------------------------
//	triangulating the height field 
//------------------------------------------------------------------------------
    // Sample points are noramlized and then incorporated the Delaunay
    // triangulation of the height field
    H->autoTessellate( plot,
		       rngH, rngD, rngG,
		       rngPriAve, rngPosAve,
		       rngPriSdv, rngPosSdv,
		       rngRatioF, rngRatioM, rngRatioL,
		       rngP, rngQ, rngR );
    cerr << HERE << " H->rngX = " << H->rngX() << endl;
    cerr << HERE << " H->rngY = " << H->rngY() << endl;
    domain = P_Bbox2( H->rngX().lower(), H->rngY().lower() ,
		      H->rngX().upper(), H->rngY().upper() );
    // attrQ is expected to represent the height
}


void manualTriangulateField( const ublas::matrix< double > & plot,
			     interval< double > & rngH,
			     interval< double > & rngD,
			     interval< double > & rngG,
			     interval< double > & rngPriAve,
			     interval< double > & rngPosAve,
			     interval< double > & rngPriSdv,
			     interval< double > & rngPosSdv,
			     interval< double > & rngRatioF,
			     interval< double > & rngRatioM,
			     interval< double > & rngRatioL,
			     interval< double > & rngP,
			     interval< double > & rngQ,
			     interval< double > & rngR,
			     Height * H )
{
//------------------------------------------------------------------------------
//	triangulating the height field 
//------------------------------------------------------------------------------
    // Sample points are noramlized and then incorporated the Delaunay
    // triangulation of the height field
    H->manualTessellate( plot,
			 rngH, rngD, rngG,
			 rngPriAve, rngPosAve,
			 rngPriSdv, rngPosSdv,
			 rngRatioF, rngRatioM, rngRatioL,
			 rngP, rngQ, rngR );

    cerr << HERE << " H->rngX = " << H->rngX() << endl;
    cerr << HERE << " H->rngY = " << H->rngY() << endl;
    domain = P_Bbox2( H->rngX().lower(), H->rngY().lower() ,
		      H->rngX().upper(), H->rngY().upper() );
    // attrQ is expected to represent the height
}


double estimateRadius( Height * H, const double & x, const double & y )
{
    const int nNeighbors = 3;
    D_Point2 ref( x, y );
    static list< D_Vertex_handle > LV;
    // static double maxDist = 0.0;
    
    LV.clear();
    CGAL::nearest_neighbors( H->T(), ref, nNeighbors, std::back_inserter( LV ) );
    double factor = 0.0;
#ifdef WITH_REDUCTION_RATIO
    double dose = 0.0;
#endif	// WITH_REDUCTION_RATIO
    double minDist = INFINITY;
    for ( list< D_Vertex_handle >::iterator it = LV.begin(); it != LV.end(); it++ ) {
	unsigned int curID = (*it)->id();
	D_Point2 plot = (*it)->point();
	double curDist = ( plot - ref ).squared_length();
	if ( curDist < minDist ) minDist = curDist;
#ifdef WITH_STANDARD_DEVIATION
	double curFactor = H->attrPosSdv()[ curID ];
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	int curType = H->attrLand()[ curID ];
	double curFactor = 0.6;
	if ( ( curType == 700 ) || ( curType == 901 ) || ( curType == 902 ) ) {
	    curFactor = 0.1;
	}
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	double curFactor = 1.0 - H->attrRatioM()[ curID ];
	double curDose = H->attrPosAve()[ curID ];
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	double curFactor = H->attrPosSdv()[ curID ];
#endif	// WITH_TWO_RATIOS
	
#ifdef SKIP
	cerr << setw( 5 ) << curID << " ( "
	     << fixed << setprecision( 6 ) << plot.x() << " , "
	     << fixed << setprecision( 6 ) << plot.y() << " ) => "
	     << H->attrPriAve()[ curID ] << ", "
	     << H->attrPosAve()[ curID ] << ", "
	     << H->attrPriSdv()[ curID ] << ", "
	     << H->attrPosSdv()[ curID ] << ", "
	     << H->attrRatio()[ curID ] << ": " << endl;
#endif	// SKIP
	
	factor += curFactor;
#ifdef WITH_REDUCTION_RATIO
	dose += curDose;
#endif	// WITH_REDUCTION_RATIO
    }
    factor /= ( double )nNeighbors;
#ifdef WITH_REDUCTION_RATIO
    dose /= ( double )nNeighbors;
#endif	// WITH_REDUCTION_RATIO
    
    // 0.0001 -> 0.00001
    if ( minDist > 0.00001 ) {
	factor = 10.0;
    }

#ifdef WITH_REDUCTION_RATIO
    //if ( dose < 0.05 ) {
    //factor = std::min( factor, 0.5 );
    //}
#endif	// WITH_REDUCTION_RATIO
    
    // if ( factor < 10.0 )
    // cerr << HERE << " factor = " << factor << " " << ends;

    // This should stay within [0.0, 1.0]
    return ( factor );
}

double uniformRadius( Height * H, const double & x, const double & y )
{
    return 0.1;
    // return 0.2;
}

void poissonDisk( Height * H, std::vector< P_Point2 > & prior )
{
    // For dense sampling
    int nX = ( int )( POISSON_SAMPLING_SCALE * width_shape );
    int nY = ( int )( POISSON_SAMPLING_SCALE * height_shape );
    // For visually appealing sampling
    // int nX = width_shape; // <==
    // int nY = height_shape; // <==
    // int nX = width_shape / 2;
    // int nY = height_shape / 2;
    // int nX = 5;
    // int nY = 5;
    //int nX = 32;
    //int nY = 32;

    interval< double > intvlX( translate_x - scale_x * MAP_SIDE,
			       translate_x + scale_x * MAP_SIDE );
    interval< double > intvlY( translate_y - scale_y * MAP_SIDE,
			       translate_y + scale_y * MAP_SIDE );


    
    radius.clear();
    radius.resize( nX, nY );

    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << endl;
    cerr << HERE << " map_scale = " << map_scale
	 << " scale_x = " << scale_x
	 << " scale_y = " << scale_y << endl;
    cerr << HERE << " intvlX = " << intvlX
	 << " size = " << intvlX.upper() - intvlX.lower() << endl;
    cerr << HERE << " intvlY = " << intvlY
	 << " size = " << intvlY.upper() - intvlY.lower() << endl;
    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    
    
    const int nNeighbors = 3;
    const double unitR =
	RADIUS_SCALE * std::min( width( intvlX ), width( intvlY ) ) / std::max( nX, nY );
    double x, y, u, v;
    list< D_Vertex_handle > LV;

    double minSdv = INFINITY, maxSdv = -INFINITY;

    cerr << HERE << " nX = " << nX << " nY = " << nY << endl;
    for ( int i = 0; i < nX; ++i ) {
	u = ( ( double )i+0.5 )/nX;
	x = ( 1.0 - u ) * intvlX.lower() + u * intvlX.upper();
	for ( int j = 0; j < nY; ++j ) {
	    v = ( ( double )j+0.5 )/nY;
	    y = ( 1.0 - v ) * intvlY.lower() + v * intvlY.upper();

	    double var = 0.0;

	    // var = estimateRadius( H, x, y );
	    var = uniformRadius( H, x, y );

	    // This seems to be correct
	    radius( i, j ) =
		unitR * ( SMALLEST_RADIUS * ( 1.0 - var ) + LARGEST_RADIUS * var ) ;
#ifdef SKIP
	    if ( ( i % 32 == 0 ) && ( j % 32 == 0 ) ) {
		cerr << " i = " << setw( 3 ) << i
		     << " j = " << setw( 3 ) << j
		     << " average var = " << var << " R = " << radius( i, j ) << endl;
	    }
	    // getchar();
#endif	// SKIP
	    if ( ! ( 0.0 <= var ) && ( var <= 1.0 ) ) {
		cerr << HERE << " var = " << var << endl;
		assert( ( 0.0 <= var ) && ( var <= 1.0 ) );
	    }
	}
    }

#ifdef TEST
    cerr << HERE << " intvlX = " << intvlX << endl;
    cerr << HERE << " intvlY = " << intvlY << endl;
    cerr << HERE << " intvlX.lower() = " << intvlX.lower() << endl;
    cerr << HERE << " intvlX.upper() = " << intvlX.upper() << endl;
    cerr << HERE << " width( intvlX ) = " << width( intvlX ) << endl;
    cerr << HERE << " median( intvlX ) = " << median( intvlX ) << endl;
    cerr << HERE << " in( 0.0, intvlX ) = " << in( 0.0, intvlX ) << endl;
    cerr << HERE << " in( -10.0, intvlX ) = " << in( -10.0, intvlX ) << endl;
#endif	// TEST
    
#ifdef SKIP
    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cerr << HERE << " minSdv = " << minSdv << " maxSdv = " << maxSdv << endl;
    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
#endif	// SKIP

    prior = poissonDiskSampling( intvlX, intvlY, radius );
}


void interpolateAttrs( const std::vector< P_Point2 > & prior, Height * H,
		       ublas::matrix< double > & adaptive )
{
    const int nNeighbors	= 3;
    std::vector< double >	diskX, diskY, diskH, diskD, diskG;
    // std::vector< int >		diskLand;
    std::vector< double >	diskPriAve, diskPosAve;
    std::vector< double >	diskPriSdv, diskPosSdv;
    std::vector< double >	diskRatioF, diskRatioM, diskRatioL;
    std::vector< std::vector< double > > diskValue;
    
    // retrieve attribute values
    list< D_Vertex_handle > LV;
    for ( unsigned int i = 0; i < prior.size(); ++i ) {
	diskX.push_back( prior[ i ].x() );
	diskY.push_back( prior[ i ].y() );
	D_Point2 p( prior[ i ].x(), prior[ i ].y() );
	LV.clear();
	CGAL::nearest_neighbors( H->T(), p, nNeighbors, std::back_inserter( LV ) );

	double vH = 0.0, vD = 0.0, vG = 0.0;
	double vPriAve = 0.0, vPosAve = 0.0, vPriSdv = 0.0, vPosSdv = 0.0;
	double vRatioF = 0.0, vRatioM = 0.0, vRatioL = 0.0;
	std::vector< double > vValue( H->nValues() );
	for ( unsigned int k = 0; k < vValue.size(); ++k )
	    vValue[ k ] = 0.0;
#ifdef SKIP
	cerr << HERE << " VID : ( "
	     << fixed << setprecision( 6 ) << p.x() << " , "
	     << fixed << setprecision( 6 ) << p.y() << " ) ";
#endif	// SKIP

	for ( list< D_Vertex_handle >::iterator it = LV.begin(); it != LV.end(); it++ ) {
	    unsigned int curID = (*it)->id(); 
#ifdef SKIP
	    cerr << setw( 8 ) << curID; 
#endif	// SKIP
	    vH += H->attrH()[ curID ];
	    vD += H->attrD()[ curID ];
	    vG += H->attrG()[ curID ];
	    vPriAve += H->attrPriAve()[ curID ];
	    vPosAve += H->attrPosAve()[ curID ];
	    vPriSdv += H->attrPriSdv()[ curID ];
	    vPosSdv += H->attrPosSdv()[ curID ];
	    vRatioF += H->attrRatioF()[ curID ];
	    vRatioM += H->attrRatioM()[ curID ];
	    vRatioL += H->attrRatioL()[ curID ];
	    for ( unsigned int k = 0; k < H->attrValue()[ curID ].size(); ++k )
		vValue[ k ] += H->attrValue()[ curID ][ k ];
	}
#ifdef SKIP
	cerr << endl;
#endif	// SKIP

	vH /= ( double )nNeighbors;
	vD /= ( double )nNeighbors;
	vG /= ( double )nNeighbors;
	vPriAve /= ( double )nNeighbors;
	vPosAve /= ( double )nNeighbors;
	vPriSdv /= ( double )nNeighbors;
	vPosSdv /= ( double )nNeighbors;
	vRatioF /= ( double )nNeighbors;
	vRatioM /= ( double )nNeighbors;
	vRatioL /= ( double )nNeighbors;
	for ( unsigned int k = 0; k < vValue.size(); ++k )
	    vValue[ k ] /= ( double )nNeighbors;

	// land use
	// list< D_Vertex_handle >::iterator itFirst = LV.begin();
	// int vLand = H->attrLand()[ (*itFirst)->id() ];

	diskH.push_back( vH );
	diskD.push_back( vD );
	// diskLand.push_back( vLand );
	diskG.push_back( vG );
	diskPriAve.push_back( vPriAve );
	diskPosAve.push_back( vPosAve );
	diskPriSdv.push_back( vPriSdv );
	diskPosSdv.push_back( vPosSdv );
	diskRatioF.push_back( vRatioF );
	diskRatioM.push_back( vRatioM );
	diskRatioL.push_back( vRatioL );
	diskValue.push_back( vValue );
    }

    cerr << HERE << " Entering tessellateDomain " << endl;
#ifdef SKIP
    cerr << HERE << " diskX.size() = " << diskX.size()
	 << " diskY.size() = " << diskY.size()
	 << " diskH.size() = " << diskH.size() << endl;
#endif	// SKIP

#ifdef REPLACEMENT
    // x, y, H, D, Land, priAve, posAve, priSdv, posSdv, ratio
    // refer to _loadAttrs in Height 
    adaptive.resize( prior.size(), FIXED_ATTR_SIZE + VARIABLE_ATTR_SIZE );
    for ( unsigned int k = 0; k < prior.size(); ++k ) {
	adaptive( k,  0 ) = diskX[ k ];
	adaptive( k,  1 ) = diskY[ k ];
	adaptive( k,  2 ) = diskH[ k ];
	adaptive( k,  3 ) = diskD[ k ];
	// adaptive( k,  4 ) = diskLand[ k ];
	adaptive( k,  4 ) = diskG[ k ];
	adaptive( k,  5 ) = diskPriSdv[ k ];
	adaptive( k,  6 ) = diskPosSdv[ k ];
	adaptive( k,  7 ) = diskPriAve[ k ];
	adaptive( k,  8 ) = diskPosAve[ k ];
	adaptive( k,  9 ) = diskRatioF[ k ];
	adaptive( k, 10 ) = diskRatioM[ k ];
	adaptive( k, 11 ) = diskRatioL[ k ];
    }
#else	// REPLACEMENT
    adaptive.resize( prior.size(), H->nValues() );
    for ( unsigned int k = 0; k < prior.size(); ++k ) {
	for ( unsigned int i = 0; i < H->nValues(); ++i )
	    adaptive( k, i ) = diskValue[ k ][ i ];
    }
#endif	// REPLACEMENT
}



void polygonizeDomain( Height * A )
{
    Polyhedron & curMesh = meshMap;
    curMesh.clear();
    ptrFacet.clear();

    tessellateDomain( curMesh,
		      A->coordX(), A->coordY(), A->attrH(), A->triID(),
		      ptrFacet );
    cerr << HERE << " number of triangle sets = " << A->triID().size() << endl;
    cerr << HERE << " ptrFacet.size() = " << ptrFacet.size() << endl;

    assignAttr	( curMesh, A->attrH(), A->attrD(),
		  // A->attrLand(),
		  A->attrG(),
		  A->attrPriAve(), A->attrPosAve(),
		  A->attrPriSdv(), A->attrPosSdv(),
		  A->attrRatioF(), A->attrRatioM(), A->attrRatioL(),
		  A->attrValue(),
		  A->attrP(), A->attrQ(), A->attrR() );

#ifdef SKIP
#ifdef WITH_STANDARD_DEVIATION
    assignFunc	( curMesh, A->attrPosAve(), A->attrH(), A->attrD() );
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
    assignFunc	( curMesh, A->attrPosAve(), A->attrH(), A->attrD() );
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
    assignFunc	( curMesh, A->attrPosAve(), A->attrRatioM(), A->attrD() );
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
    assignFunc	( curMesh, A->attrRatioF(), A->attrRatioL(), A->attrPosAve() );
#endif	// WITH_REDUCTION_RATIO
#endif	// SKIP

    region.clear();

    mapP = funcP;
    mapQ = funcQ;
    mapR = funcR;

    // Multivariate function values on the samples are assumed to be normalized
    // already
#ifdef ENABLE_MAP_COORD_NORMALIZATION
    boundPQ =	P_Bbox2( -1.0, -1.0,  1.0,  1.0 );
    boundPR =	P_Bbox2( -1.0, -1.0,  1.0,  1.0 );
    boundQR =	P_Bbox2( -1.0, -1.0,  1.0,  1.0 );
    boundPQR =	P_Bbox3( -1.0, -1.0, -1.0,  1.0,  1.0,  1.0 );
#else	// ENABLE_MAP_COORD_NORMALIZATION
    boundPQ =	P_Bbox2(  0.0,  0.0,  1.0,  1.0 );
    boundPR =	P_Bbox2(  0.0,  0.0,  1.0,  1.0 );
    boundQR =	P_Bbox2(  0.0,  0.0,  1.0,  1.0 );
    boundPQR =	P_Bbox3(  0.0,  0.0,  0.0,  1.0,  1.0,  1.0 );
#endif	// ENABLE_MAP_COORD_NORMALIZATION

    initVars();

    cerr << HERE << " boundPQ = " << boundPQ << endl;
    cerr << HERE << " boundPR = " << boundPR << endl;
    cerr << HERE << " boundQR = " << boundQR << endl;
    cerr << HERE << " boundPQR = " << boundPQR << endl;

    // Check here!!
    for ( unsigned int i = 0; i < NUM_VALUES; ++i )
	isProjected[ i ] = false;
    // isProjected[ i ] = true;

    //#define AUTOMATIC_PAINTING_FOR_NOGAMI
    // #ifdef AUTOMATIC_PAINTING_FOR_NOGAMI
    if ( place.compare( "Nogami" ) == 0 ) {
	// Intentionally flag on the vertex around the south of NR No. 228
	//const double cy =  37.402757;
	//const double cx = 140.899891;
	//const double cy =  37.397437;
	//const double cx = 140.902214;
	// const double cy = 37.393166175236544;
	// const double cx = 140.9074063810512;
	const double cy = 37.394;
	const double cx = 140.905;
	const double radius = 0.026;
	for ( unsigned int i = 0; i < ptrFacet.size(); ++i ) {
	    P_Facet_handle fh = ptrFacet[ i ];
	    P_Halfedge_handle hh = fh->halfedge();
	    P_Point3 p = hh->prev()->vertex()->point();
	    P_Point3 q = hh->vertex()->point();
	    P_Point3 r = hh->next()->vertex()->point();
	    D_Point2 P( p.x(), p.y() );
	    D_Point2 Q( q.x(), q.y() );
	    D_Point2 R( r.x(), r.y() );
	    double aveX = ( p.x() + q.x() + r.x() )/3.0;
	    double aveY = ( p.y() + q.y() + r.y() )/3.0;
	    if ( SQUARE( aveX - cx ) + SQUARE( 3.0*(aveY - cy)/2.0 ) < SQUARE( radius ) ) {
		// cerr << "O";
		ptrFacet[ i ]->setFlag();
	    }
	    else {
		// cerr << "X";
		ptrFacet[ i ]->clearFlag();
	    }
	}
    }
    cerr << endl;
    // #endif	// AUTOMATIC_PAINTING_FOR_NOGAMI

// #define AUTOMATIC_PAINTING_FOR_DATE
// #ifdef AUTOMATIC_PAINTING_FOR_DATE
    if ( place.compare( "Date" ) == 0 ) {
	const double cy = 37.794;
	const double cx = 140.488;
	// const double radius = 0.100;
	const double radius = 0.110;
	for ( unsigned int i = 0; i < ptrFacet.size(); ++i ) {
	    P_Facet_handle fh = ptrFacet[ i ];
	    P_Halfedge_handle hh = fh->halfedge();
	    P_Point3 p = hh->prev()->vertex()->point();
	    P_Point3 q = hh->vertex()->point();
	    P_Point3 r = hh->next()->vertex()->point();
	    D_Point2 P( p.x(), p.y() );
	    D_Point2 Q( q.x(), q.y() );
	    D_Point2 R( r.x(), r.y() );
	    double aveX = ( p.x() + q.x() + r.x() )/3.0;
	    double aveY = ( p.y() + q.y() + r.y() )/3.0;
	    if ( SQUARE( aveX - cx ) + SQUARE( 3.0*(aveY - cy)/2.0 ) < SQUARE( radius ) ) {
		// cerr << "O";
		ptrFacet[ i ]->setFlag();
	    }
	    else {
		// cerr << "X";
		ptrFacet[ i ]->clearFlag();
	    }
	}
    }
    cerr << endl;
    // #endif	// AUTOMATIC_PAINTING_FOR_NOGAMI
}


void initLandUse( Height * A )
{
//------------------------------------------------------------------------------
//	find the interpolated land use
//------------------------------------------------------------------------------
    if ( A->triID().size() != ptrFacet.size() ) {
	cerr << HERE << " A->triID().size() = " << A->triID().size() << endl;
	cerr << HERE << " ptrFacet.size() = " << ptrFacet.size() << endl;
	assert( A->triID().size() == ptrFacet.size() );
    }

    for ( unsigned int k = 0; k < A->triID().size(); ++k ) {
	if ( k % 10000 == 0 )
	    cerr << HERE << " Handling " << setw( 6 ) << k << "-th triangle" << endl;

	assert( A->triID()[ k ].size() == 3 );
	ptrFacet[ k ]->setLabel( 500 ); // forest by default
    }
}


void writePlotCSV( Height * H,
		   const char *name,
		   const std::vector< unsigned int > & poolFID )
{
    const int nNeighbors = 500;
    ofstream ofs( name );
    if ( ! ofs.is_open() ) {
	cerr << HERE << " cannot open the file : " << name << endl;
    }
    for ( unsigned int k = 0; k < title.size(); ++k ) {
	ofs << title[ k ] << ",";
    }
    ofs << "Water,Urban,Rice field,Farmland,Meadow," 
	<< "Deciduous broadleaf,Deciduous coniferous,"
	<< "Evergreen boardleaf,Evergree coniferous,"
	<< "Bare land,Banboo forest,Solar Panel,";
    ofs << "cv1,cv2,mean,gauss,ridgeDeg,ravineDeg,peak,pit,saddle,ridge,ravine" 
	<< endl;

    cerr << HERE << " Saving the csv file: ";
    ofs << setprecision( 8 );
    for ( unsigned int i = 0; i < poolFID.size(); ++i ) {
	if ( i % 100 == 0 ) cerr << ">";

	unsigned int idF = poolFID[ i ];
	P_Facet_handle fh = ptrFacet[ idF ];
	P_Halfedge_handle hh = fh->halfedge();
	P_Point3 p = hh->prev()->vertex()->point();
	P_Point3 q = hh->vertex()->point();
	P_Point3 r = hh->next()->vertex()->point();
	D_Point2 P( p.x(), p.y() );
	D_Point2 Q( q.x(), q.y() );
	D_Point2 R( r.x(), r.y() );
	D_Triangle2 triangle( P, Q, R );

	double aveX = ( p.x() + q.x() + r.x() )/3.0;
	double aveY = ( p.y() + q.y() + r.y() )/3.0;
	D_Point2 S( aveX, aveY );

	// retrieve attribute values
	list< D_Vertex_handle > LV;
	CGAL::nearest_neighbors( H->T(), S, nNeighbors, std::back_inserter( LV ) );
	
	double vX, vY;
	double rH, vH, rD, vD, rG, vG;
	double rPriAve, rPosAve, rPriSdv, rPosSdv;
	double vPriAve, vPosAve, vPriSdv, vPosSdv;
	double rRatioF, rRatioM, rRatioL;
	double vRatioF, vRatioM, vRatioL;
	int vLand;

	int counter = 0;
	
	// cerr << HERE << " LV.size = " << LV.size() << endl;
	for ( list< D_Vertex_handle >::iterator it = LV.begin(); it != LV.end(); it++ ) {
	    unsigned int curID = (*it)->id(); 

	    D_Point2 plot = (*it)->point();

	    if ( triangle.has_on_bounded_side( plot ) ) {
		counter++;

		for ( unsigned int k = 0; k < H->attrValue()[ curID ].size(); ++k ) {
		    ofs << H->attrValue()[ curID ][ k ] << ",";
		}

		vX = H->coordX()[ curID ];
		vY = H->coordY()[ curID ];

	//------------------------------------------------------------------------------
	//	Plotting land usage attributes
	//------------------------------------------------------------------------------

		int vLand = landMap.landID( vY, vX );
		
		for ( unsigned int k = 0; k < NUM_CATEGORIES; ++k ) {
		    if ( k == vLand ) ofs << "1,";
		    else ofs << "0,";
		}

#ifdef ENABLE_USE_BIT
		// Urban
		// if ( vLand == 1 ) fh->setUse();
		// Deciduous broadleaf
		// if ( vLand == 5 ) fh->setUse();
#endif	// ENABLE_USE_BIT
		
	//------------------------------------------------------------------------------
	//	Computing curvature and drainage network
	//------------------------------------------------------------------------------
		double k1, k2;
		double ridgeDeg, ravineDeg;
		std::vector< double > e1, e2;
		H->eigen( vX, vY, k1, k2, e1, e2 );
		if ( k1 < k2 ) {
		    cerr << HERE << " Unwanted results for principal curvatures" << endl;
		    assert( k1 >= k2 );
		}
		int bump = H->drainage( vX, vY, k1, k2, e1, e2, ridgeDeg, ravineDeg );
		ofs << fixed << setprecision( 6 ) << k1 << ",";
		ofs << fixed << setprecision( 6 ) << k2 << ",";
		ofs << fixed << setprecision( 6 ) << 0.5*(k1+k2) << ",";
		ofs << fixed << setprecision( 6 ) << k1*k2 << ",";
		ofs << fixed << setprecision( 6 ) << ridgeDeg << ",";
		ofs << fixed << setprecision( 6 ) << ravineDeg << ",";
		if ( ( k1 < 0 ) && ( k2 < 0 ) ) {
		    ofs << "1,0,0,";
#ifdef ENABLE_USE_BIT
		    // Peak
		    // fh->setUse();
#endif	// ENABLE_USE_BIT
		}
		else if ( ( k1 > 0 ) && ( k2 > 0 ) ) {
		    ofs << "0,1,0,";
		}
		else {
		    ofs << "0,0,1,";
#ifdef ENABLE_USE_BIT
		    // Pit
		    // fh->setUse();
#endif	// ENABLE_USE_BIT
		}
		switch ( bump ) {
		  case 1: // ridge
		      ofs << "1,0";
		      fh->setRidge();
		      fh->clearRavine();
		      break;
		  case 2: // ravine
		      ofs << "0,1";
		      fh->clearRidge();
		      fh->setRavine();
		      break;
		  case 0: // none
		      fh->clearRidge();
		      fh->clearRavine();
		      ofs << "0,0";
		      break;
		  default:
		      cerr << HERE << " Illegal case:  bump = " << bump << endl;
		      assert( false );
		      break;
		}
		ofs << endl;
	    }
	}
	double hitRatio = ( double )counter/( double )nNeighbors;
	if ( hitRatio > 0.5 ) {
	    cerr << HERE << " Hit ratio : " << 100.0 * hitRatio << " % " << endl;
	}
    }
    cerr << " Done." << endl;
    ofs.close();
}		   


void matchSamples( Height * H,
		   std::vector< P_Facet_handle > & ptrFacet )
{
    const int nNeighbors	= 100;
    std::vector< double >	diskX, diskY, diskH, diskD;
    std::vector< int >		diskLand;
    std::vector< double >	diskPriAve, diskPosAve;
    std::vector< double >	diskPriSdv, diskPosSdv;
    std::vector< double >	diskRatioF, diskRatioM, diskRatioL;
    
//------------------------------------------------------------------------------
//	Domain samples
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolD;
    for ( unsigned int i = 0; i < ptrFacet.size(); ++i ) {
	if ( ptrFacet[ i ]->flag() ) poolD.push_back( i );
    }

    writePlotCSV( H, "domain.csv", poolD );

//------------------------------------------------------------------------------
//	Collect PQ face IDs;
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolPQ;
    for ( unsigned int i = 0; i < pixelPQ.size(); ++i ) {
	unsigned int idG = pixelPQ[ i ];
	for ( unsigned int k = 0; k < curLevel->facePQ()[ idG ].size(); ++k ) {
	    P_Facet_handle fh = curLevel->facePQ()[ idG ][ k ];
	    unsigned int idF = fh->id();
	    if ( ! binary_search( poolPQ.begin(), poolPQ.end(), idF ) ) {
		std::vector< unsigned int >::iterator itF =
		    upper_bound( poolPQ.begin(), poolPQ.end(), idF );
		poolPQ.insert( itF, idF );
	    }
	}
    }
//------------------------------------------------------------------------------
//	Collect PR face IDs;
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolPR;
    for ( unsigned int i = 0; i < pixelPR.size(); ++i ) {
	unsigned int idG = pixelPR[ i ];
	for ( unsigned int k = 0; k < curLevel->facePR()[ idG ].size(); ++k ) {
	    P_Facet_handle fh = curLevel->facePR()[ idG ][ k ];
	    unsigned int idF = fh->id();
	    if ( ! binary_search( poolPR.begin(), poolPR.end(), idF ) ) {
		std::vector< unsigned int >::iterator itF =
		    upper_bound( poolPR.begin(), poolPR.end(), idF );
		poolPR.insert( itF, idF );
	    }
	}
    }
//------------------------------------------------------------------------------
//	Collect QR face IDs;
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolQR;
    for ( unsigned int i = 0; i < pixelQR.size(); ++i ) {
	unsigned int idG = pixelQR[ i ];
	for ( unsigned int k = 0; k < curLevel->faceQR()[ idG ].size(); ++k ) {
	    P_Facet_handle fh = curLevel->faceQR()[ idG ][ k ];
	    unsigned int idF = fh->id();
	    if ( ! binary_search( poolQR.begin(), poolQR.end(), idF ) ) {
		std::vector< unsigned int >::iterator itF =
		    upper_bound( poolQR.begin(), poolQR.end(), idF );
		poolQR.insert( itF, idF );
	    }
	}
    }
//------------------------------------------------------------------------------
//	Intersection of PQ and PR
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolPQandPR;
    std::set_intersection( poolPQ.begin(), poolPQ.end(),
			   poolPR.begin(), poolPR.end(),
			   std::inserter( poolPQandPR, poolPQandPR.end() ) );
//------------------------------------------------------------------------------
//	Intersection of PR and QR
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolPRandQR;
    std::set_intersection( poolPR.begin(), poolPR.end(),
			   poolQR.begin(), poolQR.end(),
			   std::inserter( poolPRandQR, poolPRandQR.end() ) );
//------------------------------------------------------------------------------
//	Intersection of PQ and QR
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolPQandQR;
    std::set_intersection( poolPQ.begin(), poolPQ.end(),
			   poolQR.begin(), poolQR.end(),
			   std::inserter( poolPQandQR, poolPQandQR.end() ) );

//------------------------------------------------------------------------------
//	Intersection of PQ, QR, PR
//------------------------------------------------------------------------------
    std::vector< unsigned int > poolAll;
    std::set_intersection( poolPQandQR.begin(), poolPQandQR.end(),
			   poolPR.begin(), poolPR.end(),
			   std::inserter( poolAll, poolAll.end() ) );

//------------------------------------------------------------------------------
//	Plot samples from Pool
//------------------------------------------------------------------------------
    writePlotCSV( H, "plotPQ.csv", poolPQ );
    writePlotCSV( H, "plotPR.csv", poolPR );
    writePlotCSV( H, "plotQR.csv", poolQR );

    writePlotCSV( H, "plotPQandPR.csv", poolPQandPR );
    writePlotCSV( H, "plotPRandQR.csv", poolPRandQR );
    writePlotCSV( H, "plotPQandQR.csv", poolPQandQR );
    
    writePlotCSV( H, "plotAll.csv", poolAll );

    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    cerr << "\a\a\a";
    cerr << HERE << " saving csv files in matchSamples ... Done!!" << endl;
    cerr << HERE << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
}


void preprocess( void )
{
    srand48( time( (time_t*)NULL ) );

    cerr << HERE << "##############################" << endl;
    cerr << HERE << "##############################" << endl;
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << endl;
    cerr << HERE << " map_scale = " << map_scale
	 << " scale_x = " << scale_x
	 << " scale_y = " << scale_y << endl;
    cerr << HERE << "##############################" << endl;
    cerr << HERE << "##############################" << endl;

    if ( ! is_processed ) {
	// #ifdef SKIP_PREPROCESS_MODULE
	resolution = new Sampling * [ NUM_VALUES ];
	for ( unsigned int i = 0; i < NUM_VALUES; ++i )
	    resolution[ i ] = new Sampling( cf );

	// Read the binary data file if it exists
	if ( exists( bin_file_name.c_str() ) &&
	     ( ! is_directory( bin_file_name.c_str() ) ) )  {
	    cerr << HERE << " Loading the entire data ... from "
		 << bin_file_name << endl;
	    loadBinaryData( bin_file_name.c_str(), title, entire );
	    cerr << " ... finished" << endl;
	}
	// Othewise, read the ascii csv data file instead.
	else {
	    cerr << HERE << " Loading the entire data ... from "
		 << csv_file_name.c_str() << endl;

	    // We expect the data is formatted as
	    // Latitude, Longitude, Height, Exponent/Distance, Dose[ 0 ], Dose[ 1 ], .....
	    loadExpData( csv_file_name.c_str(), title, entire );
	    cerr << " ... finished" << endl;
	    
	    cerr << HERE << " Set the number of attribute values in Height" << endl;
	    
	    cerr << HERE << " Saving the entire data ... into "
		 << bin_file_name.c_str() << endl;

	    // Then save the binary file for next analysis
	    saveBinaryData( bin_file_name.c_str(), title, entire );
	    cerr << " ... finished" << endl;
	}
	    
	cerr << HERE << " number of columns = " << title.size() << endl;
	for ( unsigned int k = 0; k < title.size(); ++k )
	    cerr << HERE << "[ " << setw( 2 ) << k << " ] : "
		 << title[ k ] << endl;
	
	cerr << HERE << " entire[ 0 ] = " << ends;
	for ( unsigned int k = 0; k < entire.size2(); ++k ) {
	    cerr << entire( 0, k ) << " , ";
	}
	cerr << endl;
	 
	cerr << HERE << "******************************" << endl;
	cerr << HERE << "******************************" << endl;
	cerr << HERE << " translate_x = " << translate_x
	     << " translate_y = " << translate_y << endl;
	cerr << HERE << " map_scale = " << map_scale
	     << " scale_x = " << scale_x
	     << " scale_y = " << scale_y << endl;
	cerr << HERE << "******************************" << endl;
	cerr << HERE << "******************************" << endl;
	// #endif	// SKIP_PREPROCESS_MODULE

//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
	if ( ! flag_data ) {
	    // #ifdef SKIP_PREPROCESS_MODULE
	    cerr << " Inawashiro Lake = "
		 << " ( 37.484367848720176, 140.09182698116246 ) : "
		 << landMap.landtype( 37.484367848720176, 140.09182698116246 ) << endl;	    
	    cerr << " Aizu Wakamatsu Sta = "
		 << " ( 37.50811358584273, 139.93038502466393 ) : "
		 << landMap.landtype( 37.50811358584273, 139.93038502466393 ) << endl;	    
	    cerr << " Inawashiro  Sta = "
		 << " ( 37.546506614021, 140.1031446409329 ) : "
		 << landMap.landtype( 37.546506614021, 140.1031446409329 ) << endl;	    
	    cerr << " Field = "
		 << " ( 37.51576817599858, 139.9790967040568 ) : "
		 << landMap.landtype( 37.51576817599858, 139.9790967040568 ) << endl;
	    cerr << " Forest = "
		 << " ( 37.51357345022376, 139.9749274789994 ) : "
		 << landMap.landtype( 37.51357345022376, 139.9749274789994 ) << endl;	    
	    cerr << " Urban = "
		 << " ( 37.42122250932595, 140.36188806080708 ) : "
		 << landMap.landtype( 37.42122250932595, 140.36188806080708 ) << endl;	    
	    cerr << " Water = "
		 << " ( 37.656421437214256, 140.12635783310645 ) : "
		 << landMap.landtype( 37.656421437214256, 140.12635783310645 ) << endl;	    
	    cerr << " Solar = "
		 << " ( 37.888599090806586, 139.9711184128596 ) : "
		 << landMap.landtype( 37.888599090806586, 139.9711184128596 ) << endl;	    
	    cerr << " Solar2 = "
		 << " ( 37.9429223182315, 140.10096342909648 ) : "
		 << landMap.landtype( 37.9429223182315, 140.10096342909648 ) << endl;	    
	    // 0.111328 , 0.0605469
	    cerr << " Solar3 = "
		 << " ( 37.9394531, 139.111328 ) : "
		 << landMap.landtype( 37.9394531, 139.111328 ) << endl;	    
	    cerr << " Hatena = "
		 << " ( 36.93089995654071, 140.37921245302974 ) : "
		 << landMap.landtype( 36.93089995654071, 140.37921245302974 ) << endl;	    
	    cerr << " River = "
		 << " ( 37.90238124504546, 139.15182812508164 ) : "
		 << landMap.landtype( 37.90238124504546, 139.15182812508164 ) << endl;	    

    
//------------------------------------------------------------------------------
//	Japan Map
//------------------------------------------------------------------------------
	    
#ifdef SKIP
	    iwate->district( "map/Iwate_coastline.shp" );		// District 03
	    iwate->district( "map/Iwate_border.shp" );			// District 03
#endif	// SKIP

	    miyagi->district( "map/Miyagi_coastline.shp" );		// District 04
	    miyagi->district( "map/Miyagi_border.shp" );		// District 04
	    
#ifdef SKIP
	    yamagata->district( "map/Yamagata_coastline.shp" );		// District 06
	    yamagata->district( "map/Yamagata_border.shp" );		// District 06
#endif	// SKIP

	    fukushima->district( "map/Fukushima_coastline.shp" );	// District 07
	    fukushima->district( "map/Fukushima_border.shp" );		// District 07

	    ibaraki->district( "map/Ibaraki_coastline.shp" );		// District 08
	    ibaraki->district( "map/Ibaraki_border.shp" );		// District 08
	    
	    tochigi->district( "map/Tochigi_border.shp" );		// District 09
	    
	    gunma->district( "map/Gunma_border.shp" );			// District 10
	    
	    flag_data = true;

	    // fukushima->flow( "map/niida-river.dat" );		// Districte 07
	    // Fukushima rivers
	    fukushima->stream( "map/W05-07_07-g_Stream.shp" );
	    
	    // Fukushima roads
	    fukushima->network( "map/N01_07L_07_Road.shp" );
	    // #endif	// SKIP_PREPROCESS_MODULE
	}

//------------------------------------------------------------------------------
//	Resampling
//------------------------------------------------------------------------------
	cerr << HERE << " Current factor for sparse sampling = " << current_factor << endl;
	// getchar();
	
	// #ifdef USING_POISSON_SAMPLING
	if ( tessellation.compare( "Poisson" ) == 0 ) {
	    sparseSampling( entire, region, current_factor );
	}
	// #endif
	// #ifdef USING_DELAUNAY_SAMPLING	
	if ( tessellation.compare( "Delaunay" ) == 0 ) {
	    if ( density.compare( "sparse" ) == 0 ) 
		sparseSampling( entire, region, 8 );
	    if ( density.compare( "dense" ) == 0 )
		sparseSampling( entire, region, 1 );
	}
	// #endif
	
#ifdef SKIP
	cerr << HERE << " region[ 0 ] = " << ends;
	for ( unsigned int k = 0; k < region.size2(); ++k ) {
	    cerr << region( 0, k ) << " , ";
	}
	cerr << endl;
#endif	// SKIP

//------------------------------------------------------------------------------
//	Triangulation
//------------------------------------------------------------------------------

// #ifdef SKIP_PREPROCESS_MODULE
	fundField = new Height();
	fundField->setNValues( region.size2() );
	fundField->setIDP( idP );
	fundField->setIDQ( idQ );
	fundField->setIDR( idR );
	autoTriangulateField( region, 
			      rangeH, rangeD, rangeG,
			      rangePriAve, rangePosAve, rangePriSdv, rangePosSdv,
			      rangeRatioF, rangeRatioM, rangeRatioL,
			      rangeP, rangeQ, rangeR,
			      fundField );

	
	cerr << HERE << " fundField->attrValue()[ 0 ] = " << ends;
	for ( unsigned int k = 0; k < fundField->attrValue()[ 0 ].size(); ++k ) {
	    cerr << fundField->attrValue()[ 0 ][ k ] << " , ";
	}
	cerr << endl;

	cerr << HERE << " rangeP = " << rangeP << endl;
	cerr << HERE << " rangeQ = " << rangeQ << endl;
#ifdef ENABLE_R_VALUE_COLORING
	// -16.4204,0.967329
	// rangeQ = interval< double >LIMIT_R_INTERVAL;
	// rangeQ = interval< double >( 0.95, 1.00 ); // clear color gradation
	//rangeQ = interval< double >( 1.10, 1.50 );
	// rangeQ = interval< double >( 0.92, 1.00 );
#endif	// ENABLE_R_VALUE_COLORING
	cerr << HERE << " ====> changed into rangeQ = " << rangeQ << endl;
	cerr << HERE << " rangeR = " << rangeR << endl;
	cerr << HERE << " This is the checkpoint => Press enter" << endl;
	cerr << "\a\a\a\a\a";
#ifdef ENABLE_R_VALUE_COLORING
	// getchar();
#endif	// ENABLE_R_VALUE_COLORING
	// #endif	// SKIP_PREPROCESS_MODULE

//------------------------------------------------------------------------------
//	DEBUG FOR RIDGE AND RAVINE
//------------------------------------------------------------------------------
#ifdef DEBUG_FOR_RIDGE_AND_RAVINE
	double x, y;
	double k1, k2;
	std::vector< double > e1, e2;
	int check;
	
	x = 140.5;
	y = 37.5;
	fundField->eigen( x, y, k1, k2, e1, e2 );
	cerr << HERE << "==============================" << endl;
	cerr << HERE << " x = " << x << " y = " << y << endl;
	cerr << HERE << " k1 = " << k1 << " k2 = " << k2 << endl;
	cerr << HERE << " e1 = " << e1[ 0 ] << " , " << e1[ 1 ] << endl;
	cerr << HERE << " e2 = " << e2[ 0 ] << " , " << e2[ 1 ] << endl;
	check = fundField->drainage( x, y, k1, k2, e1, e2 );
	cerr << HERE << " check = " << check << endl;
	getchar();
	
	x = 140.6;
	y = 37.5;
	fundField->eigen( x, y, k1, k2, e1, e2 );
	cerr << HERE << "==============================" << endl;
	cerr << HERE << " x = " << x << " y = " << y << endl;
	cerr << HERE << " k1 = " << k1 << " k2 = " << k2 << endl;
	cerr << HERE << " e1 = " << e1[ 0 ] << " , " << e1[ 1 ] << endl;
	cerr << HERE << " e2 = " << e2[ 0 ] << " , " << e2[ 1 ] << endl;
	check = fundField->drainage( x, y, k1, k2, e1, e2 );
	cerr << HERE << " check = " << check << endl;
	getchar();
	
	x = 140.5;
	y = 37.6;
	fundField->eigen( x, y, k1, k2, e1, e2 );
	cerr << HERE << "==============================" << endl;
	cerr << HERE << " x = " << x << " y = " << y << endl;
	cerr << HERE << " k1 = " << k1 << " k2 = " << k2 << endl;
	cerr << HERE << " e1 = " << e1[ 0 ] << " , " << e1[ 1 ] << endl;
	cerr << HERE << " e2 = " << e2[ 0 ] << " , " << e2[ 1 ] << endl;
	check = fundField->drainage( x, y, k1, k2, e1, e2 );
	cerr << HERE << " check = " << check << endl;
	getchar();
#endif	// DEBUG_FOR_RIDGE_AND_RAVINE
//------------------------------------------------------------------------------

	
#ifdef AUTOMATIC_NORMALIZATION

#ifdef SKIP
#ifdef WITH_STANDARD_DEVIATION
	rangeP = rangePosAve;
	rangeQ = rangeH;
	rangeR = rangeD;
#endif	// WITH_STANDARD_DEVIATION
#ifdef WITH_LAND_TYPE
	rangeP = rangePosAve;
	rangeQ = rangeH;
	rangeR = rangeD;
#endif	// WITH_LAND_TYPE
#ifdef WITH_REDUCTION_RATIO
	rangeP = rangePosAve;
	rangeQ = rangeRatioM;
	rangeR = rangeD;
#endif	// WITH_REDUCTION_RATIO
#ifdef WITH_TWO_RATIOS
	rangeP = rangePosAve;
	rangeQ = rangeRatioM;
	rangeR = rangeD;
#endif	// WITH_TWO_RATIOS
	rangeP = rangeRatioF;
	rangeQ = rangeRatioL;
	rangeR = rangePosAve;
#endif	// AUTOMATIC_NORMALIZATION
#endif	// SKIP

	is_processed = true;
    }
    

#ifdef USING_POISSON_SAMPLING
//------------------------------------------------------------------------------
//	Adaptive sampling
//------------------------------------------------------------------------------
    cerr << HERE << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cerr << HERE << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << endl;
    cerr << HERE << " map_scale = " << map_scale
	 << " scale_x = " << scale_x
	 << " scale_y = " << scale_y << endl;
    cerr << HERE << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
    cerr << HERE << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;


    if ( ! is_focused ) {
	// initialization
	std::vector< P_Point2 > prior;
	cerr << HERE << "Performing the Poisson disk sampling" << endl;
	poissonDisk( fundField, prior );

	cerr << HERE << "Retriving attribute values through interpolation" << endl;
	interpolateAttrs( prior, fundField, poisson );
	adaptField = new Height();
	adaptField->setNValues( fundField->nValues() );
	adaptField->setIDP( idP );
	adaptField->setIDQ( idQ );
	adaptField->setIDR( idR );
	
	cerr << HERE << " poisson : " << poisson.size1() << " x " << poisson.size2() << endl;
	cerr << HERE << " Triangulate the field" << endl;
	
	interval< double > scopeH	= interval< double >( 0.0, 1.0 );
	interval< double > scopeD	= interval< double >( 0.0, 1.0 );
	interval< double > scopeG	= interval< double >( 0.0, 1.0 );
	interval< double > scopePriAve	= interval< double >( 0.0, 1.0 );
	interval< double > scopePosAve	= interval< double >( 0.0, 1.0 );
	interval< double > scopePriSdv	= interval< double >( 0.0, 1.0 );
	interval< double > scopePosSdv	= interval< double >( 0.0, 1.0 );
	interval< double > scopeRatioF	= interval< double >( 0.0, 1.0 );
	interval< double > scopeRatioM	= interval< double >( 0.0, 1.0 );
	interval< double > scopeRatioL	= interval< double >( 0.0, 1.0 );
	interval< double > scopeP	= interval< double >( 0.0, 1.0 );
	interval< double > scopeQ	= interval< double >( 0.0, 1.0 );
	interval< double > scopeR	= interval< double >( 0.0, 1.0 );

#ifdef SKIP
	manualTriangulateField( poisson,  
				scopeH, scopeD, scopeG,
				scopePriAve, scopePosAve, scopePriSdv, scopePosSdv,
				scopeRatioF, scopeRatioM, scopeRatioL,
				scopeP, scopeQ, scopeR,
				adaptField );
#endif	// SKIP
	autoTriangulateField( poisson,  
			      scopeH, scopeD, scopeG,
			      scopePriAve, scopePosAve, scopePriSdv, scopePosSdv,
			      scopeRatioF, scopeRatioM, scopeRatioL,
			      scopeP, scopeQ, scopeR,
			      adaptField );

	cerr << HERE << " adaptField->attrValue()[ 0 ] = " << ends;
	for ( unsigned int k = 0; k < adaptField->attrValue()[ 0 ].size(); ++k ) {
	    cerr << adaptField->attrValue()[ 0 ][ k ] << ", ";
	}
	cerr << endl;

	//cerr << HERE << " This is the checkpoint => Press enter" << endl;
	//cerr << "\a\a\a\a\a";
	//getchar();

	cerr << HERE << " Polygonize the domain" << endl;
	
	polygonizeDomain( adaptField );

	cerr << HERE << " Conducting Re-rasterization" << endl;
	perform_task( PROJECT_TRIANGLES );

	is_focused = true;
    }
#endif	// USING_POISSON_SAMPLING
#ifdef USING_DELAUNAY_SAMPLING
    polygonizeDomain( fundField );

    perform_task( PROJECT_TRIANGLES );
#endif	// USING_DELAUNAY_SAMPLING

    // cerr << HERE << endl;
    entire.clear();
    region.clear();

#ifdef USE_POISSON_SAMPLING
    // cerr << HERE << endl;
    delete	adaptField;
    adaptField = NULL;
#endif	// USE_POISSON_SAMPLING

    // cerr << HERE << endl;
    
    if ( attrR.size() > 1 )
	cerr << HERE << " attrR[ 0 ] = " << attrR[ 0 ] << endl;
    
//------------------------------------------------------------------------------
//	Shape window
//------------------------------------------------------------------------------
    Polyhedron & curMesh = meshMap;
    initAnnotation( curMesh, domain );
    // #endif	// SKIP_PREPROCESS_MODULE

    cerr << HERE << "##############################" << endl;
    cerr << HERE << "##############################" << endl;
    cerr << HERE << "##### curLevel = " << curLevel << endl;
    cerr << HERE << "##############################" << endl;
    cerr << HERE << "##############################" << endl;
    
    redisplay_all();
}


void timer( int value )
{
    // cerr << HERE << " timer : value = " << value << endl;
    if ( is_displayed_shape &&
	 is_displayed_PQ && is_displayed_PR && is_displayed_QR &&
	 is_displayed_range ) {
	cerr << HERE << "All the windows are displayed." << endl;
	if ( ( ! is_focused ) || ( ! is_processed ) ) {
	    cerr << HERE << "Start the preprocess" << endl;
	    usleep( 3000000 );
	    preprocess();
	    is_processed = true;
	    is_focused = true;
	}
	else {
	    glutTimerFunc( 50 , timer , 0 );
	}
    }
    else {
	glutTimerFunc( 50 , timer , 0 );
    }
}

//------------------------------------------------------------------------------
//	Interface
//------------------------------------------------------------------------------

void initGLUI( void )
{
    // setup the glui window
    GLUI_Panel *rootPanel;
    GLUI_RadioGroup *gridGroup;

#ifdef RIGHTMOST_PANEL
//------------------------------------------------------------------------------
#ifdef HIDE_RANGE_WINDOW
    glutInitWindowPosition( DEFAULT_WIDTH + LARGE_WIDTH, 0 );
#else	// HIDE_RANGE_WINDOW
    glutInitWindowPosition( 1920 - 192, 0 );
#endif	// HIDE_RANGE_WINDOW
//------------------------------------------------------------------------------
#else	// RIGHTMOST_PANEL
//------------------------------------------------------------------------------
    glutInitWindowPosition( 0, DEFAULT_HEIGHT * 2 + 80 );
//------------------------------------------------------------------------------
#endif	// RIGHTMOST_PANEL

    if ( glui != NULL ) glui->close();
    glui = GLUI_Master.create_glui( (char *) GLUI_TITLE );

    // glui->add_statictext( (char *) GLUI_TITLE );
    // glui->add_separator();

    rootPanel = glui->add_panel( "Panel" );

//------------------------------------------------------------------------------
//	Grid Panel
//------------------------------------------------------------------------------
#ifdef USE_ROLLOUT
    // GLUI_Rollout * gridPanel
    // = glui->add_rollout_to_panel	( rootPanel, "Resolution", false );
    GLUI_Panel * gridPanel
	= glui->add_panel_to_panel	( rootPanel, "Resolution" );
#else	// USE_ROLLOUT
    GLUI_Panel * gridPanel
	= glui->add_panel_to_panel	( rootPanel, "Resolution" );
#endif	// USE_ROLLOUT
    gridGroup
	= glui->add_radiogroup_to_panel	( gridPanel, &gridSwitch,
					  0, (GLUI_Update_CB)perform_task );
    glui->add_radiobutton_to_group	( gridGroup, "004" );
    glui->add_radiobutton_to_group	( gridGroup, "008" );
    glui->add_radiobutton_to_group	( gridGroup, "016" );
    glui->add_radiobutton_to_group	( gridGroup, "032" );
    glui->add_radiobutton_to_group	( gridGroup, "064" );
    glui->add_button_to_panel		( gridPanel, "Project",
					  PROJECT_TRIANGLES,
					  (GLUI_Update_CB)perform_task );
    

//------------------------------------------------------------------------------
//	Step Panel
//------------------------------------------------------------------------------
#ifndef RIGHTMOST_PANEL
    glui->add_column_to_panel		( rootPanel, GLUI_FALSE );
#endif	// RIGHTMOST_PANEL

#ifdef USE_ROLLOUT
    // GLUI_Rollout * stepPanel
    // = glui->add_rollout_to_panel	( rootPanel, "Value Type", false );
    GLUI_Panel * stepPanel
	= glui->add_panel_to_panel	( rootPanel, "Value Type" );
#else	// USE_ROLLOUT
    GLUI_Panel * stepPanel
	= glui->add_panel_to_panel	( rootPanel, "Value Type" );
#endif	// USE_ROLLOUT
    GLUI_RadioGroup * stepGroup
	= glui->add_radiogroup_to_panel	( stepPanel, &stepSwitch,
					  0, (GLUI_Update_CB)perform_task );
    glui->add_radiobutton_to_group	( stepGroup, "Prior" );
    glui->add_radiobutton_to_group	( stepGroup, "Posterior" );
    glui->add_radiobutton_to_group	( stepGroup, "Prior STD" );
    glui->add_radiobutton_to_group	( stepGroup, "Posterior STD" );
    
    glui->add_button_to_panel		( stepPanel, "Update",
					  VALUE_TYPE_UPDATE,
					  (GLUI_Update_CB)perform_task );
    
#ifdef SKIP_LAND_USE_CHECK_BOXES
//------------------------------------------------------------------------------
//	Land use check box
//------------------------------------------------------------------------------
#ifndef RIGHTMOST_PANEL
    glui->add_column_to_panel		( rootPanel, GLUI_FALSE );
#endif	// RIGHTMOST_PANEL

    GLUI_Panel * landPanel
	= glui->add_panel_to_panel	( rootPanel, "Land Use" );
    glui->add_checkbox_to_panel		( landPanel, "Forest",
					  &forestBit,
					  GLUI_FALSE, (GLUI_Update_CB)perform_task );
    glui->add_checkbox_to_panel		( landPanel, "Farm",
					  &farmBit,
					  GLUI_FALSE, (GLUI_Update_CB)perform_task );
    glui->add_checkbox_to_panel		( landPanel, "Urban",
					  &urbanBit,
					  GLUI_FALSE, (GLUI_Update_CB)perform_task );
    glui->add_checkbox_to_panel		( landPanel, "Ocean",
					  &oceanBit,
					  GLUI_FALSE, (GLUI_Update_CB)perform_task );

    glui->add_button_to_panel		( landPanel, "Filter",
					  LAND_FILTER, (GLUI_Update_CB)perform_task );
#endif	// SKIP_LAND_USE_CHECK_BOXES

//------------------------------------------------------------------------------
//	Style Panel
//------------------------------------------------------------------------------
    // glui->add_column_to_panel		( rootPanel, GLUI_FALSE );
    GLUI_Panel * stylePanel
	= glui->add_panel_to_panel	( rootPanel, "Drawing style" );
//------------------------------------------------------------------------------
    GLUI_Listbox * outlineList
	= glui->add_listbox_to_panel	( stylePanel, "Outline:",
					  &outlineSwitch,
					  RENDER_MAP, (GLUI_Update_CB)perform_task );
    outlineList->add_item		( 1, "ON" );
    outlineList->add_item		( 0, "OFF" );
#ifdef SKIP
    GLUI_RadioGroup * outlineGroup
	= glui->add_radiogroup_to_panel	( stylePanel, &outlineSwitch,
					  0, (GLUI_Update_CB)perform_task );
    glui->add_radiobutton_to_group	( outlineGroup, "Outline ON" );
    glui->add_radiobutton_to_group	( outlineGroup, "Outline OFF" );
#endif	// SKIP
//------------------------------------------------------------------------------
    GLUI_Listbox * fillList
	= glui->add_listbox_to_panel	( stylePanel, "Fill      :",
					  &fillSwitch,
					  RENDER_MAP, (GLUI_Update_CB)perform_task );
    fillList->add_item			( 1, "ON" );
    fillList->add_item			( 0, "OFF" );
#ifdef SKIP
    GLUI_RadioGroup * fillGroup
	= glui->add_radiogroup_to_panel	( stylePanel, &fillSwitch,
					  0, (GLUI_Update_CB)perform_task );
    glui->add_radiobutton_to_group	( fillGroup, "Fill ON" );
    glui->add_radiobutton_to_group	( fillGroup, "Fill OFF" );
#endif	// SKIP
    
#ifdef SKIP
    glui->add_button_to_panel		( stylePanel, "Render",
					  DIST_SEGMENT, (GLUI_Update_CB)perform_task );
#endif	// SKIP

//------------------------------------------------------------------------------
//	Distance limits
//------------------------------------------------------------------------------
#ifndef RIGHTMOST_PANEL
    glui->add_column_to_panel		( rootPanel, GLUI_FALSE );
#endif	// RIGHTMOST_PANEL

#ifdef USE_ROLLOUT
    // GLUI_Rollout * distPanel
    // = glui->add_rollout_to_panel	( rootPanel, "Distance", false );
    GLUI_Panel * distPanel
	= glui->add_panel_to_panel	( rootPanel, "Distance" );
#else	// USE_ROLLOUT
    GLUI_Panel * distPanel
	= glui->add_panel_to_panel	( rootPanel, "Distance" );
#endif	// USE_ROLLOUT

    GLUI_Panel * limitPanel
	= glui->add_panel_to_panel	( distPanel, "Limits" );

    GLUI_Panel * lowerPanel
	= glui->add_panel_to_panel	( limitPanel, "Lower" );
    GLUI_RadioGroup * lowerGroup
	= glui->add_radiogroup_to_panel	( lowerPanel, &lowerSwitch,
					  UPDATE_LOWER, (GLUI_Update_CB)perform_task );
    glui->add_radiobutton_to_group	( lowerGroup, "    0" );
    glui->add_radiobutton_to_group	( lowerGroup, "  20" );
    glui->add_radiobutton_to_group	( lowerGroup, "  40" );
    glui->add_radiobutton_to_group	( lowerGroup, "  60" );
    glui->add_radiobutton_to_group	( lowerGroup, "  80" );
    glui->add_radiobutton_to_group	( lowerGroup, "100" );

    glui->add_column_to_panel		( limitPanel, GLUI_FALSE );

    GLUI_Panel * upperPanel
	= glui->add_panel_to_panel	( limitPanel, "Upper" );
    GLUI_RadioGroup * upperGroup
	= glui->add_radiogroup_to_panel	( upperPanel, &upperSwitch,
					  UPDATE_UPPER, (GLUI_Update_CB)perform_task );
    glui->add_radiobutton_to_group	( upperGroup, "  20" );
    glui->add_radiobutton_to_group	( upperGroup, "  40" );
    glui->add_radiobutton_to_group	( upperGroup, "  60" );
    glui->add_radiobutton_to_group	( upperGroup, "  80" );
    glui->add_radiobutton_to_group	( upperGroup, "100" );
    glui->add_radiobutton_to_group	( upperGroup, "infty" );

    glui->add_button_to_panel		( distPanel, "Segment",
					  DIST_SEGMENT, (GLUI_Update_CB)perform_task );
    
//------------------------------------------------------------------------------
//	Transformation panel
//------------------------------------------------------------------------------

#ifdef USE_ROLLOUT
    // GLUI_Rollout * transformPanel
    // = glui->add_rollout_to_panel	( rootPanel, "Transformation", false );
    GLUI_Panel * transformPanel
	= glui->add_panel_to_panel	( rootPanel, "Transformation" );
#else	// USE_ROLLOUT
    GLUI_Panel * transformPanel
	= glui->add_panel_to_panel	( rootPanel, "Transformation" );
#endif	// USE_ROLLOUT

    GLUI_Spinner * xSpinner =
	glui->add_spinner_to_panel	( transformPanel, "X Translate",
					  GLUI_SPINNER_FLOAT,
					  &translate_x,
					  TRANSFORM_MAP, (GLUI_Update_CB)perform_task );
#ifdef ENABLE_MAP_COORD_NORMALIZATION
    xSpinner->set_float_limits		( -2.0, 2.0, GLUI_LIMIT_CLAMP );
#else	// ENABLE_MAP_COORD_NORMALIZATION
    xSpinner->set_float_limits		( 130.0, 150.0, GLUI_LIMIT_CLAMP );
#endif	// ENABLE_MAP_COORD_NORMALIZATION

    GLUI_Spinner * ySpinner =
	glui->add_spinner_to_panel	( transformPanel, "Y Translate",
					  GLUI_SPINNER_FLOAT,
					  &translate_y,
					  TRANSFORM_MAP, (GLUI_Update_CB)perform_task );
#ifdef ENABLE_MAP_COORD_NORMALIZATION
    ySpinner->set_float_limits		( -2.0, 2.0, GLUI_LIMIT_CLAMP );
#else	// ENABLE_MAP_COORD_NORMALIZATION
    ySpinner->set_float_limits		( 30.0, 46.0, GLUI_LIMIT_CLAMP );
#endif	// ENABLE_MAP_COORD_NORMALIZATION

    GLUI_Spinner * scaleSpinner =
	glui->add_spinner_to_panel	( transformPanel, "Scale",
					  GLUI_SPINNER_FLOAT,
					  &map_scale,
					  TRANSFORM_MAP, (GLUI_Update_CB)perform_task );
#ifdef ENABLE_MAP_COORD_NORMALIZATION
    scaleSpinner->set_float_limits	( 0.5, 4.0, GLUI_LIMIT_CLAMP );
#else	// ENABLE_MAP_COORD_NORMALIZATION
    scaleSpinner->set_float_limits	( 0.1, 1.0, GLUI_LIMIT_CLAMP );
#endif	// ENABLE_MAP_COORD_NORMALIZATION

//------------------------------------------------------------------------------
//	CSV panel
//------------------------------------------------------------------------------
    GLUI_Panel * CSVPanel
	= glui->add_panel_to_panel	( rootPanel, "CSV" );
    glui->add_button_to_panel		( CSVPanel, "CSV file",
					  CSV_OUTPUT, (GLUI_Update_CB)perform_task );


//------------------------------------------------------------------------------
//	Sampling panel
//------------------------------------------------------------------------------

    GLUI_Panel * samplingPanel
	= glui->add_panel_to_panel	( rootPanel, "Sampling" );
    GLUI_Listbox * factorList
	= glui->add_listbox_to_panel	( samplingPanel, "Factor:",
					  &factorSwitch,
					  CHANGE_FACTOR, (GLUI_Update_CB)perform_task );
    for ( unsigned int k = 0; k < NUM_SKIP_FACTORS; ++k ) {
	ostringstream ostr;
	ostr << skip_factor[ k ] << ends;
	factorList->add_item		( skip_factor[ k ], ostr.str().c_str() );
    }
    factorList->set_int_val		( factorSwitch );

    //glui->add_button_to_panel		( samplingPanel, "Downsample",
    //				  DOWNSAMPLE_MAP, (GLUI_Update_CB)perform_task );
    glui->add_button_to_panel		( samplingPanel, "Re-sample",
					  RESAMPLE_MAP, (GLUI_Update_CB)perform_task );
    
//------------------------------------------------------------------------------
//	Capture panel
//------------------------------------------------------------------------------
#ifndef RIGHTMOST_PANEL
    glui->add_column_to_panel		( rootPanel, GLUI_FALSE );
#endif	// RIGHTMOST_PANEL

#ifdef USE_ROLLOUT
    GLUI_Rollout * capturePanel
	= glui->add_rollout_to_panel	( rootPanel, "Capture", false );
#else	// USE_ROLLOUT
    GLUI_Panel * capturePanel
	= glui->add_panel_to_panel	( rootPanel, "Capture" );
#endif	// USE_ROLLOUT
    glui->add_button_to_panel		( capturePanel, "Shape",
					  CAPTURE_SHAPE, (GLUI_Update_CB)select_item );
    glui->add_button_to_panel		( capturePanel, "PQ",
					  CAPTURE_PQ, (GLUI_Update_CB)select_item );
    glui->add_button_to_panel		( capturePanel, "PR",
					  CAPTURE_PR, (GLUI_Update_CB)select_item );
    glui->add_button_to_panel		( capturePanel, "QR",
					  CAPTURE_QR, (GLUI_Update_CB)select_item );
    glui->add_button_to_panel		( capturePanel, "All",
					  CAPTURE_ALL, (GLUI_Update_CB)select_item );
    glui->add_button_to_panel		( capturePanel, "EPS",
					  WIREFRAME_EPS, (GLUI_Update_CB)select_item );



//------------------------------------------------------------------------------
//	Quit button
//------------------------------------------------------------------------------
    // glui->add_button_to_panel( rootPanel, "Quit",
    // QUIT, (GLUI_Update_CB)select_item );
    glui->add_button( "Quit",
		      QUIT, (GLUI_Update_CB)perform_task );

    // tell glui what graphics window it needs to post a redisplay to:
    glui->set_main_gfx_window( win_shape );

    // set the graphics window's idle function:
    GLUI_Master.set_glutIdleFunc( NULL );
}



//------------------------------------------------------------------------------
//	Main boday
//------------------------------------------------------------------------------
int main( int argc, char *argv[] ) 
{
//------------------------------------------------------------------------------
//	configuration setup
//------------------------------------------------------------------------------
    string path = "configs/default.conf";
    int option;

    //------------------------------------------------------------------------------
    //	Command line handling
    // "c:"	allow -c with one argument for the option
    // reference
    // "c"	no argument
    // "c::"	argument is optional (allow both no arguments and one argument)
    while ( ( option = getopt( argc, argv, "c:" ) ) != -1 ) {
	switch ( option ) {
	  case 'c':		// option: -c <config file name>
	      path = optarg;
	      break;
	  case '?':		// unknown option
	      cerr << argv[ 0 ] << ": Unknown option: default setup will be used" << endl;
	      break;
	}
    }
    cerr << "loading configuration from " << path << endl;

    //------------------------------------------------------------------------------
    //	read the configuration file
    OZ::Option::Config conf( path );
    //---------- 
    if ( conf.has( "place" ) )	place = conf.gets( "place" );
    cerr << HERE << " place = [" << place << "]" << endl;
    //---------- 
    if ( conf.has( "tessellation" ) ) tessellation = conf.gets( "tessellation" );
    cerr << HERE << " tessellation = [" << tessellation << "]" << endl;
    //---------- 
    if ( conf.has( "density" ) ) density = conf.gets( "density" );
    cerr << HERE << " density = [" << density << "]" << endl;
    //----------
    if ( conf.has( "x_min" ) ) x_min = conf.getlf( "x_min" );
    cerr << HERE << " x_min = " << x_min << endl;
    if ( conf.has( "x_max" ) ) x_max = conf.getlf( "x_max" );
    cerr << HERE << " x_max = " << x_max << endl;
    if ( conf.has( "y_min" ) ) y_min = conf.getlf( "y_min" );
    cerr << HERE << " y_min = " << y_min << endl;
    if ( conf.has( "y_max" ) ) y_max = conf.getlf( "y_max" );
    cerr << HERE << " y_max = " << y_max << endl;
    rangeX = interval< double >( x_min, x_max );
    rangeY = interval< double >( y_min, y_max );
    cerr << HERE << " rangeX = " << rangeX << endl;
    cerr << HERE << " rangeY = " << rangeY << endl;
    //----------
    // associated calculations
    translate_x = median( rangeX );
    translate_y = median( rangeY );
    map_scale	= 0.5 * std::max( width( rangeX ), width( rangeY )*3.0/2.0 );
    scale_x	= map_scale;
    scale_y	= 2.0 * map_scale / 3.0;
    cerr << HERE << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    cerr << HERE << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << endl;
    cerr << HERE << " map_scale = " << map_scale
	 << " scale_x = " << scale_x
	 << " scale_y = " << scale_y << endl;
    cerr << HERE << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    cerr << HERE << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << endl;
    //----------
    // the values P, Q, and R are selected through the config file settings.
    if ( conf.has( "idP" ) ) idP = conf.geti( "idP" );
    cerr << HERE << " idP = " << idP << endl;
    if ( conf.has( "idQ" ) ) idQ = conf.geti( "idQ" );
    cerr << HERE << " idQ = " << idQ << endl;
    if ( conf.has( "idR" ) ) idR = conf.geti( "idR" );
    cerr << HERE << " idR = " << idR << endl;
    //----------
    if ( conf.has( "file_name" ) ) file_name = conf.gets( "file_name" );
    cerr << HERE << " file_name = " << file_name << endl;
    bin_file_name = file_name + ".bin";
    csv_file_name = file_name + ".csv";
    
//------------------------------------------------------------------------------
//	main body
//------------------------------------------------------------------------------

#ifdef SKIP
    cerr << HERE << " test distance = "
	 << calcDistHubeny( 37.0, 140.0,
			    37.0, 140.1 );
#endif	// SKIP

    glutInit( &argc, argv );                /* Initialize GLUT */

    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize ( width_shape, height_shape );
#ifdef HIDE_RANGE_WINDOW
    glutInitWindowPosition( DEFAULT_WIDTH, 0 );
#else	// HIDE_RANGE_WINDOW
    glutInitWindowPosition( 2*DEFAULT_WIDTH, 0 );
#endif	// HIDE_RANGE_WINDOW
    win_shape = glutCreateWindow ( "Shape" );

    initShape();
    initTextures();

    // #ifdef SKIP
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << " map_scale = " << map_scale << endl;
    // #endif	// SKIP

    //------------------------------------------------------------------------------
    //	Shape Window
    //------------------------------------------------------------------------------
    glutDisplayFunc	( display_shape );
    glutReshapeFunc	( reshape_shape );
    glutKeyboardFunc	( keyboard_shape );
    glutSpecialFunc	( special_shape );
    glutMouseFunc	( mouse_shape );
    glutMotionFunc	( motion_shape );
    glutVisibilityFunc	( visibility_shape );


    // #ifdef SKIP
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << " map_scale = " << map_scale << endl;
    // #endif	// SKIP

    //------------------------------------------------------------------------------
    //	PQ range window
    //------------------------------------------------------------------------------
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize	( width_PQ, height_PQ );
    glutInitWindowPosition( 0, 0 );
    win_PQ	= glutCreateWindow ( "Range PQ" );
    initRange();
    glutDisplayFunc	( display_PQ );
    glutReshapeFunc	( reshape_scatterplot );
    glutMouseFunc	( mouse_PQ );
    glutMotionFunc	( motion_PQ );
    glutVisibilityFunc	( visibility_PQ );

    //------------------------------------------------------------------------------
    //	PR range window
    //------------------------------------------------------------------------------
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize	( width_PR, height_PR );
#ifdef HIDE_RANGE_WINDOW
    glutInitWindowPosition( 0, DEFAULT_HEIGHT + TITLEBAR_GAP );
#else	// HIDE_RANGE_WINDOW
    glutInitWindowPosition( 0, DEFAULT_HEIGHT + 64 );
#endif	// HIDE_RANGE_WINDOW
    win_PR	= glutCreateWindow ( "Range PR" );
    initRange();
    glutDisplayFunc	( display_PR );
    glutReshapeFunc	( reshape_scatterplot );
    glutMouseFunc	( mouse_PR );
    glutMotionFunc	( motion_PR );
    glutVisibilityFunc	( visibility_PR );

    //------------------------------------------------------------------------------
    //	QR range window
    //------------------------------------------------------------------------------
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize	( width_QR, height_QR );
#ifdef HIDE_RANGE_WINDOW
    glutInitWindowPosition( 0, DEFAULT_HEIGHT*2 + TITLEBAR_GAP*3 );
#else	// HIDE_RANGE_WINDOW
    glutInitWindowPosition( DEFAULT_WIDTH, DEFAULT_HEIGHT+64 );
#endif	// HIDE_RANGE_WINDOW
    win_QR	= glutCreateWindow ( "Range QR" );
    initRange();
    glutDisplayFunc	( display_QR );
    // glutReshapeFunc	( reshape_range );
    glutReshapeFunc	( reshape_scatterplot );
    glutMouseFunc	( mouse_QR );
    glutMotionFunc	( motion_QR );
    glutVisibilityFunc	( visibility_QR );


    //------------------------------------------------------------------------------
    //	Range window
    //------------------------------------------------------------------------------
    glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize	( width_range, height_range );
    glutInitWindowPosition( DEFAULT_WIDTH, 0 );
    win_range	= glutCreateWindow ( "Range" );

    initRange();

    // #ifdef SKIP
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << " map_scale = " << map_scale << endl;
    // #endif	// SKIP

    glutReshapeFunc	( reshape_range );
    //    glutKeyboardFunc	( keyboard_range );
    glutDisplayFunc	( display_range );
    glutMouseFunc	( mouse_range );
    glutMotionFunc	( motion_range );
    glutVisibilityFunc	( visibility_range );

#ifdef HIDE_RANGE_WINDOW
    glutHideWindow	();
#endif	// HIDE_RANGE_WINDOW

    //------------------------------------------------------------------------------
    //	Panels
    //------------------------------------------------------------------------------
    initGLUI();

    // #ifdef SKIP
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << " map_scale = " << map_scale << endl;
    // #endif	// SKIP

    //------------------------------------------------------------------------------
    //	Menus
    //------------------------------------------------------------------------------
    int menu_render	= glutCreateMenu( select_item );
    // glutAddMenuEntry	( "Flat",		FLAT_MODE );
    glutAddMenuEntry	( "Basement",		BASEMENT_MODE );
    glutAddMenuEntry	( "Wireframe",		WIREFRAME_MODE );
    glutAddMenuEntry	( "Texture",		TEXTURE_MODE );
#ifdef SKIP
    glutAddMenuEntry	( "==========",		-1 );
    glutAddMenuEntry	( "Switch raster",	SWITCH_RASTER_MODE );
#endif	// SKIP
    // glutAddMenuEntry	( "Plot",		PLOT_MODE );

#ifdef NO_NEED
    int menu_vars	= glutCreateMenu( select_item );
    glutAddMenuEntry	( "CS137 & Height & Distance",	CS137_HEIGHT_DISTANCE );
    glutAddMenuEntry	( "CS137 & Height & Gradient",	CS137_HEIGHT_GRAD );
    glutAddMenuEntry	( "CS137 & Height & Accum",	CS137_HEIGHT_ACCUM );
#endif	// NO_NEED

    unsigned int res;

    int menu_PQR	= glutCreateMenu( select_item );
    res = 1;
    for ( int k = 0; k < NUM_LEVELS; ++k ) {
	ostringstream ostr;
	ostr << setfill( '0' ) << setw( 3 ) << res << ends;
	glutAddMenuEntry( ostr.str().c_str(), RESOLUTION_BASE_ALL + k );
	res *= 2;
    }

    int menu_P		= glutCreateMenu( select_item );
    res = 1;
    for ( int k = 0; k < NUM_LEVELS; ++k ) {
	ostringstream ostr;
	ostr << setfill( '0' ) << setw( 3 ) << res << ends;
	glutAddMenuEntry( ostr.str().c_str(), RESOLUTION_BASE_P + k );
	res *= 2;
    }

    int menu_Q		= glutCreateMenu( select_item );
    res = 1;
    for ( int k = 0; k < NUM_LEVELS; ++k ) {
	ostringstream ostr;
	ostr << setfill( '0' ) << setw( 3 ) << res << ends;
	glutAddMenuEntry( ostr.str().c_str(), RESOLUTION_BASE_Q + k );
	res *= 2;
    }

    int menu_R		= glutCreateMenu( select_item );
    res = 1;
    for ( int k = 0; k < NUM_LEVELS; ++k ) {
	ostringstream ostr;
	ostr << setfill( '0' ) << setw( 3 ) << res << ends;
	glutAddMenuEntry( ostr.str().c_str(), RESOLUTION_BASE_R + k );
	res *= 2;
    }

    // #ifdef SKIP
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << " map_scale = " << map_scale << endl;
    // #endif	// SKIP

    int menu_view	= glutCreateMenu( select_item );
    glutAddMenuEntry	( "Init viewpoint",	INIT_VIEWPOINT );
    glutAddMenuEntry	( "Set viewpoint",	CHANGE_VIEWPOINT );

    int menu_capture	= glutCreateMenu( select_item );
    glutAddMenuEntry	( "Dump Selected",	DUMP_SELECTED );
    glutAddMenuEntry	( "Capture Shape",	CAPTURE_SHAPE );
    glutAddMenuEntry	( "Capture PQ",		CAPTURE_PQ );
    glutAddMenuEntry	( "Capture PR",		CAPTURE_PR );
    glutAddMenuEntry	( "Capture QR",		CAPTURE_QR );
    glutAddMenuEntry	( "Capture Range",	CAPTURE_RANGE );
    glutAddMenuEntry	( "Capture All",	CAPTURE_ALL );

    int menu_style	= glutCreateMenu( select_item );
    glutAddMenuEntry	( "OUTLINE ON",		OUTLINE_ON );
    glutAddMenuEntry	( "OUTLINE OFF",	OUTLINE_OFF );
    glutAddMenuEntry	( "FILL ON",		FILL_ON );
    glutAddMenuEntry	( "FILL OFF",		FILL_OFF );
    glutAddMenuEntry	( "ALL ON",		COMMON_ON );
    glutAddMenuEntry	( "ALL OFF",		COMMON_OFF );

    int menu_land	= glutCreateMenu( select_item );
    glutAddMenuEntry	( "All ON",			ALL_ON );
    glutAddMenuEntry	( "All OFF",			ALL_OFF );
    glutAddMenuEntry	( "-----",			BLANK_ITEM );
    glutAddMenuEntry	( "Rice (Tanbo) ON",		RICE_ON );
    glutAddMenuEntry	( "Rice (Tanbo) OFF",		RICE_OFF );
    glutAddMenuEntry	( "Farm (Nouyouchi) ON",	FARM_ON );
    glutAddMenuEntry	( "Farm (Nouyouchi) OFF",	FARM_OFF );
    glutAddMenuEntry	( "Forest (Shinrin) ON",	FOREST_ON );
    glutAddMenuEntry	( "Forest (Shinrin) OFF",	FOREST_OFF );
    glutAddMenuEntry	( "Waste (Arachi) ON",		WASTE_ON );
    glutAddMenuEntry	( "Waste (Arachi) OFF",		WASTE_OFF );
    glutAddMenuEntry	( "Urban (Tatemono) ON",	URBAN_ON );
    glutAddMenuEntry	( "Urban (Tatemono) OFF",	URBAN_OFF );
    glutAddMenuEntry	( "Road (Douro) ON",		ROAD_ON );
    glutAddMenuEntry	( "Road (Douro) OFF",		ROAD_OFF );
    glutAddMenuEntry	( "Railway (Tetsudou) ON",	RAILWAY_ON );
    glutAddMenuEntry	( "Railway (Tetsudou) OFF",	RAILWAY_OFF );
    glutAddMenuEntry	( "Marsh (Kasen/Koshou) ON",	MARSH_ON );
    glutAddMenuEntry	( "Marsh (Kasen/Koshou) OFF",	MARSH_OFF );
    glutAddMenuEntry	( "Beach (Kaihin) ON",		BEACH_ON );
    glutAddMenuEntry	( "Beach (Kaihin) OFF",		BEACH_OFF );
    glutAddMenuEntry	( "Ocean (Kaisui) ON",		OCEAN_ON );
    glutAddMenuEntry	( "Ocean (Kaisui) OFF",		OCEAN_OFF );
    glutAddMenuEntry	( "Golf (Gorufu) ON",		GOLF_ON );
    glutAddMenuEntry	( "Golf (Gorufu) OFF",		GOLF_OFF );
    glutAddMenuEntry	( "Other (Sonota) ON",		OTHER_ON );
    glutAddMenuEntry	( "Other (Sonota) OFF",		OTHER_OFF );

    int menu_dist	= glutCreateMenu( select_item );
    glutAddMenuEntry	( "Distance 0-20",		DIST_LIMIT_000_020 );
    glutAddMenuEntry	( "Distance 0-40",		DIST_LIMIT_000_040 );
    glutAddMenuEntry	( "Distance 0-60",		DIST_LIMIT_000_060 );
    glutAddMenuEntry	( "Distance 0-80",		DIST_LIMIT_000_080 );
    glutAddMenuEntry	( "Distance 0-100",		DIST_LIMIT_000_100 );
    glutAddMenuEntry	( "Distance 20-40",		DIST_LIMIT_020_040 );
    glutAddMenuEntry	( "Distance 40-60",		DIST_LIMIT_040_060 );
    glutAddMenuEntry	( "Distance 60-80",		DIST_LIMIT_060_080 );
    glutAddMenuEntry	( "Distance 80-100",		DIST_LIMIT_080_100 );
    glutAddMenuEntry	( "Distance Limit OFF",		DIST_LIMIT_OFF );

    int menu_top;
    menu_top		= glutCreateMenu( select_item );
    glutAddSubMenu	( "Render",		menu_render );
#ifdef SKIP
    glutAddSubMenu	( "Vars",		menu_vars );
#endif	// SKIP
    glutAddSubMenu	( "Resolution PQR",	menu_PQR );
    glutAddSubMenu	( "Resolution P",	menu_P );
    glutAddSubMenu	( "Resolution Q",	menu_Q );
    glutAddSubMenu	( "Resolution R",	menu_R );
    glutAddSubMenu	( "Style",		menu_style );
    glutAddSubMenu	( "Land Use Type",	menu_land );
    glutAddSubMenu	( "Distance Threshold",	menu_dist );
#ifdef SKIP
    glutAddSubMenu	( "Temporal Snapshots",	menu_time );
    glutAddMenuEntry	( "Process All",	PROCESS_ALL );
#endif	// SKIP
    glutAddSubMenu	( "View",		menu_view );
    glutAddSubMenu	( "File I/O",		menu_capture );
    glutAddMenuEntry	( "Quit",		QUIT );
    glutAttachMenu( GLUT_LEFT_BUTTON );

    // #ifdef SKIP
    cerr << HERE << " translate_x = " << translate_x
	 << " translate_y = " << translate_y << " map_scale = " << map_scale << endl;
    // #endif	// SKIP

    //------------------------------------------------------------------------------
    //	Preprocess
    //------------------------------------------------------------------------------
    glutTimerFunc( 50, timer, 0 );
    glutMainLoop();                       /* Start GLUT event-processing loop */

    return 0;

}
