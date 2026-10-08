import can

can_bus = can.Bus(interface='kvaser', channel=0, bitrate=1000000)
print("Receiver ready: channel=0, bitrate=1000000. Waiting for messages... (Ctrl+C to stop)", flush=True)

try:
    while True:
        can_msg = can_bus.recv(timeout=5.0)
        if can_msg:
            print(f"Message received: {can_msg}", flush=True)
        else:
            print("No message received in 5 seconds. Still listening...", flush=True)
except KeyboardInterrupt:
    print("Receiver stopped.")
finally:
    can_bus.shutdown()
