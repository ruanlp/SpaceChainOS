/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: arm_support.h
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2013 �� 12 �� 09 ��
**
** ��        ��: ARM ��ϵ����֧��.
*********************************************************************************************************/

#ifndef __ARCH_ARM_SUPPORT_H
#define __ARCH_ARM_SUPPORT_H

#define __LW_SCHEDULER_BUG_TRACE_EN                                     /*  ���Զ�˵�����              */
#define __LW_KERNLOCK_BUG_TRACE_EN                                      /*  �����ں���                  */

/*********************************************************************************************************
  ������ͷ�ļ�
*********************************************************************************************************/

#include "arch/assembler.h"

/*********************************************************************************************************
  �洢������ (CPU ջ������)
*********************************************************************************************************/

#define CPU_STK_GROWTH              1                                   /*  1����ջ�Ӹߵ�ַ��͵�ַ     */
                                                                        /*  0����ջ�ӵ͵�ַ��ߵ�ַ     */
/*********************************************************************************************************
  arch �Ѿ��ṩ�Ľӿ�����:
*********************************************************************************************************/

#define __ARCH_KERNEL_PARAM         archKernelParam

VOID    archKernelParam(CPCHAR  pcParam);

/*********************************************************************************************************
  ARM ����������
*********************************************************************************************************/

VOID    archAssert(INT  iCond, CPCHAR  pcFunc, CPCHAR  pcFile, INT  iLine);

/*********************************************************************************************************
  ARM �������߳���������ؽӿ�
*********************************************************************************************************/

PLW_STACK       archTaskCtxCreate(PTHREAD_START_ROUTINE  pfuncTask,
                                  PVOID                  pvArg,
                                  PLW_STACK              pstkTop, 
                                  ULONG                  ulOpt);
VOID            archTaskCtxSetFp(PLW_STACK  pstkDest, PLW_STACK  pstkSrc);
ARCH_REG_CTX   *archTaskRegsGet(PLW_STACK  pstkTop, ARCH_REG_T *pregSp);
VOID            archTaskRegsSet(PLW_STACK  pstkTop, const ARCH_REG_CTX  *pregctx);

#if LW_CFG_DEVICE_EN > 0
VOID        archTaskCtxShow(INT  iFd, PLW_STACK  pstkTop);
#endif                                                                  /*  LW_CFG_DEVICE_EN > 0        */
VOID        archTaskCtxPrint(PVOID  pvBuffer, size_t  stSize, PLW_STACK  pstkTop);

VOID        archTaskCtxStart(PLW_CLASS_CPU  pcpuSw);
VOID        archTaskCtxSwitch(PLW_CLASS_CPU  pcpuSw);

#if LW_CFG_COROUTINE_EN > 0
VOID        archCrtCtxSwitch(PLW_CLASS_CPU  pcpuSw);
#endif                                                                  /*  LW_CFG_COROUTINE_EN > 0     */

VOID        archIntCtxLoad(PLW_CLASS_CPU  pcpuSw);
VOID        archSigCtxLoad(PVOID  pvStack);

/*********************************************************************************************************
  ARM ���������Խӿ�
*********************************************************************************************************/

#if LW_CFG_GDB_EN > 0
VOID    archDbgBpInsert(addr_t   ulAddr, size_t stSize, ULONG  *pulIns, BOOL  bLocal);
VOID    archDbgAbInsert(addr_t   ulAddr, ULONG  *pulIns);
VOID    archDbgBpRemove(addr_t   ulAddr, size_t stSize, ULONG   ulIns, BOOL  bLocal);
VOID    archDbgBpPrefetch(addr_t ulAddr);
UINT    archDbgTrapType(addr_t   ulAddr, PVOID   pvArch);
VOID    archDbgBpAdjust(PVOID  pvDtrace, PVOID   pvtm);
#endif                                                                  /*  LW_CFG_GDB_EN > 0           */

/*********************************************************************************************************
  ARM �������쳣�ӿ�
*********************************************************************************************************/

#if !defined(__SYLIXOS_ARM_ARCH_M__)
VOID    archIntEntry(VOID);
VOID    archAbtEntry(VOID);
VOID    archPreEntry(VOID);
VOID    archUndEntry(VOID);
VOID    archSwiEntry(VOID);

