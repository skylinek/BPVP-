/* eLoyalty application for SmartDeck */

#pragma attribute("aid", "f0 00 00 01")
#pragma attribute("dir", "61 10 4f 4 f0 00 00 01 50 8 65 6c 6f 79 61 6c 74 79")

#include "multos.h"
#include <multoscomms.h>
#include <multoscrypto.h>

//#define MODLEN  256
//#define EXPLEN  2
//#include <RSA.h>

#include <multosarith.h>
#include <string.h>
#include <ISO7816.h>

#include "public512.cert"
#include "private512.cert"

/*
 #include "public1024.cert"
 #include "private1024.cert"
 */
/*
 #include "public2048.cert"
 #include "private2048.cert"
 */
/*
 #include "public3072.cert"
 #include "private3072.cert"*/
/*
 #include "public4096.cert"
 #include "private4096.cert"
 */
//#include <multosccr.h> // for ZFlag()#define ERR_OK          0x9000
#define ERR_WRONGCLASS  0x6402
#define ERR_BAD_INS     0x6404
#define ERR_UNDERFLOW   0x6406

#define INS_SET_KEY      0x01
#define INS_SET_IV       0x02
#define INS_SET_OPP1     0x03
#define INS_GET_OPP1     0x04
#define INS_SET_OPP2     0x05
#define INS_GET_OPP2     0x06
#define INS_SET_MODULUS  0x07
#define INS_GET_MODULUS  0x08
#define INS_SET_MULField  0xA0
#define INS_GET_MULField  0xA1
#define INS_SET_COUNTER  0xA2
#define INS_SET_DOMAIN  0x92
#define INS_GET_DOMAIN  0x93
#define INS_GET_MODLEN  0x94

#define INS_DES      	0x10
#define INS_DES_BLOCK   0x11
#define INS_TRIPLEDES   0x12
#define INS_AES	  	 	0x13

#define INS_RSA    		0x20
#define INS_DSA    		0x21
//#define INS_DH    		0x22

#define INS_ECCGenerateKey   0x22
#define INS_ECDH    	0x23
#define INS_ECDSA    	0x24

#define INS_SHA1	    0x32
#define INS_SHA	    	0x31

#define INS_RNG	    	0x40
#define INS_TEST_SPEED	0x41

#define INS_Modular_Addition  	0x50
#define INS_Modular_MUL	    	0x51
#define INS_Modular_EXP		 	0x52
#define INS_Modular_Reduction	0x53
#define INS_Modular_Inversion	0x54

#define MYAPP_CLA       0x80

//typedef unsigned char BYTE;
//typedef unsigned short WORD;

//#define ECC_KEY_LEN 20 //160
#define ECC_KEY_LEN 24 //192//#define ECC_KEY_LEN 28 //224
//#define ECC_KEY_LEN 32   //256
//#define ECC_KEY_LEN 40 // 320
//#define ECC_KEY_LEN 48 // 384
//#define ECC_KEY_LEN 64 //512
//#define MAXSIZE_OF_EC  		64
typedef struct {
	BYTE x[ECC_KEY_LEN];
	BYTE y[ECC_KEY_LEN];
} ecc_public_s;
typedef struct {
	ecc_public_s publicKey;
	BYTE privateKey[ECC_KEY_LEN];
} ecc_s;

/* Data from APDU; this is placed at PB[0] */
#pragma melpublic
BYTE apduData[255];

#pragma melstatic
ecc_s sEccKeyPair;
ecc_s sEccMyKey;

//BYTE pointG[1+2*MAXSIZE_OF_EC];
//BYTE pointP[1+2*MAXSIZE_OF_EC];
//BYTE multiplier[1+MAXSIZE_OF_EC];

int lenHash;
int velikostDat;
int i;
int j;
int counter;
BYTE supported;

WORD velikostDatReturn;

BYTE MULField[512];
WORD velikostMULField;

BYTE MULOpp1[512];
WORD velikostOpp1;

BYTE MULOpp2[512];
WORD velikostOpp2;

BYTE MODULUS[512];
WORD velikostMODULUS;

BYTE key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x01, 0x02, 0x03,
		0x04, 0x05, 0x06, 0x07, 0x08 };
WORD velikostKey;

WORD velikostDatFix = 128;
WORD velikostDatFix1 = 128;

