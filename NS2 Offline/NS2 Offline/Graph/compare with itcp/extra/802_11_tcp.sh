#INPUT: output file AND number of iterations
output_file_format="802_11_tcp";
under="_";





#iteration=$(printf %.0f $iteration_float);
read iteration
iteration_float=$iteration
start=$iteration
end=$iteration
r=$start

output_file="$output_file_format.out"
while [ $r -le $end ]
do
echo "total iteration: $iteration"
###############################START A ROUND
l=0;thr=0.0;del=0.0;s_packet=0.0;r_packet=0.0;d_packet=0.0;del_ratio=0.0;
dr_ratio=0.0;time=0.0;t_energy=0.0;
#energy_bit=0.0;energy_byte=0.0;energy_packet=0.0;total_retransmit=0.0;
i=0
while [ $i -lt $iteration ]
do
#################START AN ITERATION
echo "                             EXECUTING $(($i+1)) th ITERATION"
node=$((10*($i+1)))
flow=$node
Tx_Range=5
packet=$node
echo -ne "$node" >> $output_file
ns 802_11_tcp.tcl $node $flow $packet $Tx_Range $(($i+1))
echo "SIMULATION COMPLETE. BUILDING STAT......"
awk -f 802_11_tcp.awk 802_11_tcp.tr > "$output_file_format$under$i.out"
while read val
do
	l=$(($l+1))

	
	
	if [ "$l" == "1" ]; then
		thr=$(echo "scale=5; $thr+$val/$iteration_float" | bc)
		echo -ne " $val\n" >> $output_file
	elif [ "$l" == "2" ]; then
		del=$(echo "scale=5; $del+$val/$iteration_float" | bc)
		#echo -ne "delay: $val " >> $output_file
	elif [ "$l" == "3" ]; then
		s_packet=$(echo "scale=5; $s_packet+$val/$iteration_float" | bc)
		#echo -ne "send packet: $val " >> $output_file
	elif [ "$l" == "4" ]; then
		r_packet=$(echo "scale=5; $r_packet+$val/$iteration_float" | bc)
		#echo -ne "received packet: $val " >> $output_file
	elif [ "$l" == "5" ]; then
		d_packet=$(echo "scale=5; $d_packet+$val/$iteration_float" | bc)
		#echo -ne "drop packet: $val " >> $output_file
	elif [ "$l" == "6" ]; then
		del_ratio=$(echo "scale=5; $del_ratio+$val/$iteration_float" | bc)
		#echo -ne "delivery ratio: $val " >> $output_file
	elif [ "$l" == "7" ]; then
		dr_ratio=$(echo "scale=5; $dr_ratio+$val/$iteration_float" | bc)
		#echo -ne "drop ratio: $val " >> $output_file
	elif [ "$l" == "8" ]; then
		time=$(echo "scale=5; $time+$val/$iteration_float" | bc)
		#echo -ne "time: $val " >> $output_file
	elif [ "$l" == "9" ]; then
		t_energy=$(echo "scale=5; $t_energy+$val/$iteration_float" | bc)
		#echo -ne "total_energy: $val \n" >> $output_file
	fi

	echo "$val"
done < "$output_file_format$under$i.out"

i=$(($i+1))
l=0
#################END AN ITERATION
done

r=$(($r+1))
#######################################END A ROUND
done

exec xgraph $output_file
