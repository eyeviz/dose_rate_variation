//==============================================================================
// LandFlag.cpp
//	: program file for flags of respective land use types
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2021-05-16 00:31:37 shigeo"
//
//==============================================================================

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include "LandFlag.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Private Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Protected Functions
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Public Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------

//
//  LandFlag::LandFlag --	default constructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandFlag::LandFlag() 
{
    _riceFlag		= true;
    _farmFlag		= true;
    _forestFlag		= true;
    _wasteFlag		= true;
    _urbanFlag		= true;
    _roadFlag		= true;
    _railwayFlag	= true;
    _otherFlag		= true;
    _marshFlag		= true;
    _beachFlag		= true;
    _oceanFlag		= true;
    _golfFlag		= true;
}


//
//  LandFlag::LandFlag --	copy constructor
//
//  Inputs
//	obj	: object of this class
//
//  Outputs
//	none
//
LandFlag::LandFlag( const LandFlag & obj )
{
    _riceFlag		= obj._riceFlag;
    _farmFlag		= obj._farmFlag;
    _forestFlag		= obj._forestFlag;
    _wasteFlag		= obj._wasteFlag;
    _urbanFlag		= obj._urbanFlag;
    _roadFlag		= obj._roadFlag;
    _railwayFlag	= obj._railwayFlag;
    _otherFlag		= obj._otherFlag;
    _marshFlag		= obj._marshFlag;
    _beachFlag		= obj._beachFlag;
    _oceanFlag		= obj._oceanFlag;
    _golfFlag		= obj._golfFlag;
}


//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------

//
//  LandFlag::~LandFlag --	destructor
//
//  Inputs
//	none
//
//  Outputs
//	none
//
LandFlag::~LandFlag()
{

}


//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------

//
//  LandFlag::setByCode --	set the flag by the code
//
//  Inputs
//	code : code for setting the specific flag
//
//  Outputs
//	none
//
void LandFlag::setByCode( const int code )
{
    switch ( code ) {
      case RICE_CODE:
	  _riceFlag = true;
	  break;
      case FARM_CODE:
	  _farmFlag = true;
	  break;
      case FOREST_CODE:
	  _forestFlag = true;
	  break;
      case WASTE_CODE:
	  _wasteFlag = true;
	  break;
      case URBAN_CODE:
	  _urbanFlag = true;
	  break;
      case ROAD_CODE:
	  _roadFlag = true;
	  break;
      case RAILWAY_CODE:
	  _railwayFlag = true;
	  break;
      case OTHER_CODE:
	  _otherFlag = true;
	  break;
      case MARSH_CODE:
	  _marshFlag = true;
	  break;
      case BEACH_CODE:
	  _beachFlag = true;
	  break;
      case OCEAN_CODE:
	  _oceanFlag = true;
	  break;
      case GOLF_CODE:
	  _golfFlag = true;
	  break;
    }
}


//
//  LandFlag::clearByCode --	clear the flag by the code
//
//  Inputs
//	code : code for clearing the specific flag
//
//  Outputs
//	none
//
void LandFlag::clearByCode( const int code )
{
    switch ( code ) {
      case RICE_CODE:
	  _riceFlag = false;
	  break;
      case FARM_CODE:
	  _farmFlag = false;
	  break;
      case FOREST_CODE:
	  _forestFlag = false;
	  break;
      case WASTE_CODE:
	  _wasteFlag = false;
	  break;
      case URBAN_CODE:
	  _urbanFlag = false;
	  break;
      case ROAD_CODE:
	  _roadFlag = false;
	  break;
      case RAILWAY_CODE:
	  _railwayFlag = false;
	  break;
      case OTHER_CODE:
	  _otherFlag = false;
	  break;
      case MARSH_CODE:
	  _marshFlag = false;
	  break;
      case BEACH_CODE:
	  _beachFlag = false;
	  break;
      case OCEAN_CODE:
	  _oceanFlag = false;
	  break;
      case GOLF_CODE:
	  _golfFlag = false;
	  break;
    }
}


//
//  LandFlag::setByType --	set the flag by the land use type
//
//  Inputs
//	type : land use type for setting the specific flag
//
//  Outputs
//	none
//
void LandFlag::setByType( const LandType ltype )
{
    switch ( ltype ) {
      case RICE:
	  _riceFlag = true;
	  break;
      case FARM:
	  _farmFlag = true;
	  break;
      case FOREST:
	  _forestFlag = true;
	  break;
      case WASTE:
	  _wasteFlag = true;
	  break;
      case URBAN:
	  _urbanFlag = true;
	  break;
      case ROAD:
	  _roadFlag = true;
	  break;
      case RAILWAY:
	  _railwayFlag = true;
	  break;
      case OTHER:
	  _otherFlag = true;
	  break;
      case MARSH:
	  _marshFlag = true;
	  break;
      case BEACH:
	  _beachFlag = true;
	  break;
      case OCEAN:
	  _oceanFlag = true;
	  break;
      case GOLF:
	  _golfFlag = true;
	  break;
      case OUT:
      default:	  
	  break;
    }
}


//
//  LandFlag::clearByType --	clear the flag by the land use type
//
//  Inputs
//	ltype : land use type for clearing the specific flag
//
//  Outputs
//	none
//
void LandFlag::clearByType( const LandType ltype )
{
    switch ( ltype ) {
      case RICE:
	  _riceFlag = false;
	  break;
      case FARM:
	  _farmFlag = false;
	  break;
      case FOREST:
	  _forestFlag = false;
	  break;
      case WASTE:
	  _wasteFlag = false;
	  break;
      case URBAN:
	  _urbanFlag = false;
	  break;
      case ROAD:
	  _roadFlag = false;
	  break;
      case RAILWAY:
	  _railwayFlag = false;
	  break;
      case OTHER:
	  _otherFlag = false;
	  break;
      case MARSH:
	  _marshFlag = false;
	  break;
      case BEACH:
	  _beachFlag = false;
	  break;
      case OCEAN:
	  _oceanFlag = false;
	  break;
      case GOLF:
	  _golfFlag = false;
	  break;
      case OUT:
      default:
	  break;
    }
}


