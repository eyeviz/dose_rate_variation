//==============================================================================
// LandPlot.cc
//	: program file for a set of land use plots
//
//------------------------------------------------------------------------------
//
//				Date: Sat Apr  7 23:03:25 2018
//
//==============================================================================


//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------
#include <ctime>
#include <fstream>
#include <boost/format.hpp>
#include "Common.h"
#include "LandPlot.h"

using namespace std;


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------
// #define SHDEBUG


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------

//
//  LandPlot::_addMeshCode --	add a string as a mesh code
//
//  Inputs
//	i	: ID of the record
//	j	: ID of the field
//	hDBR	: DEF handle
//
//  Outputs
//	none
//
void LandPlot::_addMeshCode( const unsigned int iRecord,
			     const unsigned int iField,
			     DBFHandle & hDBF )
{
    string nstr;

    // Must be the head mesh code
    assert( iField == 0 );

    nstr.assign( DBFReadStringAttribute( hDBF, iRecord, iField ) );
#ifdef SHDEBUG
    cerr << "[  0 ] = " << nstr << endl;
#endif	// SHDEBUG
    _meshcode.push_back( nstr );
}
    
//
//  LandType::_addLandType --	add a string as a type of land use
//
//  Inputs
//	i	: ID of the record
//	j	: ID of the field
//	hDBR	: DEF handle
//
//  Outputs
//	none
//
void LandPlot::_addLandType( const unsigned int iRecord,
			    const unsigned int iField,
			    DBFHandle & hDBF )
{
    string nstr;

    // Must be the head mesh code
    assert( iField == 1 );

    nstr.assign( DBFReadStringAttribute( hDBF, iRecord, iField ) );
#ifdef SHDEBUG
    cerr << "[  0 ] = " << nstr << endl;
#endif	// SHDEBUG
    assert( nstr.length() == 4 );

    // 0100:	rice field
    // 0200:	other farmland
    // 0500:	forest
    // 0600:	wasteland (abandoned land)
    // 0700:	buidling sites
    // 0901:	roads
    // 0902:	railways
    // 1000:	other sites
    // 1100:	river and lakes
    // 1400:	seaside
    // 1500:	ocean
    // 1600:	golf courses
    if ( std::equal( nstr.begin(), nstr.end(), "0100" ) ) {
	_landtype.push_back( RICE );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "0200" ) ) {
	_landtype.push_back( FARM );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "0500" ) ) {
	_landtype.push_back( FOREST );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "0600" ) ) {
	_landtype.push_back( WASTE );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "0700" ) ) {
	_landtype.push_back( URBAN );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "0901" ) ) {
	_landtype.push_back( ROAD );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "0902" ) ) {
	_landtype.push_back( RAILWAY );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "1000" ) ) {
	_landtype.push_back( OTHER );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "1100" ) ) {
	_landtype.push_back( MARSH );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "1400" ) ) {
	_landtype.push_back( BEACH );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "1500" ) ) {
	_landtype.push_back( OCEAN );
    }
    else if ( std::equal( nstr.begin(), nstr.end(), "1600" ) ) {
	_landtype.push_back( GOLF );
    }
    else {
	cerr << " Illegal land type : " << nstr << endl;
	exit( 0 );
    }
}

//
//  LandPlot::_addString --	add a string as a new record
//
//  Inputs
//	i	: ID of the record
//	j	: ID of the field
//	hDBR	: DEF handle
//
//  Outputs
//	none
//
void LandPlot::_addString( const unsigned int iRecord,
			  const unsigned int iField,
			  DBFHandle & hDBF )
{
    if ( iField == 0 ) {
	_addMeshCode( iRecord, iField, hDBF );
    }
    else if ( iField  == 1 ) {
	_addLandType( iRecord, iField, hDBF );
    }
    else {
	cerr << " Illegal field ID : " << iField << endl;
	exit( 0 );
    }
}

//
//  LandPlot::_addInteger --	add an integer value as a new record
//
//  Inputs
//	i	: ID of the record
//	j	: ID of the field
//	hDBR	: DEF handle
//
//  Outputs
//	none
//
void LandPlot::_addInteger( const unsigned int iRecord,
			   const unsigned int iField,
			   DBFHandle & hDBF )
{
    int ivalue = DBFReadIntegerAttribute( hDBF, iRecord, iField );
#ifdef SHDEBUG
    cerr << "[ " << setw( 2 ) << iField << " ] = " << ivalue << endl;
#endif	// SHDEBUG
    cerr << HERE << " Do not call this function " << endl;
}

