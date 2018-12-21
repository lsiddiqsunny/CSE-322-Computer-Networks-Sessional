# simple-wireless.tcl
# A simple example for wireless simulation
# ======================================================================
# Define variables
# ======================================================================
set cbr_size 64 ; #[lindex $argv 2]; #4,8,16,32,64
set cbr_rate 11.0Mb
set x_dim 500 ; #[lindex $argv 1]
set y_dim 500 ; #[lindex $argv 1]
set time_duration 25 ; #[lindex $argv 5] ;#50
set start_time 10 ;#100
set extra_time 10
set flow_start_gap 0.0
set motion_start_gap 0.05
set factor [lindex $argv 4]
set num_node  [lindex $argv 0];# [expr 20*$factor]
set num_flow  [lindex $argv 1];# [expr 10*$factor]
set cbr_pckt_per_sec  [lindex $argv 2]; # [expr 100*$factor]
set Tx_Range     [lindex $argv 3];# [expr 5*$factor]
set cbr_interval [expr 1.0/$cbr_pckt_per_sec] ;# ?????? 1 for 1 packets per second and 0.1 for 10 packets per second
set num_motion [expr int($num_node*rand()/2)]
set tcp_src Agent/TCP
set tcp_sink Agent/TCPSink

# ======================================================================
# Define options
# ======================================================================
set val(chan)           Channel/WirelessChannel    ;# channel type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy            ;# network interface type
set val(mac)            Mac/802_11                 ;# MAC type
set val(ifq)            Queue/DropTail/PriQueue    ;# interface queue type
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                         ;# max packet in ifq
set val(nn)             $num_node                  ;# number of mobilenodes
set val(rp)             DSDV                       ;# routing protocol

# ======================================================================
# Energy Parameters
# ======================================================================
set val(energymodel_11)    EnergyModel     ;
set val(initialenergy_11)  1000            ;# Initial energy in Joules

set val(idlepower_11) 869.4e-3			;#LEAP (802.11g) 
set val(rxpower_11) 1560.6e-3			;#LEAP (802.11g)
set val(txpower_11) [expr $Tx_Range*1679.4e-3]	;#LEAP (802.11g)
set val(sleeppower_11) 37.8e-3			;#LEAP (802.11g)
set val(transitionpower_11) 176.695e-3		;#LEAP (802.11g)	??????????????????????????????/
set val(transitiontime_11) 2.36			;#LEAP (802.11g)

#
# Other Options
#
Mac/802_11 set dataRate_ 11Mb
Mac/802_11 set syncFlag_ 1
Mac/802_11 set dutyCycle_ cbr_interval
# ======================================================================
# Main Program
# ======================================================================

set tr 802_11_tcp.tr
set nm 802_11_tcp.nam
set topo_file topo_802_11_tcp_$factor.txt

#
# Initialize Global Variables
#
set ns_		[new Simulator]
set tracefd     [open $tr w]
set namtrace    [open $nm w]
$ns_ trace-all $tracefd
$ns_ namtrace-all-wireless $namtrace $x_dim $y_dim

# set up topography object
set topofile   [open $topo_file "w"]
set topo       [new Topography]

$topo load_flatgrid $x_dim $y_dim

#
# Create God
#
create-god $val(nn)

#
#  Create the specified number of mobilenodes [$val(nn)] and "attach" them
#  to the channel. 
#  Here two nodes are created : node(0) and node(1)

# configure node

        $ns_ node-config -adhocRouting $val(rp) \
			 -llType $val(ll) \
			 -macType $val(mac) \
			 -ifqType $val(ifq) \
			 -ifqLen $val(ifqlen) \
			 -antType $val(ant) \
			 -propType $val(prop) \
			 -phyType $val(netif) \
			 -channel [new $val(chan)] \
			 -topoInstance $topo \
			 -energyModel $val(energymodel_11) \
			 -idlePower $val(idlepower_11) \
			 -rxPower $val(rxpower_11) \
			 -txPower $val(txpower_11) \
          		 -sleepPower $val(sleeppower_11) \
          		 -transitionPower $val(transitionpower_11) \
			 -transitionTime $val(transitiontime_11) \
			 -initialEnergy $val(initialenergy_11)\
			 -agentTrace ON \
			 -routerTrace On \
			 -macTrace OFF \
			 -movementTrace OFF
			 #-channelType $val(chan) \			

