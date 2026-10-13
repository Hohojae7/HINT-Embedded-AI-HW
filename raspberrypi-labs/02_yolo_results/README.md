# 02. YOLO 결과 객체 읽기

`model()`이 돌려주는 `Results`에서 작업별 결과를 꺼내 출력해 본 실습입니다. 모델 이름 뒤의 접미사(`-seg`, `-pose` 등)만 바꾸면 같은 코드 구조로 다른 작업을 할 수 있습니다.

| 파일 | 모델 | 확인한 결과 | 내용 |
|---|---|---|---|
| [`p44_results_single_batch.py`](p44_results_single_batch.py) | yolo26n | `len(results)` | 이미지 1장과 2장(배치)을 넣어, 결과가 이미지 수만큼의 리스트로 나오는지 확인 |
| [`p51_boxes.py`](p51_boxes.py) | yolo26n | `r.boxes` | 탐지 박스의 좌표, 신뢰도, 클래스 ID |
| [`p52_instance_masks.py`](p52_instance_masks.py) | yolo26n-seg | `r.masks` | 객체마다 하나씩 나오는 픽셀 마스크 (인스턴스 분할) |
| [`p53_semantic_mask.py`](p53_semantic_mask.py) | yolo26n-sem | `r.semantic_mask.data` | 픽셀마다 클래스 ID를 매긴 지도 한 장 (시맨틱 분할) |
| [`p54_keypoints.py`](p54_keypoints.py) | yolo26n-pose | `r.keypoints` | 사람마다 관절 17개의 좌표 |
| [`p55_class_probs.py`](p55_class_probs.py) | yolo26n-cls | `r.probs` | 이미지 전체에 대한 클래스 확률과 상위 1개, 5개 클래스 |
| [`p56_obb.py`](p56_obb.py) | yolo26n-obb | `r.obb` | `boats.jpg`의 배처럼 기울어진 물체를 감싸는 회전 박스 |
| [`p57_plot_results.py`](p57_plot_results.py) | yolo26n | `plot()`, `save()` | 결과를 이미지에 그려 화면에 띄우고 `outputs/`에 저장 |

- 인스턴스 분할은 "사람 1, 사람 2"처럼 객체를 하나하나 구분하고, 시맨틱 분할은 같은 클래스면 하나로 칠합니다.
- `plot()`이 돌려주는 이미지는 OpenCV 순서(BGR)라서, PIL로 다룰 때는 `[..., ::-1]`로 RGB 순서로 바꿉니다.