//
//  LandPlot::_addDouble --	add a floating value as a new record
//
//  Inputs
//	i	: ID of the record
//	j	: ID of the field
//	hDBR	: DEF handle
//
//  Outputs
//	none
//
void LandPlot::_addDouble( const unsigned int iRecord,
			  const unsigned int iField,
			  DBFHandle & hDBF )
{
    double rvalue = DBFReadDoubleAttribute( hDBF, iRecord, iField );
#ifdef SHDEBUG
    cerr << "[ " << setw( 2 ) << iField << " ] = " << rvalue << endl;
#endif	// SHDEBUG
    cerr << HERE << " Do not call this function " << endl;
}

//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------

//
//  LandPlot::_type2code --	convert a land use type to a numerial code
//
//  Inputs
//	lt	: land use type
//
//  Outputs
//	numerical code
//
int LandPlot::_type2code( const LandType lt )
{
    int code = 0;
    switch ( lt ) {
      case RICE:		// 0100: rice field
	  code = 100;
	  break;
      case FARM:		// 0200: other farmland
	  code = 200;
	  break;
      case FOREST:		// 0500: forest
	  code = 500;
	  break;
      case WASTE:		// 0600: wasteland
	  code = 600;
	  break;
      case URBAN:		// 0700: building sites
	  code = 700;
	  break;
      case ROAD:		// 0901: roads
	  code = 901;
	  break;
      case RAILWAY:		// 0902: railways
	  code = 902;
	  break;
      case OTHER:		// 1000: other sites
	  code = 1000;
	  break;
      case MARSH:		// 1100: river and lakes
	  code = 1100;
	  break;
      case BEACH:		// 1400: seaside
	  code = 1400;
	  break;
      case OCEAN:		// 1500: ocean
	  code = 1500;
	  break;
      case GOLF:		// 1600: golf courses
	  code = 1600;
	  break;
      case OUT:
      default:
	  code = 0;
	  break;
    }
    return code;
}


//
//  LandPlot::_code2type --	convert a numerical code to a land use type
//
//  Inputs
//	code	: numerical code
//
//  Outputs
//	land use type
//
LandType LandPlot::_code2type( const int code )
{
    LandType lt = OUT;
    switch ( code ) {
      case 100:			// 0100: rice field
	  lt = RICE;
	  break;
      case 200:			// 0200: other farmland
	  lt = FARM;
	  break;
      case 500:			// 0500: forest
	  lt = FOREST;
	  break;
      case 600:			// 0600: wasteland
	  lt = WASTE;
	  break;
      case 700:			// 0700: building sites
	  lt = URBAN;
	  break;
      case 901:			// 0901: roads
	  lt = ROAD;
	  break;
      case 902:			// 0902: railways
	  lt = RAILWAY;
	  break;
      case 1000:		// 1000: other sites
	  lt = OTHER;
	  break;
      case 1100:		// 1100: river and lakes
	  lt = MARSH;
	  break;
      case 1400:		// 1400: seaside
	  lt = BEACH;
	  break;
      case 1500:		// 1500: ocean
	  lt = OCEAN;
	  break;
      case 1600:		// 1600: golf courses
	  lt = GOLF;
	  break;
      default:
	  lt = OUT;
	  break;
    }
    return lt;
}


//
//  LandPlot::_printType --	print the land use type
//
//  Inputs
//	lt	: land use type
//
//  Outputs
//	none
//
void LandPlot::_printType( const LandType lt )
{
    int code = 0;
    switch ( lt ) {
      case RICE:		// 0100: rice field
	  cerr << " RICE " << ends;
	  break;
      case FARM:		// 0200: other farmland
	  cerr << " FARM " << ends;
	  break;
      case FOREST:		// 0500: forest
	  cerr << " FOREST " << ends;
	  break;
      case WASTE:		// 0600: wasteland
	  cerr << " WASTE " << ends;
	  break;
      case URBAN:		// 0700: building sites
	  cerr << " URBAN " << ends;
	  break;
      case ROAD:		// 0901: roads
	  cerr << " ROAD " << ends;
	  break;
      case RAILWAY:		// 0902: railways
	  cerr << " RAILWAY " << ends;
	  break;
      case OTHER:		// 1000: other sites
	  cerr << " OTHER " << ends;
	  break;
      case MARSH:		// 1100: river and lakes
	  cerr << " MARSH " << ends;
	  break;
      case BEACH:		// 1400: seaside
	  cerr << " BEACH " << ends;
	  break;
      case OCEAN:		// 1500: ocean
	  cerr << " OCEAN " << ends;
	  break;
      case GOLF:		// 1600: golf courses
	  cerr << " GOLF " << ends;
	  break;
      case OUT:
      default:
	  cerr << " ***UNKNOWN*** " << ends;
	  break;
    }
    return;
}


