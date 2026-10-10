# AUTOSAR MEMORY — NvM 블록·서비스 연동

Application SWC에서 NvM 블록의 읽기·쓰기를 요청하고 완료 통지를 받는 실습입니다. RAM 버퍼, NvM 블록 설정, 서비스 포트와 Runnable을 연결했습니다.

## NvM 블록 설정

| 항목 | 설정 |
|---|---|
| 블록 | `NvMBlock_N_Block1` |
| Block ID | `10` |
| 관리 방식 | `NVM_BLOCK_NATIVE` |
| 길이 | 10 bytes |
| RAM 주소 | `RamBlock_N_Block1` |
| CRC 사용 | 비활성 |
| 주기 처리 | `NvM_BlockOpTask()`, 100 ms |
| 완료 통지 | `NvMJobFinished_N_Block1()` |

## RAM 블록과 동작 흐름

RAM 정의와 외부 선언을 추가하고, NvM 설정에서 같은 이름을 참조하도록 구성합니다.

```c
/* App_NvM_Ram.c */
uint8 RamBlock_N_Block1[10];

/* App_NvM_Ram.h */
extern uint8 RamBlock_N_Block1[10];
```

1. 100 ms Runnable에서 `GetErrorStatus`로 Pending 상태를 확인합니다.
2. 쓰기 요청이 있으면 RAM 값을 갱신하고 `WriteBlock`을 호출합니다. 읽기 요청에는 `ReadBlock`을 사용합니다.
3. API의 성공 반환은 요청 승인으로 처리하고, 실제 작업 결과는 완료 Callback의 `JobResult`로 받습니다.

읽기 요청 실패에는 재시도 카운터를 사용합니다. NvM 서비스 모델과 Application SWC 사이의 포트 연결로 요청·통지 경로를 구성했습니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [Ecud_NvM.arxml](Configuration/ECU/Ecud_NvM.arxml) | 블록 ID·크기·RAM 주소·Callback 설정 |
| [App_NvM.arxml](Configuration/System/Swcd_App/App_NvM.arxml) | 서비스 포트·주기 Event·완료 통지 Event |
| [Swcd_Bsw_NvM.arxml](Generated/Bsw_Output/swcd/Swcd_Bsw_NvM.arxml) | NvM 서비스 SWC 모델 |
| [구현 설명](Static_Code/Reference_Code/README.md) | RAM 선언과 제공 Runnable의 처리 흐름 |
| [generate.py](Build/generate.py) | `App_NvM` 생성 입력 등록 |

제공 C·헤더 전체는 배포 제한 표기 때문에 제외하고 설정과 구현 설명을 보존했습니다. 강의의 여러 블록 설정 예제 중, 이 저장본은 Native 블록 서비스 연동을 중심으로 정리했습니다.
