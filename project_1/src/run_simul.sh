N_mc="17";
for N_d in "1" "2" "3"
do
for N_p in "100"
do
    echo "simulating with Nd: $N_d and N_p: $N_p and Nmc: $N_mc" 
    ./app_c.x $N_p $N_d $N_mc 
done
done