//
//  LandPlot::_init --	initialize the network (called once when necessary)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void LandPlot::_init( void )
{
    cerr << HERE << "in LandPlot::_init :  Nothing to be done" << endl;
}


//
//  LandPlot::_reset --	reset the network (called when being reused)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void LandPlot::_reset( void )
{
    _sample.clear();
    _meshcode.clear();
    _landtype.clear();
}


//
//  LandPlot::_normalize --	normalize the scalar field
//
//  Inputs
//	none
//
//  Outputs
//	none
//
void LandPlot::_normalize( void )
{
    cerr << HERE << " Nothing to do here" << endl;
#ifdef SKIP
    for ( unsigned int i = 0; i < _meshcode.size(); ++i ) {
	// sum up the land areas for each type
	double sum = 0.0;
	for ( unsigned int k = 0; k < NUM_USES; ++k )
	    sum += _usetype[ k ][ i ];
	// normalize
	for ( unsigned int k = 0; k < NUM_USES; ++k )
	    _usetype[ k ][ i ] /= sum;

#ifdef SHDEBUG
	if ( i % 10 == 0 )
	    cerr << "|   rice|   farm| forest|  waste|  build|  roads|railway|  other|  river|seaside|  ocean|   golf" << endl;

	// output
	for ( unsigned int k = 0; k < NUM_USES; ++k )
	    cerr << boost::format( " %7.2f" ) % _usetype[ k ][ i ];
	cerr << endl;
#endif	// SHDEBUG
    }
#endif	// SKIP
}


