#include "GPS.h"
#include "peripheral/uart_dev.h"

#include <math.h>
#include <stdlib.h>

#define GPS_INVALID_F_ANGLE     1000.0
#define GPS_INVALID_F_ALTITUDE  1000000.0
#define GPS_INVALID_F_SPEED     -1.0

#define COMBINE(sentence_type, term_number) (((unsigned)(sentence_type) << 5) | term_number)

//Properties
unsigned long time_gps, new_time;
unsigned long date, new_date;
long latitude, new_latitude;
long longitude, new_longitude;
long altitude, new_altitude;
unsigned long speed, new_speed;
unsigned long course, new_course;
unsigned long hdop, new_hdop;
unsigned short numsats, new_numsats;

unsigned long last_time_fix, new_time_fix;
unsigned long last_position_fix, new_position_fix;

//Parsing state variables
uint8_t parity;
bool is_checksum_term;
char term[15];
uint8_t sentence_type;
uint8_t term_number;
uint8_t term_offset;
bool gps_data_good;

//statistics
#ifndef GPS_NO_STATS
    uint32_t encoded_characters = 0;
    uint32_t good_sentences = 0;
    bool failed_checksum = false;
    bool passed_checksum = false;
#endif


int from_hex(char a);
unsigned long parse_decimal(void);
unsigned long parse_degrees(void);
bool gpsisdigit(char c);
long gpsatol(const char *str);
int gpsstrcmp(const char *str1, const char *str2);
bool term_complete(void);
bool GPS_encode(char c);

//Based on tiny GPS library from mikalhart
void GPS_init(void)
{
    uart_dev_init(UART1, 12000000, 9600);

    time_gps = GPS_INVALID_TIME;
    date = GPS_INVALID_DATE;
    latitude = GPS_INVALID_ANGLE;
    longitude = GPS_INVALID_ANGLE;
    altitude = GPS_INVALID_ALTITUDE;
    speed = GPS_INVALID_SPEED;
    course = GPS_INVALID_ANGLE;
    hdop = GPS_INVALID_HDOP;
    numsats = GPS_INVALID_SATELLITES;
    last_time_fix = GPS_INVALID_FIX_TIME;
    last_position_fix = GPS_INVALID_FIX_TIME;
    parity = 0;
    is_checksum_term = false;
    sentence_type = GPS_SENTENCE_OTHER;
    term_number = 0;
    term_offset = 0;
    gps_data_good = false;

    #ifndef GPS_NO_STATS
        encoded_characters = 0;
        good_sentences = 0;
        failed_checksum = 0;
    #endif

    term[0] = '\0';
}

void GPS_read(void)
{
    bool new_data = false;
    uint32_t start = 1200000;

    while(start--)
    {
        char c = uart_dev_get_char(UART1);

        if(GPS_encode(c))
            new_data = true;
    }

    if(new_data)
    {

    }
}

bool GPS_encode(char c)
{
    bool valid_sentence = false;

    #ifndef GPS_NO_STATS
        ++encoded_characters;
    #endif

    switch(c)
    {
    case ',':
        parity ^= c;
    case '\r':
    case '\n':
    case '*':
        if(term_offset < sizeof(term))
        {
            term[term_offset] = 0;
            valid_sentence = term_complete();
        }
        ++term_number;
        term_offset = 0;
        is_checksum_term = c == '*';
        return valid_sentence;

    case '$':
        term_number = term_offset;
        parity = 0;
        sentence_type = GPS_SENTENCE_OTHER;
        is_checksum_term = false;
        gps_data_good = false;
        return valid_sentence;
    }

    if(term_offset < sizeof(term) - 1)
    {
        term[term_offset++] = c;
    }
    if(!is_checksum_term)
    {
        parity ^= c;
    }

    return valid_sentence;
}

#ifndef GPS_NO_STATS
void GPS_stats(unsigned long *chars, unsigned short *sentences, unsigned short *failed_cs)
{
    if(chars) *chars = encoded_characters;
    if (sentences) *sentences = good_sentences;
    if (failed_cs) *failed_cs = failed_checksum;
}
#endif

//Internal utilities
int from_hex(char a)
{
    if (a >= 'A' && a <= 'F')
      return a - 'A' + 10;
    else if (a >= 'a' && a <= 'f')
      return a - 'a' + 10;
    else
      return a - '0';
}

