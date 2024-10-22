/*
*********************************************************************************************************
*
*                                            CPU PORT FILE
*
*                                            ARM-Cortex-M4
*                                            GNU C Compiler
*
* Filename      : cpu_mb.h
* Version       : V0.0.00 2024/8/21
* Programmer(s) : ZGQ
*                 FT
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This CPU header file is protected from multiple pre-processor inclusion through use of
*               the  CPU module present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  CPU_MODULE_PRESENT                                     /* See Note #1.                                         */
#define  CPU_MODULE_PRESENT
/*$PAGE*/
/*
*********************************************************************************************************
*                                    CONFIGURE STANDARD DATA TYPES
*
* Note(s) : (1) Configure standard data types according to CPU-/compiler-specifications.
*
*           (2) (a) (1) 'CPU_FNCT_VOID' data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has no arguments.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_VOID  FnctName;
*
*                           FnctName();
*
*               (b) (1) 'CPU_FNCT_PTR'  data type defined to replace the commonly-used function pointer
*                       data type of a pointer to a function which returns void & has a single void
*                       pointer argument.
*
*                   (2) Example function pointer usage :
*
*                           CPU_FNCT_PTR   FnctName;
*                           void          *p_obj
*
*                           FnctName(p_obj);
*********************************************************************************************************
*/

typedef            void        CPU_VOID;
typedef            char        CPU_CHAR;                        /*  8-bit character                                     */
typedef  unsigned  char        CPU_BOOLEAN;                     /*  8-bit boolean or logical                            */
typedef  unsigned  char        CPU_INT08U;                      /*  8-bit unsigned integer                              */
typedef    signed  char        CPU_INT08S;                      /*  8-bit   signed integer                              */
typedef  unsigned  short       CPU_INT16U;                      /* 16-bit unsigned integer                              */
typedef    signed  short       CPU_INT16S;                      /* 16-bit   signed integer                              */
typedef  unsigned  int         CPU_INT32U;                      /* 32-bit unsigned integer                              */
typedef    signed  int         CPU_INT32S;                      /* 32-bit   signed integer                              */
typedef  unsigned  long  long  CPU_INT64U;                      /* 64-bit unsigned integer                              */
typedef    signed  long  long  CPU_INT64S;                      /* 64-bit   signed integer                              */

typedef            float       CPU_FP32;                        /* 32-bit floating point                                */
typedef            double      CPU_FP64;                        /* 64-bit floating point                                */


typedef  volatile  CPU_INT08U  CPU_REG08;                       /*  8-bit register                                      */
typedef  volatile  CPU_INT16U  CPU_REG16;                       /* 16-bit register                                      */
typedef  volatile  CPU_INT32U  CPU_REG32;                       /* 32-bit register                                      */
typedef  volatile  CPU_INT64U  CPU_REG64;                       /* 64-bit register                                      */


typedef            void      (*CPU_FNCT_VOID)(void);            /* See Note #2a.                                        */
typedef            void      (*CPU_FNCT_PTR )(void *p_obj);     /* See Note #2b.                                        */

#define  DEF_FALSE                                         0u
#define  DEF_TRUE                                          1u

#define  CPU_CRITICAL_ENTER()  do { __disable_irq(); } while (0)          /* Disable   interrupts.                        */
#define  CPU_CRITICAL_EXIT()   do { __enable_irq();  } while (0)          /* Re-enable interrupts.                        */


/*$PAGE*/
/*
*********************************************************************************************************
*                                             MODULE END
*
* Note(s) : (1) See 'cpu.h  MODULE'.
*********************************************************************************************************
*/

#endif                                                          /* End of CPU module include.                           */

