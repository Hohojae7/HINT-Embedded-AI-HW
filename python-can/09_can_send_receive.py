import can
import cantools
import time
import random
import threading
import sys
from pathlib import Path

filters = [
    {"can_id": 0x001, "can_mask": 0xFFF, "extended": False},
    {"can_id": 0x005, "can_mask": 0x00F, "extended": False},
    {"can_id": 0x100, "can_mask": 0xFF0, "extended": False},
    {"can_id": 0x400, "can_mask": 0xF00, "extended": False},
    {"can_id": 0x501, "can_mask": 0xFF1, "extended": False},
    {"can_id": 0x602, "can_mask": 0xFFF, "extended": False},
    {"can_id": 0x700, "can_mask": 0xFF2, "extended": False},
]

if len(sys.argv) != 2:
    raise SystemExit("Usage: python 09_can_send_receive.py <channel> (0 or 1 for this practice)")

try:
    channel = int(sys.argv[1])
    if channel < 0:
        raise ValueError
except ValueError:
    raise SystemExit("Channel must be a non-negative integer (0 or 1 for this practice).") from None

timing = can.BitTiming(f_clock=16_000_000, brp=2, tseg1=5, tseg2=2, sjw=2)
can_bus = can.ThreadSafeBus(interface='kvaser', channel=channel, timing=timing, can_filters=filters)
stop_event = threading.Event()
worker_failed = threading.Event()
periodic_tasks = []


def recv_msg():
    try:
        while not stop_event.is_set():
            msg = can_bus.recv(timeout=0.5)
            if msg:
                print(f"Message received: {msg}", flush=True)
    except Exception:
        worker_failed.set()
        stop_event.set()
        raise

def send_msg():
    try:
        can_db = cantools.database.load_file(Path(__file__).with_name('project.dbc'))
        messages_to_send = [message for message in can_db.messages if 'ECU1' in message.senders]
        for msg in messages_to_send:
            if stop_event.is_set():
                break
            data = {sig.name: random.randint(sig.minimum or 0, sig.maximum or (2**sig.length - 1)) for sig in msg.signals}
            can_msg = can.Message(arbitration_id = msg.frame_id, data=msg.encode(data), is_extended_id=False)
            try:
                if (msg.send_type == 'Cyclic') and (msg.cycle_time is not None) and (msg.cycle_time > 0):
                    periodic_tasks.append(can_bus.send_periodic(can_msg, msg.cycle_time / 1000))
                else:
                    can_bus.send(can_msg)
                print(f"Message sent: {can_msg}", flush=True)
            except can.CanError as error:
                print(f"Message NOT sent: {error}")
    except Exception:
        worker_failed.set()
        stop_event.set()
        raise


thread_msg_rx = threading.Thread(target=recv_msg, args=())
thread_msg_rx.daemon = True

thread_msg_tx = threading.Thread(target=send_msg, args=())
thread_msg_tx.daemon = True

try:
    print(f"CAN ready: channel={channel}, bitrate={timing.bitrate}. (Ctrl+C to stop)", flush=True)
    thread_msg_rx.start()
    if not stop_event.wait(5):
        thread_msg_tx.start()
    while not stop_event.is_set():
        time.sleep(0.1)
except KeyboardInterrupt:
    print("End of transmission")
finally:
    stop_event.set()
    if thread_msg_rx.is_alive():
        thread_msg_rx.join()
    if thread_msg_tx.is_alive():
        thread_msg_tx.join()
    for task in periodic_tasks:
        task.stop()
    for task in periodic_tasks:
        thread = getattr(task, 'thread', None)
        if thread is not None and thread.is_alive():
            thread.join()
    can_bus.shutdown()

if worker_failed.is_set():
    raise SystemExit(1)