unsigned long parse_decimal(void)
{
    char *p = term;
    bool isneg = *p == '-';
    if(isneg) ++p;

    unsigned long ret = 100UL * gpsatol(p);
    while (gpsisdigit(*p)) ++p;
    if (*p == '.')
    {
      if (gpsisdigit(p[1]))
      {
        ret += 10 * (p[1] - '0');
        if (gpsisdigit(p[2]))
          ret += p[2] - '0';
      }
    }
    return isneg ? -ret : ret;
}

unsigned long parse_degrees(void)
{
    char *p;
    unsigned long left_of_decimal = gpsatol(term);
    unsigned long hundred1000ths_of_minute = (left_of_decimal % 100UL) * 100000UL;
    for (p = term; gpsisdigit(*p); ++p);
    if (*p == '.')
    {
      unsigned long mult = 10000;
      while (gpsisdigit(*++p))
      {
        hundred1000ths_of_minute += mult * (*p - '0');
        mult /= 10;
      }
    }
    return (left_of_decimal / 100) * 1000000 + (hundred1000ths_of_minute + 3) / 6;
}

bool gpsisdigit(char c)
{
    return c >= '0' && c <= '9';
}

long gpsatol(const char *str)
{
    long ret = 0;
    while (gpsisdigit(*str))
      ret = 10 * ret + *str++ - '0';
    return ret;
}

int gpsstrcmp(const char *str1, const char *str2)
{
    while (*str1 && *str1 == *str2)
      ++str1, ++str2;
    return *str1;
}

bool term_complete(void)
{
    if (is_checksum_term)
    {
      uint8_t checksum = 16 * from_hex(term[0]) + from_hex(term[1]);
      if (checksum == parity)
      {
        if (gps_data_good)
        {
          #ifndef GPS_NO_STATS
            ++good_sentences;
          #endif
          last_time_fix = new_time_fix;
          last_position_fix = new_position_fix;

          switch(sentence_type)
          {
          case GPS_SENTENCE_GPRMC:
            time_gps      = new_time;
            date      = new_date;
            latitude  = new_latitude;
            longitude = new_longitude;
            speed     = new_speed;
            course    = new_course;
            break;
          case GPS_SENTENCE_GPGGA:
            altitude  = new_altitude;
            time_gps      = new_time;
            latitude  = new_latitude;
            longitude = new_longitude;
            numsats   = new_numsats;
            hdop      = new_hdop;
            break;
          }

          return true;
        }
      }

      #ifndef GPS_NO_STATS
          else
              ++failed_checksum;
      #endif

      return false;
    }

    // the first term determines the sentence type
    if (term_number == 0)
    {
      if (!gpsstrcmp(term, GPRMC_TERM))
        sentence_type = GPS_SENTENCE_GPRMC;
      else if (!gpsstrcmp(term, GPGGA_TERM))
        sentence_type = GPS_SENTENCE_GPGGA;
      else
        sentence_type = GPS_SENTENCE_OTHER;
      return false;
    }

    if (sentence_type != GPS_SENTENCE_OTHER && term[0])
    {
        switch(COMBINE(sentence_type, term_number))
        {
          case COMBINE(GPS_SENTENCE_GPRMC, 1): // Time in both sentences
          case COMBINE(GPS_SENTENCE_GPGGA, 1):
            new_time = parse_decimal();
            //new_time_fix = millis();
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 2): // GPRMC validity
            gps_data_good = term[0] == 'A';
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 3): // Latitude
          case COMBINE(GPS_SENTENCE_GPGGA, 2):
            new_latitude = parse_degrees();
            //new_position_fix = millis();
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 4): // N/S
          case COMBINE(GPS_SENTENCE_GPGGA, 3):
            if (term[0] == 'S')
              new_latitude = -new_latitude;
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 5): // Longitude
          case COMBINE(GPS_SENTENCE_GPGGA, 4):
            new_longitude = parse_degrees();
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 6): // E/W
          case COMBINE(GPS_SENTENCE_GPGGA, 5):
            if (term[0] == 'W')
              new_longitude = -new_longitude;
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 7): // Speed (GPRMC)
            new_speed = parse_decimal();
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 8): // Course (GPRMC)
            new_course = parse_decimal();
            break;
          case COMBINE(GPS_SENTENCE_GPRMC, 9): // Date (GPRMC)
            new_date = gpsatol(term);
            break;
          case COMBINE(GPS_SENTENCE_GPGGA, 6): // Fix data (GPGGA)
            gps_data_good = term[0] > '0';
            break;
          case COMBINE(GPS_SENTENCE_GPGGA, 7): // Satellites used (GPGGA)
            new_numsats = (unsigned char)atoi(term);
            break;
          case COMBINE(GPS_SENTENCE_GPGGA, 8): // HDOP
            new_hdop = parse_decimal();
            break;
          case COMBINE(GPS_SENTENCE_GPGGA, 9): // Altitude (GPGGA)
            new_altitude = parse_decimal();
            break;
        }
    }

    return false;
}

