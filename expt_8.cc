#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include <fstream>
#include "ns3/csma-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  NodeContainer nodes;
  nodes.Create (8);

  CsmaHelper pointToPoint;
  pointToPoint.SetChannelAttribute ("DataRate", StringValue ("50Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);
  InternetStackHelper stack;
  stack.Install (nodes);
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer interfaces = address.Assign (devices);

//Flow

  Address localAddress (InetSocketAddress (Ipv4Address::GetAny (), 9));
  PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", localAddress);
  ApplicationContainer server = packetSinkHelper.Install (nodes.Get (1));
  server.Start (Seconds (1.0));
  server.Stop (Seconds (3));

  OnOffHelper onoff ("ns3::TcpSocketFactory", Ipv4Address::GetAny ());
  onoff.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.2]"));
  onoff.SetAttribute ("PacketSize", UintegerValue (1024));
  onoff.SetAttribute ("DataRate", StringValue ("20Mb/s")); //bit/s
  ApplicationContainer client;

  InetSocketAddress rmt (interfaces.GetAddress (1), 9);
  AddressValue remoteAddress (rmt);
  onoff.SetAttribute ("Remote", remoteAddress);
  client.Add (onoff.Install (nodes.Get (0)));
  client.Start (Seconds (1.0));
  client.Stop (Seconds (3.0));



//Flow2

  PacketSinkHelper sink ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address::GetAny (), 9)));
  ApplicationContainer server1 = sink.Install (nodes.Get (3));
  server1.Start (Seconds (5));
  server1.Stop (Seconds (7));

  OnOffHelper onoff1 ("ns3::UdpSocketFactory", Ipv4Address::GetAny ());
  onoff1.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff1.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.2]"));
  onoff1.SetAttribute ("PacketSize", UintegerValue (1024));
  onoff1.SetAttribute ("DataRate", StringValue ("20Mb/s")); //bit/s
  ApplicationContainer client1;

  InetSocketAddress rmt1 (interfaces.GetAddress (3), 9);
  AddressValue remoteAddress1 (rmt1);
  onoff1.SetAttribute ("Remote", remoteAddress1);
  client1.Add (onoff1.Install (nodes.Get (2)));
  client1.Start (Seconds (5.0));
  client1.Stop (Seconds (7.0));



//Flow3

  Address localAddress2 (InetSocketAddress (Ipv4Address::GetAny (), 9));
  PacketSinkHelper packetSinkHelper2 ("ns3::TcpSocketFactory", localAddress2);  
  ApplicationContainer server2 = packetSinkHelper2.Install (nodes.Get (5));
  server2.Start (Seconds (11.0));
  server2.Stop (Seconds (13));

  OnOffHelper onoff2 ("ns3::TcpSocketFactory", Ipv4Address::GetAny ());
  onoff2.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff2.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.2]"));
  onoff2.SetAttribute ("PacketSize", UintegerValue (1024));
  onoff2.SetAttribute ("DataRate", StringValue ("20Mb/s")); //bit/s
  ApplicationContainer client2;

  InetSocketAddress rmt2 (interfaces.GetAddress (5), 9);
  AddressValue remoteAddress2 (rmt2);
  onoff2.SetAttribute ("Remote", remoteAddress2);
  client2.Add (onoff2.Install (nodes.Get (4)));
  client2.Start (Seconds (11.0));
  client2.Stop (Seconds (13));


//Flow4

  Address localAddress3 (InetSocketAddress (Ipv4Address::GetAny (), 9));
  PacketSinkHelper packetSinkHelper3 ("ns3::UdpSocketFactory", localAddress3);  
  ApplicationContainer server3 = packetSinkHelper3.Install (nodes.Get (7));
  server3.Start (Seconds (11.0));
  server3.Stop (Seconds (13));

  OnOffHelper onoff3 ("ns3::UdpSocketFactory", Ipv4Address::GetAny ());
  onoff3.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onoff3.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0.2]"));
  onoff3.SetAttribute ("PacketSize", UintegerValue (1024));
  onoff3.SetAttribute ("DataRate", StringValue ("20Mb/s")); //bit/s
  ApplicationContainer client3;

  InetSocketAddress rmt3 (interfaces.GetAddress (7), 9);
  AddressValue remoteAddress3 (rmt3);
  onoff3.SetAttribute ("Remote", remoteAddress3);
  client3.Add (onoff3.Install (nodes.Get (6)));
  client3.Start (Seconds (11.0));
  client3.Stop (Seconds (13));


  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("1.tr"));
 // pointToPoint.EnablePcapAll ("1", false);

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds(50.0));

  Simulator::Run ();

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {
	  Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
      if ((t.sourceAddress=="10.1.1.1" && t.destinationAddress == "10.1.1.2")||(t.sourceAddress=="10.1.1.2" && t.destinationAddress == "10.1.1.1")||(t.sourceAddress=="10.1.1.3" && t.destinationAddress == "10.1.1.4")||(t.sourceAddress=="10.1.1.4" && t.destinationAddress == "10.1.1.3")||(t.sourceAddress=="10.1.1.5" && t.destinationAddress == "10.1.1.6")||(t.sourceAddress=="10.1.1.6" && t.destinationAddress == "10.1.1.5")||(t.sourceAddress=="10.1.1.7" && t.destinationAddress == "10.1.1.8")||(t.sourceAddress=="10.1.1.8" && t.destinationAddress == "10.1.1.7"))
      {
          std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
      	  std::cout << "  Throughput: " << i->second.rxBytes * 8.0/ (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024  << " Mbps\n";
      }
     }
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
  return 0;
}

