# 09. MATLAB 시리얼 통신: 4자리 7세그먼트

SparkFun 시리얼 7세그먼트 디스플레이를 Pi의 UART에 연결하고, MATLAB에서 바이트를 보내 숫자와 문자를 표시한 실습입니다. Pi는 보내기만 하므로 Pi의 TX 한 선과 전원만 연결합니다.

| 디스플레이 | Pi |
|---|---|
| RX | UART TX (GPIO14) |
| VCC | 3.3V |
| GND | GND |

[`p31_serial_seven_segment.m`](p31_serial_seven_segment.m)은 다음 순서로 진행됩니다.

1. `serialdev(mypi, '/dev/ttyAMA0', 9600)`로 시리얼 포트 열기
2. `0`, `7`, `0x0A`, `0x0B`를 차례로 보내 `07ab` 표시
3. `0x76`을 보내 화면 지우기
4. `0x79`, `2`로 커서를 세 번째 자리에 놓고 `9` 표시
5. 기존 포트를 닫고 `raspi.internal.serialsegmentdisplay` 클래스로 `112`, `dead`, `beef` 출력
6. 1부터 1000까지 0.01초 간격으로 세는 카운터

디스플레이는 0~15 값을 받으면 해당 16진수 글자를 표시하고, `0x76`, `0x79` 같은 특수 바이트는 명령으로 처리합니다.
