# 01. Simulink 동적 시스템 모델링

수식을 블록선도로 옮기는 기본기를 익힌 실습입니다. 적분기(Integrator) 출력을 다시 입력 쪽으로 되먹이면 미분방정식이 되고, 결과는 Scope로 보거나 워크스페이스로 꺼내 그래프로 그렸습니다.

| 파일 | 내용 |
|---|---|
| [`ex5_3.slx`](ex5_3.slx) | $3\sin(2\pi t)$를 적분하고, 적분 전후 신호를 Mux로 묶어 To Workspace(`simout`, Timeseries)로 저장 |
| [`ex5_3_plot.m`](ex5_3_plot.m) | `out.simout`에서 시간과 두 신호를 꺼내 한 그래프에 그림 |
| [`ex5_8.slx`](ex5_8.slx) | 1차 미분방정식 모델 |
| [`ex5_12.slx`](ex5_12.slx) | 질량-스프링-댐퍼 2차 시스템. 감쇠 `c`와 강성 `k`를 워크스페이스 변수로 받음 |
| [`ex5_12.m`](ex5_12.m) | `c`, `k`를 3가지씩 바꿔 9번 `sim()`을 실행하고 응답을 겹쳐 그림 |

## 수식

**적분 (ex5_3)**: 초기값 0에서 적분하면 0과 $3/\pi$ 사이를 오가는 코사인 모양이 됩니다.

```math
y(t) = \int_0^t 3\sin(2\pi\tau)\,d\tau = \frac{3}{2\pi}\big(1 - \cos 2\pi t\big)
```

**1차 시스템 (ex5_8)**: 시정수 0.2 s로 초기값의 영향이 금방 사라지고, 이후에는 입력과 같은 3 rad/s로 진동합니다(진폭 $4/\sqrt{34} \approx 0.69$).

```math
\dot{y} = 4\sin 3t - 5y, \qquad y(0) = 1
```

**2차 시스템 (ex5_12)**: 질량 1, 크기 5의 일정한 힘을 받는 질량-스프링-댐퍼입니다.

```math
\ddot{y} + c\dot{y} + ky = 5, \qquad y(0) = \dot{y}(0) = 0
```

```math
\omega_n = \sqrt{k}, \qquad \zeta = \frac{c}{2\sqrt{k}}, \qquad y_{ss} = \frac{5}{k}
```

$c \in \{1, 2, 3\}$, $k \in \{2, 3, 4\}$ 조합에서 `c`가 클수록 진동이 줄고, `k`가 클수록 빨라지면서 최종값은 작아집니다. 9개 중 $c = 3, k = 2$만 $\zeta > 1$인 과감쇠입니다.

## 사용한 블록

| 블록 | 용도 |
|---|---|
| Sine Wave, Constant | 입력 신호 |
| Gain, Sum | 계수 곱하기, 피드백 항 빼기 |
| Integrator | $\dot{y} \to y$ 적분. 초기 조건으로 $y(0)$ 지정 |
| Mux, Scope | 여러 신호를 묶어 한 화면에 표시 |
| To Workspace | 결과를 `out.simout`으로 워크스페이스에 저장 |
