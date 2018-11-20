#ifndef __MULTOS_H
#define __MULTOS_H


/*
** ----------------------------------------------------------------------------
** Copyright (C) 2012-2016
** MAOSCO Limited.
**
** FILE NAME:       multos.h
**
** DESCRIPTION:     Standard MULTOS API for SmartDeck.
** VERSION:         1.4
**
** DISCLAIMER: This header file is provided "as is" and comes with no warranty
** or guarantee of correct operation. Anyone using functions from this file
** MUST independently ensure correct operation of applications containing calls
** to these functions.
**
** CHANGES in 1.3: 
** Added multosSetSelectCLSW, multosBlockLookupWord, multosGetProcessEvent, multosRejectProcessEvent
** multosRsaVerify, multosSetATSHistoricalCharacters, multosSetFCIFileRecord, multosInitialisePIN
** multosReadPIN, multosVerifyPIN, multosSetPINTryCounter, multosSetPINTryLimit, multosGetPINTryCount
** multosGetPINTryLimit, multosGetPINStatus, multosFlushPublic
**
** Corrected parameter order (in macro) for multosBlockEncipherECB (ciphertext and plaintext params were inverted)
**
** Added #defines for Process Events
**
** CHANGES in 1.4:
** Added multosBlockShiftLeftVar, multosBlockShiftRightVar, multosBlockRotateLeft, multosBlockRotateRight,
** multosUpdateStaticSize
**-----------------------------------------------------------------------------
*/

#define __SMARTWORKS    1
#define __SMARTDECK     1

/*
**=============================================================================
** Standard type names used by the C-API.
**=============================================================================
*/
typedef unsigned char BOOL;
typedef unsigned char BYTE;
typedef signed char SBYTE;
typedef unsigned short WORD;
typedef short SWORD;
typedef unsigned long DWORD;
typedef long SDWORD;

typedef void (*FN_PTR) (void);

#define TRUE    (1)
#define FALSE   (0)
#define NULL    (0)

#define MAX_BYTE    (255)
#define MAX_WORD    (65535)
#define MAX_DWORD   (4294967295)

/*
** Crypto algorithms
*/
#define ALGORITHM_DES   (3)
#define ALGORITHM_3DES  (4)
#define ALGORITHM_SEED  (5)
#define ALGORITHM_AES   (6)
#define ALGORITHM_RSA   (7)
#define ALGORITHM_COMP128   (8)

/*
** Possible application execution events 
*/
#define EVENT_APP_APDU                       0x00
#define EVENT_SELECT_APDU                    0x01
#define EVENT_AUTO_SELECT                    0x02
#define EVENT_RESELECT_APDU                  0x03
#define EVENT_DESELECT                       0x04
#define EVENT_CREATE                         0x05
#define EVENT_DELETE                         0x06

/*
**=============================================================================
** Specific SmartWorks constants and macros.  Note that these do not form a
** part of the C-API.
**=============================================================================
*/
/* Support for N byte block arithmetic. */
#define __BLOCKCAST(N) *(struct { BYTE __x[N]; }*)

/* OP N blocks at OP1 and OP2 and store in RES.  N and RES must be constants. */
#define __BINARY_OPN(N, OP, RES, OP1, OP2) \
do \
    {  \
  __push (__BLOCKCAST(N)(__typechk(BYTE *, OP1))); \
  __push (__BLOCKCAST(N)(__typechk(BYTE *, OP2))); \
  __code (OP, N);  \
  __code (__POPN, N);  \
  __code (__STORE, __typechk(BYTE *, RES), N); \
} while (0)

/* OP N block */
#define __UNARY_OPN(N, OP, V) \
  __code(OP, V, N);

/* Internal system variables. */
/*extern BYTE ProtocolFlags;
extern BYTE ProtocolType;
extern BYTE GetResponseCLA;
extern BYTE GetResponseSW1;
extern BYTE CLA;
extern BYTE INS;
extern BYTE P1;
extern BYTE P2;
extern BYTE P3;
extern WORD P1P2;               /* P1 in MSB, P2 in LSB. */
/*extern WORD Lc;
extern WORD Le;
extern WORD La;
extern BYTE SW1;
extern BYTE SW2;
extern WORD SW12;               /* SW1 in MSB, SW2 in LSB. */

/* The values of each MULTOS instruction. */
#define __SYSTEM    0x00
#define __BRANCH    0x01
#define __JUMP      0x02
#define __CALL      0x03
#define __STACK     0x04
#define __PRIMRET   0x05
#define __UNUSED    0x06
#define __LOAD      0x07
#define __STORE     0x08
#define __LOADI     0x09
#define __STOREI    0x0A
#define __LOADA     0x0B
#define __INDEX     0x0C
#define __SETB      0x0D
#define __CMPB      0x0E
#define __ADDB      0x0F
#define __SUBB      0x10
#define __SETW      0x11
#define __CMPW      0x12
#define __ADDW      0x13
#define __SUBW      0x14
#define __CLEARN    0x15
#define __TESTN     0x16
#define __INCN      0x17
#define __DECN      0x18
#define __NOTN      0x19
#define __CMPN      0x1A
#define __ADDN      0x1B
#define __SUBN      0x1C
#define __ANDN      0x1D
#define __ORN       0x1E
#define __XORN      0x1F

#define __RET       0x20
#define __POPB      0x21
#define __POPW      0x22
#define __POPN      0x23
#define __PUSHB     0x24
#define __PUSHW     0x25
#define __PUSHZ     0x26
#define __PRIM      0x27

/* The values of each set zero MULTOS primitive. */
#define __PRIM_CHECK_CASE                           0x01
#define __PRIM_RESET_WWT                            0x02
#define __PRIM_LOAD_CCR                             0x05
#define __PRIM_STORE_CCR                            0x06
#define __PRIM_SET_ATR_FILE_RECORD                  0x07
#define __PRIM_SET_ATR_HISTORICAL_CHARACTERS        0x08
#define __PRIM_GET_MEMORY_RELIABILITY               0x09
#define __PRIM_LOOKUP                               0x0A
#define __PRIM_MEMORY_COMPARE                       0x0B
#define __PRIM_MEMORY_COPY                          0x0C
#define __PRIM_QUERY_INTERFACE_TYPE                 0x0D
#define __PRIM_SET_ATS_HISTORICAL_CHARACTERS        0x0E
#define __PRIM_MEMORY_COPY_NON_ATOMIC               0x0F
#define __PRIM_CONTROL_AUTO_RESET_WWT               0x10
#define __PRIM_SET_FCI_FILE_RECORD                  0x11
#define __PRIM_CARD_UNBLOCK                         0x13
#define __PRIM_LOOKUP_WORD                          0x14
#define __PRIM_EXIT_TO_MULTOS_AND_RESTART           0x17
#define __PRIM_DELEGATE                             0x80
#define __PRIM_RESET_SESSION_DATA                   0x81
#define __PRIM_CHECKSUM                             0x82
#define __PRIM_CALL_CODELET                         0x83
#define __PRIM_QUERY_CODELET                        0x84
#define __PRIM_EXCHANGE_DATA                        0x85
#define __PRIM_QUERY_CHANNEL                        0x86
#define __PRIM_PLATFORM_OPTIMISED_CHECKSUM          0x89
#define __PRIM_QUERY_ALGO                           0x8A
#define __PRIM_DES_ECB_ENCIPHER                     0xC1
#define __PRIM_MODULAR_MULTIPLICATION               0xC2
#define __PRIM_MODULAR_REDUCTION                    0xC3
#define __PRIM_GET_RANDOM_NUMBER                    0xC4
#define __PRIM_DES_ECB_DECIPHER                     0xC5
#define __PRIM_GENERATE_DES_CBC_SIGNATURE           0xC6
#define __PRIM_GENERATE_TRIPLE_DES_CBC_SIGNATURE    0xC7
#define __PRIM_MODULAR_EXPONENTIATION               0xC8
#define __PRIM_MODULAR_EXPONENTIATION_CRT           0xC9
#define __PRIM_SHA_1                                0xCA
#define __PRIM_GSM_AUTHENTICATE                     0xCB
#define __PRIM_SECURE_HASH                          0xCF
#define __PRIM_ECC_ADDITION                         0xD0
#define __PRIM_ECC_CONVERT_REPRESENTATION           0xD1
#define __PRIM_ECC_EQUALITY_TEST                    0xD2
#define __PRIM_ECC_INVERSE                          0xD3
#define __PRIM_ECC_SCALAR_MULTIPLICATION            0xD4
#define __PRIM_AES_ECB_DECIPHER                     0xD6
#define __PRIM_AES_ECB_ENCIPHER                     0xD7
#define __PRIM_3DES_ECB_DECIPHER                    0xD8
#define __PRIM_3DES_ECB_ENCIPHER                    0xD9
#define __PRIM_MODULAR_EXPONENTIATION_CRT_PROTECTED 0xDC
#define __PRIM_SECURE_HASH_IV                       0xE4
#define __PRIM_INITIALISE_PIN                       0xE5
#define __PRIM_READ_PIN                             0xE6
#define __PRIM_VERIFY_PIN                           0xE7
#define __PRIM_GET_PROCESS_EVENT                    0xE8
#define __PRIM_REJECT_PROCESS_EVENT                 0xE9
#define __PRIM_RSA_VERIFY                           0xEB
#define __PRIM_FLUSH_PUBLIC                         0xEC


