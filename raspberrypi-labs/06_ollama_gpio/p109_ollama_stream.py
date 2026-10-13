import json

import requests


def ask_llm_stream(prompt, model="gemma3:1b"):
    """Ollama의 답변을 스트리밍으로 받아 실시간 출력"""

    with requests.post(
        "http://localhost:11434/api/generate",
        json={
            "model": model,
            "prompt": prompt,
            "stream": True,
        },
        stream=True,
        timeout=600,
    ) as response:
        # HTTP 요청이 실패했다면 오류를 표시
        response.raise_for_status()


        for line in response.iter_lines(chunk_size=1):
            if not line:
                continue

            data = json.loads(line)

            # 스트리밍 중 Ollama가 오류를 반환한 경우
            if data.get("error"):
                raise RuntimeError(data["error"])


            print(
                data.get("response", ""),
                end="",
                flush=True,
            )


            if data.get("done"):
                print()
                break


if __name__ == "__main__":
    question = "IoT 프로젝트 아이디어 3가지 제안해줘"

    print(f"질문: {question}")
    print("\n=== LLM 답변 (실시간 출력) ===")

    ask_llm_stream(question)