BYTE abDomainParams[(6 * ECC_KEY_LEN) + 3];
WORD velikostDomain;
/*
 BYTE abDomainParams[] = {
 0x00, // Format of domain params
 0x1C, // Prime length in bytes
 0xAC, 0x75, 0xCF, 0x35, 0x99, 0x88, 0x5A, 0x6A, 0x26, 0xB2, 0x0F, 0x52,
 0x71, 0xAB, 0x95, 0xA3, 0xF0, 0xD2, 0x4B, 0x74, 0x37, 0x21, 0x46, 0xCC,
 0xDB, 0xA0, 0x5F,
 0xA9, // P
 0x93, 0x62, 0xE8, 0xF2, 0x7B, 0xDC, 0xA9, 0x6F, 0x81, 0xE6, 0xBF, 0xA6,
 0x79, 0x5E, 0x10, 0x60, 0xA9, 0x69, 0xD2, 0x0D, 0x9F, 0x88, 0x2E, 0xB4,
 0xD8, 0xE8, 0xD4,
 0x20, // A
 0x89, 0xD8, 0x66, 0x9D, 0x59, 0x20, 0x5C, 0xB4, 0xA3, 0x6E, 0xEC, 0x01,
 0x22, 0xC6, 0x49, 0x1C, 0x92, 0xB6, 0x18, 0xB8, 0xFC, 0x09, 0xB6, 0xD6,
 0xF3, 0x24, 0xAA,
 0xCA, // B
 0x2E, 0xDA, 0x6A, 0x9C, 0xE8, 0x53, 0x3B, 0xBC, 0xB8, 0x1D, 0x49, 0xF4,
 0x69, 0xB5, 0x43, 0x95, 0xD3, 0x1A, 0x64, 0xB8, 0x14, 0x8B, 0x92, 0xB3,
 0x6B, 0xC0, 0x23,
 0x00, // Gx
 0x84, 0xDA, 0x69, 0x9D, 0xF7, 0x56, 0xBF, 0x58, 0xC9, 0x50, 0x76, 0x7A,
 0xD7, 0xF8, 0x84, 0x62, 0x1E, 0x2F, 0x5C, 0xFC, 0x28, 0x25, 0x97, 0x99,
 0x14, 0x05, 0xB2,
 0x4D, // Gy
 0x0F, 0xAD, 0x9E, 0x79, 0x3C, 0x80, 0xC2, 0x66, 0xBD, 0xB3, 0x18, 0xAA,
 0x67, 0x6C, 0x9E, 0xDB, 0x4F, 0xB6, 0x53, 0xCF, 0x4F, 0x67, 0x92, 0x37,
 0x13, 0x37, 0x56, 0xA1, // N
 0x0B // H
 };

 */

unsigned char iv[] = { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
		0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 };
WORD velikostIV = sizeof(iv);

//void setMyVariable(BYTE* data, WORD velikost);

