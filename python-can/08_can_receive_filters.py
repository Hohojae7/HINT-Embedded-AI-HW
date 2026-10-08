import can

filters = [
    {"can_id": 0x001, "can_mask": 0xFFF, "extended": False},
    {"can_id": 0x005, "can_mask": 0x00F, "extended": False},
    {"can_id": 0x100, "can_mask": 0xFF0, "extended": False},
    {"can_id": 0x400, "can_mask": 0xF00, "extended": False},
    {"can_id": 0x501, "can_mask": 0xFF1, "extended": False},
    {"can_id": 0x602, "can_mask": 0xFFF, "extended": False},
    {"can_id": 0x700, "can_mask": 0xFF2, "extended": False},
]

bus = can.Bus(interface='kvaser', channel=0, bitrate=1000000, can_filters=filters)
print("Receiver ready: channel=0, bitrate=1000000, filters enabled. Waiting for messages... (Ctrl+C to stop)", flush=True)

try:
    while True:
        msg = bus.recv(timeout=5.0)
        if msg:
            print(f"Message received: {msg}", flush=True)
        else:
            print("No matching message received in 5 seconds. Still listening...", flush=True)
except KeyboardInterrupt:
    print("Receiver stopped.")
finally:
    bus.shutdown()