//
//  LandFlag::passByCode --	check if the input code is acceptable or not
//
//  Inputs
//	code : code for checking the acceptability
//
//  Outputs
//	boolean value
//
bool LandFlag::passByCode( const int code )
{
    bool flag = true;
    switch ( code ) {
      case RICE_CODE:
	  flag = _riceFlag;
	  break;
      case FARM_CODE:
	  flag = _farmFlag;
	  break;
      case FOREST_CODE:
	  flag = _forestFlag;
	  break;
      case WASTE_CODE:
	  flag = _wasteFlag;
	  break;
      case URBAN_CODE:
	  flag = _urbanFlag;
	  break;
      case ROAD_CODE:
	  flag = _roadFlag;
	  break;
      case RAILWAY_CODE:
	  flag = _railwayFlag;
	  break;
      case OTHER_CODE:
	  flag = _otherFlag;
	  break;
      case MARSH_CODE:
	  flag = _marshFlag;
	  break;
      case BEACH_CODE:
	  flag = _beachFlag;
	  break;
      case OCEAN_CODE:
	  flag = _oceanFlag;
	  break;
      case GOLF_CODE:
	  flag = _golfFlag;
	  break;
    }
    return flag;
}


//
//  LandFlag::passByType --	check if the input land use type is acceptable
//				or not
//
//  Inputs
//	ltype : land use type for clearing the specific flag
//
//  Outputs
//	boolean value
//
bool LandFlag::passByType( const LandType ltype )
{
    bool flag = true;
    switch ( ltype ) {
      case RICE:
	  flag = _riceFlag;
	  break;
      case FARM:
	  flag = _farmFlag;
	  break;
      case FOREST:
	  flag = _forestFlag;
	  break;
      case WASTE:
	  flag = _wasteFlag;
	  break;
      case URBAN:
	  flag = _urbanFlag;
	  break;
      case ROAD:
	  flag = _roadFlag;
	  break;
      case RAILWAY:
	  flag = _railwayFlag;
	  break;
      case OTHER:
	  flag = _otherFlag;
	  break;
      case MARSH:
	  flag = _marshFlag;
	  break;
      case BEACH:
	  flag = _beachFlag;
	  break;
      case OCEAN:
	  flag = _oceanFlag;
	  break;
      case GOLF:
	  flag = _golfFlag;
	  break;
      case OUT:
      default:
	  break;
    }
    return flag;
}



//------------------------------------------------------------------------------
//	Assignment opereators
//------------------------------------------------------------------------------

//
//  LandFlag::operator = --	assignement
//
//  Inputs
//	obj	: objects of this class
//
//  Outputs
//	this object
//
LandFlag & LandFlag::operator = ( const LandFlag & obj )
{
    if ( this != &obj ) {
	_riceFlag		= obj._riceFlag;
	_farmFlag		= obj._farmFlag;
	_forestFlag		= obj._forestFlag;
	_wasteFlag		= obj._wasteFlag;
	_urbanFlag		= obj._urbanFlag;
	_roadFlag		= obj._roadFlag;
	_railwayFlag		= obj._railwayFlag;
	_otherFlag		= obj._otherFlag;
	_marshFlag		= obj._marshFlag;
	_beachFlag		= obj._beachFlag;
	_oceanFlag		= obj._oceanFlag;
	_golfFlag		= obj._golfFlag;
    }
    return *this;
}


//------------------------------------------------------------------------------
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
ostream & operator << ( ostream & stream, const LandFlag & obj )
{
    stream << endl;

    stream << "Rice     ( ";
    if ( obj._riceFlag ) stream << " on";
    else stream << "off";
    stream << " ) | Farm     ( ";
    if ( obj._farmFlag ) stream << " on";
    else stream << "off";
    stream << " )" << endl;

    stream << "Forest   ( ";
    if ( obj._forestFlag ) stream << " on";
    else stream << "off";
    stream << " ) | Waste    ( ";
    if ( obj._wasteFlag ) stream << " on";
    else stream << "off";
    stream << " )" << endl;

    stream << "Urban    ( ";
    if ( obj._urbanFlag ) stream << " on";
    else stream << "off";
    stream << " ) | Road     ( ";
    if ( obj._roadFlag ) stream << " on";
    else stream << "off";
    stream << " )" << endl;

    stream << "Railway  ( ";
    if ( obj._railwayFlag ) stream << " on";
    else stream << "off";
    stream << " ) | Marsh    ( ";
    if ( obj._marshFlag ) stream << " on";
    else stream << "off";
    stream << " )" << endl;


    stream << "Beach    ( ";
    if ( obj._beachFlag ) stream << " on";
    else stream << "off";
    stream << " ) | Ocean    ( ";
    if ( obj._oceanFlag ) stream << " on";
    else stream << "off";
    stream << " )" << endl;

    stream << "Golf     ( ";
    if ( obj._golfFlag ) stream << " on";
    else stream << "off";
    stream << " ) | Other    ( ";
    if ( obj._otherFlag ) stream << " on";
    else stream << "off";
    stream << " )" << endl;
    
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
istream & operator >> ( istream & stream, LandFlag & obj )
{
    // stream >> ( %%% & )obj;
    return stream;
}


// end of header file
// Do not add any stuff under this line.
