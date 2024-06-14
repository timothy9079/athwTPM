#ifndef __ATHW_ATHW_TPM2_H
#define __ATHW_ATHW_TPM2_H


#ifdef __cplusplus
extern "C"
{
#endif

#include "main.h"
#include "athw_tpm_io.h"
#include "athw_tpmio_types.h"
#include "athw_tpm2_packet.h"
#include "athw_tpm2_types.h"
#include "athw_tpm_tis.h"

#define ATHW_TPM_TIMEOUT                5



enum ATHW_TPM_tis_access {
    ATHW_TPM_ACCESS_VALID            = 0x80,
    ATHW_TPM_ACCESS_ACTIVE_LOCALITY  = 0x20,
    ATHW_TPM_ACCESS_REQUEST_PENDING  = 0x04,
    ATHW_TPM_ACCESS_REQUEST_USE      = 0x02,
};

enum ATHW_TPM_tis_status {
    ATHW_TPM_STS_VALID               = 0x80,
    ATHW_TPM_STS_COMMAND_READY       = 0x40,
    ATHW_TPM_STS_GO                  = 0x20,
    ATHW_TPM_STS_DATA_AVAIL          = 0x10,
    ATHW_TPM_STS_DATA_EXPECT         = 0x08,
    ATHW_TPM_STS_SELF_TEST_DONE      = 0x04,
    ATHW_TPM_STS_RESP_RETRY          = 0x02,
};

/* enum ATHW_TPM_tis_int_flags */
#define ATHW_TPM_GLOBAL_INT_ENABLE       0x80000000UL
#define ATHW_TPM_INTF_BURST_COUNT_STATIC 0x100
#define ATHW_TPM_INTF_CMD_READY_INT      0x080
#define ATHW_TPM_INTF_INT_EDGE_FALLING   0x040
#define ATHW_TPM_INTF_INT_EDGE_RISING    0x020
#define ATHW_TPM_INTF_INT_LEVEL_LOW      0x010
#define ATHW_TPM_INTF_INT_LEVEL_HIGH     0x008
#define ATHW_TPM_INTF_LOC_CHANGE_INT     0x004
#define ATHW_TPM_INTF_STS_VALID_INT      0x002
#define ATHW_TPM_INTF_DATA_AVAIL_INT     0x001


#ifndef ATHW_TPM_BASE_ADDRESS
    #define ATHW_TPM_BASE_ADDRESS (0xD40000u)
#endif

#ifdef ATHW_TPM_I2C
/* For I2C only the lower 8-bits of the address are used */
    #define ATHW_TPM_ACCESS(l)           (ATHW_TPM_BASE_ADDRESS | 0x0004u | ((l) << 12u))
    #define ATHW_TPM_INTF_CAPS(l)        (ATHW_TPM_BASE_ADDRESS | 0x0030u | ((l) << 12u))
    #define ATHW_TPM_DID_VID(l)          (ATHW_TPM_BASE_ADDRESS | 0x0048u | ((l) << 12u))
    #define ATHW_TPM_RID(l)              (ATHW_TPM_BASE_ADDRESS | 0x004Cu | ((l) << 12u))
    #define ATHW_TPM_I2C_DEVICE_ADDR(l)  (ATHW_TPM_BASE_ADDRESS | 0x0038u | ((l) << 12u))
    #define ATHW_TPM_DATA_CSUM_ENABLE(l) (ATHW_TPM_BASE_ADDRESS | 0x0040u | ((l) << 12u))
    #define ATHW_TPM_DATA_CSUM(l)        (ATHW_TPM_BASE_ADDRESS | 0x0044u | ((l) << 12u))
#else
    #define ATHW_TPM_ACCESS(l)           (ATHW_TPM_BASE_ADDRESS | 0x0000u | ((l) << 12u))
    #define ATHW_TPM_INTF_CAPS(l)        (ATHW_TPM_BASE_ADDRESS | 0x0014u | ((l) << 12u))
    #define ATHW_TPM_DID_VID(l)          (ATHW_TPM_BASE_ADDRESS | 0x0F00u | ((l) << 12u))
    #define ATHW_TPM_RID(l)              (ATHW_TPM_BASE_ADDRESS | 0x0F04u | ((l) << 12u))
#endif

#define ATHW_TPM_INT_ENABLE(l)       (ATHW_TPM_BASE_ADDRESS | 0x0008u | ((l) << 12u))
#define ATHW_TPM_INT_VECTOR(l)       (ATHW_TPM_BASE_ADDRESS | 0x000Cu | ((l) << 12u))
#define ATHW_TPM_INT_STATUS(l)       (ATHW_TPM_BASE_ADDRESS | 0x0010u | ((l) << 12u))
#define ATHW_TPM_STS(l)              (ATHW_TPM_BASE_ADDRESS | 0x0018u | ((l) << 12u))
#define ATHW_TPM_BURST_COUNT(l)      (ATHW_TPM_BASE_ADDRESS | 0x0019u | ((l) << 12u))
#define ATHW_TPM_DATA_FIFO(l)        (ATHW_TPM_BASE_ADDRESS | 0x0024u | ((l) << 12u))
#define ATHW_TPM_XDATA_FIFO(l)       (ATHW_TPM_BASE_ADDRESS | 0x0083u | ((l) << 12u))


typedef enum {
    TPM_ST_RSP_COMMAND          = 0x00C4,
    TPM_ST_NULL                 = 0X8000,
    TPM_ST_NO_SESSIONS          = 0x8001,
    TPM_ST_SESSIONS             = 0x8002,
    TPM_ST_ATTEST_NV            = 0x8014,
    TPM_ST_ATTEST_COMMAND_AUDIT = 0x8015,
    TPM_ST_ATTEST_SESSION_AUDIT = 0x8016,
    TPM_ST_ATTEST_CERTIFY       = 0x8017,
    TPM_ST_ATTEST_QUOTE         = 0x8018,
    TPM_ST_ATTEST_TIME          = 0x8019,
    TPM_ST_ATTEST_CREATION      = 0x801A,
    TPM_ST_CREATION             = 0x8021,
    TPM_ST_VERIFIED             = 0x8022,
    TPM_ST_AUTH_SECRET          = 0x8023,
    TPM_ST_HASHCHECK            = 0x8024,
    TPM_ST_AUTH_SIGNED          = 0x8025,
    TPM_ST_FU_MANIFEST          = 0x8029,
} TPM_ST_T;

typedef enum{
    TPM_CC_FIRST                    = 0x0000011F,
    TPM_CC_NV_UndefineSpaceSpecial  = TPM_CC_FIRST,
    TPM_CC_EvictControl             = 0x00000120,
    TPM_CC_HierarchyControl         = 0x00000121,
    TPM_CC_NV_UndefineSpace         = 0x00000122,
    TPM_CC_ChangeEPS                = 0x00000124,
    TPM_CC_ChangePPS                = 0x00000125,
    TPM_CC_Clear                    = 0x00000126,
    TPM_CC_ClearControl             = 0x00000127,
    TPM_CC_ClockSet                 = 0x00000128,
    TPM_CC_HierarchyChangeAuth      = 0x00000129,
    TPM_CC_NV_DefineSpace           = 0x0000012A,
    TPM_CC_PCR_Allocate             = 0x0000012B,
    TPM_CC_PCR_SetAuthPolicy        = 0x0000012C,
    TPM_CC_PP_Commands              = 0x0000012D,
    TPM_CC_SetPrimaryPolicy         = 0x0000012E,
    TPM_CC_FieldUpgradeStart        = 0x0000012F,
    TPM_CC_ClockRateAdjust          = 0x00000130,
    TPM_CC_CreatePrimary            = 0x00000131,
    TPM_CC_NV_GlobalWriteLock       = 0x00000132,
    TPM_CC_GetCommandAuditDigest    = 0x00000133,
    TPM_CC_NV_Increment             = 0x00000134,
    TPM_CC_NV_SetBits               = 0x00000135,
    TPM_CC_NV_Extend                = 0x00000136,
    TPM_CC_NV_Write                 = 0x00000137,
    TPM_CC_NV_WriteLock             = 0x00000138,
    TPM_CC_DictionaryAttackLockReset = 0x00000139,
    TPM_CC_DictionaryAttackParameters = 0x0000013A,
    TPM_CC_NV_ChangeAuth            = 0x0000013B,
    TPM_CC_PCR_Event                = 0x0000013C,
    TPM_CC_PCR_Reset                = 0x0000013D,
    TPM_CC_SequenceComplete         = 0x0000013E,
    TPM_CC_SetAlgorithmSet          = 0x0000013F,
    TPM_CC_SetCommandCodeAuditStatus = 0x00000140,
    TPM_CC_FieldUpgradeData         = 0x00000141,
    TPM_CC_IncrementalSelfTest      = 0x00000142,
    TPM_CC_SelfTest                 = 0x00000143,
    TPM_CC_Startup                  = 0x00000144,
    TPM_CC_Shutdown                 = 0x00000145,
    TPM_CC_StirRandom               = 0x00000146,
    TPM_CC_ActivateCredential       = 0x00000147,
    TPM_CC_Certify                  = 0x00000148,
    TPM_CC_PolicyNV                 = 0x00000149,
    TPM_CC_CertifyCreation          = 0x0000014A,
    TPM_CC_Duplicate                = 0x0000014B,
    TPM_CC_GetTime                  = 0x0000014C,
    TPM_CC_GetSessionAuditDigest    = 0x0000014D,
    TPM_CC_NV_Read                  = 0x0000014E,
    TPM_CC_NV_ReadLock              = 0x0000014F,
    TPM_CC_ObjectChangeAuth         = 0x00000150,
    TPM_CC_PolicySecret             = 0x00000151,
    TPM_CC_Rewrap                   = 0x00000152,
    TPM_CC_Create                   = 0x00000153,
    TPM_CC_ECDH_ZGen                = 0x00000154,
    TPM_CC_HMAC                     = 0x00000155,
    TPM_CC_Import                   = 0x00000156,
    TPM_CC_Load                     = 0x00000157,
    TPM_CC_Quote                    = 0x00000158,
    TPM_CC_RSA_Decrypt              = 0x00000159,
    TPM_CC_HMAC_Start               = 0x0000015B,
    TPM_CC_SequenceUpdate           = 0x0000015C,
    TPM_CC_Sign                     = 0x0000015D,
    TPM_CC_Unseal                   = 0x0000015E,
    TPM_CC_PolicySigned             = 0x00000160,
    TPM_CC_ContextLoad              = 0x00000161,
    TPM_CC_ContextSave              = 0x00000162,
    TPM_CC_ECDH_KeyGen              = 0x00000163,
    TPM_CC_EncryptDecrypt           = 0x00000164,
    TPM_CC_FlushContext             = 0x00000165,
    TPM_CC_LoadExternal             = 0x00000167,
    TPM_CC_MakeCredential           = 0x00000168,
    TPM_CC_NV_ReadPublic            = 0x00000169,
    TPM_CC_PolicyAuthorize          = 0x0000016A,
    TPM_CC_PolicyAuthValue          = 0x0000016B,
    TPM_CC_PolicyCommandCode        = 0x0000016C,
    TPM_CC_PolicyCounterTimer       = 0x0000016D,
    TPM_CC_PolicyCpHash             = 0x0000016E,
    TPM_CC_PolicyLocality           = 0x0000016F,
    TPM_CC_PolicyNameHash           = 0x00000170,
    TPM_CC_PolicyOR                 = 0x00000171,
    TPM_CC_PolicyTicket             = 0x00000172,
    TPM_CC_ReadPublic               = 0x00000173,
    TPM_CC_RSA_Encrypt              = 0x00000174,
    TPM_CC_StartAuthSession         = 0x00000176,
    TPM_CC_VerifySignature          = 0x00000177,
    TPM_CC_ECC_Parameters           = 0x00000178,
    TPM_CC_FirmwareRead             = 0x00000179,
    TPM_CC_GetCapability            = 0x0000017A,
    TPM_CC_GetRandom                = 0x0000017B,
    TPM_CC_GetTestResult            = 0x0000017C,
    TPM_CC_Hash                     = 0x0000017D,
    TPM_CC_PCR_Read                 = 0x0000017E,
    TPM_CC_PolicyPCR                = 0x0000017F,
    TPM_CC_PolicyRestart            = 0x00000180,
    TPM_CC_ReadClock                = 0x00000181,
    TPM_CC_PCR_Extend               = 0x00000182,
    TPM_CC_PCR_SetAuthValue         = 0x00000183,
    TPM_CC_NV_Certify               = 0x00000184,
    TPM_CC_EventSequenceComplete    = 0x00000185,
    TPM_CC_HashSequenceStart        = 0x00000186,
    TPM_CC_PolicyPhysicalPresence   = 0x00000187,
    TPM_CC_PolicyDuplicationSelect  = 0x00000188,
    TPM_CC_PolicyGetDigest          = 0x00000189,
    TPM_CC_TestParms                = 0x0000018A,
    TPM_CC_Commit                   = 0x0000018B,
    TPM_CC_PolicyPassword           = 0x0000018C,
    TPM_CC_ZGen_2Phase              = 0x0000018D,
    TPM_CC_EC_Ephemeral             = 0x0000018E,
    TPM_CC_PolicyNvWritten          = 0x0000018F,
    TPM_CC_PolicyTemplate           = 0x00000190,
    TPM_CC_CreateLoaded             = 0x00000191,
    TPM_CC_PolicyAuthorizeNV        = 0x00000192,
    TPM_CC_EncryptDecrypt2          = 0x00000193,
    TPM_CC_LAST                     = TPM_CC_EncryptDecrypt2,

    CC_VEND                         = 0x20000000,
    TPM_CC_Vendor_TCG_Test          = CC_VEND + 0x0000,

}TPM_CC_T;
 
typedef enum {
    ATHW_TPM_RC_SUCCESS  = 0x000,
    ATHW_TPM_RC_BAD_TAG  = 0x01E,

    RC_VER1 = 0x100,
    ATHW_TPM_RC_INITIALIZE           = RC_VER1 + 0x000,
    ATHW_TPM_RC_FAILURE              = RC_VER1 + 0x001,
    ATHW_TPM_RC_SEQUENCE             = RC_VER1 + 0x003,
    ATHW_TPM_RC_PRIVATE              = RC_VER1 + 0x00B,
    ATHW_TPM_RC_HMAC                 = RC_VER1 + 0x019,
    ATHW_TPM_RC_DISABLED             = RC_VER1 + 0x020,
    ATHW_TPM_RC_EXCLUSIVE            = RC_VER1 + 0x021,
    ATHW_TPM_RC_AUTH_TYPE            = RC_VER1 + 0x024,
    ATHW_TPM_RC_AUTH_MISSING         = RC_VER1 + 0x025,
    ATHW_TPM_RC_POLICY               = RC_VER1 + 0x026,
    ATHW_TPM_RC_PCR                  = RC_VER1 + 0x027,
    ATHW_TPM_RC_PCR_CHANGED          = RC_VER1 + 0x028,
    ATHW_TPM_RC_UPGRADE              = RC_VER1 + 0x02D,
    ATHW_TPM_RC_TOO_MANY_CONTEXTS    = RC_VER1 + 0x02E,
    ATHW_TPM_RC_AUTH_UNAVAILABLE     = RC_VER1 + 0x02F,
    ATHW_TPM_RC_REBOOT               = RC_VER1 + 0x030,
    ATHW_TPM_RC_UNBALANCED           = RC_VER1 + 0x031,
    ATHW_TPM_RC_COMMAND_SIZE         = RC_VER1 + 0x042,
    ATHW_TPM_RC_COMMAND_CODE         = RC_VER1 + 0x043,
    ATHW_TPM_RC_AUTHSIZE             = RC_VER1 + 0x044,
    ATHW_TPM_RC_AUTH_CONTEXT         = RC_VER1 + 0x045,
    ATHW_TPM_RC_NV_RANGE             = RC_VER1 + 0x046,
    ATHW_TPM_RC_NV_SIZE              = RC_VER1 + 0x047,
    ATHW_TPM_RC_NV_LOCKED            = RC_VER1 + 0x048,
    ATHW_TPM_RC_NV_AUTHORIZATION     = RC_VER1 + 0x049,
    ATHW_TPM_RC_NV_UNINITIALIZED     = RC_VER1 + 0x04A,
    ATHW_TPM_RC_NV_SPACE             = RC_VER1 + 0x04B,
    ATHW_TPM_RC_NV_DEFINED           = RC_VER1 + 0x04C,
    ATHW_TPM_RC_BAD_CONTEXT          = RC_VER1 + 0x050,
    ATHW_TPM_RC_CPHASH               = RC_VER1 + 0x051,
    ATHW_TPM_RC_PARENT               = RC_VER1 + 0x052,
    ATHW_TPM_RC_NEEDS_TEST           = RC_VER1 + 0x053,
    ATHW_TPM_RC_NO_RESULT            = RC_VER1 + 0x054,
    ATHW_TPM_RC_SENSITIVE            = RC_VER1 + 0x055,
    RC_MAX_FM0                  = RC_VER1 + 0x07F,

    RC_FMT1 = 0x080,
    ATHW_TPM_RC_ASYMMETRIC       = RC_FMT1 + 0x001,
    ATHW_TPM_RC_ATTRIBUTES       = RC_FMT1 + 0x002,
    ATHW_TPM_RC_HASH             = RC_FMT1 + 0x003,
    ATHW_TPM_RC_VALUE            = RC_FMT1 + 0x004,
    ATHW_TPM_RC_HIERARCHY        = RC_FMT1 + 0x005,
    ATHW_TPM_RC_KEY_SIZE         = RC_FMT1 + 0x007,
    ATHW_TPM_RC_MGF              = RC_FMT1 + 0x008,
    ATHW_TPM_RC_MODE             = RC_FMT1 + 0x009,
    ATHW_TPM_RC_TYPE             = RC_FMT1 + 0x00A,
    ATHW_TPM_RC_HANDLE           = RC_FMT1 + 0x00B,
    ATHW_TPM_RC_KDF              = RC_FMT1 + 0x00C,
    ATHW_TPM_RC_RANGE            = RC_FMT1 + 0x00D,
    ATHW_TPM_RC_AUTH_FAIL        = RC_FMT1 + 0x00E,
    ATHW_TPM_RC_NONCE            = RC_FMT1 + 0x00F,
    ATHW_TPM_RC_PP               = RC_FMT1 + 0x010,
    ATHW_TPM_RC_SCHEME           = RC_FMT1 + 0x012,
    ATHW_TPM_RC_SIZE             = RC_FMT1 + 0x015,
    ATHW_TPM_RC_SYMMETRIC        = RC_FMT1 + 0x016,
    ATHW_TPM_RC_TAG              = RC_FMT1 + 0x017,
    ATHW_TPM_RC_SELECTOR         = RC_FMT1 + 0x018,
    ATHW_TPM_RC_INSUFFICIENT     = RC_FMT1 + 0x01A,
    ATHW_TPM_RC_SIGNATURE        = RC_FMT1 + 0x01B,
    ATHW_TPM_RC_KEY              = RC_FMT1 + 0x01C,
    ATHW_TPM_RC_POLICY_FAIL      = RC_FMT1 + 0x01D,
    ATHW_TPM_RC_INTEGRITY        = RC_FMT1 + 0x01F,
    ATHW_TPM_RC_TICKET           = RC_FMT1 + 0x020,
    ATHW_TPM_RC_RESERVED_BITS    = RC_FMT1 + 0x021,
    ATHW_TPM_RC_BAD_AUTH         = RC_FMT1 + 0x022,
    ATHW_TPM_RC_EXPIRED          = RC_FMT1 + 0x023,
    ATHW_TPM_RC_POLICY_CC        = RC_FMT1 + 0x024,
    ATHW_TPM_RC_BINDING          = RC_FMT1 + 0x025,
    ATHW_TPM_RC_CURVE            = RC_FMT1 + 0x026,
    ATHW_TPM_RC_ECC_POINT        = RC_FMT1 + 0x027,
    RC_MAX_FMT1             = RC_FMT1 + 0x03F,

    RC_WARN = 0x900,
    ATHW_TPM_RC_CONTEXT_GAP      = RC_WARN + 0x001,
    ATHW_TPM_RC_OBJECT_MEMORY    = RC_WARN + 0x002,
    ATHW_TPM_RC_SESSION_MEMORY   = RC_WARN + 0x003,
    ATHW_TPM_RC_MEMORY           = RC_WARN + 0x004,
    ATHW_TPM_RC_SESSION_HANDLES  = RC_WARN + 0x005,
    ATHW_TPM_RC_OBJECT_HANDLES   = RC_WARN + 0x006,
    ATHW_TPM_RC_LOCALITY         = RC_WARN + 0x007,
    ATHW_TPM_RC_YIELDED          = RC_WARN + 0x008,
    ATHW_TPM_RC_CANCELED         = RC_WARN + 0x009,
    ATHW_TPM_RC_TESTING          = RC_WARN + 0x00A,
    ATHW_TPM_RC_REFERENCE_H0     = RC_WARN + 0x010,
    ATHW_TPM_RC_REFERENCE_H1     = RC_WARN + 0x011,
    ATHW_TPM_RC_REFERENCE_H2     = RC_WARN + 0x012,
    ATHW_TPM_RC_REFERENCE_H3     = RC_WARN + 0x013,
    ATHW_TPM_RC_REFERENCE_H4     = RC_WARN + 0x014,
    ATHW_TPM_RC_REFERENCE_H5     = RC_WARN + 0x015,
    ATHW_TPM_RC_REFERENCE_H6     = RC_WARN + 0x016,
    ATHW_TPM_RC_REFERENCE_S0     = RC_WARN + 0x018,
    ATHW_TPM_RC_REFERENCE_S1     = RC_WARN + 0x019,
    ATHW_TPM_RC_REFERENCE_S2     = RC_WARN + 0x01A,
    ATHW_TPM_RC_REFERENCE_S3     = RC_WARN + 0x01B,
    ATHW_TPM_RC_REFERENCE_S4     = RC_WARN + 0x01C,
    ATHW_TPM_RC_REFERENCE_S5     = RC_WARN + 0x01D,
    ATHW_TPM_RC_REFERENCE_S6     = RC_WARN + 0x01E,
    ATHW_TPM_RC_NV_RATE          = RC_WARN + 0x020,
    ATHW_TPM_RC_LOCKOUT          = RC_WARN + 0x021,
    ATHW_TPM_RC_RETRY            = RC_WARN + 0x022,
    ATHW_TPM_RC_NV_UNAVAILABLE   = RC_WARN + 0x023,
    RC_MAX_WARN             = RC_WARN + 0x03F,

    ATHW_TPM_RC_NOT_USED         = RC_WARN + 0x07F,

    ATHW_TPM_RC_H        = 0x000,
    ATHW_TPM_RC_P        = 0x040,
    ATHW_TPM_RC_S        = 0x800,
    ATHW_TPM_RC_1        = 0x100,
    ATHW_TPM_RC_2        = 0x200,
    ATHW_TPM_RC_3        = 0x300,
    ATHW_TPM_RC_4        = 0x400,
    ATHW_TPM_RC_5        = 0x500,
    ATHW_TPM_RC_6        = 0x600,
    ATHW_TPM_RC_7        = 0x700,
    ATHW_TPM_RC_8        = 0x800,
    ATHW_TPM_RC_9        = 0x900,
    ATHW_TPM_RC_A        = 0xA00,
    ATHW_TPM_RC_B        = 0xB00,
    ATHW_TPM_RC_C        = 0xC00,
    ATHW_TPM_RC_D        = 0xD00,
    ATHW_TPM_RC_E        = 0xE00,
    ATHW_TPM_RC_F        = 0xF00,
    ATHW_TPM_RC_N_MASK   = 0xF00,

    /* use negative codes for internal errors */
    ATHW_TPM_RC_TIMEOUT = -100,
} tpm_ret_t;

/* Errors from wolfssl/wolfcrypt/error-crypt.h */
#define BAD_MUTEX_E           -106  /* Bad mutex operation */
#define WC_TIMEOUT_E          -107  /* timeout error */
#define MEMORY_E              -125  /* out of memory error */
#define BUFFER_E              -132  /* output buffer too small or input too large */
#define BAD_FUNC_ARG          -173  /* Bad function argument provided */
#define NOT_COMPILED_IN       -174  /* Feature not compiled in */
#define LENGTH_ONLY_E         -202  /* Returning output length only */
#define INPUT_SIZE_E          -412  /* input size too big error */


//int athw_tpm2_init(void *handle, int tmotries);

#ifdef __cplusplus
}
#endif

#endif
