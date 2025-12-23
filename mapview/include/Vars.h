//------------------------------------------------------------------------------
//	Macro switch for variables
//------------------------------------------------------------------------------
#define USE_DISTANCES
#define USE_AREAMAP

#ifndef USE_DISTANCES
#define USE_EXPONENTS
#endif

#define DISCARD_FIRST_TITLE_LINE

//------------------------------------------------------------------------------
// #define NIIDA_COMBINED_RANGES_FOR_P_AND_Q
//------------------------------------------------------------------------------

// [[[Select one of them]]]
//#define NORMAL_DOSE_MAP
#define RATIO_COMPARISON


//------------------------------------------------------------------------------
//	Macro variables
//------------------------------------------------------------------------------
#define FIXED_ATTR_SIZE			(5)

//++++++++++++++++++++++++++++++
#ifdef NORMAL_DOSE_MAP
//#define VARIABLE_ATTR_SIZE		(5)	// 4 + 1
#define VARIABLE_ATTR_SIZE		(7)
//******************************
//#define WITH_STANDARD_DEVIATION
// #define WITH_LAND_TYPE
#define WITH_REDUCTION_RATIO
//******************************
#endif	// NORMAL_DOSE_MAP
//++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++
#ifdef RATIO_COMPARISON
#define VARIABLE_ATTR_SIZE		(7)	// 4 + 1
//******************************
#define WITH_TWO_RATIOS
//******************************
#endif	// RATIO_COMPARISON
//++++++++++++++++++++++++++++++

#define NUM_VALUES			(1)	// for the size of rasterization buffers

#define NUM_YEARS			(10)
#define NUM_TUPLES			(4) // quartet

#define SMALLEST_RADIUS			(1.0)
//#define LARGEST_RADIUS		(24.0)
#define LARGEST_RADIUS			(36.0)
//#define RADIUS_SCALE			(1.6)
//#define RADIUS_SCALE			(0.8) // for presentation
#define RADIUS_SCALE			(0.4) // for rendering
//#define RADIUS_SCALE			(0.2) // for fine rendering

#ifndef LIMIT_NUM_COMPS
//#define LIMIT_NUM_COMPS		(512.0)
//#define LIMIT_NUM_COMPS		(256.0)
//#define LIMIT_NUM_COMPS		(128.0)
#define LIMIT_NUM_COMPS		(64.0)
#endif	// LIMIT_NUM_COMPS

#define BASE_INTENSITY		(0.1)
//#define BASE_INTENSITY		(0.0) // <===
// #define CONTRAST_INTENSITY	(0.7)
#define CONTRAST_INTENSITY	(0.4) // <===
#define FULL_INTENSITY		(1.0)
#define MODERATE_INTENSITY	(0.9)
// #define MODERATE_INTENSITY	(0.8)
// #define MODERATE_INTENSITY	(0.6) // <===
#define AUXILIARY_INTENSITY	(0.2)

#define DOWNSAMPLE_FACTOR	(25)
//#define DOWNSAMPLE_FACTOR	(12)
//#define DOWNSAMPLE_FACTOR	(8) // <===
//#define DOWNSAMPLE_FACTOR	(4)


//------------------------------------------------------------------------------
//	Scale for attribute values
//------------------------------------------------------------------------------
//#define INTERVAL_DOSE		interval< double >( 0.0, log10(20.0) )
//#define INTERVAL_DOSE		interval< double >( 0.0, log10(10.0) )

#define INTERVAL_DOSE		interval< double >( -2.0, 1.602 )

//#define INTERVAL_PRI_AVE	interval< double >( -2.0, 1.602 )
//#define INTERVAL_POS_AVE	interval< double >( -2.0, 1.602 )
//#define INTERVAL_PRI_SDV	interval< double >( -2.0, 1.602 )
//#define INTERVAL_POS_SDV	interval< double >( -2.0, 1.602 )

// minPriAve = -1.39901 maxPriAve = 1.34771
// minPosAve = -1.39882 maxPosAve = 1.34628
// minPriSdv = 0.077599 maxPriSdv = 0.435364
// minPosSdv = 0.077373 maxPosSdv = 0.360502
#define INTERVAL_PRI_AVE	interval< double >( -1.40, 1.40 )
#define INTERVAL_POS_AVE	interval< double >( -1.40, 1.40 )
#define INTERVAL_PRI_SDV	interval< double >( 0.077, 0.436 )
#define INTERVAL_POS_SDV	interval< double >( 0.077, 0.361 )
// minPriAve = -1.59817 maxPriAve = 1.42336
// minPosAve = -1.85013 maxPosAve = 1.49732
// minPriSdv = 0.041025 maxPriSdv = 0.621334
// minPosSdv = 0.041 maxPosSdv = 0.397978
// minRatio = -1.9346 maxRatio = 0.918284
#define INTERVAL_RATIO			interval< double >( -1.0, 1.0 )

#define INTERVAL_HEIGHT		interval< double >( 0.0, 10.0 )
//#define INTERVAL_HEIGHT		interval< double >( 0.0, 400.0 )
//#define INTERVAL_HEIGHT		interval< double >( 0.0, 800.0 )
// #define INTERVAL_HEIGHT		interval< double >( 0.0, 1200.0 )
//#define INTERVAL_HEIGHT		interval< double >( 0.0, 2000.0 )	 // <==

// Maximum distance = 264.130315
// #define INTERVAL_DISTANCE		interval< double >( 0.0, 280.0 )	// <==
// #define INTERVAL_DISTANCE		interval< double >( 0.0, 160.0 )	
// #define INTERVAL_DISTANCE		interval< double >( 0.0, 80.0 )	
// #define INTERVAL_DISTANCE		interval< double >( 0.0, 50.0 )	
#define INTERVAL_DISTANCE		interval< double >( 0.0, 10.0 )	

//#define INTERVAL_GRADIENT	interval< double >( 0.0, 800.0 )	
#define INTERVAL_GRADIENT	interval< double >( 0.0, 2000.0 )	

#define INTERVAL_EXPONENT	interval< double >( -0.0015, 0.0001 )	

//------------------------------------------------------------------------------
//	Radius for paint by mouse
//------------------------------------------------------------------------------
#define PAINT_RADIUS	(0.0)
#define MAP_RADIUS	(1.0)
//#define MAP_RADIUS	(2.0)

//------------------------------------------------------------------------------
//	COLOR INTENSITY RATIO
//------------------------------------------------------------------------------
// #define COLOR_INTENSITY_RATIO	(1.0)
// #define COLOR_INTENSITY_RATIO	(0.5)
// #define COLOR_INTENSITY_RATIO	(0.1)
#define COLOR_INTENSITY_RATIO	(0.01)


//------------------------------------------------------------------------------
//	
//------------------------------------------------------------------------------
#define FUKUSHIMA_DAIICHI_LAT	(37.423056)
#define FUKUSHIMA_DAIICHI_LON	(141.033056)

#ifdef OLD
#define BESSEL_A		(6377397.155)
#define BESSEL_E2		(0.00667436061028297)
#define BESSEL_MNUM		(6334832.10663254)
#endif	// OLD

#define BESSEL_LONG_R		(6377397.155)
#define BESSEL_SHORT_R		(6356079.000)
#define BESSEL_E1		(0.00667436061028297384)
#define BESSEL_A		(BESSEL_LONG_R)
#define BESSEL_E2		(0.00004454708955609691)
#define BESSEL_MNUM		(6377113.06051780141735326170)