# ======================================================================
# Node Creation
# ======================================================================
puts "start node creation"			 
for {set i 0} {$i < $val(nn) } {incr i} {
	set node_($i) [$ns_ node]	
	$node_($i) random-motion 0		;# disable random motion
#
# Provide initial (X,Y, for now Z=0) co-ordinates for mobilenodes
#
	set x_pos [expr int($x_dim*rand())] ;#random settings
	set y_pos [expr int($y_dim*rand())] ;#random settings
	$node_($i) set X_ $x_pos;
	$node_($i) set Y_ $y_pos;
	$node_($i) set Z_ 0.0
	puts -nonewline $topofile "$i x: [$node_($i) set X_] y: [$node_($i) set Y_] \n"
}
#puts "RANDOM topology"
puts "node creation complete"

for {set i 0} {$i < $val(nn)} { incr i } {
	$ns_ initial_node_pos $node_($i) 4
}


# ======================================================================
# Flow Creation
# ======================================================================
puts "num_flows is set $num_flow"
# Setup traffic flow between nodes
for {set i 0} {$i < $num_flow} {incr i} { ;#sink
	set udp_($i) [new $tcp_src]
	$udp_($i) set class_ $i
	set null_($i) [new $tcp_sink]
	$udp_($i) set fid_ $i
	if { [expr $i%2] == 0} {
		$ns_ color $i Blue
	} else {
		$ns_ color $i Red
	}
} 

#
# RANDOM FLOW
#

# Creating udp_node & null_node
for {set i 0} {$i < $num_flow} {incr i} {
	set udp_node [expr int($num_node*rand())] ;# src node
	set null_node $udp_node
	while {$null_node==$udp_node} {
		set null_node [expr int($num_node*rand())] ;# dest node
	}
	$ns_ attach-agent $node_($udp_node) $udp_($i)
  	$ns_ attach-agent $node_($null_node) $null_($i)
	puts -nonewline $topofile "RANDOM:  Src: $udp_node Dest: $null_node\n"
}

# Connecting udp_node & null_node
for {set i 0} {$i < $num_flow } {incr i} {
     $ns_ connect $udp_($i) $null_($i)
}
# Creating packet generator (CBR) for source node
for {set i 0} {$i < $num_flow } {incr i} {
	set cbr_($i) [new Application/Traffic/CBR]
	$cbr_($i) set packetSize_ $cbr_size
	$cbr_($i) set rate_ $cbr_rate
	$cbr_($i) set interval_ $cbr_interval
	$cbr_($i) attach-agent $udp_($i)
}  

# Declaring packet generation time
for {set i 0} {$i < $num_flow } {incr i} {
     $ns_ at [expr $start_time+$i*$flow_start_gap] "$cbr_($i) start"
}
puts "no of packets per second $cbr_pckt_per_sec"

puts "flow creation complete"

# ======================================================================
# Ending the simulation
# ======================================================================
#
# Tell nodes when the simulation ends
#
for {set i 0} {$i < $val(nn) } {incr i} {
    $ns_ at [expr $start_time+$time_duration] "$node_($i) reset";
}
$ns_ at [expr $start_time+$time_duration +$extra_time] "finish"
#$ns_ at 150.01 "puts \"NS EXITING...\" ; $ns_ halt"
$ns_ at [expr $start_time+$time_duration +$extra_time] "$ns_ nam-end-wireless [$ns_ now]; puts \"NS Exiting...\"; $ns_ halt"

$ns_ at [expr $start_time+$time_duration/2] "puts \"half of the simulation is finished\""
$ns_ at [expr $start_time+$time_duration] "puts \"end of simulation duration\""
proc finish {} {
    puts "finishing"
    global ns_ tracefd namtrace topofile nm
    $ns_ flush-trace
    close $tracefd
    close $namtrace
    close $topofile
    exec nam 802_11_tcp.nam &
    exit 0
}

puts "Starting Simulation..."
$ns_ run
