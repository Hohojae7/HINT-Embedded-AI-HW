# 06. Ollama 로컬 LLM과 GPIO

Pi에 Ollama를 설치해 인터넷 없이 LLM(gemma3:1b)을 돌리고, REST API로 파이썬에서 호출한 실습입니다. 마지막에는 자연어 명령을 LLM이 해석해 LED를 켜고 끄는 스마트홈 비서를 만들었습니다.

| 파일 | 내용 |
|---|---|
| [`p108_ollama_api.py`](p108_ollama_api.py) | `localhost:11434/api/generate`에 질문을 보내고 답변을 한 번에 받기 (`stream=False`) |
| [`p109_ollama_stream.py`](p109_ollama_stream.py) | 답변을 JSON 줄 단위 조각으로 받아 생성되는 대로 바로 출력 (`stream=True`) |
| [`p114_ai_gpio_assistant.py`](p114_ai_gpio_assistant.py) | "거실 불 켜줘" 같은 명령을 LLM이 JSON 명령으로 바꾸면, 검증한 뒤 해당 LED를 제어 |

실행 전에 `ollama run gemma3:1b`로 모델을 한 번 내려받아 둡니다.

## 스마트홈 비서 (p114)

```text
사용자 입력 → 프롬프트(규칙 + 현재 LED 상태 + 입력) → gemma3:1b → JSON 명령 → 검증 → LED 제어
```

| 장소 | GPIO (BCM) | 물리 핀 |
|---|---|---|
| 거실 | 17 | 11 |
| 방 | 27 | 13 |
| 주방 | 22 | 15 |

- 동작은 `on`, `off`, `toggle`, `status` 네 가지이며, "다 꺼"처럼 여러 장소를 한 번에 제어할 수도 있습니다.
- 요청에 `format: "json"`과 `temperature: 0`을 넣어, 모델이 정해진 JSON 형식으로 흔들림 없이 답하도록 했습니다.
- 명령 전체를 먼저 검증하고, 등록되지 않은 장소나 지원하지 않는 동작이 하나라도 있으면 LED를 건드리지 않고 다시 입력받습니다.
- GPIO는 `gpiozero`에 `lgpio` 백엔드(`chip=0`)를 지정해 사용하고, 종료할 때 LED를 모두 끕니다.