void main(void) {

	/* Check class in APDU. */
	if (CLA != MYAPP_CLA)
		ExitSW(ERR_WRONGCLASS);

	/* Decode instruction. */
	switch (INS) {

	case INS_SET_KEY:
		if (!CheckCase(4))
			ExitSW(ERR_WRONGCLASS);
		setMyVariable(key, &velikostKey);
		break;

	case INS_SET_IV:
		if (!CheckCase(4))
			ExitSW(ERR_WRONGCLASS);
		setMyVariable(iv, &velikostIV);
		break;
	case INS_SET_OPP1:
		if (!CheckCase(4))
			ExitSW(ERR_WRONGCLASS);
		setMyVariable(MULOpp1, &velikostOpp1);
		break;
	case INS_GET_OPP1:
		if (!CheckCase(2))
			ExitSW(ERR_WRONGCLASS);
		getMyVariable(MULOpp1, &velikostOpp1);
		break;
	case INS_SET_OPP2:
		if (!CheckCase(4))
			ExitSW(ERR_WRONGCLASS);
		setMyVariable(MULOpp2, &velikostOpp2);
		break;
	case INS_GET_OPP2:
		if (!CheckCase(2))
			ExitSW(ERR_WRONGCLASS);
		getMyVariable(MULOpp2, &velikostOpp2);
		break;
	case INS_SET_MODULUS:
		if (!CheckCase(4))
			ExitSW(ERR_WRONGCLASS);
		setMyVariable(MODULUS, &velikostMODULUS);
		break;

	case INS_GET_MODULUS:
		if (!CheckCase(2))
			ExitSW(ERR_WRONGCLASS);
		getMyVariable(MODULUS, &velikostMODULUS);
		break;

	case INS_SET_MULField:
		if (!CheckCase(4))
			ExitSW(ERR_WRONGCLASS);
		setMyVariable(MULField, &velikostMULField);
		break;

	case INS_GET_MULField:
		if (!CheckCase(2))
			ExitSW(ERR_WRONGCLASS);
		getMyVariable(MULField, &velikostMULField);
		break;
	case INS_SET_COUNTER:
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);
		counter = P1;
		break;

	case INS_SET_DOMAIN:
		if (!CheckCase(4))
			ExitSW(ERR_WRONGCLASS);
		setMyVariable(abDomainParams, &velikostDomain);
		break;

	case INS_GET_DOMAIN:
		if (!CheckCase(2))
			ExitSW(ERR_WRONGCLASS);
		getMyVariable(abDomainParams, &velikostDomain);
		break;
	case INS_GET_MODLEN:
		if (!CheckCase(2))
			ExitSW(ERR_WRONGCLASS);
		i = MODLEN;
		memcpy(apduData, &i, 0x02);
		ExitLa(0x2);
		break;

	case INS_DES:
		/* Ensure case 4 command. */
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);
		if (P1 == 0) {

			for (j = 0; j < counter; j = j + 1) {
				i = 0;
				for (i = 0; i < velikostOpp1; i = i + 8) {
					DESECBEncipher(MULOpp1 + i, MULOpp2 + i, key);
				}
			}
			velikostOpp2 = velikostOpp1;
			velikostMODULUS = velikostOpp1;
		}
		if (P1 == 1) {

			for (j = 0; j < counter; j = j + 1) {
				i = 0;
				for (i = 0; i < velikostOpp2; i = i + 8) {
					DESECBDecipher(MULOpp2 + i, MODULUS + i, key);
				}
			}
		}
		multosExitSW(0x9000);
		break;

	case INS_DES_BLOCK:

		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);
		if (P1 == 0) {
			for (j = 0; j < counter; j = j + 1) {
				multosBlockEncipherECB(0x03, velikostOpp1, MULOpp1, MULOpp2, 8,
						key);
			}
			velikostOpp2 = velikostOpp1;
			velikostMODULUS = velikostOpp1;

		}
		if (P1 == 1) {
			for (j = 0; j < counter; j = j + 1) {
				multosBlockDecipherECB(0x03, velikostOpp1, MULOpp2, MODULUS, 8,
						key);
			}
		}

		multosExitSW(0x9000);
		break;

	case INS_TRIPLEDES:
		/* Ensure case 3 command. */
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		/*
		 void multosBlockEncipherCBC (const BYTE algorithm, WORD inputLength,
		 BYTE *plainText, BYTE *cipherText,
		 BYTE initialValueLength, BYTE *initialValue,
		 BYTE keyLength, BYTE *key);*/

		//3DES 0x04
		if (P1 == 0) {
			for (j = 0; j < counter; j = j + 1) {
				multosBlockEncipherECB(0x04, velikostOpp1, MULOpp1, MULOpp2,
						velikostKey, key);
			}
			velikostOpp2 = velikostOpp1;
			velikostMODULUS = velikostOpp1;
		}
		//void multosBlockDecipherCBC (const BYTE algorithm, WORD inputLength,  BYTE *cipherText, BYTE *plainText,  BYTE initialValueLength, BYTE *initialValue,  BYTE keyLength, BYTE *key);
		//3DES 0x04
		if (P1 == 1) {
			for (j = 0; j < counter; j = j + 1) {
				multosBlockDecipherECB(0x04, velikostOpp2, MULOpp2, MODULUS,
						velikostKey, key);
			}
		}
		multosExitSW(0x9000);
		break;

	case INS_AES: // vstup nasobky 16 bytu, klic mi fungoval pouze 16 byte , IV 16 byte
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		/*
		 void multosBlockEncipherCBC (const BYTE algorithm, WORD inputLength,
		 BYTE *plainText, BYTE *cipherText,
		 BYTE initialValueLength, BYTE *initialValue,
		 BYTE keyLength, BYTE *key);*/
		if (P1 == 0) {

			for (j = 0; j < counter; j = j + 1) {
				for (i = 0; i < velikostOpp1; i = i + 16) {

					multosAESECBEncipher(MULOpp1 + i, MULOpp2 + i, velikostKey,
							key);
				}
			}
			//	multosAESECBEncipher (MULOpp1, MULOpp2, velikostKey,key);

			velikostOpp2 = velikostOpp1;
			velikostMODULUS = velikostOpp1;
		}
		if (P1 == 1) {

			for (j = 0; j < counter; j = j + 1) {

				for (i = 0; i < velikostMODULUS; i = i + 16) {

					multosAESECBDecipher(MULOpp2 + i, MODULUS + i, velikostKey,
							key);

				}
			}
			//multosAESECBDecipher (MULOpp2, MULOpp1, velikostKey,key);

		}
		multosExitSW(0x9000);
		break;

	case INS_RSA:
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);
		if (P1 == 0) {
			for (j = 0; j < counter; j = j + 1) {
				RSAEncipher(&publicKey, MULOpp1, MULOpp2);
			}
			velikostOpp2 = MODLEN;

		} else if (P1 == 1) {
			for (j = 0; j < counter; j = j + 1) {
				RSADecipher(&privateKey, MULOpp2, MODULUS);
			}
			velikostMODULUS = MODLEN;

		}
		multosExitSW(0x9000);

		break;
	case INS_ECCGenerateKey:
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		__push(abDomainParams); //ECC param 
		__push(&sEccMyKey);   // keyAdress 
		__code(PRIM, 0xE7, 0x80); //generate key pair , protect private key 

		__push(abDomainParams); //ECC param 
		__push(&sEccKeyPair);   // keyAdress 
		__code(PRIM, 0xE7, 0x80); //generate key pair , protect private key 

		multosExitSW(0x9000);
		break;
	case INS_ECDH:
		for (j = 0; j < counter; j = j + 1) {

			__push(abDomainParams); //ECC param 
			__push((BYTE*) &sEccMyKey.privateKey);  //privateKeyAddr
			__push(&sEccKeyPair.publicKey); //publicKeyAddr
			__push((BYTE*) &MULOpp2); //sharedAddr
			__code(PRIM, 0xE8, 0x80);

		}
		velikostOpp2 = ECC_KEY_LEN;

		multosExitSW(0x9000);
		break;

	case INS_ECDSA:
		/* Ensure case 3 command. */
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		if (P1 == 0) {
			for (j = 0; j < counter; j = j + 1) {

				__push(abDomainParams); //ECC param 
				__push((BYTE*) &sEccMyKey.privateKey); //privatekey
				__push((BYTE*) &MULOpp1); // hashAddr
				__push((BYTE*) &MULOpp2); //sigAddr
				__code(PRIM, 0xE5, 0x80);
			}
		}
		if (P1 == 1) {
			for (j = 0; j < counter; j = j + 1) {

				multosEccVerifySignature(abDomainParams, MULOpp1, MULOpp2,
						(BYTE*) &sEccMyKey.publicKey, 0x00, &supported); // pada pokud to neni uspesne na karte 
			}
		}
		multosExitSW(0x9000);

		break;

	case INS_SHA1:
		/* Ensure case 3 command. */
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		//velikostDat = Lc;
		for (j = 0; j < counter; j = j + 1) {

			__push((BYTE*) velikostOpp1); //lenMsg
			__push((BYTE*) &MULOpp2); //addrHash
			__push((BYTE*) &MULOpp1); //addrMsg
			__code(PRIM, 0xCA);
		}
		velikostOpp2 = 20;
		multosExitSW(0x9000);
		break;

	case INS_SHA:
		/* Ensure case 3 command. */
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		lenHash = P1;
		for (j = 0; j < counter; j = j + 1) {

			__push((BYTE*) velikostOpp1); //lenMsg
			__push((BYTE*) lenHash); //lenHash
			__push((BYTE*) &MULOpp2); // addrHash
			__push((BYTE*) &MULOpp1); //addrMsg
			__code(PRIM, 0xCF);
		}
		multosExitSW(0x9000);
		break;

	case INS_RNG:
		/* Ensure case 3 command. */
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);
		for (j = 0; j < counter; j = j + 1) {

			i = 0;
			velikostOpp2 = P1;
			for (velikostOpp1 = P1; i < velikostOpp1; i = i + 8) {

				__code(PRIM, 0xC4);
				// __push((BYTE*)&randomField);                                  
				__code(STORE, (BYTE*) &MULOpp1, 8);

				memcpy(MULOpp2 + i, MULOpp1, 8);
			}
		}
		multosExitSW(0x9000);
		break;

	case INS_TEST_SPEED:
		if (!CheckCase(2))
			ExitSW(ERR_WRONGCLASS);
		multosExitSW(0x9000);

		break;

	case INS_Modular_Addition:
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		for (j = 0; j < counter; j = j + 1) {

			ADDN(128, MULField, MULOpp1, MULOpp2);
			__push((BYTE*) velikostOpp2); // len opp
			__push((BYTE*) velikostMODULUS); // len modulus
			__push((BYTE*) &MULField);  //addrOp   --->result 
			__push((BYTE*) &MODULUS); //addrMod
			__code(PRIM, 0xC3);  //reduction
		}
		velikostMULField = velikostOpp2;

		multosExitSW(0x9000);
		break;

	case INS_Modular_MUL:
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);

		__push((WORD) velikostMODULUS); // lenMod
		__push((BYTE*) &MULOpp1); //opp1  -->result
		__push((BYTE*) &MULOpp2); //opp2
		__push((BYTE*) &MODULUS); // modulus
		__code(PRIM, 0xC2);

		j = 0;
		for (j = 0; j < (counter - 1); j = j + 1) {

			__push((WORD) velikostMODULUS); // lenMod
			__push((BYTE*) &MULField); //opp1  -->result
			__push((BYTE*) &MULOpp2); //opp2
			__push((BYTE*) &MODULUS); // modulus
			__code(PRIM, 0xC2);

		}
		velikostOpp1 = velikostMODULUS;
		multosExitSW(0x9000);
		break;

	case INS_Modular_EXP:
		if (!CheckCase(1))
			ExitSW(ERR_WRONGCLASS);
		for (j = 0; j < counter; j = j + 1) {

			__push((WORD) velikostOpp2); // lenExponent
			__push((WORD) velikostMODULUS); // lenModulus
			__push((BYTE*) &MULOpp2);  //exponent addres
			__push((BYTE*) &MODULUS); //modulus adres
			__push((BYTE*) &MULOpp1); //input
			__push((BYTE*) &MULField); //output
			__code(PRIM, 0xC8);
		}
		velikostMULField = velikostMODULUS;

		multosExitSW(0x9000);
		break;

	case INS_Modular_Reduction:
		for (j = 0; j < counter; j = j + 1) {

			__push((BYTE*) velikostOpp1); // len opp
			__push((BYTE*) velikostMODULUS); // len modulus
			__push((BYTE*) &MULOpp1);  //addrOp   --->result 
			__push((BYTE*) &MODULUS); //addrMod
			__code(PRIM, 0xC3);  //reduction
		}
		velikostOpp1 = velikostMODULUS;
		multosExitSW(0x9000);
		break;

	case INS_Modular_Inversion:
		for (j = 0; j < counter; j = j + 1) {

			__push((WORD) velikostMODULUS); // len modulus
			__push((BYTE*) &MODULUS);  //modulus
			__push((WORD) velikostOpp1);  // len opp
			__push((BYTE*) &MULOpp1); // input ->operand
			__push((BYTE*) &MULField); // result
			__code(PRIM, 0xD0, 0x00);
		}
		velikostMULField = velikostMODULUS;
		multosExitSW(0x9000);

		break;

	default:
		ExitSW(ERR_BAD_INS);
	}

}
void setMyVariable(BYTE* data, WORD* velikost) {
	if (P1 == 0x00) {
		*velikost = (WORD) Lc;
		memcpy(data, apduData, Lc);
		ExitLa (Lc);
	} else if (P1 == 0x01) {
		memcpy(data + *velikost, apduData, Lc);
		*velikost = *velikost + Lc;
		ExitLa (Lc);
	} else if (P1 == 0x02) {
		memcpy(data + *velikost, apduData, Lc);
		*velikost = *velikost + Lc;
		ExitLa (Lc);
	}

}

void getMyVariable(BYTE* data, WORD* velikost) {
	if (P2 == 0x00) {
		velikostDatReturn = 255;

	} else {

		velikostDatReturn = *velikost;
		if (velikostDatReturn > 255) {
			if (P1 == 0x00) {
				velikostDatReturn = 255;
			} else if (P1 == 0x01) {

				velikostDatReturn = *velikost - 255;
			}
		}

	}

	if (P1 == 0x00) {
		memcpy(apduData, data, velikostDatReturn);
		ExitLa(velikostDatReturn);
	} else if (P1 == 0x01) {
		memcpy(apduData, data + 255, velikostDatReturn);
		ExitLa(velikostDatReturn);
	}

}
