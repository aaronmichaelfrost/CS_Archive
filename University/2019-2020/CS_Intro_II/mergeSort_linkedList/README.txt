Author: 
  Aaron Frost
  6/7/2020
  CS 162
  Assignment 5



Description: 
  This program is made to test some functionalities for a data structure called a linked list. You can imagine a linked list to be a chain that ties together many objects/nodes,
  one by one. A linked list is one of many ways to structure and organize your data. This program implements a single linked list, which means you can only travel/iterate from 
  item to item in one direction. If you aren't familiar with iteration, think of it as a way to access each element of data in an organized fashion. 
  
  This program tests a linked list that may only contain nodes that store an integer and a reference to the next node.
  
  On the surface level, this program figures solutions for the following:
    -inserting elements to a list at any position
    -calculating the amount of prime numbers the list contains
    -sorting the list by ascending order
    -sorting the list by descending order
    -printing the elements of the list
    -deleting the lists elements
    
    
    
Instructions: (By default, program runs through the test cases inside "test_linked_list.cpp"
  1) COMPILE: enter in the console "make run"
  2) RUN: enter in the console "./run"
  3) VIEW TEST CASES: press enter key
  
  4) CREATE YOUR OWN: open "test_linked_list.cpp
  
        *Create a list named l: Linked_List l;
        *Add the number x to the front: l.push_front(x);
        *-->To the back: l.push_back(x);
        *Insert x at the end: l.insert(x, l.get_length()); 
        *Insert x at index y: l.insert(x, y);  
        *View the list: l.print();
        *Sort it by ascending order: l.sort_ascending();
        *-->By descending order: l.sort_descending();
        *Get the legnth: l.get_length()
        *Get the amount of prime numbers: l.count_prime()
  
  
Limitations:
  -Make sure if using insert(x, y) that y is a valid index/not null, otherwise the number will not be added to the list
  -Only positive numbers are counted prime 
  -I could not figure out how to actually swap the pointers with merge sort
  
  
  
Complexity Analysis:
  Sort ascending and sort descending have the same complexity as they are both using the same merge sort function.
  
  That said, the merge sort function I implemented has a complexity of O(nlogn).
  
  The algorithim divides the indices into halves recursively, then merges back together the subparts in a sorted fashion.
  
  The count prime function has a complexity of O(n).
  