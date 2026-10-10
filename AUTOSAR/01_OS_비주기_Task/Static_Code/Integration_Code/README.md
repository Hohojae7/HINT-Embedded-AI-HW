# 시작 경로와 실습 Task 연결

플랫폼의 `Static_Code/Integration_Code/integration_EcuM/fixedcode/EcuM_Boot.c`에서 `OsTask_BSW_Init`은 `EcuM_StartupTwo()` 이후 LED 초기화 Task를 활성화합니다.

```c
ActivateTask(OsTask_LED_Init);
```

이 호출로 [App_Os.c](../Reference_Code/App_Os.c)의 `OsTask_LED_Init`이 실행되어 LED2(PE5)를 초기화합니다.

```text
OsTask_BSW_Init → EcuM_StartupTwo() → OsTask_LED_Init → LED2 초기화
```

플랫폼 시작 소스 전체는 배포 제한에 따라 포함하지 않았습니다.
