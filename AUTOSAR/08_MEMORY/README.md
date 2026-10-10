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

전역 요청 플래그 `AppNvM_Gub_N_Block1WrFlag`가 1이면 Write, 이 값이 0이고 `AppNvM_Gub_N_Block1RdFlag`가 1이면 Read를 요청합니다.
초기값은 Write=1·Read=0으로 시작 후 쓰기를 요청하며, 각 요청이 승인되면 해당 플래그를 0으로 초기화합니다.

Write 시 RAM 첫 바이트를 1 증가시키고, 나머지 9바이트를 앞 바이트의 값+1로 채웁니다. 초기 RAM이 0이면 첫 쓰기 데이터는 1~10입니다.
저장 경로는 `NvM → MemIf → Fee → Fls → Data Flash`이며, NvM 블록은 `FeeBlock_N_Block1`에 연결되어 있습니다.

읽기 요청 실패에는 재시도 카운터를 사용합니다. NvM 서비스 모델과 Application SWC 사이의 포트 연결로 요청·통지 경로를 구성했습니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [Ecud_NvM.arxml](Configuration/ECU/Ecud_NvM.arxml) | 블록 ID·크기·RAM 주소·Callback 설정 |
| [App_NvM.arxml](Configuration/System/Swcd_App/App_NvM.arxml) | 서비스 포트·주기 Event·완료 통지 Event |
| [Swcd_Bsw_NvM.arxml](Generated/Bsw_Output/swcd/Swcd_Bsw_NvM.arxml) | NvM 서비스 SWC 모델 |
| [구현 설명](Static_Code/Reference_Code/README.md) | RAM 선언과 제공 Runnable의 처리 흐름 |
| [generate.py](Build/generate.py) | `App_NvM` 생성 입력 등록 |

이 실습은 Native 블록의 설정과 NvM 서비스 연동을 다룹니다. 제공 C·헤더 전체는 배포 제한에 따라 포함하지 않았으며, RAM 선언과 Runnable의 처리 흐름은 [구현 설명](Static_Code/Reference_Code/README.md)에서 확인할 수 있습니다.