//
//  LandPlot::_append -- load the height samples
//
//  Inputs
//      shpname       : shp file name
//      dbfname       : dbf file name
//
//  Outputs
//      none
//
void LandPlot::_append( const char * filename )
{
    ostringstream shpname, dbfname;

    shpname << filename << ".shp" << ends;
    dbfname << filename << ".dbf" << ends;
//------------------------------------------------------------------------------
//	Opening the SHP file
//------------------------------------------------------------------------------
    SHPHandle hSHP = SHPOpen( shpname.str().c_str(), "rb" );
    if ( hSHP == NULL ) {
        cerr << HERE << "Unable to open: " << shpname.str() << endl;
        return;
    }

//------------------------------------------------------------------------------
//	Opening the BDF file
//------------------------------------------------------------------------------
    // read attributes
    DBFHandle hDBF = DBFOpen( dbfname.str().c_str(), "rb" );
    if ( hDBF == NULL ){
        cerr << HERE << "Unable to open: " << dbfname.str() << endl;
	if ( hSHP != NULL ) SHPClose( hSHP );
	return;
    }
    //	If there is no data in this file let the user know.	
    if ( DBFGetFieldCount( hDBF ) == 0 ) {
	cerr << HERE << "There are no fields in this table!" << endl;
	if ( hSHP != NULL ) SHPClose( hSHP );
	return;
    }

//------------------------------------------------------------------------------
//	Loading data from the SHP file
//------------------------------------------------------------------------------
    int		nShapeType, nEntities;
    double      adfMinBound[4], adfMaxBound[4];
    SHPGetInfo( hSHP, &nEntities, &nShapeType, adfMinBound, adfMaxBound );
    //#ifdef SHDEBUG
    cerr << HERE << " nEntities = " << nEntities << endl;
    //#endif	// SHDEBUG

    for ( int i = 0; i < nEntities; i++ ) {
        SHPObject *psElem =	SHPReadObject( hSHP, i );
#ifdef SHDEBUG
	cerr << HERE << " psShape->nVertices = " << psElem->nVertices << endl;
#endif	// SHDEBUG
	unsigned int nCorners = psElem->nVertices;
	if ( psElem->nVertices == 5 ) nCorners = 4;
	
	double *padfX =		new double[ psElem->nVertices ];
	double *padfY =		new double[ psElem->nVertices ];
	double *padfZ =		new double[ psElem->nVertices ];

	std::vector< D_Point2 > corner;

        // for ( int j = 0, iPart = 1; j < psElem->nVertices; j++ )
        for ( int j = 0; j < nCorners; j++ ) {
	    padfX[ j ] = psElem->padfX[ j ];
	    padfY[ j ] = psElem->padfY[ j ];
	    padfZ[ j ] = psElem->padfZ[ j ];
#ifdef SHDEBUG
	    cerr << HERE << " j = " << j << " coord = ( "
		 << padfX[ j ] << " , " << padfY[ j ] << " , "
		 << padfZ[ j ] << " ) " << endl;
#endif	// SHDEBUG
	    D_Point2 point( psElem->padfX[ j ], psElem->padfY[ j ] );
#ifdef SHDEBUG
	    cerr << HERE << " j = " << j << " point = " << point << endl;
#endif	// SHDEBUG
	    corner.push_back( point );
        }

	delete [] padfX;
	delete [] padfY;
	delete [] padfZ;
	SHPDestroyObject( psElem );

	// compute the average coordinates
	D_Point2 ave( 0.0, 0.0 );
	for ( unsigned int j = 0; j < corner.size(); ++j )
	    ave = ave + ( corner[ j ] - CGAL::ORIGIN );
	ave = CGAL::ORIGIN + ( 1.0/(double)corner.size() ) * ( ave - CGAL::ORIGIN );

	_sample.push_back( ave );
#ifdef SHDEBUG
	cerr << " corner0 = " << corner[ 0 ] << endl;
	cerr << " corner1 = " << corner[ 1 ] << endl;
	cerr << " corner2 = " << corner[ 2 ] << endl;
	cerr << " corner3 = " << corner[ 3 ] << endl;
	cerr << " ave = " << ave << endl;
	// getchar();
#endif	// SHDEBUG
    }


//------------------------------------------------------------------------------
//	Loading data from the DBF file
//------------------------------------------------------------------------------
    // check field
    unsigned int nFields	= DBFGetFieldCount( hDBF );
    unsigned int nRecords	= DBFGetRecordCount( hDBF );
    cerr << HERE << " DBF #field  = " << nFields << " #record = " << nRecords << endl;

    assert( nEntities == nRecords );

    for ( int iRecord = 0; iRecord < nRecords; iRecord++ ) {
#ifdef SHDEBUG
        cerr << " Record: " << iRecord << endl;
#endif	// SHDEBUG
        
	for ( int iField = 0; iField < nFields; iField++ ) {
            DBFFieldType	eType;
	    char		szTitle[11];
	    int			nWidth, nDecimals;
	    // char		*pszTypeName;
	    
            eType = DBFGetFieldInfo( hDBF, iField, szTitle, &nWidth, &nDecimals );

#ifdef SHDEBUG
            cerr << szTitle << endl;
#endif	// SHDEBUG
            
#ifdef SHDEBUG
	    cerr << boost::format( "Field %d" ) % iField << endl;
#endif	// SHDEBUG

	    switch( eType ) {
	      case FTString:	// "String"
#ifdef SHDEBUG
		  cerr << HERE << DBFReadStringAttribute( hDBF, iRecord, iField ) << endl;
		  cerr << HERE << " Type = String" << endl;
#endif	// SHDEBUG
		  _addString( iRecord, iField, hDBF );
		  break;
	      case FTInteger:	// "Integer"
#ifdef SHDEBUG
		  cerr << HERE << DBFReadIntegerAttribute( hDBF, iRecord, iField ) << endl;
		  cerr << HERE << " Type = Integer" << endl;
#endif	// SHDEBUG
		  _addInteger( iRecord, iField, hDBF );
		  break;
	      case FTDouble:	// "Double"
#ifdef SHDEBUG
		  cerr << HERE << DBFReadDoubleAttribute( hDBF, iRecord, iField ) << endl;
		  cerr << HERE << " Type = Double" << endl;
#endif	// SHDEBUG
		  _addDouble( iRecord, iField, hDBF );
		  break;
	      case FTLogical:	// "Logical"
#ifdef SHDEBUG
		  cerr << HERE << " Type = Logical" << endl;
#endif	// SHDEBUG
		  break;
	      default:		// "Invalid"
#ifdef SHDEBUG
		  cerr << HERE << " Type = Invalid" << endl;
#endif	// SHDEBUG
		  break;
	    }
#ifdef SHDEBUG
	    cerr << boost::format( " Title = %s, Width = %d, Decimals = %d" ) %
		szTitle % nWidth % nDecimals << endl;
#endif	// SHDEBUG
	}
    }

    // cerr << "nEntity = " << nEntities << endl;
    SHPClose( hSHP );
    DBFClose( hDBF );

    return;
}


