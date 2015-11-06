class SensorAck(object):
    def __init__(self, SensorAck):
        self.SensorAck = SensorAck

class TicketDispenser(object):
    def getSensorAck(self):
        newTurnNumber = TurnNumberSequence.next_turn_number()
        newSensorAck = SensorAck(newTurnNumber)
        return newSensorAck
