/*
  Air Quality Module DS-HCHO
  
  author: terryoy
  email: terry.ouyang@gmail.com
*/
#include <math.h>

/* Request data definition */
enum Command {
  QUERY = 0x01,
  CLEAR = 0x02,
  SPAN = 0x03,
  FACTORY = 0x04,
};

typedef struct {
  char begin_1 = 0x42;
  char begin_2 = 0x4d;
  char cmd = QUERY;
  char dhh = 0;
  char dll = 0;
  char lrch = 0;
  char lrcl = 0;
  
} AirCommand;

/* Response data definition */

enum AirType {
  NO_SENSOR = 0x00,
  CO = 0x01,
  H2S = 0x02, 
  CH4 = 0x03,
  CL2 = 0x04,
  HCL = 0x05,
  F2 = 0x06,
  HF = 0x07,
  NH3 = 0x08,
  HCN = 0x09,
  PH3 = 0x0a,
  NO = 0x0b,
  NO2 = 0x0c, 
  O3 = 0x0d, 
  O2 = 0x0e,
  SO2 = 0x0f,
  CLO2 = 0x10,
  COCL2 = 0x11,
  PH3_ = 0x12, // duplicated with 0x0a
  SIH4 = 0x13,
  HCHO = 0x14,
  CO2 = 0x15,
  VOC = 0x16,
  ETO = 0x17,
  C2H4 = 0x18,
  C2H2 = 0x19,
  SF6 = 0x1a,
  ASH3 = 0x1b,
  H2 = 0x1c,
  TOX1 = 0x1d,
  TOX2 = 0x1e,
  GAS_FLOW = 0x1f, // L/M
  BATTERY = 0x20,
};

enum MeasureUnit {
  PPM = 0x01,
  VOL = 0x02,
  LEL = 0x03,
  PPB = 0x04,
  MG_M3 = 0x05,
};

enum Equivalent {
  ONE = 0x01,
  TEN = 0x02,
  HUNDRED = 0x03,
  THOUSAND = 0x04,
};

typedef struct {
  char begin_1 = 0x42;
  char begin_2 = 0x4d;
  char length = 0x08;
  char air_type = 0x00;
  char unit = 0x00;
  char vh = 0x00; //
  char dhh = 0x00;
  char dll = 0x00;
  char lrch = 0x00;
  char lrcl = 0x00;
} AirResponse; // length 10

void parse_air_response(AirResponse *response, char *raw) {
  response->begin_1 = raw[0];
  response->begin_2 = raw[1];
  response->length = raw[2];
  response->air_type = raw[3];
  response->unit = raw[4];
  response->vh = raw[5];
  response->dhh = raw[6];
  response->dll = raw[7];
  response->lrch = raw[8];
  response->lrcl = raw[9];
}

char* get_unit_display(AirResponse response) {
  switch(response.unit) {
    case PPM:
      return "ppm";
    case VOL:
      return "VOL";
    case LEL:
      return "LEL";
    case PPB:
      return "Ppb";
    case MG_M3:
      return "Mg/m3";
    default:
      return "";
  }
}

float get_read_value(AirResponse response) {
  float equivalent = pow(10, response.vh) / 10;
  int read_value = (int)response.dhh << 8 | (int)response.dll;
  return read_value / equivalent;
}

