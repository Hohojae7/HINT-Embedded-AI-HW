# NvM 실습의 RAM 선언과 서비스 흐름

NvM 블록의 RAM 버퍼 정의와 Application Runnable의 서비스 호출 흐름입니다.

## RAM 블록

RAM 버퍼 정의 (`App_NvM_Ram.c`):

```c
uint8 RamBlock_N_Block1[10];
```

외부 선언 (`App_NvM_Ram.h`):

```c
extern uint8 RamBlock_N_Block1[10];
```

[Ecud_NvM.arxml](../../Configuration/ECU/Ecud_NvM.arxml)의 `NvMBlock_N_Block1`은 이 이름을 RAM 주소로 참조합니다. 블록 ID는 `10`, 길이는 `10`, 관리 방식은 `NVM_BLOCK_NATIVE`입니다.

## Application 동작 연결

`App_NvM.c`의 주기 Runnable과 완료 통지 Callback을 연결합니다.

| 처리 | 동작 |
|---|---|
| `NvM_BlockOpTask` | 100 ms 주기 Runnable에서 `GetErrorStatus`로 블록 상태를 확인 |
| Write 요청 | Pending이 아닐 때 요청 플래그를 확인하고 RAM 값을 갱신한 뒤 `WriteBlock` 요청 |
| Write 요청 승인 | API가 성공 반환하면 RAM 갱신·Write 요청 플래그를 초기화; 실제 NV 저장 완료와 구분 |
| Read 요청 | Write 요청이 없을 때 Read 플래그를 확인해 `ReadBlock` 요청; 실패 시 재시도 카운터 처리 |
| `NvMJobFinished_N_Block1` | 완료 통지의 `JobResult`를 저장 |

Application의 Event·포트·Runnable 정의는 [App_NvM.arxml](../../Configuration/System/Swcd_App/App_NvM.arxml), NvM 서비스 포트는 [Swcd_Bsw_NvM.arxml](../../Generated/Bsw_Output/swcd/Swcd_Bsw_NvM.arxml), 생성 입력 등록은 [generate.py](../../Build/generate.py)에 있습니다.

제공 C·헤더 전체는 배포 제한에 따라 포함하지 않았습니다.
