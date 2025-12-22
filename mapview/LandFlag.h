//==============================================================================
// LandFlag.h
//	: header file for flags of respective land use types
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2021-05-13 16:00:10 shigeo"
//
//==============================================================================

#ifndef _LandFlag_H		// begining of header file
#define _LandFlag_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------

#include <iostream>
using namespace std;

#include "LandType.h"


//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//	Defining Classes
//------------------------------------------------------------------------------

class LandFlag {

  private:

  protected:

    bool	_riceFlag;
    bool	_farmFlag;
    bool	_forestFlag;
    bool	_wasteFlag;
    bool	_urbanFlag;
    bool	_roadFlag;
    bool	_railwayFlag;
    bool	_otherFlag;
    bool	_marshFlag;
    bool	_beachFlag;
    bool	_oceanFlag;
    bool	_golfFlag;
    
  public:

//------------------------------------------------------------------------------
//	Constructors
//------------------------------------------------------------------------------
    LandFlag();			// default constructor
    LandFlag( const LandFlag & obj );
				// copy constructor

//------------------------------------------------------------------------------
//	Destructor
//------------------------------------------------------------------------------
    ~LandFlag();			// destructor

//------------------------------------------------------------------------------
//	Referrring to members
//------------------------------------------------------------------------------
    void	setByCode	( const int code );
    void	clearByCode	( const int code );
    void	setByType	( const LandType ltype );
    void	clearByType	( const LandType ltype );

    bool	passByCode	( const int code );
    bool	passByType	( const LandType ltype );

//------------------------------------------------------------------------------
//	Assignment opereators
//------------------------------------------------------------------------------
    LandFlag & operator = ( const LandFlag & obj );
				// assignment

//------------------------------------------------------------------------------
//	I/O functions
//------------------------------------------------------------------------------
    friend ostream & operator << ( ostream & stream, const LandFlag & obj );
				// output
    friend istream & operator >> ( istream & stream, LandFlag & obj );
				// input

//------------------------------------------------------------------------------
//	Class name
//------------------------------------------------------------------------------
    virtual const char * className( void ) const { return "LandFlag"; }
				// class name
};



#endif // _LandFlag_H

// end of header file
// Do not add any stuff under this line.
