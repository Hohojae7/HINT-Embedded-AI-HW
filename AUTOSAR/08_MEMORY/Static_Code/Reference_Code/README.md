# NvM 실습의 RAM 선언과 서비스 흐름

강의 PDF 966–970쪽과 저장된 백업의 구현을 대조한 설명입니다. 교육용 공급사 코드 전체를 재배포하는 소스 파일은 포함하지 않았습니다.

## 강의에서 추가하는 RAM 블록

원본 `Static_Code/Reference_Code/App_NvM_Ram.c`의 선언:

```c
uint8 RamBlock_N_Block1[10];
```

원본 `Static_Code/Reference_Code/App_NvM_Ram.h`의 외부 선언:

```c
extern uint8 RamBlock_N_Block1[10];
```

[Ecud_NvM.arxml](../../Configuration/ECU/Ecud_NvM.arxml)의 `NvMBlock_N_Block1`은 이 이름을 RAM 주소로 참조합니다. 블록 ID는 `10`, 길이는 `10`, 관리 방식은 `NVM_BLOCK_NATIVE`입니다.

## Application 동작 연결

원본 구현 위치: `Static_Code/Reference_Code/App_NvM.c`

| 처리 | 저장된 예제의 동작 |
|---|---|
| `NvM_BlockOpTask` | 100ms 주기 Runnable에서 `GetErrorStatus`로 블록 상태를 확인 |
| Write 요청 | Pending이 아닐 때 요청 플래그를 확인하고 RAM 값을 갱신한 뒤 `WriteBlock` 요청 |
| Write 요청 승인 | API가 성공 반환하면 RAM 갱신·Write 요청 플래그를 초기화; 실제 NV 저장 완료와 구분 |
| Read 요청 | Write 요청이 없을 때 Read 플래그를 확인해 `ReadBlock` 요청; 실패 시 재시도 카운터 처리 |
| `NvMJobFinished_N_Block1` | 완료 통지의 `JobResult`를 저장 |

Application의 Event·포트·Runnable 정의는 [App_NvM.arxml](../../Configuration/System/Swcd_App/App_NvM.arxml), NvM 서비스 포트는 [Swcd_Bsw_NvM.arxml](../../Generated/Bsw_Output/swcd/Swcd_Bsw_NvM.arxml), 생성 입력 등록은 [generate.py](../../Build/generate.py)에 있습니다.

세 원본 C·헤더에는 공급사의 기밀·배포 제한 및 사전 허가 없는 복사 금지 문구가 있습니다. 이를 삭제해서 재게시하지 않고 전체 파일을 제외했습니다. 위의 간단한 RAM 선언과 동작 설명은 실습 변경과 구성의 관계를 보여주기 위한 기록입니다.
