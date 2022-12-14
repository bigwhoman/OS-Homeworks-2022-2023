This program gets a number of pairs (m,n) and calculates the Ackerman function on the numbers.
**NOTE** : the rate of growth of the program is so big that when you calculate pairs with m >= 4, you will get segfault because of
recursive stack overflow (approximatly 19000 digits).
First pairs and then when finished giving the pairs, give -1 to either of inputs to start calculating the program gives each thread a task.
Build with build.sh


example input ( the -1 -1 means that the calculations would start) :
1 2
2 3
3 1
3 2
3 3
3 4
3 5
1 2
1 3
1 8
3 3
2 3
2 4
-1 -1
