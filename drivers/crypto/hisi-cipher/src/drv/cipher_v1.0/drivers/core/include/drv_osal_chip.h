#ifndef __DRV_OSAL_CHIP_H__
#define __DRV_OSAL_CHIP_H__

#if defined(CHIP_TYPE_hi3559av100es)
#include "drv_osal_hi3559aes.h"

#elif defined(CHIP_TYPE_hi3559av100)
#include "drv_osal_hi3559.h"

#elif (defined(CHIP_TYPE_hi3519av100) || defined(CHIP_TYPE_hi3556av100))
#include "drv_osal_hi3519av100.h"

#elif (defined(CHIP_TYPE_hi3516cv500) || defined(CHIP_TYPE_hi3516dv300) || defined(CHIP_TYPE_hi3556v200) || defined(CHIP_TYPE_hi3559v200) || defined(CHIP_TYPE_hi3516av300))
#include "drv_osal_hi3516cv500.h"

#elif (defined(CHIP_TYPE_hi3516ev200) || (CHIP_TYPE_hi3516ev300) || (CHIP_TYPE_hi3518ev300) || (CHIP_TYPE_hi3516dv200))
#include "drv_osal_hi3516ev200.h"

#else
#error You need to define a configuration file for chip !
#endif

#if defined(CHIP_HASH_VER_V100) || defined(CHIP_HASH_VER_V200)
#define CHIP_HASH_SUPPORT
#endif

#if defined(CHIP_SYMC_VER_V100) || defined(CHIP_SYMC_VER_V200)
#define CHIP_SYMC_SUPPORT
#endif

#if defined(CHIP_TRNG_VER_V100) || defined(CHIP_TRNG_VER_V200)
#define CHIP_TRNG_SUPPORT
#endif

#if defined(CHIP_IFEP_RSA_VER_V100) || defined(CHIP_SIC_RSA_VER_V100)
#define CHIP_RSA_SUPPORT
#endif

#if defined(CHIP_HDCP_VER_V100) || defined(CHIP_HDCP_VER_V200)
#define CHIP_HDCP_SUPPORT
#endif

#if defined(CHIP_SM2_VER_V100)
#define CHIP_SM2_SUPPORT
#endif

#endif
