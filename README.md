# Azra: An Assignment Cheater Finder

This is a simple python script that finds the longest common subsequence (LCS) between two text files and hightlights them on both texts.
You can run it on a real example of cheating using the text files submitted by two studens. To see the result for these given text files you can run:

`sh example.sh`

Look into the example.sh file to see how to use the acf.py script.

## Usage:
If run give only a directory, all the files in the directory are compared to each other and the comparisons are rankend based on the length of the LCS.

If you additionally give two file names in the directory their comparison will be shown in detail. I would suggest look for the areas in both texts where there are matched characters (with the red color) with little or no unmatched (white colored) characters. Those areas could be plagiarized. 
