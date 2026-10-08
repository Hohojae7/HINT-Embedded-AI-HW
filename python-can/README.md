# python-can — CAN 통신 실습

HINT 교육과정 자동차 통신 시스템 강의의 python-can 실습 코드입니다. 실제 차량이나 CAN 장비 없이 Kvaser 가상 CAN 채널 위에서 파이썬 스크립트가 가상 ECU 역할을 하며 메시지를 주고받습니다.
메시지 하나를 보내는 것에서 시작해 DBC 기반 자동 송신, 주기 송신, 필터 수신, 송수신 통합, 리모트 프레임까지 실제 ECU 동작에 가까워지도록 단계적으로 진행했습니다.

## 개발 환경

| 항목 | 내용 |
|---|---|
| 라이브러리 | python-can, cantools |
| CAN 인터페이스 | Kvaser 가상 CAN 채널 (channel 0과 1이 내부에서 연결됨) |
| 모니터링 툴 | Kvaser CanKing |
| CAN 데이터베이스 | [`project.dbc`](project.dbc) (ECU1~4, TestClient의 메시지 30개) |
| 통신 | Classic CAN, 1 Mbps |

- **python-can**: Kvaser, PEAK, Vector처럼 장비마다 다른 드라이버를 공통 인터페이스로 감싸 주는 라이브러리
- **cantools**: DBC 파일을 읽고, 신호 값을 DBC 정의대로 바이트열로 인코딩해 주는 라이브러리

## CAN 기초: 메시지 하나 보내기

> [!NOTE]
> **`Bus` 클래스의 `send()` 예제**
>
> ```python
> import can
>
> can_bus = can.Bus(interface='kvaser', channel=0, bitrate=1000000)
> can_msg = can.Message(arbitration_id=0x123, data=[0, 25, 0, 1, 3, 1, 4, 1], is_extended_id=False)
> can_bus.send(can_msg, 0.1)
> ```
>
> Kvaser 0번 채널에 1 Mbps로 접속해서, ID가 0x123인 8바이트 메시지를 0.1초 안에 보내는 코드입니다.
>
> - `can.Bus(...)` : CAN 버스에 접속하는 객체
> - `interface='kvaser'` : 사용할 CAN 장비의 드라이버
> - `channel=0` : 장비 안의 채널(포트) 번호
> - `bitrate=1000000` : 통신 속도 1 Mbps(초당 100만 비트)로, Classic CAN의 최고 속도. 같은 버스에 있는 노드는 모두 같은 값을 써야 통신됨
> - `can.Message(...)` : 버스로 보낼 CAN 메시지(프레임) 하나
> - `arbitration_id=0x123` : 메시지 ID. `0x`는 16진수 표기(10진수 291)
> - `data=[0, 25, 0, 1, 3, 1, 4, 1]` : 실제로 보낼 데이터
>   - 원소가 8개라서 데이터 길이(DLC)가 자동으로 8이 됨 (Classic CAN의 최대 길이)
>   - 출력 화면에는 16진수 `00 19 00 01 03 01 04 01`로 보임 (25 = 0x19)
> - `is_extended_id=False` : 11비트 표준 ID(0x000~0x7FF)를 쓴다는 뜻. `True`면 29비트 확장 ID
> - `can_bus.send(can_msg, 0.1)` : 메시지 송신

## 실습 목록

파일 번호 순서가 강의 실습 순서입니다. 송신 스크립트는 channel 1, 수신 스크립트는 channel 0을 사용합니다.

### 1. 기본 송수신

| No | 파일 | 내용 | 핵심 개념 |
|---|---|---|---|
| 00 | [`00_can_list_channel.py`](00_can_list_channel.py) | PC에서 사용할 수 있는 Kvaser CAN 채널 조회 | `detect_available_configs()` |
| 01 | [`01_can_send.py`](01_can_send.py)<br>[`01_can_receive.py`](01_can_receive.py) | ID 0x123 메시지를 1초마다 송신하고, 다른 채널에서 수신해 출력 | `Bus`, `Message`, `send()`, `recv()` |
| 02 | [`02_can_send_multi.py`](02_can_send_multi.py) | ID 3개(0x101~0x103)를 0.1초 간격으로 번갈아 송신 | 메시지 리스트, 표준/확장 ID, DLC |

### 2. DBC 기반 송신과 주기 송신

