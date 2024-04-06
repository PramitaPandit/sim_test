/*
 * my_simulation_network.cc
 *
 *  Created on: Feb 1, 2024
 *      Author: pramita
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class node11 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variable
    double x; // Variable x

  public:
    node11(int id = 0, double initialX = 5.0) : nodeId(id), x(initialX) {} // Initialize with nodeId and initialX in constructor

    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval

};

Define_Module(node11);
void node11::initialize()
{
    nodeId = atoi(getName());
    EV << "Node " << nodeId << " initialized\n";

    // Initialize x from a parameter
    x = par("initialX").doubleValue();

    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);
}
void node11::handleMessage(cMessage *msg)
{
    // Check if the message received is "Hello There!"
        if (strcmp(msg->getName(), "Hello There!") == 0) {
            EV << "Received 'Hello There!' message from the hub. Starting message transmission.\n";
            // Start transmitting messages
            transmitMessage();
        } else {
            // Handle other messages here
            EV << "Node " << getName() << " received a message, but waiting for 'Hello There!' from the hub.\n";
            // Store the received message and handle it later
            // Example: storeMessage(msg);
        }
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "Node " << getName() << " (ID: " << nodeId << ") decremented energy to " << x << " at time " << simTime() << ".\n";

        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        if (!channelBusy && simTime() >= nodeId * slotDuration) {
            transmitMessage();
            EV << "Node " << nodeId << " transmitted a message.\n";
            bubble("Message Transmitted from Node11!");
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing energy to " << x << "\n";
            bubble("Message Received at Node11!");

            backoff();
            // Reschedule the message for the next time slot
            scheduleAt(nodeId * slotDuration, msg);
        }

        // Check if decrementXMsg is already scheduled, cancel it before rescheduling
        if (decrementXMsg->isScheduled()) {
            cancelEvent(decrementXMsg);
        }
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    }
    // Forward the message to Obn_node
        int gateIndex = 0; // Assuming you want to forward all messages through gate index 0
        cModule *obnNode = findModuleByPath("OBN_node");
        if (obnNode) {
            cMessage *copyOfMsg = msg->dup(); // Duplicate the message
            send(copyOfMsg, "out", gateIndex); // Send the duplicated message to Obn_node
        } else {
            EV << "Error: OBN_node not found\n";
        }
}

void node11::transmitMessage()
{
    // Create and send the message
    cMessage *msg11 = new cMessage("145");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg11->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node11!");
    send(msg11, "output_gate", gateIndex);
    cMessage *msg12 = new cMessage("120");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg12->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node11!");
    send(msg12, "output_gate", gateIndex);
    cMessage *msg13 = new cMessage("130");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg13->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node12!");
    send(msg13, "output_gate", gateIndex);
    cMessage *msg14 = new cMessage("172");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg14->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node11!");
    send(msg14, "output_gate", gateIndex);
    cMessage *msg15 = new cMessage("190");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg15->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node11!");
    send(msg15, "output_gate", gateIndex);
}
void node11::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}



class node12 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variable
    double x; // Variable x

  public:
    node12() : x(5.0) {} // Default constructor with x initialized to 5
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval

};

Define_Module(node12);
void node12::initialize()
{
    nodeId = atoi(getName());
    EV << "Node " << nodeId << " initialized\n";

    // Initialize x from a parameter
    x = par("initialX").doubleValue();

    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);
}
void node12::handleMessage(cMessage *msg)
{
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "Node " << getName() << " (ID: " << nodeId << ") decremented x to " << x << " at time " << simTime() << ".\n";

        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        if (!channelBusy && simTime() >= nodeId * slotDuration) {
            transmitMessage();
            EV << "Node " << nodeId << " transmitted a message.\n";
            bubble("Message Transmitted from Node12!");
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";
            bubble("Message Received at Node12!");

            backoff();
            // Reschedule the message for the next time slot
            scheduleAt(nodeId * slotDuration, msg);
        }

        // Check if decrementXMsg is already scheduled, cancel it before rescheduling
        if (decrementXMsg->isScheduled()) {
            cancelEvent(decrementXMsg);
        }
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    }
}

void node12::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("204");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node12!");
    send(msg1, "output_gate", gateIndex);
    cMessage *msg2 = new cMessage("233");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg2->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node12!");
    send(msg2, "output_gate", gateIndex);
    cMessage *msg3 = new cMessage("260");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg3->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node12!");
    send(msg3, "output_gate", gateIndex);
    cMessage *msg4 = new cMessage("354");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg4->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node12!");
    send(msg4, "output_gate", gateIndex);
    cMessage *msg5 = new cMessage("192");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg5->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node12!");
    send(msg5, "output_gate", gateIndex);
}
void node12::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}


class node21 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variable
    double x; // Variable x

  public:
    node21() : x(5.0) {} // Default constructor with x initialized to 5
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval

};

Define_Module(node21);
void node21::initialize()
{
    nodeId = atoi(getName());
    EV << "Node " << nodeId << " initialized\n";

    // Initialize x from a parameter
    x = par("initialX").doubleValue();

    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);
}
void node21::handleMessage(cMessage *msg)
{
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "Node " << getName() << " (ID: " << nodeId << ") decremented x to " << x << " at time " << simTime() << ".\n";

        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        if (!channelBusy && simTime() >= nodeId * slotDuration) {
            transmitMessage();
            EV << "Node " << nodeId << " transmitted a message.\n";
            bubble("Message Transmitted from Node21!");
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";
            bubble("Message Received at Node21!");

            backoff();
            // Reschedule the message for the next time slot
            scheduleAt(nodeId * slotDuration, msg);
        }

        // Check if decrementXMsg is already scheduled, cancel it before rescheduling
        if (decrementXMsg->isScheduled()) {
            cancelEvent(decrementXMsg);
        }
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    }
}

void node21::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("150");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node21!");
    send(msg1, "output_gate", gateIndex);
    cMessage *msg2 = new cMessage("120");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg2->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node21!");
    send(msg2, "output_gate", gateIndex);
    cMessage *msg3 = new cMessage("115");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg3->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node21!");
    send(msg3, "output_gate", gateIndex);
    cMessage *msg4 = new cMessage("95");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg4->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node21!");
    send(msg4, "output_gate", gateIndex);
    cMessage *msg5 = new cMessage("72");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg5->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node21!");
    send(msg5, "output_gate", gateIndex);
}
void node21::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}


class node22 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variable
    double x; // Variable x

  public:
    node22() : x(5.0) {} // Default constructor with x initialized to 5
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval

};

Define_Module(node22);
void node22::initialize()
{
    nodeId = atoi(getName());
    EV << "Node " << nodeId << " initialized\n";

    // Initialize x from a parameter
    x = par("initialX").doubleValue();

    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);
}
void node22::handleMessage(cMessage *msg)
{
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "Node " << getName() << " (ID: " << nodeId << ") decremented x to " << x << " at time " << simTime() << ".\n";

        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        if (!channelBusy && simTime() >= nodeId * slotDuration) {
            transmitMessage();
            EV << "Node " << nodeId << " transmitted a message.\n";
            bubble("Message Transmitted from Node22!");
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";
            bubble("Message Received at Node22!");

            backoff();
            // Reschedule the message for the next time slot
            scheduleAt(nodeId * slotDuration, msg);
        }

        // Check if decrementXMsg is already scheduled, cancel it before rescheduling
        if (decrementXMsg->isScheduled()) {
            cancelEvent(decrementXMsg);
        }
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    }
}

void node22::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("160");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node22!");
    send(msg1, "output_gate", gateIndex);
    cMessage *msg2 = new cMessage("187");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg2->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node22!");
    send(msg2, "output_gate", gateIndex);
    cMessage *msg3 = new cMessage("172");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg3->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node22!");
    send(msg3, "output_gate", gateIndex);
    cMessage *msg4 = new cMessage("139");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg4->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node22!");
    send(msg4, "output_gate", gateIndex);
    cMessage *msg5 = new cMessage("115");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg5->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node22!");
    send(msg5, "output_gate", gateIndex);
}
void node22::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}


class node31 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variable
    double x; // Variable x

  public:
    node31() : x(5.0) {} // Default constructor with x initialized to 5
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval

};

Define_Module(node31);
void node31::initialize()
{
    nodeId = atoi(getName());
    EV << "Node " << nodeId << " initialized\n";

    // Initialize x from a parameter
    x = par("initialX").doubleValue();

    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);
}
void node31::handleMessage(cMessage *msg)
{
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "Node " << getName() << " (ID: " << nodeId << ") decremented x to " << x << " at time " << simTime() << ".\n";

        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        if (!channelBusy && simTime() >= nodeId * slotDuration) {
            transmitMessage();
            EV << "Node " << nodeId << " transmitted a message.\n";
            bubble("Message Transmitted from Node31!");
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";
            bubble("Message Received at Node31!");

            backoff();
            // Reschedule the message for the next time slot
            scheduleAt(nodeId * slotDuration, msg);
        }

        // Check if decrementXMsg is already scheduled, cancel it before rescheduling
        if (decrementXMsg->isScheduled()) {
            cancelEvent(decrementXMsg);
        }
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    }
}

void node31::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("1.02");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node31!");
    send(msg1, "output_gate", gateIndex);
    cMessage *msg2 = new cMessage("1.005");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg2->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node31!");
    send(msg2, "output_gate", gateIndex);
    cMessage *msg3 = new cMessage("1.01");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg3->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node31!");
    send(msg3, "output_gate", gateIndex);
    cMessage *msg4 = new cMessage("1.025");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg4->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node31!");
    send(msg4, "output_gate", gateIndex);
    cMessage *msg5 = new cMessage("1.015");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg5->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node31!");
    send(msg5, "output_gate", gateIndex);
}
void node31::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}


class node32 : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void transmitMessage();
    virtual void backoff();

    // Existing variables
    int nodeId;
    double slotDuration = 0.01;
    bool channelBusy = false;

    // New variable
    double x; // Variable x

  public:
    node32() : x(5.0) {} // Default constructor with x initialized to 5
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval

};

Define_Module(node32);
void node32::initialize()
{
    nodeId = atoi(getName());
    EV << "Node " << nodeId << " initialized\n";

    // Initialize x from a parameter
    x = par("initialX").doubleValue();

    if (nodeId == 0) {
        char msgname[20];
        sprintf(msgname, "Hello-%d", nodeId);
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
    decrementXMsg = new cMessage("decrementX");
    scheduleAt(simTime() + decrementInterval, decrementXMsg);
}
void node32::handleMessage(cMessage *msg)
{
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "Node " << getName() << " (ID: " << nodeId << ") decremented x to " << x << " at time " << simTime() << ".\n";

        // No need to cancel the event here since we're within its handling code, and it's automatically unscheduled
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Handle other messages
        if (!channelBusy && simTime() >= nodeId * slotDuration) {
            transmitMessage();
            EV << "Node " << nodeId << " transmitted a message.\n";
            bubble("Message Transmitted from Node32!");
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";
            bubble("Message Received at Node32!");

            backoff();
            // Reschedule the message for the next time slot
            scheduleAt(nodeId * slotDuration, msg);
        }

        // Check if decrementXMsg is already scheduled, cancel it before rescheduling
        if (decrementXMsg->isScheduled()) {
            cancelEvent(decrementXMsg);
        }
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    }
}

void node32::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("4");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node32!");
    send(msg1, "output_gate", gateIndex);
    cMessage *msg2 = new cMessage("2");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg2->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node32!");
    send(msg2, "output_gate", gateIndex);
    cMessage *msg3 = new cMessage("3");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg3->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node32!");
    send(msg3, "output_gate", gateIndex);
    cMessage *msg4 = new cMessage("0");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg4->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node32!");
    send(msg4, "output_gate", gateIndex);
    cMessage *msg5 = new cMessage("1");
    EV << "Node " << nodeId +1 << " transmitting message: " << msg5->getName() << " on gate " << gateIndex << "\n";
    bubble("Message Transmitted from Node32!");
    send(msg5, "output_gate", gateIndex);
}
void node32::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}







