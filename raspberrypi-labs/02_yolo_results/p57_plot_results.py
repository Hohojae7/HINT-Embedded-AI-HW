from pathlib import Path
from PIL import Image
from ultralytics import YOLO

model = YOLO("yolo26n.pt")

results = model(
    [
        "https://ultralytics.com/images/bus.jpg",
        "https://ultralytics.com/images/zidane.jpg",
    ],
    device="cpu",
    imgsz=320,
)

output_dir = Path("outputs")
output_dir.mkdir(exist_ok=True)

for i, r in enumerate(results):
    im_bgr = r.plot()
    im_rgb = Image.fromarray(im_bgr[..., ::-1])

    r.show()
    r.save(filename=str(output_dir / f"results{i}.jpg"))