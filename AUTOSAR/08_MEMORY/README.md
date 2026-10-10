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
| 저장 경로 | `NvM → MemIf → Fee → Fls → Data Flash` (`FeeBlock_N_Block1`) |

## RAM 블록과 동작 흐름

RAM 정의와 외부 선언을 추가하고, NvM 설정에서 같은 이름을 참조하도록 구성합니다.

```c
/* App_NvM_Ram.c */
uint8 RamBlock_N_Block1[10];

/* App_NvM_Ram.h */
extern uint8 RamBlock_N_Block1[10];
```

1. 100 ms Runnable에서 `GetErrorStatus`로 블록이 Pending인지 확인합니다.
2. `AppNvM_Gub_N_Block1WrFlag`가 1이면 RAM 값을 갱신하고 `WriteBlock`을, Write 플래그가 0이고 `AppNvM_Gub_N_Block1RdFlag`가 1이면 `ReadBlock`을 요청합니다. 읽기 요청이 실패하면 재시도 카운터를 사용합니다.
3. 요청이 승인되면 해당 플래그를 0으로 되돌리고, 실제 작업 결과는 완료 Callback의 `JobResult`로 받습니다.

플래그 초기값이 Write=1, Read=0이라 시작하자마자 한 번 쓰기를 요청합니다. Write 때는 RAM 첫 바이트를 1 증가시키고 나머지 9바이트를 앞 바이트 값+1로 채우므로, 초기 RAM이 0이면 첫 쓰기 데이터는 1~10입니다.

## 주요 파일

| 파일 | 역할 |
|---|---|
| [Ecud_NvM.arxml](Configuration/ECU/Ecud_NvM.arxml) | 블록 ID·크기·RAM 주소·Callback 설정 |
| [App_NvM.arxml](Configuration/System/Swcd_App/App_NvM.arxml) | 서비스 포트·주기 Event·완료 통지 Event |
| [Swcd_Bsw_NvM.arxml](Generated/Bsw_Output/swcd/Swcd_Bsw_NvM.arxml) | NvM 서비스 SWC 모델 |
| [구현 설명](Static_Code/Reference_Code/README.md) | RAM 선언과 제공 Runnable의 처리 흐름 |
| [generate.py](Build/generate.py) | `App_NvM` 생성 입력 등록 |

이 실습은 Native 블록의 설정과 NvM 서비스 연동을 다룹니다. 제공 C·헤더 전체는 배포 제한에 따라 포함하지 않았으며, RAM 선언과 Runnable의 처리 흐름은 [구현 설명](Static_Code/Reference_Code/README.md)에서 확인할 수 있습니다.
