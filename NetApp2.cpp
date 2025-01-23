/*
array [10, 1, 4, 3, 6]
find 2 numbers such that their difference is max
10 and 3
10->0
3->3

0 is i
3 is j

j > i

abs(a[j] - a[i])

{1}

(a[j] - a[i]) and j>i

{1,4} -> 3
{3,6} -> 3
total = 3 + 3
{1,6} -> 5

ans = 0;
[1,10,3,3,3,3,3,3]
freq[9] = 1
freq[2] = 6

6*2
over 9*1


cnt*diff
unordered_map<int,int> freq
[10, 1, 4, 3, 6, 100] -> 
freq[3]++;
freq[2]++;

freq[5]++;
freq[2]++;
freq[3]++;

iterate through freq
ans = max(ans, freq[diff]*diff);

{1, 3, 7, 15}

{1,3} -> 2
{7,15} -> 8

ans = 10

ans = 2
min = 3, used = true;
{7, 15}



*/