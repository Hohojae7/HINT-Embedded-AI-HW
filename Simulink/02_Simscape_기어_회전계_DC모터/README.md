# 02. Simscape 기어 회전계와 DC 모터

Simscape의 물리 블록(관성, 댐퍼, 기어, 모터)을 선으로 이어 회전 시스템을 모델링한 실습입니다. 블록선도처럼 수식을 직접 세우지 않고 부품을 연결하면 Simscape가 방정식을 만들어 풉니다.

| 파일 | 내용 |
|---|---|
| [`gear_train.slx`](gear_train.slx) | 토크원 → 관성·댐퍼 → 기어(40/100) → 관성·댐퍼. 기어 앞뒤 축의 각속도 비교 |
| [`dc_motor.slx`](dc_motor.slx) | DC 모터에 1 V를 걸고 전류, 전압, 각속도, 회전각, RPM 관측 |

## 기어 회전계 (gear_train)

각 축은 관성과 점성 감쇠를 가진 회전체입니다.

```math
T = J\dot{\omega} + B\omega
```

기어비 $N = \omega_{in}/\omega_{out}$로 출력 축을 입력 축 기준으로 환산하면 하나의 회전체처럼 볼 수 있습니다.

```math
J_{eq} = J_1 + \frac{J_2}{N^2}, \qquad B_{eq} = B_1 + \frac{B_2}{N^2}
```

| $J_1$ | $B_1$ | $J_2$ | $B_2$ | $N$ | 입력 토크 |
|---|---|---|---|---|---|
| 4.27×10⁻⁷ kg·m² | 1×10⁻⁴ N·m·s/rad | 1.0×10⁻⁶ kg·m² | 1×10⁻⁴ N·m·s/rad | 40/100 | Constant 10 |

## DC 모터 (dc_motor)

전기 쪽(전압 방정식)과 기계 쪽(회전 방정식)이 역기전력 상수 $K$로 이어집니다.

```math
V = Ri + L\frac{di}{dt} + K\omega, \qquad J\frac{d\omega}{dt} + B\omega = Ki
```

```math
\frac{\omega(s)}{V(s)} = \frac{K}{(Ls + R)(Js + B) + K^2}
```

| $R$ | $L$ | $K$ | $J$ | $B$ |
|---|---|---|---|---|
| 1 Ω | 1 H | 0.5 V·s/rad | 0.1 kg·m² | 0.5 N·m·s/rad |

이 값이면 특성방정식이 $s^2 + 6s + 7.5 = 0$이고 극점은 $-1.78$, $-4.22$입니다. $\zeta \approx 1.1$인 과감쇠라서 속도가 오버슈트 없이 올라가 아래 값에 수렴합니다.

```math
\omega_{ss} = \frac{K}{RB + K^2}\,V = \frac{0.5}{0.75} \times 1 \approx 0.67\ \text{rad/s}\ (\approx 6.4\ \text{rpm})
```

각속도는 Gain `60/(2*pi)`로 RPM으로 바꿔 표시했습니다. Solver Configuration은 로컬 솔버(Backward Euler, 1×10⁻⁵ s)를 쓰고, 모델은 고정 스텝 이산 솔버로 5초 동안 실행합니다.

## 사용한 블록

| 구분 | 블록 |
|---|---|
| 기계 | Ideal Torque Source, Inertia, Rotational Damper, Gear Box, Mechanical Rotational Reference |
| 전기 | DC Voltage Source, DC Motor, Electrical Reference |
| 센서 | Ideal Rotational Motion Sensor, Current Sensor, Voltage Sensor |
| 연결 | Solver Configuration, Simulink-PS Converter, PS-Simulink Converter |
