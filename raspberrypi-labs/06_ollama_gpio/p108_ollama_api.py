import requests


def ask_llm(prompt, model="gemma3:1b"):
    """Ollama REST API로 질문을 보내고 답변 텍스트를 반환한다."""

    response = requests.post(
        "http://localhost:11434/api/generate",
        json={
            "model": model,
            "prompt": prompt,
            "stream": False,
        },
        timeout=600,
    )

    # HTTP 요청이 실패했다면 오류를 표시한다.
    response.raise_for_status()

    # 응답 데이터에서 모델의 답변 텍스트를 꺼낸다.
    return response.json()["response"]


if __name__ == "__main__":
    question = "Python에서 LED를 제어하는 코드를 작성해줘"

    print(f"질문: {question}")
    print("답변을 생성하고 있습니다. 잠시 기다려 주세요.")

    answer = ask_llm(question)

    print("\n=== LLM 답변 ===")
    print(answer)