#include "headers.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("Experiment7");
int 
main (int argc, char *argv[])
{
Address serverAddress[4];
  uint32_t np=1;
  uint32_t ps=1024;
  double Interval=1;
  uint32_t dr=16;
 CommandLine cmd;
 cmd.AddValue("np","Number of Packets",np);
  cmd.AddValue("ps","Packet Size",ps);
 cmd.AddValue("Interval","Interval",Interval);
 cmd.AddValue("dr","Data Rate",dr);
 cmd.Parse(argc,argv);
Time interPacketInterval=Seconds(Interval);
 
 LogComponentEnable("UdpEchoClientApplication",LOG_LEVEL_INFO);
 LogComponentEnable("UdpEchoServerApplication",LOG_LEVEL_INFO);


  NS_LOG_INFO ("Create nodes.");
  NodeContainer nodes;
  nodes.Create (8);
  InternetStackHelper internet;
  internet.Install (nodes);
  NS_LOG_INFO ("Create channels.");

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue(DataRate(dr*1000)));
  csma.SetChannelAttribute ("Delay", TimeValue(MilliSeconds(2)));
  csma.SetDeviceAttribute ("Mtu", UintegerValue (1400));
  NetDeviceContainer d = csma.Install (nodes);

int addr_server[4]={4,5,2,3};

  NS_LOG_INFO ("Assign IP Addresses.");
      Ipv4AddressHelper ipv4;
      ipv4.SetBase ("10.1.1.0", "255.255.255.0");
      Ipv4InterfaceContainer i = ipv4.Assign (d);

     for(int k=0;k<4;k++)
{
 serverAddress[k]=Address(i.GetAddress(addr_server[k]));
}
  
  NS_LOG_INFO ("Create Applications.");

  ApplicationContainer apps[4];
  uint16_t port = 9; 
 for(int i=0;i<4;i++)
{
  UdpEchoServerHelper server(port);
  apps[i] = server.Install (nodes.Get(addr_server[i]));
  apps[i].Start (Seconds (1.0));
  apps[i].Stop (Seconds (60000.0));
}

int addr_client[4]={0,1,6,7};
for(int i=0;i<4;i++)
{

UdpEchoClientHelper client(serverAddress[i], port);
  client.SetAttribute ("MaxPackets", UintegerValue (np));
  client.SetAttribute ("Interval", TimeValue(interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (ps));
 apps[i] = client.Install (nodes.Get (addr_client[i]));
  apps[i].Start (Seconds (2.0));
  apps[i].Stop (Seconds (60000.0));
}

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  NS_LOG_INFO ("Run Simulation.");
  Simulator::Stop (Seconds(60001.0));
  Simulator::Run ();

  monitor->CheckForLostPackets ();
double avtp=0,avfd=0,avjt=0;

  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i)
    {           
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first);
          double txp = i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds())/1024/1024;
          double fd=i->second.delaySum.GetSeconds();
          double jt=i->second.jitterSum.GetSeconds();
          std::cout << "Flow " << i->first  << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
          std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
          std::cout<<"Throughput: "<<txp<< "Mbps\n";
          std::cout<< "Forwarding Delay: "<<fd<< "sec\n";
          std::cout<<"Jitter Sum: "<<jt<< "sec\n";
          avtp+=txp;avfd+=fd;avjt+=jt;
    }
      	  
     std::cout << "Average Throughput = "<< avtp/8 << "Mbps\n";
     std::cout << "Average Forwarding Delay = "<< avfd/8 << "Mbps\n";
     std::cout << "Average Jitter = "<<avjt/8 << "sec\n";
 
 // monitor->SerializeToXmlFile("csma-udp.flowmon", true, true);
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");
}
