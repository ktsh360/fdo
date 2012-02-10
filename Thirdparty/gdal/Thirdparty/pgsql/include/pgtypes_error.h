/* $PostgreSQL: pgsql/src/interfaces/ecpg/include/pgtypes_error.h,v 1.8 2006/08/15 06:40:19 meskes Exp $ */

#define PGTYPES_NUM_OVERFLOW		301
#define PGTYPES_NUM_BAD_NUMERIC		302
#define PGTYPES_NUM_DIVIDE_ZERO		303
#define PGTYPES_NUM_UNDERFLOW		304

#define PGTYPES_DATE_BAD_DATE		310
#define PGTYPES_DATE_ERR_EARGS		311
#define PGTYPES_DATE_ERR_ENOSHORTDATE	312
#define PGTYPES_DATE_ERR_ENOTDMY	313
#define PGTYPES_DATE_BAD_DAY		314
#define PGTYPES_DATE_BAD_MONTH		315

#define PGTYPES_TS_BAD_TIMESTAMP	320
#define PGTYPES_TS_ERR_EINFTIME		321

#define PGTYPES_INTVL_BAD_INTERVAL	330
