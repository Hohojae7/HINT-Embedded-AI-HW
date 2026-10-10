# 시작 경로와 실습 Task 연결

원본 위치: `Static_Code/Integration_Code/integration_EcuM/fixedcode/EcuM_Boot.c`

OS 1 백업의 단일 코어 경로에서는 `OsTask_BSW_Init` 안에서 `EcuM_StartupTwo()` 호출 다음에 아래 호출이 들어 있습니다.

```c
ActivateTask(OsTask_LED_Init);
```

이 호출은 [App_Os.c](../Reference_Code/App_Os.c)의 LED 초기화 Task를 활성화합니다. Task 본문만 보존하면 이 시작 연결을 놓칠 수 있어 해당 위치와 역할을 함께 기록했습니다.

원본 `EcuM_Boot.c`에는 공급사의 기밀·배포 제한 및 사전 허가 없는 복사 제한이 명시되어 있습니다. 이 폴더에는 그 파일 전체를 복제하지 않고 실습 Task 연결 호출을 설명합니다. 수정 전 파일이 없으므로 적용 가능한 패치 파일로 표현하지 않았습니다.