#else
VOID    archResetHandle(VOID);
VOID    archNMIIntEntry(VOID);
VOID    archHardFaultEntry(VOID);
VOID    archMemFaultEntry(VOID);
VOID    archBusFaultEntry(VOID);
VOID    archUsageFaultEntry(VOID);
VOID    archDebugMonitorEntry(VOID);
VOID    archPendSVEntry(VOID);
VOID    archSysTickIntEntry(VOID);
VOID    archSvcEntry(VOID);
VOID    archReservedIntEntry(VOID);
VOID    archIntEntry16(VOID);                                           /*  archIntEntry16 ~ 255        */
#endif                                                                  /*  !__SYLIXOS_ARM_ARCH_M__     */

VOID    archIntHandle(ULONG  ulVector, BOOL  bPreemptive);              /*  bspIntHandle ��Ҫ���ô˺��� */

/*********************************************************************************************************
  ARM ͨ�ÿ�
*********************************************************************************************************/

INT     archFindLsb(UINT32 ui32);
INT     archFindMsb(UINT32 ui32);

/*********************************************************************************************************
  ARM ��������׼�ײ��
*********************************************************************************************************/

#define KN_INT_DISABLE()            archIntDisable()
#define KN_INT_ENABLE(intLevel)     archIntEnable(intLevel)
#define KN_INT_ENABLE_FORCE()       archIntEnableForce()

INTREG  archIntDisable(VOID);
VOID    archIntEnable(INTREG  iregInterLevel);
VOID    archIntEnableForce(VOID);

VOID    archPageCopy(PVOID pvTo, PVOID pvFrom);

#define KN_COPY_PAGE(to, from)      archPageCopy(to, from)

VOID    archReboot(INT  iRebootType, addr_t  ulStartAddress);

#if !defined(__SYLIXOS_ARM_ARCH_M__)
INTREG  archGetCpsr(VOID);
#endif

/*********************************************************************************************************
  ARM ������ BogoMIPS ѭ��
*********************************************************************************************************/

VOID    archBogoMipsLoop(ULONG  ulLoop);

#define __ARCH_BOGOMIPS_LOOP            archBogoMipsLoop
#define __ARCH_BOGOMIPS_INS_PER_LOOP    2

/*********************************************************************************************************
  ARM CP15 ��������
*********************************************************************************************************/

#if LW_CFG_ARM_CP15 > 0
VOID    armWaitForInterrupt(VOID);
VOID    armFastBusMode(VOID);
VOID    armAsyncBusMode(VOID);
VOID    armSyncBusMode(VOID);
#endif                                                                  /*  LW_CFG_ARM_CP15 > 0         */

/*********************************************************************************************************
  ARM ������ CACHE ����
*********************************************************************************************************/

#define ARM_MACHINE_920     "920"                                       /*  ARMv4                       */
#define ARM_MACHINE_926     "926"                                       /*  ARMv5                       */
#define ARM_MACHINE_1136    "1136"                                      /*  ARMv6                       */
#define ARM_MACHINE_1176    "1176"

#define ARM_MACHINE_A5      "A5"                                        /*  ARMv7                       */
#define ARM_MACHINE_A7      "A7"
#define ARM_MACHINE_A8      "A8"
#define ARM_MACHINE_A9      "A9"
#define ARM_MACHINE_A15     "A15"
#define ARM_MACHINE_A17     "A17"
#define ARM_MACHINE_A53     "A53"                                       /*  ARMv8                       */
#define ARM_MACHINE_A57     "A57"                                       /*  FT1500A FT2000 ...          */
#define ARM_MACHINE_A72     "A72"
#define ARM_MACHINE_A73     "A73"

#define ARM_MACHINE_R4      "R4"                                        /*  ARMv7 R                     */
#define ARM_MACHINE_R5      "R5"
#define ARM_MACHINE_R7      "R7"

#define ARM_MACHINE_M3      "M3"                                        /*  ARMv7 M                     */
#define ARM_MACHINE_M4      "M4"
#define ARM_MACHINE_M7      "M7"

#if LW_CFG_CACHE_EN > 0
VOID    archCacheReset(CPCHAR     pcMachineName);
VOID    archCacheInit(CACHE_MODE  uiInstruction, CACHE_MODE  uiData, CPCHAR  pcMachineName);

#define __ARCH_CACHE_INIT   archCacheInit
#endif                                                                  /*  LW_CFG_CACHE_EN > 0         */

