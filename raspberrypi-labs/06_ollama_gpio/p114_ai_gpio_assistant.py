import json

import requests
from gpiozero import LED
from gpiozero.pins.lgpio import LGPIOFactory


OLLAMA_URL = "http://localhost:11434/api/generate"
MODEL = "gemma3:1b"

# BCM GPIO 번호: 거실=물리 핀 11, 방=13, 주방=15
LED_PINS = {
    "거실": 17,
    "방": 27,
    "주방": 22,
}

devices = {}
device_states = {name: False for name in LED_PINS}

SYSTEM_PROMPT = """당신은 스마트홈 AI 비서입니다.
사용자의 자연어 명령을 분석하여 JSON 형식으로만 응답하세요.

제어 가능한 장소: 거실, 방, 주방
가능한 동작: on (켜기), off (끄기), toggle (전환), status (상태 확인)

응답 형식:
{"commands": [{"room": "장소", "action": "동작"}], "message": "사용자에게 보여줄 응답"}

규칙:
- 장치 제어는 거실, 방, 주방 중 해당 장소를 지정하세요.
- 모든 불을 제어할 때는 장소별 명령을 각각 작성하세요.
- 상태 확인은 room을 "all", action을 "status"로 지정하세요.
- 이해하지 못한 요청에는 commands를 빈 배열로 응답하세요.
- JSON 밖에 설명이나 코드 블록을 붙이지 마세요.

예시:
사용자: 거실 불 켜
응답: {"commands":[{"room":"거실","action":"on"}],"message":"거실 불을 켜겠습니다."}

사용자: 다 꺼
응답: {"commands":[{"room":"거실","action":"off"},{"room":"방","action":"off"},{"room":"주방","action":"off"}],"message":"모든 불을 끄겠습니다."}

사용자: 방 불 상태를 반대로 바꿔
응답: {"commands":[{"room":"방","action":"toggle"}],"message":"방 불의 상태를 전환하겠습니다."}

사용자: 지금 상태 알려줘
응답: {"commands":[{"room":"all","action":"status"}],"message":"현재 상태를 확인합니다."}
"""


def get_device_status():
    """프로그램이 관리하는 LED 상태를 문자열로 반환한다."""
    status_lines = []

    for name, state in device_states.items():
        status = "켜짐" if state else "꺼짐"
        status_lines.append(f"  - {name}: {status}")

    return "\n".join(status_lines)


def control_device(room, action):
    """지정한 LED를 제어하고 결과를 반환한다."""
    if room not in devices:
        return f"등록되지 않은 장소: {room}"

    led = devices[room]

    if action == "on":
        led.on()
        device_states[room] = True
        return f"{room} 불을 켰습니다."

    if action == "off":
        led.off()
        device_states[room] = False
        return f"{room} 불을 껐습니다."

    if action == "toggle":
        led.toggle()
        device_states[room] = not device_states[room]

        state = "켰습니다" if device_states[room] else "껐습니다"
        return f"{room} 불을 {state}."

    return f"알 수 없는 동작: {action}"


def parse_command(user_input):
    """Ollama로 명령을 분석하고 응답 구조를 확인한다."""
    prompt = (
        f"{SYSTEM_PROMPT}\n\n"
        f"현재 장치 상태:\n{get_device_status()}\n\n"
        f"사용자: {user_input}"
    )

    response = requests.post(
        OLLAMA_URL,
        json={
            "model": MODEL,
            "prompt": prompt,
            "stream": False,
            "format": "json",
            "options": {"temperature": 0},
        },
        timeout=(10, 600),
    )

    response.raise_for_status()

    # 첫 번째 JSON: Ollama API의 응답 전체
    content = response.json().get("response")

    if not isinstance(content, str):
        raise ValueError("Ollama 응답에 답변 텍스트가 없습니다.")

    # 두 번째 JSON: 모델이 작성한 장치 제어 명령
    parsed = json.loads(content)

    if not isinstance(parsed, dict):
        raise ValueError("명령 응답이 JSON 객체가 아닙니다.")

    commands = parsed.get("commands")

    if not isinstance(commands, list):
        raise ValueError("commands가 배열이 아닙니다.")

    # 모든 명령을 확인한 다음 실제 LED를 제어한다.
    for cmd in commands:
        if not isinstance(cmd, dict):
            raise ValueError("명령 형식이 올바르지 않습니다.")

        room = cmd.get("room")
        action = cmd.get("action")

        if action == "status" and room == "all":
            continue

        if not isinstance(room, str) or room not in LED_PINS:
            raise ValueError("모델이 등록되지 않은 장소를 지정했습니다.")

        if action not in ("on", "off", "toggle"):
            raise ValueError("모델이 지원하지 않는 동작을 지정했습니다.")

    return parsed


def main():
    factory = None

    try:
        # 현재 Raspberry Pi OS Trixie의 Pi 5 GPIO 장치
        factory = LGPIOFactory(chip=0)

        for room, pin in LED_PINS.items():
            devices[room] = LED(
                pin,
                initial_value=False,
                pin_factory=factory,
            )

        print("=" * 50)
        print("스마트홈 AI 비서")
        print("제어 가능: 거실, 방, 주방")
        print("예: 거실 불 켜줘 / 다 꺼 / 지금 상태 알려줘")
        print("'종료' 또는 'quit'으로 종료")
        print("=" * 50)

        while True:
            user_input = input("\n명령: ").strip()

            if user_input.lower() in ("종료", "quit", "exit", "q"):
                break

            if not user_input:
                continue

            print("분석 중...", flush=True)

            try:
                parsed = parse_command(user_input)

            except requests.RequestException as error:
                print(f"Ollama 통신 오류: {error}")
                continue

            except ValueError as error:
                print(f"명령 해석 실패: {error}")
                print("명령을 조금 더 명확하게 입력해 주세요.")
                continue

            for cmd in parsed["commands"]:
                if cmd["action"] == "status":
                    print(f"\n현재 상태:\n{get_device_status()}")
                else:
                    result = control_device(
                        cmd["room"],
                        cmd["action"],
                    )
                    print(result)

            message = parsed.get("message", "")

            if isinstance(message, str) and message:
                print(f"\nAI: {message}")

    except (KeyboardInterrupt, EOFError):
        print("\n종료합니다.")

    finally:
        for led in devices.values():
            led.off()
            led.close()

        if factory is not None:
            factory.close()

        print("LED를 모두 끄고 종료했습니다.")


if __name__ == "__main__":
    main()