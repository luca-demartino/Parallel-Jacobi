seed=10
K=100
trials=6
filename="fixed_dim"
debug=0
fixed_dim=1
max_nw=32

for n in 2000 5600 11000; do
  ./overhead ${trials} ${max_nw} ${n}
done

for n in 2000 5600 11000; do
  for mode in "seq" "std_par" "ff"; do
    if [ "$mode" = "seq" ]; then
      ./main ${n} ${seed} ${K} ${trials} ${mode} ${filename} ${debug} ${fixed_dim}
    fi

    if [ "$mode" = "std_par" ] || [ "$mode" = "ff" ]; then
      for((i = 1; i <= max_nw; i++)); do
        ./main ${n} ${seed} ${K} ${trials} ${mode} ${i} ${filename} ${debug} ${fixed_dim}
      done
    fi
  done
done