
#INPUT: output file AND number of iterations
output_file_format="802_11_tcp";
under="_";
iteration_float=2.0;




#iteration=$(printf %.0f $iteration_float);
read iteration

start=$iteration
end=$iteration
r=$start
while [ $r -le $end ]
do
echo "total iteration: $iteration"
###############################START A ROUND
l=0;thr=0.0;del=0.0;s_packet=0.0;r_packet=0.0;d_packet=0.0;del_ratio=0.0;
dr_ratio=0.0;time=0.0;t_energy=0.0;
#energy_bit=0.0;energy_byte=0.0;energy_packet=0.0;total_retransmit=0.0;energy_efficiency=0.0;

i=0
while [ $i -lt $iteration ]
do
#################START AN ITERATION
echo "                             EXECUTING $(($i+1)) th ITERATION"
read node
read flow
read speed
read packet
ns 802_11_tcp.tcl $node $flow $speed $node $(($i+1))
echo "SIMULATION COMPLETE. BUILDING STAT......"


awk -f 802_11_tcp.awk 802_11_tcp.tr > "$output_file_format$under$i.out"

ok=1;
while read val
do
#	l=$(($l+$inc))
	l=$(($l+1))


	if [ "$l" == "1" ]; then
		thr=$(echo "scale=5; $thr+$val/$iteration_float" | bc)
#		echo -ne "throughput: $thr "
	elif [ "$l" == "2" ]; then
		del=$(echo "scale=5; $del+$val/$iteration_float" | bc)
#		echo -ne "delay: "
	elif [ "$l" == "3" ]; then
		s_packet=$(echo "scale=5; $s_packet+$val/$iteration_float" | bc)
#		echo -ne "send packet: "
	elif [ "$l" == "4" ]; then
		r_packet=$(echo "scale=5; $r_packet+$val/$iteration_float" | bc)
#		echo -ne "received packet: "
	elif [ "$l" == "5" ]; then
		d_packet=$(echo "scale=5; $d_packet+$val/$iteration_float" | bc)
#		echo -ne "drop packet: "
	elif [ "$l" == "6" ]; then
		del_ratio=$(echo "scale=5; $del_ratio+$val/$iteration_float" | bc)
#		echo -ne "delivery ratio: "
	elif [ "$l" == "7" ]; then
		dr_ratio=$(echo "scale=5; $dr_ratio+$val/$iteration_float" | bc)
#		echo -ne "drop ratio: "
	elif [ "$l" == "8" ]; then
		time=$(echo "scale=5; $time+$val/$iteration_float" | bc)
#		echo -ne "time: "
	elif [ "$l" == "9" ]; then
		t_energy=$(echo "scale=5; $t_energy+$val/$iteration_float" | bc)
#		echo -ne "total_energy: "
	fi


	echo "$val"

done < "$output_file_format$under$i.out"

i=$(($i+1))
l=0
#################END AN ITERATION
done


output_file="$output_file_format$under$r.out"

echo -ne "\nThroughput:          $thr \n" >> $output_file
echo -ne "AverageDelay:         $del \n" >> $output_file
echo -ne "Sent Packets:         $s_packet \n" >> $output_file
echo -ne "Received Packets:         $r_packet \n" >> $output_file
echo -ne "Dropped Packets:         $d_packet \n" >> $output_file
echo -ne "PacketDeliveryRatio:      $del_ratio \n" >> $output_file
echo -ne "PacketDropRatio:      $dr_ratio \n" >> $output_file
echo -ne "Total time:  $time \n" >> $output_file
echo -ne "" >> $output_file
echo -ne "" >> $output_file
echo -ne "Total energy consumption:        $t_energy \n" >> $output_file
#echo -ne "Average Energy per bit:         $energy_bit " >> $output_file
#echo -ne "Average Energy per byte:         $energy_byte " >> $output_file
#echo -ne "Average energy per packet:         $energy_packet " >> $output_file
#echo -ne "total_retransmit:         $total_retransmit " >> $output_file
#echo -ne "energy_efficiency(nj/bit):         $enr_nj " >> $output_file
#echo "" >> $output_file

r=$(($r+1))
#######################################END A ROUND
done