float distance_between(float lat1, float long1, float lat2, float long2)
{
    // returns distance in meters between two positions, both specified
    // as signed decimal-degrees latitude and longitude. Uses great-circle
    // distance computation for hypothetical sphere of radius 6372795 meters.
    // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
    // Courtesy of Maarten Lamers
    float delta = (M_PI/180.0f)*(long1-long2);
    float sdlong = sin(delta);
    float cdlong = cos(delta);
    lat1 = (M_PI/180.0f)*lat1;
    lat2 = (M_PI/180.0f)*lat2;
    float slat1 = sin(lat1);
    float clat1 = cos(lat1);
    float slat2 = sin(lat2);
    float clat2 = cos(lat2);
    delta = (clat1 * slat2) - (slat1 * clat2 * cdlong);
    delta = delta*delta;
    delta += (clat2 * sdlong)*(clat2 * sdlong);
    delta = sqrt(delta);
    float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong);
    delta = atan2(delta, denom);

    return delta * 6372795;
}

float course_to(float lat1, float long1, float lat2, float long2)
{
    // returns course in degrees (North=0, West=270) from position 1 to position 2,
    // both specified as signed decimal-degrees latitude and longitude.
    // Because Earth is no exact sphere, calculated course may be off by a tiny fraction.
    // Courtesy of Maarten Lamers
    float dlon = (M_PI/180.0f)*(long2-long1);
    lat1 = (M_PI/180.0f)*lat1;
    lat2 = (M_PI/180.0f)*lat2;
    float a1 = sin(dlon) * cos(lat2);
    float a2 = sin(lat1) * cos(lat2) * cos(dlon);
    a2 = cos(lat1) * sin(lat2) - a2;
    a2 = atan2(a1, a2);

    if (a2 < 0.0)
    {
      a2 += 2.0f*M_PI;
    }

    return ((180.0f/M_PI)*a2);
}

const char *cardinal(float course)
{
    static const char* directions[] = {"N", "NNE", "NE", "ENE", "E", "ESE", "SE", "SSE", "S", "SSW", "SW", "WSW", "W", "WNW", "NW", "NNW"};
    int direction = (int)((course + 11.25f) / 22.5f);

    return directions[direction % 16];
}

void get_position(long *_latitude, long *_longitude, unsigned long *_fix_age)
{
    if (_latitude) *_latitude = latitude;
    if (_longitude) *_longitude = longitude;

    //if (fix_age) *fix_age = _last_position_fix == GPS_INVALID_FIX_TIME ? GPS_INVALID_AGE : millis() - _last_position_fix;
}

void get_datetime(unsigned long *_date, unsigned long *_time, unsigned long *_age)
{
    if (_date) *_date = date;
    if (_time) *_time = time_gps;
    //if (age) *age = last_time_fix == GPS_INVALID_FIX_TIME ? GPS_INVALID_AGE : millis() - _last_time_fix;
}

float f_altitude(void)
{
    return (altitude == GPS_INVALID_ALTITUDE) ? GPS_INVALID_F_ALTITUDE : (altitude / 100.0);
}

float f_course(void)
{
    return course == GPS_INVALID_ANGLE ? GPS_INVALID_F_ANGLE : course / 100.0;
}

float f_speed_knots(void)
{
    return speed == GPS_INVALID_SPEED ? GPS_INVALID_F_SPEED : speed / 100.0;
}

float f_speed_mph(void)
{
    float sk = f_speed_knots();
    return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED : GPS_MPH_PER_KNOT * sk;
}

float f_speed_mps(void)
{
  float sk = f_speed_knots();
  return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED : GPS_MPS_PER_KNOT * sk;
}

float f_speed_kmph(void)
{
  float sk = f_speed_knots();
  return sk == GPS_INVALID_F_SPEED ? GPS_INVALID_F_SPEED : GPS_KMPH_PER_KNOT * sk;
}

