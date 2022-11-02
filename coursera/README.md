
## 2-sum problem
This is a variant of 2-sum problem: find all t in [-10000, 10000] such that x+y=t for some x,y in S.
### Algorithm
1. solution to 2-sum problem  
  1-1 for each t and x in S, search t-y in S.
  
    Time Complexity: O(n) * 20000 = 20 billion

  
2. Use Binary search  
	2-1 get sorted set xs  
	2-2 for each x in xs, find interval I of xs such that -10000-x <= y <=10000-x. (binary search)  
  2-3 Insert x+xs[I] elements in a hash table.  
  2-4 repeat 2, 3 for every elements in xs
	
	Time Complexity: O(nlogn) for n=|xs|
  
### Performance
We implement hash table with both chaining and open addressing method to solve collisions.  

Reading file...: 6.481s  
Sorting arrays...: 0.683s  

Performance - std::set  
Binary Searching...: 2.125s  
Number of target values t: 427  

Hash Performance - Chaining Method  
Binary Searching...: 2.127s  
Number of target values t: 427  

Hash Performance - OpenAddressing Method  
Binary Searching...: 2.111s  
Number of target values t: 427  
