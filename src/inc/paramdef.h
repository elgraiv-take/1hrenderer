/**
 * @file paramdef.h
 *
 * @author take
 */

#ifndef PARAMDEF_H_
#define PARAMDEF_H_


//#define PATHTRACE_SUMPLE_NUM (100)
//#define PATHTRACE_SUMPLE_NUM (1000)
#define PATHTRACE_SUMPLE_NUM (500)

//#define PATHTRACE_DEPTH_NUM (10)
#define PATHTRACE_DEPTH_NUM (10)

#define PPM_PHOTON_NUM (1000000000)
//#define PPM_PHOTON_NUM (10000000)
//#define PPM_PHOTON_NUM (1000000)
//#define PPM_PHOTON_NUM (10000)
#define MAP_SIZE (500000)
#define APPLY_SIZE (5000)

//#define PPM_DEFAULT_RADIUS (0.5f)
#define PPM_DEFAULT_RADIUS (0.2f)
#define PPM_ALPHA (0.7f)
#define PPM_MAX_PHOTON_DEPTH (10)
#define PPM_MAX_RAY_DEPTH (10)
#define PPM_FINAL_GATHER_NUM (100)

#define OUTPUT_TIME_STEP (1000*60)
//#define OUTPUT_TIME_STEP (1000*10)
//#define OUTPUT_TIME_STEP (1000*20)

#define DEAD_LINE (1000*60*60)
//#define DEAD_LINE (1000*60)
//#define DEAD_LINE (1000*60)
//#define DEAD_LINE (1000*30)

#define SAFETY_MARGIN (1000)
//#define SAFETY_MARGIN (2000)

//#define SPPM_RAY_SAMPLE (500)
#define SPPM_RAY_SAMPLE (100)
#define SPPM_RAY_RADIUS (0.8f)

#endif /* PARAMDEF_H_ */
