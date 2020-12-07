#include "headers.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int main (int argc, char *argv[])
{

  NodeContainer nodes;
  nodes.Create (3);
  NodeContainer c1 = NodeContainer (nodes.Get (0), nodes.Get (1));
  NodeContainer c2 = NodeContainer (nodes.Get (1), nodes.Get (2));

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("50Mbps"));
  csma.SetChannelAttribute ("Delay", StringValue ("1ms"));

  NetDeviceContainer devices1;
  devices1 = csma.Install (c1);
  NetDeviceContainer devices2;
  devices2 = csma.Install (c2);
  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces1 = address.Assign (devices1);
  address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces2 = address.Assign (devices2);

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  NS_LOG_INFO ("Create Applications.");

// Create a packet sink to receive these packets
  PacketSinkHelper sink ("ns3::UdpSocketFactory", Address (InetSocketAddress (interfaces1.GetAddress (0), 9)));
  ApplicationContainer apps = sink.Install (nodes.Get (2));
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (interfaces2.GetAddress (1), 9)));
  onoff.SetAttribute ("DataRate", StringValue ("1Mb/s")); //bit/s
  apps = onoff.Install (nodes.Get (0));

  ns3::int64x64_t total_throughput;
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  AsciiTraceHelper ascii;
  csma.EnableAsciiAll (ascii.CreateFileStream ("1.tr"));
  csma.EnablePcapAll ("1", false);

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds(2.0));
  Simulator::Run ();

  monitor->CheckForLostPackets ();

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {           
          total_throughput = total_throughput + i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024;
    }
        
     std::cout << "Average Throughput = "<< total_throughput/3 << "\n";
 
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}

