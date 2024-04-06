package my_simulation.simulations;
package inet.physicallayer.wireless.common.pathloss;
package inet.physicallayer.wireless.common.pathloss;

import inet.physicallayer.wireless.common.contract.packetlevel.IPathLoss;

network My_simulation_network
{
    @display("bgb=735,470");
    types:
        simple OBN_node
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple Hub_node1
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple Hub_node2
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple Hub_node3
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple node11
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        
		simple node12
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple node21
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple node22
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple node31
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        simple node32
        {
            parameters:
                double timeSlot @unit(s); // Time slot duration
                double initialX; // Add this line
                //int initialX = default(5)
            gates:
                input input_gate[];
                output output_gate[];
        }
        // Define the Rayleigh channel module
        channel RayleighChannel extends ned.DatarateChannel
        {
            parameters:
                double alpha @unit(dB); // Path loss exponent for Rayleigh fading
                double systemLoss @unit(dB) = default(0dB); // System loss
                double distance @unit(cm); // Custom parameter for distance
        }



    submodules:
        OBN: OBN_node {
            //parameters:
            //initialX = 5;
            initialX = 1000; // Example setting for OBN
            @display("p=31,214");
        }
        Hub_1: Hub_node1 {
            initialX = 1000; // Example setting for Hub_1
            @display("p=166,98");
        }
        Hub_2: Hub_node2 {initialX = 1000; // Example setting for Hub_1
            @display("p=166,226");
        }
        Hub_3: Hub_node3 {initialX = 1000; // Example setting for Hub_1
            @display("p=166,331");
        }
        Node_11: node11 {initialX = 1000; // Example setting for Hub_1
            @display("p=317,47");
        }
        Node_12: node12 {initialX = 1000; // Example setting for Hub_1
            @display("p=317,119");
        }
        Node_21: node21 {initialX = 1000; // Example setting for Hub_1
            @display("p=314,197");
        }
        Node_22: node22 {initialX = 1000; // Example setting for Hub_1
            @display("p=314,265");
        }
        Node_31: node31 {initialX = 1000; // Example setting for Hub_1
            @display("p=314,331");
        }
        Node_32: node32 {initialX = 1000; // Example setting for Hub_1
            @display("p=314,395");
        }

    connections:
         // Connect nodes through the Rayleigh channel
        
         //Sending messages from OBN to Hubs
// 1) OBN -> Hub_1 AND Hub_2 AND Hub_3
OBN.output_gate++ --> RayleighChannel { delay = 10ms; distance = 100cm; } --> Hub_1.input_gate++;
OBN.output_gate++ --> RayleighChannel { delay = 10ms; distance = 90cm; } --> Hub_2.input_gate++;
OBN.output_gate++ --> RayleighChannel { delay = 10ms; distance = 80cm; } --> Hub_3.input_gate++;

//Forwarding from Hubs to Nodes
// 2) Hub_1 -> Node11 AND Node12
Hub_1.output_gate++ --> RayleighChannel { delay = 10ms; distance = 50cm; } --> Node_11.input_gate++;
Hub_1.output_gate++ --> RayleighChannel { delay = 10ms; distance = 40cm; } --> Node_12.input_gate++;

// 2) Hub_2 -> Node21 AND Node22
Hub_2.output_gate++ --> RayleighChannel { delay = 10ms; distance = 70cm; } --> Node_21.input_gate++;
Hub_2.output_gate++ --> RayleighChannel { delay = 10ms; distance = 60cm; } --> Node_22.input_gate++;

// 2) Hub_3 -> Node31 AND Node32
Hub_3.output_gate++ --> RayleighChannel { delay = 10ms; distance = 110cm; } --> Node_31.input_gate++;
Hub_3.output_gate++ --> RayleighChannel { delay = 10ms; distance = 120cm; } --> Node_32.input_gate++;

// Receiving messages from Nodes to Hubs
// 3) Node11 AND Node12 -> Hub_1
Node_11.output_gate++ --> RayleighChannel { delay = 10ms; distance = 80cm; } --> Hub_1.input_gate++;
Node_12.output_gate++ --> RayleighChannel { delay = 10ms; distance = 90cm; } --> Hub_1.input_gate++;

// 3) Node21 AND Node22 -> Hub_2
Node_21.output_gate++ --> RayleighChannel { delay = 10ms; distance = 60cm; } --> Hub_2.input_gate++;
Node_22.output_gate++ --> RayleighChannel { delay = 10ms; distance = 50cm; } --> Hub_2.input_gate++;

// 3) Node31 AND Node32 -> Hub_3
Node_31.output_gate++ --> RayleighChannel { delay = 10ms; distance = 40cm; } --> Hub_3.input_gate++;
Node_32.output_gate++ --> RayleighChannel { delay = 10ms; distance = 30cm; } --> Hub_3.input_gate++;

//Forwarding from Hubs to OBN
// 4) Hub_1 AND Hub_2 AND Hub_3 -> OBN
Hub_1.output_gate++ --> RayleighChannel { delay = 10ms; distance = 100cm; } --> OBN.input_gate++;
Hub_2.output_gate++ --> RayleighChannel { delay = 10ms; distance = 90cm; } --> OBN.input_gate++;
Hub_3.output_gate++ --> RayleighChannel { delay = 10ms; distance = 80cm; } --> OBN.input_gate++;
         
}