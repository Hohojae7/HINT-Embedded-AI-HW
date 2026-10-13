# 09. Arduino Blockset 배포

아날로그 입력값으로 서보 모터 두 개의 각도를 바꾸는 모델을 Arduino Uno에 올린 실습입니다. 모델 파일은 없고, `ert.tlc`로 생성한 C 코드와 보드에 올린 펌웨어를 보관했습니다.

## 블록과 핀

| 블록 | 핀 | 동작 |
|---|---|---|
| Analog Input | A0 | 10비트 ADC 값(0~1023) 읽기 |
| Gain | - | 180/1023을 곱해 0~180°로 변환 |
| Rate Limiter | - | 각도 변화 속도를 ±720°/s로 제한 |
| Standard Servo Write | D9 | 서보 1 |
| Standard Servo Write1 | D10 | 서보 2 (서보 1과 같은 각도) |

```math
\theta = \frac{180}{1023}\,\text{ADC}, \qquad |\theta_k - \theta_{k-1}| \le 720 \times 0.01 = 7.2^\circ
```

모델은 10 ms마다 실행되므로, 입력이 갑자기 바뀌어도 서보 각도는 한 번에 최대 7.2°씩만 움직입니다. 서보 블록은 각도를 0~180°로 제한하고 반올림해 출력합니다.

## 파일

| 파일 | 역할 |
|---|---|
| [`generated_code/untitled.c`](generated_code/untitled.c) | `untitled_step()`: ADC 읽기 → Gain → Rate Limiter → 서보 2개 출력 |
| [`generated_code/ert_main.c`](generated_code/ert_main.c) | 타이머 인터럽트로 10 ms마다 `rt_OneStep()` → `untitled_step()` 호출. 이전 스텝이 안 끝났으면 overrun 표시 |
| [`generated_code/untitled_data.c`](generated_code/untitled_data.c) | 파라미터 (Rate Limiter ±720, Gain) |
| [`generated_code/MW_target_hardware_resources.h`](generated_code/MW_target_hardware_resources.h) | 보드(Arduino Uno)와 통신, 스케줄러 설정 |
| `generated_code/` 나머지 `.h` | 모델 구조체와 타입 정의 |
| [`firmware/untitled.hex`](firmware/untitled.hex) | 보드에 업로드한 펌웨어 |
| `firmware/untitled.elf`, `untitled.eep` | 심볼 정보가 있는 실행 파일, EEPROM 초기값 |

- ADC 값이 정수(`uint16`)라서 Gain도 정수 46125로 저장되고, 코드에서 $2^{-18}$을 곱해 $46125 \times 2^{-18} \approx 0.17595 \approx 180/1023$을 만듭니다.
- `Standard Servo Write` 두 블록은 `MW_servoAttach(0, 9)`, `MW_servoAttach(1, 10)`으로 D9, D10에 연결됩니다.
