# AURIX TC275 — MCU 프로그래밍 실습

HINT 교육과정의 MCU 프로그래밍 실습 코드입니다. Infineon AURIX TC275의 CPU0에서 iLLD 레지스터 헤더(`Ifx*_reg.h`)로 주변장치를 직접 설정하는 bare-metal 방식으로 작성했습니다.
디지털 입출력에서 시작해 인터럽트·타이머, 아날로그 측정과 신호 처리까지 16개 실습을 순서대로 진행했습니다.

## 개발 환경

| 항목 | 내용 |
|---|---|
| 보드 | ShieldBuddy TC275 + Easy Module Shield V1 |
| MCU | Infineon AURIX TC275 (TC27D), CPU0만 사용 |
| IDE / 컴파일러 | AURIX Development Studio (ADS), TASKING |
| 라이브러리 | iLLD 1.20.0 |

### 핀 구성

| 부품 | 쉴드 | TC275 핀 | 설정 |
|---|---|---|---|
| LED1 (빨강) | D12 | P10.1 | push-pull 출력, High = ON |
| LED2 (파랑) | D13 | P10.2 | push-pull 출력, High = ON |
| SW1 | D2 | P02.0 | pull-up 입력, 누름 = Low |
| SW2 | D3 | P02.1 | pull-up 입력, 누름 = Low |
| RGB R / G / B | D9 / D10 / D11 | P02.7 / P10.5 / P10.3 | push-pull 출력, High = ON |
| DHT11 DATA | D4 | P10.4 | open-drain, 외부 4.7 kΩ pull-up |
| 가변저항 | A0 | AN39 (VADC G4 CH7) | 0 ~ 5 V |
| LDR 분압 | A1 | AN38 (VADC G4 CH6) | 밝을수록 높음 |
| LM35 | A2 | AN37 (VADC G4 CH5) | 10 mV/℃ |

## 실습 목록

### 1. 디지털 입출력 (GPIO)

| No | 파일 | 내용 | 핵심 개념 |
|---|---|---|---|
| 01 | [`Lab01_led_on.c`](Lab01_led_on.c) | LED1 켜기 | `IOCR` 출력 모드, `OMR` set |
| 02 | [`Lab02_led_blink.c`](Lab02_led_blink.c) | LED1을 약 500 ms 간격으로 점멸 | `OMR` set/reset, busy-wait 지연 |
| 03 | [`Lab03_button_led.c`](Lab03_button_led.c) | SW1을 누르는 동안 LED1 켜기 | pull-up 입력, `IN` 레지스터 polling |
| 04 | [`Lab04_button_toggle.c`](Lab04_button_toggle.c) | SW2를 한 번 누를 때마다 LED2 토글 | 소프트웨어 디바운스(약 20 ms), 상승 에지 검출 |
| 05 | [`Lab05_rgb_led.c`](Lab05_rgb_led.c) | RGB LED로 8가지 색을 약 1초마다 순환 | 3비트 색 코드, 여러 포트 동시 제어 |

### 2. 인터럽트와 타이머 (STM, ERU)

| No | 파일 | 내용 | 핵심 개념 |
|---|---|---|---|
| 06 | [`Lab06_stm_delay_measure.c`](Lab06_stm_delay_measure.c) | 지연 루프의 실행 시간을 STM으로 측정 | STM `TIM0`, tick → µs 환산 (100 MHz) |
| 07 | [`Lab07_stm_isr_blink.c`](Lab07_stm_isr_blink.c) | STM 비교 인터럽트로 500 ms마다 LED1 토글 | `CMP0` compare match, `SRC` 설정, ISR |
| 08 | [`Lab08_eru_button_toggle.c`](Lab08_eru_button_toggle.c) | SW1 하강 에지 인터럽트로 LED2 토글 | ERU(`EICR`/`IGCR`), ISR 내 시간 기반 디바운스, ISR↔main 임계 구역 |
| 09 | [`Lab09_1ms_tick_tasks.c`](Lab09_1ms_tick_tasks.c) | 1 ms tick으로 LED1 100 ms, LED2 1 s, RGB 250 ms 주기 동시 실행 | 주기 tick, `every_ms()` 기반 비선점 다중 주기 태스크 |
| 10 | [`Lab10_dht11.c`](Lab10_dht11.c) | DHT11 온습도 읽기와 decoder 자체 시험 | open-drain 통신, pulse 폭 측정, checksum, 상태 머신 |

