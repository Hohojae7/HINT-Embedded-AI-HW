# 10. Simulink 모델을 Pi에 배포하기

Simulink로 LED 점멸 모델을 만들고, 코드 생성으로 Pi에 올려 단독 실행한 실습입니다. 07~09처럼 MATLAB이 명령을 보내는 방식이 아니라, 모델에서 생성한 C 코드가 Pi에서 빌드되어 MATLAB 없이도 돌아갑니다.

| 파일 | 용도 |
|---|---|
| [`raspberrypi_gettingstarted.slx`](raspberrypi_gettingstarted.slx) | Blockset 기본 예제 모델. 내장 LED를 0.5초마다 켜고 끔 |
| [`p53_led_from_scratch.slx`](p53_led_from_scratch.slx) | 빈 모델부터 직접 만든 같은 구조의 모델. 1초마다 켜고 끔 |
| [`p43_model_control.m`](p43_model_control.m) | 배포된 모델을 MATLAB에서 확인, 정지, 재실행하고 로그를 가져오는 스크립트 |
| [`logs/p53_led_from_scratch.log`](logs/p53_led_from_scratch.log) | Pi에서 실행된 p53 모델의 실행 로그 |

```text
Pulse Generator → Data Type Conversion → LED (led0)
```

- Pulse Generator가 0과 1을 번갈아 내고, LED 블록이 그 값으로 내장 LED를 켜고 끕니다. 기본 예제는 0.1초 샘플에 주기 10·폭 5, 직접 만든 모델은 1초 샘플에 주기 2·폭 1입니다.
- Hardware 탭에서 Raspberry Pi를 대상 보드로 정하고 Build, Deploy & Start로 배포합니다.
- `p43_model_control.m`은 `raspberrypi` 객체의 `isModelRunning()`, `stopModel()`, `runModel()`을 사용하고, Pi에 남은 `<모델 이름>.log`를 `getFile()`로 가져옵니다.
