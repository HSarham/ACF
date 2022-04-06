import numpy as np
import sys

def read_text_file(path):
    s = ""
    the_file = open(path,"r")
    for line in the_file:
        s += line
    return s

def print_matched(s, s_inds):
    s_matched = ""
    prev_in = False
    for i in range(len(s)):
        if i in s_inds:
            if not prev_in:
                s_matched += "\u001b[0;31m"
                prev_in = True
        elif prev_in:
            s_matched += "\u001b[0;37m"
            prev_in = False

        s_matched += s[i]

    print(s_matched)

if len(sys.argv)<3:
    print("Args: <file_path_1> <file_path_2>")
    exit(-1)

file_path_1 = sys.argv[1]
file_path_2 = sys.argv[2]

s1 = read_text_file(file_path_1)
s2 = read_text_file(file_path_2)

lcs_array = np.zeros((len(s1),len(s2)))
dir_array = np.array([["EE"]*len(s2)]*len(s1))

for i in range(len(s1)):
    for j in range(len(s2)):
        if s1[i] == s2[j]:
            dir_array[i,j] = "UL"
            if i*j == 0:
                lcs_array[i,j] = 1
            else:
                lcs_array[i,j] = lcs_array[i-1,j-1] + 1
        elif lcs_array[i-1,j] > lcs_array[i,j-1]:
            dir_array[i,j] = "U"
            lcs_array[i,j] = lcs_array[i-1,j]
        else:
            dir_array[i,j] = "L"
            lcs_array[i,j] = lcs_array[i,j-1]

# print(lcs_array[-1,-1])
# print(lcs_array)
# print(dir_array)

## give one lcs

s1_inds = []
s2_inds = []

i = len(s1) - 1
j = len(s2) - 1

while i>=0 and j>=0:
    if dir_array[i,j] == "UL":
        s1_inds = [i] + s1_inds
        s2_inds = [j] + s2_inds
        i -= 1
        j -= 1
    elif dir_array[i,j] == "U":
        i -= 1
    else:
        j -=1

# print(s1_inds)
# print(s2_inds)

print("\u001b[0;33mfile 1: "+file_path_1+":\u001b[0;37m")
print_matched(s1,s1_inds)
print("\u001b[0;33mfile 2: "+file_path_2+":\u001b[0;37m")
print_matched(s2,s2_inds)
print(":\u001b[0;37m")


## dfs
# visited = np.zeros((len(s1),len(s2))) == 1
# stack = [(len(s1)-1, len(s2)-1)]
# while len(stack) > 0:
#     if visited[stack[-1]]:
#         stack.pop()
#     else:
#         (i,j) = stack(-1)
#         if



            
