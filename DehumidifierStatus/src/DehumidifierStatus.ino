
const int photoresistor = A1;
const int signalThreshold = 1500;
const int thresholdSeconds = 30;

enum TankState {
    UNKNOWN,
    EMPTY,
    FULL
};

int analogValue;
TankState currentState = UNKNOWN;
TankState newState = UNKNOWN;
int newStateTime = -1;

TankState boolToTankState(bool tankFull) {
    return tankFull ? FULL : EMPTY;
}

Timer checkTimer(5000, checkValue);

void setup() {
    pinMode(photoresistor,INPUT);

    Particle.variable("analogvalue", &analogValue, INT);

    checkTimer.start();
    Particle.publish("dehumid/start");
}

void checkValue() {
    analogValue = analogRead(photoresistor);
    bool tankFull = analogValue > signalThreshold;
    TankState value = boolToTankState(tankFull);

    // Does this match the current state
    if (currentState == value) {
        newState = UNKNOWN;
        return;
    }

    if (newState != value) {
        newState = value;
        newStateTime = Time.now();
    }

    if (Time.now() - newStateTime > thresholdSeconds) {
        currentState = newState;
        newState = UNKNOWN;
        newStateTime = -1;

        Particle.publish("dehumid/state", String(currentState));
    }
}