/*********************************************************************************************************
  ARM ������ MMU ���� (armMmu*() �ɹ��� bsp ʹ��)
*********************************************************************************************************/

#if LW_CFG_VMM_EN > 0
VOID    archMmuInit(CPCHAR  pcMachineName);

#define __ARCH_MMU_INIT     archMmuInit

VOID    armMmuV7ForceShare(BOOL  bEnOrDis);                             /*  ǿ����λ����λ              */
VOID    armMmuV7ForceNonSecure(BOOL  bEnOrDis);                         /*  ǿ��ʹ�� Non-Secure ģʽ    */

#define ARM_MMU_V7_DEV_STRONGLY_ORDERED     0                           /*  �豸�ڴ�ǿ����              */
#define ARM_MMU_V7_DEV_SHAREABLE            1                           /*  �����豸�ڴ� (Ĭ��)         */
#define ARM_MMU_V7_DEV_NON_SHAREABLE        2                           /*  �ǹ����豸�ڴ�              */

VOID    armMmuV7ForceDevType(UINT  uiType);                             /*  ѡ���豸�ڴ�����            */
#endif                                                                  /*  LW_CFG_VMM_EN > 0           */

/*********************************************************************************************************
  ARM ������ MPU ���� (bsp ��ʼ���׶�ֱ�ӵ������溯����ʼ�� MPU)
  ע��: archMpuInit() �����Ὺ�� MPU.
        Cortex-M bsp �ڵ��ô˺���ǰ, �����ú� MPU CTRL �Ĵ��� MPU HFNMI and PRIVILEGED Access control
        Cortex-M bsp �ڵ��ô˺�����, ��ʹ�� MEM FAULT �쳣, ����쳣�п�ѡ����ò���ϵͳ��ؽӿڴ�ӡ����ջ
        �Լ�����������. Ȼ��λ.
*********************************************************************************************************/

#if (LW_CFG_VMM_EN == 0) && (LW_CFG_ARM_MPU > 0)
VOID    archMpuInit(CPCHAR  pcMachineName, const ARM_MPU_REGION  mpuregion[]);
#endif

/*********************************************************************************************************
  ARM �������������������
*********************************************************************************************************/

#if LW_CFG_SMP_EN > 0
#if LW_CFG_ARM_SPINLOCK_DEP_CACHE > 0
VOID    archSpinWork(VOID);

#define __ARCH_SPIN_WORK    archSpinWork
#endif                                                                  /*  LW_CFG_ARM_SPINLOCK_DEP_C...*/

VOID    archSpinInit(spinlock_t  *psl);
VOID    archSpinDelay(VOID);
VOID    archSpinNotify(VOID);

#define __ARCH_SPIN_INIT    archSpinInit
#define __ARCH_SPIN_DELAY   archSpinDelay
#define __ARCH_SPIN_NOTIFY  archSpinNotify

INT     archSpinLock(spinlock_t  *psl);
INT     archSpinTryLock(spinlock_t  *psl);
INT     archSpinUnlock(spinlock_t  *psl);

#define __ARCH_SPIN_LOCK    archSpinLock
#define __ARCH_SPIN_TRYLOCK archSpinTryLock
#define __ARCH_SPIN_UNLOCK  archSpinUnlock

INT     archSpinLockRaw(spinlock_t  *psl);
INT     archSpinTryLockRaw(spinlock_t  *psl);
INT     archSpinUnlockRaw(spinlock_t  *psl);

#define __ARCH_SPIN_LOCK_RAW    archSpinLockRaw
#define __ARCH_SPIN_TRYLOCK_RAW archSpinTryLockRaw
#define __ARCH_SPIN_UNLOCK_RAW  archSpinUnlockRaw

ULONG   archMpCur(VOID);
VOID    archMpInt(ULONG  ulCPUId);
#endif                                                                  /*  LW_CFG_SMP_EN               */

/*********************************************************************************************************
  ARM �ڴ�����
*********************************************************************************************************/

#ifdef __GNUC__
#if __SYLIXOS_ARM_ARCH__ >= 7
#define armIsb()        __asm__ __volatile__ ("isb" : : : "memory")
#define armDsb()        __asm__ __volatile__ ("dsb" : : : "memory")
#define armDmb()        __asm__ __volatile__ ("dmb" : : : "memory")

#elif __SYLIXOS_ARM_ARCH__ == 6
#define armIsb()        __asm__ __volatile__ ("mcr p15, 0, %0, c7, c5,  4" \
                                              : : "r" (0) : "memory")