| No | 파일 | 내용 | 핵심 개념 |
|---|---|---|---|
| 03 | [`03_can_send_multi_dbc.py`](03_can_send_multi_dbc.py) | `project.dbc`에서 ECU1이 보내는 메시지 17개를 읽어, 신호마다 랜덤 값을 채워 송신 | cantools, `encode()` |
| 04 | [`04_can_send_multi_dbc_siginfo.py`](04_can_send_multi_dbc_siginfo.py) | 03과 같이 송신하면서 신호 속성(시작 비트, 길이, 바이트 순서, 범위)을 출력 | DBC 신호 구조, Intel/Motorola 바이트 순서 |
| 05 | [`05_can_send_multi_dbc_cycle.py`](05_can_send_multi_dbc_cycle.py) | DBC의 송신 방식과 주기를 읽어 송신 간격에 반영. 한 루프 안에서 차례로 기다리기 때문에 주기가 누적되는 한계를 확인 | `send_type`, `cycle_time` |
| 06 | [`06_can_send_multi_dbc_cycle_threads.py`](06_can_send_multi_dbc_cycle_threads.py) | 메시지마다 스레드를 띄워, 각 메시지가 자기 주기대로 독립적으로 송신 | `threading`, `ThreadSafeBus` |
| 07 | [`07_can_send_multi_dbc_cycle_periodic.py`](07_can_send_multi_dbc_cycle_periodic.py) | 스레드를 직접 만들지 않고 라이브러리의 주기 송신 기능으로 06과 같은 동작 구현. 등록할 때 만든 데이터가 그대로 반복됨 | `send_periodic()` |

### 3. 수신 필터와 송수신 통합

| No | 파일 | 내용 | 핵심 개념 |
|---|---|---|---|
| 08 | [`08_can_receive_filters.py`](08_can_receive_filters.py) | ID와 마스크로 필요한 메시지만 골라 수신. ECU1 메시지 17개 중 11개만 통과 | `can_filters`, ID 마스크 |
| 09 | [`09_can_send_receive.py`](09_can_send_receive.py) | 한 스크립트에서 수신 스레드와 송신 스레드를 함께 실행. 두 창에서 채널 0, 1로 띄우면 ECU 두 개처럼 서로 주고받음 | `ThreadSafeBus`, `sys.argv`, `BitTiming` |
| 10 | [`10_can_request_remote.py`](10_can_request_remote.py)<br>[`10_can_response_remote.py`](10_can_response_remote.py) | 리모트 프레임으로 ID 0x123의 데이터를 요청하고, 같은 ID의 데이터 프레임으로 응답 | `is_remote_frame` |

## 실행 방법

1. Kvaser 드라이버를 설치하고 가상 채널 0, 1이 보이는지 `00_can_list_channel.py`로 확인합니다.
2. 라이브러리를 설치합니다: `pip install python-can cantools`
3. 명령 프롬프트 창을 두 개 띄우고, **창 1을 먼저 실행한 뒤** 창 2를 실행합니다. 종료는 `Ctrl + C`입니다.

| 실습 | 창 1 (먼저 실행) | 창 2 |
|---|---|---|
| 00 | `python 00_can_list_channel.py` | - |
| 01 | `python 01_can_receive.py` | `python 01_can_send.py` |
| 02 ~ 07 | `python 01_can_receive.py` | 해당 번호의 송신 파일 |
| 08 | `python 08_can_receive_filters.py` | `python 06_can_send_multi_dbc_cycle_threads.py` |
| 09 | `python 09_can_send_receive.py 0` | `python 09_can_send_receive.py 1` |
| 10 | `python 10_can_response_remote.py` | `python 10_can_request_remote.py` |

## 강의 예제에서 다듬은 점

- **종료 처리**: 버스를 여는 모든 스크립트가 `Ctrl + C`를 처리하고 `shutdown()`으로 버스를 닫습니다. 06, 07, 09는 송신 스레드와 주기 송신 작업까지 멈춘 뒤 종료합니다.
- **수신 대기**: 수신에 timeout을 둬서, 메시지가 없을 때도 멈춰 있지 않고 대기 중임을 알려 줍니다. 10번 요청 쪽은 2초 안에 응답이 없으면 다시 요청합니다.
- **DBC 경로**: `project.dbc`를 스크립트 위치 기준으로 찾아서, 어느 폴더에서 실행해도 동작합니다.
- **스레드 안전**: 여러 스레드가 버스 하나를 같이 쓰는 06, 09는 `ThreadSafeBus`를 사용합니다.
- **비트 타이밍**: 09는 강의 코드에서 만들기만 하고 적용하지 않던 `can.BitTiming`을 `timing=` 인자로 실제 적용했습니다. 16 MHz 클럭에서 1 Mbps, 샘플 포인트 75%가 되는 값입니다.
