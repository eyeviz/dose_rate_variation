//==============================================================================
// LandMap.cpp
//	: program file for the land map as a grid of satellite images
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2024-01-09 02:04:12 shigeo"
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "LandMap.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------

unsigned char LandMap::_category[ NUM_CATEGORIES ][ 3 ] = {
    {   0,   0, 100 },		// water area
    { 255,   0,   0 },		// urban area
    {   0, 128, 255 },		// rice paddy field area
    { 255, 193, 191 },		// farmland area
    { 255, 255,   0 },		// meadow area
    { 128, 255,   0 },		// deciduous broadleaf tree area
    {   0, 255, 128 },		// deciduous coniferous tree araa
    {  86, 172,   0 },		// evergreen broadleaf tree area
    {   0, 172,  86 },		// evergreen coniferous tree araa
    { 128, 100,   0 },		// bare soil area
    {   0, 172,  86 },		// bamboo forest area
    {   0, 172,  86 },		// solar panel area
};

//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------

// Load the map over the Fukushima Pref.
void LandMap::_init( void )
{
    _tile[ 0 ][ 0 ].loadData( 36, 139 );
    _tile[ 0 ][ 1 ].loadData( 36, 140 );

    _tile[ 1 ][ 0 ].loadData( 37, 139 );
    _tile[ 1 ][ 1 ].loadData( 37, 140 );
    _tile[ 1 ][ 2 ].loadData( 37, 141 );
}

// return the pixel color at the specific positoin
void LandMap::_pixel( const double & latitude, const double & longitude,
		      unsigned char & R, unsigned char & G, unsigned char & B )
{
    double fy = latitude - BASE_LATITUDE;
    double fx = longitude - BASE_LONGITUDE;
    unsigned int ix = ( int )floor( fx );
    unsigned int iy = ( int )floor( fy );
    fx -= ( double )ix;
    fy -= ( double )iy;
    LandTile & curTile = _tile[ iy ][ ix ]; 
    int px = min( ( int )(curTile.cols() - 1), ( int )round( fx * curTile.cols() - 0.5 ) );
    int py = curTile.rows() - 1 - min( ( int )(curTile.rows() - 1), ( int )round( fy * curTile.rows() - 0.5 ) );
    R = curTile.image().at< cv::Vec3b >( py, px )[ 2 ]; 
    G = curTile.image().at< cv::Vec3b >( py, px )[ 1 ]; 
    B = curTile.image().at< cv::Vec3b >( py, px )[ 0 ]; 
#ifdef DEBUG
    cerr << " ix = " << ix << " iy = " << iy << endl;
    cerr << " fx = " << fx << " fy = " << fy << endl;
    cerr << " px = " << px << " py = " << py << endl;
    cerr << " ( R, G, B ) = " << (int)R << " , " << (int)G << " , " << (int)B << endl;
#endif	// DEBUG
}
    
// return the land use type as string from the color
unsigned int LandMap::_landID( const unsigned char & R, 
			       const unsigned char & G,
			       const unsigned char & B )
{
    unsigned int id = NUM_CATEGORIES;
    for ( unsigned int i = 0; i < NUM_CATEGORIES; ++i ) {
	if ( ( LandMap::_category[ i ][ 0 ] == R ) &&
	     ( LandMap::_category[ i ][ 1 ] == G ) &&
	     ( LandMap::_category[ i ][ 2 ] == B ) ) {
	    id = i;
	}
    }
    return id;
}

    
// return the land use type as string from the color
string LandMap::_landuse( const unsigned int & id )
{
    string label;
    switch ( id ) {
      case 0:
	  label = "water";
	  break;
      case 1:
	  label = "urban";
	  break;
      case 2:
	  label = "rice baddy field";
	  break;
      case 3:
	  label = "farmland";
	  break;
      case 4:
	  label = "meadow";
	  break;
      case 5:
	  label = "deciduous broadleaf trees";
	  break;
      case 6:
	  label = "deciduous coniferous trees";
	  break;
      case 7:
	  label = "evergreen broadleaf trees";
	  break;
      case 8:
	  label = "evergreen coniferous trees";
	  break;
      case 9:
	  label = "area soil area";
	  break;
      case 10:
	  label = "bamboo forest area";
	  break;
      case 11:
	  label = "solar panel area";
	  break;
      default:
	  label = "unknown";
	  break;
    }

    return label;
}    
    

//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------

//
//  LandMap::LandMap --	default constructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandMap::LandMap() 
{
    _init();
}



//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------

//
//  LandMap::~LandMap --	destructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandMap::~LandMap()
{

}


//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------

// end of header file
// Do not add any stuff under this line.