#ifdef SKIP
//
//  LandPlot::_filter --	filter out the sample points outside the region
//
//  Inputs
//	map	: map data
//
//  Outputs
//	none
//
void LandPlot::_filter( const Map & map )
{
#ifdef SKIP
    cerr << HERE << " Number of sample points = " << _sample.size() << endl;
    cerr << HERE << " filtering sample points " << endl;

    vector< unsigned int > index;
    for ( unsigned int k = 0; k < _sample.size(); ++k ) {
	if ( k % 10000 == 0 ) cerr << ".";
	if ( !map.isInside( _sample[ k ] ) )
	    index.push_back( k );
    }
    cerr << endl;

    reverse( index.begin(), index.end() );

    cerr << HERE << " Number of sample points to be deleted = " << index.size() << endl;
    cerr << HERE << " deleting sample points outside the region " << endl;
    for ( unsigned int k = 0; k < index.size(); ++k ) {
	if ( k % 10000 == 0 ) cerr << "x";
	_sample.erase		( _sample.begin()	+ index[ k ] );
	_aveHeight.erase	( _aveHeight.begin()	+ index[ k ] );
	_maxHeight.erase	( _maxHeight.begin()	+ index[ k ] );
	_minHeight.erase	( _minHeight.begin()	+ index[ k ] );
	_codeHeight.erase	( _codeHeight.begin()	+ index[ k ] );
	_maxGrad.erase		( _maxGrad.begin()	+ index[ k ] );
	_maxDir.erase		( _maxDir.begin()	+ index[ k ] );
	_minGrad.erase		( _minGrad.begin()	+ index[ k ] );
	_minDir.erase		( _minDir.begin()	+ index[ k ] );
	_aveGrad.erase		( _aveGrad.begin()	+ index[ k ] );
    }
    cerr << endl;
#endif	// SKIP
}
#endif	// SKIP



//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	コンストラクタ
//	Constructors
//------------------------------------------------------------------------------

//
//  LandPlot::LandPlot --	default constructor
//			コンストラクタ(デフォルト)
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandPlot::LandPlot()
{
    _init();
    _reset();
    // _geo( MAP_TEXTURE_FILENAME );
}


//
//  LandPlot::LandPlot --	copy constructor
//			コピー・コンストラクタ
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
LandPlot::LandPlot( const LandPlot & obj )
{
    _init();
    _reset();
    if ( this != &obj ) {
	_sample		= obj._sample;
	_meshcode	= obj._meshcode;
	_landtype	= obj._landtype;
    }
}


//------------------------------------------------------------------------------
//	デストラクタ
//	Destructor
//------------------------------------------------------------------------------

//
//  LandPlot::~LandPlot --	destructor
//			デストラクタ
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandPlot::~LandPlot()
{
    _reset();
}


//------------------------------------------------------------------------------
//	メンバ参照
//	Referrring to members
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Misc. functions
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//	Static functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	ソートのための関数
//	Functions for sorting
//------------------------------------------------------------------------------

#ifdef SKIP
//
//  compare --	comparison function for sorting
//		ソートのための比較関数
//
//  Inputs
//	a, b	: pointers to objects of this class
//		  このクラスへのオブジェクトへのポインタ
//
//  Outputs
//	a <  b	-1
//	a == b	 0
//	a >  b	 1
//
int compare ( const LandPlot * a, const LandPlot * b )
{
    int (*cf)( const Graph *, const Graph * );
    cf = compare;
    return ( (*cf)( &(a->Graph), &(b->Graph) ) );

    return compare( ( const Graph * )a, ( const Graph * )b );
}
#endif	// SKIP


//------------------------------------------------------------------------------
//	等号
//	equality
//------------------------------------------------------------------------------

#ifdef SKIP
//
//  operator == --	equality
//			等号
//
//  Inputs
//	a, b	: objects of this class
//
//  Outputs
//	boolean value
//
int operator == ( const LandPlot & a, const LandPlot & b )
{

    return ( ( const Graph & )a == ( const Graph & )b );
}
#endif	// SKIP


//------------------------------------------------------------------------------
//	代入演算子
//	Assignment opereators
//------------------------------------------------------------------------------

//
//  LandPlot::operator = --	assignement
//				代入
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
LandPlot & LandPlot::operator = ( const LandPlot & obj )
{
    if ( this != &obj ) {
	_sample		= obj._sample;
	_meshcode	= obj._meshcode;
	_landtype	= obj._landtype;
    }
    return *this;
}


//------------------------------------------------------------------------------
//	入出力
//	I/O functions
//------------------------------------------------------------------------------

//
//  operator << --	output
//
//  Argument
//	stream	: output stream
//	obj	: object of this class
//
//  Outputs
//	output stream
//
ostream & operator << ( ostream & stream, const LandPlot & obj )
{
    return stream;
}


//
//  operator >> --	input
//
//  Inputs
//	stream	: input stream
//	obj	: object of this class
//
//  Outputs
//	input stream
//
istream & operator >> ( istream & stream, LandPlot & obj )
{
    return stream;
}


// end of header file
// Do not add any stuff under this line.
