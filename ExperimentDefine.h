#ifndef EXPERIMENT_DEFINE

#define EXPERIMENT_DEFINE

#define ONE_NAO_VERSION

#ifdef ONE_NAO_VERSION
#define HEAD_NUM 1
#define OBJ_NUM  4
#endif

#ifdef TWO_NAOS_VERSION
#define HEAD_NUM 2
#define OBJ_NUM  4
#endif

#ifdef FIVE_OBJ_VERSION
#define HEAD_NUM 1
#define OBJ_NUM 5
#endif

#ifdef FIRST_VERSION
#define HEAD_NUM 1
#define OBJ_NUM 3
#endif

#endif