//==============================================================================
// LandType.h
//	: header file for the land use types
//
//------------------------------------------------------------------------------
//
//				Time-stamp: "2021-05-13 14:09:42 shigeo"
//
//==============================================================================

#ifndef _LandType_H		// begining of header file
#define _LandType_H		// notifying that this file is included

//------------------------------------------------------------------------------
//	Including Header Files
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//	Defining Macros
//------------------------------------------------------------------------------

#define NUM_USES	(12)

typedef enum {
    RICE,			// 0100: rice field
    FARM,			// 0200: other farmland
    FOREST,			// 0500: forest
    WASTE,			// 0600: wasteland
    URBAN,			// 0700: building sites
    ROAD,			// 0901: roads
    RAILWAY,			// 0902: railways
    OTHER,			// 1000: other sites
    MARSH,			// 1100: river and lakes
    BEACH,			// 1400: seaside
    OCEAN,			// 1500: ocean
    GOLF,			// 1600: golf courses
    OUT
} LandType;


#define RICE_CODE	(100)
#define FARM_CODE	(200)
#define FOREST_CODE	(500)
#define WASTE_CODE	(600)

#define URBAN_CODE	(700)
#define ROAD_CODE	(901)
#define RAILWAY_CODE	(902)
#define OTHER_CODE	(1000)

#define MARSH_CODE	(1100)
#define BEACH_CODE	(1400)
#define OCEAN_CODE	(1500)
#define GOLF_CODE	(1600)


#endif // _LandType_H

// end of header file
// Do not add any stuff under this line.