#define armDsb()        __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 4" \
                                              : : "r" (0) : "memory")
#define armDmb()        __asm__ __volatile__ ("mcr p15, 0, %0, c7, c10, 5" \
                                              : : "r" (0) : "memory")
#else
#define armIsb()        __asm__ __volatile__ ("" : : : "memory")
#define armDsb()        __asm__ __volatile__ ("" : : : "memory")
#define armDmb()        __asm__ __volatile__ ("" : : : "memory")
#endif                                                                  /*  __SYLIXOS_ARM_ARCH__        */

#else
VOID    armIsb(VOID);
VOID    armDsb(VOID);
VOID    armDmb(VOID);
#endif                                                                  /*  __GNUC__                    */


#if __SYLIXOS_ARM_ARCH__ >= 7 && LW_CFG_ARM_CACHE_L2 > 0
VOID    armL2Sync(VOID);
#define KN_MB()         do { armDsb(); armL2Sync(); } while(0)
#else
#define KN_MB()         armDsb()
#endif                                                                  /*  __SYLIXOS_ARM_ARCH__ >= 7   */
                                                                        /*  LW_CFG_ARM_CACHE_L2 > 0     */
#define KN_RMB()        armDsb()
#define KN_WMB()        KN_MB()

#if LW_CFG_SMP_EN > 0
#define KN_SMP_MB()     armDmb()
#define KN_SMP_RMB()    armDmb()
#define KN_SMP_WMB()    armDmb()

#else
#ifdef __GNUC__
#define KN_SMP_MB()     __asm__ __volatile__ ("" : : : "memory")
#define KN_SMP_RMB()    __asm__ __volatile__ ("" : : : "memory")
#define KN_SMP_WMB()    __asm__ __volatile__ ("" : : : "memory")

#else
#define KN_SMP_MB()     
#define KN_SMP_RMB()
#define KN_SMP_WMB()
#endif                                                                  /*  __GNUC__                    */
#endif                                                                  /*  LW_CFG_SMP_EN               */

#ifdef __GNUC__
#define KN_BARRIER()    __asm__ __volatile__ ("" : : : "memory")
#else
#define KN_BARRIER()
#endif                                                                  /*  __SYLIXOS_ARM_ARCH__ >= 7   */

/*********************************************************************************************************
  ARM ���������������� 
  ע��: neon ��Ӧ�ĸ���������Ϊ ARM_FPU_VFPv3 ���� ARM_FPU_VFPv4
  VFP9 ��ʾ VFPv2 for ARM9 ������ VFP11 ��ʾ VFPv2 for ARM11 ������, VFP11 VFPv3 VFPv4 �������жϼĴ�����
  ARM_FPU_VFP9 ��Ҫ�û�ѡ��Ĵ��������� D16 ���� D32.
  ����Ӳ����֧�ֵ� Cortex-M bsp ��ѡ�� vfpv4 ����.
*********************************************************************************************************/

#define ARM_FPU_NONE        "none"
#define ARM_FPU_VFP9_D16    "vfp9-d16"
#define ARM_FPU_VFP9_D32    "vfp9-d32"
#define ARM_FPU_VFP11       "vfp11"
#define ARM_FPU_VFPv3       "vfpv3"
#define ARM_FPU_VFPv4       "vfpv4"
#define ARM_FPU_NEONv3      ARM_FPU_VFPv3                               /*  Context same as vfpv3       */
#define ARM_FPU_NEONv4      ARM_FPU_VFPv4                               /*  Context same as vfpv4       */

#if LW_CFG_CPU_FPU_EN > 0
VOID    archFpuPrimaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);
#if LW_CFG_SMP_EN > 0
VOID    archFpuSecondaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);
#endif                                                                  /*  LW_CFG_SMP_EN               */

VOID    archFpuCtxInit(PVOID pvFpuCtx);
VOID    archFpuEnable(VOID);
VOID    archFpuDisable(VOID);
VOID    archFpuSave(PVOID pvFpuCtx);
VOID    archFpuRestore(PVOID pvFpuCtx);

#define __ARCH_FPU_CTX_INIT     archFpuCtxInit
#define __ARCH_FPU_ENABLE       archFpuEnable
#define __ARCH_FPU_DISABLE      archFpuDisable
#define __ARCH_FPU_SAVE         archFpuSave
#define __ARCH_FPU_RESTORE      archFpuRestore

