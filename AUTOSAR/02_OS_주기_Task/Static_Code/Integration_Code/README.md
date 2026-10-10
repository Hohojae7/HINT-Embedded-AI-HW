# 시작 경로와 LED 초기화 Task 연결

원본 위치: `Static_Code/Integration_Code/integration_EcuM/fixedcode/EcuM_Boot.c`

이 백업의 단일 코어 시작 경로는 `OsTask_BSW_Init`에서 `EcuM_StartupTwo()` 이후 다음 호출로 LED 초기화 Task를 활성화합니다.

```c
ActivateTask(OsTask_LED_Init);
```

Task 본문은 [App_Os.c](../Reference_Code/App_Os.c), 구성은 [Ecud_Os.arxml](../../Configuration/ECU/Ecud_Os.arxml)에 있습니다. 이 호출은 RTE Runnable을 대신 실행하는 호출이 아니라 OS LED 초기화 Task의 시작 연결입니다.

원본 파일의 기밀·배포 제한 및 사전 허가 없는 복사 금지 문구 때문에 파일 전체는 복제하지 않았습니다. 수정 전 원본이 없어 적용 가능한 패치로 만들지 않고 위치와 연결을 설명했습니다.