### 3. 아날로그 데이터 처리 (VADC, DSADC)

| No | 파일 | 내용 | 핵심 개념 |
|---|---|---|---|
| 11 | [`Lab11_vadc_pot.c`](Lab11_vadc_pot.c) | 가변저항 값을 한 번씩 읽어 중간값 이상이면 LED1 ON | VADC G4 queue 단발 변환, `VF` polling |
| 12 | [`Lab12_vadc_tick_stream.c`](Lab12_vadc_tick_stream.c) | 1 ms마다 자동 변환한 가변저항 값으로 LED1 밝기 제어 | tick trigger 변환, ADC 결과 인터럽트, ring buffer, 이동평균, 소프트웨어 PWM |
| 13 | [`Lab13_lm35_iir.c`](Lab13_lm35_iir.c) | LM35 온도를 필터링해 십의 자리만큼 LED1 점멸 | IIR 저역통과(α = 1/64), 고정소수점 보정, 비차단 점멸 상태 머신 |
| 14 | [`Lab14_ldr_hysteresis.c`](Lab14_ldr_hysteresis.c) | 주변이 어두우면 LED2 ON | 3점 중앙값 + IIR 필터, 히스테리시스 |
| 15 | [`Lab15_sensor_scan.c`](Lab15_sensor_scan.c) | 세 센서를 10 ms마다 한 묶음으로 측정해 게시 | 다채널 scan, timeout, 유효성 mask, sequence·timestamp, stale 감시 |
| 16 | [`Lab16_dsadc_vadc.c`](Lab16_dsadc_vadc.c) | 가변저항·LDR은 DSADC, LM35는 VADC로 측정해 LED와 RGB 제어 | DSADC 차동 입력·mux 전환·settling, 센서별 변환기 선택 |

## 공통 모듈

실습을 진행하며 반복되는 코드를 모듈로 분리했습니다.

| 파일 | 역할 | 사용 실습 |
|---|---|---|
| [`my_gpio.h`](my_gpio.h) | 보드 핀 정의와 GPIO 함수 5개 (`init_out`, `init_in_pullup`, `write`, `read`, `toggle`) | 06 ~ 10 |
| [`my_tick.c`](my_tick.c) / [`my_tick.h`](my_tick.h) | STM0 `CMP0` 기반 1 ms tick, `elapsed()` / `every_ms()` | 09, 10, 13 ~ 16 |
| [`my_tick_lab12.c`](my_tick_lab12.c) | `my_tick.c`에 tick마다 VADC 변환을 시작하는 코드를 더한 Lab12 전용 버전 | 12 |
| [`my_dht11.c`](my_dht11.c) / [`my_dht11.h`](my_dht11.h) | DHT11 start 신호, 40비트 pulse capture, decode, 측정 상태 머신 | 10 |
| [`my_adc.c`](my_adc.c) / [`my_adc.h`](my_adc.h) | VADC 초기화, 단발 변환, tick stream 설정, 이동평균·3점 중앙값 필터 | 11 ~ 16 |
| [`my_scan.c`](my_scan.c) / [`my_scan.h`](my_scan.h) | 3채널 scan 요청, 결과 수집, 측정 record 게시 | 15 |
| [`my_dsadc.c`](my_dsadc.c) / [`my_dsadc.h`](my_dsadc.h) | DSADC 초기화, 입력 mux 전환, 결과를 VADC scale(0 ~ 4095)로 환산 | 16 |

## ADS에서 다시 빌드하기

1. ADS에서 TC275(TC27D) 기본 프로젝트를 만듭니다.
2. 생성된 `Cpu0_Main.c`의 내용을 실습 파일 내용으로 바꿉니다.
3. 아래 표의 모듈 파일을 프로젝트에 추가합니다.

| 실습 | 함께 넣을 파일 |
|---|---|
| 01 ~ 05 | 없음 |
| 06 ~ 08 | `my_gpio.h` |
| 09 | `my_gpio.h`, `my_tick.c/h` |
| 10 | `my_gpio.h`, `my_tick.c/h`, `my_dht11.c/h` |
| 11 | `my_adc.c/h` |
| 12 | `my_adc.c/h`, `my_tick.h`, `my_tick_lab12.c` |
| 13, 14 | `my_adc.c/h`, `my_tick.c/h` |
| 15 | `my_adc.c/h`, `my_tick.c/h`, `my_scan.c/h` |
| 16 | `my_adc.c/h`, `my_tick.c/h`, `my_dsadc.c/h` |