/* The values of each set one MULTOS primitive. */
#define __PRIM_QUERY                                0x00   /* Range 0x00-0x03 */
#define __PRIM_UPDATE_STATIC_SIZE					0x04
#define __PRIM_DIVIDEN                              0x08
#define __PRIM_GET_DIR_FILE_RECORD                  0x09
#define __PRIM_GET_FILE_CONTROL_INFORMATION         0x0A
#define __PRIM_GET_MANUFACTURER_DATA                0x0B
#define __PRIM_GET_MULTOS_DATA                      0x0C
#define __PRIM_GET_PURSE_TYPE                       0x0D
#define __PRIM_MEMORY_COPY_FIXED_LENGTH             0x0E
#define __PRIM_MEMORY_COMPARE_FIXED_LENGTH          0x0F
#define __PRIM_MULTIPLYN                            0x10
#define __PRIM_ADD_BCDN                             0x11
#define __PRIM_SUBTRACT_BCDN                        0x12
#define __PRIM_MEMORY_COPY_NON_ATOMIC_FIXED_LENGTH  0x13
#define __PRIM_CONVERT_BCD                          0x14
#define __PRIM_PAD                                  0x15
#define __PRIM_UNPAD                                0x16
#define __PRIM_SET_TRANSACTION_PROTECTION           0x80
#define __PRIM_GET_DELEGATOR_AID                    0x81
#define __PRIM_SET_PIN_DATA                         0x85
#define __PRIM_GET_PIN_DATA                         0x86
#define __PRIM_GET_DATA                             0x87
#define __PRIM_SET_PROTECTED_MEMORY_ACCESS          0x88
#define __PRIM_GENERATE_ASYMMETRIC_HASH             0xC4
#define __PRIM_ECC_VERIFY                           0xD1
#define __PRIM_CONFIG_READ_BINARY                   0xDC
#define __PRIM_COPY_ADDITIONAL_STATIC               0xDD
#define __PRIM_FILL_ADDITIONAL_STATIC               0xDE
#define __PRIM_GET_STATIC_SIZE                      0xDF
#define __PRIM_SET_SILENT_MODE                      0xE3
#define __PRIM_GEN_ECC_SIG                          0xE5
#define __PRIM_VER_ECC_SIG                          0xE6
#define __PRIM_GEN_ECC_PAIR                         0xE7
#define __PRIM_ECCDH                                0xE8
#define __PRIM_ECC_DEC                              0xE9
#define	__PRIM_ECC_ENC                              0xEA

/* The values of each set two MULTOS primitive. */
#define __PRIM_BIT_MANIPULATE_BYTE                  0x01
#define __PRIM_SHIFT_LEFT                           0x02
#define __PRIM_SHIFT_RIGHT                          0x03
#define __PRIM_SET_SELECT_SW                        0x04
#define __PRIM_CARD_BLOCK                           0x05
#define __PRIM_SET_SELECT_CL_SW                     0x06
#define __PRIM_SHIFT_ROTATE                         0x07
#define __PRIM_RETURN_FROM_CODELET                  0x80
#define __PRIM_BLOCK_DECIPHER_ECB                   0xDA
#define __PRIM_BLOCK_ENCIPHER_ECB                   0xDB
#define __PRIM_GEN_RSA_KEYPAIR                      0xE0

/* The values of each set three MULTOS primitive. */
#define __PRIM_BIT_MANIPULATE_WORD                  0x01
#define __PRIM_CALL_EXTENSION_PRIMITIVE             0x80   /* Range 0x80-0x86 */

/*
**=============================================================================
** MULTOS C-API variables.
**=============================================================================
*/
#define multosProtocolFlags     ProtocolFlags
#define multosProtocolType      ProtocolType
#define multosGetResponseCLA    GetResponseCLA
#define multosGetResponseSW1    GetResponseSW1
#define multosCLA               CLA
#define multosINS               INS
#define multosP1                P1
#define multosP2                P2
#define multosP3                P3
#define multosP1P2              P1P2
#define multosLc                Lc
#define multosLe                Le
#define multosLa                La
#define multosSW1               SW1
#define multosSW2               SW2
#define multosSW12              SW12

/*
**=============================================================================
** MULTOS C-API functions.
**=============================================================================
*/

#define multosPrintf0(fmt)
#define multosPrintf1(fmt, a)
#define multosPrintf2(fmt, a, b)
#define multosPrintf3(fmt, a, b, c)
#define multosPrintf4(fmt, a, b, c, d)

/*
**-----------------------------------------------------------------------------
** void multosAddBCD (const BYTE length,
**                    BYTE *operand1,
**                    BYTE *operand2,
**                    BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosAddBCD (const BYTE length, BYTE *operand1, BYTE *operand2,
                   BYTE *result);
#else
#define multosAddBCD(length, operand1, operand2, result) \
do \
{ \
    __push (__BLOCKCAST(length)(__typechk(BYTE *, operand1))); \
    __push (__BLOCKCAST(length)(__typechk(BYTE *, operand2))); \
    __code (__PRIM, __PRIM_ADD_BCDN, length); \
    __code (__STORE, __typechk(BYTE *, result), length); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosAESECBDecipher (BYTE *cipherText,
**                            BYTE *plainText,
                              BYTE keyLength,
**                            BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosAESECBDecipher (BYTE *cipherText, BYTE *plainText, BYTE keyLength, 
                           BYTE *key);
#else
#define multosAESECBDecipher(cipherText, plainText, keyLength, key) \
do \
{ \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, plainText)); \
    __push (__typechk(BYTE *, cipherText)); \
    __code (__PRIM, __PRIM_AES_ECB_DECIPHER); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosAESECBEncipher (BYTE *cipherText,
**                            BYTE *plainText,
                              BYTE keyLength,
**                            BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosAESECBEncipher (BYTE *plainText, BYTE *cipherText, BYTE keyLength, 
                           BYTE *key);
#else
#define multosAESECBEncipher(plainText, cipherText, keyLength, key) \
do \
{ \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, cipherText)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_AES_ECB_ENCIPHER); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockAdd (const BYTE blockLength,
**                      BYTE *block1,
**                      BYTE *block2,
**                      const BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockAdd (const BYTE blockLength, BYTE *block1, BYTE *block2,
                     const BYTE *result);
#else
#define multosBlockAdd(blockLength, block1, block2, result) \
    __BINARY_OPN (blockLength, __ADDN, result, block1, block2)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockAnd (const BYTE blockLength,
**                      BYTE *block1,
**                      BYTE *block2,
**                      const BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockAnd (const BYTE blockLength, BYTE *block1, BYTE *block2,
                     const BYTE *result);
#else
#define multosBlockAnd(blockLength, block1, block2, result) \
    __BINARY_OPN (blockLength, __ANDN, result, block1, block2)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockCompare (WORD blockLength,
**                          BYTE *block1,
**                          BYTE *block2,
**                          BYTE *result)
**-----------------------------------------------------------------------------
*/
#define MULTOS_BLOCK1_GT_BLOCK2     (0x00)
#define MULTOS_BLOCK2_GT_BLOCK1     (0x08)
#define MULTOS_BLOCK1_EQ_BLOCK2     (0x01)
#ifdef __FUNCTION_PROTOTYPES
void multosBlockCompare (WORD blockLength, BYTE *block1, BYTE *block2,
                         BYTE *result);
#else
#define multosBlockCompare(blockLength, block1, block2, result) \
do \
{ \
    __push (__typechk(WORD, blockLength)); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, block1))); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, block2))); \
    __code (__PRIM, __PRIM_MEMORY_COMPARE); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 0x09); \
    __code (__STORE, __typechk(BOOL *, result), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockCompareFixedLength (BYTE blockLength,
**                                     BYTE *block1,
**                                     BYTE *block2,
**                                     BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockCompareFixedLength (BYTE blockLength, BYTE *block1,
                                    BYTE *block2, BYTE *result);
#else
#define multosBlockCompareFixedLength(blockLength, block1, block2, result) \
do \
{ \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, block1))); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, block2))); \
    __code (__PRIM, __PRIM_MEMORY_COMPARE_FIXED_LENGTH, blockLength); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 0x09); \
    __code (__STORE, __typechk(BOOL *, result), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockClear (const BYTE blockLength,
**                        const BYTE *block)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockClear (const BYTE blockLength, const BYTE *block);
#else
#define multosBlockClear(blockLength, block) \
    __UNARY_OPN (blockLength, __CLEARN, block)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockCopy (WORD blockLength,
**                       BYTE *blockSource,
**                       BYTE *blockDest)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockCopy (WORD blockLength, BYTE *blockSource, BYTE *blockDest);
#else
#define multosBlockCopy(blockLength, blockSource, blockDest) \
do  \
{ \
    __push (__typechk(WORD, blockLength)); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockDest))); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockSource))); \
    __code (__PRIM, __PRIM_MEMORY_COPY); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockCopyFixedLength (BYTE blockLength,
**                                  BYTE *blockSource,
**                                  BYTE *blockDest)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockCopyFixedLength (BYTE blockLength, BYTE *blockSource,
                                 BYTE *blockDest);
#else
#define multosBlockCopyFixedLength(blockLength, blockSource, blockDest) \
do  \
{ \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockDest))); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockSource))); \
    __code (__PRIM, __PRIM_MEMORY_COPY_FIXED_LENGTH, blockLength); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockCopyNonAtomic (WORD blockLength,
**                                BYTE *blockSource,
**                                BYTE *blockDest)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockCopyNonAtomic (WORD blockLength, BYTE *blockSource,
                               BYTE *blockDest);
#else
#define multosBlockCopyNonAtomic(blockLength, blockSource, blockDest) \
do  \
{ \
    __push (__typechk(WORD, blockLength)); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockDest))); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockSource))); \
    __code (__PRIM, __PRIM_MEMORY_COPY_NON_ATOMIC); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockCopyNonAtomicFixedLength (BYTE blockLength,
**                                           BYTE *blockSource,
**                                           BYTE *blockDest)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockCopyNonAtomicFixedLength (BYTE blockLength, BYTE *blockSource,
                                          BYTE *blockDest);
