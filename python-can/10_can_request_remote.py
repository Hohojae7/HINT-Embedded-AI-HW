import can
import time

can_bus = can.Bus(interface='kvaser', channel=1, bitrate=1000000)

try:
    rtr_msg = can.Message(arbitration_id=0x123, dlc=8, is_remote_frame=True, is_extended_id=False)
    print("Requester ready: channel=1, bitrate=1000000. (Ctrl+C to stop)", flush=True)
    while True:
        try:
            can_bus.send(rtr_msg)
            print(f"Request Sent: {rtr_msg}", flush=True)
            deadline = time.monotonic() + 2.0
            while True:
                remaining = deadline - time.monotonic()
                if remaining <= 0:
                    print("No response received within 2 seconds. Retrying...", flush=True)
                    break
                msg = can_bus.recv(timeout=min(0.5, remaining))
                if (msg is not None and msg.arbitration_id == rtr_msg.arbitration_id
                        and not msg.is_remote_frame and not msg.is_extended_id
                        and not msg.is_error_frame and msg.dlc == rtr_msg.dlc):
                    print(f"Response Received: {msg}", flush=True)
                    break
        except can.CanError as error:
            print(f"Message NOT sent: {error}")
        time.sleep(1)
except KeyboardInterrupt:
    print("Requester stopped.")
finally:
    can_bus.shutdown()
