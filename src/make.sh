


num_philosophers=5
time_to_die=310
time_to_eat=200
time_to_sleep=100
num_iterations=7

gcc -Wall -Werror -Wextra *.c -pthread -o philo && Leaks -atExit -- ./philo 2 100 200 200



: <<'end'
1 800 200 200 #a philo should die
4 310 200 100 #a philo should die
4 200 205 200 #a philo should die
5 800 200 200 7 #no one should die, simulation should stop after 7 eats
4 800 200 200 #no one should die

end