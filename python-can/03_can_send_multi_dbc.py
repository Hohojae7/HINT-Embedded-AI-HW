import can
import cantools
import time
import random
from pathlib import Path

can_bus = can.Bus(interface='kvaser', channel=1, bitrate=1000000)

try:
    can_db = cantools.database.load_file(Path(__file__).with_name('project.dbc'))
    messages_to_send = [message for message in can_db.messages if 'ECU1' in message.senders]

    while True:
        for msg in messages_to_send:
            data = {sig.name: random.randint(sig.minimum or 0, sig.maximum or (2**sig.length - 1)) for sig in msg.signals}
            can_msg = can.Message(arbitration_id = msg.frame_id, data=msg.encode(data), is_extended_id=False)
            try:
                can_bus.send(can_msg)
                print(f"Message sent: {can_msg}")
            except can.CanError as error:
                print(f"Message NOT sent: {error}")
            time.sleep(0.1)
except KeyboardInterrupt:
    print("Sender stopped.")
finally:
    can_bus.shutdown()