#if LW_CFG_DEVICE_EN > 0
VOID    archFpuCtxShow(INT  iFd, PVOID pvFpuCtx);
#define __ARCH_FPU_CTX_SHOW     archFpuCtxShow
#endif                                                                  /*  #if LW_CFG_DEVICE_EN        */

INT     archFpuUndHandle(PLW_CLASS_TCB  ptcbCur);
#endif                                                                  /*  LW_CFG_CPU_FPU_EN           */

/*********************************************************************************************************
  bsp ��Ҫ�ṩ�Ľӿ�����:
*********************************************************************************************************/
/*********************************************************************************************************
  ARM �������쳣�ص� (�ɲ�����)
*********************************************************************************************************/

#if LW_CFG_CPU_EXC_HOOK_EN > 0
#define ARCH_INSTRUCTION_EXCEPTION  0                                   /*  iExcType                    */
#define ARCH_DATA_EXCEPTION         1
#define ARCH_MACHINE_EXCEPTION      2
#define ARCH_BUS_EXCEPTION          3

INT     bspCpuExcHook(PLW_CLASS_TCB   ptcb,
                      addr_t          ulRetAddr,
                      addr_t          ulExcAddr,
                      INT             iExcType, 
                      INT             iExcInfo);
#endif                                                                  /*  LW_CFG_CPU_EXC_HOOK_EN      */

/*********************************************************************************************************
  ARM �������ж������ж�
*********************************************************************************************************/

VOID    bspIntInit(VOID);
VOID    bspIntHandle(VOID);

VOID    bspIntVectorEnable(ULONG  ulVector);
VOID    bspIntVectorDisable(ULONG  ulVector);
BOOL    bspIntVectorIsEnable(ULONG  ulVector);

#define __ARCH_INT_VECTOR_ENABLE    bspIntVectorEnable
#define __ARCH_INT_VECTOR_DISABLE   bspIntVectorDisable
#define __ARCH_INT_VECTOR_ISENABLE  bspIntVectorIsEnable

#if LW_CFG_INTER_PRIO > 0
ULONG   bspIntVectorSetPriority(ULONG  ulVector, UINT  uiPrio);
ULONG   bspIntVectorGetPriority(ULONG  ulVector, UINT *puiPrio);

#define __ARCH_INT_VECTOR_SETPRIO   bspIntVectorSetPriority
#define __ARCH_INT_VECTOR_GETPRIO   bspIntVectorGetPriority
#endif                                                                  /*  LW_CFG_INTER_PRIO > 0       */

#if LW_CFG_INTER_TARGET > 0
ULONG   bspIntVectorSetTarget(ULONG  ulVector, size_t  stSize, const PLW_CLASS_CPUSET  pcpuset);
ULONG   bspIntVectorGetTarget(ULONG  ulVector, size_t  stSize, PLW_CLASS_CPUSET  pcpuset);

#define __ARCH_INT_VECTOR_SETTARGET bspIntVectorSetTarget
#define __ARCH_INT_VECTOR_GETTARGET bspIntVectorGetTarget
#endif                                                                  /*  LW_CFG_INTER_TARGET > 0     */

/*********************************************************************************************************
  CPU ��ʱ��ʱ��
*********************************************************************************************************/

VOID    bspTickInit(VOID);
VOID    bspDelayUs(ULONG ulUs);
VOID    bspDelayNs(ULONG ulNs);

/*********************************************************************************************************
  TICK �߾���ʱ������
  ��δʵ����ɶ���Ϊ�պ���, ͬʱϵͳ��ʼ��ǰ����� API_TimeNanoSleepMethod(LW_TIME_NANOSLEEP_METHOD_TICK)
*********************************************************************************************************/

#if LW_CFG_TIME_HIGH_RESOLUTION_EN > 0
VOID    bspTickHighResolution(struct timespec *ptv);
#endif                                                                  /*  LW_CFG_TIME_HIGH_...        */

/*********************************************************************************************************
  �ں˹ؼ�λ�ûص�����
*********************************************************************************************************/

