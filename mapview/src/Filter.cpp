#include <fstream>
#include <sstream>
using namespace std;

#include "Common.h"
#include "Vars.h"
#include "CSVIO.h"
#include "Filter.h"
#include <boost/numeric/ublas/matrix_proxy.hpp>
using namespace boost::numeric::ublas;

// #define DEBUG_FOR_TRACKING_FEATURE_VALUES

//------------------------------------------------------------------------------
//	Data handling
//------------------------------------------------------------------------------
void loadExpData( const char * filename,
		  std::vector< string > & title,
		  ublas::matrix< double > & entire )
{
    ifstream ifs( filename );
    string strbuf;
    if ( ! ifs.is_open() ) {
	cerr << "cannot open the file: " << filename << endl;
	entire.clear();
	return;
    }

    unsigned int counter = 0;
    std::vector< ublas::vector< double > > databuf;

    // Discard the first line
    getline( ifs, strbuf );
    title = CSVIO::split( strbuf, ',' );
    // cerr << HERE << " strbuf = " << strbuf << endl;
    
    unsigned int nCols = 0;

    // We expect the data is formatted as
    // Latitude, Longitude, Height, Exponent, Dose[ 0 ], Dose[ 1 ], .....
    while ( getline( ifs, strbuf ) ) {
	// if ( counter % (50000*DOWNSAMPLE_FACTOR) == 0 )
	// cerr << HERE << " counter = " << counter << endl;
	// cerr << HERE << " counter = " << counter << " : strbuf = " << strbuf << endl;
	if ( counter % 50000 == 0 )
	    cerr << HERE << " counter = " << counter << endl;
	//if ( counter % DOWNSAMPLE_FACTOR != 0 ) continue;
	
#ifdef SKIP
//------------------------------------------------------------------------------
	if ( counter >= 50000 ) {
	    cerr << HERE << " Cut out data at counter == " << counter << endl;
	    break;
	}
//------------------------------------------------------------------------------
#endif	// SKIP
	std::vector< double > val;

	ublas::vector< double, std::vector< double > > oneline;

	CSVIO::csvline( strbuf, val );

	if ( nCols < val.size() ) nCols = val.size();
	
	double x = val[ 0 ];
	double y = val[ 1 ];
	double height = val[ 2 ];
	double distance = val[ 3 ];
	double gradient = val[ 4 ];
	
	oneline.data().push_back( x );
	oneline.data().push_back( y );
	oneline.data().push_back( height );
	oneline.data().push_back( distance );
	oneline.data().push_back( gradient );
	
	for ( unsigned int i = 5; i < val.size(); ++i ) {
	    double each = val[ i ];
	    oneline.data().push_back( each );
	}
	
	databuf.push_back( oneline );

	// ***** counter incremented *****
	counter++;
	// *******************************
    }
    ifs.close();

    cerr << HERE << " Number of columns = " << nCols << endl;

    entire.resize( databuf.size(), nCols );
    for ( unsigned int k = 0; k < entire.size1(); ++k ) {
	ublas::row( entire, k ) = databuf[ k ];
#ifdef SKIP
	for ( unsigned int m = 0; m < entire.size2(); ++m ) {
	    cerr << " [ " << m << " ] " << entire( k, m ) << " vs. "
		 << databuf[ k ][ m ] << ": ";
	}
	cerr << endl;
#endif	// SKIP
    }

    return;
}


void saveBinaryData( const char * filename,
		     const std::vector< string > & title,
		     const ublas::matrix< double > & entire )
{
    ofstream ofs( filename, ios::out|ios::binary|ios::trunc );
    if ( ! ofs.is_open() ) {
	cerr << "cannot open the file: " << filename << endl;
	return;
    }

    //------------------------------------------------------------------------------
    //	titles
    //------------------------------------------------------------------------------
    unsigned int size = title.size();
    ofs.write( ( char * )&size, sizeof( unsigned int ) );
    for ( unsigned int i = 0; i < title.size(); ++i ) {
	size_t len = title[ i ].length() + 1;
	char * cstr = new char [ len ];
	std::strcpy( cstr, title[ i ].c_str() );
	ofs.write( (char*)&len, sizeof( size_t ) );
	ofs.write( (char*)cstr, sizeof( char )*len );
	delete [] cstr;
    }

    //------------------------------------------------------------------------------
    //	data entries
    //------------------------------------------------------------------------------
    unsigned int size1 = entire.size1();
    unsigned int size2 = entire.size2();
    
    ofs.write( ( char * )&size1, sizeof( unsigned int ) );
    ofs.write( ( char * )&size2, sizeof( unsigned int ) );
    for ( unsigned int i = 0; i < entire.size1(); ++i ) {
	if ( i % 100000 == 0 )
	    cerr << HERE << " counter = " << i << endl;
	for ( unsigned int j = 0; j < entire.size2(); ++j ) {
	    double val = entire( i, j );
	    ofs.write( ( char *)&val, sizeof( double ) );
	}
    }

    ofs.close();

    return;
}


