/*
 * my_simulation_network2.cc
 *
 *  Created on: Mar 8, 2024
 *      Author: pramita
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include <stdlib.h>

using namespace omnetpp;

#include "SimpleKalmanFilter.h"

#include "SimpleKalmanFilter.h"

class Hub_node1 : public cSimpleModule {
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

    // Kalman filter for Node_11 and Node_12 inputs
    SimpleKalmanFilter kf_node11;
    SimpleKalmanFilter kf_node12;

public:
    Hub_node1() : x(5.0), kf_node11(2.0, 2.0, 0.01), kf_node12(2.0, 2.0, 0.01) {} // Default constructor with x initialized to 5
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval
};

Define_Module(Hub_node1);

void Hub_node1::initialize() {
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

void Hub_node1::handleMessage(cMessage *msg)
{
    // Check if the message received is "Hello There!"
            if (strcmp(msg->getSenderModule()->getName(), "OBN") == 0) {
                EV << "Received 'Hello There!' message from the OBN. Starting message transmission.\n";
                // Start transmitting messages
                transmitMessage();
            } else {
                // Handle other messages here
                EV << "Node " << getName() << " received a message, but waiting for 'Hello There!' from the OBN.\n";
                // Store the received message and handle it later
                // Example: storeMessage(msg);
            }
    if (msg == decrementXMsg) {
        // Decrement x and reschedule the message
        x -= decrementAmount;
        EV << "Node " << getName() << " (ID: " << nodeId << ") decremented x to " << x << " at time " << simTime() << ".\n";
        scheduleAt(simTime() + decrementInterval, decrementXMsg);
    } else {
        // Check if the message is received from Node_11 or Node_12
        if (strcmp(msg->getSenderModule()->getName(), "Node_11") == 0 || strcmp(msg->getSenderModule()->getName(), "Node_12") == 0) {
            // Perform Kalman filtering on the input
            if (strcmp(msg->getSenderModule()->getName(), "Node_11") == 0) {
                // Filter input from Node_11
                double receivedValue = atof(msg->getName());
                double filteredValue = kf_node11.updateEstimate(receivedValue);
                double measurementError = kf_node11.getEstimateError();
                EV << "Received value from Node_11: " << receivedValue << ", Predicted value: " << filteredValue

                   << ", Measurement Error: " << measurementError << endl;
                bubble("Message Received from Node11!");
            } else {
                // Filter input from Node_12
                double receivedValue = atof(msg->getName());
                double filteredValue = kf_node12.updateEstimate(receivedValue);
                double measurementError = kf_node12.getEstimateError();
                EV << "Received value from Node_12: " << receivedValue << ", Predicted value: " << filteredValue

                   << ", Measurement Error: " << measurementError << endl;
                bubble("Message Received from Node12!");
            }

            // Forward the message to Obn_node
            send(msg, "output_gate", 2); // Assuming output_gate[2] is the gate connected to Obn_node
        } else {
            // Handle other messages here
            EV << "Received a message from " << msg->getSenderModule()->getName() << ", but not forwarding it to Obn_node.\n";
            // Handle the message according to your application logic
        }
    }
}
//        else {
//            x += 1.0; // This seems to be an increment operation, adjust according to your logic
//            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";

//            backoff();
            // Reschedule the message for the next time slot
//            scheduleAt(nodeId * slotDuration, msg);
//        }
               // Check if decrementXMsg is already scheduled, cancel it before rescheduling
//        if (decrementXMsg->isScheduled()) {
//            cancelEvent(decrementXMsg);
//        }
//        scheduleAt(simTime() + decrementInterval, decrementXMsg);

//}

void Hub_node1::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("Hello There!");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    send(msg1, "output_gate", gateIndex);
    // Check if this is the specific node to send the second message to
        const char* specificNodeName = "OBN"; // Change this to the specific node name you want to send "Back to You!" to

        // Retrieve the module pointer of the specific node
        cModule* specificNodeModule = getModuleByPath(specificNodeName);
        if (specificNodeModule) {
            // Send the message to the specific node
            cMessage *msg2 = new cMessage("Back to You!");
            EV << "Node " << nodeId << " transmitting message: " << msg2->getName() << " to specific node " << specificNodeName << "\n";
            send(msg2, "output_gate", gateIndex); // Assuming gate index is correct for sending to the specific node
        } else {
            EV << "Node " << nodeId << " unable to find specific node " << specificNodeName << ". Message not sent.\n";
        }
}

void Hub_node1::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}

class Hub_node2 : public cSimpleModule
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

        // Kalman filter for Node_21 and Node_22 inputs
        SimpleKalmanFilter kf_node21;
        SimpleKalmanFilter kf_node22;

  public:
     Hub_node2 () : x(5.0), kf_node21(2.0, 2.0, 0.01), kf_node22(2.0, 2.0, 0.01) {} // Default constructor with x initialized to 5
      cMessage *decrementXMsg = nullptr; // Pointer for the self-message
      double decrementInterval = 0.0005; // Interval in seconds to decrement x
      double decrementAmount = 0.3; // Amount to decrement x by each interval
};

Define_Module(Hub_node2);

void Hub_node2::initialize()
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


void Hub_node2::handleMessage(cMessage *msg)
{
    // Check if the message received is "Hello There!"
            if (strcmp(msg->getSenderModule()->getName(), "OBN") == 0) {
                EV << "Received 'Hello There!' message from the OBN. Starting message transmission.\n";
                // Start transmitting messages
                transmitMessage();
            } else {
                // Handle other messages here
                EV << "Node " << getName() << " received a message, but waiting for 'Hello There!' from the OBN.\n";
                // Store the received message and handle it later
                // Example: storeMessage(msg);
            }
    // Check if the message is received from Node_21 or Node_22
        if (strcmp(msg->getSenderModule()->getName(), "Node_21") == 0 || strcmp(msg->getSenderModule()->getName(), "Node_22") == 0) {
            // Perform Kalman filtering on the input
            if (strcmp(msg->getSenderModule()->getName(), "Node_21") == 0) {
                // Filter input from Node_21
                double receivedValue = atof(msg->getName());
                double filteredValue = kf_node21.updateEstimate(receivedValue);
                double measurementError = kf_node21.getEstimateError();
                EV << "Received value from Node_21: " << receivedValue << ", Predicted value: " << filteredValue

                   << ", Measurement Error: " << measurementError << endl;
                bubble("Message Received from Node21!");
            } else {
                // Filter input from Node_22
                double receivedValue = atof(msg->getName());
                double filteredValue = kf_node22.updateEstimate(receivedValue);
                double measurementError = kf_node22.getEstimateError();
                EV << "Received value from Node_22: " << receivedValue << ", Predicted value: " << filteredValue
                                           << ", Measurement Error: " << measurementError << endl;
                bubble("Message Received from Node22!");
            }

            // Forward the message to Obn_node
            send(msg, "output_gate", 2); // Assuming output_gate[2] is the gate connected to Obn_node
        } else {
            // Handle other messages here
            EV << "Received a message from " << msg->getSenderModule()->getName() << ", but not forwarding it to Obn_node.\n";
            // Handle the message according to your application logic
        }
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
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";

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

void Hub_node2::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("Hello There!");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    send(msg1, "output_gate", gateIndex);
    // Check if this is the specific node to send the second message to
            const char* specificNodeName = "OBN"; // Change this to the specific node name you want to send "Back to You!" to

            // Retrieve the module pointer of the specific node
            cModule* specificNodeModule = getModuleByPath(specificNodeName);
            if (specificNodeModule) {
                // Send the message to the specific node
                cMessage *msg2 = new cMessage("Back to You!");
                EV << "Node " << nodeId << " transmitting message: " << msg2->getName() << " to specific node " << specificNodeName << "\n";
                send(msg2, "output_gate", gateIndex); // Assuming gate index is correct for sending to the specific node
            } else {
                EV << "Node " << nodeId << " unable to find specific node " << specificNodeName << ". Message not sent.\n";
            }
}



void Hub_node2::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}
class Hub_node3 : public cSimpleModule
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

    // Kalman filter for Node_31 and Node_32 inputs
           SimpleKalmanFilter kf_node31;
           SimpleKalmanFilter kf_node32;

  public:
   Hub_node3() : x(5.0), kf_node31(0.01, 0.01, 0.01), kf_node32(0.5, 0.5, 0.01) {} // Default constructor with x initialized to 5
    cMessage *decrementXMsg = nullptr; // Pointer for the self-message
    double decrementInterval = 0.0005; // Interval in seconds to decrement x
    double decrementAmount = 0.3; // Amount to decrement x by each interval

};

Define_Module(Hub_node3);

void Hub_node3::initialize()
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


void Hub_node3::handleMessage(cMessage *msg)
{
    // Check if the message received is "Hello There!"
        if (strcmp(msg->getSenderModule()->getName(), "OBN") == 0) {
            EV << "Received 'Hello There!' message from the OBN. Starting message transmission.\n";
            // Start transmitting messages
            transmitMessage();
        } else {
            // Handle other messages here
            EV << "Node " << getName() << " received a message, but waiting for 'Hello There!' from the OBN.\n";
            // Store the received message and handle it later
            // Example: storeMessage(msg);
        }
    // Check if the message is received from Node_31 or Node_32
        if (strcmp(msg->getSenderModule()->getName(), "Node_31") == 0 || strcmp(msg->getSenderModule()->getName(), "Node_32") == 0) {
            // Perform Kalman filtering on the input
            if (strcmp(msg->getSenderModule()->getName(), "Node_31") == 0) {
                // Filter input from Node_31
                double receivedValue = atof(msg->getName());
                double filteredValue = kf_node31.updateEstimate(receivedValue);
                double measurementError = kf_node31.getEstimateError();
                EV << "Received value from Node_31: " << receivedValue << ", Predicted value: " << filteredValue
                   << ", Measurement Error: " << measurementError << endl;
                bubble("Message Received from Node31!");
            } else {
                // Filter input from Node_32
                double receivedValue = atof(msg->getName());
                double filteredValue = kf_node32.updateEstimate(receivedValue);
                double measurementError = kf_node32.getEstimateError();
                EV << "Received value from Node_32: " << receivedValue << ", Predicted value: " << filteredValue
                   << ", Measurement Error: " << measurementError << endl;
                bubble("Message Received from Node32!");
            }

            // Forward the message to Obn_node
            send(msg, "output_gate", 2); // Assuming output_gate[2] is the gate connected to Obn_node
        } else {
            // Handle other messages here
            EV << "Received a message from " << msg->getSenderModule()->getName() << ", but not forwarding it to Obn_node.\n";
            // Handle the message according to your application logic
        }
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
        } else {
            x += 1.0; // This seems to be an increment operation, adjust according to your logic
            EV << "Node " << nodeId << " received a message. Incrementing x to " << x << "\n";

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

void Hub_node3::transmitMessage()
{
    // Create and send the message
    cMessage *msg1 = new cMessage("Hello There!");
    int gateIndex = intuniform(0, gateSize("output_gate") - 1);
    EV << "Node " << nodeId << " transmitting message: " << msg1->getName() << " on gate " << gateIndex << "\n";
    send(msg1, "output_gate", gateIndex);
    // Check if this is the specific node to send the second message to
            const char* specificNodeName = "OBN"; // Change this to the specific node name you want to send "Back to You!" to

            // Retrieve the module pointer of the specific node
            cModule* specificNodeModule = getModuleByPath(specificNodeName);
            if (specificNodeModule) {
                // Send the message to the specific node
                cMessage *msg2 = new cMessage("Back to You!");
                EV << "Node " << nodeId << " transmitting message: " << msg2->getName() << " to specific node " << specificNodeName << "\n";
                send(msg2, "output_gate", gateIndex); // Assuming gate index is correct for sending to the specific node
            } else {
                EV << "Node " << nodeId << " unable to find specific node " << specificNodeName << ". Message not sent.\n";
            }
}



void Hub_node3::backoff()
{
    // Simple backoff mechanism (you can replace this with CSMA/CA or other algorithms)
    // For simplicity, just wait for a random time within a range
    double backoffTime = uniform(0, 0.1); // Adjust the range as needed
    EV << "Node " << nodeId << " backing off for " << backoffTime << "s\n";
    wait(backoffTime);
}
