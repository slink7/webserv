min=20
max=30
ip="127.0.0.1"
port="8080"
for i in $(seq $min $max)
do
    printf '%d%*s' $i $i "" | telnet $ip $port
	# sleep 0.025
done
