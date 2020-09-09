#ifndef GPS_H_
#define GPS_H_

#include <stdbool.h>

#define GPS_NO_STATS

#define GPRMC_TERM   "GPRMC"
#define GPGGA_TERM   "GPGGA"

#define GPS_INVALID_AGE         0xFFFFFFFF
#define GPS_INVALID_ANGLE       999999999
#define GPS_INVALID_ALTITUDE    999999999
#define GPS_INVALID_DATE        0
#define GPS_INVALID_TIME        0xFFFFFFFF
#define GPS_INVALID_SPEED       999999999
#define GPS_INVALID_FIX_TIME    0xFFFFFFFF
#define GPS_INVALID_SATELLITES  0xFF
#define GPS_INVALID_HDOP        0xFFFFFFFF

#define GPS_SENTENCE_GPGGA      0
#define GPS_SENTENCE_GPRMC      1
#define GPS_SENTENCE_OTHER      2

#define GPS_MPH_PER_KNOT       1.15077945
#define GPS_MPS_PER_KNOT       0.51444444
#define GPS_KMPH_PER_KNOT      1.852
#define GPS_MILES_PER_METER    0.00062137112
#define GPS_KM_PER_METER       0.001

void GPS_init(void);
void GPS_read(void);

#endif /* GPS_H_ */
