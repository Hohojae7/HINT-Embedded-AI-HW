# 08. MATLAB GPIO: LED와 버튼

브레드보드에 외부 LED와 푸시 버튼을 연결하고, MATLAB에서 GPIO로 쓰고 읽은 실습입니다. 코드는 [`Lab2_gpio_led_button.m`](Lab2_gpio_led_button.m) 하나입니다.

| 부품 | GPIO | 설정 |
|---|---|---|
| LED | 24 | `DigitalOutput`, 저항을 거쳐 GND로 연결 |
| 버튼 | 23 | `DigitalInput`, 누르면 1(High)이 읽히도록 풀다운으로 연결 |

스크립트는 다음 순서로 진행됩니다.

1. `showPins()`로 Pi 40핀 배치도 확인
2. LED 켜기, 0.5초 간격으로 10번 깜빡이기
3. 버튼 상태를 한 번 읽어 출력
4. 0.1초마다 버튼을 100번 확인하면서, 눌려 있으면 LED를 0.05초 간격으로 빠르게 10번 깜빡이기
5. 마지막에 LED 끄기

- `configurePin()`으로 핀 방향을 먼저 정한 뒤 `writeDigitalPin()`, `readDigitalPin()`을 사용합니다.
- Pi 종료 명령(`sudo shutdown`)은 실수로 실행되지 않도록 주석으로만 남겨 두었습니다.
