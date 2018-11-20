#define MODLEN 256
#define EXPLEN 2
#include <rsa.h>
RSAPrivateCRTKey privateKey = 
{
  MODLEN,
  {
  // DP
  0x0e, 0x77, 0x22, 0xb3, 0x12, 0x74, 0xba, 0xd2,
  0x18, 0xa3, 0x6d, 0xe7, 0x5a, 0xf0, 0x1d, 0xf4,
  0xb7, 0x41, 0x10, 0x46, 0x96, 0xce, 0xdb, 0xe8,
  0x7c, 0x50, 0x1c, 0xdb, 0xb2, 0x40, 0xca, 0xf8,
  0xff, 0xde, 0xb2, 0x09, 0xda, 0x36, 0xea, 0x2f,
  0x50, 0x2b, 0x70, 0x5a, 0x92, 0xf5, 0xfb, 0x00,
  0xc0, 0x40, 0x3b, 0xe3, 0x08, 0x9c, 0x59, 0xd0,
  0xb4, 0xab, 0x93, 0xd6, 0x1d, 0x00, 0xe6, 0xd9,
  0x0e, 0x0e, 0x06, 0x62, 0xfa, 0xb2, 0x06, 0x27,
  0x24, 0x26, 0x53, 0x95, 0xed, 0x67, 0xd1, 0xee,
  0xc5, 0x48, 0x2e, 0x97, 0xd6, 0xf7, 0xe2, 0xa0,
  0xaf, 0x26, 0x28, 0x07, 0xbe, 0x79, 0x2c, 0xbc,
  0xf2, 0xd8, 0x4b, 0x1c, 0x32, 0x85, 0xa4, 0x69,
  0x8f, 0xef, 0xae, 0x07, 0x7f, 0x9a, 0xab, 0x0f,
  0x45, 0x0f, 0x02, 0xb2, 0x3c, 0x26, 0x01, 0xde,
  0x01, 0xc6, 0xaa, 0x75, 0x11, 0x83, 0x69, 0xbf
  },
  {
  // DQ
  0x2a, 0xba, 0xec, 0xe2, 0xcf, 0x9e, 0xa9, 0x69,
  0x52, 0x91, 0x1d, 0x24, 0xbf, 0xae, 0x01, 0x1a,
  0x71, 0xa6, 0x18, 0xc4, 0x63, 0x71, 0xdc, 0xda,
  0x31, 0xbf, 0x59, 0x0f, 0x93, 0xac, 0x07, 0x3c,
  0xa9, 0xb6, 0x8d, 0x0f, 0xc1, 0x26, 0x6c, 0x60,
  0xd1, 0x84, 0x30, 0x43, 0xeb, 0x06, 0x64, 0x81,
  0x16, 0x4b, 0x04, 0x5b, 0x54, 0x11, 0x0a, 0x98,
  0x3c, 0x1e, 0xa4, 0xa3, 0x15, 0xd5, 0x3f, 0xa8,
  0xc3, 0xb2, 0x41, 0x54, 0x8c, 0x88, 0x88, 0xce,
  0x4b, 0x7c, 0x68, 0xd9, 0x36, 0x9a, 0xc3, 0xa3,
  0xd1, 0x53, 0xbf, 0x23, 0x5c, 0x4f, 0x65, 0xf8,
  0x35, 0x44, 0xae, 0x31, 0x5e, 0xf4, 0xdd, 0x29,
  0xca, 0x58, 0x86, 0xba, 0x6b, 0x77, 0xc9, 0xd3,
  0x42, 0xa8, 0x00, 0xde, 0xbb, 0xc4, 0x92, 0x3b,
  0xad, 0x66, 0x0e, 0xde, 0x48, 0x09, 0xff, 0x9c,
  0x37, 0x52, 0xa2, 0x92, 0xb1, 0xeb, 0x0f, 0xc7
  },
  {
  // P
  0xc7, 0xf9, 0xc6, 0x93, 0x4b, 0x83, 0x04, 0xc8,
  0xed, 0x6b, 0x58, 0xf4, 0xe8, 0x63, 0x46, 0xc9,
  0x00, 0x5a, 0x22, 0x42, 0x5f, 0x28, 0x2b, 0xd6,
  0x99, 0xae, 0x09, 0x88, 0xab, 0xb7, 0x12, 0x93,
  0xc1, 0x8b, 0xb3, 0x5d, 0x62, 0x47, 0x7f, 0x77,
  0x80, 0x0a, 0x85, 0xa8, 0x0b, 0xd2, 0x21, 0xf8,
  0x31, 0x35, 0x53, 0x8a, 0xa9, 0xed, 0x64, 0xa6,
  0xc5, 0x46, 0x3a, 0x40, 0x4d, 0x39, 0x56, 0xb6,
  0xe4, 0xa0, 0x56, 0xbc, 0x0e, 0xc8, 0xc0, 0x7f,
  0xe8, 0xeb, 0xb1, 0x99, 0xc2, 0x49, 0x4b, 0x5f,
  0x86, 0xd3, 0x79, 0x4b, 0x84, 0x63, 0xa7, 0x4e,
  0xbd, 0x75, 0x8c, 0xfa, 0x97, 0xad, 0xc8, 0x78,
  0x7a, 0xb0, 0xe5, 0x3b, 0xfe, 0x07, 0xe5, 0x70,
  0x3e, 0x16, 0x86, 0xb2, 0xaf, 0x78, 0x11, 0xb3,
  0x63, 0x49, 0x9a, 0x32, 0x81, 0xb1, 0xfc, 0x5e,
  0xb9, 0x13, 0x20, 0xa8, 0xc0, 0xc7, 0xd0, 0x6d
  },
  {
  // Q
  0xcc, 0x3f, 0x74, 0xe4, 0x37, 0x7b, 0x59, 0x98,
  0x5c, 0xda, 0x8a, 0x27, 0x85, 0x24, 0xc9, 0x96,
  0x3c, 0x74, 0x55, 0x0e, 0x0e, 0xa2, 0x2d, 0xaf,
  0x29, 0x1c, 0x1d, 0x7b, 0x72, 0xf7, 0x77, 0x63,
  0x22, 0x4f, 0xb8, 0x69, 0xd2, 0x6e, 0x45, 0x3a,
  0x63, 0xb0, 0x62, 0x43, 0x8b, 0x19, 0x83, 0xda,
  0x29, 0x96, 0x33, 0x0c, 0x3a, 0x2c, 0xe5, 0x5c,
  0x1d, 0xa1, 0x9e, 0xd6, 0x49, 0xb2, 0x83, 0x58,
  0x56, 0x89, 0xbd, 0xbe, 0x99, 0x15, 0xd0, 0xe7,
  0x5d, 0xc1, 0xeb, 0x7a, 0xa7, 0x41, 0x4b, 0xb8,
  0x9a, 0xd9, 0x79, 0x9f, 0x6d, 0xbd, 0xe3, 0x20,
  0x54, 0x19, 0x7f, 0x9f, 0x6a, 0xf9, 0x50, 0xcb,
  0x73, 0x7b, 0xc9, 0x5e, 0xfb, 0x8c, 0x5c, 0x2b,
  0x71, 0xaf, 0x9c, 0xfc, 0x49, 0x42, 0x58, 0x9b,
  0xfe, 0x7b, 0x52, 0xe3, 0xb5, 0x88, 0x0f, 0xe7,
  0x47, 0x3c, 0x11, 0xba, 0x86, 0xc8, 0xd2, 0x55
  },
  {
  // U
  0x41, 0x31, 0x63, 0x5e, 0xe1, 0x74, 0xa8, 0x5f,
  0xc4, 0x0e, 0xbe, 0x00, 0xe8, 0x0b, 0x69, 0xd3,
  0xb4, 0x4e, 0xc6, 0x04, 0x27, 0xf6, 0x8f, 0x07,
  0xc4, 0x84, 0xf7, 0x88, 0x94, 0x44, 0x2e, 0x61,
  0x64, 0xd7, 0x4b, 0x11, 0x19, 0xc2, 0x27, 0x48,
  0x3c, 0x71, 0x76, 0x85, 0xaa, 0x89, 0x9a, 0x49,
  0x4e, 0x4b, 0xa3, 0x6e, 0xd8, 0x03, 0x13, 0xfa,
  0xc4, 0xe5, 0xa2, 0xf2, 0xc0, 0x29, 0x33, 0xf5,
  0x42, 0xe9, 0x39, 0x11, 0xa9, 0x31, 0xfa, 0xa8,
  0x7f, 0x01, 0xef, 0xda, 0xea, 0xc9, 0x32, 0xd1,
  0xf0, 0x02, 0xf5, 0x43, 0xed, 0x9b, 0x88, 0xa6,
  0x51, 0x2e, 0xfc, 0x99, 0xd6, 0xf4, 0xca, 0x4c,
  0x34, 0x10, 0x55, 0xcf, 0x9d, 0xc5, 0x0c, 0xe3,
  0xe6, 0x8c, 0x2f, 0xf7, 0x81, 0xcd, 0x1c, 0xd0,
  0x4d, 0x94, 0x5e, 0x72, 0x5a, 0x99, 0xd6, 0x6c,
  0x2a, 0x14, 0x11, 0x17, 0x94, 0xb7, 0xd8, 0x6b
  }
};

