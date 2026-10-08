import can

can_bus = can.Bus(interface='kvaser', channel=0, bitrate=1000000)

try:
    response_data = [0, 25, 0, 1, 3, 1, 4, 1]
    print("Responder ready: channel=0, bitrate=1000000. Waiting for requests... (Ctrl+C to stop)", flush=True)
    while True:
        msg = can_bus.recv(timeout=0.5)
        if msg is None:
            continue
        if msg.is_remote_frame and not msg.is_extended_id and msg.arbitration_id == 0x123:
            print(f"Request Received: {msg}", flush=True)
            response_msg = can.Message(arbitration_id=0x123, data=response_data, is_extended_id=False)
            try:
                can_bus.send(response_msg)
                print(f"Data sent: {response_msg}", flush=True)
            except can.CanError as error:
                print(f"Message NOT sent: {error}")
except KeyboardInterrupt:
    print("Responder stopped.")
finally:
    can_bus.shutdown()