void loadBinaryData( const char * filename,
		     std::vector< string > & title, ublas::matrix< double > & entire )
{
    ifstream ifs( filename, ios::in | ios::binary );
    if ( ! ifs.is_open() ) {
	cerr << "cannot open the file: " << filename << endl;
	return;
    }

    //------------------------------------------------------------------------------
    //	titles
    //------------------------------------------------------------------------------
    unsigned int size;
    ifs.read( ( char * )&size, sizeof( unsigned int ) );
    title.resize( size );
    for ( unsigned int i = 0; i < title.size(); ++i ) {
	size_t len;
	ifs.read( (char*)&len, sizeof( size_t ) );
	char * cstr = new char [ len ];
	ifs.read( (char*)cstr, sizeof( char )*len );
	string newtitle( cstr );
	title[ i ] = newtitle;
    }

    //------------------------------------------------------------------------------
    //	data entries
    //------------------------------------------------------------------------------
    unsigned int size1, size2;
    ifs.read( ( char * )&size1, sizeof( unsigned int ) );
    ifs.read( ( char * )&size2, sizeof( unsigned int ) );
    entire.resize( size1, size2 );

    for ( unsigned int i = 0; i < entire.size1(); ++i ) {
	if ( i % 100000 == 0 )
	    cerr << HERE << " counter = " << i << endl;
//------------------------------------------------------------------------------
#ifdef DEBUG_FOR_TRACKING_FEATURE_VALUES
	const double targetX = 140.547040;
	const double targetY = 37.851180;
	const double eps = 0.0001;
	bool isOn = false;
#endif	// DEBUG_FOR_TRACKING_FEATURE_VALUES
//------------------------------------------------------------------------------

	for ( unsigned int j = 0; j < entire.size2(); ++j ) {

	    double val;
	    ifs.read( ( char * )&val, sizeof( double ) );
	    entire( i, j ) = val;

//------------------------------------------------------------------------------
#ifdef DEBUG_FOR_TRACKING_FEATURE_VALUES
	    if ( ( j == 0 ) && ( abs( val - targetX ) < eps ) ) {
		cerr << HERE << " j = " << setw(3) << j
		     << " Longitude : " << fixed << setprecision( 8 )
		     << targetX << " is found "
		     << fixed << setprecision( 8 ) << val << endl;
		isOn = true;
		// getchar();
	    }
//------------------------------------------------------------------------------
	    if ( ( isOn ) && ( j == 1 ) ) {
		if ( abs( val - targetY ) < eps ) {
		    cerr << HERE << " j = " << setw(3) << j
			 << " Latitude : "  << fixed << setprecision( 8 )
			 << targetY << " is found => "
			 << fixed << setprecision( 8 ) << val << endl;
		    getchar();
		}
		else {
		    isOn = false;
		    cerr << HERE << " j = " << setw(3) << j
			 << " Latitude : " << fixed << setprecision( 8 )
			 << targetY << " is NOT found = > "
			 << fixed << setprecision( 8 ) << val << endl;
		    // getchar();
		}
	    }
//------------------------------------------------------------------------------
	    if ( ( isOn ) && ( j > 1 ) ) {
		cerr << HERE << "[ " << setw(3) << j << " ] = "
		     << val << endl;
		getchar();
	    }
#endif	// DEBUG_FOR_TRACKING_FEATURE_VALUES
//------------------------------------------------------------------------------
	}
    }

    ifs.close();

    return;
}


void downSample( ublas::matrix< double > & entire,
		 ublas::matrix< double > & region,
		 unsigned int stepSize )
{
    unsigned int newSize = ceil( ( double )entire.size1() / ( double )stepSize );
    region.resize( newSize, entire.size2() );
    cerr << " region size = " << region.size1() << " x " << region.size2() << ends;

    for ( unsigned int k = 0; k < entire.size1(); k += stepSize ) {
	unsigned int id = k / stepSize;
#ifdef DEBUG
	cerr << " k = " << k << " / " << entire.size1() << " id = " << id
	     << " newSize = " << newSize << endl;
#endif	// DEBUG
	ublas::row( region, id ) = ublas::row( entire, k );

//------------------------------------------------------------------------------
#ifdef DEBUG_FOR_TRACKING_FEATURE_VALUES
	ublas::vector< double > val = ublas::row( entire, k );
	
	const double targetX = 140.547040;
	const double targetY = 37.851180;
	const double eps = 0.0001;

	if ( ( abs( val[ 0 ] - targetX ) < eps ) &&
	     ( abs( val[ 1 ] - targetY ) < eps ) ) {
	    cerr << HERE 
		 << " Longitude : " << fixed << setprecision( 8 )
		 << targetX << " is found "
		 << fixed << setprecision( 8 ) << val[ 0 ] << endl;
	    cerr << HERE
		 << " Latitude : "  << fixed << setprecision( 8 )
		 << targetY << " is found => "
		 << fixed << setprecision( 8 ) << val[ 1 ] << endl;
	    for ( unsigned int i = 2; i < val.size(); ++i ) {
		cerr << HERE << "[ " << setw(3) << i << " ] = "
		     << val[ i ] << endl;
		getchar();
	    }
	}
#endif	// DEBUG_FOR_TRACKING_FEATURE_VALUES
    }
}