#else
#define multosBlockCopyNonAtomicFixedLength(blockLength, blockSource, blockDest) \
do  \
{ \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockDest))); \
    __push (/*__BLOCKCAST(blockLength)*/(__typechk(BYTE *, blockSource))); \
    __code (__PRIM, __PRIM_MEMORY_COPY_NON_ATOMIC_FIXED_LENGTH, blockLength); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockDecipherCBC (const BYTE algorithm,
**                              WORD inputLength,
**                              BYTE *cipherText,
**                              BYTE *plainText,
**                              BYTE initialValueLength,
**                              BYTE *initialValue,
**                              BYTE keyLength,
**                              BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockDecipherCBC (const BYTE algorithm, WORD inputLength,
                             BYTE *cipherText, BYTE *plainText, 
                             BYTE initialValueLength, BYTE *initialValue,
                             BYTE keyLength, BYTE *key);
#else
#define multosBlockDecipherCBC(algorithm, inputLength, cipherText, plainText, initialValueLength, initialValue, keyLength, key) \
do \
{ \
    __push (__typechk(BYTE, initialValueLength)); \
    __push (__typechk(BYTE *, initialValue)); \
    __push (__typechk(WORD, inputLength)); \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, plainText)); \
    __push (__typechk(BYTE *, cipherText)); \
    __code (__PRIM, __PRIM_BLOCK_DECIPHER_ECB, algorithm, 2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockDecipherECB (const BYTE algorithm,
**                              WORD inputLength,
**                              BYTE *cipherText,
**                              BYTE *plainText,
**                              BYTE keyLength,
**                              BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockDecipherECB (const BYTE algorithm, WORD inputLength,
                             BYTE *cipherText, BYTE *plainText, BYTE keyLength,
                             BYTE *key);
#else
#define multosBlockDecipherECB(algorithm, inputLength, cipherText, plainText, keyLength, key) \
do \
{ \
    __push (__typechk(WORD, inputLength)); \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, plainText)); \
    __push (__typechk(BYTE *, cipherText)); \
    __code (__PRIM, __PRIM_BLOCK_DECIPHER_ECB, algorithm, 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockDecrement (const BYTE blockLength,
**                            const BYTE *block)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockDecrement (const BYTE blockLength, const BYTE *block);
#else
#define multosBlockDecrement(blockLength, block) \
    __UNARY_OPN (blockLength, __DECN, block);
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockDivide (const BYTE blockLength,
**                         BYTE *numerator,
**                         BYTE *denominator,
**                         BYTE *quotient,
**                         BYTE *remainder)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockDivide (const BYTE blockLength, BYTE *numerator,
                        BYTE *denominator, BYTE *quotient, BYTE *remainder);
#else
#define multosBlockDivide(blockLength, numerator, denominator, quotient, remainder) \
do  \
{ \
    __push (__BLOCKCAST(blockLength)(__typechk(BYTE *, numerator))); \
    __push (__BLOCKCAST(blockLength)(__typechk(BYTE *, denominator))); \
    __code (__PRIM, __PRIM_DIVIDEN, blockLength); \
    __code (__STORE, __typechk(BYTE *, remainder), blockLength); \
    __code (__STORE, __typechk(BYTE *, quotient), blockLength);     \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockEncipherCBC (const BYTE algorithm,
**                              WORD blockLength,
**                              BYTE *plainText,
**                              BYTE *cipherText,
**                              BYTE initialValueLength,
**                              BYTE *initialValue,
**                              BYTE keyLength,
**                              BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockEncipherCBC (const BYTE algorithm, WORD inputLength,
                             BYTE *plainText, BYTE *cipherText, 
                             BYTE initialValueLength, BYTE *initialValue,
                             BYTE keyLength, BYTE *key);
#else
#define multosBlockEncipherCBC(algorithm, inputLength, plainText, cipherText, initialValueLength, initialValue, keyLength, key) \
do \
{ \
    __push (__typechk(BYTE, initialValueLength)); \
    __push (__typechk(BYTE *, initialValue)); \
    __push (__typechk(WORD, inputLength)); \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, cipherText)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_BLOCK_ENCIPHER_ECB, algorithm, 2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockEncipherECB (const BYTE algorithm,
**                              WORD blockLength,
**                              BYTE *plainText,
**                              BYTE *cipherText,
**                              BYTE keyLength,
**                              BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockEncipherECB (const BYTE algorithm, WORD inputLength,
                             BYTE *plainText, BYTE *cipherText, BYTE keyLength,
                             BYTE *key);
#else
#define multosBlockEncipherECB(algorithm, inputLength, plainText, cipherText, keyLength, key) \
do \
{ \
    __push (__typechk(WORD, inputLength)); \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, cipherText)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_BLOCK_ENCIPHER_ECB, algorithm, 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockIncrement (const BYTE blockLength,
**                            const BYTE *block)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockIncrement (const BYTE blockLength, const BYTE *block);
#else
#define multosBlockIncrement(blockLength, block) \
    __UNARY_OPN (blockLength, __INCN, block);
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockInvert (const BYTE blockLength,
**                         const BYTE *block)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockInvert (const BYTE blockLength, const BYTE *block);
#else
#define multosBlockInvert(blockLength, block) \
    __UNARY_OPN (blockLength, __NOTN, block);
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockLookup (BYTE value,
**                         BYTE *block,
**                         BYTE *result,
**                         BOOL *byteFound)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockLookup (BYTE value, BYTE *block, BYTE *result,
                        BOOL *byteFound);
#else
#define multosBlockLookup(value, block, result, byteFound) \
do  \
{ \
    __push (__typechk(BYTE, value)); \
    __push (__typechk(BYTE *, block)); \
    __code (__PRIM, __PRIM_LOOKUP); \
    __code (__STORE, __typechk(BYTE *, result), 1); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 0x01); \
    __code (__STORE, __typechk(BOOL *, byteFound), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockMultiply (const BYTE blockLength,
**                           BYTE *block1,
**                           BYTE *block2,
**                           BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockMultiply (const BYTE blockLength, BYTE *block1, BYTE *block2,
                          BYTE *result);
#else
#define multosBlockMultiply(blockLength, block1, block2, result) \
do \
{ \
    __push (__BLOCKCAST(blockLength)(__typechk(BYTE *, block1))); \
    __push (__BLOCKCAST(blockLength)(__typechk(BYTE *, block2))); \
    __code (__PRIM, __PRIM_MULTIPLYN, blockLength); \
    __code (__STORE, __typechk(BYTE *, result), blockLength*2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockOr (const BYTE blockLength,
**                     BYTE *block1,
**                     BYTE *block2,
**                     BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockOr (const BYTE blockLength, BYTE *block1, BYTE *block2,
                    BYTE *result);
#else
#define multosBlockOr(blockLength, block1, block2, result) \
    __BINARY_OPN (blockLength, __ORN, result, block1, block2)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockShiftLeft (const BYTE blockLength,
**                            const BYTE numShiftBits,
**                            BYTE *blockSource,
**                            BYTE *blockDest)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockShiftLeft (const BYTE blockLength, const BYTE numShiftBits,
                           BYTE *blockSource, BYTE *blockDest);
#else
#define multosBlockShiftLeft(blockLength, numShiftBits, blockSource, blockDest) \
do \
{ \
    __push (__BLOCKCAST(blockLength)(__typechk(BYTE *, blockSource))); \
    __code (__PRIM, __PRIM_SHIFT_LEFT, blockLength, numShiftBits); \
    __code (__STORE, __typechk(BYTE *, blockDest), blockLength); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockShiftRight (const BYTE blockLength,
**                             const BYTE numShiftBits,
**                             BYTE *blockSource,
**                             BYTE *blockDest)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockShiftRight (const BYTE blockLength, const BYTE numShiftBits,
                            BYTE *blockSource, BYTE *blockDest);
#else
#define multosBlockShiftRight(blockLength, numShiftBits, blockSource, blockDest) \
do \
{ \
    __push (__BLOCKCAST(blockLength)(__typechk(BYTE *, blockSource))); \
    __code (__PRIM, __PRIM_SHIFT_RIGHT, blockLength, numShiftBits); \
    __code (__STORE, __typechk(BYTE *, blockDest), blockLength); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockSubtract (const BYTE blockLength,
**                          BYTE *block1,
**                          BYTE *block2,
**                          const BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockSubtract (const BYTE blockLength, BYTE *block1, BYTE *block2,
                         const BYTE *result);
#else
#define multosBlockSubtract(blockLength, block1, block2, result) \
    __BINARY_OPN (blockLength, __SUBN, result, block1, block2)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockTestZero (const BYTE blockLength,
**                           const BYTE *block,
**                           BOOL *isZero)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockTestZero (const BYTE blockLength, const BYTE *block,
                          BOOL *isZero);
#else
#define multosBlockTestZero(blockLength, block, isZero) \
do  \
{ \
    __UNARY_OPN (blockLength, __TESTN, block) \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 0x01); \
    __code (__STORE, __typechk(BOOL *, isZero), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosBlockXor (const BYTE blockLength,
**                      BYTE *block1,
**                      BYTE *block2,
**                      BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockXor (const BYTE blockLength, BYTE *block1, BYTE *block2,
                     BYTE *result);
#else
#define multosBlockXor(blockLength, block1, block2, result) \
    __BINARY_OPN (blockLength, __XORN, result, block1, block2)
#endif

/*
**-----------------------------------------------------------------------------
** void multosCallCodelet (WORD codeletID,
**                         WORD entryAddress)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosCallCodelet (WORD codeletID, WORD entryAddress);
#else
#define multosCallCodelet(codeletID, entryAddress) \
do \
{ \
    __push (__typechk(WORD, codeletID)); \
    __push (__typechk(WORD, entryAddress)); \
    __code (__PRIM, __PRIM_CALL_CODELET); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosCallExtensionPrimitive (const BYTE extensionNum,
**                                    const BYTE primTypeLo,
**                                    const BYTE primTypeHi,
**                                    const BYTE paramByte)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosCallExtensionPrimitive (const BYTE extensionNum,
                                   const BYTE primTypeLo, const BYTE primTypeHi,
                                   const BYTE paramByte);
#else
#define multosCallExtensionPrimitive(extensionNum, primTypeLo, primTypeHi, paramByte) \
    __code (__PRIM, __PRIM_CALL_EXTENSION_PRIMITIVE+extensionNum, primTypeLo, primTypeHi, paramByte)
#endif

/*
**-----------------------------------------------------------------------------
** void multosCardBlock (const BYTE offsetMAChi,
**                       const BYTE offsetMAClo,
**                       BOOL *cardBlockedOk)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosCardBlock (const BYTE offsetMAChi, const BYTE offsetMAClo,
                      BOOL *cardBlockedOk);
#else
#define multosCardBlock(offsetMAChi, offsetMAClo, cardBlockedOk) \
do \
{ \
    __code (__PRIM, __PRIM_CARD_BLOCK, offsetMAChi, offsetMAClo); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
    __code (__STORE, __typechk(BOOL *, cardBlockedOk), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** BOOL multosCheckCase (BYTE isoCase)
**-----------------------------------------------------------------------------
*/
BOOL CheckCase (BYTE isocase);
#ifdef __FUNCTION_PROTOTYPES
BOOL multosCheckCase (BYTE isoCase);
#else
#define multosCheckCase(isoCase) (BOOL)CheckCase(isoCase)
#endif

/*
**-----------------------------------------------------------------------------
** void multosChecksum (WORD blockLength,
**                      BYTE *block,
**                      DWORD *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosChecksum (WORD blockLength, BYTE *block, DWORD *result);
#else
#define multosChecksum(blockLength, block, result) \
do \
{ \
    __push (__typechk(WORD, blockLength)); \
    __push (__typechk(BYTE *, block)); \
    __code (__PRIM, __PRIM_CHECKSUM); \
    __code (__STORE, __typechk(DWORD *, result), 4); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosControlAutoResetWWT (BOOL disable)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosControlAutoResetWWT (BOOL disable);
#else
#define multosControlAutoResetWWT(disable) \
do \
{ \
    __push (__typechk(BOOL, disable)); \
    __code (__PRIM, __PRIM_CONTROL_AUTO_RESET_WWT); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosConvertBCD (BYTE mode)
**-----------------------------------------------------------------------------
*/
#define MULTOS_BCD_TO_BIN   (0x00)
#define MULTOS_BIN_TO_BCD   (0x01)
#ifdef __FUNCTION_PROTOTYPES
void multosConvertBCD (BYTE mode, BYTE srcLength, BYTE destLength, BYTE *src,
                       BYTE *dest, BOOL *convertedOK);
#else
#define multosConvertBCD(mode, srcLength, destLength, src, dest, convertedOK) \
do \
{ \
    __push (__typechk(BYTE, destLength)); \
    __push (__typechk(BYTE, srcLength)); \
    __push (__typechk(BYTE *, dest)); \
    __push (__typechk(BYTE *, src)); \
    __code (__PRIM, __PRIM_CONVERT_BCD, mode); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
    __code (__STORE, __typechk(BOOL *, convertedOK), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosDelegate (BYTE *aid)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosDelegate (BYTE *aid);
#else
#define multosDelegate(aid) \
do \
{\
    __push (__typechk(BYTE *, aid)); \
    __code (__PRIM, __PRIM_DELEGATE); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosDESECBDecipher (BYTE *cipherText,
**                            BYTE *plainText,
**                            BYTE key[8])
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosDESECBDecipher (BYTE *cipherText, BYTE *plainText, BYTE key[8]);
#else
#define multosDESECBDecipher(cipherText, plainText, key) \
do \
{ \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE *, plainText)); \
    __push (__typechk(BYTE *, cipherText));    \
    __code (__PRIM, __PRIM_DES_ECB_DECIPHER); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosDESECBEncipher (BYTE *plainText,
**                            BYTE *cipherText,
**                            BYTE key[8])
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosDESECBEncipher (BYTE *plainText, BYTE *cipherText, BYTE key[8]);
#else
#define multosDESECBEncipher(plainText, cipherText, key) \
do \
{ \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE *, cipherText)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_DES_ECB_ENCIPHER); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosExchangeData (BYTE channelID,
**                          BYTE *data)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosExchangeData (BYTE channelID, BYTE *data);
#else
#define multosExchangeData(channelID, data) \
do \
{ \
    __push (__typechk(BYTE, channelID)); \
    __push (__typechk(BYTE *, data)); \
    __code (__PRIM, __PRIM_EXCHANGE_DATA); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosExit (void)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosExit (void);
#else
#define multosExit() \
    __code (__SYSTEM, 4)
#endif

/*
**-----------------------------------------------------------------------------
** void multosExitLa (const BYTE la)
**-----------------------------------------------------------------------------
*/
/*#ifdef __FUNCTION_PROTOTYPES
void multosExitLa (const BYTE la);
#else
#define multosExitLa(la) \
    __code (__SYSTEM, 6, la)
#endif
*/
/*
**-----------------------------------------------------------------------------
** void multosExitSW (const WORD sw)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosExitSW (const WORD sw);
#else
#define multosExitSW(sw) \
    __code (__SYSTEM, 5, sw)
#endif

/*
**-----------------------------------------------------------------------------
** void multosExitSWLa (const WORD sw,
**                      const BYTE la)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosExitSWLa (const WORD sw, const BYTE la);
#else
#define multosExitSWLa(sw, la) \
  __code (__SYSTEM, 7, sw, la)
#endif

/*
**-----------------------------------------------------------------------------
** void multosExitToMultosAndRestart (void)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosExitToMultosAndRestart (void);
#else
#define multosExitToMultosAndRestart() \
    __code (__PRIM, __PRIM_EXIT_TO_MULTOS_AND_RESTART)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGenerateAsymmetricHash (WORD plainTextLength,
**                                    BYTE *plainText,
**                                    BYTE hash[16])
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGenerateAsymmetricHash (WORD plainTextLength, BYTE *plainText,
                                   BYTE hash[16]);
#else
#define multosGenerateAsymmetricHash(plainTextLength, plainText, hash) \
do \
{ \
    __push (__typechk(WORD, plainTextLength)); \
    __push (__typechk(BYTE *, hash)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_GENERATE_ASYMMETRIC_HASH, 0); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGenerateAsymmetricHashIV (BYTE initialValue[16],
**                                      WORD plainTextlength,
**                                      BYTE *plainText,
**                                      BYTE hash[16])
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGenerateAsymmetricHashIV (BYTE initialValue[16],
                                     WORD plainTextlength, BYTE *plainText,
                                     BYTE hash[16]);
#else
#define multosGenerateAsymmetricHashIV(initialValue, plainTextlength, plainText, hash) \
do \
{ \
    __push (__typechk(BYTE *, initialValue)); \
    __push (__typechk(WORD, plainTextlength)); \
    __push (__typechk(BYTE *, hash)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_GENERATE_ASYMMETRIC_HASH, 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGenerateDESCBCSignature (WORD plainTextLength,
**                                     BYTE *plainText,
**                                     BYTE initialValue[8],
**                                     BYTE key[8],
**                                     BYTE signature[8])
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGenerateDESCBCSignature (WORD plainTextLength, BYTE *plainText,
                                    BYTE initialValue[8], BYTE key[8],
                                    BYTE signature[8]);
#else
#define multosGenerateDESCBCSignature(plainTextLength, plainText, initialValue, key, signature) \
do \
{ \
    __push (__typechk(WORD, plainTextLength)); \
    __push (__typechk(BYTE *, initialValue)); \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE *, signature)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_GENERATE_DES_CBC_SIGNATURE); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGenerateTripleDESCBCSignature (WORD plainTextLength,
**                                           BYTE *plainText,
**                                           BYTE initialValue[8],
**                                           BYTE keys[16],
**                                           BYTE signature[8])
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGenerateTripleDESCBCSignature (WORD plainTextLength, BYTE *plainText,
                                          BYTE initialValue[8], BYTE keys[16],
                                          BYTE signature[8]);
#else
#define multosGenerateTripleDESCBCSignature(plainTextLength, plainText, initialValue, keys, signature) \
do \
{ \
    __push (__typechk(WORD, plainTextLength)); \
    __push (__typechk(BYTE *, initialValue)); \
    __push (__typechk(BYTE *, keys)); \
    __push (__typechk(BYTE *, signature)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_GENERATE_TRIPLE_DES_CBC_SIGNATURE); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetData (const BYTE numDataBytes,
**                     BYTE *numCopied,
**                     BYTE *output)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetData (const BYTE numDataBytes, BYTE *numCopied, BYTE *output);
#else
#define multosGetData(numDataBytes, numCopied, output) \
do \
{ \
    __push (__typechk(BYTE *, output));     \
    __code (__PRIM, __PRIM_GET_DATA, numDataBytes); \
    __code (__STORE, __typechk(BYTE *, numCopied), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetDelegatorAID (const WORD aidLength,
**                             BYTE *aid,
**                             BOOL *notADelegate)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetDelegatorAID (const BYTE aidLength, BYTE *aid,
                            BOOL *notADelegate);
#else
#define multosGetDelegatorAID(aidLength, aid, notADelegate) \
do  \
{ \
    __push (__typechk(BYTE *, aid)); \
    __code (__PRIM, __PRIM_GET_DELEGATOR_AID, aidLength); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BYTE *, notADelegate), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetDIRFileRecord (const BYTE dirRecordLength,
**                              BYTE recordNum,
**                              BYTE *numCopied,
**                              BYTE *output,
**                              BOOL *noRecordFound)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetDIRFileRecord (const BYTE dirRecordLength, BYTE recordNum,
                             BYTE *numCopied, BYTE *output,
                             BOOL *noRecordFound);
#else
#define multosGetDIRFileRecord(dirRecordLength, recordNum, numCopied, output, noRecordFound) \
do \
{ \
    __push (__typechk(BYTE *, output)); \
    __push (__typechk(BYTE, recordNum)); \
    __code (__PRIM, __PRIM_GET_DIR_FILE_RECORD, dirRecordLength); \
    __code (__STORE, __typechk(BYTE *, numCopied), 1); \
    __code (__POPN, 3);    \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BYTE *, noRecordFound), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetFileControlInformation (const BYTE fciRecordLength,
**                                       BYTE recordNum,
**                                       BYTE *numCopied,
**                                       BYTE *output,
**                                       BOOL *noRecordFound)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetFileControlInformation (const BYTE fciRecordLength,
                                      BYTE recordNum, BYTE *numCopied,
                                      BYTE *output, BOOL *noRecordFound);
#else
#define multosGetFileControlInformation(fciRecordLength, recordNum, numCopied, output, noRecordFound) \
do \
{ \
    __push (__typechk(BYTE *, output)); \
    __push (__typechk(BYTE, recordNum)); \
    __code (__PRIM, __PRIM_GET_FILE_CONTROL_INFORMATION, fciRecordLength); \
    __code (__STORE, __typechk(BYTE *, numCopied), 1); \
    __code (__POPN, 3); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BYTE *, noRecordFound), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetManufacturerData (const BYTE numDataBytes,
**                                 BYTE *numCopied,
**                                 BYTE *output)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetManufacturerData (const BYTE numDataBytes, BYTE *numCopied,
                                BYTE *output);
#else
#define multosGetManufacturerData(numDataBytes, numCopied, output) \
do \
{ \
    __push (__typechk(BYTE *, output)); \
    __code (__PRIM, __PRIM_GET_MANUFACTURER_DATA, numDataBytes); \
    __code (__STORE, __typechk(BYTE *, numCopied), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetMemoryReliability (BYTE *result)
**-----------------------------------------------------------------------------
*/
#define MULTOS_MEMORY_RELIABLE     (0)
#define MULTOS_MEMORY_MARGINAL     (1)
#define MULTOS_MEMORY_UNRELIABLE   (8)
#ifdef __FUNCTION_PROTOTYPES
void multosGetMemoryReliability (BYTE *result);
#else
#define multosGetMemoryReliability(result) \
do \
{ \
    __code (__PRIM, __PRIM_GET_MEMORY_RELIABILITY); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x09, 1); \
    __code (__STORE, __typechk(BYTE *, result), 1);  \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetMultosData (const BYTE numDataBytes,
**                           BYTE *numCopied,
**                           BYTE *output)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetMultosData (const BYTE numDataBytes, BYTE *numCopied,
                          BYTE *output);
#else
#define multosGetMultosData(numDataBytes, numCopied, output) \
do \
{ \
    __push (__typechk(BYTE *, output));    \
    __code (__PRIM, __PRIM_GET_MULTOS_DATA, numDataBytes); \
    __code (__STORE, __typechk(BYTE *, numCopied), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetRandomNumber (BYTE result[8])
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetRandomNumber (BYTE result[8]);
#else
#define multosGetRandomNumber(result) \
do \
{ \
  __code (__PRIM, __PRIM_GET_RANDOM_NUMBER); \
  __code (__STORE, __typechk(BYTE *, result), 8); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosIndex (BYTE blockLength,
**                   BYTE index,
**                   BYTE *baseAddr,
**                   BYTE *resultData)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosIndex (BYTE blockLength, BYTE index, BYTE *baseAddr,
                  BYTE *resultData);
#else
#define multosIndex(blockLength, index, baseAddr, resultData) \
do \
{ \
    __push (__typechk (BYTE, index)); \
    __code (__INDEX, baseAddr, blockLength); \
    __code (__LOADI, blockLength); \
    __code (__STORE, __typechk(BYTE *, resultData), blockLength); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosModularExponentiation (WORD exponentLength,
**                                   WORD modulusLength,
**                                   BYTE *exponent,
**                                   BYTE *modulus,
**                                   BYTE *input,
**                                   BYTE *output)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosModularExponentiation (WORD exponentLength, WORD modulusLength,
                                  BYTE *exponent, BYTE *modulus, BYTE *input,
                                  BYTE *output);
#else
#define multosModularExponentiation(exponentLength, modulusLength, exponent, modulus, input, output) \
do \
{ \
    __push (__typechk(WORD, exponentLength)); \
    __push (__typechk(WORD, modulusLength)); \
    __push (__typechk(BYTE *, exponent)); \
    __push (__typechk(BYTE *, modulus)); \
    __push (__typechk(BYTE *, input)); \
    __push (__typechk(BYTE *, output)); \
    __code (__PRIM, __PRIM_MODULAR_EXPONENTIATION); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosModularExponentiationCRT (WORD dpdqLength,
**                                      BYTE *dpdq,
**                                      BYTE *pqu,
**                                      BYTE *input,
**                                      BYTE *output)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosModularExponentiationCRT (WORD dpdqLength, BYTE *dpdq, BYTE *pqu,
                                     BYTE *input, BYTE *output);
#else
#define multosModularExponentiationCRT(dpdqLength, dpdq, pqu, input, output) \
do \
{ \
    __push (__typechk(WORD, dpdqLength)); \
    __push (__typechk(BYTE *, dpdq)); \
    __push (__typechk(BYTE *, pqu)); \
    __push (__typechk(BYTE *, input)); \
    __push (__typechk(BYTE *, output));    \
    __code (__PRIM, __PRIM_MODULAR_EXPONENTIATION_CRT); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosModularExponentiationCRTProtected (WORD dpdqLength,
**                                               BYTE *dpdq,
**                                               BYTE *pqu,
**                                               BYTE *input,
**                                               BYTE *output)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosModularExponentiationCRTProtected (WORD dpdqLength, BYTE *dpdq,
                                              BYTE *pqu, BYTE *input,
                                              BYTE *output);
#else
#define multosModularExponentiationCRTProtected(dpdqLength, dpdq, pqu, input, output) \
do \
{ \
    __push (__typechk(WORD, dpdqLength)); \
    __push (__typechk(BYTE *, dpdq)); \
    __push (__typechk(BYTE *, pqu)); \
    __push (__typechk(BYTE *, input)); \
    __push (__typechk(BYTE *, output));    \
    __code (__PRIM, __PRIM_MODULAR_EXPONENTIATION_CRT_PROTECTED); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosModularMultiplication (WORD modulusLength,
**                                   BYTE *modulus,
**                                   BYTE *block1,
**                                   BYTE *block2)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosModularMultiplication (WORD modulusLength, BYTE *modulus,
                                  BYTE *block1, BYTE *block2);
#else
#define multosModularMultiplication(modulusLength, modulus, block1, block2) \
do \
{ \
    __push (__typechk(WORD, modulusLength)); \
    __push (__typechk(BYTE *, block1)); \
    __push (__typechk(BYTE *, block2)); \
    __push (__typechk(BYTE *, modulus)); \
    __code (__PRIM, __PRIM_MODULAR_MULTIPLICATION); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosModularReduction (WORD operandLength,
**                              WORD modulusLength,
**                              BYTE *operand,
**                              BYTE *modulus)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosModularReduction (WORD operandLength, WORD modulusLength,
                             BYTE *operand, BYTE *modulus);
#else
#define multosModularReduction(operandLength, modulusLength, operand, modulus) \
do \
{ \
    __push (__typechk(WORD, operandLength)); \
    __push (__typechk(WORD, modulusLength)); \
    __push (__typechk(BYTE *, operand)); \
    __push (__typechk(BYTE *, modulus)); \
    __code (__PRIM, __PRIM_MODULAR_REDUCTION); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosPlatformOptimisedChecksum (WORD blockLength,
**                                       BYTE *block,
**                                       DWORD *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosPlatformOptimisedChecksum (WORD blockLength, BYTE *block,
                                      DWORD *result);
#else
#define multosPlatformOptimisedChecksum(blockLength, block, result) \
do \
{ \
    __push (__typechk(WORD, blockLength)); \
    __push (__typechk(BYTE *, block)); \
    __code (__PRIM, __PRIM_PLATFORM_OPTIMISED_CHECKSUM); \
    __code (__STORE, __typechk(DWORD *, result), 4); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosQueryInterfaceType (BOOL *isContactless)
**
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosQueryInterfaceType (BOOL *isContactless);
#else
#define multosQueryInterfaceType(isContactless) \
do \
{ \
    __code (__PRIM, __PRIM_QUERY_INTERFACE_TYPE); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BOOL *, isContactless), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosQueryChannel (BYTE channelID,
**                          BOOL *channelSupported)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosQueryChannel (BYTE channelID, BOOL *channelSupported);
#else
#define multosQueryChannel(channelID, channelSupported) \
do \
{ \
    __push (__typechk(BYTE, channelID)); \
    __code (__PRIM, __PRIM_QUERY_CHANNEL); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BOOL *, channelSupported), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosQueryCodelet (WORD codeletID,
**                          BOOL *codeletSupported)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosQueryCodelet (WORD codeletID, BOOL *codeletSupported);
#else
#define multosQueryCodelet(codeletID, codeletSupported) \
do \
{ \
    __push (__typechk(WORD, codeletID)); \
    __code (__PRIM, __PRIM_QUERY_CODELET); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BOOL *, codeletSupported), 1); \
} while (0);
#endif

/*
**-----------------------------------------------------------------------------
** void multosQueryPrimitive (const BYTE setNum,
**                            const BYTE primitiveNum,
**                            BOOL *primitiveSupported)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosQueryPrimitive (const BYTE setNum, const BYTE primitiveNum,
                           BOOL *primitiveSupported);
#else
#define multosQueryPrimitive(setNum, primitiveNum, primitiveSupported) \
do  \
{ \
    __code (__PRIM, __PRIM_QUERY + setNum, primitiveNum); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BOOL *, primitiveSupported), 1); \
} while (0);
#endif

/*
**-----------------------------------------------------------------------------
** void multosResetSessionData (void)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosResetSessionData (void);
#else
#define multosResetSessionData() \
  __code (__PRIM, __PRIM_RESET_SESSION_DATA)
#endif

/*
**-----------------------------------------------------------------------------
** void multosResetWWT (void)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosResetWWT (void);
#else
#define multosResetWWT() \
    __code (__PRIM, __PRIM_RESET_WWT)
#endif

/*
**-----------------------------------------------------------------------------
** void multosReturnFromCodelet (const BYTE numBytesIn,
**                               const BYTE numBytesOut)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosReturnFromCodelet (const BYTE numBytesIn, const BYTE numBytesOut);
#else
#define multosReturnFromCodelet(numBytesIn, numBytesOut) \
    __code (__PRIM, __PRIM_RETURN_FROM_CODELET, numBytesIn, numBytesOut)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetATRFileRecord (BYTE *record,
**                              BYTE *numBytesWritten)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetATRFileRecord (BYTE *record, BYTE *numBytesWritten);
#else
#define multosSetATRFileRecord(record, numBytesWritten) \
do \
{ \
    __push (__typechk(BYTE *, record)); \
    __code (__PRIM, __PRIM_SET_ATR_FILE_RECORD); \
    __code (__STORE, __typechk(BYTE *, numBytesWritten), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetATRHistoricalCharacters (BYTE *input,
**                                        BYTE *numBytesWritten)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetATRHistoricalCharacters (BYTE *input, BYTE *numBytesWritten);
#else
#define multosSetATRHistoricalCharacters(input, numBytesWritten) \
do \
{ \
    __push (__typechk(BYTE *, input)); \
    __code (__PRIM, __PRIM_SET_ATR_HISTORICAL_CHARACTERS); \
    __code (__STORE, __typechk(BYTE *, numBytesWritten), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetProtectedMemoryAccess (const BYTE options)
**-----------------------------------------------------------------------------
*/
#define MULTOS_PROTECTED_MEMORY_ACCESS_OFF  (0)
#define MULTOS_PROTECTED_MEMORY_ACCESS_ON   (1)
#ifdef __FUNCTION_PROTOTYPES
void multosSetProtectedMemoryAccess (const BYTE options);
#else
#define multosSetProtectedMemoryAccess(options) \
    __code(__PRIM, __PRIM_SET_PROTECTED_MEMORY_ACCESS, options)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetTransactionProtection (const BYTE options)
**-----------------------------------------------------------------------------
*/
#define MULTOS_TP_OFF_AND_DISCARD   (0)
#define MULTOS_TP_OFF_AND_COMMIT    (1)
#define MULTOS_TP_ON_AND_DISCARD    (2)
#define MULTOS_TP_ON_AND_COMMIT     (3)
#ifdef __FUNCTION_PROTOTYPES
void multosSetTransactionProtection (const BYTE options);
#else
#define multosSetTransactionProtection(options) \
    __code(__PRIM, __PRIM_SET_TRANSACTION_PROTECTION, options)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetSelectSW (const BYTE sw1,
**                         const BYTE sw2)
**-----------------------------------------------------------------------------
*/
/* Set SELECT SW codes */
#ifdef __FUNCTION_PROTOTYPES
void multosSetSelectSW (const BYTE sw1, const BYTE sw2);
#else
#define multosSetSelectSW(sw1, sw2) \
    __code (__PRIM, __PRIM_SET_SELECT_SW, sw1, sw2)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSHA1 (WORD messageLength,
**                  BYTE *message,
**                  BYTE *hash)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSHA1 (WORD messageLength, BYTE *message, BYTE *hash);
#else
#define multosSHA1(messageLength, message, hash) \
do \
{ \
    __push (__typechk(WORD, messageLength)); \
    __push (__typechk(BYTE *, hash)); \
    __push (__typechk(BYTE *, message)); \
    __code (__PRIM, __PRIM_SHA_1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSubtractBCD (const BYTE length,
**                         BYTE *operand1,
**                         BYTE *operand2,
**                         BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSubtractBCD (const BYTE length, BYTE *operand1, BYTE *operand2,
                        BYTE *result);
#else
#define multosSubtractBCD(length, operand1, operand2, result) \
do \
{ \
    __push (__BLOCKCAST(length)(__typechk(BYTE *, operand1))); \
    __push (__BLOCKCAST(length)(__typechk(BYTE *, operand2))); \
    __code (__PRIM, __PRIM_SUBTRACT_BCDN, length); \
    __code (__STORE, __typechk(BYTE *, result), length); \
} while (0)
#endif

/*--------------------- MULTOS 4.3 and 4.3.1 additions -----------------------*/
/*
**-----------------------------------------------------------------------------
** void multosQueryCryptographicAlgorithm (BYTE algoId,
**                                         BOOL *isSupported)
** algoId as defined by ALGORITHM_DES etc.
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosQueryCryptographicAlgorithm (BYTE algoId,BOOL *isSupported);
#else
#define multosQueryCryptographicAlgorithm(algoId, isSupported) \
do \
{ \
    __push (__typechk(BYTE, algoId)); \
    __code (__PRIM, __PRIM_QUERY_ALGO); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 1); \
    __code (__STORE, __typechk(BOOL *, isSupported), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSecureHash (WORD msgLen,
**                        WORD hashLen,
**                        BYTE *hashOut,
**                        BYTE *msgIn)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSecureHash (WORD msgLen, WORD hashLen, BYTE *hashOut, BYTE *msgIn);
#else
#define multosSecureHash(msgLen, hashLen, hashOut, msgIn) \
do \
{ \
    __push (__typechk(WORD, msgLen)); \
    __push (__typechk(WORD, hashLen)); \
    __push (__typechk(BYTE *, hashOut)); \
    __push (__typechk(BYTE *, msgIn)); \
    __code (__PRIM, __PRIM_SECURE_HASH); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosTripleDESECBDecipher (BYTE *cipherText,
**                            BYTE *plainText,
**                            BYTE keyLength,
**                            BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosTripleDESECBDecipher (BYTE *cipherText, BYTE *plainText, BYTE keyLength, BYTE *key);
#else
#define multosTripleDESECBDecipher(cipherText, plainText, keyLength, key) \
do \
{ \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, plainText)); \
    __push (__typechk(BYTE *, cipherText)); \
    __code (__PRIM, __PRIM_3DES_ECB_DECIPHER); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosTripleDESECBEncipher (BYTE *cipherText,
**                            BYTE *plainText,
**                            BYTE keyLength,
**                            BYTE *key)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosTripleDESECBEncipher (BYTE *plainText, BYTE *cipherText, BYTE keyLength, BYTE *key);
#else
#define multosTripleDESECBEncipher(plainText, cipherText, keyLength, key) \
do \
{ \
    __push (__typechk(BYTE *, key)); \
    __push (__typechk(BYTE, keyLength)); \
    __push (__typechk(BYTE *, cipherText)); \
    __push (__typechk(BYTE *, plainText)); \
    __code (__PRIM, __PRIM_3DES_ECB_ENCIPHER); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosPad (WORD msgLen,
**           BYTE *msg,
**           BYTE blockLen,
**           WORD *paddedMsgLen,
**           const BYTE padMethod)
** padMethod = 0x01 : 0x80 followed by zero or more 0x00
** padMethod = 0x02 : 0x80 followed by one or more 0x00
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosPad (WORD msgLen,BYTE *msg,BYTE blockLen,WORD *paddedMsgLen,const BYTE padMethod);
#else
#define multosPad(msgLen, msg, blockLen, paddedMsgLen, padMethod) \
do \
{ \
    __push (__typechk(BYTE, blockLen)); \
    __push (__typechk(WORD, msgLen)); \
    __push (__typechk(BYTE *, msg)); \
    __code (__PRIM, __PRIM_PAD, padMethod); \
    __code (__STORE, __typechk(WORD *, paddedMsgLen), 2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosUnPad (WORD msgLen,
**           BYTE *msg,
**           WORD *unpaddedMsgLen,
**           const BYTE padMethod)
** padMethod = 0x01 : 0x80 followed by zero or more 0x00
** padMethod = 0x02 : 0x80 followed by one or more 0x00
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosUnPad (WORD msgLen,BYTE *msg,WORD *unpaddedMsgLen,const BYTE padMethod);
#else
#define multosUnPad(msgLen, msg, unpaddedMsgLen, padMethod) \
do \
{ \
    __push (__typechk(WORD, msgLen)); \
    __push (__typechk(BYTE *, msg)); \
    __code (__PRIM, __PRIM_UNPAD, padMethod); \
    __code (__STORE, __typechk(WORD *, unpaddedMsgLen), 2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetSilentMode (const BYTE mode)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetSilentMode (const BYTE mode);
#else
#define multosSetSilentMode(mode) \
do \
{ \
    __code (__PRIM, __PRIM_SET_SILENT_MODE, mode); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosCardUnBlock (BOOL *cardUnBlockedOk)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosCardUnBlock (BOOL *cardUnBlockedOk);
#else
#define multosCardUnBlock(cardUnBlockedOk) \
do \
{ \
    __code (__PRIM, __PRIM_CARD_UNBLOCK); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
    __code (__STORE, __typechk(BOOL *, cardUnBlockedOk), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosDeactivateAcceleratedRead (BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosDeactivateAcceleratedRead (BOOL *success);
#else
#define multosDeactivateAcceleratedRead(success) \
do \
{ \
    __code (__PRIM, __PRIM_CONFIG_READ_BINARY, 0x00); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
    __code (__STORE, __typechk(BOOL *, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosAcceleratedReadNoBAC (BYTE channel, BYTE* dataAddr, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosAcceleratedReadNoBAC (BYTE channel, BYTE* dataAddr, BOOL *success);
#else
#define multosAcceleratedReadNoBAC(channel, dataAddr, success) \
do \
{ \
    __push (__typechk(BYTE, channel)); \
    __push (__typechk(BYTE*, dataAddr)); \
    __code (__PRIM, __PRIM_CONFIG_READ_BINARY, 0x10); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
    __code (__STORE, __typechk(BOOL *, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosAcceleratedReadBAC (BYTE* SSCAddr,
**                                BYTE* KeyMacAddr,
**                                BYTE* KeyEncAddr,
**                                BYTE channel,
**                                BYTE* dataAddr,
**                                BOOL optional, 
**                                BOOL *success)
** optional: false = Mandatory, true = Optional
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosAcceleratedReadBAC (BYTE* SSCAddr, BYTE* KeyMacAddr, BYTE* KeyEncAddr, BYTE channel, BYTE* dataAddr, BOOL optional, BOOL *success);
#else
#define multosAcceleratedReadBAC(SSCAddr, KeyMacAddr, KeyEncAddr, channel, dataAddr, optional, success) \
do \
{ \
    __push (__typechk(BYTE*, SSCAddr)); \
    __push (__typechk(BYTE*, KeyMacAddr)); \
    __push (__typechk(BYTE*, KeyEncAddr)); \
    __push (__typechk(BYTE, channel)); \
    __push (__typechk(BYTE*, dataAddr)); \
    if(optional) \
        __code (__PRIM, __PRIM_CONFIG_READ_BINARY, 0x11); \
    else \
        __code (__PRIM, __PRIM_CONFIG_READ_BINARY, 0x12); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
    __code (__STORE, __typechk(BOOL *, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosAcceleratedReadBACLite ( BYTE* KeyEncAddr,
**                                BYTE channel,
**                                BYTE* dataAddr,
**                                BOOL optional, 
**                                BOOL *success)
** optional: false = Mandatory, true = Optional
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosAcceleratedReadBACLite (BYTE* KeyEncAddr, BYTE channel, BYTE* dataAddr, BOOL optional, BOOL *success);
#else
#define multosAcceleratedReadBACLite(KeyEncAddr, channel, dataAddr, optional, success) \
do \
{ \
    __push (__typechk(BYTE*, KeyEncAddr)); \
    __push (__typechk(BYTE, channel)); \
    __push (__typechk(BYTE*, dataAddr)); \
    if(optional) \
        __code (__PRIM, __PRIM_CONFIG_READ_BINARY, 0x13); \
    else \
        __code (__PRIM, __PRIM_CONFIG_READ_BINARY, 0x14); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
    __code (__STORE, __typechk(BOOL *, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetStaticSize ( DWORD* value, BOOL *success )
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetStaticSize ( DWORD* value, BOOL *success );
#else
#define multosGetStaticSize(value, success) \
do \
{ \
	__code (__PRIM, __PRIM_GET_STATIC_SIZE, 0x00); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
	__code (__STORE, __typechk(BOOL *, success), 1); \
	if(*success) \
		__code (__STORE, __typechk(DWORD *,value), 4); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetStaticSizeHuge ( BYTE* value, BOOL *success )
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetStaticSizeHuge ( BYTE* value, BOOL *success );
#else
#define multosGetStaticSizeHuge(value, success) \
do \
{ \
	__code (__PRIM, __PRIM_GET_STATIC_SIZE, 0x01); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x01); \
	__code (__STORE, __typechk(BOOL *, success), 1); \
	if(*success) \
		__code (__STORE, __typechk(BYTE *,value), 8); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGenerateRsaKeyPair ( const BYTE method, const BYTE mode, WORD keyLen,
** WORD expLen, BYTE* expAddr, BYTE* dpdqpquAddr, BYTE* mAddr, WORD modLen, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGenerateRsaKeyPair ( const BYTE method, const BYTE mode, WORD keyLen, WORD expLen, BYTE* expAddr, BYTE* dpdqpquAddr, BYTE* mAddr, WORD modLen, BOOL *success);
#else
#define multosGenerateRsaKeyPair(method, mode, keyLen, expLen, expAddr, dpdqpquAddr, mAddr, modLen, success) \
do \
{ \
	__push (__typechk(WORD, keyLen)); \
	__push (__typechk(WORD, expLen)); \
	__push (__typechk(BYTE*, expAddr)); \
	__push (__typechk(BYTE*, dpdqpquAddr)); \
	__push (__typechk(BYTE*, mAddr)); \
	__push (__typechk(WORD, modLen)); \
	__code (__PRIM, __PRIM_GEN_RSA_KEYPAIR, method, mode ); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x83), 0x08); \
	__code (__STORE, __typechk(BOOL*, success), 1); \
	if(*success==8) \
		*success = FALSE; \
	else \
		*success = TRUE; \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGsmAuthenticate(BYTE* random, BYTE* key, BYTE* result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGsmAuthenticate(BYTE* random, BYTE* key, BYTE* result);
#else
#define multosGsmAuthenticate(random, key, result) \
do \
{ \
	__push (__typechk(BYTE*, random)); \
	__push (__typechk(BYTE*, key)); \
	__push (__typechk(BYTE*, result)); \
	__code (__PRIM, __PRIM_GSM_AUTHENTICATE ); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
multosCopyToAdditionalStatic(DWORD destAddr, BYTE* srcAddr, WORD toCopy, BYTE atomic)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosCopyToAdditionalStatic(DWORD destAddr, BYTE* srcAddr, WORD toCopy, BYTE atomic);
#else
#define multosCopyToAdditionalStatic(destAddr, srcAddr, toCopy, atomic) \
do \
{ \
	__push (__typechk(WORD, toCopy)); \
	__push (__typechk(DWORD, destAddr)); \
	__push (__typechk(BYTE*, srcAddr)); \
	if(atomic) \
		__code (__PRIM, __PRIM_COPY_ADDITIONAL_STATIC, 0x80 ); \
	else \
		__code (__PRIM, __PRIM_COPY_ADDITIONAL_STATIC, 0x00 ); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
multosCopyWithinAdditionalStatic(DWORD destAddr, DWORD srcAddr, DWORD toCopy, BYTE atomic)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosCopyWithinAdditionalStatic(DWORD destAddr, DWORD srcAddr, DWORD toCopy, BYTE atomic);
#else
#define multosCopyWithinAdditionalStatic(destAddr, srcAddr, toCopy, atomic) \
do \
{ \
	__push (__typechk(DWORD, toCopy)); \
	__push (__typechk(DWORD, destAddr)); \
	__push (__typechk(DWORD, srcAddr)); \
	if(atomic) \
		__code (__PRIM, __PRIM_COPY_ADDITIONAL_STATIC, 0x82 ); \
	else \
		__code (__PRIM, __PRIM_COPY_ADDITIONAL_STATIC, 0x02 ); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
multosCopyAdditionalToStatic(BYTE* destAddr, DWORD srcAddr, WORD toCopy, BYTE atomic)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosCopyAdditionalToStatic(BYTE* LONG destAddr, DWORD srcAddr, WORD toCopy, BYTE atomic);
#else
#define multosCopyAdditionalToStatic(destAddr, srcAddr, toCopy, atomic) \
do \
{ \
	__push (__typechk(WORD, toCopy)); \
	__push (__typechk(BYTE*, destAddr)); \
	__push (__typechk(DWORD, srcAddr)); \
	if(atomic) \
		__code (__PRIM, __PRIM_COPY_ADDITIONAL_STATIC, 0x81 ); \
	else \
		__code (__PRIM, __PRIM_COPY_ADDITIONAL_STATIC, 0x01 ); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
multosFillAdditionalStatic(DWORD destAddr, BYTE value, DWORD toFill, BYTE atomic)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosFillAdditionalStatic(DWORD destAddr, BYTE value, DWORD toFill, BYTE atomic);
#else
#define multosFillAdditionalStatic(destAddr, value, toFill, atomic) \
do \
{ \
	__push (__typechk(BYTE, value)); \
	__push (__typechk(DWORD, toFill)); \
	__push (__typechk(DWORD, destAddr)); \
	if(atomic) \
		__code (__PRIM, __PRIM_FILL_ADDITIONAL_STATIC, 0x80 ); \
	else \
		__code (__PRIM, __PRIM_FILL_ADDITIONAL_STATIC, 0x00 ); \
} while (0)
#endif


/*
**-----------------------------------------------------------------------------
** void multosSecureHashIV (WORD msgLen,                 // IN
**                        WORD hashLen,                  // IN
**                        BYTE *hashOut,                 // OUT
**                        BYTE *msgIn,                   // IN
**                        BYTE *intermediateHash,        // IN
**                        DWORD *numPrevHashedBytes,     // IN/OUT
**                        WORD *numMsgRemainder,         // IN/OUT
**                        WORD *msgRemainder             // IN/OUT
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSecureHashIV (WORD msgLen, WORD hashLen, BYTE *hashOut, BYTE *msgIn, BYTE *intermediateHash, DWORD *numPrevHashedBytes, WORD *numMsgRemainder, WORD *msgRemainder);
#else
#define multosSecureHashIV(msgLen, hashLen, hashOut, msgIn, intermediateHash, numPrevHashedBytes, numMsgRemainder, msgRemainder) \
do \
{ \
	__push (__typechk(WORD, msgLen)); \
	__push (__typechk(WORD, hashLen)); \
	__push (__typechk(BYTE *, hashOut)); \
	__push (__typechk(BYTE *, msgIn)); \
	__push (__typechk(BYTE *, intermediateHash)); \
	__push (__typechk(DWORD *, numPrevHashedBytes)); \
	__push (__typechk(WORD, *numMsgRemainder)); \
	__push (__typechk(WORD, *msgRemainder)); \
    __code (__PRIM, __PRIM_SECURE_HASH_IV); \
	__code(__STORE, msgRemainder, 2); \
	__code(__STORE, numMsgRemainder, 2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosEccGenerateKeyPair(BYTE *keyOut, BYTE *domain, const BYTE option, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosEccGenerateKeyPair(BYTE *keyOut, BYTE *domain, const BYTE option, BOOL *success);
#else
#define multosEccGenerateKeyPair(keyOut, domain, option, success) \
do \
{ \
	__push (__typechk(BYTE *, domain)); \
	__push (__typechk(BYTE *, keyOut)); \
	__code (__PRIM, __PRIM_GEN_ECC_PAIR, option); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x80), 0x01); \
	__code (__STORE, __typechk(BOOL*, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosEccGenerateSignature(BYTE *domainIn, BYTE *keyIn, BYTE *hashIn, BYTE *sigOut, const BYTE option, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosEccGenerateSignature(BYTE *domainIn, BYTE *keyIn, BYTE *hashIn, BYTE *sigOut, const BYTE option, BOOL *success);
#else
#define multosEccGenerateSignature(domainIn, keyIn, hashIn, sigOut, option, success) \
do \
{ \
	__push (__typechk(BYTE *, domainIn)); \
	__push(__typechk(BYTE *, keyIn)); \
	__push(__typechk(BYTE *, hashIn)); \
	__push(__typechk(BYTE *, sigOut)); \
	__code (__PRIM, __PRIM_GEN_ECC_SIG, option); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x80), 0x01); \
	__code (__STORE, __typechk(BOOL*, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosEccVerifySignature(BYTE *domainIn, BYTE *hashIn, BYTE *sigIn,
**                               BYTE *pubKeyIn, const BYTE option, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosEccVerifySignature(BYTE *domainIn, BYTE *hashIn, BYTE *sigIn, BYTE *pubKeyIn, const BYTE option, BOOL *success);
#else
#define multosEccVerifySignature(domainIn, hashIn, sigIn, pubKeyIn, option, success) \
do \
{ \
	__push(__typechk(BYTE *, domainIn)); \
	__push(__typechk(BYTE *, pubKeyIn)); \
	__push(__typechk(BYTE *, sigIn)); \
	__push(__typechk(BYTE *, hashIn)); \
	__code (__PRIM, __PRIM_VER_ECC_SIG, option); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x80), 0x01); \
	__code (__STORE, __typechk(BOOL*, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosEccDiffieHelman(BYTE *domainIn, BYTE *privateKeyIn, BYTE *publicKeyIn, BYTE *bufferOut, BYTE option, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosEccDiffieHelman(BYTE *domainIn, BYTE *privateKeyIn, BYTE *publicKeyIn, BYTE *bufferOut, BYTE option, BOOL *success);
#else
#define multosEccDiffieHelman(domainIn, privateKeyIn, publicKeyIn, bufferOut, option, success) \
do \
{ \
	__push(__typechk(BYTE *, domainIn)); \
	__push(__typechk(BYTE *, privateKeyIn)); \
	__push(__typechk(BYTE *, publicKeyIn)); \
	__push(__typechk(BYTE *, bufferOut)); \
	__code (__PRIM, __PRIM_ECCDH, option); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x80), 0x01); \
	__code (__STORE, __typechk(BOOL*, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosEccECIESEncipher(BYTE *domainIn, WORD msgLen, BYTE *publicKeyIn, BYTE *msgIn, BYTE *bufferOut, BYTE option, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosEccECIESEncipher(BYTE *domainIn, WORD msgLen, BYTE *publicKeyIn, BYTE *msgIn, BYTE *bufferOut, BYTE option, BOOL *success);
#else
#define multosEccECIESEncipher(domainIn, msgLen, publicKeyIn, msgIn, bufferOut, option, success) \
do \
{ \
	__push(__typechk(BYTE *, domainIn)); \
	__push(__typechk(WORD, msgLen)); \
	__push(__typechk(BYTE *, publicKeyIn)); \
	__push(__typechk(BYTE *, msgIn)); \
	__push(__typechk(BYTE *, bufferOut)); \
	__code (__PRIM, __PRIM_ECC_ENC, option); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x80), 0x01); \
	__code (__STORE, __typechk(BOOL*, success), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosEccECIESDecipher(BYTE *domainIn, WORD msgLen, BYTE *privateKeyIn, BYTE *msgIn, BYTE *bufferOut, BYTE option, BOOL *success)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosEccECIESDecipher(BYTE *domainIn, WORD msgLen, BYTE *privateKeyIn, BYTE *msgIn, BYTE *bufferOut, BYTE option, BOOL *success);
#else
#define multosEccECIESDecipher(domainIn, msgLen, privateKeyIn, msgIn, bufferOut, option, success) \
do \
{ \
	__push(__typechk(BYTE *, domainIn)); \
	__push(__typechk(WORD, msgLen)); \
	__push(__typechk(BYTE *, privateKeyIn)); \
	__push(__typechk(BYTE *, msgIn)); \
	__push(__typechk(BYTE *, bufferOut)); \
	__code (__PRIM, __PRIM_ECC_DEC, option); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, (0x80), 0x01); \
	__code (__STORE, __typechk(BOOL*, success), 1); \
} while (0)
#endif

/*--------------------- MULTOS 4.3.2 and 4.4/4.5 additions -----------------------*/
/*
**-----------------------------------------------------------------------------
** void multosSetSelectCLSW (const BYTE sw1,const BYTE sw2)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetSelectCLSW (const BYTE sw1,const BYTE sw2);
#else
#define multosSetSelectCLSW(sw1, sw2)                                         \
    __code (__PRIM, __PRIM_SET_SELECT_CL_SW, sw1, sw2)
#endif	

/*
*------------------------------------------------------------------------------
** void multosBlockLookupWord (WORD value,
**                         WORD *block,
**                         WORD *result,
**                         BOOL *byteFound)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockLookupWord (WORD value, WORD *block, WORD *result,
                        BYTE *wordFound);
#else
#define multosBlockLookupWord(value, block, result, wordFound) \
do  \
{ \
    __push (__typechk(WORD, value)); \
    __push (__typechk(WORD *, block)); \
    __code (__PRIM, __PRIM_LOOKUP_WORD); \
    __code (__STORE, __typechk(WORD *, result), 2); \
    __code (__PRIM, __PRIM_LOAD_CCR); \
    __code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 0x09); \
    __code (__STORE, __typechk(BYTE *, wordFound), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetProcessEvent (BYTE *event)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetProcessEvent (BYTE *event);
#else
#define multosGetProcessEvent(event) \
do \
{ \
    __code (__PRIM, __PRIM_GET_PROCESS_EVENT); \
    __code (__STORE, __typechk (BYTE *, event), 1); \
} \
while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosRejectProcessEvent (void)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosRejectProcessEvent(void);
#else
#define multosRejectProcessEvent() \
do \
{ \
    __code (__PRIM, __PRIM_REJECT_PROCESS_EVENT); \
} \
while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosRsaVerify (WORD exponentLength,
**                                   WORD modulusLength,
**                                   BYTE *exponent,
**                                   BYTE *modulus,
**                                   BYTE *input,
**                                   BYTE *output)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosRsaVerify (WORD exponentLength, WORD modulusLength,
                                  BYTE *exponent, BYTE *modulus, BYTE *input,
                                  BYTE *output);
#else
#define multosRsaVerify(exponentLength, modulusLength, exponent, modulus, input, output) \
do \
{ \
    __push (__typechk(WORD, exponentLength)); \
    __push (__typechk(WORD, modulusLength)); \
    __push (__typechk(BYTE *, exponent)); \
    __push (__typechk(BYTE *, modulus)); \
    __push (__typechk(BYTE *, input)); \
    __push (__typechk(BYTE *, output)); \
    __code (__PRIM, __PRIM_RSA_VERIFY); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetATSHistoricalCharacters (BYTE *input,
**                                        BYTE *numBytesWritten)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetATSHistoricalCharacters (BYTE *input, BYTE *numBytesWritten);
#else
#define multosSetATSHistoricalCharacters(input, numBytesWritten) \
do \
{ \
    __push (__typechk(BYTE *, input)); \
    __code (__PRIM, __PRIM_SET_ATS_HISTORICAL_CHARACTERS); \
    __code (__STORE, __typechk(BYTE *, numBytesWritten), 1); \
} while (0)
#endif


/*
**-----------------------------------------------------------------------------
** void multosSetFCIFileRecord (BYTE *record,
**                              BYTE *numBytesWritten)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetFCIFileRecord (BYTE *record, BYTE *numBytesWritten);
#else
#define multosSetFCIFileRecord(record, numBytesWritten) \
do \
{ \
    __push (__typechk(BYTE *, record)); \
    __code (__PRIM, __PRIM_SET_FCI_FILE_RECORD); \
    __code (__STORE, __typechk(BYTE *, numBytesWritten), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosInitialisePIN (BYTE *initDataAddr)
**
** initDataAddr is a 15 data block formatted as follows
** - PIN Reference Data [8]
** - PIN Length [1]
** - PIN Try Counter [1]
** - PIN Try Limit [1]
** - MULTOS Checkum [4]
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosInitialisePIN (BYTE *initDataAddr);
#else
#define multosInitialisePIN(initDataAddr) \
do \
{ \
    __push (__typechk(BYTE *, initDataAddr)); \
    __code (__PRIM, __PRIM_INITIALISE_PIN); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosReadPIN (BYTE *outAddr, BYTE *pinLength)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosReadPIN (BYTE *outAddr, BYTE *pinLength);
#else
#define multosReadPIN(outAddr, pinLength) \
do \
{ \
    __push (__typechk(BYTE *, outAddr)); \
    __code (__PRIM, __PRIM_READ_PIN); \
    __code (__STORE, __typechk(BYTE *, pinLength), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosVerifyPIN (BYTE pinLen, BYTE *pinAddr, WORD *status)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosVerifyPIN (BYTE pinLen, BYTE *pinAddr, WORD *status);
#else
#define multosVerifyPIN(pinLen, pinAddr, status) \
do \
{ \
    __push (__typechk(BYTE, pinLen)); \
    __push (__typechk(BYTE *, pinAddr)); \
    __code (__PRIM, __PRIM_VERIFY_PIN); \
    __code (__STORE, __typechk(WORD *, status), 2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetPINTryCounter (BYTE ptc)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetPINTryCounter (BYTE ptc);
#else
#define multosSetPINTryCounter(ptc) \
do \
{ \
	__push (__typechk(BYTE, ptc)); \
	__code (__PRIM, __PRIM_SET_PIN_DATA, 0x00); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosSetPINTryLimit (BYTE ptl)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosSetPINTryLimit (BYTE ptl);
#else
#define multosSetPINTryLimit(ptl) \
do \
{ \
	__push (__typechk(BYTE, ptl)); \
	__code (__PRIM, __PRIM_SET_PIN_DATA, 0x01); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetPINTryCounter (BYTE *ptc)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetPINTryCounter (BYTE *ptc);
#else
#define multosGetPINTryCounter(ptc) \
do \
{ \
	__code (__PRIM, __PRIM_GET_PIN_DATA, 0x00); \
	__code (__STORE, __typechk(BYTE *, ptc), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetPINTryLimit (BYTE *ptl)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetPINTryLimit (BYTE *ptl);
#else
#define multosGetPINTryLimit(ptl) \
do \
{ \
	__code (__PRIM, __PRIM_GET_PIN_DATA, 0x01); \
	__code (__STORE, __typechk(BYTE *, ptl), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosGetPINStatus(BYTE *status)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosGetPINStatus (BYTE *status);
#else
#define multosGetPINStatus(status) \
do \
{ \
	__code (__PRIM, __PRIM_GET_PIN_DATA, 0x02); \
	__code (__STORE, __typechk(BYTE *, status), 1); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosFlushPublic (WORD blockSize, BOOL *LaFlag)
** LaFlag is set to 1 if La > 0
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosFlushPublic (WORD blockSize, BOOL *LaFlag);
#else
#define multosFlushPublic(blockSize, LaFlag) \
do \
{ \
	__push (__typechk(WORD, blockSize)); \
	__code (__PRIM, __PRIM_FLUSH_PUBLIC); \
	__code (__PRIM, __PRIM_LOAD_CCR); \
	__code (__PRIM, __PRIM_BIT_MANIPULATE_BYTE, 0x83, 0x01); \
	__code (__STORE, __typechk(BOOL *, LaFlag), 1); \
} while (0)
#endif

/*--------------------- MULTOS 4.5.1 additions -----------------------*/

/*
**-----------------------------------------------------------------------------
** void multosBlockShiftLeftVar (WORD num_bits, WORD data_len, BYTE *data_addr)
** void multosBlockShiftRightVar               "
** void multosBlockRotateLeft                  "
** void multosBlockRotateRight                 "
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosBlockShiftLeftVar (WORD num_bits, WORD data_len, BYTE *data_addr);
#else
#define multosBlockShiftLeftVar(numBits,dataLen,dataAddr) \
do \
{ \
	__push (__typechk(WORD, numBits)); \
	__push (__typechk(WORD, dataLen)); \
	__push (__typechk(BYTE*, dataAddr)); \
	__code (__PRIM, __PRIM_SHIFT_ROTATE, 1, 1); \
} while (0)
#endif
#ifdef __FUNCTION_PROTOTYPES
void multosBlockShiftRightVar (WORD num_bits, WORD data_len, BYTE *data_addr);
#else
#define multosBlockShiftRightVar(numBits,dataLen,dataAddr) \
do \
{ \
	__push (__typechk(WORD, numBits)); \
	__push (__typechk(WORD, dataLen)); \
	__push (__typechk(BYTE*, dataAddr)); \
	__code (__PRIM, __PRIM_SHIFT_ROTATE, 1, 2); \
} while (0)
#endif
#ifdef __FUNCTION_PROTOTYPES
void multosBlockRotateLeft (WORD num_bits, WORD data_len, BYTE *data_addr);
#else
#define multosBlockRotateLeft(numBits,dataLen,dataAddr) \
do \
{ \
	__push (__typechk(WORD, numBits)); \
	__push (__typechk(WORD, dataLen)); \
	__push (__typechk(BYTE*, dataAddr)); \
	__code (__PRIM, __PRIM_SHIFT_ROTATE, 2, 1); \
} while (0)
#endif
#ifdef __FUNCTION_PROTOTYPES
void multosBlockRotateRight (WORD num_bits, WORD data_len, BYTE *data_addr);
#else
#define multosBlockRotateRight(numBits,dataLen,dataAddr) \
do \
{ \
	__push (__typechk(WORD, numBits)); \
	__push (__typechk(WORD, dataLen)); \
	__push (__typechk(BYTE*, dataAddr)); \
	__code (__PRIM, __PRIM_SHIFT_ROTATE, 2, 2); \
} while (0)
#endif

/*
**-----------------------------------------------------------------------------
** void multosUpdateStaticSize (DWORD length,                             
**                              BYTE *result)
**-----------------------------------------------------------------------------
*/
#ifdef __FUNCTION_PROTOTYPES
void multosUpdateStaticSize (DWORD length, BYTE * result);
#else
#define multosUpdateStaticSize(length, result) \
do \
{ \
    __push (__typechk(DWORD, length)); \
    __code (__PRIM, __PRIM_UPDATE_STATIC_SIZE, 0); \
    __code (__STORE, __typechk(BYTE *, result), 1); \
} while (0)
#endif

#endif // #ifndef MULTOS_H
