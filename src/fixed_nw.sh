max_n=16000
seed=10
K=100
trials=6
filename="fixed_nw"
debug=0
fixed_dim=0
nw=22

for((n = 2000; n <= max_n; n+=800)); do
  for mode in "seq" "std_par" "ff"; do
    if [ "$mode" = "seq" ]; then
      ./main ${n} ${seed} ${K} ${trials} ${mode} ${filename} ${debug} ${fixed_dim}
    fi

    if [ "$mode" = "std_par" ] || [ "$mode" = "ff" ]; then
      ./main ${n} ${seed} ${K} ${trials} ${mode} ${nw} ${filename} ${debug} ${fixed_dim}
    fi
  done
done