VOID    bspTaskCreateHook(LW_OBJECT_ID  ulId);
VOID    bspTaskDeleteHook(LW_OBJECT_ID  ulId, PVOID  pvReturnVal, PLW_CLASS_TCB  ptcb);
VOID    bspTaskSwapHook(LW_OBJECT_HANDLE  hOldThread, LW_OBJECT_HANDLE  hNewThread);
VOID    bspTaskIdleHook(VOID);
VOID    bspTickHook(INT64  i64Tick);
VOID    bspWdTimerHook(LW_OBJECT_ID  ulId);
VOID    bspTCBInitHook(LW_OBJECT_ID  ulId, PLW_CLASS_TCB   ptcb);
VOID    bspKernelInitHook(VOID);

/*********************************************************************************************************
  ϵͳ�������� (ulStartAddress �������ڵ���, BSP �ɺ���)
*********************************************************************************************************/

VOID    bspReboot(INT  iRebootType, addr_t  ulStartAddress);

/*********************************************************************************************************
  ϵͳ�ؼ���Ϣ��ӡ (��ӡ��Ϣ���������κβ���ϵͳ api)
*********************************************************************************************************/

VOID    bspDebugMsg(CPCHAR pcMsg);

/*********************************************************************************************************
  BSP ��Ϣ
*********************************************************************************************************/

CPCHAR  bspInfoCpu(VOID);
CPCHAR  bspInfoCache(VOID);
CPCHAR  bspInfoPacket(VOID);
CPCHAR  bspInfoVersion(VOID);
ULONG   bspInfoHwcap(VOID);
addr_t  bspInfoRomBase(VOID);
size_t  bspInfoRomSize(VOID);
addr_t  bspInfoRamBase(VOID);
size_t  bspInfoRamSize(VOID);

/*********************************************************************************************************
  ARM ������ CACHE ����
*********************************************************************************************************/

#if LW_CFG_ARM_CACHE_L2 > 0
INT     bspL2CBase(addr_t *pulBase);                                    /*  L2 cache ����������ַ       */
INT     bspL2CAux(UINT32 *puiAuxVal, UINT32 *puiAuxMask);               /*  L2 cache aux                */
#endif                                                                  /*  LW_CFG_ARM_CACHE_L2 > 0     */

/*********************************************************************************************************
  ARM ������ MMU ����
*********************************************************************************************************/

#if LW_CFG_VMM_EN > 0
ULONG   bspMmuPgdMaxNum(VOID);
ULONG   bspMmuPteMaxNum(VOID);
#endif                                                                  /*  LW_CFG_VMM_EN > 0           */

/*********************************************************************************************************
  ARM ��������˲���
*********************************************************************************************************/

#if LW_CFG_SMP_EN > 0
VOID    bspMpInt(ULONG  ulCPUId);
VOID    bspCpuUp(ULONG  ulCPUId);                                       /*  ����һ�� CPU                */

#if LW_CFG_SMP_CPU_DOWN_EN > 0
VOID    bspCpuDown(ULONG  ulCPUId);                                     /*  ֹͣһ�� CPU                */
#endif                                                                  /*  LW_CFG_SMP_CPU_DOWN_EN > 0  */

#if LW_CFG_CPU_ARCH_SMT > 0
ULONG   bspCpuLogic2Physical(ULONG  ulCPUId);
#endif
#endif                                                                  /*  LW_CFG_SMP_EN               */

/*********************************************************************************************************
  ARM ������ CPU ���� (������Ƶ)
*********************************************************************************************************/

#if LW_CFG_POWERM_EN > 0
VOID    bspSuspend(VOID);                                               /*  ϵͳ����                    */
VOID    bspCpuPowerSet(UINT  uiPowerLevel);                             /*  ���� CPU ��Ƶ�ȼ�           */
VOID    bspCpuPowerGet(UINT *puiPowerLevel);                            /*  ��ȡ CPU ��Ƶ�ȼ�           */
#endif                                                                  /*  LW_CFG_POWERM_EN > 0        */

/*********************************************************************************************************
  Trusted computing
*********************************************************************************************************/

#if LW_CFG_TRUSTED_COMPUTING_EN > 0
VOID    bspTrustedModuleInit(VOID);
INT     bspTrustedModuleCheck(const PCHAR  pcPath);
VOID    bspTrustedModuleLoad(PVOID  pvModule);
VOID    bspTrustedModuleUnload(PVOID  pvModule);
#endif                                                                  /*  LW_CFG_TRUSTED_COMPUTING_EN */

#endif                                                                  /*  __ARCH_ARM_SUPPORT_H        */
/*********************************************************************************************************
  END
*********************************************************************************************************/