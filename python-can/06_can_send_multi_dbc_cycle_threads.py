import can
import cantools
import random
import threading
from pathlib import Path

stop_event = threading.Event()
worker_failed = threading.Event()


def send_cyclic_msg(msg):
    interval = msg.cycle_time / 1000.0
    while not stop_event.is_set():
        send_msg(msg)
        if stop_event.wait(interval):
            break


def send_msg(msg):
    try:
        if stop_event.is_set():
            return
        data = {sig.name: random.randint(sig.minimum or 0, sig.maximum or (2**sig.length - 1)) for sig in msg.signals}
        can_msg = can.Message(arbitration_id = msg.frame_id, data=msg.encode(data), is_extended_id=False)
        can_bus.send(can_msg)
        print(f"Message sent: {can_msg}", flush=True)
    except can.CanError as error:
        print(f"Message NOT sent: {error}")
    except Exception:
        worker_failed.set()
        stop_event.set()
        raise


can_bus = can.ThreadSafeBus(interface='kvaser', channel=1, bitrate=1000000)
threads = []

try:
    can_db = cantools.database.load_file(Path(__file__).with_name('project.dbc'))
    messages_to_send = [message for message in can_db.messages if 'ECU1' in message.senders]

    for msg in messages_to_send:
        if stop_event.is_set():
            break
        if (msg.send_type == 'Cyclic') and (msg.cycle_time is not None) and (msg.cycle_time > 0):
            target = send_cyclic_msg
        else:
            target = send_msg
        thread_msg = threading.Thread(target=target, args=(msg,), daemon=True)
        threads.append(thread_msg)
        thread_msg.start()

    while not stop_event.wait(0.1):
        pass
except KeyboardInterrupt:
    print("End of transmission")
finally:
    stop_event.set()
    for thread_msg in threads:
        if thread_msg.is_alive():
            thread_msg.join()
    can_bus.shutdown()

if worker_failed.is_set():
    raise SystemExit(1)
